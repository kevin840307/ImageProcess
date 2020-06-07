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
    public partial class Form1 : Form
    {
        IVCLibrary _lib = new IVCLibrary();
        PerspectiveTransType _transType = PerspectiveTransType.TRANS_NEAREST;
        private static string FILE_NAME = Environment.CurrentDirectory + @"\Lenna.jpg";
        //private static string FILE_NAME = @"D:/test2.bmp";
        //private static string FILE_NAME = @"C:\Users\USER\Desktop\Lenna.jpg";
        private Bitmap _fileImage = null;
        private Bitmap _gridImage = null;
        private Bitmap _nowImage = null;
        private Bitmap _grayImage = null;

        public Form1()
        {
            InitializeComponent();
            this.DoubleBuffered = true;
            SetBitmap(FILE_NAME);

        }

        private void SetBitmap(string path)
        {
            FILE_NAME = path;
            _fileImage = new Bitmap(FILE_NAME);
            _nowImage = new Bitmap(FILE_NAME);
            _grayImage = _lib.Change8BitColor(_fileImage, ColerType.BGR2GRAY_8BIT);
            pic_src.Image = _fileImage;
            InitGrid();
        }

        private void btn_linear_Click(object sender, EventArgs e)
        {
            float zoom = 0.0f;

            if (!float.TryParse(text_room.Text, out zoom))
            {
                MessageBox.Show("請輸入數字");
                return;
            }

            Bitmap bitmap = _lib.Change8BitColor(_nowImage, ColerType.BGR2GRAY_8BIT);
            bitmap = _lib.Resize8bit(bitmap, (int)(bitmap.Width * zoom), (int)(bitmap.Height * zoom), ResizeType.LINEAR);

            pic_process.Image = bitmap;
            if (ch_other_form.Checked)
            {
                new ImageForm(bitmap, "linearImage").Show();
            }
        }

        private void btn_RGB2Gray8_Click(object sender, EventArgs e)
        {
            Bitmap bitmap = _lib.Change8BitColor(_nowImage, ColerType.BGR2GRAY_8BIT);
        }

        private void text_room_KeyPress(object sender, KeyPressEventArgs e)
        {
            //base.OnKeyPress(e);
            //if (!char.IsDigit(e.KeyChar) && !char.IsControl(e.KeyChar))
            //{
            //    e.Handled = true;
            //}
        }

        private void btn_nearest_Click(object sender, EventArgs e)
        {
            float zoom = 0.0f;

            if (!float.TryParse(text_room.Text, out zoom))
            {
                MessageBox.Show("請輸入數字");
                return;
            }

            Bitmap bitmap = _lib.Change8BitColor(_nowImage, ColerType.BGR2GRAY_8BIT);
            bitmap = _lib.Resize8bit(bitmap, (int)(bitmap.Width * zoom), (int)(bitmap.Height * zoom), ResizeType.NEAREST);

            pic_process.Image = bitmap;
            if (ch_other_form.Checked)
            {
                new ImageForm(bitmap, "nearestImage").Show();
            }

        }

        private void btn_bicubic_Click(object sender, EventArgs e)
        {
            float zoom = 0.0f;

            if (!float.TryParse(text_room.Text, out zoom))
            {
                MessageBox.Show("請輸入數字");
                return;
            }

            Bitmap bitmap = _lib.Change8BitColor(_nowImage, ColerType.BGR2GRAY_8BIT);
            bitmap = _lib.Resize8bit(bitmap, (int)(bitmap.Width * zoom), (int)(bitmap.Height * zoom), ResizeType.BICUBIC);

            pic_process.Image = bitmap;
            if (ch_other_form.Checked)
            {
                new ImageForm(bitmap, "bicubicImage").Show();
            }
        }

        PointF2D[,] _points = new PointF2D[5, 5];

        private void InitGrid()
        {
            _points = GetInitPoints();

            //LinkPoint();
        }

        private PointF2D[,] GetInitPoints()
        {
            PointF2D[,] points = new PointF2D[5, 5];
            int fixWidth = _fileImage.Width >> 2;
            int fixHeight = _fileImage.Height >> 2;
            //DrawF draw = new DrawF(79.0f, 5.0f, 5.0f);
            //draw.drawBlock(e.Graphics, 4.0f, 4.0f);
            pic_grid.Controls.Clear();
            for (int col = 0; col < 5; col++)
            {
                float fixCol = fixHeight * col + 10.0f;
                for (int row = 0; row < 5; row++)
                {
                    float fixRow = fixWidth * row + 10.0f;
                    points[col, row] = new PointF2D(fixRow, fixCol);

                    PictureBox pointPic = new PictureBox();
                    pointPic.Location = new Point((int)fixRow - 5, (int)fixCol - 5);
                    pointPic.Size = new Size(13, 13);
                    pointPic.Image = new Bitmap(Environment.CurrentDirectory + @"\red.png");
                    //pointPic.Image = new Bitmap( @"D:\VisualStudio\VisualStudioProject\IVCLibrary\x64\Debug\red.png");
                    pointPic.Name = row + "," + col;
                    pointPic.SizeMode = PictureBoxSizeMode.Zoom;
                    pointPic.Parent = pic_grid;
                    pic_grid.Controls.Add(pointPic);

                    pointPic.MouseMove += new MouseEventHandler(point_pic_MouseMove);
                    pointPic.MouseDown += new MouseEventHandler(point_pic_MouseDown);
                    pointPic.MouseUp += new MouseEventHandler(point_pic_MouseUp);
                    //draw.drawPoint(e.Graphics, Brushes.Red, fixRow, fixCol);
                    //draw.drawImage(e.Graphics, image, fixRow, fixCol);

                }
            }
            return points;
        }

        private void LinkPoint()
        {


            DrawF draw = new DrawF(79.0f, 5.0f, 5.0f);
            BufferedGraphicsContext currentContext = BufferedGraphicsManager.Current;
            BufferedGraphics bufferGraphics = currentContext.Allocate(pic_grid.CreateGraphics(), pic_grid.DisplayRectangle);
            _gridImage = new Bitmap(_fileImage.Width, _fileImage.Height);
            Graphics graphics = bufferGraphics.Graphics;
            Graphics graphicsBMP = Graphics.FromImage(_gridImage);
            graphics.Clear(pic_grid.BackColor);
            graphicsBMP.FillRectangle(Brushes.White, 0, 0, _fileImage.Width, _fileImage.Height);
            for (int col = 0; col < 5; col++)
            {
                for (int row = 0; row < 5; row++)
                {
                    PointF2D point = _points[col, row];
                    float x1 = point.X - 10 == _fileImage.Width ? point.X - 11 : point.X - 10;
                    float y1 = point.Y - 10 == _fileImage.Height ? point.Y - 11 : point.Y - 10;
                    if (row < 4)
                    {
                        PointF2D rightPoint = _points[col, row + 1];
                        float x2 = rightPoint.X - 10 == _fileImage.Width ? rightPoint.X - 11 : rightPoint.X - 10;
                        float y2 = rightPoint.Y - 10 == _fileImage.Height ? rightPoint.Y - 11 : rightPoint.Y - 10;
                        draw.drawLine(graphics, point.X, point.Y, rightPoint.X, rightPoint.Y);
                        draw.drawLine(graphicsBMP, x1, y1, x2, y2, 3);
                    }

                    if (col < 4)
                    {
                        PointF2D bottomPoint = _points[col + 1, row];
                        float x2 = bottomPoint.X - 10 == _fileImage.Width ? bottomPoint.X - 11 : bottomPoint.X - 10;
                        float y2 = bottomPoint.Y - 10 == _fileImage.Height ? bottomPoint.Y - 11 : bottomPoint.Y - 10;
                        draw.drawLine(graphics, point.X, point.Y, bottomPoint.X, bottomPoint.Y);
                        draw.drawLine(graphicsBMP, x1, y1, x2, y2, 3);
                    }
                }
            }
            if (ch_timely.Checked)
            {
                pic_result.Image = _lib.PerspectiveTransform8bit(_grayImage, _points, _transType);
            }
            //int[,] graph = new int[pic_grid.Height, pic_grid.Width];
            //int mark = 1;
            //List<int> links = new List<int>();
            //links.Add(0);
            //for (int row = 1; row < bmpPic.Height - 1; row++)
            //{
            //    for (int col = 1; col < bmpPic.Width - 1; col++)
            //    {
            //        List<int> link = new List<int>();

            //        if (bmpPic.GetPixel(col, row).R != 255)
            //        {
            //            continue;
            //        }

            //        if (graph[row - 1, col - 1] != 0)
            //        {
            //            link.Add(graph[row - 1, col - 1]);
            //        }

            //        if (graph[row - 1, col] != 0)
            //        {
            //            link.Add(graph[row - 1, col]);
            //        }

            //        if (graph[row - 1, col + 1] != 0)
            //        {
            //            link.Add(graph[row - 1, col + 1]);
            //        }

            //        if (graph[row, col - 1] != 0)
            //        {
            //            link.Add(graph[row, col - 1]);
            //        }

            //        if (link.Count == 0)
            //        {
            //            links.Add(mark);
            //            graph[row, col] = mark++;
            //        }
            //        else
            //        {
            //            link.Sort();
            //            int minLink = links[link[0]];
            //            for (int index = 0; index < link.Count; index++)
            //            {
            //                if (minLink < links[link[index]])
            //                {
            //                    links[link[index]] = minLink;
            //                }
            //                else if (minLink > links[link[index]])
            //                {
            //                    links[link[0]] = links[link[index]];
            //                }
            //            }
            //            graph[row, col] = minLink;
            //        }
            //    }
            //}

            //for (int index = 1; index < links.Count(); index++)
            //{
            //    int saveValue = links[index];
            //    int saveValueIndex = index;
            //    while (saveValue != saveValueIndex)
            //    {
            //        saveValue = links[links[saveValueIndex]];
            //        saveValueIndex = links[saveValueIndex];
            //    }
            //    links[index] = saveValue;
            //}
            //HashSet<int> companyTeams = new HashSet<int>();
            //for (int row = 1; row < bmpPic.Height - 1; row++)
            //{
            //    for (int col = 1; col < bmpPic.Width - 1; col++)
            //    {

            //        if (bmpPic.GetPixel(col, row).R != 255)
            //        {
            //            continue;
            //        }
            //        graph[row, col] = links[graph[row, col]];
            //        companyTeams.Add(graph[row, col]);
            //    }
            //}

            //Color[] colors = new Color[companyTeams.Count + 1];
            //Dictionary<int, int> norml = new Dictionary<int, int>();
            //for (int index = 0; index < companyTeams.Count + 1; index++)
            //{
            //    colors[index] = GetColor(index);
            //}
            //norml[0] = 0;
            //int offset = 1;
            //foreach (int data in companyTeams)
            //{
            //    norml[data] = offset;
            //    offset++;
            //}

            //for (int row = 0; row < pic_grid.Height; row++)
            //{
            //    for (int col = 0; col < pic_grid.Width; col++)
            //    {
            //        //bmpPic.SetPixel(col, row, colors[norml[graph[row, col]]]);
            //        if (bmpPic.GetPixel(col, row).R != 0)
            //        {
            //            continue;
            //        }
            //    }
            //}
            //_gridImage.Save(@"D:/test.bmp");
            bufferGraphics.Render(pic_grid.CreateGraphics());
            bufferGraphics.Dispose();

        }

        private Color GetColor(int seed)
        {
            Random random = new Random(seed);
            int pix = random.Next(0, 255);
            return Color.FromArgb(pix, pix, pix);
        }

        bool _gridDown = false;

        private void point_pic_MouseDown(object sender, MouseEventArgs e)
        {
            _gridDown = true;
        }

        private void point_pic_MouseUp(object sender, MouseEventArgs e)
        {
            _gridDown = false;
        }

        private void point_pic_MouseMove(object sender, MouseEventArgs e)
        {
            if (_gridDown)
            {
                int fixWidth = _fileImage.Width >> 2;
                int fixHeight = _fileImage.Height >> 2;

                PictureBox pointPic = (PictureBox)sender;
                string[] name = pointPic.Name.Split(',');
                int row = int.Parse(name[0]);
                int col = int.Parse(name[1]);
                Point point = pointPic.Location;
                int x = point.X + e.X;
                int y = point.Y + e.Y;

                if (Math.Abs(y - col * fixHeight) > (fixHeight >> 1) - 10)
                {
                    return;
                }

                if (Math.Abs(x - row * fixWidth) > (fixWidth >> 1) - 10)
                {
                    return;
                }

                if (y < 10 || y > _fileImage.Height + 10)
                {
                    return;
                }

                if (x < 10 || x > _fileImage.Width + 10)
                {
                    return;
                }
                //Console.WriteLine(pointPic.Name);
                //pointPic.Location = new Point(x, y);
                pointPic.Location = new Point(x - 6, y - 6);

                _points[col, row].X = x;
                _points[col, row].Y = y;
                
            }
        }

        bool _gridInit = true;
        private void pic_grid_Paint(object sender, PaintEventArgs e)
        {
            if (!_gridInit)
            {
                LinkPoint();
                return;
            }

            DrawF draw = new DrawF(79.0f, 5.0f, 5.0f);
            for (int col = 0; col < 5; col++)
            {
                for (int row = 0; row < 5; row++)
                {
                    PointF2D point = _points[col, row];
                    if (row < 4)
                    {
                        PointF2D rightPoint = _points[col, row + 1];
                        draw.drawLine(e.Graphics, point.X, point.Y, rightPoint.X, rightPoint.Y);
                    }

                    if (col < 4)
                    {
                        PointF2D bottomPoint = _points[col + 1, row];
                        draw.drawLine(e.Graphics, point.X, point.Y, bottomPoint.X, bottomPoint.Y);
                    }
                }
            }
            _gridInit = false;
        }

        private void btn_per_tran_Click(object sender, EventArgs e)
        {
            //int zoom = int.Parse(text_room.Text);

            Bitmap bitmap = _lib.Change8BitColor(_nowImage, ColerType.BGR2GRAY_8BIT);
            //Bitmap gridBitmap = _lib.Change8BitColor(_gridImage, ColerType.BGR2GRAY_8BIT);

            bitmap = _lib.PerspectiveTransform8bit(bitmap, _points, _transType);
            pic_process.Image = bitmap;

            if (ch_other_form.Checked)
            {
                new ImageForm(bitmap, "perspectiveTransformImage").Show();
            }

            bitmap = _lib.PerspectiveTransformReverse8bit(bitmap, _points, _transType);
            pic_process2.Image = bitmap;
            if (ch_other_form.Checked)
            {
                new ImageForm(bitmap, "perspectiveTransformReverseImage").Show();
            }

        }

        private void btn_test_Click(object sender, EventArgs e)
        {
            Bitmap bitmap = _lib.Change8BitColor(_nowImage, ColerType.BGR2GRAY_8BIT);
            bitmap = _lib.ContourFrame8bit(bitmap, bitmap.Width, bitmap.Height);

            pic_process.Image = bitmap;
            if (ch_other_form.Checked)
            {
                new ImageForm(bitmap, "testImage").Show();
            }
        }

        private void checkBox2_CheckedChanged(object sender, EventArgs e)
        {
            string type = ((CheckBox)sender).Text;
            if (type == "鄰近" && ((CheckBox)sender).Checked)
            {
                _transType = PerspectiveTransType.TRANS_NEAREST;
                checkBox3.Checked = false;
                checkBox4.Checked = false;
            }
            else if (type == "雙線" && ((CheckBox)sender).Checked)
            {
                _transType = PerspectiveTransType.TRANS_LINEAR;
                checkBox2.Checked = false;
                checkBox3.Checked = false;
            }
            else if (((CheckBox)sender).Checked)
            {
                _transType = PerspectiveTransType.TRANS_BICUBIC;
                checkBox2.Checked = false;
                checkBox4.Checked = false;
            }
        }

        private void btn_open_Click(object sender, EventArgs e)
        {
            OpenFileDialog dialog = new OpenFileDialog();
            dialog.Title = "Select file";
            dialog.InitialDirectory = ".\\";
            dialog.Filter = "圖檔 (*.bmp)|*.bmp|圖檔 (*.jpg)|*.jpg|圖檔 (*.png)|*.png|所有檔案 (*.*)|*.*";
            if (dialog.ShowDialog() == DialogResult.OK)
            {
                SetBitmap(dialog.FileName);
            }
        }
    }
}
