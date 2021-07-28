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

#ifndef __INET_PACKETMULTIPLEXER2_H
#define __INET_PACKETMULTIPLEXER2_H

#include "inet/queueing/common/PacketMultiplexer.h"

namespace inet {
namespace queueing {

class INET_API PacketMultiplexer2 : public PacketMultiplexer
{
  public:
    virtual bool isForwardingProtocol(cGate *gate, ServicePrimitive servicePrimitive) const override {
        return false;
    }

    virtual bool isForwardingService(cGate *gate, ServicePrimitive servicePrimitive) const override {
        if (gate->getType() == cGate::INPUT)
            return servicePrimitive == SP_INDICATION || servicePrimitive == SP_CONFIRM;
        else if (gate->getType() == cGate::OUTPUT)
            return servicePrimitive == SP_REQUEST || servicePrimitive == SP_RESPONSE;
        else
            throw cRuntimeError("Unknown gate type");
    }
};

} // namespace queueing
} // namespace inet

#endif

