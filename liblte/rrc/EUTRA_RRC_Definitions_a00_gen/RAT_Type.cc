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

#include "RAT_Type.h"

#include <cmath>

int RAT_Type::Pack(std::vector<uint8_t> &bits)
{
    if(!present)
    {
        printf("RAT_Type::Pack() presence failure\n");
        return -1;
    }
    if(internal_value > k_spare1)
    {
        bits.push_back(1);
    }else{
        bits.push_back(0);
    }
    uint32_t N_bits = 3;
    if(internal_value > k_spare1)
        N_bits = 0;
    for(uint32_t i=0; i<N_bits; i++)
    {
        if(internal_value > k_spare1)
        {
            bits.push_back(((internal_value - k_spare1) >> (N_bits-1-i)) & 1);
        }else{
            bits.push_back((internal_value >> (N_bits-1-i)) & 1);
        }
    }
    return 0;
}

int RAT_Type::Unpack(std::vector<uint8_t> bits)
{
    uint32_t idx = 0;
    return Unpack(bits, idx);
}

int RAT_Type::Unpack(std::vector<uint8_t> bits, uint32_t &idx)
{
    if((idx + 1) > bits.size())
    {
        printf("RAT_Type::Unpack() index out of bounds for extension\n");
        return -1;
    }
    bool has_extension = bits[idx++];
    uint32_t packed_val = 0;
    uint32_t N_bits = 3;
    if(has_extension)
        N_bits = 0;
    if((idx + N_bits) > bits.size())
    {
        printf("RAT_Type::Unpack() index out of bounds for value\n");
        return -1;
    }
    for(uint32_t i=0; i<N_bits; i++)
        packed_val |= bits[idx++] << (N_bits-i-1);
    if(has_extension)
    {
        internal_value = (Enum)(packed_val + 8);
    }else{
        if(packed_val > 7)
        {
            printf("RAT_Type::Unpack() max failure\n");
            return -1;
        }
        internal_value = (Enum)packed_val;
    }
    present = true;
    return 0;
}

RAT_Type::Enum RAT_Type::Value() const
{
    if(present)
        return internal_value;
    return (RAT_Type::Enum)0;
}

int RAT_Type::SetValue(Enum value)
{
    internal_value = value;
    present = true;
    return 0;
}

int RAT_Type::SetValue(std::string value)
{
    if("eutra" == value)
    {
        internal_value = k_eutra;
        present = true;
        return 0;
    }
    if("utra" == value)
    {
        internal_value = k_utra;
        present = true;
        return 0;
    }
    if("geran_cs" == value)
    {
        internal_value = k_geran_cs;
        present = true;
        return 0;
    }
    if("geran_ps" == value)
    {
        internal_value = k_geran_ps;
        present = true;
        return 0;
    }
    if("cdma2000_1XRTT" == value)
    {
        internal_value = k_cdma2000_1XRTT;
        present = true;
        return 0;
    }
    if("spare3" == value)
    {
        internal_value = k_spare3;
        present = true;
        return 0;
    }
    if("spare2" == value)
    {
        internal_value = k_spare2;
        present = true;
        return 0;
    }
    if("spare1" == value)
    {
        internal_value = k_spare1;
        present = true;
        return 0;
    }
    return 1;
}

std::string RAT_Type::ValueToString(Enum value) const
{
    if(k_eutra == value)
        return "eutra";
    if(k_utra == value)
        return "utra";
    if(k_geran_cs == value)
        return "geran_cs";
    if(k_geran_ps == value)
        return "geran_ps";
    if(k_cdma2000_1XRTT == value)
        return "cdma2000_1XRTT";
    if(k_spare3 == value)
        return "spare3";
    if(k_spare2 == value)
        return "spare2";
    if(k_spare1 == value)
        return "spare1";
    return "";
}

uint64_t RAT_Type::NumberOfValues() const
{
    return 8;
}

std::string RAT_Type::ToString(uint32_t indent) const
{
    std::string out;
    for(uint32_t i=0; i<indent; i++)
        out += "    ";
    out += "RAT_Type = " + ValueToString(internal_value) + "\n";
    return out;
}

std::string RAT_Type::ToStringNoNewLines() const
{
    std::string out = "RAT_Type=" + ValueToString(internal_value) + ",";
    return out;
}

int RAT_Type::Clear()
{
    present = false;
    return 0;
}

bool RAT_Type::IsPresent() const
{
    return present;
}

