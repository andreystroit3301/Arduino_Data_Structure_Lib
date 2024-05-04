// Libstdc++ and GCC GNU Lincese boilerplate:      (this is required since this code is copyrighted under the GCC compiler. I did not design this myself.)
/*
  Partial Modified Implementation Of Meta-Functions from the <utility/uses_allocator/rel_ops/> Libstdc++ header  -*- C++ -*-

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
  UtilityBase.hpp [V1.0.0] (Base Template Utilities for rest of Util folder)
    By: Andrey Stroitelev

  ~This file just has some base utilities used by a number of files in the util folder.
  ~The implementations in this file are from different c++ standards, so some parts are from c++11, some from c++14, and some from the current standard.
  ~This file contains implementations from multiple libstdc++ files
*/


// Start of UtilityBase.hpp
#ifndef _DLIB_UTILITY_BASE_HPP_
#define _DLIB_UTILITY_BASE_HPP_

#ifndef _DLIB_UTILITY_BASE_VERSION_
#define _DLIB_UTILITY_BASE_VERSION_ 0x010000
#endif


#include <Arduino.h>
#include "Config.h"
#include "Type_Traits.hpp"
#include "Pair.hpp"
#include "Move.hpp"
#include "Initializer_List.hpp"
#include "Iterator_Traits.hpp"


_DLIB_HEADER


// Start of UtilityBase.hpp implementations:
namespace std _DLIB_VISIBILITY {

  // Start of stl_relops implementations:
  // NOTE: I'm putting this here as I start on this file, but I most likely wont need these and will get rid of them
  // Note to self: If i end up keeping this I need to add this namespace to the note in Data_Structure_Lib.hpp
  /*namespace rel_ops { // namespace for relational operator definitions

    template<class _Tp>
    inline constexpr bool operator!=(const _Tp& __x, const _Tp& __y) {
      return !(__x == __y);
    }

    template<class _Tp>
    inline constexpr bool operator>(const _Tp& __x, const _Tp& __y) {
      return __y < __x;
    }

    template<class _Tp>
    inline constexpr bool operator<=(const _Tp& __x, const _Tp& __y) {
      return !(__y < __x);
    }

    template<class _Tp>
    inline constexpr bool operator>=(const _Tp& __x, const _Tp& __y) {
      return !(__x < __y);
    }

  }*/ // end of rel_ops:: namespace
  // End of stl_relops implementations:


  // Start of c++14 implementation of utility.h (mainly for tuple class)
  
  template<class _Tp>
  class tuple_size;

  template<size_t _Int, class _Tp>
  class tuple_element;

  template<class _Tp1, class _Tp2>
  struct tuple_size<pair<_Tp1, _Tp2>>
    : public integral_constant<size_t, 2> { };

  template<class _Tp1, class _Tp2>
  struct tuple_element<0, pair<_Tp1, _Tp2>> { typedef _Tp1 type; };

  template<class _Tp1, class _Tp2>
  struct tuple_element<1, pair<_Tp1, _Tp2>> { typedef _Tp2 type; };

  namespace __detail { // namespace for internal implementations

    template<size_t _Int>
    struct __pair_get;

    template<>
    struct __pair_get<0> {
      template<class _Tp1, class _Tp2>
      static constexpr _Tp1& __get(pair<_Tp1, _Tp2>& __pair) noexcept {
        return __pair.first;
      }

      template<class _Tp1, class _Tp2>
      static constexpr _Tp1&& __move_get(pair<_Tp1, _Tp2>&& __pair) noexcept {
        return std::forward<_Tp1>(__pair.first);
      }

      template<class _Tp1, class _Tp2>
      static constexpr const _Tp1& __const_get(const pair<_Tp1, _Tp2>& __pair) noexcept {
        return __pair.first;
      }
    };

    template<>
    struct __pair_get<1> {
      template<class _Tp1, class _Tp2>
      static constexpr _Tp2& __get(pair<_Tp1, _Tp2>& __pair) noexcept {
        return __pair.second;
      }

      template<class _Tp1, class _Tp2>
      static constexpr _Tp2&& __move_get(pair<_Tp1, _Tp2>&& __pair) noexcept {
        return std::forward<_Tp2>(__pair.second);
      }

      template<class _Tp1, class _Tp2>
      static constexpr const _Tp2& __const_get(const pair<_Tp1, _Tp2>& __pair) noexcept {
        return __pair.second;
      }
    };

  } // end of __detail:: namespace

  template<size_t _Int, class _Tp1, class _Tp2>
  constexpr typename tuple_element<_Int, pair<_Tp1, _Tp2>>::type&
  get(pair<_Tp1, _Tp2>& __in) noexcept {
    return __detail::__pair_get<_Int>::__get(__in);
  }

  template<size_t _Int, class _Tp1, class _Tp2>
  constexpr typename tuple_element<_Int, pair<_Tp1, _Tp2>>::type&&
  get(pair<_Tp1, _Tp2>&& __in) noexcept {
    return __detail::__pair_get<_Int>::__move_get(std::move(__in));
  }

  template<size_t _Int, class _Tp1, class _Tp2>
  constexpr const typename tuple_element<_Int, pair<_Tp1, _Tp2>>::type&
  get(const pair<_Tp1, _Tp2>& __in) noexcept {
    return __detail::__pair_get<_Int>::__const_get(__in);
  }

# if(USING_CPP14_METHOD) // in libstdc++ preprocessor checks for __cplusplus > 201103L. I might just remove the macro check to make this a part of the complete implementation
  
  template<class _Tp, class _Up>
  constexpr _Tp& get(pair<_Tp, _Up>& __p) noexcept { return __p.first; }

  template<class _Tp, class _Up>
  constexpr const _Tp& get(const pair<_Tp, _Up>& __p) noexcept { return __p.first; }

  template<class _Tp, class _Up>
  constexpr _Tp&& get(pair<_Tp, _Up>&& __p) noexcept { return std::move(__p.first); }

  template<class _Tp, class _Up>
  constexpr _Tp& get(pair<_Up, _Tp>& __p) noexcept { return __p.second; }

  template<class _Tp, class _Up>
  constexpr const _Tp& get(const pair<_Up, _Tp>& __p) noexcept { return __p.second; }

  template<class _Tp, class _Up>
  constexpr _Tp&& get(pair<_Up, _Tp>&& __p) noexcept { return std::move(__p.second); }

  template<class _Tp, class _Up = _Tp>
  inline _Tp exchange(_Tp& __obj, _Up&& __new_val) {
    _Tp __old_val = std::move(__obj);
    __obj = std::forward<_Up>(__new_val);
    return __old_val;
  }

# endif

  namespace __detail { // internal implementation namespace

