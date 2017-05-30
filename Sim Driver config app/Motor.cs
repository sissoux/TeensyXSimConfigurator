﻿using System;
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

        private byte _ID = 100;
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

        private double _kp = 100;
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

        private double _ki = 10;
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

        private double _kd = 0;
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

        public event PropertyChangedEventHandler PropertyChanged;

        public Motor(byte motorID)
        {
            this.ID = motorID;
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