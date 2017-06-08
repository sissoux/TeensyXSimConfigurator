using System;
using System.Collections.Generic;
using System.Globalization;
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
using System.Runtime.Serialization;
using System.Runtime.Serialization.Json;
using Newtonsoft.Json;
using System.IO.Ports;
using System.IO;
using System.ComponentModel;
using System.Runtime.CompilerServices;
using Sim_Driver_config_app;
using System.Threading.Tasks;
using System.Threading;

namespace Sim_Driver_config_app
{
    /// <summary>
    /// Logique d'interaction pour MainWindow.xaml
    /// </summary>
    
    public partial class MainWindow : Window
    {
        SerialPort myPort = new SerialPort();
        volatile static List<string> SerialBuffer = new List<string>();
        volatile static int NewFullFrameFlag = 0;
        volatile Xsimulator mySim = new Xsimulator();
        private bool NewConnection = true;
        //int Counter = 0;
        List<CaptureData> CapturedDataBuffer = new List<CaptureData>();

        public MainWindow()
        {
            InitializeComponent();
            this.DataContext = this;
            DataDisplay.DataContext = mySim;
            MotorDisplay1.DataContext = mySim.Motors[0];
            MotorDisplay2.DataContext = mySim.Motors[1];
            LiveCheckbox.DataContext = mySim;
            StatusDisplay.DataContext = this;
            MainSerial.Port = myPort;
            myPort.DataReceived += new SerialDataReceivedEventHandler(MyDataReceivedHandler);

            var dueTime = TimeSpan.FromMilliseconds(100);
            var interval = TimeSpan.FromMilliseconds(100);
            RunPeriodicAsync(OnTick, dueTime, interval, CancellationToken.None);

        }

        private void OnTick()
        {

            if (myPort.IsOpen && NewConnection)
            {
                NewConnection = false;
                Command myCommand = new Command("getBoardInfo");
                sendCommand(myCommand);
            }
            while (NewFullFrameFlag>0)  //Parse every frame in buffer, FIFO
            {
                NewFullFrameFlag--;
                try
                {
                    handleJson();
                }
                catch (Exception e)
                {
                    StatusDisplay.Text = e.Message;
                }
            }
            if (mySim.LiveMode)
            {
                if (mySim.Motors.Exists(Motor => Motor.NewPosition))
                {
                    Command myCommand = new Command("setPosition");
                    sendCommand(myCommand);
                    mySim.Motors.ForEach(Motor => Motor.NewPosition = false);
                }
            }
        }

        // The `onTick` method will be called periodically unless cancelled.
        private static async Task RunPeriodicAsync(Action onTick, TimeSpan dueTime, TimeSpan interval, CancellationToken token)
        {
            // Initial wait time before we begin the periodic loop.
            if (dueTime > TimeSpan.Zero)
                await Task.Delay(dueTime, token);

            // Repeat this loop until cancelled.
            while (!token.IsCancellationRequested)
            {
                // Call our onTick function.
                onTick?.Invoke();

                // Wait to repeat again.
                if (interval > TimeSpan.Zero)
                    await Task.Delay(interval, token);
            }
        }

        private static void MyDataReceivedHandler(object sender, SerialDataReceivedEventArgs e)
        {
            SerialPort sp = sender as SerialPort;
            while(sp.BytesToRead>0)
            {
                SerialBuffer.Add(sp.ReadLine());
                NewFullFrameFlag++;
            }
        }

        private void handleJson()
        {
            if (SerialBuffer.Count > 0)
            {
                string JSONFrame = SerialBuffer[0];

                InputBuffer IncomingObject = new InputBuffer();
                try
                {
                    IncomingObject = JsonConvert.DeserializeObject<InputBuffer>(JSONFrame);

                    switch (IncomingObject.ReceivedCommand)
                    {
                        case null:
                            break;
                        case "BoardInfo":
                            newBoardInfo(IncomingObject);
                            break;
                        case "NewCapture":
                            CapturedDataBuffer.Clear();
                            foreach (var dataSet in IncomingObject.Buffer)
                            {
                                CapturedDataBuffer.Add(dataSet);
                            }
                            break;
                        case "Capture":
                            foreach (var dataSet in IncomingObject.Buffer)
                            {
                                CapturedDataBuffer.Add(dataSet);
                            }
                            if (IncomingObject.FrameNumber == IncomingObject.NumberOfFrame)
                            {
                                UpdateGraph(CapturedDataBuffer);
                            }
                            break;
                        default:
                            throw new Exception("Received Json is not a valid object");
                    }

                }
                catch (Exception e)
                {
                    //Log failed to parse frame
                    throw e;
                }
                finally
                {
                    SerialBuffer.RemoveAt(0);
                }

            }
            else throw new Exception("Nothing To Parse");
        }

        private void newBoardInfo(InputBuffer sender)
        {
            try
            {
                mySim.BoardName = sender.BoardName;
                mySim.FirmRevision = sender.FirmwareRevision;
                mySim.BoardRevision = sender.BoardRevision;
            }
            catch (Exception e)
            {
                //StatusDisplay.Text = e.Message;
            }
        }

        private void UpdateGraph(List<CaptureData> DataToDisplay)
        {
            List<PointCollection> series = new List<PointCollection>(3);
            series.Add(new PointCollection(DataToDisplay.Count));
            series.Add(new PointCollection(DataToDisplay.Count));
            series.Add(new PointCollection(DataToDisplay.Count));
            foreach (var value in DataToDisplay)
            {
                //series[0].Add(new Point(0, value.Output));
                //series[1].Add(new Point(0, value.RawPosition));
                series[2].Add(new Point(0, value.ClampedSetPoint));
            }
            MyXYGraph.cleanGraph();
            //MyXYGraph.addToGraph(series[0]);
            //MyXYGraph.addToGraph(series[1]);
            MyXYGraph.addToGraph(series[2]);
        }

        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            //UpdateGraph();
        }

        private void ButtonClick(object sender, RoutedEventArgs e)
        {
           
            var button = sender as Button;
            Motor ActiveMotor = ((Motor)button.DataContext);
            if (button.CommandParameter != null)
            {
                bool NewParam = true;
                Command command = null;
                switch (button.CommandParameter)
                {
                    case "HL":
                        ActiveMotor.HighLimit = ActiveMotor.TargetInt;
                        break;

                    case "LL":
                        ActiveMotor.LowLimit = ActiveMotor.TargetInt;
                        break;

                    case "Offset":
                        ActiveMotor.Offset = ActiveMotor.TargetInt;
                        break;
                    case "StepCapture":
                        NewParam = false;
                        command = new Command("getClampedSetPoint");
                        break;
                    default:
                        NewParam = false;
                        break;
                }
                if ( NewParam ) command = new Command("setMotorParameters", mySim.Motors);
                sendCommand(command);
            }
        }

        private void sendCommand(Command CMD)
        {
            if (myPort.IsOpen)
            {
                MemoryStream stream1 = new MemoryStream();
                DataContractJsonSerializer ser = new DataContractJsonSerializer(typeof(Command));
                ser.WriteObject(stream1, CMD);
                stream1.Position = 0;
                StreamReader sr = new StreamReader(stream1);
                string mastring = sr.ReadToEnd();
                mastring += '\r';
                myPort.Write(mastring);
            }
        }
    }

}