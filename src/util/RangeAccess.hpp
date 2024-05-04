// Libstdc++ and GCC GNU Lincese boilerplate:      (this is required since this code is copyrighted under the GCC compiler. I did not design this myself.)
/*
  Partial Modified Implementation Of Meta-Functions from the <bits/range_access> Libstdc++ header  -*- C++ -*-

  Copyright (C) 1997-2024 Free Software Foundation, Inc.

  This file is copying, modifying, and referenceing the GNU ISO C++ Library. 
  This library is free software; you can redistribute it and/or modify it under the
  terms of the GNU General Public License as published by the
  Free Software Foundation; either version 3, or (at your option)
  any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  Under Section 7 of GPL version 3, you are granted additional
  permissions described in the GCC Runtime Library Exception, version
  3.1, as published by the Free Software Foundation.

  You should have received a copy of the GNU General Public License and
  a copy of the GCC Runtime Library Exception along with this program;
  If not, see the NOTICE.txt and NOTICE_RUNTIME.txt files for copies the 
  libstdc++ and GCC Runtime licenses, or write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
  <Alternatively go to -> http://www.gnu.org/licenses/>


      99% of this code is not originally mine. These meta-functions are mostly either copies of the current implementation, 
      copies of an old implementation(C++11-14), or re-implementations of the original Libstdc++ library source code. 
      All of this code is thus also copyrighted by Free Software Foundation, Inc. under the GPLv3(GNU General Public License) open source license.
      All files containing this boilerplate include code from the C++ standard template library.
      For the full libstdc++ license please read the NOTICE file or go to the link at the bottom of this boilerplate.
*/ 
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
  RangeAccess.hpp  [V1.0.0]  (internal library range access utilities)
    By: Andrey Stroitelev

  ~This is just a copied over version of bits/range_access.h in libstdc++ (gcc-4.9.0)
  ~This file provides needed utilities for the container concept and for containers overall.
  ~This will most likely be a modified implementation since this is an embedded system running c++11 instead of c++14.
  ~This file also declares iterator related functions such as std::begin and std::end which container classes will have to speciallize/overload.
*/


// Start of RangeAccess.hpp
#ifndef _RANGE_ACCESS_HPP_
#define _RANGE_ACCESS_HPP_

#ifndef _RANGE_ACCESS_VERSION_
#define _RANGE_ACCESS_VERSION_ 0x010000
#endif


#include <Arduino.h>
#include "Config.h"


_DLIB_HEADER


// Start of range access utility implementations:
namespace std {

  template<class _Container>
  inline auto begin(_Container& __cont) -> decltype(__cont.begin()) {
    return __cont.begin();
  }

  template<class _Container>
  inline auto begin(const _Container& __cont) -> decltype(__cont.begin()) {
    return __cont.begin();
  }

  template<class _Container>
  inline auto end(_Container& __cont) -> decltype(__cont.end()) {
    return __cont.end();
  }

  template<class _Container>
  inline auto end(const _Container& __cont) -> decltype(__cont.end()) {
    return __cont.end();
  }

  template<class _Tp, size_t _Nm>
  inline _Tp* begin(_Tp (&__arr)[_Nm]) { return __arr; }

  template<class _Tp, size_t _Nm>
  inline _Tp* end(_Tp (&__arr)[_Nm]) { return __arr + _Nm; }

} // end of std:: namespace


#endif // End of RangeAccess.hpp
