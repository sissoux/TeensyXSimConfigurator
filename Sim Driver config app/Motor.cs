using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Runtime.CompilerServices;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Sim_Driver_config_app
{
    public class Motor : INotifyPropertyChanged
    {

        private byte _ID = 0;
        public byte ID
        {
            get { return _ID; }
            set
            {
                if (value == _ID) return;
                _ID = value;
                NotifyPropertyChanged();
            }
        }

        private double _kp = 10;
        public double kp
        {
            get { return _kp; }
            set
            {
                if (value == _kp) return;
                _kp = value;
                NotifyPropertyChanged();
            }
        }

        private double _ki = 5;
        public double ki
        {
            get { return _ki; }
            set
            {
                if (value == _ki) return;
                _ki = value;
                NotifyPropertyChanged();
            }
        }

        private double _kd = 2;
        public double kd
        {
            get { return _kd; }
            set
            {
                if (value == _kd) return;
                _kd = value;
                NotifyPropertyChanged();
            }
        }

        private UInt16 _RawPosition = 0;
        public UInt16 RawPosition
        {
            get { return _RawPosition; }
            set
            {
                if (value == _RawPosition) return;
                _RawPosition = value;
                NotifyPropertyChanged();
            }
        }

        private UInt16 _Target = 65353/2;
        public Double Target
        {
            get { return _Target; }
            set
            {
                if (value == _Target) return;
                _Target = Convert.ToUInt16(value);
                NotifyPropertyChanged();
            }
        }
        public UInt16 TargetInt
        {
            get { return _Target; }
        }

        public UInt16 HighLimit { get; set; }

        public UInt16 LowLimit { get; set; }

        public UInt16 Offset { get; set; }

        public event PropertyChangedEventHandler PropertyChanged;

        public Motor(byte motorID)
        {
            this.ID = motorID;
            HighLimit = 65535;
            LowLimit = 0;
            Offset = 65535 / 2;
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
}
