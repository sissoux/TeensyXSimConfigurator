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

namespace Sim_Driver_config_app
{
    /// <summary>
    /// Logique d'interaction pour MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        SerialPort myPort = new SerialPort();
        string SimDriverFirmwareVersion = "2.00";
        volatile string SerialBuffer;
        volatile Xsimulator mySim = new Xsimulator();

        public MainWindow()
        {
            InitializeComponent();
            this.DataContext = this;
            DataDisplay.DataContext = mySim;
            MotorDisplay1.DataContext = mySim.Motors[0];
            MotorDisplay2.DataContext = mySim.Motors[1];
            MainSerial.Port = myPort;
            myPort.DataReceived += new SerialDataReceivedEventHandler(MyDataReceivedHandler);
        }

        void MyDataReceivedHandler(object sender, SerialDataReceivedEventArgs e)
        {
            int count = myPort.BytesToRead;
            char[] InBuffer = new char[count];
            myPort.Read(InBuffer, 0, count);
            SerialBuffer = SerialBuffer + new String(InBuffer);
            bool EOF_Detected = false;
            foreach (char c in InBuffer)
            {
                if (c == '\r')
                {
                    EOF_Detected = true;
                    break;
                }
            }
            if (EOF_Detected) handleJson();
        }

        private void handleJson()
        {
            char[] separator = { '\r', '\n' };
            String[] temp = SerialBuffer.Split(separator);

            if (temp.Length > 1)
            {
                for (int i = 1; i < temp.Length; i++)
                {
                    SerialBuffer += temp[i];
                }
            }
            else SerialBuffer = null;

            InputBuffer IncomingObject = new InputBuffer();
            try
            {
                IncomingObject = JsonConvert.DeserializeObject<InputBuffer>(temp[0]);

            }
            catch (Exception e)
            {
                //StatusDisplay.Text = e.Message;
            }


            switch (IncomingObject.ReceivedCommand)
            {
                case null:
                    break;
                case "BoardInfo":
                    newBoardInfo(IncomingObject);
                    break;
            }
        }

        private void newBoardInfo(InputBuffer sender)
        {
            try
            {
                mySim.BoardName = "Board name: " + sender.BoardName;
                mySim.FirmRevision = "Firmware revision: " + sender.FirmwareRevision;
                mySim.BoardRevision = "Board revision: " + sender.BoardRevision;
            }
            catch (Exception e)
            {
                //StatusDisplay.Text = e.Message;
            }
        }

