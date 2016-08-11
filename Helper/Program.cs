using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;

namespace Helper
{
    class Program
    {
        static void Main(string[] args)
        {
            var Files = Directory.GetFiles(@"C:\Users\Philip\Desktop\1\DS\P-Erm\P-Emerald_2\P-Emerald_2\FSROOT\PICS");
            foreach (var item in Files)
            {
                if (item.EndsWith(".raw"))
                    continue;
                using (StreamWriter BW = new StreamWriter(new FileStream(item.Replace(".s", ".raw"), FileMode.Create)))
                {
                    var In = File.ReadAllLines(item);
                    int index = 20;
                    for (int i = 0; i < 192; i++)
                    {
                        for (int j = 0; j < 8; j++)
                        {
                            var ln = In[index++].Split(' ', ',');
                            string s = "";
                            for (int k = 0; k < 8; k++)
                                s += ln[k + 1] + " ";
                            BW.Write(s);
                        }
                        ++index;
                    }
                    index = 1752;
                    for (int i = 0; i < 4; i++)
                    {
                        for (int j = 0; j < 8; j++)
                        {
                            var ln = In[index++].Split(' ', ',');
                            string s = "";
                            for (int k = 0; k < 8; k++)
                                s += ln[k + 1] + " ";
                            BW.Write(s);
                        }
                        ++index;
                    }
                }
                File.Delete(item);
            }
        }
    }
}
