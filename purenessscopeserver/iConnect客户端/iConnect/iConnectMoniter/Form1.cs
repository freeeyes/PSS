using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using iConnect;
using System.Resources;
using System.Xml;
using System.Net.Sockets;

namespace iConnectMoniter
{
    public partial class Form1 : Form
    {
        ClientMoniter cc;
        internal static ResourceManager res;
        int serverid;

        public delegate void InvokeRefreshServer(ClienUpdateEventArgs e);
        Dictionary<string, serverpanel> serverplist = new Dictionary<string, serverpanel>();

        public Form1()
        {
            InitializeComponent();
            res = new ResourceManager("iConnectMoniter.Resource", this.GetType().Assembly);
            XmlDocument doc = new XmlDocument();
            doc.Load("Config.xml");
            XmlNodeList nodes = doc.GetElementsByTagName("Server");
            serverid = Convert.ToUInt16(nodes[0].Attributes.GetNamedItem("id").InnerText);

            cc = (ClientMoniter)Communicator.CreateClient(APPID.iMoniter);
            cc.updateClientEvent += new Client.updateClientDelegate(cc_updateClientEvent);
            cc.ExceptionEvent += new Client.ExceptionDelegate(cc_ExceptionEvent);
            //cc.ReceivedEvent += new Client.ReceiveDelegate(cc_ReceivedEvent);
        }

        void cc_ExceptionEvent(Exception e)
        {
            SocketException se = e as SocketException;
            if (se != null)
            {
                switch (se.ErrorCode)
                {
                    case 10065:
                        labelcurrent.Text = "服务器连接失败，找不到主机";
                        break;
                    case 10061:
                        labelcurrent.Text = "服务器连接失败，请检查服务器是否启动以及防火墙设置";
                        break;
                    case 10054:
                        labelcurrent.Text = "服务器连接被中断";
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
                MessageBox.Show("iConnectMoniter 已经在运行，并已连接到服务器。不允许重复连接\n 请关闭其他iConnectMoniter再运行", "注意", MessageBoxButtons.OK, MessageBoxIcon.Warning);
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
                            var d= from u in (e.data as List<ServerClass>) orderby u.m_Serverid descending select u;
                            foreach (var v in d)
                            {
                                makeServerView(v.m_Serverid, v.m_szStationID, v.isonline);
                                if (v.m_Serverid == serverid) labelcurrent.Text = "当前连接：Serverid = " + v.m_Serverid + "  StationID = " + v.m_szStationID;
                            }
                        }
                        break;
                    case CLIENTUPDATE.updateApp:
                        lock (serverplist)
                        {
                            foreach (var v in e.data as List<AppObj>)
                            {
                                UpdateAppView(cc.ServeridtoStationId(v.m_Serverid), v.app, v.isonline);
                            }
                        }
                        break;
                    default:
                        break;
                }
            }
        }

        private void makeServerView(ushort Serverid, string StationID, bool isonline)
        {
            if (!serverplist.ContainsKey(StationID))
            {
                
                serverpanel sp = new serverpanel(Serverid, StationID);
                sp.Online = isonline;
                sp.IniApp(Communicator.getVisualApp(APPID.iMoniter));
                sp.mainpanel.Parent = panel1;
                serverplist.Add(StationID, sp);
            }
            else
            {
                serverplist[StationID].Online = isonline;
            }
        }

        private void UpdateAppView(string StationID, APPID aPPID, bool isonline)
        {
            if (serverplist.ContainsKey(StationID))
            {
                serverplist[StationID].UpdateApp(aPPID, isonline);
            }
        }

        public class serverpanel
        {
            public Panel mainpanel;
            public Panel titlepanel;
            public PictureBox serverimg;
            public Label serverlabel;
            public Dictionary<APPID, apppanel> applist = new Dictionary<APPID, apppanel>();
            private bool _online = false;
            public bool Online
            {
                get { return _online; }
                set
                {
                    _online = value;
                    if (_online)
                        serverimg.Image = res.GetObject("on") as Image;
                    else
                    {
                        serverimg.Image = res.GetObject("off") as Image;
                        foreach(var v in applist)
                        {
                            v.Value.Online = false;
                        }
                    }
                }
            }

            public serverpanel(ushort Serverid, string StationID)
            {
                mainpanel = new Panel();
                mainpanel.Dock = DockStyle.Top;
                mainpanel.Height = 115;
                titlepanel = new Panel();
                titlepanel.Dock = DockStyle.Top;
                titlepanel.BorderStyle = BorderStyle.FixedSingle;
                titlepanel.BackColor = Color.Gainsboro;
                titlepanel.Height = 29;
                titlepanel.Parent = mainpanel;
                serverimg = new PictureBox();
                serverimg.Image = res.GetObject("off") as Image;
                serverimg.Location = new Point(6, 7);
                serverimg.Width = 16;
                serverimg.Height = 16;
                serverimg.Parent = titlepanel;
                serverlabel = new Label();
                serverlabel.Location = new Point(37, 9);
                serverlabel.Text = "Serverid = " + Serverid + "  StationID = " + StationID;
                serverlabel.AutoSize = true;
                serverlabel.Parent = titlepanel;
            }

            internal void UpdateApp(APPID aPPID, bool isonline)
            {
                if (isonline) Online = true;
                if(applist.ContainsKey(aPPID))
                {
                    applist[aPPID].Online = isonline;
                }
            }

            internal void IniApp(List<APPID> list)
            {
                int x = 0;
                int y = 0;
                applist.Clear();
                foreach(var v in list)
                {
                    apppanel ap = new apppanel(v);
                    ap.Online = false;
                    ap.mainpanel.Location = new Point((129 + 5) * x +5 , 29 + (33 + 3) * y + 3);
                    ap.mainpanel.Parent = mainpanel;
                    applist.Add(v, ap);
                    x++;
                    if(x>6)
                    {
                        x = 0; y++;
                    }
                }
            }
        }
        public class apppanel
        {
            public Panel mainpanel;
            public PictureBox appimg;
            public Label applabel;
            private bool _online = false;
            public bool Online
            {
                get { return _online; }
                set
                {
                    _online = value;
                    if (_online)
                        appimg.Image = res.GetObject("on") as Image;
                    else
                        appimg.Image = res.GetObject("off") as Image;
                }
            }

            public apppanel(APPID aPPID)
            {
                mainpanel = new Panel();
                mainpanel.Width = 129;
                mainpanel.Height = 33;
                appimg = new PictureBox();
                appimg.Image = res.GetObject("off") as Image;
                appimg.Location = new Point(4, 7);
                appimg.Width = 16;
                appimg.Height = 16;
                appimg.Parent = mainpanel;
                applabel = new Label();
                applabel.Text = aPPID.ToString();
                applabel.Location = new Point(27, 9);
                applabel.Parent = mainpanel;
            }
        }
    }
}
