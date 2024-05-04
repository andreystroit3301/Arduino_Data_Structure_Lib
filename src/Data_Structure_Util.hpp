/*
  Copyright (C) 2024 Andrey Stroitelev <email=andrey.stroitelev3301@gmail.com> (URL=NA)

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
  Data_Structure_Utility.hpp [V1.0.0]   (Library header file)
    By: Andrey Stroitelev

    Library Version: V1.0.0 // still adding data structures so there is no v1.0.0 yet
                      ^ ^ ^
     Major Changes  ~ | | |
     Header Changes   ~ | |
     Definition Changes ~ |

  ~This header file consolidates all of the low level abstract base classes that are used as an interface for the data structures in this library.
  ~This header file also include utility classes such as std::move() to allow for move semantics in the arduino IDE(which is not natively included along with the rest of the std namespace)
  ~This is just to consolidate all of the helper files I'm using for the implementation of the data structures in this library
    *Including the main Data_Structure_Lib.hpp will also include everything included in this file

  ~If you have compiler warnings enabled you will get a warning for some variable templates which it says arent supported, but they function just fine and as intended.
*/


// Start of header
#ifndef _DATA_STRUCTURE_UTIL_HPP_
#define _DATA_STRUCTURE_UTIL_HPP_


// I will eventually organize these #include statments
#include "Versions.h"
#include "util/Config.h"
#include "util/ArduinoUtil.h"
#include "util/Move.hpp"
#include "util/Invoke.hpp"
#include "util/Stdexcept.hpp"
#include "util/UtilityBase.hpp"
#include "util/Debug.hpp"
#include "util/RangeAccess.hpp"
#include "util/AlgoBase.hpp"
#include "util/Array.hpp"
#include "util/Pair.hpp"
#include "util/Tuple.hpp"
#include "util/Type_Traits.hpp"
#include "util/Concept_Check.hpp"
#include "util/Concepts.hpp"
#include "util/Initializer_List.hpp"
#include "util/Iterator_Traits.hpp"
#include "util/Iterator_Funcs.hpp"
#include "util/Iterator.hpp"
#include "util/Allocator.hpp"
#include "util/Alloc_Traits.hpp"
#include "util/Utility.hpp"


#ifndef _DATA_STRUCTURE_UTIL_VERSION_
#define _DATA_STRUCTURE_UTIL_VERSION_ 0x010000
#endif


#endif // end of header
