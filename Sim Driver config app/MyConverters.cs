using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Data;

namespace Sim_Driver_config_app
{

    public class SliderConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            if (parameter != null && parameter.ToString() == "%")
            {
                double val = System.Convert.ToDouble(value);
                return parameter.ToString() + (Math.Round((double)val / 655.35, 1)).ToString();
            }
            else return (Math.Round((double)value, 3)).ToString();

        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            string val = value.ToString();
            if (value != null && targetType == typeof(double))
            {
                return double.Parse(val);
            }
            else return 0;
        }
    }
}