    template<size_t... _Indexes>
    struct _Index_tuple { 
      typedef _Index_tuple<_Indexes..., sizeof...(_Indexes)> __next; 
    };

    template<size_t _Num>
    struct _Build_index_tuple {
      typedef typename _Build_index_tuple<_Num - 1>::__type::__next __type;
    };

    template<>
    struct _Build_index_tuple<0> { typedef _Index_tuple<> __type; };

  } // end of __detail:: namespace

# if(USING_CPP14_METHOD)
  template<class _Tp, _Tp... _Idx>
  struct integer_sequence {
    typedef _Tp value_type;
    static constexpr size_t size() { return sizeof...(_Idx); }
  };

  namespace __detail { // internal implementation namespace

    template<class _Tp, _Tp _Num,
             typename _ISeq = typename _Build_index_tuple<_Num>::__type>
    struct _Make_integer_sequence;

    template<class _Tp, _Tp _Num, size_t... _Idx>
    struct _Make_integer_sequence<_Tp, _Num, _Index_tuple<_Idx...>> {
      static_assert(_Num >= 0, "Cannot make an integer sequence with a negative length!");
      typedef integer_sequence<_Tp, static_cast<_Tp>(_Idx)...> __type;
    };

  } // end of __detail:: namespace

  template<class _Tp, _Tp _Num>
  using make_integer_sequence = typename __detail::_Make_integer_sequence<_Tp, _Num>::__type;

  template<size_t... _Idx>
  using index_sequence = integer_sequence<size_t, _Idx...>;

  template<size_t _Num>
  using make_index_sequence = make_integer_sequence<size_t, _Num>;

  template<class... _Types>
  using index_sequence_for = make_index_sequence<sizeof...(_Types)>;
# endif

  // End of c++14 implementation of utility.h


  // Start of current uses_allocator.h implementation:

# if(USES_ALLOC_CPP14)

  struct allocator_arg_t { };

  constexpr allocator_arg_t allocator_arg = allocator_arg_t();

  namespace __detail { // internal implementation namespace

    _GLIBCXX_HAS_NESTED_TYPE(allocator_type)

    template<class _Tp, class _Alloc,
             bool = __has_allocator_type<_Tp>::value>
    struct __uses_allocator_helper : public false_type { };

    template<class _Tp, class _Alloc>
    struct __uses_allocator_helper<_Tp, _Alloc, true>
      : public integral_constant<bool, is_convertible<_Alloc, typename _Tp::allocator_type>::value> { };

  } // end of __detail:: namespace

  template<class _Tp, class _Alloc>
  struct uses_allocator
    : public integral_constant<bool, __detail::__uses_allocator_helper<_Tp, _Alloc>::value> { };

  namespace __detail { // internal implementation namespace

    template<class _Tp, class _Alloc, class... _Args>
    struct __uses_allocator_arg
      : public is_constructible<_Tp, _Alloc, _Args...> {
      static_assert(uses_allocator<_Tp, _Alloc>::value, "need and allocator for methods that require it!");
    };

    struct __uses_alloc_base { };
    
    struct __uses_alloc0 : public __uses_alloc_base {
      struct _Anything { _Anything(...) { } } _M_a; 
    };

    template<class _Alloc>
    struct __uses_alloc1 : public __uses_alloc_base { const _Alloc* _M_a; };

    template<class _Alloc>
    struct __uses_alloc2 : public __uses_alloc_base { const _Alloc* _M_a; };

    template<bool, class _Alloc, class... _Args>
    struct __uses_alloc;

    template<class _Tp, class _Alloc, class... _Args>
    struct __uses_alloc<true, _Tp, _Alloc, _Args...>
      : public conditional<is_constructible<_Tp, allocator_arg_t, _Alloc, _Args...>::value,
                           __uses_alloc1<_Alloc>,
                           __uses_alloc2<_Alloc>>::type { };
    
    template<class _Tp, class _Alloc, class... _Args>
    struct __uses_alloc<false, _Tp, _Alloc, _Args...> : public __uses_alloc0 { };

    template<class _Tp, class _Alloc, class... _Args>
    struct __uses_alloc_impl
      : public __uses_alloc<uses_allocator<_Tp, _Alloc>::value, _Tp, _Alloc, _Args...> { };
    
    template<class _Tp, class _Alloc, class... _Args>
    __uses_alloc_impl<_Tp, _Alloc, _Args...>
    __use_alloc(const _Alloc& __a) {
      __uses_alloc_impl<_Tp, _Alloc, _Args...> __ret;
      __ret._M_a = &__a;
      return __ret;
    }

  } // end of __detail:: namespace

# endif // end of uses_allocator c++14 implementation

# if(USES_ALLOC_CPP23)

  namespace __detail { // namespace for internal implementations

    struct __erased_type { };

    template<class _Alloc, class _Tp>
    using __is_erased_or_convertible = __or_<is_convertible<_Alloc, _Tp>,
                                             is_same<_Tp, __erased_type>>;

  } // end of __detail:: namespace

  struct allocator_arg_t { explicit allocator_arg_t() = default; };

  inline constexpr allocator_arg_t allocator_arg = allocator_arg_t();

  namespace __detail { // continuing private implementation

    template<class _Tp, class _Alloc, class = void_t<>>
    struct __uses_allocator_helper : public false_type { };

    template<class _Tp, class _Alloc>
    struct __uses_allocator_helper<_Tp, _Alloc, void_t<typename _Tp::allocator_type>>
      : __is_erased_or_convertible<_Alloc, typename _Tp::allocator_type>::type { };

  } // end of __detail:: namespace

  template<class _Tp, class _Alloc>
  struct uses_allocator : public __detail::__uses_allocator_helper<_Tp, _Alloc>::type { };

  namespace __detail { // more internal implementation stuff

    struct __uses_alloc_base { };

    struct __uses_alloc0 : public __uses_alloc_base {
      struct _Sink { void /*constexpr*/ operator=(const void*) { } } _M_a; // this operator is supposed to be constexpr, but it throws an error.
    };

    template<class _Alloc>
    struct __uses_alloc1 : public __uses_alloc_base { const _Alloc* _M_a; };

    template<class _Alloc>
    struct __uses_alloc2 : public __uses_alloc_base { const _Alloc* _M_a; };

    template<bool, class _Tp, class _Alloc, class... _Args>
    struct __uses_alloc;

    template<class _Tp, class _Alloc, class... _Args>
    struct __uses_alloc<true, _Tp, _Alloc, _Args...>
      : public conditional_t<is_constructible<_Tp, allocator_arg_t, const _Alloc&, _Args...>::value,
                             __uses_alloc1<_Alloc>,
                             __uses_alloc2<_Alloc>> {
      static_assert(__or_<is_constructible<_Tp, allocator_arg_t, const _Alloc&, _Args...>,
                          is_constructible<_Tp, _Args..., const _Alloc&>>::value,
        "construction with an allocator must be possible if uses_allocator is true!");
    };

