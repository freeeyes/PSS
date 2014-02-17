using System;
using System.Collections.Generic;
using System.Drawing;
using System.Windows.Forms;
using System.Linq;
using iConnect;
using System.Threading;
using System.IO;
using System.Xml;
using System.Resources;
using System.Net.Sockets;

namespace iConnectTest
{

    public partial class Form1 : Form
    {
        private List<Thread> getThread = new List<Thread>();
        private int getThreadCount = 5;

        ClientP2P cc;
        string stationid;
        int serverid;
        Dictionary<string, serverpanel> serverplist = new Dictionary<string, serverpanel>();
        Dictionary<ushort, SendReportPanel> sendreports = new Dictionary<ushort, SendReportPanel>();
        Dictionary<ushort, ReceiveReportPanel> receivereports = new Dictionary<ushort, ReceiveReportPanel>();
        public delegate void InvokeRefreshServer(ClienUpdateEventArgs e);
        public delegate void InvokeExceptionEvent(Exception e);
        public delegate void InvokeReceivedEvent(ReceivedobjEventArgs e);
        public delegate void InvokeSendStatusEvent(SendStatusEventArgs e);
        public static ResourceManager res;
        private uint _taskid = 0;
        private readonly object syntaskid = new object();
        public uint Taskid
        {
            get
            {
                lock (syntaskid)
                {
                    _taskid++;
                    return _taskid;
                }
            }
        }
        public Dictionary<uint, DateTime> tasklist = new Dictionary<uint, DateTime>();

        public Form1()
        {
            InitializeComponent();
            res = new ResourceManager("iConnectTest.Resource", this.GetType().Assembly);
            this.timer1.Enabled = true;

            XmlDocument doc = new XmlDocument();
            doc.Load("Config.xml");
            XmlNodeList nodes = doc.GetElementsByTagName("Server");
            serverid = Convert.ToUInt16(nodes[0].Attributes.GetNamedItem("id").InnerText);

            cc = (ClientP2P)Communicator.CreateClient(APPID.TestApp2App);
            cc.updateClientEvent += new Client.updateClientDelegate(cc_updateClientEvent);
            cc.SendStatusEvent += new Client.SendStatusDelegate(cc_SendStatusEvent);
            //cc.ReceivedEvent += new Client.ReceiveDelegate(cc_ReceivedEvent);
            cc.ExceptionEvent += new Client.ExceptionDelegate(cc_ExceptionEvent);

            for (int i = 0; i < getThreadCount; i++)
            {
                getThread.Add(new Thread(GetccData));
            }

            foreach (var v in getThread)
            {
                v.IsBackground = true;
                v.Start();
            }
        }

        private void GetccData()
        {
            while(true)
            {
                ReceivedobjEventArgs e = cc.GetData();
                if(e!=null)
                {
                    cc_ReceivedEvent(e);
                }
            }
        }

