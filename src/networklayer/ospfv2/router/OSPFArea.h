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

#ifndef __INET_OSPFAREA_H
#define __INET_OSPFAREA_H

#include <vector>
#include <map>
#include "OSPFcommon.h"
#include "OSPFInterface.h"
#include "LSA.h"
#include "OSPFRoutingTableEntry.h"

namespace OSPF {

class Router;

class Area : public cPolymorphic {
private:
    AreaID                                                  areaID;
    std::map<IPv4AddressRange, bool, IPv4AddressRange_Less> advertiseAddressRanges;
    std::vector<IPv4AddressRange>                           areaAddressRanges;
    std::vector<Interface*>                                 associatedInterfaces;
    std::vector<HostRouteParameters>                        hostRoutes;
    std::map<LinkStateID, RouterLSA*>                       routerLSAsByID;
    std::vector<RouterLSA*>                                 routerLSAs;
    std::map<LinkStateID, NetworkLSA*>                      networkLSAsByID;
    std::vector<NetworkLSA*>                                networkLSAs;
    std::map<LSAKeyType, SummaryLSA*, LSAKeyType_Less>      summaryLSAsByID;
    std::vector<SummaryLSA*>                                summaryLSAs;
    bool                                                    transitCapability;
    bool                                                    externalRoutingCapability;
    Metric                                                  stubDefaultCost;
    RouterLSA*                                              spfTreeRoot;

    Router*                                                 parentRouter;
public:
            Area(AreaID id = BackboneAreaID);
    virtual ~Area(void);

    void                SetAreaID                       (AreaID areaId)                                 { areaID = areaId; }
    AreaID              GetAreaID                       (void) const                                    { return areaID; }
    void                AddAddressRange                 (IPv4AddressRange addressRange, bool advertise) { areaAddressRanges.push_back(addressRange); advertiseAddressRanges[addressRange] = advertise; }
    unsigned int        getAddressRangeCount            (void) const                                    { return areaAddressRanges.size(); }
    IPv4AddressRange    getAddressRange                 (unsigned int index) const                      { return areaAddressRanges[index]; }
    void                AddHostRoute                    (HostRouteParameters& hostRouteParameters)      { hostRoutes.push_back(hostRouteParameters); }
    void                setTransitCapability            (bool transit)                                  { transitCapability = transit; }
    bool                getTransitCapability            (void) const                                    { return transitCapability; }
    void                setExternalRoutingCapability    (bool flooded)                                  { externalRoutingCapability = flooded; }
    bool                getExternalRoutingCapability    (void) const                                    { return externalRoutingCapability; }
    void                setStubDefaultCost              (Metric cost)                                   { stubDefaultCost = cost; }
    Metric              getStubDefaultCost              (void) const                                    { return stubDefaultCost; }
    void                setSPFTreeRoot                  (RouterLSA* root)                               { spfTreeRoot = root; }
    RouterLSA*          getSPFTreeRoot                  (void)                                          { return spfTreeRoot; }
    const RouterLSA*    getSPFTreeRoot                  (void) const                                    { return spfTreeRoot; }

    void                setRouter                       (Router* router)                                { parentRouter = router; }
    Router*             getRouter                       (void)                                          { return parentRouter; }
    const Router*       getRouter                       (void) const                                    { return parentRouter; }

    unsigned long       getRouterLSACount               (void) const                                    { return routerLSAs.size(); }
    RouterLSA*          getRouterLSA                    (unsigned long i)                               { return routerLSAs[i]; }
    const RouterLSA*    getRouterLSA                    (unsigned long i) const                         { return routerLSAs[i]; }
    unsigned long       getNetworkLSACount              (void) const                                    { return networkLSAs.size(); }
    NetworkLSA*         getNetworkLSA                   (unsigned long i)                               { return networkLSAs[i]; }
    const NetworkLSA*   getNetworkLSA                   (unsigned long i) const                         { return networkLSAs[i]; }
    unsigned long       getSummaryLSACount              (void) const                                    { return summaryLSAs.size(); }
    SummaryLSA*         getSummaryLSA                   (unsigned long i)                               { return summaryLSAs[i]; }
    const SummaryLSA*   getSummaryLSA                   (unsigned long i) const                         { return summaryLSAs[i]; }