        private void UpdateGraph()
        {
            string json = "{\"Cmd\":\"Capture\",\"Size\":10,\"Buffer\":[{\"T\":0,\"RawPosition\":32767,\"Output\":128,\"ClampedSetPoint\":32767},{\"T\":10000,\"RawPosition\":32867,\"Output\":141,\"ClampedSetPoint\":36661},{\"T\":20000,\"RawPosition\":32867,\"Output\":134,\"ClampedSetPoint\":39941},{\"T\":30000,\"RawPosition\":32867,\"Output\":139,\"ClampedSetPoint\":42087},{\"T\":40000,\"RawPosition\":32867,\"Output\":135,\"ClampedSetPoint\":42763},{\"T\":50000,\"RawPosition\":32767,\"Output\":137,\"ClampedSetPoint\":41860},{\"T\":60000,\"RawPosition\":32867,\"Output\":135,\"ClampedSetPoint\":39522},{\"T\":70000,\"RawPosition\":32767,\"Output\":135,\"ClampedSetPoint\":36117},{\"T\":80000,\"RawPosition\":32867,\"Output\":134,\"ClampedSetPoint\":32183},{\"T\":90000,\"RawPosition\":32767,\"Output\":134,\"ClampedSetPoint\":28342},{\"T\":100000,\"RawPosition\":32867,\"Output\":132,\"ClampedSetPoint\":25199},{\"T\":110000,\"RawPosition\":32767,\"Output\":133,\"ClampedSetPoint\":23251},{\"T\":120000,\"RawPosition\":32867,\"Output\":130,\"ClampedSetPoint\":22805},{\"T\":130000,\"RawPosition\":32767,\"Output\":130,\"ClampedSetPoint\":23932},{\"T\":140000,\"RawPosition\":32867,\"Output\":129,\"ClampedSetPoint\":26454},{\"T\":150000,\"RawPosition\":32767,\"Output\":128,\"ClampedSetPoint\":29973},{\"T\":160000,\"RawPosition\":32867,\"Output\":127,\"ClampedSetPoint\":33932},{\"T\":170000,\"RawPosition\":32767,\"Output\":126,\"ClampedSetPoint\":37708},{\"T\":180000,\"RawPosition\":32867,\"Output\":125,\"ClampedSetPoint\":40704},{\"T\":190000,\"RawPosition\":32767,\"Output\":124,\"ClampedSetPoint\":42446},{\"T\":200000,\"RawPosition\":32867,\"Output\":123,\"ClampedSetPoint\":42661},{\"T\":210000,\"RawPosition\":32767,\"Output\":122,\"ClampedSetPoint\":41313},{\"T\":220000,\"RawPosition\":32867,\"Output\":121,\"ClampedSetPoint\":38616},{\"T\":230000,\"RawPosition\":32767,\"Output\":120,\"ClampedSetPoint\":34996},{\"T\":240000,\"RawPosition\":32867,\"Output\":120,\"ClampedSetPoint\":31024},{\"T\":250000,\"RawPosition\":32767,\"Output\":119,\"ClampedSetPoint\":27327},{\"T\":260000,\"RawPosition\":32867,\"Output\":118,\"ClampedSetPoint\":24489},{\"T\":270000,\"RawPosition\":32767,\"Output\":118,\"ClampedSetPoint\":22958},{\"T\":280000,\"RawPosition\":32767,\"Output\":118,\"ClampedSetPoint\":22975},{\"T\":290000,\"RawPosition\":32767,\"Output\":117,\"ClampedSetPoint\":24539},{\"T\":300000,\"RawPosition\":32767,\"Output\":117,\"ClampedSetPoint\":27401},{\"T\":310000,\"RawPosition\":42967,\"Output\":116,\"ClampedSetPoint\":31111},{\"T\":320000,\"RawPosition\":38367,\"Output\":118,\"ClampedSetPoint\":35082},{\"T\":330000,\"RawPosition\":41267,\"Output\":116,\"ClampedSetPoint\":38688},{\"T\":340000,\"RawPosition\":39567,\"Output\":117,\"ClampedSetPoint\":41359},{\"T\":350000,\"RawPosition\":40567,\"Output\":118,\"ClampedSetPoint\":42673},{\"T\":360000,\"RawPosition\":39967,\"Output\":117,\"ClampedSetPoint\":42424},{\"T\":370000,\"RawPosition\":40267,\"Output\":119,\"ClampedSetPoint\":40650},{\"T\":380000,\"RawPosition\":40067,\"Output\":118,\"ClampedSetPoint\":37631},{\"T\":390000,\"RawPosition\":40167,\"Output\":120,\"ClampedSetPoint\":33845},{\"T\":400000,\"RawPosition\":40167,\"Output\":120,\"ClampedSetPoint\":29888},{\"T\":410000,\"RawPosition\":40167,\"Output\":121,\"ClampedSetPoint\":26386},{\"T\":420000,\"RawPosition\":40167,\"Output\":122,\"ClampedSetPoint\":23891},{\"T\":430000,\"RawPosition\":40167,\"Output\":122,\"ClampedSetPoint\":22798},{\"T\":440000,\"RawPosition\":40067,\"Output\":124,\"ClampedSetPoint\":23279},{\"T\":450000,\"RawPosition\":40167,\"Output\":125,\"ClampedSetPoint\":25257},{\"T\":460000,\"RawPosition\":40067,\"Output\":125,\"ClampedSetPoint\":28421},{\"T\":470000,\"RawPosition\":40167,\"Output\":127,\"ClampedSetPoint\":32272},{\"T\":480000,\"RawPosition\":40067,\"Output\":127,\"ClampedSetPoint\":36200},{\"T\":490000,\"RawPosition\":40167,\"Output\":129,\"ClampedSetPoint\":39587}]}";

            InputBuffer test = new InputBuffer();
            test = JsonConvert.DeserializeObject<InputBuffer>(json);
            
               List<PointCollection> series = new List<PointCollection>(3);
            series.Add(new PointCollection(test.Size));
            series.Add(new PointCollection(test.Size));
            series.Add(new PointCollection(test.Size));
            foreach (var value in test.Buffer)
            {
                series[0].Add(new Point(0, value.Output));
                series[1].Add(new Point(0, value.RawPosition));
                series[2].Add(new Point(0, value.ClampedSetPoint));
            }
            MyXYGraph.addToGraph(series[0]);
            MyXYGraph.addToGraph(series[1]);
            MyXYGraph.addToGraph(series[2]);
        }

        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            UpdateGraph();
        }

        private void Send_button_Click(object sender, RoutedEventArgs e)
        {
            if (myPort.IsOpen)
            {
                Command myCommand = new Command("getBoardInfo");


                MemoryStream stream1 = new MemoryStream();
                DataContractJsonSerializer ser = new DataContractJsonSerializer(typeof(Command));
                ser.WriteObject(stream1, myCommand);
                stream1.Position = 0;
                StreamReader sr = new StreamReader(stream1);
                string mastring = sr.ReadToEnd();
                mastring += '\r';
                myPort.Write(mastring);
            }
        }
        
    }

    public class SliderConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            if (parameter != null && parameter.ToString() == "%")
            {
                double val = System.Convert.ToDouble(value);
                return (Math.Round((double)val /655.35 , 1)).ToString() + parameter.ToString();
            }
            else return (Math.Round((double)value, 2)).ToString();

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