    template<class _Tp, class _Alloc, class... _Args>
    struct __uses_alloc<false, _Tp, _Alloc, _Args...> : public __uses_alloc0 { };

    template<class _Tp, class _Alloc, class... _Args>
    using __uses_alloc_t = __uses_alloc<uses_allocator<_Tp, _Alloc>::value, _Tp, _Alloc, _Args...>;

    template<class _Tp, class _Alloc, class... _Args>
    constexpr inline __uses_alloc_t<_Tp, _Alloc, _Args...>
    __use_alloc(const _Alloc& __a) {
      __uses_alloc_t<_Tp, _Alloc, _Args...> __ret;
      __ret._M_a = __detail::__addressof(__a);
      return __ret;
    }

    template<class _Tp, class _Alloc, class... _Args>
    void __use_alloc(const _Alloc&&) = delete;

  } // end of __detail:: namespace

  template<class _Tp, class _Alloc>
  inline constexpr bool uses_allocator_v = uses_allocator<_Tp, _Alloc>::value;

  namespace __detail { // continuing internal implementations

    template<template<class...> class _Predicate,
             class _Tp, class _Alloc, class... _Args>
    struct __is_uses_allocator_predicate
      : public conditional_t<uses_allocator<_Tp, _Alloc>::value,
                             __or_<_Predicate<_Tp, allocator_arg_t, _Alloc, _Args...>,
                                   _Predicate<_Tp, _Args..., _Alloc>>,
                             _Predicate<_Tp, _Args...>> { };
    
    template<class _Tp, class _Alloc, class... _Args>
    struct __is_uses_allocator_constructible
      : public __is_uses_allocator_predicate<is_constructible, _Tp, _Alloc, _Args...> { };

    template<class _Tp, class _Alloc, class... _Args>
    inline constexpr bool __is_uses_allocator_constructible_v
      = __is_uses_allocator_constructible<_Tp, _Alloc, _Args...>::value;

    template<class _Tp, class _Alloc, class... _Args>
    struct __is_nothrow_uses_allocator_constructible
      : public __is_uses_allocator_predicate<is_nothrow_constructible, _Tp, _Alloc, _Args...> { };
    
    template<class _Tp, class _Alloc, class... _Args>
    inline constexpr bool __is_nothrow_uses_allocator_constructible_v
      = __is_nothrow_uses_allocator_constructible<_Tp, _Alloc, _Args...>::value;

    template<class _Tp, class... _Args>
    void __uses_allocator_construct_impl(__uses_alloc0, _Tp* __ptr, _Args&&... __args) {
      ::new ((void*)__ptr) _Tp(std::forward<_Args>(__args)...);
    }

    template<class _Tp, class _Alloc, class... _Args>
    void __uses_allocator_construct_impl(__uses_alloc1<_Alloc> __a, _Tp* __ptr, _Args&&... __args) {
      ::new ((void*)__ptr) _Tp(allocator_arg, *__a._M_a, std::forward<_Args>(__args)...);
    }

    template<class _Tp, class _Alloc, class... _Args>
    void __uses_allocator_construct_impl(__uses_alloc2<_Alloc> __a, _Tp* __ptr, _Args&&... __args) {
      ::new ((void*)__ptr) _Tp(std::forward<_Args>(__args)..., *__a._M_a);
    }

    template<class _Tp, class _Alloc, class... _Args>
    void __uses_allocator_construct(const _Alloc& __a, _Tp* __ptr, _Args&&... __args) {
      __uses_allocator_construct_impl(
        __use_alloc<_Tp, _Alloc, _Args...>(__a),
        __ptr,
        std::forward<_Args>(__args)...
      );
    }

  } // end of __detail:: namespace

# endif // end of uses_allocator c++23 implementation

  // End of uses_allocator.h implementation


  // Start of bits/stl_function.h c++14 implementation:

  // Start of std::unary_function and std::binary_function implementations:
  // unary_function and  binary_function are deprecated in the current libstdc++, so I deprecated them here as well.
  // I'm leaving this deprecated since only certain internal implementations require this, but these shouldn't be used by the user.
  // All internal code that uses these will ignore its deprecated attribute.
  template<class _Arg, class _Result>
  struct [[deprecated]] unary_function {
    typedef _Arg argument_type;
    typedef _Result result_type;
  };

  template<class _Arg1, class _Arg2, class _Result>
  struct [[deprecated]] binary_function {
    typedef _Arg1 first_argument_type;
    typedef _Arg2 second_argument_type;
    typedef _Result result_type;
  };
  // End of std::unary_function and std::binary_function

# if (_TRANSPARENT_OPERATORS_)
  // forward declarations for transparent arithmetic functor specializations
  namespace __detail { // keeping this declaration in the internal namespace
    struct __is_transparent; // undefined
  }

  template<class _Tp = void>
  struct plus;

  template<class _Tp = void>
  struct minus;

  template<class _Tp = void>
  struct multiplies;

  template<class _Tp = void>
  struct divides;

  template<class _Tp = void>
  struct modulus;

  template<class _Tp = void>
  struct negate;
# endif

_USE_DEPRECATED 

  // Start of arithmetic operator functor implementations:
  template<class _Tp>
  struct plus : public binary_function<_Tp, _Tp, _Tp> {
    constexpr _Tp operator()(const _Tp& __x, const _Tp& __y) const { 
      return __x + __y; 
    }
  };

  template<class _Tp>
  struct minus : public binary_function<_Tp, _Tp, _Tp> {
    constexpr _Tp operator()(const _Tp& __x, const _Tp& __y) const { 
      return __x - __y; 
    }
  };

  template<class _Tp>
  struct multiplies : public binary_function<_Tp, _Tp, _Tp> {
    constexpr _Tp operator()(const _Tp& __x, const _Tp& __y) const { 
      return __x * __y; 
    }
  };

  template<class _Tp>
  struct divides : public binary_function<_Tp, _Tp, _Tp> {
    constexpr _Tp operator()(const _Tp& __x, const _Tp& __y) const { 
      return __x / __y;
    }
  };

  template<class _Tp>
  struct modulus : public binary_function<_Tp, _Tp, _Tp> {
    constexpr _Tp operator()(const _Tp& __x, const _Tp& __y) const { 
      return __x % __y; 
    }
  };

