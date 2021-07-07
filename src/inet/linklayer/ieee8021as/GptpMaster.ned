//
// @authors: Enkhtuvshin Janchivnyambuu
//           Henning Puttnies
//           Peter Danielis
//           University of Rostock, Germany
// 

package inet.linklayer.ieee8021as;

module GptpMaster extends GptpEndstation
{
    parameters:
        @display("i=device/pc2");
        forwarding = default(false);  // disable routing by default
        networkLayer.proxyARP = default(false);
        gptp.grandMaster = default(true);
    connections allowunconnected:
}
