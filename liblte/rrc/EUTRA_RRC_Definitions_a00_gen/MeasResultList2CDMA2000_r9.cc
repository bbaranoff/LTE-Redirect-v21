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

#include "MeasResultList2CDMA2000_r9.h"

#include <cmath>

#include "maxCellReport.h"

int MeasResultList2CDMA2000_r9::Pack(std::vector<uint8_t> &bits)
{
    if(!present)
    {
        printf("MeasResultList2CDMA2000_r9::Pack() presence failure\n");
        return -1;
    }
    uint32_t N_size_bits = 1;
    while(pow(2, N_size_bits) <= (maxCellReport - 1))
        N_size_bits++;
    uint32_t local_size = internal_value.size() - 1;
    for(uint32_t i=0; i<N_size_bits; i++)
        bits.push_back((local_size >> (N_size_bits-i-1)) & 1);
    for(auto item : internal_value)
        if(0 != item.Pack(bits))
        {
            printf("MeasResultList2CDMA2000_r9::Pack() item_pack_failure\n");
            return -1;
        }
    return 0;
}

int MeasResultList2CDMA2000_r9::Unpack(std::vector<uint8_t> bits)
{
    uint32_t idx = 0;
    return Unpack(bits, idx);
}

int MeasResultList2CDMA2000_r9::Unpack(std::vector<uint8_t> bits, uint32_t &idx)
{
    uint32_t N_size_bits = 1;
    while(pow(2, N_size_bits) <= (maxCellReport - 1))
        N_size_bits++;
    uint32_t size = 0;
    for(uint32_t i=0; i<N_size_bits; i++)
        size |= bits[idx++] << (N_size_bits-i-1);
    size += 1;
    internal_value.clear();
    for(uint32_t i=0; i<size; i++)
    {
        MeasResult2CDMA2000_r9 tmp;
        if(0 != tmp.Unpack(bits, idx))
        {
            printf("MeasResultList2CDMA2000_r9::Unpack() item unpack failure\n");
            return -1;
        }
        internal_value.push_back(tmp);
    }
    present = true;
    return 0;
}

std::vector<MeasResult2CDMA2000_r9> MeasResultList2CDMA2000_r9::Value() const
{
    if(present)
        return internal_value;
    return {};
}

int MeasResultList2CDMA2000_r9::SetValue(std::vector<MeasResult2CDMA2000_r9> value)
{
    if(value.size() < 1 || value.size() > maxCellReport)
    {
        printf("MeasResultList2CDMA2000_r9::SetValue() list size failure\n");
        return -1;
    }
    internal_value = value;
    present = true;
    return 0;
}

std::string MeasResultList2CDMA2000_r9::ToString(uint32_t indent) const
{
    std::string out;
    for(uint32_t i=0; i<indent; i++)
        out += "    ";
    out += "MeasResultList2CDMA2000_r9:\n";
    for(auto item : internal_value)
        out += item.ToString(indent+1);
    return out;
}

std::string MeasResultList2CDMA2000_r9::ToStringNoNewLines() const
{
    std::string out = "MeasResultList2CDMA2000_r9:";
    for(auto item : internal_value)
        out += item.ToStringNoNewLines();
    return out;
}

