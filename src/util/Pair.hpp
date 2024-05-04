// Libstdc++ and GCC GNU Lincese boilerplate:      (this is required since this code is copyrighted under the GCC compiler. I did not design this myself.)
/*
  Partial Modified Implementation Of Meta-Functions from the <stl_pair> Libstdc++ header  -*- C++ -*-

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
  Pair.hpp [V1.0.0] (Template Header file for Pair Class Implementation)
    By: Andrey Stroitelev

  ~This file is to implement std::pair
  ~std::pair is a template class that just holds 2 elements of any type.
  ~The elements in the pair are accessed through the members first and second.
  ~These are used to represent 2 values that are associated with each other as a single variable.
    *Instead of something like {int a_pos_x, b_pos_x, a_pos_y, b_pos_y} you could do {std::pair<int, int> a_coord; std::pair<int, int> b_coord} with a_coord.first for x and a_coord.second for y.
    *These can also be used for a key value pair for example an int and a char to associate a point number with a grade letter
  ~This is the c++14 implementation of the std::pair class.
    *There are no comments labeling and deviding anything since this is all just one implementation.
*/


// Start of Pair.hpp
#ifndef _DLIB_PAIR_HPP_
#define _DLIB_PAIR_HPP_

#ifndef _DLIB_PAIR_VERSION_
#define _DLIB_PAIR_VERSION_ 0x010000
#endif


#include <Arduino.h>
#include "Config.h"
#include "Type_Traits.hpp"
#include "Move.hpp"


// Start of Pair.hpp Implementations:
namespace std {

  // forward declarations for stuff used by std::pair, but not yet defined
  struct piecewise_construct_t { };

  constexpr piecewise_construct_t piecewise_construct = piecewise_construct_t();

  template<class...>
  class tuple;

  namespace __detail { 
    template<size_t...>
    struct _Index_tuple;
  }

  template<class _T1, class _T2>
  struct pair {
    // type alias 
    typedef _T1 first_type;
    typedef _T2 second_type;

    // variables stored in the pair
    _T1 first;
    _T2 second;

    // Standard constructors
    constexpr pair() : first(), second() { }
    constexpr pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }


    // constructor that doesn't check if the different _U1 & _U2 types are convertible to _T1 & _T2
    //template<class _U1, class _U2>
    //pair(const pair<_U1, _U2>& __p) : first(__p.first), second(__p.second) { }


    // copy constructor for pair with different types
    template<class _U1, class _U2,
             class = enable_if_t<__and_<is_convertible<const _U1&, _T1>,
                                        is_convertible<const _U2&, _T2>
                                       >::value>>
    constexpr pair(const pair<_U1, _U2>& __p) : first(__p.first), second(__p.second) { }


    // default copy and move constructors
    constexpr pair(const pair&) = default;
    constexpr pair(pair&&) = default;


    // constructors for when the given parameters are a different type than the pair
    template<class _U1, 
             class = enable_if_t<is_convertible<_U1, _T1>::value>>
    constexpr pair(_U1&& __x, const _T2& __y) : first(std::forward<_U1>(__x)), second(__y) { }

    template<class _U2,
             class = enable_if_t<is_convertible<_U2, _T2>::value>>
    constexpr pair(const _T1& __x, _U2&& __y) : first(__x), second(std::forward<_U2>(__y)) { }

    template<class _U1, class _U2,
             class = enable_if_t<__and_<is_convertible<_U1, _T1>,
                                        is_convertible<_U2, _T2>
                                       >::value>>
    constexpr pair(_U1&& __x, _U2&& __y) : first(std::forward<_U1>(__x)), second(std::forward<_U2>(__y)) { }

    template<class _U1, class _U2,
             class = enable_if_t<__and_<is_convertible<_U1, _T1>,
                                        is_convertible<_U2, _T2>
                                       >::value>>
    constexpr pair(pair<_U1, _U2>&& __p) : first(std::forward<_U1>(__p.first)), second(std::forward<_U2>(__p.second)) { }


    // tuple constructor declared but not defined
    template<class... _Args1, class... _Args2>
    pair(piecewise_construct_t, tuple<_Args1...>, tuple<_Args2...>);

    
    // assignment operators:
    pair& operator=(const pair& __p) {
      first = __p.first;
      second = __p.second;
      return *this;
    }

    pair& operator=(pair&& __p) noexcept(__and_<is_nothrow_move_assignable<_T1>,
                                                is_nothrow_move_assignable<_T2>
                                               >::value) {
      first = std::forward<first_type>(__p.first);
      second = std::forward<second_type>(__p.second);
      return *this;
    }

    template<class _U1, class _U2>
    pair& operator=(const pair<_U1, _U2>& __p) {
      first = __p.first;
      second = __p.second;
      return *this;
    }

    template<class _U1, class _U2>
    pair& operator=(pair<_U1, _U2>&& __p) {
      first = std::forward<_U1>(__p.first);
      second = std::forward<_U2>(__p.second);
      return *this;
    }

    
    // member swap function:
    void swap(pair& __p) noexcept(noexcept(std::swap(first, __p.first)) &&
                                  noexcept(std::swap(second, __p.second))) {
      std::swap(first, __p.first);
      std::swap(second, __p.second);
    }

    private:
      template<class... _Args1, size_t... _Indexes1,
               class... _Args2, size_t... _Indexes2>
      pair(tuple<_Args1...>&, tuple<_Args2...>&,
           __detail::_Index_tuple<_Indexes1...>, 
           __detail::_Index_tuple<_Indexes2...>);
  };


  // Non-member boolean comparison operators:
  template<class _T1, class _T2>
  inline constexpr bool operator==(const pair<_T1, _T2>& __lhs, const pair<_T1, _T2>& __rhs) {
    return __lhs.first == __rhs.first && __lhs.second == __rhs.second;
  }

  template<class _T1, class _T2>
  inline constexpr bool operator<(const pair<_T1, _T2>& __lhs, const pair<_T1, _T2>& __rhs) {
    return __lhs.first < __rhs.first ||
           (!(__rhs.first < __lhs.first) && __lhs.second < __rhs.second);
  }

  template<class _T1, class _T2>
  inline constexpr bool operator!=(const pair<_T1, _T2>& __lhs, const pair<_T1, _T2>& __rhs) {
    return !(__lhs == __rhs);
  }

  template<class _T1, class _T2>
  inline constexpr bool operator>(const pair<_T1, _T2>& __lhs, const pair<_T1, _T2>& __rhs) {
    return __rhs < __lhs;
  }

  template<class _T1, class _T2>
  inline constexpr bool operator<=(const pair<_T1, _T2>& __lhs, const pair<_T1, _T2>& __rhs) {
    return !(__rhs < __rhs);
  }

  template<class _T1, class _T2>
  inline constexpr bool operator>=(const pair<_T1, _T2>& __lhs, const pair<_T1, _T2>& __rhs) {
    return !(__lhs < __rhs);
  }


  // std::swap function specialization for std::pair
  template<class _T1, class _T2>
  inline void swap(pair<_T1, _T2>& __a, pair<_T1, _T2>& __b) noexcept(noexcept(__a.swap(__b))) {
    __a.swap(__b);
  }


  // Start of std::make_pair implementation:
  template<class _T1, class _T2>
  inline pair<_T1, _T2> make_pair(_T1 __a, _T2 __b) {
    return pair<_T1, _T2>(__a, __b);
  }
  // End of std::make_pair

} // end of std:: namespace


#endif // End of Pair.hpp
