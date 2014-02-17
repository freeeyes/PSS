#pragma once
#include <ace/Asynch_IO.h>
#include <ace/Proactor.h>

class Fast_Asynch_Read_Stream : public ACE_Asynch_Read_Stream
{
public:
	Fast_Asynch_Read_Stream (void)
	{
	}

	/// Destructor.
	virtual ~Fast_Asynch_Read_Stream (void)
	{
	}

	int open (ACE_Handler &handler,
		ACE_HANDLE handle = ACE_INVALID_HANDLE,
		const void *completion_key = 0,
		ACE_Proactor *proactor = 0)
	{
		if(implementation_==NULL)
		{
			proactor = this->get_proactor (proactor, handler);
			if ((this->implementation_ = proactor->create_asynch_read_stream ()) == 0)
				return -1;
		}

		// Call the <open> method of the base class.
		return ACE_Asynch_Operation::open (handler,
			handle,
			completion_key,
			proactor);
	}
};

class Fast_Asynch_Write_Stream : public ACE_Asynch_Write_Stream
{
public:

	Fast_Asynch_Write_Stream (void){}

	virtual ~Fast_Asynch_Write_Stream (void){}

	int open (ACE_Handler &handler,
		ACE_HANDLE handle = ACE_INVALID_HANDLE,
		const void *completion_key = 0,
		ACE_Proactor *proactor = 0)
	{
		if(implementation_==NULL)
		{
			proactor = this->get_proactor (proactor, handler);

			// Now let us get the implementation initialized.
			if ((this->implementation_ = proactor->create_asynch_write_stream ()) == 0)
				return -1;
		}
		// Call the <open> method of the base class.
		return ACE_Asynch_Operation::open (handler,
			handle,
			completion_key,
			proactor);
	}
};