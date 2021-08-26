//
// Copyright (C) 2004 Andras Varga
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

#include "inet/transportlayer/tcp_common/TCPSegment.h"

namespace inet {

namespace tcp {

Register_Class(Sack);

bool Sack::empty() const
{
    return start == 0 && end == 0;
}

bool Sack::contains(const Sack& other) const
{
    return seqLE(start, other.start) && seqLE(other.end, end);
}

void Sack::clear()
{
    start = end = 0;
}

void Sack::setSegment(unsigned int start_par, unsigned int end_par)
{
    setStart(start_par);
    setEnd(end_par);
}

std::string Sack::str() const
{
    std::stringstream out;

    out << "[" << start << ".." << end << ")";
    return out.str();
}

Register_Class(TCPSegment);

uint32_t TCPSegment::getSegLen()
{
    return payloadLength + (finBit ? 1 : 0) + (synBit ? 1 : 0);
}

void TCPSegment::truncateSegment(uint32 firstSeqNo, uint32 endSeqNo)
{
    ASSERT(payloadLength > 0);

    // must have common part:
#ifndef NDEBUG
    if (!(seqLess(sequenceNo, endSeqNo) && seqLess(firstSeqNo, sequenceNo + payloadLength))) {
        throw cRuntimeError(this, "truncateSegment(%u,%u) called on [%u, %u) segment\n",
                firstSeqNo, endSeqNo, sequenceNo, sequenceNo + payloadLength);
    }
#endif // ifndef NDEBUG

    unsigned int truncleft = 0;
    unsigned int truncright = 0;

    if (seqLess(sequenceNo, firstSeqNo)) {
        truncleft = firstSeqNo - sequenceNo;
    }

    if (seqGreater(sequenceNo + payloadLength, endSeqNo)) {
        truncright = sequenceNo + payloadLength - endSeqNo;
    }

    truncateData(truncleft, truncright);
}

unsigned short TCPSegment::getHeaderOptionArrayLength()
{
    unsigned short usedLength = 0;

    for (uint i = 0; i < getHeaderOptionArraySize(); i++)
        usedLength += getHeaderOption(i)->getLength();

    return usedLength;
}

TCPSegment& TCPSegment::operator=(const TCPSegment& other)
{
    if (this == &other)
        return *this;
    clean();
    TCPSegment_Base::operator=(other);
    copy(other);
    return *this;
}

TCPSegment::~TCPSegment()
{
}

void TCPSegment::truncateData(unsigned int truncleft, unsigned int truncright)
{
    ASSERT(payloadLength >= truncleft + truncright);

    if (0 != byteArray.getDataArraySize())
        byteArray.truncateData(truncleft, truncright);

    while (payload_arraysize > 0 && (payload[0].getEndSequenceNo() - sequenceNo) <= truncleft) {
        erasePayload(0);
    }

    sequenceNo += truncleft;
    payloadLength -= truncleft + truncright;

    // truncate payload data correctly
    while (payload_arraysize > 0 && (payload[payload_arraysize-1].getEndSequenceNo() - sequenceNo) > payloadLength) {
        erasePayload(payload_arraysize-1);
    }
}

void TCPSegment::addPayloadMessage(cPacket *msg, uint32 endSequenceNo)
{
    take(msg);

    TCPPayloadMessage payload;
    payload.setEndSequenceNo(endSequenceNo);
    payload.setMsg(msg);
    insertPayload(payload);
}

cPacket *TCPSegment::removeFirstPayloadMessage(uint32& endSequenceNo)
{
    cPacket *msg = nullptr;
    if (payload_arraysize > 0) {
        msg = payload[0].dropMsg();
        endSequenceNo = payload[0].getEndSequenceNo();
        erasePayload(0);
        drop(msg);
    }
    return msg;
}

void TCPSegment::addHeaderOption(TCPOption *option)
{
    insertHeaderOption(option);
}

void TCPSegment::dropHeaderOptions()
{
    setHeaderOptionArraySize(0);
}


} // namespace tcp

} // namespace inet

