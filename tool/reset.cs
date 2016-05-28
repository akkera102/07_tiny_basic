using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO.Ports;

namespace ConsoleApplication1
{
    class Program
    {
        static int Main(string[] args)
        {
            if(args.Length != 1)
            {
                Console.WriteLine("args error");
                return -1;
            }


            Console.WriteLine("resetting...");
            SerialPort port = new SerialPort(args[0], 1200);
            port.DtrEnable = true;

            try
            {
                port.Open();
            }
            catch (Exception e)
            {
                Console.WriteLine(e.Message);
                return -1;
            }

            System.Threading.Thread.Sleep(500);

            port.DtrEnable = false;
            port.Close();
            port.Dispose();

            System.Threading.Thread.Sleep(2000);

            Console.WriteLine("done");
            return 0;
        }
    }
}