  template<class _Tp>
  struct negate : public unary_function<_Tp, _Tp> {
    constexpr _Tp operator()(const _Tp& __x) const { 
      return -__x; 
    }
  };
  // End of arithmetic operator functors

_END_DEPRECATED

# if (_TRANSPARENT_OPERATORS_)
  // Start of transparent arithmetic functor specializations
  template<>
  struct plus<void> {
    template<class _Tp, class _Up>
    constexpr auto operator()(_Tp&& __t, _Up&& __u) const
    noexcept(noexcept(std::forward<_Tp>(__t) + std::forward<_Up>(__u))) 
    -> decltype(std::forward<_Tp>(__t) + std::forward<_Up>(__u)) {
      return std::forward<_Tp>(__t) + std::forward<_Up>(__u);
    }

    typedef __detail::__is_transparent is_transparent;
  };

  template<>
  struct minus<void> {
    template<class _Tp, class _Up>
    constexpr auto operator()(_Tp&& __t, _Up&& __u) const
    noexcept(noexcept(std::forward<_Tp>(__t) - std::forward<_Up>(__u)))
    -> decltype(std::forward<_Tp>(__t) - std::forward<_Up>(__u)) {
      return std::forward<_Tp>(__t) - std::forward<_Up>(__u);
    }

    typedef __detail::__is_transparent is_transparent;
  };

  template<>
  struct multiplies<void> {
    template<class _Tp, class _Up>
    constexpr auto operator()(_Tp&& __t, _Up&& __u) const
    noexcept(noexcept(std::forward<_Tp>(__t) * std::forward<_Up>(__u)))
    -> decltype(std::forward<_Tp>(__t) * std::forward<_Up>(__u)) {
      return std::forward<_Tp>(__t) * std::forward<_Up>(__u);
    }

    typedef __detail::__is_transparent is_transparent;
  };

  template<>
  struct divides<void> {
    template<class _Tp, class _Up>
    constexpr auto operator()(_Tp&& __t, _Up&& __u) const
    noexcept(noexcept(std::forward<_Tp>(__t) / std::forward<_Up>(__u)))
    -> decltype(std::forward<_Tp>(__t) / std::forward<_Up>(__u)) {
      return std::forward<_Tp>(__t) / std::forward<_Up>(__u);
    }

    typedef __detail::__is_transparent is_transparent;
  };

  template<>
  struct modulus<void> {
    template<class _Tp, class _Up>
    constexpr auto operator()(_Tp&& __t, _Up&& __u) const
    noexcept(noexcept(std::forward<_Tp>(__t) % std::forward<_Up>(__u)))
    -> decltype(std::forward<_Tp>(__t) % std::forward<_Up>(__u)) {
      return std::forward<_Tp>(__t) % std::forward<_Up>(__u);
    }

    typedef __detail::__is_transparent is_transparent;
  };

  template<>
  struct negate<void> {
    template<class _Tp>
    constexpr auto operator()(_Tp&& __t) const
    noexcept(noexcept(-std::forward<_Tp>(__t))) -> decltype(-std::forward<_Tp>(__t)) {
      return -std::forward<_Tp>(__t);
    }

    typedef __detail::__is_transparent is_transparent;
  };
  // End of transparent arithmetic functors
# endif

# if (_TRANSPARENT_OPERATORS_)
  // forward declarations for transparent specializations
  template<class _Tp = void>
  struct equal_to;

  template<class _Tp = void>
  struct not_equal_to;

  template<class _Tp = void>
  struct greater;

  template<class _Tp = void>
  struct less;

  template<class _Tp = void>
  struct greater_equal;

  template<class _Tp = void>
  struct less_equal;
# endif

_USE_DEPRECATED

  // Start of comparison operator functors:
  template<class _Tp>
  struct equal_to : public binary_function<_Tp, _Tp, bool> {
    constexpr bool operator()(const _Tp& __x, const _Tp& __y) const {
      return __x == __y;
    }
  };

  template<class _Tp>
  struct not_equal_to : public binary_function<_Tp, _Tp, bool> {
    constexpr bool operator()(const _Tp& __x, const _Tp& __y) const {
      return __x != __y;
    }
  };

  template<class _Tp>
  struct greater : public binary_function<_Tp, _Tp, bool> {
    constexpr bool operator()(const _Tp& __x, const _Tp& __y) const {
      return __x > __y;
    }
  };

  template<class _Tp>
  struct less : public binary_function<_Tp, _Tp, bool> {
    constexpr bool operator()(const _Tp& __x, const _Tp& __y) const {
      return __x < __y;
    }
  };

  template<class _Tp>
  struct greater_equal : public binary_function<_Tp, _Tp, bool> {
    constexpr bool operator()(const _Tp& __x, const _Tp& __y) const {
      return __x >= __y;
    }
  };

  template<class _Tp>
  struct less_equal : public binary_function<_Tp, _Tp, bool> {
    constexpr bool operator()(const _Tp& __x, const _Tp& __y) const {
      return __x <= __y;
    }
  };
  // End of comparison operator functors
  // no specializations for pointers due to the lack of uinptr_t type.
  // There is also the function std::__is_constant_evaluated() which I can't find the source code for anywhere
_END_DEPRECATED 

# if(_TRANSPARENT_OPERATORS_)
  // Start of transparent comparison operator functor specializations:
  template<>
  struct equal_to<void> {
    template<class _Tp, class _Up>
    constexpr auto operator()(_Tp&& __t, _Up&& __u) const
    noexcept(noexcept(std::forward<_Tp>(__t) == std::forward<_Up>(__u)))
    -> decltype(std::forward<_Tp>(__t) == std::forward<_Up>(__u)) {
      return std::forward<_Tp>(__t) == std::forward<_Up>(__u);
    }

    typedef __detail::__is_transparent is_transparent;
  };

  template<>
  struct not_equal_to<void> {
    template<class _Tp, class _Up>
    constexpr auto operator()(_Tp&& __t, _Up&& __u) const
    noexcept(noexcept(std::forward<_Tp>(__t) != std::forward<_Up>(__u)))
    -> decltype(std::forward<_Tp>(__t) != std::forward<_Up>(__u)) {
      return std::forward<_Tp>(__t) != std::forward<_Up>(__u);
    }

    typedef __detail::__is_transparent is_transparent;
  };

  template<>
  struct greater<void> {
    template<class _Tp, class _Up>
    constexpr auto operator()(_Tp&& __t, _Up&& __u) const
    noexcept(noexcept(std::forward<_Tp>(__t) > std::forward<_Up>(__u)))
    -> decltype(std::forward<_Tp>(__t) > std::forward<_Up>(__u)) {
      return std::forward<_Tp>(__t) > std::forward<_Up>(__u);
    }

    typedef __detail::__is_transparent is_transparent;
  };

