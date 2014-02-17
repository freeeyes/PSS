using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
//using iCClientClr;
using System.Net;
using System.Net.Sockets;


namespace iConnect
{
    ///<summary>通讯类，通过CreateClient来生成通讯客户端</summary>
    public static class Communicator
    {
        private static readonly object SynObject = new object();
        private static Dictionary<APPID, Client> Clients = new Dictionary<APPID, Client>();
        private static List<AppClass> Apps = new List<AppClass>() {  // 所有App信息，以后可移到配置文件
        new AppClass(){id = APPID.Authorization,appname = "Authorization",type=APPTYPE.Authorization},
        new AppClass(){id = APPID.TestApp2App,appname = "TestApp2App",type=APPTYPE.App2App},
        new AppClass(){id = APPID.TestApp,appname = "TestApp",type=APPTYPE.App2Res},
        new AppClass(){id = APPID.TestAppResource,appname = "TestAppResource",type=APPTYPE.Res2App},
        new AppClass(){id = APPID.iMoniter,appname = "iMoniter",type=APPTYPE.Moniter}
        };

        public static APPTYPE getApptype(APPID appid)
        {
            foreach(var v in Apps)
            {
                if (v.id == appid) return v.type;
            }
            return APPTYPE.App2App;
        }

        public static string getAppname(APPID appid)
        {
            foreach (var v in Apps)
            {
                if (v.id == appid) return v.appname;
            }
            return "";
        }
        // 返回该APP可以通讯的APP列表，权限控制
        public static List<APPID> getVisualApp(APPID appid)
        {
            List<APPID> result = new List<APPID>();
            APPTYPE type = getApptype(appid);
            switch (type)
            {
                case APPTYPE.App2Res:
                    switch (appid)
                    {
                        case APPID.TestApp:
                            result.Add(APPID.Authorization);
                            result.Add(APPID.TestApp);
                            result.Add(APPID.TestAppResource);
                            break;
                        default:
                            break;
                    }
                    break;
                case APPTYPE.App2App:
                    result.Add(appid);
                    break;
                case APPTYPE.Res2App:
                    switch (appid)
                    {
                        case APPID.TestAppResource:
                            result.Add(APPID.Authorization);
                            result.Add(APPID.TestApp);
                            result.Add(APPID.TestAppResource);
                            break;
                        default:
                            break;
                    }
                    break;
                case APPTYPE.Authorization:
                    result.Add(APPID.Authorization);
                    result.Add(APPID.TestApp);
                    result.Add(APPID.TestAppResource);
                    break;
                case APPTYPE.Moniter:
                    result.Add(APPID.Authorization);
                    result.Add(APPID.TestApp);
                    result.Add(APPID.TestAppResource);
                    result.Add(APPID.TestApp2App);
                    result.Add(APPID.iMoniter);
                    break;
                default:
                    break;
            }
            return result;
        }

        public static APPID getResID(APPID appid)
        {
            switch (appid)
            {
                case APPID.TestApp:
                    return APPID.TestAppResource;
                default:
                    return APPID.Unkown;
            }
        }
        ///<summary>生产通讯客户端。每一个module只生成唯一的实例</summary>
        ///<param name="module">辨识应用的唯一字符串，大小写敏感</param>
        public static Client CreateClient(APPID App)
        {
            return CreateClient(App, "");
        }
        public static Client CreateClient(APPID App, string ConfigPath)
        {
            if (!Clients.ContainsKey(App))
            {
                lock (SynObject)
                {
                    if (!Clients.ContainsKey(App))
                    {
                        foreach (var v in Apps)
                        {
                            if (v.id == App)
                            {
                                switch (v.type)
                                {
                                    case APPTYPE.App2Res:
                                        Client c1 = new ClientApp(v, ConfigPath);
                                        Clients.Add(App, c1);
                                        break;
                                    case APPTYPE.App2App:
                                        Client c2 = new ClientP2P(v, ConfigPath);
                                        Clients.Add(App, c2);
                                        break;
                                    case APPTYPE.Res2App:
                                        Client c3 = new ClientRes(v, ConfigPath);
                                        Clients.Add(App, c3);
                                        break;
                                    case APPTYPE.Authorization:
                                        Client c4 = new ClientAuthor(v, ConfigPath);
                                        Clients.Add(App, c4);
                                        break;
                                    case APPTYPE.Moniter:
                                        Client c5 = new ClientMoniter(v, ConfigPath);
                                        Clients.Add(App, c5);
                                        break;
                                    default:
                                        break;
                                }
                                break;
                            }
                        }
                    }
                }
            }
            return Clients[App];
        }
    }
}