        void cc_ExceptionEvent(Exception e)
        {
            if (this.InvokeRequired)
            {
                InvokeExceptionEvent ivo = new InvokeExceptionEvent(this.cc_ExceptionEvent);
                this.Invoke(ivo, e);
            }
            else
            {
                SocketException se = e as SocketException;
                if (se != null)
                {
                    switch (se.ErrorCode)
                    {
                        case 10065:
                            label1.Text = "服务器连接失败，找不到主机";
                            break;
                        case 10061:
                            label1.Text = "服务器连接失败，请检查服务器是否启动以及防火墙设置";
                            break;
                        case 10054:
                            label1.Text = "服务器连接被中断";
                            break;
                        default:
                            break;
                    }
                    foreach(var v in serverplist)
                    {
                        v.Value.Online = false;
                    }
                }
                else if (e.Message == "应用注册失败，同类应用可能已经在运行。")
                {
                    MessageBox.Show("iConnectTest 已经在运行，并连接到服务器。不允许重复连接\n 请关闭其他iConnectTest再运行", "注意", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                }
                else
                {
                    labelerr.Text = "错误信息：" + e.Message;
                    labelerrstack.Text = e.StackTrace;
                }
            }
        }

        void cc_ReceivedEvent(ReceivedobjEventArgs e)
        {
//             if (this.InvokeRequired)
//             {
//                 InvokeReceivedEvent ivo = new InvokeReceivedEvent(this.cc_ReceivedEvent);
//                 this.Invoke(ivo, e);
//             }
//             else
//             {
                handlereceivedata(((testpacket)e.Data));
/*            }*/
        }

        void handlereceivedata(testpacket Data)
        {
            switch (Data.type)
            {
                case PacketType.sdata:
                    receivereports[Data.senderServerid].Receive(Data.type);
                    break;
                case PacketType.mdata:
                    receivereports[Data.senderServerid].Receive(Data.type);
                    break;
                case PacketType.ldata:
                    receivereports[Data.senderServerid].Receive(Data.type);
                    break;
                case PacketType.text:
                    textBox8.AppendText(Data.text);
                    break;
                case PacketType.file:
                    saveFileDialog1.FileName = Data.text;
                    if (saveFileDialog1.ShowDialog() == DialogResult.OK)
                    {
                        File.WriteAllBytes(saveFileDialog1.FileName, Data.data);
                    }
                    break;
                default:
                    break;
            }
        }

        void cc_SendStatusEvent(SendStatusEventArgs e)
        {
            if (this.InvokeRequired)
            {
                InvokeSendStatusEvent ivo = new InvokeSendStatusEvent(this.cc_SendStatusEvent);
                this.Invoke(ivo, e);
            }
            else
            {
                switch (e.Status)
                {
                    case SENDSTATUS.PACKET_RECEIVED:
                        switch (((testpacket)e.Packet.data).type)
                        {
                            case PacketType.sdata:
                                sendreports[((testpacket)e.Packet.data).recvServerid].SendOK(((testpacket)e.Packet.data).type, e.Packet.packetsize, e.Packet.sendtime,e.Packet.sendfinishtime,e.Packet.outtime);
                                if (((testpacket)e.Packet.data).isEnd)
                                {
                                    double runtime = (DateTime.Now - tasklist[((testpacket)e.Packet.data).taskid]).TotalMilliseconds;
                                    tasklist.Remove(((testpacket)e.Packet.data).taskid);
                                    sendreports[((testpacket)e.Packet.data).recvServerid].TaskDone(((testpacket)e.Packet.data).type, runtime);
                                }
                                break;
                            case PacketType.mdata:
                                sendreports[((testpacket)e.Packet.data).recvServerid].SendOK(((testpacket)e.Packet.data).type, e.Packet.packetsize, e.Packet.sendtime, e.Packet.sendfinishtime, e.Packet.outtime);
                                if (((testpacket)e.Packet.data).isEnd)
                                {
                                    double runtime = (DateTime.Now - tasklist[((testpacket)e.Packet.data).taskid]).TotalMilliseconds;
                                    tasklist.Remove(((testpacket)e.Packet.data).taskid);
                                    sendreports[((testpacket)e.Packet.data).recvServerid].TaskDone(((testpacket)e.Packet.data).type, runtime);
                                }
                                break;
                            case PacketType.ldata:
                                sendreports[((testpacket)e.Packet.data).recvServerid].SendOK(((testpacket)e.Packet.data).type, e.Packet.packetsize, e.Packet.sendtime, e.Packet.sendfinishtime, e.Packet.outtime);
                                if (((testpacket)e.Packet.data).isEnd)
                                {
                                    double runtime = (DateTime.Now - tasklist[((testpacket)e.Packet.data).taskid]).TotalMilliseconds;
                                    tasklist.Remove(((testpacket)e.Packet.data).taskid);
                                    sendreports[((testpacket)e.Packet.data).recvServerid].TaskDone(((testpacket)e.Packet.data).type, runtime);
                                }
                                break;
                            case PacketType.text:
                                break;
                            case PacketType.file:
                                break;
                            default:
                                break;
                        }
                        break;
                    case SENDSTATUS.PACKET_SERVERDOWN:
                        textBox9.AppendText(e.Msg);
                        //textBox9.AppendText(":" + e.Packet.ToString());
                        textBox9.AppendText(((char)10).ToString());
                        break;
                    case SENDSTATUS.PACKET_APPDOWN:
                        textBox9.AppendText(e.Msg);
                        //textBox9.AppendText(":" + e.Packet.ToString());
                        textBox9.AppendText(((char)10).ToString());
                        break;
                    case SENDSTATUS.PACKET_REJECT:
                        break;
                    case SENDSTATUS.PACKET_ERR:
                        textBox9.AppendText(e.Msg);
                        textBox9.AppendText(((char)10).ToString());
                        break;
                    case SENDSTATUS.PACKET_TIMEOUT:
                        textBox9.AppendText(e.Msg + "/n");
                        break;
                    case SENDSTATUS.PACKET_SERVERUNKOWN:
                        break;
                    case SENDSTATUS.PACKET_CANNOTCROSSAPP:
                        break;
                    default:
                        break;
                }
            }
        }

        void cc_updateClientEvent(ClienUpdateEventArgs e)
        {
            if (this.InvokeRequired)
            {
                InvokeRefreshServer ivo = new InvokeRefreshServer(this.cc_updateClientEvent);
                this.Invoke(ivo, e);
            }
            else
            {
                switch (e.clientupdate)
                {
                    case CLIENTUPDATE.updateServer:
                        lock (serverplist)
                        {
                            var d = from u in (e.data as List<ServerClass>) orderby u.m_Serverid descending select u;
                            foreach (var v in d)
                            {
                                makeServerView(v.m_szStationID, v.isonline);
                                makeSendReportPanel(v.m_Serverid);
                                makeReceiveReportPanel(v.m_Serverid);
                                if (v.m_Serverid == serverid) label1.Text = "当前服务器：" + v.m_szStationID;
                            }
                        }
                        break;
                    case CLIENTUPDATE.updateApp:
                        lock (serverplist)
                        {
                            foreach (var v in e.data as List<AppObj>)
                            {
                                UpdateServerView(cc.ServeridtoStationId(v.m_Serverid), v.isonline);
                            }
                        }
                        break;
                    default:
                        break;
                }
            }
        }

        private void makeSendReportPanel(ushort serverid)
        {
            if(!sendreports.ContainsKey(serverid))
            {
                SendReportPanel p = new SendReportPanel(serverid);
                p.MainPanel.Parent = panelSendReport;
                sendreports.Add(serverid, p);
            }
        }

        private void makeReceiveReportPanel(ushort serverid)
        {
            if (!receivereports.ContainsKey(serverid))
            {
                ReceiveReportPanel p = new ReceiveReportPanel(serverid);
                p.MainPanel.Parent = panelReceiveReport;
                receivereports.Add(serverid, p);
            }
        }

        private void UpdateServerView(string stationid, bool online)
        {
            if (serverplist.ContainsKey(stationid))
            {
                serverplist[stationid].AppOnline = online;
            }
        }

        private void makeServerView(string stationid,bool online)
        {
            if (!serverplist.ContainsKey(stationid))
            {
                serverpanel sp = new serverpanel();
                sp.p.Parent = panel1;
                sp.b.Text = stationid;
                sp.b.Click += new EventHandler(b_Click);
                sp.Online = online;
                serverplist.Add(stationid, sp);
            }
            else
            {
                serverplist[stationid].Online = online;
            }
        }

        void b_Click(object sender, EventArgs e)
        {
            stationid = ((Button)sender).Text;
            labelcurrentselect.Text = "当前选择服务器："+((Button)sender).Text;
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            foreach (var v in sendreports)
            {
                v.Value.UpdateView();
            }
            foreach (var v in receivereports)
            {
                v.Value.UpdateView();
            }
        }

        private void buttonreset_Click(object sender, EventArgs e)
        {
            labelerr.Text = "";
            labelerrstack.Text = "";
            textBox9.Text = "";
            foreach(var v in sendreports)
            {
                v.Value.Reset();
            }
            foreach (var v in receivereports)
            {
                v.Value.Reset();
            }
        }

        private void buttonsends_Click(object sender, EventArgs e)
        {
           if (checkstationid())
           {
               try
               {
                   int count = Convert.ToInt32(textBoxsends.Text) - 1;
                   uint taskid = Taskid;
                   tasklist.Add(taskid, DateTime.Now);
                   for (int i = 0; i < count + 1; i++)
                   {
                       testpacket p = new testpacket();
                       p.taskid = taskid;
                       if (i == count) p.isEnd = true;
                       p.senderServerid = cc.LocalServerid;
                       p.recvServerid = cc.StationIdtoServerid(stationid);
                       p.type = PacketType.sdata;
                       p.data = new byte[1024];
                       cc.Send(stationid, p);
                       sendreports[p.recvServerid].Send(PacketType.sdata);
                   }
               }
               catch (FormatException ex)
               {
                   MessageBox.Show("请输入正确的数字");
               }
           }
        }

        private bool checkstationid()
        {
            if (stationid == null)
            {
                MessageBox.Show("请先选择服务器");
                return false;
            }
            else
                return true;
        }

        private void buttonsendm_Click(object sender, EventArgs e)
        {
            if (checkstationid())
            {
                try
                {
                    int count = Convert.ToInt32(textBoxsendm.Text) - 1;
                    uint taskid = Taskid;
                    tasklist.Add(taskid, DateTime.Now);
                    for (int i = 0; i < count + 1; i++)
                    {
                        testpacket p = new testpacket();
                        p.taskid = taskid;
                        if (i == count) p.isEnd = true;
                        p.senderServerid = cc.LocalServerid;
                        p.recvServerid = cc.StationIdtoServerid(stationid);
                        p.type = PacketType.mdata;
                        p.data = new byte[51200];
                        cc.Send(stationid, p);
                        sendreports[p.recvServerid].Send(PacketType.mdata);
                    }
                }
                catch (FormatException ex)
                {
                    MessageBox.Show("请输入正确的数字");
                }
            }
        }

        private void buttonsendl_Click(object sender, EventArgs e)
        {
            if (checkstationid())
            {
                try
                {
                    int count = Convert.ToInt32(textBoxsendl.Text) - 1;
                    uint taskid = Taskid;
                    tasklist.Add(taskid, DateTime.Now);
                    for (int i = 0; i < count + 1; i++)
                    {
                        testpacket p = new testpacket();
                        p.taskid = taskid;
                        if (i == count) p.isEnd = true;
                        p.senderServerid = cc.LocalServerid;
                        p.recvServerid = cc.StationIdtoServerid(stationid);
                        p.type = PacketType.ldata;
                        p.data = new byte[20971520];
                        cc.Send(stationid, p);
                        sendreports[p.recvServerid].Send(PacketType.ldata);
                    }
                }
                catch (FormatException ex)
                {
                    MessageBox.Show("请输入正确的数字");
                }
            }
        }

        private void button16_Click(object sender, EventArgs e)
        {
            sendtext();
        }
        private void sendtext()
        {
            if (checkstationid())
            {
                testpacket p = new testpacket();
                p.type = PacketType.text;
                p.text = textBox7.Text;
                cc.Send(stationid, p);
                textBox7.Clear();
            }
        }

        private void textBox7_KeyUp(object sender, KeyEventArgs e)
        {
            if (e.KeyValue == 13)
            {
                sendtext();
            }
        }

        private void button17_Click(object sender, EventArgs e)
        {
            if (checkstationid())
            {
                if (openFileDialog1.ShowDialog() == DialogResult.OK)
                {
                    testpacket p = new testpacket();
                    p.type = PacketType.file;
                    p.text = openFileDialog1.SafeFileName;
                    p.data = File.ReadAllBytes(openFileDialog1.FileName);
                    cc.Send(stationid, p);
                }
            }
        }

        public class serverpanel
        {
            public Panel p;
            public Button b;
            public PictureBox img;
            public PictureBox appimg;
            private bool _online = false;
            public bool Online
            {
                get { return _online; }
                set
                {
                    _online = value;
                    if (_online)
                        img.Image = res.GetObject("ServerOn") as Image;
                    else
                    {
                        img.Image = res.GetObject("ServerOff") as Image;
                        AppOnline = false;
                    }
                }
            }
            private bool _apponline = false;
            public bool AppOnline
            {
                get { return _apponline; }
                set
                {
                    _apponline = value;
                    if (_apponline)
                    {
                        appimg.Image = res.GetObject("AppOn") as Image;
                        Online = true;
                    }
                    else
                    {
                        appimg.Image = res.GetObject("AppOff") as Image;
                    }
                }
            }

            public serverpanel()
            {
                p = new System.Windows.Forms.Panel();
                p.BorderStyle = BorderStyle.FixedSingle;
                p.Dock = DockStyle.Top;
                p.Height = 38;
                b = new System.Windows.Forms.Button();
                b.Location = new Point(63, 3);
                b.Height = 30;
                b.Width = 131;
                b.Parent = p;
                appimg = new System.Windows.Forms.PictureBox();
                appimg.Image = res.GetObject("AppOff") as Image;
                appimg.Location = new Point(37, 9);
                appimg.Parent = p;
                img = new System.Windows.Forms.PictureBox();
                img.Image = res.GetObject("ServerOff") as Image;
                img.Location = new Point(11, 9);
                img.Parent = p;
            }
        }
        public class SendReportPanel
        {
            public SpeedType speedtype;

            public Panel MainPanel;
            public Panel Serveridpanel;
            public Panel midGridpanel;
            public Label ServeridLabel;
            public Label sPacketSend;
            public Label sPacketArrived;
            public Label sPacketSpeed;
            public Label mPacketSend;
            public Label mPacketArrived;
            public Label mPacketSpeed;
            public Label lPacketSend;
            public Label lPacketArrived;
            public Label lPacketSpeed;

            private int sPsend = 0;
            private int sParrive = 0;
            private ulong sPsize = 0;
            private double sPtime = 0;
            private double sPtime2 = 0;
            private double sPrealtime = 0;
            private int mPsend = 0;
            private int mParrive = 0;
            private ulong mPsize = 0;
            private double mPtime = 0;
            private double mPtime2 = 0;
            private double mPrealtime = 0;
            private int lPsend = 0;
            private int lParrive = 0;
            private ulong lPsize = 0;
            private double lPtime = 0;
            private double lPtime2 = 0;
            private double lPrealtime = 0;
            private readonly object synobj = new object();

            public SendReportPanel(ushort serverid)
            {
                MainPanel = new Panel();
                MainPanel.Height = 25;
                MainPanel.Dock = DockStyle.Top;
                MainPanel.BackColor = Color.Azure;
                Serveridpanel = new Panel();
                Serveridpanel.Width = 38;
                Serveridpanel.Dock = DockStyle.Left;
                Serveridpanel.BackColor = Color.LightSkyBlue;
                Serveridpanel.Parent = MainPanel;
                midGridpanel = new Panel();
                midGridpanel.Width = 175;
                midGridpanel.Height = 27;
                midGridpanel.Location = new Point(220, 0);
                midGridpanel.BackColor = Color.LightCyan;
                midGridpanel.Parent = MainPanel;
                ServeridLabel = new Label();
                ServeridLabel.Text = serverid.ToString();
                ServeridLabel.Location = new Point(6, 8);
                ServeridLabel.Parent = Serveridpanel;
                sPacketSpeed = new Label();
                sPacketSpeed.Text = "0";
                sPacketSpeed.Location = new Point(138, 8);
                sPacketSpeed.Parent = MainPanel;
                sPacketArrived = new Label();
                sPacketArrived.Text = "0";
                sPacketArrived.Location = new Point(91, 8);
                sPacketArrived.Parent = MainPanel;
                sPacketSend = new Label();
                sPacketSend.Text = "0";
                sPacketSend.Location = new Point(44, 8);
                sPacketSend.Parent = MainPanel;
                mPacketSpeed = new Label();
                mPacketSpeed.Text = "0";
                mPacketSpeed.Location = new Point(97, 8);
                mPacketSpeed.Parent = midGridpanel;
                mPacketArrived = new Label();
                mPacketArrived.Text = "0";
                mPacketArrived.Location = new Point(50, 8);
                mPacketArrived.Parent = midGridpanel;
                mPacketSend = new Label();
                mPacketSend.Text = "0";
                mPacketSend.Location = new Point(3, 8);
                mPacketSend.Parent = midGridpanel;
                lPacketSpeed = new Label();
                lPacketSpeed.Text = "0";
                lPacketSpeed.Location = new Point(501, 8);
                lPacketSpeed.Parent = MainPanel;
                lPacketArrived = new Label();
                lPacketArrived.Text = "0";
                lPacketArrived.Location = new Point(454, 8);
                lPacketArrived.Parent = MainPanel;
                lPacketSend = new Label();
                lPacketSend.Text = "0";
                lPacketSend.Location = new Point(407, 8);
                lPacketSend.Parent = MainPanel;
            }

            internal void Send(PacketType packetType)
            {
                lock(synobj)
                {
                    switch (packetType)
                    {
                        case PacketType.sdata:
                            sPsend++;
                            break;
                        case PacketType.mdata:
                            mPsend++;
                            break;
                        case PacketType.ldata:
                            lPsend++;
                            break;
                        case PacketType.text:
                            break;
                        case PacketType.file:
                            break;
                        default:
                            break;
                    }
                }
            }

            internal void SendOK(PacketType packetType, uint size, DateTime sendtime, DateTime sendfinishtime, DateTime sendOKtime)
            {
                lock (synobj)
                {
                    switch (packetType)
                    {
                        case PacketType.sdata:
                            sPsize = sPsize + size;
                            sPtime = sPtime + (sendfinishtime - sendtime).TotalMilliseconds;
                            sPtime2 = sPtime2 + (sendOKtime - sendtime).TotalMilliseconds;
                            sParrive++;
                            break;
                        case PacketType.mdata:
                            mPsize = mPsize + size;
                            mPtime = mPtime + (sendfinishtime - sendtime).TotalMilliseconds;
                            mPtime2 = mPtime2 + (sendOKtime - sendtime).TotalMilliseconds;
                            mParrive++;
                            break;
                        case PacketType.ldata:
                            lPsize = lPsize + size;
                            lPtime = lPtime + (sendfinishtime - sendtime).TotalMilliseconds;
                            lPtime2 = lPtime2 + (sendOKtime - sendtime).TotalMilliseconds;
                            lParrive++;
                            break;
                        case PacketType.text:
                            break;
                        case PacketType.file:
                            break;
                        default:
                            break;
                    }
                }
            }
            internal void Reset()
            {
                lock (synobj)
                {
                    sPsend = 0;
                    sParrive = 0;
                    sPsize = 0;
                    sPtime = 0;
                    sPtime2 = 0;
                    sPrealtime = 0;
                    mPsend = 0;
                    mParrive = 0;
                    mPsize = 0;
                    mPtime = 0;
                    mPtime2 = 0;
                    mPrealtime = 0;
                    lPsend = 0;
                    lParrive = 0;
                    lPsize = 0;
                    lPtime = 0;
                    lPtime2 = 0;
                    lPrealtime = 0;
                }
            }
            internal void UpdateView()
            {
                bool viewspeed = true;
                double stime = 0;
                double mtime = 0;
                double ltime = 0;
                switch (speedtype)
                {
                    case SpeedType.RealSpeed:
                        stime = sPrealtime;
                        mtime = mPrealtime;
                        ltime = lPrealtime;
                        viewspeed = true;
                        break;
                    case SpeedType.SingleTripSpeed:
                        stime = sPtime;
                        mtime = mPtime;
                        ltime = lPtime;
                        viewspeed = true;
                        break;
                    case SpeedType.AvgSpeedperPacket:
                        stime = sPtime2;
                        mtime = mPtime2;
                        ltime = lPtime2;
                        viewspeed = true;
                        break;
                    case SpeedType.RealCountSpeed:
                        stime = sPrealtime;
                        mtime = mPrealtime;
                        ltime = lPrealtime;
                        viewspeed = false;
                        break;
                    case SpeedType.SingleTripCountSpeed:
                        stime = sPtime;
                        mtime = mPtime;
                        ltime = lPtime;
                        viewspeed = false;
                        break;
                    case SpeedType.AvgCountSpeedperPacket:

                        stime = sPtime2;
                        mtime = mPtime2;
                        ltime = lPtime2;
                        viewspeed = false;
                        break;
                    default:
                        break;
                }
                sPacketSend.Text = sPsend.ToString();
                mPacketSend.Text = mPsend.ToString();
                lPacketSend.Text = lPsend.ToString();
                sPacketArrived.Text = sParrive.ToString();
                mPacketArrived.Text = mParrive.ToString();
                lPacketArrived.Text = lParrive.ToString();
                if (stime == 0)
                    sPacketSpeed.Text = "0";
                else
                {
                    if (viewspeed)
                    {
                        double speed = sPsize / stime * 1000;
                        sPacketSpeed.Text = speedStr(speed);
                    }
                    else
                    {
                        double speed = sParrive / stime * 1000;
                        sPacketSpeed.Text = countspeedStr(speed);
                    }
                }
                if (mtime == 0)
                    mPacketSpeed.Text = "0";
                else
                {
                    if (viewspeed)
                    {
                        double speed = mPsize / mtime * 1000;
                        mPacketSpeed.Text = speedStr(speed);
                    }
                    else
                    {
                        double speed = mParrive / mtime * 1000;
                        mPacketSpeed.Text = countspeedStr(speed);
                    }
                }
                if (ltime == 0)
                    lPacketSpeed.Text = "0";
                else
                {
                    if (viewspeed)
                    {
                        double speed = lPsize / ltime * 1000;
                        lPacketSpeed.Text = speedStr(speed);
                    }
                    else
                    {
                        double speed = lParrive / ltime * 1000;
                        lPacketSpeed.Text = countspeedStr(speed);
                    }
                }
            }
            private string speedStr(double speed)
            {
                if (speed < 1024)
                    return Math.Round(speed, 2).ToString() + "byte/s";
                else if (speed < 1048576)
                    return Math.Round((speed / 1024.0), 2).ToString() + "KB/s";
                else if (speed < 1073741824)
                    return Math.Round((speed / 1048576.0), 2).ToString() + "MB/s";
                else
                    return Math.Round((speed / 1073741824.0), 2).ToString() + "GB/s";
            }
            private string countspeedStr(double speed)
            {
                    return Math.Round(speed, 2).ToString() + "包/s";
            }
            internal void TaskDone(PacketType packetype, double runtime)
            {
                lock (synobj)
                {
                    switch (packetype)
                    {
                        case PacketType.sdata:
                            sPrealtime = sPrealtime + runtime;
                            break;
                        case PacketType.mdata:
                            mPrealtime = mPrealtime + runtime;
                            break;
                        case PacketType.ldata:
                            lPrealtime = lPrealtime + runtime;
                            break;
                        case PacketType.text:
                            break;
                        case PacketType.file:
                            break;
                        default:
                            break;
                    }
                }
            }
        }

        public class ReceiveReportPanel
        {
            public Panel MainPanel;
            public Panel Serveridpanel;
            public Panel midGridpanel;
            public Label ServeridLabel;
            public Label sPacketReceive;
            public Label mPacketReceive;
            public Label lPacketReceive;
            private int sPrecv = 0;
            private int mPrecv = 0;
            private int lPrecv = 0;
            private readonly object synobj = new object();
            public ReceiveReportPanel(ushort serverid)
            {
                MainPanel = new Panel();
                MainPanel.Height = 25;
                MainPanel.Dock = DockStyle.Top;
                MainPanel.BackColor = Color.Azure;
                Serveridpanel = new Panel();
                Serveridpanel.Width = 38;
                Serveridpanel.Dock = DockStyle.Left;
                Serveridpanel.BackColor = Color.LightSkyBlue;
                Serveridpanel.Parent = MainPanel;
                midGridpanel = new Panel();
                midGridpanel.Width = 72;
                midGridpanel.Height = 27;
                midGridpanel.Location = new Point(109, 0);
                midGridpanel.BackColor = Color.LightCyan;
                midGridpanel.Parent = MainPanel;
                ServeridLabel = new Label();
                ServeridLabel.Text = serverid.ToString();
                ServeridLabel.Location = new Point(6, 8);
                ServeridLabel.Parent = Serveridpanel;
                mPacketReceive = new Label();
                mPacketReceive.Text = "0";
                mPacketReceive.Location = new Point(3, 8);
                mPacketReceive.Parent = midGridpanel;
                lPacketReceive = new Label();
                lPacketReceive.Text = "0";
                lPacketReceive.Location = new Point(187, 8);
                lPacketReceive.Parent = MainPanel;
                sPacketReceive = new Label();
                sPacketReceive.Text = "0";
                sPacketReceive.Location = new Point(44, 8);
                sPacketReceive.Parent = MainPanel;
            }

            internal void Receive(PacketType packetType)
            {
                lock (synobj)
                {
                    switch (packetType)
                    {
                        case PacketType.sdata:
                            sPrecv++;
                            break;
                        case PacketType.mdata:
                            mPrecv++;
                            break;
                        case PacketType.ldata:
                            lPrecv++;
                            break;
                        case PacketType.text:
                            break;
                        case PacketType.file:
                            break;
                        default:
                            break;
                    }
                }
            }
            internal void Reset()
            {
                lock (synobj)
                {
                    sPrecv = 0;
                    mPrecv = 0;
                    lPrecv = 0;
                }
            }
            internal void UpdateView()
            {
                sPacketReceive.Text = sPrecv.ToString();
                mPacketReceive.Text = mPrecv.ToString();
                lPacketReceive.Text = lPrecv.ToString();
            }
        }

        private void radioButton1_CheckedChanged(object sender, EventArgs e)
        {
            SpeedType speedtype = SpeedType.RealSpeed;
            if(radioButton1.Checked)
                speedtype = SpeedType.RealSpeed;
            else if(radioButton2.Checked)
                speedtype = SpeedType.SingleTripSpeed;
            else if (radioButton3.Checked)
                speedtype = SpeedType.AvgSpeedperPacket;
            else if (radioButton4.Checked)
                speedtype = SpeedType.RealCountSpeed;
            else if (radioButton5.Checked)
                speedtype = SpeedType.SingleTripCountSpeed;
            else if (radioButton6.Checked)
                speedtype = SpeedType.AvgCountSpeedperPacket;
            lock (sendreports)
            {
                foreach (var v in sendreports)
                {
                    v.Value.speedtype = speedtype;
                }
            }
        }
    }

    public class myEventWaitHandle : EventWaitHandle
    {
        public bool blocked = false;
        public myEventWaitHandle(bool ini, EventResetMode mode)
            : base(ini, mode)
        {

        }
    }


    [Serializable]
    public class testpacket
    {
        public uint taskid = 0;
        public bool isEnd = false;
        public ushort senderServerid;
        public ushort recvServerid;
        public PacketType type;
        public byte[] data = null;
        public string text = "";
    }
    public enum PacketType
    {
        sdata,
        mdata,
        ldata,
        text,
        file
    }
    public enum SpeedType
    {
        RealSpeed,
        SingleTripSpeed,
        AvgSpeedperPacket,
        RealCountSpeed,
        SingleTripCountSpeed,
        AvgCountSpeedperPacket,
    }
}
