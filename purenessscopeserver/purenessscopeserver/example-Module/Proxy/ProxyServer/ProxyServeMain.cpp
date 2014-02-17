// ACE_ReactorServer.cpp : 定义控制台应用程序的入口点。
//
#include <ace/OS_main.h>
#include <ace/Reactor.h>
#include <ace/SOCK_Acceptor.h> 
#include <ace/Auto_Ptr.h>
#include <ace/OS_NS_stdio.h>

class ClientService : public ACE_Event_Handler
{
public:
	ACE_SOCK_Stream &peer (void) { return this->sock_; }

	int open (void)
	{
		ACE_INET_Addr ClientaddrRemote;
		this->peer().get_remote_addr(ClientaddrRemote);
		ACE_OS::printf("[CConnectHandler::open] Connection from [%s:%d]\n", ClientaddrRemote.get_host_addr(),  ClientaddrRemote.get_port_number());

		//注册读就绪回调函数
		return this->reactor ()->register_handler(this, ACE_Event_Handler::READ_MASK);
	}

	virtual ACE_HANDLE get_handle (void) const { return this->sock_.get_handle (); }

	virtual int handle_input (ACE_HANDLE fd )
	{
		//一个简单的EchoServer，将客户端的信息返回
		int rev = peer().recv(buf, 4096);
		if(rev<=0)
			return -1;
 
		ssize_t nSzie = peer().send(buf,rev);
		return 0;
	}

	// 释放相应资源
	virtual int handle_close (ACE_HANDLE, ACE_Reactor_Mask mask)
	{
		ACE_INET_Addr ClientaddrRemote;
		this->peer().get_remote_addr(ClientaddrRemote);
		ACE_OS::printf("[CConnectHandler::open] Close from [%s:%d]\n", ClientaddrRemote.get_host_addr(),  ClientaddrRemote.get_port_number());


		if (mask == ACE_Event_Handler::WRITE_MASK)
			return 0;
		mask = ACE_Event_Handler::ALL_EVENTS_MASK |
			ACE_Event_Handler::DONT_CALL;
		this->reactor ()->remove_handler (this, mask);
		this->sock_.close ();
		delete this;    //socket出错时，将自动删除该客户端，释放相应资源
		return 0;
	}

protected:
	char buf[100];
	ACE_SOCK_Stream sock_;
};

class ClientAcceptor : public ACE_Event_Handler
{
public:
	virtual ~ClientAcceptor (){this->handle_close (ACE_INVALID_HANDLE, 0);}

	int open (const ACE_INET_Addr &listen_addr)
	{
		if (this->acceptor_.open (listen_addr, 1) == -1)
		{
			ACE_OS::printf("open port fail");
			return -1;
		}
		//注册接受连接回调事件
		return this->reactor ()->register_handler(this, ACE_Event_Handler::ACCEPT_MASK);
	}

	virtual ACE_HANDLE get_handle (void) const
	{ return this->acceptor_.get_handle (); }

	virtual int handle_input (ACE_HANDLE fd )
	{
		ClientService *client = new ClientService();
		auto_ptr<ClientService> p (client);

		if (this->acceptor_.accept (client->peer ()) == -1)
		{
			ACE_OS::printf("accept client fail");
			return -1;
		}
		p.release ();
		client->reactor (this->reactor ());
		if (client->open () == -1)
			client->handle_close (ACE_INVALID_HANDLE, 0);
		return 0;
	}

	virtual int handle_close (ACE_HANDLE handle,
		ACE_Reactor_Mask close_mask)
	{
		if (this->acceptor_.get_handle () != ACE_INVALID_HANDLE)
		{
			ACE_Reactor_Mask m = ACE_Event_Handler::ACCEPT_MASK |
				ACE_Event_Handler::DONT_CALL;
			this->reactor ()->remove_handler (this, m);
			this->acceptor_.close ();
		}
		return 0;
	}

protected:
	ACE_SOCK_Acceptor acceptor_;
};


int main(int argc, char* argv[])
{
	ACE_INET_Addr addr(10040, "127.0.0.1");
	ClientAcceptor server;
	server.reactor(ACE_Reactor::instance());
	server.open(addr);

	ACE_OS::printf("Listen is success.\n");

	while(true)
	{
		ACE_Reactor::instance()->handle_events(); 
	}

	return 0;
}

