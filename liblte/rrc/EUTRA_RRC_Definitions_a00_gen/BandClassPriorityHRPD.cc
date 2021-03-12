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

#include "BandClassPriorityHRPD.h"

#include <cmath>

int BandClassPriorityHRPD::Pack(std::vector<uint8_t> &bits)
{
    // Extension indicator

    // Optional indicators

    // Fields
    {
        if(0 != bandClass.Pack(bits))
        {
            printf("BandClassPriorityHRPD:: field pack failure\n");
            return -1;
        }
    }
    {
        if(0 != cellReselectionPriority.Pack(bits))
        {
            printf("BandClassPriorityHRPD:: field pack failure\n");
            return -1;
        }
    }
    return 0;
}

int BandClassPriorityHRPD::Unpack(std::vector<uint8_t> bits)
{
    uint32_t idx = 0;
    return Unpack(bits, idx);
}

int BandClassPriorityHRPD::Unpack(std::vector<uint8_t> bits, uint32_t &idx)
{
    // Extension indicator

    // Optional indicators

    // Fields
    {
        if(0 != bandClass.Unpack(bits, idx))
        {
            printf("BandClassPriorityHRPD:: field unpack failure\n");
            return -1;
        }
    }
    {
        if(0 != cellReselectionPriority.Unpack(bits, idx))
        {
            printf("BandClassPriorityHRPD:: field unpack failure\n");
            return -1;
        }
    }
    return 0;
}

std::string BandClassPriorityHRPD::ToString(uint32_t indent) const
{
    std::string out;
    for(uint32_t i=0; i<indent; i++)
        out += "    ";
    out += "BandClassPriorityHRPD:\n";
    out += bandClass_ToString(indent+1);
    out += cellReselectionPriority_ToString(indent+1);
    return out;
}

std::string BandClassPriorityHRPD::ToStringNoNewLines() const
{
    std::string out = "BandClassPriorityHRPD:";
    out += bandClass_ToStringNoNewLines();
    out += cellReselectionPriority_ToStringNoNewLines();
    return out;
}

BandclassCDMA2000* BandClassPriorityHRPD::bandClass_Set()
{
    bandClass_present = true;
    return &bandClass;
}

int BandClassPriorityHRPD::bandClass_Set(BandclassCDMA2000 &value)
{
    bandClass_present = true;
    bandClass = value;
    return 0;
}

const BandclassCDMA2000& BandClassPriorityHRPD::bandClass_Get() const
{
    return bandClass;
}

std::string BandClassPriorityHRPD::bandClass_ToString(uint32_t indent) const
{
    std::string out;
    for(uint32_t i=0; i<indent; i++)
        out += "    ";
    out += "bandClass:\n";
    out += bandClass.ToString(indent+1);
    return out;
}

std::string BandClassPriorityHRPD::bandClass_ToStringNoNewLines() const
{
    std::string out = "bandClass:";
    out += bandClass.ToStringNoNewLines();
    return out;
}

int BandClassPriorityHRPD::bandClass_Clear()
{
    bandClass_present = false;
    return 0;
}

bool BandClassPriorityHRPD::bandClass_IsPresent() const
{
    return bandClass_present;
}

CellReselectionPriority* BandClassPriorityHRPD::cellReselectionPriority_Set()
{
    cellReselectionPriority_present = true;
    return &cellReselectionPriority;
}

int BandClassPriorityHRPD::cellReselectionPriority_Set(CellReselectionPriority &value)
{
    cellReselectionPriority_present = true;
    cellReselectionPriority = value;
    return 0;
}

const CellReselectionPriority& BandClassPriorityHRPD::cellReselectionPriority_Get() const
{
    return cellReselectionPriority;
}

std::string BandClassPriorityHRPD::cellReselectionPriority_ToString(uint32_t indent) const
{
    std::string out;
    for(uint32_t i=0; i<indent; i++)
        out += "    ";
    out += "cellReselectionPriority:\n";
    out += cellReselectionPriority.ToString(indent+1);
    return out;
}

std::string BandClassPriorityHRPD::cellReselectionPriority_ToStringNoNewLines() const
{
    std::string out = "cellReselectionPriority:";
    out += cellReselectionPriority.ToStringNoNewLines();
    return out;
}

int BandClassPriorityHRPD::cellReselectionPriority_Clear()
{
    cellReselectionPriority_present = false;
    return 0;
}

bool BandClassPriorityHRPD::cellReselectionPriority_IsPresent() const
{
    return cellReselectionPriority_present;
}

