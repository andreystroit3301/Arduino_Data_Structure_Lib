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
  {WIP}Data_Structure_Lib.hpp [V1.0.0]   (Library header file)
    By: Andrey Stroitelev

    Library Version: V1.0.0 // still adding data structures so there is no v1.0.0 yet
                      ^ ^ ^
     Major Changes  ~ | | |
     Header Changes   ~ | |
     Definition Changes ~ |

  ~this header file just consolidates all of the data structures in this library.
  ~Including this file will include all of the data structures with a single include statement
  ~The std::pair, std::tuple, and std::array classes will be in the util folder and included in Data_Structure_Util.hpp


  ~The header Files for c++ std/stl library implementations are named the same as they are in the originally in the std/stl libraries, but I capitalized each word and put .hpp instead of .h for the extensions
    *example: instead of <type_traits.h> it would be <Type_Traits.hpp>
    *all of the names of the classes and functions in those files are still the same though, so std::initializer_list would still be std::initializer_list


  !WARNING! ~ You must use Arduino IDE 2.0.0 or higher for this library to work properly!
              Alot of the features used in this library only started being supported in arduino IDE V2.0.0
              Not using a supported version can cause errors, crashes, freezes, memory leaks, and reboot loops. There are more issues that can occur such as conflicts and in scope memory being overwritten but these are the main ones.
              This library started development using arduino IDE V2.2.0, and currently continuing development in IDE V2.3.2
              RECOMENDED: use the most recent version of the arduino IDE and you wont have to worry about anything.
*/
/*
  Library Namespace Layout:
  NOTE: Users must not define/declare, specialize, or overload anything in the std namespace or any of it's nested namespaces!
        If you define/declare, specialize, or overload anything in the library namespaces it will make the library no longer function correctly
  
  namespace std { // main namespace to use the library. Everything in the library will be in the std namespace or a namespace netsted in std.

    namespace __detail {
      // private stuff used internally by the library
      inline namespace __H { // I will most likely get rid of this namespace as I only used it once.
        // inline namespace for helper implementations when defining a private method that stays in __detail namespace(essentially for helpers that arent detecated to a specific std method.)
        // for example this namespace will have helpers for meta-functions such as std::__detail::__is_implicitly_default_constructible which doesn't have any std::is_implicitly_default_constructible counter-part.
      }
    }

    // I might get rid of these namespaces since I moving towards c++14 libstdc++ implementations instead of the current c++23 implementations because I wasn't able to make the newer implementations work properly.
    namespace ranges {
      // public namespace for certain iteration and data structure base functions.
      // Most of the functions in this namespace will have public specializations in the std namespace which should be used instead of the base functions in this namespace if possible

      inline namespace __H { // will remove along with ranges
        // inline namespace for certain generalized custom implementations.
        // mainly for ranges::swap, ranges::move, etc.
      }
    }

    |conditionally inline| namespace __debug {
        This namespace becomes inline when debug mode is enabled via the debugging toggle macro in Config.h
        Normally this namespace needs to be manually accessed with std::__debug::, but when 
        debug mode is enabled all normal container implementations will go into the __cxxstd::.
        Debug mode works by essentially changing the namespace that certain classes are in to std::__cxxstd::, and
        then the debug versions of the classes is made accessible from just std:: by inlining this __debug:: namespace.
        This way you don't need to change any code.
        When you enable debug mode every container class will automatically use the debugging implementation instead.
        
      // this namespace is used for the public debugging functions made for the library.
      // the debug functions in this namespace can be used by the user, but it's mainly for internal use.
    }

  }

  namespace __gnu_cxx {
    // This namespace is used for more internal language extension implementations outside of the standard std:: and std::__detail:: namespaces.
    // This namespace is only used internally by the library for certain implementations in the util folder header files.
    namespace __ops { 
      // this namespace is dedicated to defining a set of default comparison predicates mainly for iterators.
      // This namespace is only found in UtilityBase.hpp for the STL bits/predefined_ops.h implementation
    }
  }

  namespace __gnu_debug {
    // This namespace is used for all of the internal debugging implementations.
    // This is a dedicated namespace for debugging which uses debug code from std::__debug
    [ using namespace std::__debug; ]
  }

  namespace util {
    // another public library namespace seperate from the std:: namespace.
    // This namespace is only for the features in the ArduinoUtil file.
    // This will store some features unrelated to the rest of the data structure lib.
    // These features are purely for quality of life when using an arduino by giving functions that let you see the amount of free memory for dynamic allocations during runtime along with some other functions I have planned that will speed up certain arduino functions.
  }
*/


// start of header
#ifndef _DATA_STRUCTURE_LIB_HPP_
#define _DATA_STRUCTURE_LIB_HPP_


#include "Versions.h"
#include "util/Config.h"
#include "Data_Structure_Util.hpp"
#include "util/Pair.hpp"
#include "util/Tuple.hpp"
#include "util/Array.hpp"
#include "Vector.hpp"


#ifndef _DATA_STRUCTURE_LIB_VERSION_
#define _DATA_STRUCTURE_LIB_VERSION_ 0x010000
#endif


#endif // end of header
