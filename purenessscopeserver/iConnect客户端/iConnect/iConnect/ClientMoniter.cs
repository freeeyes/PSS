using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace iConnect
{
    public class ClientMoniter : Client
    {
        internal ClientMoniter(AppClass App, string ConfigPath)
            : base(App,ConfigPath)
        {
        }
    }
}