  template<>
  struct less<void> {
    template<class _Tp, class _Up>
    constexpr auto operator()(_Tp&& __t, _Up&& __u) const
    noexcept(noexcept(std::forward<_Tp>(__t) < std::forward<_Up>(__u)))
    -> decltype(std::forward<_Tp>(__t) < std::forward<_Up>(__u)) {
      return std::forward<_Tp>(__t) < std::forward<_Up>(__u);
    }

    typedef __detail::__is_transparent is_transparent;
  };

  template<>
  struct greater_equal<void> {
    template<class _Tp, class _Up>
    constexpr auto operator()(_Tp&& __t, _Up&& __u) const
    noexcept(noexcept(std::forward<_Tp>(__t) >= std::forward<_Up>(__u)))
    -> decltype(std::forward<_Tp>(__t) >= std::forward<_Up>(__u)) {
      return std::forward<_Tp>(__t) >= std::forward<_Up>(__u);
    }

    typedef __detail::__is_transparent is_transparent;
  };

  template<>
  struct less_equal<void> {
    template<class _Tp, class _Up>
    constexpr auto operator()(_Tp&& __t, _Up&& __u) const
    noexcept(noexcept(std::forward<_Tp>(__t) <= std::forward<_Up>(__u)))
    -> decltype(std::forward<_Tp>(__t) <= std::forward<_Up>(__u)) {
      return std::forward<_Tp>(__t) <= std::forward<_Up>(__u);
    }

    typedef __detail::__is_transparent is_transparent;
  };
  // End of transparent comparison operator functors
# endif

# if(_TRANSPARENT_OPERATORS_)
  // forward declarations for transparent logical operator functors:
  template<class _Tp = void>
  struct logical_and;

  template<class _Tp = void>
  struct logical_or;

  template<class _Tp = void>
  struct logical_not;
# endif

_USE_DEPRECATED

  // Start of logical operator functors:
  template<class _Tp>
  struct logical_and : public binary_function<_Tp, _Tp, bool> {
    constexpr bool operator()(const _Tp& __x, const _Tp& __y) const {
      return __x && __y;
    }
  };

  template<class _Tp>
  struct logical_or : public binary_function<_Tp, _Tp, bool> {
    constexpr bool operator()(const _Tp& __x, const _Tp& __y) const {
      return __x || __y;
    }
  };

  template<class _Tp>
  struct logical_not : public unary_function<_Tp, bool> {
    constexpr bool operator()(const _Tp& __x, const _Tp& __y) const {
      return !__x;
    }
  };
  // End of logical operator functors

_END_DEPRECATED

# if(_TRANSPARENT_OPERATORS_)
  // Start of specializations for transparent logical operator functors:
  template<>
  struct logical_and<void> {
    template<class _Tp, class _Up>
    constexpr auto operator()(_Tp&& __t, _Up&& __u) const
    noexcept(noexcept(std::forward<_Tp>(__t) && std::forward<_Up>(__u)))
    -> decltype(std::forward<_Tp>(__t) && std::forward<_Up>(__u)) {
      return std::forward<_Tp>(__t) && std::forward<_Up>(__u);
    }

    typedef __detail::__is_transparent is_transparent;
  };

  template<>
  struct logical_or<void> {
    template<class _Tp, class _Up>
    constexpr auto operator()(_Tp&& __t, _Up&& __u) const
    noexcept(noexcept(std::forward<_Tp>(__t) || std::forward<_Up>(__u)))
    -> decltype(std::forward<_Tp>(__t) || std::forward<_Up>(__u)) {
      return std::forward<_Tp>(__t) || std::forward<_Up>(__u);
    }

    typedef __detail::__is_transparent is_transparent;
  };

  template<>
  struct logical_not<void> {
    template<class _Tp>
    constexpr auto operator()(_Tp&& __t) const
    noexcept(noexcept(!std::forward<_Tp>(__t))) -> decltype(!std::forward<_Tp>(__t)) {
      return !std::forward<_Tp>(__t);
    }

    typedef __detail::__is_transparent is_transparent;
  };
  // End of transparent logical operator functors
# endif

# if(_TRANSPARENT_OPERATORS_)
  // forward declarations for transparent bitwise operator functors:
  template<class _Tp = void>
  struct bit_and;

  template<class _Tp = void>
  struct bit_or;

  template<class _Tp = void>
  struct bit_xor;

  template<class _Tp = void>
  struct bit_not;
# endif

_USE_DEPRECATED

  // Start of bitwise operator functors:
  template<class _Tp>
  struct bit_and : public binary_function<_Tp, _Tp, _Tp> {
    constexpr _Tp operator()(const _Tp& __x, const _Tp& __y) const {
      return __x & __y;
    }
  };

  template<class _Tp>
  struct bit_or : public binary_function<_Tp, _Tp, _Tp> {
    constexpr _Tp operator()(const _Tp& __x, const _Tp& __y) const {
      return __x | __y;
    }
  };

  template<class _Tp>
  struct bit_xor : public binary_function<_Tp, _Tp, _Tp> {
    constexpr _Tp operator()(const _Tp& __x, const _Tp& __y) const  {
      return __x ^ __y;
    }
  };

  template<class _Tp>
  struct bit_not : public unary_function<_Tp, _Tp> {
    constexpr _Tp operator()(const _Tp& __x) const {
      return ~__x;
    }
  };
  // End of bitwise operator functors

_END_DEPRECATED

# if(_TRANSPARENT_OPERATORS_)
  // Start of transparent bitwise operator functors
  template<>
  struct bit_and<void> {
    template<class _Tp, class _Up>
    constexpr auto operator()(_Tp&& __t, _Up&& __u) const
    noexcept(noexcept(std::forward<_Tp>(__t) & std::forward<_Up>(__u)))
    -> decltype(std::forward<_Tp>(__t) & std::forward<_Up>(__u)) {
      return std::forward<_Tp>(__t) & std::forward<_Up>(__u);
    }

    typedef __detail::__is_transparent is_transparent;
  };

  template<>
  struct bit_or<void> {
    template<class _Tp, class _Up>
    constexpr auto operator()(_Tp&& __t, _Up&& __u) const
    noexcept(noexcept(std::forward<_Tp>(__t) | std::forward<_Up>(__u)))
    -> decltype(std::forward<_Tp>(__t) | std::forward<_Up>(__u)) {
      return std::forward<_Tp>(__t) | std::forward<_Up>(__u);
    }

    typedef __detail::__is_transparent is_transparent;
  };

