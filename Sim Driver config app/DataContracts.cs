using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Serialization;
using System.Text;
using System.Threading.Tasks;

namespace Sim_Driver_config_app
{
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