[DataContract]
public class Command
{
    [DataMember]
    internal string Cmd;

    [DataMember]
    internal byte Id;

    [DataMember]
    UInt16 Span;

    public Command(string Cmd)
    {
        this.Cmd = Cmd;
    }
    
}

[DataContract]
public class CaptureData
{
    [DataMember]
    internal UInt32 T;

    [DataMember]
    internal UInt16 RawPosition;

    [DataMember]
    internal byte Output;

    [DataMember]
    internal UInt16 ClampedSetPoint;

}

[DataContract]
public class MotorInfo
{
    [DataMember(Name ="ID",IsRequired =true)]
    internal byte ID;

    [DataMember(Name ="Position",IsRequired =true)]
    internal UInt16 RawPosition;

    [DataMember(Name = "kp")]
    internal double kp;

    [DataMember(Name = "ki")]
    internal double ki;

    [DataMember(Name = "kd")]
    internal double kd;

}

[DataContract]
public class InputBuffer
{
    [DataMember(Name = "Cmd", IsRequired = true)]
    internal string ReceivedCommand;

    [DataMember]
    internal UInt16 Size;

    [DataMember(Name = "BN")]
    internal string BoardName;

    [DataMember(Name = "FirmRev")]
    internal string FirmwareRevision;

    [DataMember(Name = "BoardRev")]
    internal string BoardRevision;

    [DataMember(Name = "Temp")]
    internal double Temperature;

    [DataMember(Name = "Buffer")]
    internal List<CaptureData> Buffer = new List<CaptureData>();

    [DataMember(Name = "MotorInfo")]
    internal List<MotorInfo> MInfo = new List<MotorInfo>();

}

public class Xsimulator : INotifyPropertyChanged
{
    public event PropertyChangedEventHandler PropertyChanged;
    public List<Motor> Motors = new List<Motor>(2);

    public Xsimulator()
    {
        Motors.Add(new Motor(1));
        Motors.Add(new Motor(2));
    }

    private string boardName = "Board Name:";

    public string BoardName
    {
        get { return boardName; }
        set
        {
            if (value == boardName) return;
            boardName = value;
            NotifyPropertyChanged();
        }
    }

    private string firmRevision = "Firmware revision:";

    public string FirmRevision
    {
        get { return firmRevision; }
        set
        {
            if (value == firmRevision) return;
            firmRevision = value;
            NotifyPropertyChanged();
        }
    }

    private string boardRevision = "Board revision:";

    public string BoardRevision
    {
        get { return boardRevision; }
        set
        {
            if (value == boardRevision) return;
            boardRevision = value;
            NotifyPropertyChanged();
        }
    }


    // This method is called by the Set accessor of each property.
    // The CallerMemberName attribute that is applied to the optional propertyName
    // parameter causes the property name of the caller to be substituted as an argument.
    private void NotifyPropertyChanged([CallerMemberName] String propertyName = "")
    {
        if (PropertyChanged != null)
        {
            PropertyChanged(this, new PropertyChangedEventArgs(propertyName));
        }
    }



}
