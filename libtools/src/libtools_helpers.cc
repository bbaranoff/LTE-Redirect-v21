/*******************************************************************************

    Copyright 2017, 2021 Ben Wojtowicz

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*******************************************************************************

    File: libtools_helpers.cc

    Description: Contains all the implementations for helper functions.

    Revision History
    ----------    -------------    --------------------------------------------
    07/29/2017    Ben Wojtowicz    Created file
    02/14/2021    Ben Wojtowicz    Massive reformat and deprecated all *int*
                                   to_string in favor of std::to_string.

*******************************************************************************/

/*******************************************************************************
                              INCLUDES
*******************************************************************************/

#include "libtools_helpers.h"
#include <iomanip>
#include <sstream>
#include <sys/time.h>

/*******************************************************************************
                              DEFINES
*******************************************************************************/


/*******************************************************************************
                              TYPEDEFS
*******************************************************************************/


/*******************************************************************************
                              GLOBAL VARIABLES
*******************************************************************************/


/*******************************************************************************
                              FUNCTIONS
*******************************************************************************/

/*********************************************************************
    Name: to_string

    Description: Converts various types to a string.
*********************************************************************/
std::string to_string(uint64 value,
                      uint32 num_digits)
{
    std::stringstream tmp_ss;
    tmp_ss << std::setw(num_digits) << std::setfill('0') << value;
    return tmp_ss.str();
}
std::string to_string(uint8  *value,
                      uint32  num_bytes)
{
    std::string str;
    uint32      i;
    char        tmp_buf[10];

    for(i=0; i<num_bytes; i++)
    {
        snprintf(tmp_buf, 10, "%02X", value[i]);
        str += tmp_buf;
    }

    return str;
}

/*********************************************************************
    Name: to_number

    Description: Converts a string to various types.
*********************************************************************/
bool to_number(std::string  str,
               uint16      *number)
{
    return to_number(str, 0, number);
}
bool to_number(std::string  str,
               uint64      *number)
{
    return to_number(str, 0, number);
}
bool to_number(std::string str, int64 &number, int64 llimit, int64 ulimit)
{
    try
    {
        int64 tmp_value = std::stoi(str);
        if(tmp_value < llimit || tmp_value > ulimit)
            return true;
        number = tmp_value;
        return false;
    }catch(...){
        return true;
    }
}
bool to_number(std::string  str,
               uint32       num_digits,
               uint16      *number)
{
    uint64 num   = 0;
    bool   error = to_number(str, num_digits, &num);

    *number = num;

    return error;
}
bool to_number(std::string  str,
               uint32       num_digits,
               uint64      *number)
{
    uint32      N_digits = num_digits;
    const char *char_str = str.c_str();

    if(0 == N_digits)
        N_digits = str.length();
    if(!is_string_valid_as_number(str, N_digits, 10))
        return true;

    *number = 0;
    for(uint32 i=0; i<N_digits; i++)
    {
        *number *= 10;
        *number += char_str[i] - '0';
    }
    return false;
}
bool to_number(std::string  str,
               uint32       num_bytes,
               uint8       *number)
{
    const char *char_str = str.c_str();
    uint8       num;

    if(!is_string_valid_as_number(str, num_bytes*2, 16))
        return true;

    for(uint32 i=0; i<num_bytes*2; i++)
    {
        if((i % 2) == 0)
            number[i/2] = 0;

        if(char_str[i] >= '0' && char_str[i] <= '9')
        {
            num = char_str[i] - '0';
        }else if(char_str[i] >= 'A' && char_str[i] <= 'F'){
            num = (char_str[i] - 'A') + 0xA;
        }else{
            num = (char_str[i] - 'a') + 0xA;
        }

        if((i % 2) == 0)
            num <<= 4;

        number[i/2] |= num;
    }
    return false;
}

/*********************************************************************
    Name: get_formatted_time

    Description: Populates a string with a formatted time.
*********************************************************************/
void get_formatted_time(std::string &time_string)
{
    struct timeval  tv;
    struct tm      *local_time;
    time_t          tmp_time;

    tmp_time   = time(NULL);
    local_time = localtime(&tmp_time);
    gettimeofday(&tv, NULL);
    time_string += to_string(local_time->tm_mon + 1, 2) + "/";
    time_string += to_string(local_time->tm_mday, 2) + "/";
    time_string += to_string(local_time->tm_year + 1900, 4) + " ";
    time_string += to_string(local_time->tm_hour, 2) + ":";
    time_string += to_string((tv.tv_sec / 60) % 60, 2) + ":";
    time_string += to_string(tv.tv_sec % 60, 2) + ".";
    time_string += to_string(tv.tv_usec, 6);
}

/*********************************************************************
    Name: is_string_valid_as_number

    Description: Checks if the string is valid as a number (base 10
                 or 16) and is of a specific number of digits.
*********************************************************************/
bool is_string_valid_as_number(std::string str,
                               uint32      num_digits,
                               uint8       base)
{
    const char *c_str = str.c_str();

    if(str.length() != num_digits)
        return false;

    if(10 == base)
    {
        for(uint32 i=0; i<num_digits; i++)
            if(!(c_str[i] >= '0' && c_str[i] <= '9'))
                return false;
    }else if(16 == base){
        for(uint32 i=0; i<num_digits; i++)
            if(!((c_str[i] >= '0' && c_str[i] <= '9') ||
                 (c_str[i] >= 'a' && c_str[i] <= 'f') ||
                 (c_str[i] >= 'A' && c_str[i] <= 'F')))
                return false;
    }else{
        return false;
    }

    return true;
}

