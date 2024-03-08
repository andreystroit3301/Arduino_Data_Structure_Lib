/*
  Copyright (C) 2024 Andrey Stroitelev <email=andrey.stroitelev3301@gmail.com> (URL=https://github.com/andreystroit3301)

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

      http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
*/
/*
  {WIP}Data_Structure_Lib.hpp [V1.0.0]   (Library header file)
    By: Andrey Stroitelev

    Library Version: V1.0.0 // still adding data structures so there is no v1.0.0 yet
                      ^ ^ ^
     Major Changes  ~ | | |
     Header Changes   ~ | |
     Definition Changes ~ |

  ~this header file just consolidates all of the data structures in this library.
  ~Including this file will include all of the data structures with a single include statement

  ~The header Files for c++ std/stl library implementations are named the same as they are in the originally in the std/stl libraries, but I capitalized each word and put .hpp instead of .h for the extensions
    *example: instead of <type_traits.h> it would be <Type_Traits.hpp>
    *all of the names of the classes and functions in those files are still the same though, so std::initializer_list would still be std::initializer_list

  !WARNING! ~ You must use Arduino IDE 2.0.0 or higher for this library to work properly!
              Alot of the features used in this library only started being supported in arduino IDE V2.0.0
              Not using a supported version can cause errors, crashes, freezes, memory leaks, and reboot loops. There are more issues that can occur such as conflicts and in scope memory being overwritten but these are the main ones.
              This library started development using arduino IDE V2.2.0, and currently continuing development in IDE V2.3.2
              RECOMENDED: use the most recent version of the arduino IDE and you wont have to worry about anything.
*/


// start of header
#ifndef _DATA_STRUCTURE_LIB_HPP_
#define _DATA_STRUCTURE_LIB_HPP_


#include "Versions.h"
#include "Data_Structure_Util.hpp"


#ifndef _DATA_STRUCTURE_LIB_VERSION_
#define _DATA_STRUCTURE_LIB_VERSION_ 0x010000
#endif


#endif // end of header
