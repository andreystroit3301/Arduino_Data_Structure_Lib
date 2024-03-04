// GCC GNU Lincese boilerplate:      (this is required since this code is copyrighted under the GCC compiler. I did not design this myself.)
/*
  Copyright (C) 2007-2013 Free Software Foundation, Inc.

  This file is part of the GNU ISO C++ Library.  This library is free
  software; you can redistribute it and/or modify it under the
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
  see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
  <http://www.gnu.org/licenses/>.
*/
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
  InitializerList.hpp [V1.1.0] (template class header/declaration file)
    By: Andrey Stroitelev

  ~This is a recreation of std::initializer_list for the arduino ide
  ~This utility function allows you to make brace enclosed initializer list constructors
    *for example: Vector<int> v = {12, 54, 123, 54};    ||    Map<int, char> m = {{1, 'a'}, {2, 'b'}, {3, 'c'}};

  ~This file doesn't include any of the initializer_list implementations for any other c++ compiler like Clang and others since the arduino IDE just uses the GCC compiler.
    *I may eventually add support for other compilers using preprocessor directives, but I currently dont see any scenario where someone would compile code for an arduino using a different compiler, and my Data_Structure_Lib library is designed for AVR boards since other boards like ESP boards natively support the std/stl libraries.

  ~Also if anyone sees this and knows why it lets me add inline to constexpr here but not anywhere else please let me know why.
    *I'm leaving in the inline keywords even though in this current version of the IDE constexpr is implicitly inline which typically gives an error or warning when compiled which it doesn't do here for some reason.
*/


// Start of Initializer_List.hpp
#ifndef _INITIALIZER_LIST_HPP_
#define _INITIALIZER_LIST_HPP_

#ifndef _INITIALIZER_LIST_VERSION_
#define _INITIALIZER_LIST_VERSION_ 0x010100
#endif


#include <Arduino.h>


// Start of std:: namespace:
namespace std {

  // Start of std::initializer_list implementation:
  template<class _Ep>
  class initializer_list {
    const _Ep* __begin_;
    size_t __size_;

    inline constexpr initializer_list(const _Ep* __b, size_t __s) noexcept : __begin_(__b), __size_(__s) {  }

    public:
      typedef _Ep value_type;
      typedef const _Ep& reference;
      typedef const _Ep& const_reference;
      typedef size_t size_type;
      typedef const _Ep* iterator;
      typedef const _Ep* const_iterator;

      inline constexpr initializer_list(void) noexcept : __begin_(nullptr), __size_(0) { }

      inline constexpr size_t size(void) const noexcept { return __size_; }

      inline constexpr const _Ep* begin(void) const noexcept { return __begin_; }
      inline constexpr const _Ep* end(void) const noexcept { return __begin_ + __size_; }
  };
  // End of std::initializer_list implementation

} // End of std:: namespace


// Global specializations for begin() and end() iterator functions:
template<class _Ep>
constexpr const _Ep* begin(std::initializer_list<_Ep> __il) noexcept { return __il.begin(); }

template<class _Ep>
constexpr const _Ep* end(std::initializer_list<_Ep> __il) noexcept { return __il.end(); }
// End of begin() and end() specializations


#endif // End of Initializer_List.hpp
