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

#include "OSPFNeighborState.h"
#include "OSPFInterface.h"
#include "OSPFArea.h"
#include "OSPFRouter.h"

void OSPF::NeighborState::ChangeState(OSPF::Neighbor* neighbor, OSPF::NeighborState* newState, OSPF::NeighborState* currentState)
{

    OSPF::Neighbor::NeighborStateType   oldState            = currentState->getState();
    OSPF::Neighbor::NeighborStateType   nextState           = newState->getState();
    bool                                rebuildRoutingTable = false;

    neighbor->ChangeState(newState, currentState);

    if ((oldState == OSPF::Neighbor::FullState) || (nextState == OSPF::Neighbor::FullState)) {
        OSPF::RouterID   routerID  = neighbor->getInterface()->getArea()->getRouter()->GetRouterID();
        OSPF::RouterLSA* routerLSA = neighbor->getInterface()->getArea()->FindRouterLSA(routerID);

        if (routerLSA != NULL) {
            long sequenceNumber = routerLSA->getHeader().getLsSequenceNumber();
            if (sequenceNumber == MAX_SEQUENCE_NUMBER) {
                routerLSA->getHeader().setLsAge(MAX_AGE);
                neighbor->getInterface()->getArea()->FloodLSA(routerLSA);
                routerLSA->IncrementInstallTime();
            } else {
                OSPF::RouterLSA* newLSA = neighbor->getInterface()->getArea()->OriginateRouterLSA();

                newLSA->getHeader().setLsSequenceNumber(sequenceNumber + 1);
                newLSA->getHeader().setLsChecksum(0);    // TODO: calculate correct LS checksum
                rebuildRoutingTable |= routerLSA->Update(newLSA);
                delete newLSA;

                neighbor->getInterface()->getArea()->FloodLSA(routerLSA);
            }
        }

        if (neighbor->getInterface()->getState() == OSPF::Interface::DesignatedRouterState) {
            OSPF::NetworkLSA* networkLSA = neighbor->getInterface()->getArea()->FindNetworkLSA(ULongFromIPv4Address(neighbor->getInterface()->getAddressRange().address));

            if (networkLSA != NULL) {
                long sequenceNumber = networkLSA->getHeader().getLsSequenceNumber();
                if (sequenceNumber == MAX_SEQUENCE_NUMBER) {
                    networkLSA->getHeader().setLsAge(MAX_AGE);
                    neighbor->getInterface()->getArea()->FloodLSA(networkLSA);
                    networkLSA->IncrementInstallTime();
                } else {
                    OSPF::NetworkLSA* newLSA = neighbor->getInterface()->getArea()->OriginateNetworkLSA(neighbor->getInterface());

                    if (newLSA != NULL) {
                        newLSA->getHeader().setLsSequenceNumber(sequenceNumber + 1);
                        newLSA->getHeader().setLsChecksum(0);    // TODO: calculate correct LS checksum
                        rebuildRoutingTable |= networkLSA->Update(newLSA);
                        delete newLSA;
                    } else {    // no neighbors on the network -> old NetworkLSA must be flushed
                        networkLSA->getHeader().setLsAge(MAX_AGE);
                        networkLSA->IncrementInstallTime();
                    }

                    neighbor->getInterface()->getArea()->FloodLSA(networkLSA);
                }
            }
        }
    }

    if (rebuildRoutingTable) {
        neighbor->getInterface()->getArea()->getRouter()->RebuildRoutingTable();
    }
}
