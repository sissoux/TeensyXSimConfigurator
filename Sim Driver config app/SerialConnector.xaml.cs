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
using System.IO.Ports;

namespace Sim_Driver_config_app
{
    /// <summary>
    /// Logique d'interaction pour SerialConnector.xaml
    /// </summary>
    public partial class SerialConnector : UserControl
    {


        public SerialPort Port;
        public String[] COMPortList
        {
            get { return SerialPort.GetPortNames(); }
            set { }
        }
        public String SelectedPort { get; set; }
        
        private void UpdateDropList(object sender, EventArgs e)
        {
            ((ComboBox)sender).ItemsSource = COMPortList;
        }

        private void connectionChange(object sender, EventArgs e)
        {
            if (SelectedPort != null)
            {
                if (!Port.IsOpen)
                {
                    try
                    {
                        Port.PortName = SelectedPort;
                        Port.Open();
                        Connect_button.Content = "Disconnect";
                        ConnectionStatusDisplay.Text = "Connected";
                        ConnectionStatusDisplay.Foreground = Brushes.Green;
                    }
                    catch (Exception exc)
                    {
                        ConnectionStatusDisplay.Text = exc.Message;
                    }
                }
                else
                {
                    try
                    {
                        Port.Close();
                        Connect_button.Content = "Connect";
                        ConnectionStatusDisplay.Text = "Not connected";
                        ConnectionStatusDisplay.Foreground = Brushes.Red;
                    }
                    catch (Exception exc)
                    {
                        ConnectionStatusDisplay.Text = exc.Message;
                    }

                }
            }
            else
            {
                ConnectionStatusDisplay.Text = "Select COM port first.";
                ConnectionStatusDisplay.Foreground = Brushes.Red;
            }
        }

        public SerialConnector()
        {
            InitializeComponent();
        }

        private void PortSelected(object sender, SelectionChangedEventArgs e)
        {
            SelectedPort = ((ComboBox)sender).SelectedItem.ToString();
        }
    }
}
