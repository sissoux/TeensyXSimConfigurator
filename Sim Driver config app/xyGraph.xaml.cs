using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace Sim_Driver_config_app
{
    /// <summary>
    /// Logique d'interaction pour xyGraph.xaml
    /// </summary>
    public partial class xyGraph : UserControl
    {
        Brush[] BrushTable = { Brushes.Red, Brushes.Green , Brushes.Blue, Brushes.Violet, Brushes.Yellow, Brushes.Magenta, Brushes.Gray, Brushes.Black, Brushes.Orange, Brushes.Lime };
        int BrushCounter = 0;

        public xyGraph()
        {
            InitializeComponent();
            initGraph();
        }

        private void initGraph()
        {
            const double margin = 10;
            double xmin = margin;
            double xmax = canGraph.Width - margin;
            double ymax = canGraph.Height - margin;
            double step = 10;

            // Make the X axis.
            GeometryGroup xaxis_geom = new GeometryGroup();
            xaxis_geom.Children.Add(new LineGeometry(
                new Point(0, ymax), new Point(canGraph.Width, ymax)));
            for (double x = xmin + step; x <= canGraph.Width - step; x += step)
            {
                xaxis_geom.Children.Add(new LineGeometry(
                    new Point(x, ymax - margin / 2),
                    new Point(x, ymax + margin / 2)));
            }

            System.Windows.Shapes.Path xaxis_path = new System.Windows.Shapes.Path();
            xaxis_path.StrokeThickness = 1;
            xaxis_path.Stroke = Brushes.Black;
            xaxis_path.Data = xaxis_geom;

            canGraph.Children.Add(xaxis_path);

            // Make the Y ayis.
            GeometryGroup yaxis_geom = new GeometryGroup();
            yaxis_geom.Children.Add(new LineGeometry(
                new Point(xmin, 0), new Point(xmin, canGraph.Height)));
            for (double y = step; y <= canGraph.Height - step; y += step)
            {
                yaxis_geom.Children.Add(new LineGeometry(
                    new Point(xmin - margin / 2, y),
                    new Point(xmin + margin / 2, y)));
            }

            System.Windows.Shapes.Path yaxis_path = new System.Windows.Shapes.Path();
            yaxis_path.StrokeThickness = 1;
            yaxis_path.Stroke = Brushes.Black;
            yaxis_path.Data = yaxis_geom;

            canGraph.Children.Add(yaxis_path);
        }
        
        public void addToGraph(PointCollection points)
        {
            if (points != null)
            {
                double maxVal = points[0].Y;
                UInt32 Sum = 0;
                UInt16 NumOfPoints = 0;
                foreach (var point in points)
                {
                    maxVal = Math.Max(maxVal, point.Y);
                    NumOfPoints++;
                    Sum += (UInt16)point.Y;
                }
                double average = Sum / NumOfPoints;
                if (2 * average >= maxVal) maxVal = 2 * average;
                maxVal += 10;   //Add some margin for max display
                double ratio = (canGraph.Height - 10) / maxVal;
                double xstep = (canGraph.Width - 10) / points.Count;

                for (var index = 0; index < points.Count; index++)
                {
                    points[index] = new Point((xstep * index) + 10, canGraph.Height - (points[index].Y * ratio) - 10);
                }
                Polyline polyline = new Polyline();
                polyline.StrokeThickness = 1;
                polyline.Stroke = BrushTable[BrushCounter];
                BrushCounter = (BrushCounter + 1) % BrushTable.Length;
                polyline.Points = points;

                canGraph.Children.Add(polyline);
            }
            else return;
        }


        public void cleanGraph()
        {
            //canGraph.Children.Remove<Polyline>();
            BrushCounter = 0;
            List<Polyline> toBeRemoved = new List<Polyline>();

            foreach (UIElement Child in canGraph.Children)
            {
                if (Child.GetType() == typeof(Polyline)) toBeRemoved.Add((Polyline)Child);
            }

            for (int i = 0; i < toBeRemoved.Count; i++)
            {
                canGraph.Children.Remove(toBeRemoved[i]);
            }
        }
    }
}
