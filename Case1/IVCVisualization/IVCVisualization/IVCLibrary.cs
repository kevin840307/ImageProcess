using System;
using System.Collections.Generic;
using System.Drawing;
using System.Drawing.Imaging;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace IVCVisualization
{
    public enum ColerType : int
    {
        BGR2GRAY_8BIT = 0,
        BGR2HSV = 1,
        HSV2BGR = 2,
        BGR2YCbCr = 3,
        YCbCr2BGR = 4
    }

    public enum ResizeType : int
    {
        NEAREST = 0,
        LINEAR = 1,
        BICUBIC = 2
    }

    public enum PerspectiveTransType : int
    {
        TRANS_NEAREST = 0,
        TRANS_LINEAR = 1,
        TRANS_BICUBIC = 2
    }

    class IVCLibrary
    {
        //private const string DLL_PATH = @"D:\VisualStudio\VisualStudioProject\IVCLibrary\x64\Debug\IVCLibrary.dll";
        //private const string DLL_PATH = @"D:\VisualStudio\VisualStudioProject\IVCLibrary\x64\Release\IVCLibrary.dll";
        private const string DLL_PATH = @"IVCLibrary.dll";
        private ColorPalette _colorPalette = null;

        public IVCLibrary()
        {
            using (Bitmap bitmap = new Bitmap(1, 1, PixelFormat.Format8bppIndexed))
            {
                _colorPalette = bitmap.Palette;
            }

            for (int i = 0; i < 256; i++)
            {
                _colorPalette.Entries[i] = Color.FromArgb(i, i, i);
            }
        }

        [DllImport(DLL_PATH)]
        unsafe private static extern void ivcChangeColor(IntPtr src, IntPtr pur
                                                                , int width, int height
                                                                , int type);

        public Bitmap Change8BitColor(Bitmap srcImage, ColerType type)
        {
            Bitmap purImage = new Bitmap(srcImage.Width, srcImage.Height, PixelFormat.Format8bppIndexed);
            Rectangle size = new Rectangle(0, 0, srcImage.Width, srcImage.Height);
            BitmapData srcData = srcImage.LockBits(size, ImageLockMode.ReadOnly, PixelFormat.Format24bppRgb);
            BitmapData purData = purImage.LockBits(size, ImageLockMode.ReadWrite, PixelFormat.Format8bppIndexed);

            unsafe
            {
                IntPtr srcPtr = srcData.Scan0;
                IntPtr purPtr = purData.Scan0;
                ivcChangeColor(srcPtr, purPtr, srcImage.Width, srcImage.Height, (int)type);
            }

            purImage.Palette = _colorPalette;
            srcImage.UnlockBits(srcData);
            purImage.UnlockBits(purData);

            return purImage;
        }

        [DllImport(DLL_PATH)]
        unsafe private static extern void ivcResize8bit(IntPtr src, IntPtr pur
            , int width, int height
            , int reWidth, int reHeight
            , int type);

        public Bitmap Resize8bit(Bitmap srcImage, int reWidth, int reHeight, ResizeType type)
        {
            Bitmap purImage = new Bitmap(reWidth, reHeight, PixelFormat.Format8bppIndexed);
            Rectangle size = new Rectangle(0, 0, srcImage.Width, srcImage.Height);
            BitmapData srcData = srcImage.LockBits(size, ImageLockMode.ReadOnly, PixelFormat.Format8bppIndexed);
            BitmapData purData = purImage.LockBits(new Rectangle(0, 0, reWidth, reHeight), ImageLockMode.ReadWrite, PixelFormat.Format8bppIndexed);

            unsafe
            {
                IntPtr srcPtr = srcData.Scan0;
                IntPtr purPtr = purData.Scan0;

                ivcResize8bit(srcPtr, purPtr
                    , srcImage.Width, srcImage.Height
                    , reWidth, reHeight
                    , (int)type);
            }

            purImage.Palette = _colorPalette;
            srcImage.UnlockBits(srcData);
            purImage.UnlockBits(purData);
            return purImage;
        }


        [DllImport(DLL_PATH)]
        unsafe private static extern void ivcContourFrame8bit(IntPtr src, IntPtr pur
        , int width, int height);

        public Bitmap ContourFrame8bit(Bitmap srcImage, int reWidth, int reHeight)
        {
            Bitmap purImage = new Bitmap(reWidth, reHeight, PixelFormat.Format8bppIndexed);
            Rectangle size = new Rectangle(0, 0, srcImage.Width, srcImage.Height);
            BitmapData srcData = srcImage.LockBits(size, ImageLockMode.ReadOnly, PixelFormat.Format8bppIndexed);
            BitmapData purData = purImage.LockBits(new Rectangle(0, 0, reWidth, reHeight), ImageLockMode.ReadWrite, PixelFormat.Format8bppIndexed);

            unsafe
            {
                IntPtr srcPtr = srcData.Scan0;
                IntPtr purPtr = purData.Scan0;

                ivcContourFrame8bit(srcPtr, purPtr
                    , srcImage.Width, srcImage.Height);
            }

            purImage.Palette = _colorPalette;
            srcImage.UnlockBits(srcData);
            purImage.UnlockBits(purData);
            return purImage;
        }

        [DllImport(DLL_PATH)]
        unsafe private static extern void ivcPerspectiveTransform8bit(IntPtr src, IntPtr pur
            , int width, int height
            , float[] points
            , int type);

        public Bitmap PerspectiveTransform8bit(Bitmap srcImage, PointF2D[,] points, PerspectiveTransType type)
        {
            Bitmap purImage = new Bitmap(srcImage.Width, srcImage.Height, PixelFormat.Format8bppIndexed);
            Rectangle size = new Rectangle(0, 0, srcImage.Width, srcImage.Height);
            BitmapData srcData = srcImage.LockBits(size, ImageLockMode.ReadOnly, PixelFormat.Format8bppIndexed);
            BitmapData purData = purImage.LockBits(size, ImageLockMode.ReadWrite, PixelFormat.Format8bppIndexed);

            unsafe
            {
                float[] linkPoints = new float[50];
                for (int row = 0; row < 5; row++)
                {
                    for (int col = 0; col < 10; col += 2)
                    {
                        int fixIndex = row * 10 + col;

                        linkPoints[fixIndex] = points[row, (col >> 1)].X == srcImage.Width + 10 ? points[row, (col >> 1)].X - 11 : points[row, (col >> 1)].X - 10;
                        linkPoints[fixIndex + 1] = points[row, (col >> 1)].Y == srcImage.Height + 10 ? points[row, (col >> 1)].Y - 11 : points[row, (col >> 1)].Y - 10;
                    }
                }
                IntPtr srcPtr = srcData.Scan0;
                IntPtr purPtr = purData.Scan0;
                ivcPerspectiveTransform8bit(srcPtr, purPtr
                    , srcImage.Width, srcImage.Height
                    , linkPoints
                    , (int)type);
            }

            purImage.Palette = _colorPalette;
            srcImage.UnlockBits(srcData);
            purImage.UnlockBits(purData);
            return purImage;
        }

        [DllImport(DLL_PATH)]
        unsafe private static extern void ivcPerspectiveTransformReverse8bit(IntPtr src, IntPtr pur
            , int width, int height
            , float[] points
            , int type);

        public Bitmap PerspectiveTransformReverse8bit(Bitmap srcImage, PointF2D[,] points, PerspectiveTransType type)
        {
            Bitmap purImage = new Bitmap(srcImage.Width, srcImage.Height, PixelFormat.Format8bppIndexed);
            Rectangle size = new Rectangle(0, 0, srcImage.Width, srcImage.Height);
            BitmapData srcData = srcImage.LockBits(size, ImageLockMode.ReadOnly, PixelFormat.Format8bppIndexed);
            BitmapData purData = purImage.LockBits(size, ImageLockMode.ReadWrite, PixelFormat.Format8bppIndexed);

            unsafe
            {
                float[] linkPoints = new float[50];
                for (int row = 0; row < 5; row++)
                {
                    for (int col = 0; col < 10; col += 2)
                    {
                        int fixIndex = row * 10 + col;

                        linkPoints[fixIndex] = points[row, (col >> 1)].X == srcImage.Width + 10 ? points[row, (col >> 1)].X - 11 : points[row, (col >> 1)].X - 10;
                        linkPoints[fixIndex + 1] = points[row, (col >> 1)].Y == srcImage.Height + 10 ? points[row, (col >> 1)].Y - 11 : points[row, (col >> 1)].Y - 10;
                    }
                }
                IntPtr srcPtr = srcData.Scan0;
                IntPtr purPtr = purData.Scan0;
                ivcPerspectiveTransformReverse8bit(srcPtr, purPtr
                    , srcImage.Width, srcImage.Height
                    , linkPoints
                    , (int)type);
            }

            purImage.Palette = _colorPalette;
            srcImage.UnlockBits(srcData);
            purImage.UnlockBits(purData);
            return purImage;
        }
    }
}
