using System;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace SolarPanellnaspectionWindows
{
    public partial class Form1 : Form
    {
        private bool _run = false;
        private string _originalFolderPath = Environment.CurrentDirectory + @"\output\";
        private string _originalListPath = Environment.CurrentDirectory + @"\secret_plan.txt";
        private string _screenshotFolderPath = Environment.CurrentDirectory + @"\Data\";
        private string _screenshotListPath = Environment.CurrentDirectory + @"\secret_plan2.txt";
        private string _detectionFolderPath = Environment.CurrentDirectory + @"\SData\";
        private string _imagePath = "";
        private string _imageOutPath = Environment.CurrentDirectory + @"\Test\";
        private string _screenshotPath = "";
        private Bitmap _bitmap = null;
        private string _exePath = Environment.CurrentDirectory + @"\SolarPanelInspection.exe";
        public Form1()
        {
            InitializeComponent();
        }

        private string GetImagePath(string filename)
        {
            string path = _imageOutPath + filename + ".bmp";
            if(path == _imagePath)
            {
                path = _imageOutPath + filename + "1" + ".bmp";
            }
            return path;
        }

        private void OpenFolder(string path)
        {
            System.Diagnostics.Process prc = new System.Diagnostics.Process();
            prc.StartInfo.FileName = path;
            prc.Start();
        }

        private string RunProcess(string arg)
        {
            if (_run)
            {
                return "運行中請稍後";
            }
            _run = true;

            ProcessStartInfo processStartInfo = new ProcessStartInfo();
            processStartInfo.FileName = _exePath;
            processStartInfo.Arguments = arg;
            processStartInfo.WindowStyle = ProcessWindowStyle.Hidden;
            processStartInfo.CreateNoWindow = true;
            processStartInfo.UseShellExecute = false;
            processStartInfo.RedirectStandardOutput = true;

            Process process = new Process();
            process.StartInfo = processStartInfo;
            process.Start();

            string output = process.StandardOutput.ReadToEnd();
            _run = false;

            return output;
        }

        private async void Detection(int type, string filename)
        {
            if (_screenshotPath.Length == 0)
            {
                MessageBox.Show("請載入截圖");
                return;
            }
            string purPath = GetImagePath(filename);
            string arg = "Detection " + _screenshotPath + " " + purPath + " " + type;
            string output = await Task.Factory.StartNew(() => RunProcess(arg));
            if (output == "1")
            {
                SetNowBitmap(purPath);
            }
        }

        public static ArrayList TraverseTree(string root)
        {
            ArrayList list = new ArrayList();
            // Data structure to hold names of subfolders to be
            // examined for files.
            Stack<string> dirs = new Stack<string>(20);

            if (!System.IO.Directory.Exists(root))
            {
                throw new ArgumentException();
            }
            dirs.Push(root);

            while (dirs.Count > 0)
            {
                string currentDir = dirs.Pop();
                string[] subDirs;
                try
                {
                    subDirs = System.IO.Directory.GetDirectories(currentDir);
                }
                // An UnauthorizedAccessException exception will be thrown if we do not have
                // discovery permission on a folder or file. It may or may not be acceptable 
                // to ignore the exception and continue enumerating the remaining files and 
                // folders. It is also possible (but unlikely) that a DirectoryNotFound exception 
                // will be raised. This will happen if currentDir has been deleted by
                // another application or thread after our call to Directory.Exists. The 
                // choice of which exceptions to catch depends entirely on the specific task 
                // you are intending to perform and also on how much you know with certainty 
                // about the systems on which this code will run.
                catch (UnauthorizedAccessException e)
                {
                    Console.WriteLine(e.Message);
                    continue;
                }
                catch (System.IO.DirectoryNotFoundException e)
                {
                    Console.WriteLine(e.Message);
                    continue;
                }

                string[] files = null;
                try
                {
                    files = System.IO.Directory.GetFiles(currentDir);
                }

                catch (UnauthorizedAccessException e)
                {

                    Console.WriteLine(e.Message);
                    continue;
                }

                catch (System.IO.DirectoryNotFoundException e)
                {
                    Console.WriteLine(e.Message);
                    continue;
                }
                // Perform the required action on each file here.
                // Modify this block to perform your required task.
                foreach (string file in files)
                {
                    try
                    {
                        list.Add(file);
                        // Perform whatever action is required in your scenario.
                        //System.IO.FileInfo fi = new System.IO.FileInfo(file);
                        //Console.WriteLine("{0}: {1}, {2}", fi.Name, fi.Length, fi.CreationTime);
                    }
                    catch (System.IO.FileNotFoundException e)
                    {
                        // If file was deleted by a separate application
                        //  or thread since the call to TraverseTree()
                        // then just continue.
                        Console.WriteLine(e.Message);
                        continue;
                    }
                }

                // Push the subdirectories onto the stack for traversal.
                // This could also be done before handing the files.
                foreach (string str in subDirs)
                    dirs.Push(str);
            }

            return list;
        }

        private void SetNowBitmap(string path)
        {
            _imagePath = path;
            _bitmap = new Bitmap(_imagePath);
            pictureBox1.Image = _bitmap;
        }

        private async void btn_screenshot_Click(object sender, EventArgs e)
        {
            if(_imagePath.Length == 0)
            {
                MessageBox.Show("請載入圖片");
                return;
            }
            string purPath = GetImagePath("screenshot");
            string arg = "Screenshot " + _imagePath + " " + purPath;
            string output = await Task.Factory.StartNew(() => RunProcess(arg));
            if(output == "1")
            {
                _screenshotPath = purPath;
                SetNowBitmap(purPath);
            }
        }

        private void btn_open_Click(object sender, EventArgs e)
        {
            OpenFileDialog dialog = new OpenFileDialog();
            dialog.Title = "Select file";
            dialog.InitialDirectory = ".\\";
            dialog.Filter = "bmp files (*.*)|*.bmp";
            if (dialog.ShowDialog() == DialogResult.OK)
            {
                SetNowBitmap(dialog.FileName);
            }
        }

        private void btn_line_Click(object sender, EventArgs e)
        {
            Detection(1, "line");
        }

        private void btn_point_Click(object sender, EventArgs e)
        {
            Detection(2, "point");
        }

        private void btn_stain_Click(object sender, EventArgs e)
        {
            Detection(4, "stain");
        }

        private void btn_detection_Click(object sender, EventArgs e)
        {
            Detection(7, "detection");
            
        }

        private async void btn_screenshot_all_Click(object sender, EventArgs e)
        {
            ArrayList list = TraverseTree(_originalFolderPath);
            using (StreamWriter sw = new StreamWriter(_originalListPath))
            {
                for (int index = 0; index < list.Count; index++)
                {
                    sw.WriteLine(list[index]);
                }
            }

            string arg = "ScreenshotAll " + _originalListPath + " " + _screenshotFolderPath;
            string output = await Task.Factory.StartNew(() => RunProcess(arg));
            if (output == "1")
            {
                OpenFolder(_screenshotFolderPath);
            }
        }

        private async void btn_detection_all_Click(object sender, EventArgs e)
        {
            ArrayList list = TraverseTree(_screenshotFolderPath);
            using (StreamWriter sw = new StreamWriter(_screenshotListPath))
            {
                for (int index = 0; index < list.Count; index++)
                {
                    sw.WriteLine(list[index]);
                }
            }

            string arg = "DetectionAll " + _screenshotListPath + " " + _detectionFolderPath;
            string output = await Task.Factory.StartNew(() => RunProcess(arg));
            if (output == "1")
            {
                OpenFolder(_detectionFolderPath);
            }
        }

        private async void btn_kmean_train_Click(object sender, EventArgs e)
        {
            string _kmeanImagePath = Environment.CurrentDirectory + @"\KMean\3.bmp";
            string _kmeanPath = Environment.CurrentDirectory + @"\KMean\kmean.txt";
            string arg = "MeanTrain " + _kmeanImagePath + " " + _kmeanPath + " 12 3 100";
            string output = await Task.Factory.StartNew(() => RunProcess(arg));
            if (output == "1")
            {
                OpenFolder(_kmeanPath);
            }
        }

        private async void btn_kmean_predict_Click(object sender, EventArgs e)
        {
            string _kmeanImagePath = Environment.CurrentDirectory + @"\KMean\1.bmp";
            string _kmeanPath = Environment.CurrentDirectory + @"\KMean\kmean.txt";
            string _saveImagePath = Environment.CurrentDirectory + @"\KMean\2.bmp";
            string arg = "MeanPredict " + _kmeanPath + " " + _kmeanImagePath + " " + _saveImagePath + " " + Form2._feature;
            string output = await Task.Factory.StartNew(() => RunProcess(arg));
            if (output == "1")
            {
                OpenFolder(_saveImagePath);
                //SetNowBitmap(_saveImagePath);
            }
        }

        private void btn_kmean_setting_Click(object sender, EventArgs e)
        {
            new Form2().Show();
        }
    }
}
