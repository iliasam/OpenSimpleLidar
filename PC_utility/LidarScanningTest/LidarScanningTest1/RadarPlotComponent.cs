using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace LidarScanningTest1
{
    public partial class RadarPlotComponent : UserControl
    {
        int BOX_WIDTH = 400;

        Bitmap img;
        Graphics g;

        int cur_radius = 350;//in cm

        public RadarPlotComponent()
        {
            InitializeComponent();

            cmbRadiusList.Items.Add((double)3.5);
            cmbRadiusList.Items.Add((double)1.0);
            cmbRadiusList.Items.Add((double)4.0);
            cmbRadiusList.Items.Add((double)5.0);
            cmbRadiusList.SelectedIndex = 0;
        }

        public void DrawRadar(RadarPoint[] points)
        {
            int i = 0;

            
            BOX_WIDTH = GetImageSize();
            img = new Bitmap(BOX_WIDTH, BOX_WIDTH);

            Pen lines_pen;
            
            Pen yellow_pen = new System.Drawing.Pen(Color.FromArgb(200, 255, 255, 0), 2);
            Pen black_pen = new System.Drawing.Pen(Color.FromArgb(255, 0, 0, 0), 2);//black

            g = Graphics.FromImage(img);

            g.SmoothingMode = System.Drawing.Drawing2D.SmoothingMode.AntiAlias;
            if (chkBlackMode.Checked)
            {
                lines_pen = new System.Drawing.Pen(Color.LightGray, 1);
                g.FillRectangle(new SolidBrush(Color.FromArgb(0, 0, 0)), 0, 0, BOX_WIDTH, BOX_WIDTH);
            }
            else
            {
                lines_pen = new System.Drawing.Pen(Color.BlueViolet, 1);
                g.FillRectangle(new SolidBrush(Color.FromArgb(0, 0, 200)), 0, 0, BOX_WIDTH, BOX_WIDTH);
            }


            //main lines
            g.DrawLine(lines_pen, 0, BOX_WIDTH / 2, BOX_WIDTH, BOX_WIDTH / 2);
            g.DrawLine(lines_pen, BOX_WIDTH / 2, 0, BOX_WIDTH / 2, BOX_WIDTH);

            if (cmbRadiusList.SelectedItem != null)
                cur_radius = (int)((double)cmbRadiusList.SelectedItem * 100);//to cm

            for (UInt16 radius = 100; radius < 600; radius += 100)
            {
                draw_circle(g, lines_pen, BOX_WIDTH / 2 * radius / cur_radius);
            }

            
            //Scan points
            for (i = 0; i < 360; i++)
            {
                Pen cur_pen;
                if (points[i].black == true)
                    cur_pen = black_pen;//black
                else
                    cur_pen = yellow_pen;

                draw_xy_point(cur_pen, points[i].x, points[i].y);
            }

            int angle = trackBar1.Value / 2 * 2;
            draw_centerd_line(yellow_pen, angle);


            pictureBox1.Image = img;
        }

        //radius - in pixels
        void draw_circle(Graphics g, Pen cur_pen, int radius)
        {
            g.DrawEllipse(cur_pen, BOX_WIDTH / 2 - radius, BOX_WIDTH / 2 - radius, radius * 2, radius * 2);
        }

        /// <summary>
        /// Draw oint
        /// </summary>
        /// <param name="cur_pen"></param>
        /// <param name="x">Position in meters</param>
        /// <param name="y">Position in meters</param>
        void draw_xy_point(Pen cur_pen, double x, double y)
        {
            double xx;
            double yy;

            xx = x / (((double)cur_radius / 100)) * ((double)(BOX_WIDTH / 2));
            yy = y / (((double)cur_radius / 100)) * ((double)(BOX_WIDTH / 2));

            g.DrawEllipse(cur_pen, (float)(BOX_WIDTH / 2 + xx), (float)(BOX_WIDTH / 2 - yy), 1 * 2, 1 * 2);
        }

        void draw_centerd_line(Pen cur_pen, double angle_deg)
        {
            float length = 1000;//cm

            float x_start = (float)(BOX_WIDTH / 2);
            float y_start = (float)(BOX_WIDTH / 2);

            angle_deg += 90;

            float angle_rad = (float)(angle_deg * Math.PI / 180);
            float x_end = (float)(length * Math.Cos(angle_rad));
            float y_end = (float)(length * Math.Sin(angle_rad));

            x_end = x_end / (((float)cur_radius / 100)) * ((float)(BOX_WIDTH / 2));//pixels from center
            y_end = y_end / (((float)cur_radius / 100)) * ((float)(BOX_WIDTH / 2));//pixels from center

            x_end = x_start + x_end;
            y_end = y_start - y_end;

            g.DrawLine(cur_pen, x_start, y_start, x_end, y_end);
        }

        int GetImageSize()
        {
            int height = pictureBox1.Height;
            int width = pictureBox1.Width;

            int min_val = Math.Min(height, width);
            return min_val;
        }

        private void trackBar1_Scroll(object sender, EventArgs e)
        {
            int angle = trackBar1.Value / 2 * 2;
            lblPointerAnle.Text = "Pointer angle, deg: " + angle.ToString();
        }

        public int GetPointerAngle()
        {
            int angle = trackBar1.Value / 2 * 2;
            return angle;
        }
    }
}
