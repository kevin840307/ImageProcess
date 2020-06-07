using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace IVCVisualization
{
    class PointF2D
    {
        private float _x;
        private float _y;

        public PointF2D(float x, float y)
        {
            _x = x;
            _y = y;
        }

        public PointF2D()
        {
            _x = 0;
            _y = 0;
        }

        public float X
        {
            get
            {
                return _x;
            }
            set
            {
                _x = value;
            }
        }

        public float Y
        {
            get
            {
                return _y;
            }
            set
            {
                _y = value;
            }
        }
    }
}
