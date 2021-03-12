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

#include "CounterCheck_r8_IEs.h"

#include <cmath>

int CounterCheck_r8_IEs::Pack(std::vector<uint8_t> &bits)
{
    // Extension indicator

    // Optional indicators
    bits.push_back(nonCriticalExtension_IsPresent());

    // Fields
    {
        if(0 != drb_CountMSB_InfoList.Pack(bits))
        {
            printf("CounterCheck_r8_IEs:: field pack failure\n");
            return -1;
        }
    }
    if(nonCriticalExtension_IsPresent())
    {
        if(0 != nonCriticalExtension.Pack(bits))
        {
            printf("CounterCheck_r8_IEs:: field pack failure\n");
            return -1;
        }
    }
    return 0;
}

int CounterCheck_r8_IEs::Unpack(std::vector<uint8_t> bits)
{
    uint32_t idx = 0;
    return Unpack(bits, idx);
}

int CounterCheck_r8_IEs::Unpack(std::vector<uint8_t> bits, uint32_t &idx)
{
    // Extension indicator

    // Optional indicators
    if((idx + 1) > bits.size())
    {
        printf("CounterCheck_r8_IEs::Unpack() index out of bounds for optional indiator nonCriticalExtension\n");
        return -1;
    }
    nonCriticalExtension_present = bits[idx++];

    // Fields
    {
        if(0 != drb_CountMSB_InfoList.Unpack(bits, idx))
        {
            printf("CounterCheck_r8_IEs:: field unpack failure\n");
            return -1;
        }
    }
    if(nonCriticalExtension_present)
    {
        if(0 != nonCriticalExtension.Unpack(bits, idx))
        {
            printf("CounterCheck_r8_IEs:: field unpack failure\n");
            return -1;
        }
    }
    return 0;
}

std::string CounterCheck_r8_IEs::ToString(uint32_t indent) const
{
    std::string out;
    for(uint32_t i=0; i<indent; i++)
        out += "    ";
    out += "CounterCheck_r8_IEs:\n";
    out += drb_CountMSB_InfoList_ToString(indent+1);
    if(nonCriticalExtension_IsPresent())
        out += nonCriticalExtension_ToString(indent+1);
    return out;
}

std::string CounterCheck_r8_IEs::ToStringNoNewLines() const
{
    std::string out = "CounterCheck_r8_IEs:";
    out += drb_CountMSB_InfoList_ToStringNoNewLines();
    if(nonCriticalExtension_IsPresent())
        out += nonCriticalExtension_ToStringNoNewLines();
    return out;
}

DRB_CountMSB_InfoList* CounterCheck_r8_IEs::drb_CountMSB_InfoList_Set()
{
    drb_CountMSB_InfoList_present = true;
    return &drb_CountMSB_InfoList;
}

int CounterCheck_r8_IEs::drb_CountMSB_InfoList_Set(DRB_CountMSB_InfoList &value)
{
    drb_CountMSB_InfoList_present = true;
    drb_CountMSB_InfoList = value;
    return 0;
}

const DRB_CountMSB_InfoList& CounterCheck_r8_IEs::drb_CountMSB_InfoList_Get() const
{
    return drb_CountMSB_InfoList;
}

std::string CounterCheck_r8_IEs::drb_CountMSB_InfoList_ToString(uint32_t indent) const
{
    std::string out;
    for(uint32_t i=0; i<indent; i++)
        out += "    ";
    out += "drb_CountMSB_InfoList:\n";
    out += drb_CountMSB_InfoList.ToString(indent+1);
    return out;
}

std::string CounterCheck_r8_IEs::drb_CountMSB_InfoList_ToStringNoNewLines() const
{
    std::string out = "drb_CountMSB_InfoList:";
    out += drb_CountMSB_InfoList.ToStringNoNewLines();
    return out;
}

int CounterCheck_r8_IEs::drb_CountMSB_InfoList_Clear()
{
    drb_CountMSB_InfoList_present = false;
    return 0;
}

bool CounterCheck_r8_IEs::drb_CountMSB_InfoList_IsPresent() const
{
    return drb_CountMSB_InfoList_present;
}

CounterCheck_v8a0_IEs* CounterCheck_r8_IEs::nonCriticalExtension_Set()
{
    nonCriticalExtension_present = true;
    return &nonCriticalExtension;
}

int CounterCheck_r8_IEs::nonCriticalExtension_Set(CounterCheck_v8a0_IEs &value)
{
    nonCriticalExtension_present = true;
    nonCriticalExtension = value;
    return 0;
}

const CounterCheck_v8a0_IEs& CounterCheck_r8_IEs::nonCriticalExtension_Get() const
{
    return nonCriticalExtension;
}

std::string CounterCheck_r8_IEs::nonCriticalExtension_ToString(uint32_t indent) const
{
    std::string out;
    for(uint32_t i=0; i<indent; i++)
        out += "    ";
    out += "nonCriticalExtension:\n";
    out += nonCriticalExtension.ToString(indent+1);
    return out;
}

std::string CounterCheck_r8_IEs::nonCriticalExtension_ToStringNoNewLines() const
{
    std::string out = "nonCriticalExtension:";
    out += nonCriticalExtension.ToStringNoNewLines();
    return out;
}

int CounterCheck_r8_IEs::nonCriticalExtension_Clear()
{
    nonCriticalExtension_present = false;
    return 0;
}

bool CounterCheck_r8_IEs::nonCriticalExtension_IsPresent() const
{
    return nonCriticalExtension_present;
}