  template<>
  struct bit_xor<void> {
    template<class _Tp, class _Up>
    constexpr auto operator()(_Tp&& __t, _Up&& __u) const
    noexcept(noexcept(std::forward<_Tp>(__t) ^ std::forward<_Up>(__u)))
    -> decltype(std::forward<_Tp>(__t) ^ std::forward<_Up>(__u)) {
      return std::forward<_Tp>(__t) ^ std::forward<_Up>(__u);
    }

    typedef __detail::__is_transparent is_transparent;
  };

  template<>
  struct bit_not<void> {
    template<class _Tp>
    constexpr auto operator()(_Tp&& __t) const
    noexcept(noexcept(~std::forward<_Tp>(__t))) -> decltype(~std::forward<_Tp>(__t)) {
      return ~std::forward<_Tp>(__t);
    }

    typedef __detail::__is_transparent is_transparent;
  };
  // End of transparent bitwise operator functors
# endif

_USE_DEPRECATED

  // Start of functor negator functors:
  template<class _Predicate>
  class [[deprecated]] unary_negate
    : public unary_function<typename _Predicate::argument_type, bool> {
    protected:
      _Predicate _M_pred;
    
    public:
      constexpr explicit unary_negate(const _Predicate& __x) : _M_pred(__x) { }

      constexpr bool operator()(const typename _Predicate::argument_type& __x) const {
        return !_M_pred(__x);
      }
  };

  template<class _Predicate>
  [[deprecated]]
  constexpr inline unary_negate<_Predicate> not1(const _Predicate& __pred) {
    return unary_negate<_Predicate>(__pred);
  }

  template<class _Predicate>
  class [[deprecated]] binary_negate
    : public binary_function<typename _Predicate::first_argument_type,
                             typename _Predicate::second_argument_type,
                             bool> {
    protected:
      _Predicate _M_pred;

    public:
      constexpr explicit binary_negate(const _Predicate& __x) : _M_pred(__x) { }

      constexpr bool operator()(const typename _Predicate::first_argument_type& __x,
                                const typename _Predicate::second_argument_type& __y) const {
        return !_M_pred(__x, __y);
      }
  };

  template<class _Predicate>
  [[deprecated]]
  constexpr inline binary_negate<_Predicate> not2(const _Predicate& __pred) {
    return binary_negate<_Predicate>(__pred);
  }
  // End of functor negator functors

  
  // Start of functor pointer adaptors:
  template<class _Arg, class _Result>
  class [[deprecated]] pointer_to_unary_function : public unary_function<_Arg, _Result> {
    protected:
      _Result (*_M_ptr)(_Arg);

    public:
      pointer_to_unary_function() { }

      explicit pointer_to_unary_function(_Result (*__x)(_Arg)) : _M_ptr(__x) { }

      _Result operator()(_Arg __x) const { return _M_ptr(__x); }
  };

  template<class _Arg, class _Result>
  [[deprecated]]
  inline pointer_to_unary_function<_Arg, _Result> ptr_fun(_Result (*__x)(_Arg)) {
    return pointer_to_unary_function<_Arg, _Result>(__x);
  }

  template<class _Arg1, class _Arg2, class _Result>
  class [[deprecated]] pointer_to_binary_function : public binary_function<_Arg1, _Arg2, _Result> {
    protected:
      _Result (*_M_ptr)(_Arg1, _Arg2);

    public:
      pointer_to_binary_function() { }

      explicit pointer_to_binary_function(_Result (*__x)(_Arg1, _Arg2)) : _M_ptr(__x) { }

      _Result operator()(_Arg1 __x, _Arg2 __y) const { return _M_ptr(__x, __y); }
  };

  template<class _Arg1, class _Arg2, class _Result>
  [[deprecated]]
  inline pointer_to_binary_function<_Arg1, _Arg2, _Result> ptr_fun(_Result (*__x)(_Arg1, _Arg2)) {
    return pointer_to_binary_function<_Arg1, _Arg2, _Result>(__x);
  }
  // End of functor pointer adaptors

  namespace __detail { // namespace for internal implementations

    template<class _Tp>
    struct _Identity : public unary_function<_Tp, _Tp> {
      _Tp& operator()(_Tp& __x) const { return __x; }

      const _Tp& operator()(const _Tp& __x) const { return __x; }
    };

    template<class _Tp> struct _Identity<const _Tp> : _Identity<_Tp> { };

    template<class _Pair>
    struct _Select1st : public unary_function<_Pair, typename _Pair::first_type> {
      typename _Pair::first_type& operator()(_Pair& __x) const {
        return __x.first;
      }

      const typename _Pair::first_type& operator()(const _Pair& __x) const {
        return __x.first;
      }

      template<class _Pair2>
      typename _Pair2::first_type& operator()(_Pair2& __x) const {
        return __x.first;
      }

      template<class _Pair2>
      const typename _Pair2::first_type& operator()(const _Pair2& __x) const {
        return __x.first;
      }
    };

    template<class _Pair>
    struct _Select2nd : public unary_function<_Pair, typename _Pair::second_type> {
      typename _Pair::second_type& operator()(_Pair& __x) const {
        return __x.second;
      }

      const typename _Pair::second_type& operator()(const _Pair& __x) const {
        return __x.second;
      }
    };

  } // end of __detail:: namespace

  // Start of member functor pointer adaptors:
  template<class _Ret, class _Tp>
  class [[deprecated]] mem_fun_t : public unary_function<_Tp*, _Ret> {
    public:
      explicit mem_fun_t(_Ret (_Tp::*__pf)()) : _M_f(__pf) { }

      _Ret operator()(_Tp* __p) const { return (__p->*_M_f)(); }

    private:
      _Ret (_Tp::*_M_f)();
  };

  template<class _Ret, class _Tp>
  class [[deprecated]] const_mem_fun_t : public unary_function<const _Tp*, _Ret> {
    public:
      explicit const_mem_fun_t(_Ret (_Tp::*__pf)() const) : _M_f(__pf) { }

      _Ret operator()(const _Tp* __p) const { return (__p->*_M_f)(); }

    private:
      _Ret (_Tp::*_M_f)() const;
  };

  template<class _Ret, class _Tp>
  class [[deprecated]] mem_fun_ref_t : public unary_function<_Tp, _Ret> {
    public:
      explicit mem_fun_ref_t(_Ret (_Tp::*__pf)()) : _M_f(__pf) { }

      _Ret operator()(_Tp& __r) const { return (__r.*_M_f)(); }

    private:
      _Ret (_Tp::*_M_f)();
  };

  template<class _Ret, class _Tp>
  class [[deprecated]] const_mem_fun_ref_t : public unary_function<_Tp, _Ret> {
    public:
      explicit const_mem_fun_ref_t(_Ret (_Tp::*__pf)() const) : _M_f(__pf) { }

      _Ret operator()(const _Tp& __r) const { return (__r.*_M_f)(); }

