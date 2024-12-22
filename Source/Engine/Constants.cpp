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

#include "Constants.h"
//------------------------------------------------------------------------------

CONSTANTS Constants;
//------------------------------------------------------------------------------

using namespace std;
//------------------------------------------------------------------------------

CONSTANTS::CONSTANTS()
{
    Constants["e" ].Set_e ();
    Constants["π" ].Set_pi();
    Constants["pi"].Set_pi();
    Constants["i" ].Set_i ();
    Constants["j" ].Set_i ();

    time_t RawTime;
    struct tm* Time;

    time(&RawTime);
    Time = localtime(&RawTime);

    Constants["__YEAR__"   ] = Time->tm_year+1900;
    Constants["__MONTH__"  ] = Time->tm_mon+1;
    Constants["__DAY__"    ] = Time->tm_mday;
    Constants["__HOUR__"   ] = Time->tm_hour;
    Constants["__MINUTE__" ] = Time->tm_min;
    Constants["__SECOND__" ] = Time->tm_sec;

    Constants["__WEEKDAY__"] = ((Time->tm_wday+6)%7)+1;
    Constants["__YEARDAY__"] = Time->tm_yday+1;
}
//------------------------------------------------------------------------------

CONSTANTS::~CONSTANTS()
{
}
//------------------------------------------------------------------------------

bool CONSTANTS::GetConstant(const string& Name, NUMBER* Constant)
{
    auto Result = Constants.find(Name);
    if(Result == Constants.end()) return false;

    *Constant = Result->second;

    return true;
}
//------------------------------------------------------------------------------

