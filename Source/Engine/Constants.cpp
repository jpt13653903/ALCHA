//==============================================================================
// Copyright (C) John-Philip Taylor
// jpt13653903@gmail.com
//
// This file is part of ALCHA
//
// This file is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>
//==============================================================================

#include "constants.h"
//------------------------------------------------------------------------------

Constants constants;
//------------------------------------------------------------------------------

using std::string;
//------------------------------------------------------------------------------

Constants::Constants()
{
    constants["e" ].set_e ();
    constants["π" ].set_pi();
    constants["pi"].set_pi();
    constants["i" ].set_i ();
    constants["j" ].set_i ();

    time_t rawTime;
    struct tm* _time;

    time(&rawTime);
    _time = localtime(&rawTime);

    constants["__YEAR__"   ] = _time->tm_year+1900;
    constants["__MONTH__"  ] = _time->tm_mon+1;
    constants["__DAY__"    ] = _time->tm_mday;
    constants["__HOUR__"   ] = _time->tm_hour;
    constants["__MINUTE__" ] = _time->tm_min;
    constants["__SECOND__" ] = _time->tm_sec;

    constants["__WEEKDAY__"] = ((_time->tm_wday+6)%7)+1;
    constants["__YEARDAY__"] = _time->tm_yday+1;
}
//------------------------------------------------------------------------------

Constants::~Constants(){}
//------------------------------------------------------------------------------

bool Constants::getConstant(const string& name, Number* constant)
{
    auto result = constants.find(name);
    if(result == constants.end()) return false;

    *constant = result->second;

    return true;
}
//------------------------------------------------------------------------------

