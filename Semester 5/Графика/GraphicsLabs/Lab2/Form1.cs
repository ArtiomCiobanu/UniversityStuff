using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Lab2
{
    public partial class Form1 : Form
    {
        private Dictionary<string, int> _data = new Dictionary<string, int>();

        private Point _leftPoint = new Point(90, 350);
        private Point _topPoint = new Point(150, 320);
        private Point _rightPoint = new Point(470, 350);
        private Point _bottomPoint = new Point(440, 385);

        public Form1()
        {
            InitializeComponent();
        }

        protected override void OnLoad(EventArgs e)
        {
            base.OnLoad(e);

            using (StreamReader streamReader = new StreamReader("Data.txt"))
            {
                while (!streamReader.EndOfStream)
                {
                    var line = streamReader.ReadLine();

                    var words = line.Split();
                    _data.Add(words[0], int.Parse(words[1]));
                }
            }
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            base.OnPaint(e);

            DrawBackgroundLines(e.Graphics);
            DrawValues(e.Graphics);


            /*Pen pen = new Pen(Color.FromArgb(255, 0, 0, 0), 2);
            var brush = new SolidBrush(Color.Yellow);*/


            /*e.Graphics.DrawPolygon(pen, new PointF[4]
            {
                new PointF(100, 100),
                new PointF(200, 100),
                new PointF(200, 200),
                new PointF(150, 200)
            });*/

            /*e.Graphics.FillPolygon(brush, new PointF[4]
            {
                new PointF(100, 100),
                new PointF(200, 100),
                new PointF(200, 200),
                new PointF(150, 200)
            });*/

            /*e.Graphics.DrawClosedCurve(pen, new PointF[3] 
            {
                new PointF(200, 300),
                new PointF(400, 500),
                new PointF(700, 600)
            });*/

            /*e.Graphics.FillClosedCurve(brush, new PointF[4]
            {
                new PointF(100, 100),
                new PointF(200, 100),
                new PointF(200, 200),
                new PointF(150, 200)
            });*/

            /*e.Graphics.draw
            e.Graphics.DrawArc(pen, new Rectangle(100, 100, 300, 300), 200, 90);*/


            /*var region = new Region();
            region.Union();
           
            e.Graphics.FillRegion(brush);*/

            // Create location and size of ellipse.
            /*float x = 0.0F;
            float y = 0.0F;
            float width = 200.0F;
            float height = 100.0F;

            // Create start and sweep angles.
            float startAngle = 0.0F;
            float sweepAngle = 45.0F;

            // Draw pie to screen.
            e.Graphics.DrawPie(pen, x, y, width, height, startAngle, sweepAngle);*/
        }

        private void DrawBackgroundLines(Graphics graphics)
        {
            Pen pen = new Pen(Color.SlateGray, 2);
            var brush = new SolidBrush(Color.Black);

            for (int value = 0; value <= 80; value += 10)
            {
                var currentLeftPoint = _leftPoint;
                var currentMiddlePoint = _topPoint;
                var currentRightPoint = _rightPoint;

                int correctedValue = value * 2;
                currentLeftPoint.Y -= correctedValue;
                currentMiddlePoint.Y -= correctedValue;
                currentRightPoint.Y -= correctedValue;

                graphics.DrawLine(pen, currentLeftPoint, currentMiddlePoint);
                graphics.DrawLine(pen, currentMiddlePoint, currentRightPoint);

                var font = new Font(Font.FontFamily, 10);

                var fontPoint = currentLeftPoint;
                fontPoint.X -= (int)(font.Size * 2.5);
                fontPoint.Y -= 10;

                graphics.DrawString(value.ToString(), font, brush, fontPoint);
            }

            graphics.DrawLine(pen, _leftPoint, _bottomPoint);
            graphics.DrawLine(pen, _bottomPoint, _rightPoint);
        }

        private void DrawValues(Graphics graphics)
        {
            var fontBrush = new SolidBrush(Color.Black);
            var brush = new SolidBrush(Color.Blue);
            var darkBrush = new SolidBrush(Color.DarkBlue);

            var length = GetDistance(_leftPoint, _bottomPoint);

            var pointAmount = _data.Count;
            var distance = length / pointAmount;


            var font = new Font(Font.FontFamily, 10);

            var beginningLeftPoint = _leftPoint;
            beginningLeftPoint.X += 20;
            beginningLeftPoint.Y -= 20;

            var previousTopPoint = beginningLeftPoint;
            var previousBottomPoint = beginningLeftPoint;
            var previousDarkPartPoint = previousTopPoint;

            int currentIndex = 0;
            foreach (var (key, value) in _data)
            {
                var currentX = currentIndex * distance;
                var currentY = currentIndex * 7;

                var currentLabelPoint = beginningLeftPoint;
                currentLabelPoint.X += (int)currentX;
                currentLabelPoint.Y += currentY + 5;

                graphics.DrawString(key, font, fontBrush, currentLabelPoint);


                var currentTopY = beginningLeftPoint.Y - value * 2;
                var currentTopPoint = new Point(currentLabelPoint.X, currentTopY);

                var currentBottomPoint = previousBottomPoint;
                currentBottomPoint.Y += 2;

                var currentDarkPartPoint = currentTopPoint;
                currentDarkPartPoint.X += 10;
                currentDarkPartPoint.Y += 20;

                /*graphics.FillPolygon(
                    darkBrush,
                    new[]
                    {
                        previousTopPoint,
                        previousDarkPartPoint,
                        currentDarkPartPoint,
                        currentTopPoint
                    });*/


                graphics.FillPolygon(
                    brush,
                    new[]
                    {
                        previousBottomPoint,
                        previousTopPoint,
                        currentTopPoint,
                        currentLabelPoint
                    });

                previousTopPoint = currentTopPoint;
                previousBottomPoint = currentBottomPoint;

                previousDarkPartPoint = currentDarkPartPoint;

                currentIndex++;
            }
        }

        private double GetDistance(Point point1, Point point2)
        {
            var height = point2.Y - point1.Y;
            var width = point2.X - point1.X;

            var value = Math.Pow(height, 2) + Math.Pow(width, 2);

            return Math.Sqrt(value);
        }
    }
}
