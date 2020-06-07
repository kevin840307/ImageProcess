using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace IVCVisualization
{
    public partial class ImageForm : Form
    {
        public ImageForm()
        {
            InitializeComponent();
        }

        public ImageForm(Bitmap image, string title)
        {
            InitializeComponent();
            this.Text = title;
            pic_image.Image = image;
            lab_data.Text = "寬："+image.Width.ToString() + " 高："+ image.Height.ToString();
        }
    }
}
