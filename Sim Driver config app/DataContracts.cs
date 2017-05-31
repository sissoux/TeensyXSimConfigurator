using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Serialization;
using System.Text;
using System.Threading.Tasks;

namespace Sim_Driver_config_app
{

    [DataContract]
    public class Command
    {
        [DataMember]
        internal string Cmd;

        [DataMember]
        internal byte Id;

        [DataMember]
        UInt16 Span;

        [DataMember]
        internal List<MotorInfo> MotorInfoList = new List<MotorInfo>();

        public Command(string Cmd)
        {
            this.Cmd = Cmd;
        }

        public Command(string Cmd, List<Motor> MotorList)
        {
            this.Cmd = Cmd;
            if (MotorList.Count == 2)
            {
                MotorInfo m1 = new MotorInfo();
                MotorInfo m2 = new MotorInfo();
                
                this.MotorInfoList.Add(m1);
                this.MotorInfoList.Add(m2);

                for (int i = 0; i<2; i++)
                {
                    MotorInfoList[i].ID = MotorList[i].ID;
                    MotorInfoList[i].kp = MotorList[i].kp;
                    MotorInfoList[i].ki = MotorList[i].ki;
                    MotorInfoList[i].kd = MotorList[i].kd;
                    MotorInfoList[i].target = MotorList[i].TargetInt;
                    MotorInfoList[i].HighLimit = MotorList[i].HighLimit;
                    MotorInfoList[i].LowLimit = MotorList[i].LowLimit;
                    MotorInfoList[i].Offset = MotorList[i].Offset;
                }
            }
            
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
        [DataMember(Name = "ID", IsRequired = true)]
        internal byte ID;

        [DataMember(Name = "Position", IsRequired = true)]
        internal UInt16 RawPosition;

        [DataMember(Name = "kp")]
        internal double kp;

        [DataMember(Name = "ki")]
        internal double ki;

        [DataMember(Name = "kd")]
        internal double kd;

        [DataMember(Name = "target")]
        internal UInt16 target;

        [DataMember(Name = "HL")]
        internal UInt16 HighLimit;

        [DataMember(Name = "LL")]
        internal UInt16 LowLimit;

        [DataMember(Name = "Offset")]
        internal UInt16 Offset;
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
}
