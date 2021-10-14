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
        private Dictionary<string, int> _sunchartData = new Dictionary<string, int>();

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

            using (StreamReader streamReader = new StreamReader("SunchartData.txt"))
            {
                while (!streamReader.EndOfStream)
                {
                    var line = streamReader.ReadLine();

                    var words = line.Split();
                    _sunchartData.Add(words[0], int.Parse(words[1]));
                }
            }
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            base.OnPaint(e);

            DrawFirstChart(e.Graphics);
            DrawSecondChart(e.Graphics);
        }

        private void DrawSecondChart(Graphics graphics)
        {
            var pen = new Pen(Color.Black, 2);

            var rightRectangle = new Rectangle(new Point(850, 100), new Size(100, 170));
            graphics.DrawRectangle(pen, rightRectangle);

            var rightPoint = new Point(860, 110);

            var mainRectangle = new Rectangle(new Point(600, 100), new Size(200, 200));

            var colors = new[]
            {
                Color.Red,
                Color.Blue,
                Color.Green,
                Color.Yellow,
                Color.BlueViolet
            };

            float previous = 0;
            var colorIndex = 0;
            foreach (var (key, value) in _sunchartData)
            {
                var coloredBrush = new SolidBrush(colors[colorIndex]);

                var angle = (float)3.6 * value;
                graphics.DrawPie(pen, mainRectangle, previous, angle);
                graphics.FillPie(coloredBrush, mainRectangle, previous, angle);


                var currentColoredPoint = rightPoint;
                currentColoredPoint.Y += colorIndex * 35;

                var coloredRectangle = new Rectangle(currentColoredPoint, new Size(10, 10));
                graphics.DrawRectangle(pen, coloredRectangle);
                graphics.FillRectangle(coloredBrush, coloredRectangle);

                var labelPoint = currentColoredPoint;
                labelPoint.X += 20;
                labelPoint.Y -= 5;
                graphics.DrawString(key, Font, new SolidBrush(Color.Black), labelPoint);

                previous += angle;
                colorIndex++;
            }

            var innerRectangle = new Rectangle(new Point(650, 150), new Size(100, 100));
            graphics.FillEllipse(new SolidBrush(BackColor), innerRectangle);
            graphics.DrawEllipse(pen, innerRectangle);
        }

        private void DrawFirstChart(Graphics graphics)
        {
            DrawBackgroundLines(graphics);
            DrawValues(graphics);
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

            var length = GetDistance(_leftPoint, _bottomPoint);

            var pointAmount = _data.Count;
            var distance = length / pointAmount;


            var font = new Font(Font.FontFamily, 10);

            var beginningLeftPoint = _leftPoint;
            beginningLeftPoint.X += 20;
            beginningLeftPoint.Y -= 20;

            var previousTopPoint = beginningLeftPoint;
            var previousBottomPoint = beginningLeftPoint;

            int currentIndex = 0;
            foreach (var (key, value) in _data)
            {
                var currentX = currentIndex * distance;
                var currentY = currentIndex * 7;

                var currentPoint = beginningLeftPoint;
                currentPoint.X += (int)currentX;
                currentPoint.Y += currentY + 5;

                var labelPoint = currentPoint;
                labelPoint.Y += 20;

                graphics.DrawString(key, font, fontBrush, labelPoint);


                var currentTopY = beginningLeftPoint.Y - value * 2;
                var currentTopPoint = new Point(currentPoint.X, currentTopY);

                var currentBottomPoint = previousBottomPoint;
                currentBottomPoint.Y += 2;

                graphics.FillPolygon(
                    brush,
                    new[]
                    {
                        previousBottomPoint,
                        previousTopPoint,
                        currentTopPoint,
                        currentPoint
                    });

                previousTopPoint = currentTopPoint;
                previousBottomPoint = currentBottomPoint;

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
