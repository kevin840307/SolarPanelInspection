using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace SolarPanellnaspectionWindows
{
    public partial class Form2 : Form
    {
        public static int _feature = 0xfff;

        public Form2()
        {
            InitializeComponent();
            InitCheckBox();
        }

        private void InitCheckBox()
        {
            int feature = 1;
            for (int index = 0; index < 12; index++)
            {
                featureboxs.Items.Add(index);
                if ((_feature & feature) == feature)
                {
                    featureboxs.SetItemChecked(index, true);
                }
                feature <<= 1;
            }
        }

        private void btn_send_Click(object sender, EventArgs e)
        {
            SetFeatureValue();
            this.Close();
        }

        private void SetFeatureValue()
        {
            int sumFeature = 0;
            int feature = 1;
            for (int index = 0; index < featureboxs.Items.Count; index++)
            {
                if(featureboxs.GetItemChecked(index))
                {
                    sumFeature |= feature;
                }
                feature <<= 1;
            }
            _feature = sumFeature;
        }

        private void btn_cancel_Click(object sender, EventArgs e)
        {
            this.Close();
        }
    }
}
