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

#include "UE_CapabilityRAT_Container.h"

#include <cmath>

int UE_CapabilityRAT_Container::Pack(std::vector<uint8_t> &bits)
{
    // Extension indicator

    // Optional indicators

    // Fields
    {
        if(0 != rat_Type.Pack(bits))
        {
            printf("UE_CapabilityRAT_Container:: field pack failure\n");
            return -1;
        }
    }
    {
        if(0 != ueCapabilityRAT_Container_Pack(bits))
        {
            printf("UE_CapabilityRAT_Container:: field pack failure\n");
            return -1;
        }
    }
    return 0;
}

int UE_CapabilityRAT_Container::Unpack(std::vector<uint8_t> bits)
{
    uint32_t idx = 0;
    return Unpack(bits, idx);
}

int UE_CapabilityRAT_Container::Unpack(std::vector<uint8_t> bits, uint32_t &idx)
{
    // Extension indicator

    // Optional indicators

    // Fields
    {
        if(0 != rat_Type.Unpack(bits, idx))
        {
            printf("UE_CapabilityRAT_Container:: field unpack failure\n");
            return -1;
        }
    }
    {
        if(0 != ueCapabilityRAT_Container_Unpack(bits, idx))
        {
            printf("UE_CapabilityRAT_Container:: field unpack failure\n");
            return -1;
        }
    }
    return 0;
}

std::string UE_CapabilityRAT_Container::ToString(uint32_t indent) const
{
    std::string out;
    for(uint32_t i=0; i<indent; i++)
        out += "    ";
    out += "UE_CapabilityRAT_Container:\n";
    out += rat_Type_ToString(indent+1);
    out += ueCapabilityRAT_Container_ToString(indent+1);
    return out;
}

std::string UE_CapabilityRAT_Container::ToStringNoNewLines() const
{
    std::string out = "UE_CapabilityRAT_Container:";
    out += rat_Type_ToStringNoNewLines();
    out += ueCapabilityRAT_Container_ToStringNoNewLines();
    return out;
}

RAT_Type* UE_CapabilityRAT_Container::rat_Type_Set()
{
    rat_Type_present = true;
    return &rat_Type;
}

int UE_CapabilityRAT_Container::rat_Type_Set(RAT_Type &value)
{
    rat_Type_present = true;
    rat_Type = value;
    return 0;
}

const RAT_Type& UE_CapabilityRAT_Container::rat_Type_Get() const
{
    return rat_Type;
}

std::string UE_CapabilityRAT_Container::rat_Type_ToString(uint32_t indent) const
{
    std::string out;
    for(uint32_t i=0; i<indent; i++)
        out += "    ";
    out += "rat_Type:\n";
    out += rat_Type.ToString(indent+1);
    return out;
}

std::string UE_CapabilityRAT_Container::rat_Type_ToStringNoNewLines() const
{
    std::string out = "rat_Type:";
    out += rat_Type.ToStringNoNewLines();
    return out;
}

int UE_CapabilityRAT_Container::rat_Type_Clear()
{
    rat_Type_present = false;
    return 0;
}

bool UE_CapabilityRAT_Container::rat_Type_IsPresent() const
{
    return rat_Type_present;
}

int UE_CapabilityRAT_Container::ueCapabilityRAT_Container_Pack(std::vector<uint8_t> &bits)
{
    if(!ueCapabilityRAT_Container_present)
    {
        printf("UE_CapabilityRAT_Container::ueCapabilityRAT_Container_Pack() presence failure\n");
        return -1;
    }
    uint32_t local_size_length = 0;
    uint32_t local_size = ueCapabilityRAT_Container_internal_value.size();
    if(local_size < 128)
    {
        bits.push_back(0);
        local_size_length = 7;
    }else if(local_size < 16383){
        bits.push_back(1);
        bits.push_back(0);
        local_size_length = 14;
    }else{
        bits.push_back(1);
        bits.push_back(1);
        uint32_t N_16k_blocks = local_size / 16383;
        for(uint32_t i=0; i<6; i++)
            bits.push_back((N_16k_blocks >> (6-i-1)) & 1);
        local_size %= 16383;
        if(local_size < 128)
        {
            bits.push_back(0);
            local_size_length = 7;
        }else{
            bits.push_back(1);
            bits.push_back(0);
            local_size_length = 14;
        }
    }
    for(uint32_t i=0; i<local_size_length; i++)
        bits.push_back((local_size >> (local_size_length-i-1)) & 1);
    for(uint32_t i=0; i<ueCapabilityRAT_Container_internal_value.size(); i++)
        for(uint32_t j=0; j<8; j++)
            bits.push_back((ueCapabilityRAT_Container_internal_value[i] >> (8-j-1)) & 1);
    return 0;
}

