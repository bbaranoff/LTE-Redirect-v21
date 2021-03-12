// Copyright 2021 Ben Wojtowicz
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "DL_CCCH_Message.h"

#include <cmath>

int DL_CCCH_Message::Pack(std::vector<uint8_t> &bits)
{
    // Extension indicator

    // Optional indicators

    // Fields
    {
        if(0 != message.Pack(bits))
        {
            printf("DL_CCCH_Message:: field pack failure\n");
            return -1;
        }
    }
    return 0;
}

int DL_CCCH_Message::Unpack(std::vector<uint8_t> bits)
{
    uint32_t idx = 0;
    return Unpack(bits, idx);
}

int DL_CCCH_Message::Unpack(std::vector<uint8_t> bits, uint32_t &idx)
{
    // Extension indicator

    // Optional indicators

    // Fields
    {
        if(0 != message.Unpack(bits, idx))
        {
            printf("DL_CCCH_Message:: field unpack failure\n");
            return -1;
        }
    }
    return 0;
}

std::string DL_CCCH_Message::ToString(uint32_t indent) const
{
    std::string out;
    for(uint32_t i=0; i<indent; i++)
        out += "    ";
    out += "DL_CCCH_Message:\n";
    out += message_ToString(indent+1);
    return out;
}

std::string DL_CCCH_Message::ToStringNoNewLines() const
{
    std::string out = "DL_CCCH_Message:";
    out += message_ToStringNoNewLines();
    return out;
}

DL_CCCH_MessageType* DL_CCCH_Message::message_Set()
{
    message_present = true;
    return &message;
}

int DL_CCCH_Message::message_Set(DL_CCCH_MessageType &value)
{
    message_present = true;
    message = value;
    return 0;
}

const DL_CCCH_MessageType& DL_CCCH_Message::message_Get() const
{
    return message;
}

std::string DL_CCCH_Message::message_ToString(uint32_t indent) const
{
    std::string out;
    for(uint32_t i=0; i<indent; i++)
        out += "    ";
    out += "message:\n";
    out += message.ToString(indent+1);
    return out;
}

std::string DL_CCCH_Message::message_ToStringNoNewLines() const
{
    std::string out = "message:";
    out += message.ToStringNoNewLines();
    return out;
}

int DL_CCCH_Message::message_Clear()
{
    message_present = false;
    return 0;
}

bool DL_CCCH_Message::message_IsPresent() const
{
    return message_present;
}

