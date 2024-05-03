// Libstdc++ and GCC GNU Lincese boilerplate:      (this is required since this code is copyrighted under the GCC compiler. I did not design this myself.)
/*
  Partial Modified Implementation Of Meta-Functions from the <initializer_list> Libstdc++ header  -*- C++ -*-

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
  Initializer_List.hpp [V1.0.0] (Template header file for std::initializer_list)
    By: Andrey Stroitelev

  ~This is the implementation file for the std::initializer_list class.
  ~This is pretty much an exact copy of the implementation in libstdc++ initializer_list.h
  ~This class is a helper class that is a parameter pack that acts like a iterable container.
    *This makes using parameter packs for non-templated definitions alot easier, but it can be used with templates as well when you can't use parameter packs.
*/


// Start of Initializer_List.hpp
#ifndef _INITIALIZER_LIST_HPP_
#define _INITIALIZER_LIST_HPP_

#ifndef _INITIALIZER_LIST_VERSION_
#define _INITIALIZER_LIST_VERSION_ 0x010000
#endif


#include <Arduino.h>
#include "Config.h"


_DLIB_HEADER


// Start of std::initializer_list implementation
namespace std {

  template<class _E>
  class initializer_list {
    public:
      typedef _E         value_type;
      typedef const _E&  reference;
      typedef const _E&  const_reference;
      typedef size_t     size_type;
      typedef const _E*  iterator;
      typedef const _E*  const_iterator;

    private:
      iterator _M_array;
      size_type _M_len;

      constexpr initializer_list(const_iterator __a, size_type __l)
      : _M_array(__a), _M_len(__l) { }
    
    public:
      constexpr initializer_list() noexcept : _M_array(0), _M_len(0) { }

      constexpr size_type size() const noexcept { return _M_len; }

      constexpr const_iterator begin() const noexcept { return _M_array; }

      constexpr const_iterator end() const noexcept { return begin() + size(); }
  };


  // Specializations for std::begin and std::end
  template<class _Tp>
  constexpr const _Tp* begin(initializer_list<_Tp> __ils) noexcept {
    return __ils.begin();
  }

  template<class _Tp>
  constexpr const _Tp* end(initializer_list<_Tp> __ils) noexcept {
    return __ils.end();
  }

} // end of std:: namespace


#endif // end of Initializer_List.hpp