    bool                ContainsAddress                     (IPv4Address address) const;
    bool                HasAddressRange                     (IPv4AddressRange addressRange) const;
    IPv4AddressRange    getContainingAddressRange           (IPv4AddressRange addressRange, bool* advertise = NULL) const;
    void                AddInterface                        (Interface* intf);
    Interface*          getInterface                        (unsigned char ifIndex);
    Interface*          getInterface                        (IPv4Address address);
    bool                HasVirtualLink                      (AreaID withTransitArea) const;
    Interface*          FindVirtualLink                     (RouterID routerID);

    bool                InstallRouterLSA                    (OSPFRouterLSA* lsa);
    bool                InstallNetworkLSA                   (OSPFNetworkLSA* lsa);
    bool                InstallSummaryLSA                   (OSPFSummaryLSA* lsa);
    RouterLSA*          FindRouterLSA                       (LinkStateID linkStateID);
    const RouterLSA*    FindRouterLSA                       (LinkStateID linkStateID) const;
    NetworkLSA*         FindNetworkLSA                      (LinkStateID linkStateID);
    const NetworkLSA*   FindNetworkLSA                      (LinkStateID linkStateID) const;
    SummaryLSA*         FindSummaryLSA                      (LSAKeyType lsaKey);
    const SummaryLSA*   FindSummaryLSA                      (LSAKeyType lsaKey) const;
    void                AgeDatabase                         (void);
    bool                HasAnyNeighborInStates              (int states) const;
    void                RemoveFromAllRetransmissionLists    (LSAKeyType lsaKey);
    bool                IsOnAnyRetransmissionList           (LSAKeyType lsaKey) const;
    bool                FloodLSA                            (OSPFLSA* lsa, Interface* intf = NULL, Neighbor* neighbor = NULL);
    bool                IsLocalAddress                      (IPv4Address address) const;
    RouterLSA*          OriginateRouterLSA                  (void);
    NetworkLSA*         OriginateNetworkLSA                 (const Interface* intf);
    SummaryLSA*         OriginateSummaryLSA                 (const RoutingTableEntry* entry,
                                                             const std::map<LSAKeyType, bool, LSAKeyType_Less>& originatedLSAs,
                                                             SummaryLSA*& lsaToReoriginate);
    void                CalculateShortestPathTree           (std::vector<RoutingTableEntry*>& newRoutingTable);
    void                CalculateInterAreaRoutes            (std::vector<RoutingTableEntry*>& newRoutingTable);
    void                ReCheckSummaryLSAs                  (std::vector<RoutingTableEntry*>& newRoutingTable);

    void        info(char* buffer);
    std::string detailedInfo(void) const;

private:
    SummaryLSA*             OriginateSummaryLSA                     (const OSPF::SummaryLSA* summaryLSA);
    bool                    HasLink                                 (OSPFLSA* fromLSA, OSPFLSA* toLSA) const;
    std::vector<NextHop>*   CalculateNextHops                       (OSPFLSA* destination, OSPFLSA* parent) const;
    std::vector<NextHop>*   CalculateNextHops                       (Link& destination, OSPFLSA* parent) const;

    LinkStateID             getUniqueLinkStateID                    (IPv4AddressRange destination,
                                                                     Metric destinationCost,
                                                                     SummaryLSA*& lsaToReoriginate) const;

    bool                    FindSameOrWorseCostRoute                (const std::vector<OSPF::RoutingTableEntry*>& newRoutingTable,
                                                                     const OSPF::SummaryLSA&                      currentLSA,
                                                                     unsigned short                               currentCost,
                                                                     bool&                                        destinationInRoutingTable,
                                                                     std::list<OSPF::RoutingTableEntry*>&         sameOrWorseCost) const;

    RoutingTableEntry*      CreateRoutingTableEntryFromSummaryLSA   (const OSPF::SummaryLSA&        summaryLSA,
                                                                     unsigned short                 entryCost,
                                                                     const OSPF::RoutingTableEntry& borderRouterEntry) const;
};

} // namespace OSPF

inline std::ostream& operator<< (std::ostream& ostr, OSPF::Area& area)
{
    ostr << area.detailedInfo();
    return ostr;
}

#endif // __INET_OSPFAREA_H