    private:
      _Ret (_Tp::*_M_f)() const;
  };

  template<class _Ret, class _Tp, class _Arg>
  class [[deprecated]] mem_fun1_t : public binary_function<_Tp*, _Arg, _Ret> {
    public:
      explicit mem_fun1_t(_Ret (_Tp::*__pf)(_Arg)) : _M_f(__pf) { }

      _Ret operator()(_Tp* __p, _Arg __x) const { return (__p->*_M_f)(__x); }
    
    private:
      _Ret (_Tp::*_M_f)(_Arg);
  };

  template<class _Ret, class _Tp, class _Arg>
  class [[deprecated]] const_mem_fun1_t : public binary_function<const _Tp*, _Arg, _Ret> {
    public:
      explicit const_mem_fun1_t(_Ret (_Tp::*__pf)(_Arg) const) : _M_f(__pf) { }

      _Ret operator()(const _Tp* __p, _Arg __x) const { return (__p->*_M_f)(__x); }

    private:
      _Ret (_Tp::*_M_f)(_Arg) const;
  };

  template<class _Ret, class _Tp, class _Arg>
  class [[deprecated]] mem_fun1_ref_t : public binary_function<_Tp, _Arg, _Ret> {
    public:
      explicit mem_fun1_ref_t(_Ret (_Tp::*__pf)(_Arg)) : _M_f(__pf) { }

      _Ret operator()(_Tp& __r, _Arg __x) const { return (__r.*_M_f)(__x); }
    
    private:
      _Ret (_Tp::*_M_f)(_Arg);
  };

  template<class _Ret, class _Tp, class _Arg>
  class [[deprecated]] const_mem_fun1_ref_t : public binary_function<_Tp, _Arg, _Ret> {
    public:
      explicit const_mem_fun1_ref_t(_Ret (_Tp::*__pf)(_Arg) const) : _M_f(__pf) { }

      _Ret operator()(const _Tp& __r, _Arg __x) const { return (__r.*_M_f)(__x); }
    
    private:
      _Ret (_Tp::*_M_f)(_Arg) const;
  };
  // End of member functor pointer adaptors

  
  // Start of mem_fun adaptor helper functions:
  template<class _Ret, class _Tp>
  [[deprecated]]
  inline mem_fun_t<_Ret, _Tp> mem_fun(_Ret (_Tp::*__f)()) {
    return mem_fun_t<_Ret, _Tp>(__f);
  }

  template<class _Ret, class _Tp>
  [[deprecated]]
  inline const_mem_fun_t<_Ret, _Tp> mem_fun(_Ret (_Tp::*__f)() const) {
    return const_mem_fun_t<_Ret, _Tp>(__f);
  }

  template<class _Ret, class _Tp>
  [[deprecated]]
  inline mem_fun_ref_t<_Ret, _Tp> mem_fun_ref(_Ret (_Tp::*__f)()) {
    return mem_fun_ref_t<_Ret, _Tp>(__f);
  }

  template<class _Ret, class _Tp>
  [[deprecated]]
  inline const_mem_fun_ref_t<_Ret, _Tp> mem_fun_ref(_Ret (_Tp::*__f)() const) {
    return const_mem_fun_ref_t<_Ret, _Tp>(__f);
  }

  template<class _Ret, class _Tp, class _Arg>
  [[deprecated]]
  inline mem_fun1_t<_Ret, _Tp, _Arg> mem_fun(_Ret (_Tp::*__f)(_Arg)) {
    return mem_fun1_t<_Ret, _Tp, _Arg>(__f);
  }

  template<class _Ret, class _Tp, class _Arg>
  [[deprecated]]
  inline const_mem_fun1_t<_Ret, _Tp, _Arg> mem_fun(_Ret (_Tp::*__f)(_Arg) const) {
    return const_mem_fun1_t<_Ret, _Tp, _Arg>(__f);
  }

  template<class _Ret, class _Tp, class _Arg>
  [[deprecated]]
  inline mem_fun1_ref_t<_Ret, _Tp, _Arg> mem_fun_ref(_Ret (_Tp::*__f)(_Arg)) {
    return mem_fun1_ref_t<_Ret, _Tp, _Arg>(__f);
  }

  template<class _Ret, class _Tp, class _Arg>
  [[deprecated]]
  inline const_mem_fun1_ref_t<_Ret, _Tp, _Arg> mem_fun_ref(_Ret (_Tp::*__f)(_Arg) const) {
    return const_mem_fun1_ref_t<_Ret, _Tp, _Arg>(__f);
  }
  // End of mem_fun adaptor helper functions

_END_DEPRECATED

# if(_TRANSPARENT_OPERATORS_)
  // helper structs to detect if a functor being used is a transparent operator:
  namespace __detail { // namespace for internal implementations

    template<class _Func, class _SfinaeType, class = void_t<>>
    struct __has_is_transparent { };

    template<class _Func, class _SfinaeType>
    struct __has_is_transparent<_Func, _SfinaeType, void_t<typename _Func::is_transparent>> {
      typedef void type;
    };

    template<class _Func, class _SfinaeType>
    using __has_is_transparent_t = typename __has_is_transparent<_Func, _SfinaeType>::type;

  } // end of __detail:: namespace
# endif

  // I will possibly add backward/binders.h since it's conditionally included at the end of bits/stl_function.h

  // End of bits/stl_function implementation

} // end of std:: namespace


// NOTE: implementations below are outside of the std namespace.(global or in __gnu_cxx for internal laguage extension implementations)


// Start of bits/predefined_ops.h STL file implementation:
// I may need to move this to it's own file, or I might put it in Concept_Checks.hpp with the rest of the c++14 style concept checks.
// This is implemented purely for compatibility with c++14 code since I'm trying to use most current implementations I can for everything.
namespace __gnu_cxx { // internal namespace for GNU extensions

  namespace __ops { // namespace dedicated for these implementations

    struct _Iter_less_iter {
      template<class _Iterator1, class _Iterator2>
      bool operator()(_Iterator1 __it1, _Iterator2 __it2) const {
        return *__it1 < *__it2;
      }
    };

    inline _Iter_less_iter __iter_less_iter() {
      return _Iter_less_iter();
    }

    struct _Iter_less_val {
      template<class _Iterator, class _Value>
      bool operator()(_Iterator __it, _Value& __val) const {
        return *__it < __val;
      }
    };

    inline _Iter_less_val __iter_less_val() { return _Iter_less_val(); }

    inline _Iter_less_val __iter_comp_val(_Iter_less_iter) { return _Iter_less_val(); }

    struct _Val_less_iter {
      template<class _Value, class _Iterator>
      bool operator()(_Value& __val, _Iterator __it) const {
        return __val < *__it;
      }
    };

