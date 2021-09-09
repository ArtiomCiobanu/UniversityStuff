﻿
using System;
using System.Drawing;
using System.Windows.Forms;

namespace TestGraphicsApp
{
    /// <summary>
    /// Вариант 3а
    /// </summary>
    partial class Form1
    {
        double _e = 0.01;

        /// <summary>
        ///  Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        ///  Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        ///  Required method for Designer support - do not modify
        ///  the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(800, 450);
            this.Text = "Form1";
        }

        #endregion

        protected override void OnPaint(PaintEventArgs e)
        {
            base.OnPaint(e);

            Pen pen = new Pen(Color.FromArgb(255, 0, 0, 0), 2);

            var xCenter = Width / 2;
            var yCenter = Height / 2;

            e.Graphics.DrawLine(pen, 0, yCenter, Width, yCenter);
            e.Graphics.DrawLine(pen, xCenter, 0, xCenter, Height);

            DrawFunction(x => F(x), e.Graphics);
        }

        private void DrawFunction(Func<double, double> function, Graphics graphics)
        {
            Pen pen = new Pen(Color.FromArgb(255, 0, 0, 0), 2);

            var xCenter = Width / 2;
            var yCenter = Height / 2;

            double x, y;

            double a = -10;
            double b = 10;

            double s = 0.00001;

            /*double scx = 100.0;
            double scy = 10.0;*/

            //double e = 0.0001;

            double multiplier = 2;

            x = a;

            while (x <= b)
            {
                y = function(x);
                x += s;

                int currentX = xCenter + (int)(x * multiplier);
                int currentY = yCenter - (int)(y * multiplier); //Чтобы график был над Ox

                graphics.FillRectangle(pen.Brush, currentX, currentY, 1, 1);
            }
        }

        private double F(double x)
        {
            var halfPi = Math.PI / 2;

            double result = halfPi;
            double difference = 1;

            int k = 0;

            while (Math.Abs(difference) >= _e)
            {
                var denominator = (2 * k + 1);
                var numerator = Power(x, denominator);

                difference = Power(-1, k + 1) * (numerator / denominator);
                result += difference;

                k++;
            }

            return result;
        }

        private double Power(double number, double power)
        {
            double result = number;

            for (int i = 1; i < power; i++)
            {
                result *= number;
            }

            return result;
        }
    }
}

