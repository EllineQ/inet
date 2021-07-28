//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.
//

package inet.examples.ethernet.tsn;

import inet.linklayer.ieee8021as.GptpMaster;
import inet.networks.TsnNetworkBase;
import inet.node.ethernet.Eth100M;
import inet.node.tsn.TsnDevice;
import inet.node.tsn.TsnSwitch;

network TsnNetworkExample extends TsnNetworkBase
{
    submodules:
        source: TsnDevice {
            @display("p=250,200");
        }
        switch1: TsnSwitch {
            @display("p=450,200");
        }
        switch2: TsnSwitch {
            @display("p=650,100");
        }
        switch3: TsnSwitch {
            @display("p=650,300");
        }
        destination: TsnDevice {
            @display("p=850,200");
        }
        gptpMaster: GptpMaster {
            @display("p=450,400");
        }
    connections:
        gptpMaster.ethg++ <--> Eth100M <--> switch1.ethg++;
        source.ethg++ <--> Eth100M <--> switch1.ethg++;
        switch1.ethg++ <--> Eth100M <--> switch2.ethg++;
        switch1.ethg++ <--> Eth100M <--> switch3.ethg++;
        switch2.ethg++ <--> Eth100M <--> destination.ethg++;
        switch3.ethg++ <--> Eth100M <--> destination.ethg++;
}

