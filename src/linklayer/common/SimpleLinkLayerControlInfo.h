//
// Copyright (C) 2011 Andras Varga
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

#ifndef __INET_SIMPLELINKLAYERCONTROLINFO_H_
#define __INET_SIMPLELINKLAYERCONTROLINFO_H_

#include "INETDefs.h"
#include "ILinkLayerControlInfo.h"
#include "SimpleLinkLayerControlInfo_m.h"

/**
 * Represents a SimpleLinkLayer control info. More info in the SimpleLinkLayerControlInfo.msg file
 * (and the documentation generated from it).
 */
class INET_API SimpleLinkLayerControlInfo : public SimpleLinkLayerControlInfo_Base, public ILinkLayerControlInfo
{
  public:
    SimpleLinkLayerControlInfo() : SimpleLinkLayerControlInfo_Base() {}
    SimpleLinkLayerControlInfo(const SimpleLinkLayerControlInfo& other) : SimpleLinkLayerControlInfo_Base(other) {}
    SimpleLinkLayerControlInfo& operator=(const SimpleLinkLayerControlInfo& other) {SimpleLinkLayerControlInfo_Base::operator=(other); return *this;}

    virtual SimpleLinkLayerControlInfo *dup() const {return new SimpleLinkLayerControlInfo(*this);}

    virtual MACAddress getSourceAddress() const { return getSrc(); }
    virtual void setSourceAddress(const MACAddress & address) { setSrc(address); }
    virtual MACAddress getDestinationAddress() const { return getDest(); }
    virtual void setDestinationAddress(const MACAddress & address) { setDest(address); };
    virtual int getInterfaceId() const { return SimpleLinkLayerControlInfo_Base::getInterfaceId(); }
    virtual void setInterfaceId(int interfaceId) { SimpleLinkLayerControlInfo_Base::setInterfaceId(interfaceId); }
};

#endif