int UE_CapabilityRAT_Container::ueCapabilityRAT_Container_Unpack(std::vector<uint8_t> bits)
{
    uint32_t idx = 0;
    return ueCapabilityRAT_Container_Unpack(bits, idx);
}

int UE_CapabilityRAT_Container::ueCapabilityRAT_Container_Unpack(std::vector<uint8_t> bits, uint32_t &idx)
{
    uint32_t local_size_length = 0;
    uint32_t size = 0;
    if(0 == bits[idx++])
    {
        local_size_length = 7;
    }else{
        if(0 == bits[idx++])
        {
            local_size_length = 14;
        }else{
            for(uint32_t i=0; i<6; i++)
                size |= bits[idx++] << (6-i-1);
            size *= 16383;
            if(0 == bits[idx++])
            {
                local_size_length = 7;
            }else{
                idx++;
                local_size_length = 14;
            }
        }
    }
    for(uint32_t i=0; i<local_size_length; i++)
        size |= bits[idx++] << (local_size_length-i-1);
    ueCapabilityRAT_Container_internal_value.clear();
    if((idx +size*8) > bits.size())
    {
        printf("UE_CapabilityRAT_Container::ueCapabilityRAT_Container_Unpack() index out of bounds\n");
        return -1;
    }
    for(uint32_t i=0; i<size; i++)
    {
        uint8_t value = 0;
        for(uint32_t j=0; j<8; j++)
            value |= bits[idx+j] << (8-j-1);
        ueCapabilityRAT_Container_internal_value.push_back(value);
        idx += 8;
    }
    ueCapabilityRAT_Container_present = true;
    return 0;
}

std::vector<uint8_t> UE_CapabilityRAT_Container::ueCapabilityRAT_Container_Value() const
{
    if(ueCapabilityRAT_Container_present)
        return ueCapabilityRAT_Container_internal_value;
    std::vector<uint8_t> tmp;
    return tmp;
}

int UE_CapabilityRAT_Container::ueCapabilityRAT_Container_SetValue(std::vector<uint8_t> value)
{
    ueCapabilityRAT_Container_internal_value = value;
    ueCapabilityRAT_Container_present = true;
    return 0;
}

std::string UE_CapabilityRAT_Container::ueCapabilityRAT_Container_ToString(uint32_t indent) const
{
    std::string out;
    for(uint32_t i=0; i<indent; i++)
        out += "    ";
    out += "ueCapabilityRAT_Container = {";
    for(auto byte : ueCapabilityRAT_Container_internal_value)
        out += std::to_string(byte) + ",";
    out = out.substr(0, out.length()-1) + "}\n";
    return out;
}

std::string UE_CapabilityRAT_Container::ueCapabilityRAT_Container_ToStringNoNewLines() const
{
    std::string out = "ueCapabilityRAT_Container={";
    for(auto byte : ueCapabilityRAT_Container_internal_value)
        out += std::to_string(byte) + ",";
    out = out.substr(0, out.length()-1) + "},";
    return out;
}

int UE_CapabilityRAT_Container::ueCapabilityRAT_Container_Clear()
{
    ueCapabilityRAT_Container_present = false;
    return 0;
}

bool UE_CapabilityRAT_Container::ueCapabilityRAT_Container_IsPresent() const
{
    return ueCapabilityRAT_Container_present;
}

