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

#ifndef __SupportedBandListUTRA_TDD384_H__
#define __SupportedBandListUTRA_TDD384_H__

#include "SupportedBandUTRA_TDD384.h"

#include <array>
#include <cstdint>
#include <vector>

class SupportedBandListUTRA_TDD384
{
public:
    int Pack(std::vector<uint8_t> &bits);
    int Unpack(std::vector<uint8_t> bits);
    int Unpack(std::vector<uint8_t> bits, uint32_t &idx);
    std::vector<SupportedBandUTRA_TDD384> Value() const;
    int SetValue(std::vector<SupportedBandUTRA_TDD384>);
    std::string ToString(uint32_t indent) const;
    std::string ToStringNoNewLines() const;
private:
    std::vector<SupportedBandUTRA_TDD384> internal_value;
    bool present;
};

#endif
