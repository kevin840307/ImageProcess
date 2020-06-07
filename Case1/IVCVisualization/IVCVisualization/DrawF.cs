using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace IVCVisualization
{
    class DrawF
    {
        PointF2D _center;
        float _offset;

        public DrawF(float offset)
        {
            _center = new PointF2D(5.0f * offset, 5.0f * offset);
            _offset = offset;
        }

        public DrawF(float offset, float x, float y)
        {
            _center = new PointF2D(x * offset, y * offset);
            _offset = offset;
        }

        public PointF2D getBlockPoint(float x, float y)
        {
            PointF2D point = new PointF2D();
            point.X = (_center.X / _offset + x) * _offset;
            point.Y = (_center.Y / _offset - y) * _offset;
            return point;
        }

        public PointF2D Center
        {
            get
            {
                return _center;
            }
        }

        public void drawBlock(Graphics graphics
                            , float x, float y)
        {
            Pen pen = new Pen(Color.FromArgb(255, 0, 0, 0), 1);
            float xLen = x * _offset + 10;
            float yLen = y * _offset + 10;

            // 水平的分割出垂直數量
            for (int row = 0; row <= x; row++)
            {
                graphics.DrawLine(pen, 10, row * _offset + 10, yLen, row * _offset + 10);
            }

            // 垂直的分割出水平數量
            for (int col = 0; col <= y; col++)
            {
                graphics.DrawLine(pen, col * _offset + 10, 10, col * _offset + 10, xLen);
            }
        }

        public void drawLine(Graphics graphics
                            , float xS, float yS
                            , float xE, float yE)
        {
            Pen pen = new Pen(Color.FromArgb(255, 0, 0, 0), 5);
            graphics.DrawLine(pen, xS, yS, xE, yE);
        }

        public void drawLine(Graphics graphics
                           , float xS, float yS
                           , float xE, float yE
                           , int size)
        {
            Pen pen = new Pen(Color.FromArgb(255, 0, 0, 0), size);
            graphics.DrawLine(pen, xS, yS, xE, yE);
        }

        public void drawPoint(Graphics graphics, Brush color
                            , float x, float y)
        {
            graphics.FillRectangle(color, x - 5.0f, y - 5.0f, 10.0f, 10.0f);
        }

        public void drawImage(Graphics graphics, Image image,float x, float y)
        {
            graphics.DrawImage(image, x - 5.0f, y - 5.0f);
        }

        public void drawPointLine(Graphics graphics, Brush color
                                , float x, float y)
        {
            graphics.FillRectangle(color, x, y, 2.0f, 2.0f);
        }

        // drawGradient畫一個梯度
        // point: 座標
        // U: x向量
        // V: y向量
        // maxSum: 最大向量和
        public void drawGradient(Graphics graphics
                                , PointF2D point
                                , float U, float V, float maxSum)
        {
            // 鄰邊 = U = x
            // 對邊 = V = y
            float absU = Math.Abs(U);
            float absV = Math.Abs(V);

            // atan取弧度(tan = 對邊 / 鄰邊
            float radian = absU != 0 ? (float)Math.Atan(absV / absU) : (float)(Math.PI * 0.5);
            Console.WriteLine(radian / Math.PI * 180);

            // cos = 鄰邊 / 斜邊, sin = 對邊 / 斜邊
            float xCos = U != 0.0f ? (float)Math.Cos(radian) : 1.0f;
            float ySin = V != 0.0f ? (float)Math.Sin(radian) : 0.0f;

            float xDirection = U < 0.0f ? -1.0f : U > 0 ? 1.0f : 0.0f;
            float yDirection = V < 0.0f ? 1.0f : V > 0 ? -1.0f : 0.0f;


            // 計算顯示長度比例
            float max = absU > absV ? absU : absV;
            float maxLen = (max / maxSum * _offset);

            for (float index = 0; index < maxLen; index += 0.01f)
            {
                // 取得斜邊
                // 取得x + 方向和y + 方向(對邊)
                float bevel = index / xCos;
                float x = index * xDirection;
                float y = ySin * bevel * yDirection;

                if (Math.Abs(y) > maxLen)
                {
                    return;
                }

                drawPointLine(graphics, Brushes.Black, point.X + x, point.Y + y);
            }
        }
    }
}
