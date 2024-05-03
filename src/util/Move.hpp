// Libstdc++ and GCC GNU Lincese boilerplate:      (this is required since this code is copyrighted under the GCC compiler. I did not design this myself.)
/*
  Partial Modified Implementation Of Meta-Functions from the <bits/move> Libstdc++ header  -*- C++ -*-

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
  Move.hpp [V1.0.0] (Template Header file for Move Semantics Implementation)
    By: Andrey Stroitelev

  ~This file is for all of the implementations needed for move semantics
  ~This file is pretty much just bits/move.h from libstdc++
*/


// Start of Move.hpp
#ifndef _DLIB_MOVE_HPP_
#define _DLIB_MOVE_HPP_

#ifndef _DLIB_MOVE_VERSION_
#define _DLIB_MOVE_VERSION_ 0x010000
#endif


#include <Arduino.h>
#include "Config.h"
#include "Type_Traits.hpp"
#include "Concept_Check.hpp"


// Start of Move.hpp implementations in std:: namespace
namespace std _DLIB_VISIBILITY {

  // Start of std::forward implementation:
  template<class _Tp>
  [[nodiscard]]
  constexpr _Tp&& forward(typename remove_reference<_Tp>::type& __t) noexcept {
    return static_cast<_Tp&&>(__t);
  }

  template<class _Tp>
  [[nodiscard]]
  constexpr _Tp&& forward(typename remove_reference<_Tp>::type&& __t) noexcept {
    static_assert(!is_lvalue_reference<_Tp>::value, 
      "Cannot forward an rvalue reference as an lvalue reference!");
    return static_cast<_Tp&&>(__t);
  }

  // I have been unable to successfully implement this function without having to edit a cmake file.
  // This function uses decltype(auto) as the return type in libstdc++ which throws an error unless you add a couple lines to the cmake file which would make this library much more cumbersome to use.
  // I also cant return auto&& since this requires a trailing return type I can't provide.
  // In this case I'm going to comment this out until I can find a solution.
  /*template<class _Tp, class _Up>
  [[nodiscard]]
  constexpr decltype(auto) forward_like(_Up&& __x) noexcept { // not sure if I will keep this functions
    constexpr bool __as_rval = is_rvalue_reference<_Tp&&>::value;

    if constexpr (is_const<remove_reference_t<_Tp>>) {
      using _Up2 = remove_reference_t<_Up>;
      if constexpr (__as_rval) return static_cast<const _Up2&&>(__x);
      else return static_cast<const _Up2&>(__x);
    } else {
      if constexpr (__as_rval) return static_cast<remove_reference_t<_Up>&&>(__x);
      else return static_cast<_Up&>(__x);
    }

  }*/

  // Possible alternate implementation for std::forward_like:
  namespace __detail { // for internal parts of the implementation

    template<class _Tp, class _Up>
    using __override_ref_t = conditional_t<is_rvalue_reference<_Tp>::value,
                                           remove_reference_t<_Up>&&, _Up&>;
    
    template<class _Tp, class _Up>
    using __copy_const_t = conditional_t<is_const<remove_reference_t<_Tp>>::value,
                                         _Up const, _Up>;

    template<class _Tp, class _Up>
    using __forward_like_t = __override_ref_t<_Tp&&,
                                              __copy_const_t<_Tp, remove_reference_t<_Up>>>;

  } // end of __detail:: namespace

  template<class _Tp, class _Up>
  [[nodiscard]]
  constexpr auto forward_like(_Up&& __x) noexcept -> __detail::__forward_like_t<_Tp, _Up> {
    return static_cast<__detail::__forward_like_t<_Tp, _Up>>(__x);
  }
  // End of std::forward 


  // Start of std::__like_t internal implementation:
  template<class _Tp, class _Up>
  using __like_t = decltype(forward_like<_Tp>(declval<_Up>()));
  // End of std::__like_t


  // Start of std::move implementation:
  template<class _Tp>
  [[nodiscard]]
  constexpr typename remove_reference<_Tp>::type&& move(_Tp&& __t) noexcept {
    return static_cast<typename remove_reference<_Tp>::type&&>(__t);
  }

  namespace __detail {

