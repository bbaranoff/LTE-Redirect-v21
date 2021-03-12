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

#include "DedicatedInfoCDMA2000.h"

#include "stdio.h"

#include <cmath>

int DedicatedInfoCDMA2000::Pack(std::vector<uint8_t> &bits)
{
    if(!present)
    {
        printf("DedicatedInfoCDMA2000::Pack() presence failure\n");
        return -1;
    }
    uint32_t local_size_length = 0;
    uint32_t local_size = internal_value.size();
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
    for(uint32_t i=0; i<internal_value.size(); i++)
        for(uint32_t j=0; j<8; j++)
            bits.push_back((internal_value[i] >> (8-j-1)) & 1);
    return 0;
}

int DedicatedInfoCDMA2000::Unpack(std::vector<uint8_t> bits)
{
    uint32_t idx = 0;
    return Unpack(bits, idx);
}

int DedicatedInfoCDMA2000::Unpack(std::vector<uint8_t> bits, uint32_t &idx)
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
    internal_value.clear();
    if((idx +size*8) > bits.size())
    {
        printf("DedicatedInfoCDMA2000::Unpack() index out of bounds\n");
        return -1;
    }
    for(uint32_t i=0; i<size; i++)
    {
        uint8_t value = 0;
        for(uint32_t j=0; j<8; j++)
            value |= bits[idx+j] << (8-j-1);
        internal_value.push_back(value);
        idx += 8;
    }
    present = true;
    return 0;
}

std::vector<uint8_t> DedicatedInfoCDMA2000::Value() const
{
    if(present)
        return internal_value;
    std::vector<uint8_t> tmp;
    return tmp;
}

int DedicatedInfoCDMA2000::SetValue(std::vector<uint8_t> value)
{
    internal_value = value;
    present = true;
    return 0;
}

std::string DedicatedInfoCDMA2000::ToString(uint32_t indent) const
{
    std::string out;
    for(uint32_t i=0; i<indent; i++)
        out += "    ";
    out += "DedicatedInfoCDMA2000 = {";
    for(auto byte : internal_value)
        out += std::to_string(byte) + ",";
    out = out.substr(0, out.length()-1) + "}\n";
    return out;
}

std::string DedicatedInfoCDMA2000::ToStringNoNewLines() const
{
    std::string out = "DedicatedInfoCDMA2000={";
    for(auto byte : internal_value)
        out += std::to_string(byte) + ",";
    out = out.substr(0, out.length()-1) + "},";
    return out;
}

int DedicatedInfoCDMA2000::Clear()
{
    present = false;
    return 0;
}

bool DedicatedInfoCDMA2000::IsPresent() const
{
    return present;
}

