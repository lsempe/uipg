using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace content.installer
{
    class Program
    {
        static void RecursiveCopy(string baseDirectory, string sourceDirectory, string destination)
        {
            foreach (string directory in Directory.GetDirectories(sourceDirectory))
            {
                //Console.WriteLine("Scanning {0}", directory);

                int index = baseDirectory.LastIndexOf('\\');
                string relativePath = directory.Substring(index, directory.Length - index);

                string destPath = string.Format("{0}{1}", destination, relativePath);
                if (!Directory.Exists(destPath))
                {
                    Console.WriteLine("CreateDirectory {0}", destPath);
                    Directory.CreateDirectory(destPath);
                }

                RecursiveCopy(baseDirectory, directory, destination);

                foreach (string file in Directory.GetFiles(directory))
                {
                    string filename = Path.GetFileName(file);
                    string target = string.Format("{0}\\{1}", destPath, filename);


                    Console.Write(".");

                    File.Copy(file, target, true);

                    Console.WriteLine("Copied {0}", target);
                }
            }
        }

        static void Main(string[] args)
        {
            if (args.Length < 2)
                return;

            // Copy all content files to passed in path.
            Console.WriteLine("INSTALLING CONTENT: {0} ===> {1}", args[0], args[1]);            
            string destination = string.Format("{0}\\{1}", args[1], "content");
            if (!Directory.Exists(destination))
            {
                Directory.CreateDirectory(destination);
            }

            RecursiveCopy(args[0], args[0], args[1]);
            Console.WriteLine("\nDONE");
        }
    }
}
