//
// Copyright (C) 2020 OpenSim Ltd.
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

#ifndef __INET_ELIGIBILITYTIMEGATE_H
#define __INET_ELIGIBILITYTIMEGATE_H

#include "inet/common/clock/ClockUserModuleMixin.h"
#include "inet/queueing/base/PacketGateBase.h"

namespace inet {

using namespace inet::queueing;

class INET_API EligibilityTimeGate : public ClockUserModuleMixin<PacketGateBase>
{
  protected:
    ClockEvent *eligibilityTimer = nullptr;

  protected:
    virtual void initialize(int stage) override;
    virtual void handleMessage(cMessage *message) override;

    virtual void updateOpen();

  public:
    virtual ~EligibilityTimeGate() { cancelAndDelete(eligibilityTimer); }

    virtual Packet *pullPacket(cGate *gate) override;

    virtual void handleCanPullPacketChanged(cGate *gate) override;
};

} // namespace inet

#endif