    template<class _Tp>
    struct __move_if_noexcept_cond 
      : public __and_<__not_<is_nothrow_move_constructible<_Tp>>,
                      is_copy_constructible<_Tp>>::type { };
    
  }
  
  template<class _Tp>
  [[nodiscard]]
  constexpr conditional_t<__detail::__move_if_noexcept_cond<_Tp>::value, const _Tp&, _Tp&&>
  move_if_noexcept(_Tp& __x) noexcept {
    return move(__x);
  }
  // End of std::move


  // Start of std::addressof implementation:
  namespace __detail { // using __detail:: to keep this function private

    template<class _Tp>
    inline constexpr _Tp* __addressof(_Tp& __r) noexcept {
      return __builtin_addressof(__r);
    }

  } // end of __detail::

  template<class _Tp>
  [[nodiscard]]
  inline constexpr _Tp* addressof(_Tp& __r) noexcept {
    return __detail::__addressof(__r);
  }

  template<class _Tp>
  const _Tp* addressof(const _Tp&&) = delete;
  // End of std::addressof


  // Start of internal __exchange function implementation:
  namespace __detail { // making this implementation private

    template<class _Tp, class _Up = _Tp>
    inline constexpr _Tp __exchange(_Tp& __obj, _Up&& __new) {
      _Tp __old = move(__obj);
      __obj = forward<_Up>(__new);
      return __old;
    }

  } // end of __detail:: namespace
  // End of __exchange


  // Start of std::swap base version implementations:
  /*template<class _Tp>
  inline constexpr typename enable_if_t<__and_<__not_<__detail::__is_tuple_like<_Tp>>,
                                               is_move_constructible<_Tp>,
                                               is_move_assignable<_Tp>
                                               >::value>::type
  swap(_Tp& __a, _Tp& __b) noexcept(__and_<is_nothrow_move_constructible<_Tp>,
                                           is_nothrow_move_constructible<_Tp>>::value) {
    _Tp __tmp = move(__a);
    __a = move(__b);
    __b = move(__tmp);
  }

  template<class _Tp, size_t _Nm>
  inline constexpr typename enable_if<__detail::__is_swappable<_Tp>::value>::type
  swap(_Tp (&__a)[_Nm], _Tp (&__b)[_Nm]) noexcept(__detail::__is_nothrow_swappable<_Tp>::value) {
    for(size_t __n = 0; __n < _Nm; ++__n) swap(__a[__n], __b[__n]);
  }*/
  // using c++14 implementation due to the above implementation not working
  template<class _Tp>
  inline void swap(_Tp& __a, _Tp& __b) noexcept(__and_<is_nothrow_move_constructible<_Tp>,
                                                       is_nothrow_move_constructible<_Tp>>::value) {
    __glibcxx_function_requires(_SGIAssignableConcept<_Tp>)

    _Tp __tmp = move(__a);
    __a = move(__b);
    __b = move(__tmp);
  }

  template<class _Tp, size_t _Nm>
  inline void swap(_Tp (&__a)[_Nm], _Tp (&__b)[_Nm]) noexcept(noexcept(swap(*__a, *__b))) {
    for(size_t __n = 0; __n < _Nm; ++__n) swap(__a[__n], __b[__n]);
  }
  // End of std::swap


  // Start of basic std::iter_move implementation(unspecialized):
  // This implementation might be removed in favor of only specialized versions for each of the iterator categories.
  // I may also move this function to Iterator.hpp or Iterator_Traits.hpp if I dont end up removing this implementation.
  template<class _Iter>
  inline constexpr auto iter_move(_Iter&& __it) 
  noexcept(noexcept(*__it)) -> decltype(move(*__it)) {
    return move(*__it);
  }
  // End of std::iter_move

} // end of std:: namespace


// Macros for internal use and testing:
#if (_CXX_OLD_CODE)
#  define _GLIBCXX_MOVE(__val) (__val)
#  define _GLIBCXX_FORWARD(_Tp, __val) (__val)
#else
#  define _GLIBCXX_MOVE(__val) std::move(__val)
#  define _GLIBCXX_FORWARD(_Tp, __val) std::forward<_Tp>(__val);
#endif


#endif // End of Move.hpp
