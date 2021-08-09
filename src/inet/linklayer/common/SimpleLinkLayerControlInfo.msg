
import inet.linklayer.common.MACAddress;

namespace inet;

class SimpleLinkLayerControlInfo
{
    @customize(true);

    MACAddress src;  // can be left empty when sending
    MACAddress dest;
    int interfaceId;
}
