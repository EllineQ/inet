//
// Copyright (C) 2009 Christoph Sommer <christoph.sommer@informatik.uni-erlangen.de>
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
//

#include "inet/networklayer/configurator/ipv4/HostAutoConfigurator.h"

#include <algorithm>

#include "inet/common/ModuleAccess.h"
#include "inet/common/lifecycle/ModuleOperations.h"
#include "inet/common/lifecycle/NodeStatus.h"
#include "inet/networklayer/common/L3AddressResolver.h"
#include "inet/networklayer/contract/IInterfaceTable.h"
#include "inet/networklayer/contract/ipv4/Ipv4Address.h"
#include "inet/networklayer/ipv4/Ipv4InterfaceData.h"

namespace inet {

Define_Module(HostAutoConfigurator);

void HostAutoConfigurator::initialize(int stage)
{
    OperationalBase::initialize(stage);
    if (stage == INITSTAGE_LOCAL) {
        interfaceTable.reference(this, "interfaceTableModule", true);
    }
}

void HostAutoConfigurator::finish()
{
}

void HostAutoConfigurator::handleMessageWhenUp(cMessage *apMsg)
{
}

void HostAutoConfigurator::setupNetworkLayer()
{
    EV_INFO << "host auto configuration started" << std::endl;

    Ipv4Address addressBase = Ipv4Address(par("addressBase").stringValue());
    Ipv4Address netmask = Ipv4Address(par("netmask").stringValue());

    // get our host module
    cModule *host = getContainingNode(this);

    Ipv4Address myAddress = Ipv4Address(addressBase.getInt() + uint32_t(host->getId()));

    // address test
    if (!Ipv4Address::maskedAddrAreEqual(myAddress, addressBase, netmask))
        throw cRuntimeError("Generated IP address is out of specified address range");

    // get our routing table
    IIpv4RoutingTable *routingTable = L3AddressResolver().getIpv4RoutingTableOf(host);
    if (!routingTable)
        throw cRuntimeError("No routing table found");

    // look at all interface table entries
    auto interfaces = check_and_cast<cValueArray *>(par("interfaces").objectValue())->asStringVector();
    for (const auto& ifname : interfaces) {
        NetworkInterface *ie = interfaceTable->findInterfaceByName(ifname.c_str());
        if (!ie)
            throw cRuntimeError("No such interface '%s'", ifname.c_str());

        // assign IP Address to all connected interfaces
        if (ie->isLoopback()) {
            EV_INFO << "interface " << ifname << " skipped (is loopback)" << std::endl;
            continue;
        }

        EV_INFO << "interface " << ifname << " gets " << myAddress.str() << "/" << netmask.str() << std::endl;

        auto ipv4Data = ie->getProtocolDataForUpdate<Ipv4InterfaceData>();
        ipv4Data->setIPAddress(myAddress);
        ipv4Data->setNetmask(netmask);
        ie->setBroadcast(true);

        // associate interface with default multicast groups
        ipv4Data->joinMulticastGroup(Ipv4Address::ALL_HOSTS_MCAST);
        ipv4Data->joinMulticastGroup(Ipv4Address::ALL_ROUTERS_MCAST);

        // associate interface with specified multicast groups
        auto mcastGroups = check_and_cast<cValueArray *>(par("mcastGroups").objectValue())->asStringVector();
        for (const auto& group : mcastGroups) {
            Ipv4Address mcastGroup(group.c_str());
            ipv4Data->joinMulticastGroup(mcastGroup);
        }
    }
}

void HostAutoConfigurator::handleStartOperation(LifecycleOperation *operation)
{
    if (operation == nullptr) {
        // in initialize:
        for (int i = 0; i < interfaceTable->getNumInterfaces(); i++)
            interfaceTable->getInterface(i)->addProtocolData<Ipv4InterfaceData>();
    }
    setupNetworkLayer();
}

void HostAutoConfigurator::handleStopOperation(LifecycleOperation *operation)
{
}

void HostAutoConfigurator::handleCrashOperation(LifecycleOperation *operation)
{
}

} // namespace inet

