//
// Copyright (C) 2006 Andras Babos and Andras Varga
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, see <http://www.gnu.org/licenses/>.
//

#include "OSPFInterfaceStateNotDesignatedRouter.h"
#include "OSPFInterfaceStateDown.h"
#include "OSPFInterfaceStateLoopback.h"
#include "OSPFArea.h"
#include "OSPFRouter.h"
#include "MessageHandler.h"

void OSPF::InterfaceStateNotDesignatedRouter::ProcessEvent(OSPF::Interface* intf, OSPF::Interface::InterfaceEventType event)
{
    if (event == OSPF::Interface::NeighborChange) {
        CalculateDesignatedRouter(intf);
    }
    if (event == OSPF::Interface::InterfaceDown) {
        intf->Reset();
        ChangeState(intf, new OSPF::InterfaceStateDown, this);
    }
    if (event == OSPF::Interface::LoopIndication) {
        intf->Reset();
        ChangeState(intf, new OSPF::InterfaceStateLoopback, this);
    }
    if (event == OSPF::Interface::HelloTimer) {
        if (intf->GetType() == OSPF::Interface::Broadcast) {
            intf->SendHelloPacket(OSPF::AllSPFRouters);
        } else {    // OSPF::Interface::NBMA
            if (intf->GetRouterPriority() > 0) {
                unsigned long neighborCount = intf->getNeighborCount();
                for (unsigned long i = 0; i < neighborCount; i++) {
                    const OSPF::Neighbor* neighbor = intf->GetNeighbor(i);
                    if (neighbor->getPriority() > 0) {
                        intf->SendHelloPacket(neighbor->getAddress());
                    }
                }
            } else {
                intf->SendHelloPacket(intf->GetDesignatedRouter().ipInterfaceAddress);
                intf->SendHelloPacket(intf->GetBackupDesignatedRouter().ipInterfaceAddress);
            }
        }
        intf->getArea()->getRouter()->getMessageHandler()->StartTimer(intf->getHelloTimer(), intf->GetHelloInterval());
    }
    if (event == OSPF::Interface::AcknowledgementTimer) {
        intf->SendDelayedAcknowledgements();
    }
}