    inline _Val_less_iter __val_less_iter() { return _Val_less_iter(); }

    inline _Val_less_iter __val_comp_iter(_Iter_less_iter) { return _Val_less_iter(); }

    struct _Iter_equal_to_iter {
      template<class _Iterator1, class _Iterator2>
      bool operator()(_Iterator1 __it1, _Iterator2 __it2) const {
        return *__it1 == *__it2;
      }
    };

    inline _Iter_equal_to_iter __iter_equal_to_iter() { return _Iter_equal_to_iter(); }

    struct _Iter_equal_to_val {
      template<class _Iterator, class _Value>
      bool operator()(_Iterator __it, _Value& __val) const {
        return *__it == __val;
      }
    };

    inline _Iter_equal_to_val __iter_equal_to_val() { return _Iter_equal_to_val(); }

    inline _Iter_equal_to_val __iter_comp_val(_Iter_equal_to_iter) { return _Iter_equal_to_val(); }

    template<class _Compare>
    struct _Iter_comp_iter {
      _Compare _M_comp;

      _Iter_comp_iter(_Compare __comp) : _M_comp(__comp) { }

      template<class _Iterator1, class _Iterator2>
      bool operator()(_Iterator1 __it1, _Iterator2 __it2) {
        return bool(_M_comp(*__it1, *__it2));
      }
    };

    template<class _Compare>
    inline _Iter_comp_iter<_Compare> __iter_comp_iter(_Compare __comp) {
      return _Iter_comp_iter<_Compare>(__comp);
    }

    template<class _Compare>
    struct _Iter_comp_val {
      _Compare _M_comp;

      _Iter_comp_val(_Compare __comp) : _M_comp(__comp) { }

      template<class _Iterator, class _Value>
      bool operator()(_Iterator __it, _Value& __val) {
        return bool(_M_comp(*__it, __val));
      }
    };

    template<class _Compare>
    inline _Iter_comp_val<_Compare> __iter_comp_val(_Compare __comp) {
      return _Iter_comp_val<_Compare>(__comp);
    }

    template<class _Compare>
    inline _Iter_comp_val<_Compare> __iter_comp_val(_Iter_comp_iter<_Compare> __comp) {
      return _Iter_comp_val<_Compare>(__comp._M_comp);
    }

    template<class _Compare>
    struct _Val_comp_iter {
      _Compare _M_comp;

      _Val_comp_iter(_Compare __comp) : _M_comp(__comp) { }

      template<class _Value, class _Iterator>
      bool operator()(_Value& __val, _Iterator __it) {
        return bool(_M_comp(__val, *__it));
      }
    };

    template<class _Compare>
    inline _Val_comp_iter<_Compare> __val_comp_iter(_Compare __comp) {
      return _Val_comp_iter<_Compare>(__comp);
    }

    template<class _Compare>
    inline _Val_comp_iter<_Compare> __val_comp_iter(_Iter_comp_iter<_Compare> __comp) {
      return _Val_comp_iter<_Compare>(__comp._M_comp);
    }

    template<class _Value>
    struct _Iter_equals_val {
      _Value& _M_value;

      _Iter_equals_val(_Value& __value) : _M_value(__value) { }

      template<class _Iterator>
      bool operator()(_Iterator __it) {
        return *__it == _M_value;
      }
    };

    template<class _Value>
    inline _Iter_equals_val<_Value> __iter_equals_val(_Value& __val) {
      return _Iter_equals_val<_Value>(__val);
    }

    template<class _Iterator1>
    struct _Iter_equals_iter {
      typename std::iterator_traits<_Iterator1>::reference _M_ref;

      _Iter_equals_iter(_Iterator1 __it1) : _M_ref(*__it1) { }

      template<class _Iterator2>
      bool operator()(_Iterator2 __it2) {
        return *__it2 == _M_ref;
      }
    };

    template<class _Iterator>
    inline _Iter_equals_iter<_Iterator> __iter_comp_iter(_Iter_equal_to_iter, _Iterator __it) {
      return _Iter_equals_iter<_Iterator>(__it);
    }

    template<class _Predicate>
    struct _Iter_pred {
      _Predicate _M_pred;

      _Iter_pred(_Predicate __pred) : _M_pred(__pred) { }

      template<class _Iterator>
      bool operator()(_Iterator __it) {
        return bool(_M_pred(*__it));
      }
    };

    template<class _Predicate>
    inline _Iter_pred<_Predicate> __pred_iter(_Predicate __pred) {
      return _Iter_pred<_Predicate>(__pred);
    }

    template<class _Compare, class _Value>
    struct _Iter_comp_to_val {
      _Compare _M_comp;
      _Value& _M_value;

      _Iter_comp_to_val(_Compare __comp, _Value& __value) 
        : _M_comp(__comp), _M_value(__value) { }

      template<class _Iterator>
      bool operator()(_Iterator __it) {
        return bool(_M_comp(*__it, _M_value));
      }
    };

    template<class _Compare, class _Value>
    _Iter_comp_to_val<_Compare, _Value> __iter_comp_to_val(_Compare __comp, _Value& __val) {
      return _Iter_comp_to_val<_Compare, _Value>(__comp, __val);
    }

    template<class _Compare, class _Iterator1>
    struct _Iter_comp_to_iter {
      _Compare _M_comp;
      typename std::iterator_traits<_Iterator1>::reference _M_ref;

      _Iter_comp_to_iter(_Compare __comp, _Iterator1 __it1) 
        : _M_comp(__comp), _M_ref(*__it1) { }
      
      template<class _Iterator2>
      bool operator()(_Iterator2 __it2) {
        return bool(_M_comp(*__it2, _M_ref));
      }
    };

    template<class _Compare, class _Iterator>
    inline _Iter_comp_to_iter<_Compare, _Iterator> 
    __iter_comp_iter(_Iter_comp_iter<_Compare> __comp, _Iterator __it) {
      return _Iter_comp_to_iter<_Compare, _Iterator>(__comp._M_comp, __it);
    }

    template<class _Predicate>
    struct _Iter_negate {
      _Predicate _M_pred;

      _Iter_negate(_Predicate __pred) : _M_pred(__pred) { }

      template<class _Iterator>
      bool operator()(_Iterator __it) {
        return !bool(_M_pred(*__it));
      }
    };

    template<class _Predicate>
    inline _Iter_negate<_Predicate> __negate(_Iter_pred<_Predicate> __pred) {
      return _Iter_negate<_Predicate>(__pred._M_pred);
    }

  } // end of __ops:: namespace

} // end of __gnu_cxx:: namespace
// End of bits/predefined_ops.h 


#endif // End of UtilityBase.hpp
