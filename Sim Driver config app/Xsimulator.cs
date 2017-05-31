using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace Sim_Driver_config_app
{
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
}
