// Libstdc++ and GCC GNU Lincese boilerplate:      (this is required since this code is copyrighted under the GCC compiler. I did not design this myself.)
/*
  Partial Modified Implementation Of Meta-Functions from the <type_traits|bits/cpp_type_traits|typeinfo> Libstdc++ header  -*- C++ -*-

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
  TypeTraits.hpp [V1.0.0] (Type Trait functions and utilities)
    By: Andrey Stroitelev

  ~This header holds a few remade functions from the standard c++ type_traits library.
  ~These functions are simple helpers when working with template types and template functions specializations
  ~The implementation of this file will be very limited as some things I just wont be able to implement, and others I see no use in implementing in regards to this data structure library
  ~Most of these "functions" are actually metafunctions which are structs that use template behavior to produce a dynamic result at compile time.
    *Since the metafunctions are struct objects, when they are used you must explicitly access the value or type in the struct with the scope specifier operator(::type or ::value).
    *Most of the  metafunctions have a helper alias or variable that are named the same with an added _t or _v at the end depending on if the metafunction produces a type or value.
    *Since the types and values are defined by the specialization, every metafunction instance can have a different type or value.
  ~This file acts as the foundation for the rest of this library and is required for pretty much everything.
  ~These functions aren't all up to date as some compiler intriniscs arent present on this version of c++ that the arduino IDE uses.
*/


// Start of Type_Traits.hpp
#ifndef _TYPE_TRAITS_HPP_
#define _TYPE_TRAITS_HPP_

#ifndef _TYPE_TRAITS_VERSION_
#define _TYPE_TRAITS_VERSION_ 0x010000 // V1.0.0
#endif


#include <Arduino.h>
#include "Config.h"
#include "Stdexcept.hpp" // for typeinfo implementation


_DLIB_HEADER


// Start of implementations for Type_Traits in the std namespace:
namespace std _DLIB_VISIBILITY {

  // Start of forward declarations: (I will try to get rid of all of these)
  template<class> struct remove_cv;

  template<class> struct is_array_unknown_bounds;

  template<class _Tp> class reference_wrapper;
  // End of forward declarations


  // Start of std::integral_constant implementation:
  template<class _Tp, _Tp __val>
  struct integral_constant {
    static constexpr _Tp value = __val;

    using value_type = _Tp;
    using type = integral_constant<_Tp, __val>;

    constexpr operator value_type() const noexcept { return value; }
    constexpr value_type operator()() const noexcept { return value; }
  };

  template<bool __b>
  using bool_constant = integral_constant<bool, __b>;

  using true_type = bool_constant<true>;
  using false_type = bool_constant<false>;
  // End of std::integral_constant implementation


  // Start of void type utility
  template<class...>
  using void_t = void;

  template<class _Tp>
  struct is_void : public false_type { };

  template<>
  struct is_void<void> : public true_type { };

  template<>
  struct is_void<const void> : public true_type { };

  template<>
  struct is_void<volatile void> : public true_type { };

  template<>
  struct is_void<const volatile void> : public true_type { };
  // End of void type utility


  // Start of std::__type_identity implementation:
  template<class _Tp>
  struct __type_identity { using type = _Tp; };

  template<class _Tp>
  using __type_identity_t = typename __type_identity<_Tp>::type;
  // End of std::__type_identity implementation


  // Start of std::enable_if implementation:
  template<bool, class _Tp = void>
  struct enable_if { };

  template<class _Tp>
  struct enable_if<true, _Tp> { using type = _Tp; };

  template<bool __cond, class _Tp = void>
  using enable_if_t = typename enable_if<__cond, _Tp>::type;
  // End of std::enable_if implementation


  // Start of std::conditional implementation:
  template<bool __b, class _Tp, class _Up>
  struct conditional { using type = _Tp; };

  template<class _Tp, class _Up>
  struct conditional<false, _Tp, _Up> { using type = _Up; };

  template<bool __b, class _Tp, class _Up>
  using conditional_t = typename conditional<__b, _Tp, _Up>::type;
  // End of std::conditional implementation


  // Start of logic operators for std::conditional implementations:
  namespace __detail { // private helper for the actual implementations

    template<class _Tp, class...>
    using __first_t = _Tp;

    template<class... _Bcn>
    auto __or_fn(int) -> __first_t<false_type, enable_if_t<!bool(_Bcn::value)>...>;

    template<class... _Bcn>
    auto __or_fn(...) -> true_type;

    template<class... _Bcn>
    auto __and_fn(int) -> __first_t<true_type, enable_if_t<bool(_Bcn::value)>...>;

    template<class... _Bcn>
    auto __and_fn(...) -> false_type;

  } // end of std::__detail::

  template<class... _Bcn>
  struct __or_ : decltype(__detail::__or_fn<_Bcn...>(0)) { };

  template<class... _Bcn>
  struct __and_ : decltype(__detail::__and_fn<_Bcn...>(0)) { };

  template<class _Bc>
  struct __not_ : bool_constant<!bool(_Bc::value)> { };

  /* // keeping old implementation commented out just in case I want to switch back to it
    template<class...>
    struct __or_;

    template<>
    struct __or_<> : public false_type { };

    template<class _Bc1>
    struct __or_<_Bc1> : public _Bc1 { };

    template<class _Bc1, class _Bc2>
    struct __or_<_Bc1, _Bc2> : public conditional<_Bc1::value, _Bc1, _Bc2>::type { };

    template<class _Bc1, class _Bc2, class _Bc3, class... _Bcn>
    struct __or_<_Bc1, _Bc2, _Bc3, _Bcn...> : public conditional<_Bc1::value, _Bc1, __or_<_Bc2, _Bc3, _Bcn...>>::type { };

    template<class...>
    struct __and_ { };

    template<>
    struct __and_<> : public true_type { };

    template<class _Bc1>
    struct __and_<_Bc1> : public _Bc1 { };

    template<class _Bc1, class _Bc2>
    struct __and_<_Bc1, _Bc2> : public conditional<_Bc1::value, _Bc2, _Bc1>::type { };

    template<class _Bc1, class _Bc2, class _Bc3, class... _Bcn> 
    struct __and_<_Bc1, _Bc2, _Bc3, _Bcn...> : public conditional<_Bc1::value, __and_<_Bc2, _Bc3, _Bcn...>, _Bc1>::type { };

    template<class _Bc>
    struct __not_ : public bool_constant<!_Bc::value> { };
  */
  // End of logic operators for std::conditional


  // Start of std::_Require which uses std::enable_if to act like the requires keyword with a varitable amount of arguments it can check for by using __and_
  // I might rename this function or create another for user use since currently this is meant for internal use in the library
  template<class... _Cond>
  using _Require = enable_if_t<__and_<_Cond...>::value>;
  // End of std::_Require


  // Start of std::__sfinae_types implementation: (for internal use)
  struct __sfinae_types {
    typedef char __one;
    typedef struct { char __arr[2]; } __two;
  };
  // End of std::__sfinae_types


  // Start of std::is_same implementation:
  template<class _Tp, class _Up>
  struct is_same : public false_type { };

  template<class _Tp>
  struct is_same<_Tp, _Tp> : public true_type { };

  template<class _Tp, class _Up>
  inline constexpr bool is_same_v = is_same<_Tp, _Up>::value;
  // End of std::is_same


  // Start of std::is_function implementation:
  template<class>
  struct is_function : public false_type { };

  template<class _Ret, class... _Args>
  struct is_function<_Ret(_Args...)> : public true_type { };

  template<class _Ret, class... _Args>
  struct is_function<_Ret(_Args......)> : public true_type { };

  template<class _Ret, class... _Args>
  struct is_function<_Ret(_Args...) const> : public true_type { };

  template<class _Ret, class... _Args>
  struct is_function<_Ret(_Args...) volatile> : public true_type { };

  template<class _Ret, class... _Args>
  struct is_function<_Ret(_Args...) const volatile> : public true_type { };

  template<class _Ret, class... _Args>
  struct is_function<_Ret(_Args......) const> : public true_type { };

  template<class _Ret, class... _Args>
  struct is_function<_Ret(_Args......) volatile> : public true_type { };

  template<class _Ret, class... _Args>
  struct is_function<_Ret(_Args......) const volatile> : public true_type { };

  template<class _Ret, class... _Args>
  struct is_function<_Ret(_Args...) &> : public true_type { };

  template<class _Ret, class... _Args>
  struct is_function<_Ret(_Args...) const &> : public true_type { };

  template<class _Ret, class... _Args>
  struct is_function<_Ret(_Args...) volatile &> : public true_type { };

  template<class _Ret, class... _Args>
  struct is_function<_Ret(_Args...) const volatile &> : public true_type { };

  template<class _Ret, class... _Args>
  struct is_function<_Ret(_Args......) &> : public true_type { };

  template<class _Ret, class... _Args>
  struct is_function<_Ret(_Args......) const &> : public true_type { };

  template<class _Ret, class... _Args>
  struct is_function<_Ret(_Args......) volatile &> : public true_type { };

  template<class _Ret, class... _Args>
  struct is_function<_Ret(_Args......) const volatile &> : public true_type { };

  template<class _Ret, class... _Args>
  struct is_function<_Ret(_Args...) &&> : public true_type { };

  template<class _Ret, class... _Args>
  struct is_function<_Ret(_Args...) const &&> : public true_type { };

  template<class _Ret, class... _Args>
  struct is_function<_Ret(_Args...) volatile &&> : public true_type { };

  template<class _Ret, class... _Args>
  struct is_function<_Ret(_Args...) const volatile &&> : public true_type { };

  template<class _Ret, class... _Args>
  struct is_function<_Ret(_Args......) &&> : public true_type { };

  template<class _Ret, class... _Args>
  struct is_function<_Ret(_Args......) const &&> : public true_type { };

  template<class _Ret, class... _Args>
  struct is_function<_Ret(_Args......) volatile &&> : public true_type { };

  template<class _Ret, class... _Args>
  struct is_function<_Ret(_Args......) const volatile &&> : public true_type { };
  // End of std::is_function


  // Start of type property query function implementations:
  template<class _Tp>
  struct alignment_of
    : public integral_constant<size_t, __alignof__(_Tp)> { };
  
  template<class>
  struct rank : public integral_constant<size_t, 0> { };

  template<class _Tp, size_t _Size>
  struct rank<_Tp[_Size]>
    : public integral_constant<size_t, 1 + rank<_Tp>::value> { };
  
  template<class _Tp>
  struct rank<_Tp[]>
    : public integral_constant<size_t, 1 + rank<_Tp>::value> { };

  template<class, unsigned = 0>
  struct extent;

  template<class, unsigned _Uint>
  struct extent : public integral_constant<size_t, 0> { };

  template<class _Tp, unsigned _Uint, size_t _Size>
  struct extent<_Tp[_Size], _Uint> 
    : public integral_constant<size_t, _Uint == 0 ? _Size : extent<_Tp, _Uint - 1>::value> { };

  template<class _Tp, unsigned _Uint>
  struct extent<_Tp[], _Uint> 
    : public integral_constant<size_t, _Uint == 0 ? 0 : extent<_Tp, _Uint - 1>::value> { };

  template<class _Tp>
  struct remove_extent { typedef _Tp type; };

  template<class _Tp, size_t _Size>
  struct remove_extent<_Tp[_Size]> { typedef _Tp type; };

  template<class _Tp>
  struct remove_extent<_Tp[]> { typedef _Tp type; };

  template<class _Tp>
  struct remove_all_extents { typedef _Tp type; };

  template<class _Tp, size_t _Size>
  struct remove_all_extents<_Tp[_Size]> {
    typedef typename remove_all_extents<_Tp>::type type;
  };

  template<class _Tp>
  struct remove_all_extents<_Tp[]> {
    typedef typename remove_all_extents<_Tp>::type type;
  };

  template<class _Tp>
  using remove_extent_t = typename remove_extent<_Tp>::type;

  template<class _Tp>
  using remove_all_extents_t = typename remove_all_extents<_Tp>::type;
  // End of type property query functions


  // Start of std::is_reference implementation:
  template<class _Tp>
  struct is_reference : public false_type { };

  template<class _Tp>
  struct is_reference<_Tp&> : public true_type { };

  template<class _Tp>
  struct is_reference<_Tp&&> : public true_type { };

  template<class _Tp>
  inline constexpr bool is_reference_v = is_reference<_Tp>::value;
  // End of std::is_reference


  // Start of std::_NestedType implementation:
  template<class _Tp, size_t = sizeof(_Tp)>
  constexpr true_type __is_complete_or_unbounded(__type_identity<_Tp>) { return {}; }
  
  template<class _TypeIdentity, class _NestedType = typename _TypeIdentity::type>
  constexpr typename __or_<
                            is_reference<_NestedType>,
                            is_function<_NestedType>,
                            is_void<_NestedType>,
                            is_array_unknown_bounds<_NestedType>
                          >::type __is_complete_or_unbounded(_TypeIdentity) { return {}; }
  // End of std::_NestedType


  // Start of std::is_array implementation:
  template<class _Tp>
  struct is_array : public false_type { };

  template<class _Tp>
  struct is_array<_Tp[]> : public true_type { };

  template<class _Tp, size_t _Nm>
  struct is_array<_Tp[_Nm]> : public true_type { };

  template<class _Tp>
  inline constexpr bool is_array_v = is_array<_Tp>::value;

  template<class _Tp>
  struct is_array_known_bounds : public bool_constant<(extent<_Tp>::value > 0)> { };

  template<class _Tp>
  struct is_array_unknown_bounds : public __and_<is_array<_Tp>, __not_<extent<_Tp>>>::type { };
  // End of std::is_array


  // Start of qualifier removal function implementations:
  template<class _Tp>
  struct remove_cv { typedef _Tp type; };

  template<class _Tp>
  struct remove_cv<const _Tp> { typedef _Tp type; };

  template<class _Tp>
  struct remove_cv<volatile _Tp> { typedef _Tp type; };

  template<class _Tp>
  struct remove_cv<const volatile _Tp> { typedef _Tp type; };

  template<class _Tp>
  struct remove_const { typedef _Tp type; };

  template<class _Tp>
  struct remove_const<const _Tp> { typedef _Tp type; };

  template<class _Tp>
  struct remove_volatile { typedef _Tp type; };

  template<class _Tp>
  struct remove_volatile<volatile _Tp> { typedef _Tp type; };

  template<class _Tp>
  using remove_cv_t = typename remove_cv<_Tp>::type;

  template<class _Tp>
  using remove_const_t = typename remove_const<_Tp>::type;

  template<class _Tp>
  using remove_volatile_t = typename remove_volatile<_Tp>::type;
  // End of qualifier removal functions


  // Start of qualifer adding function implementations:
  template<class _Tp>
  struct add_cv { typedef const volatile _Tp type; };

  template<class _Tp>
  struct add_const { typedef const _Tp type; };

  template<class _Tp>
  struct add_volatile { typedef volatile _Tp type; };

  template<class _Tp>
  using add_cv_t = typename add_cv<_Tp>::type;

  template<class _Tp>
  using add_const_t = typename add_const<_Tp>::type;

  template<class _Tp>
  using add_volatile_t = typename add_volatile<_Tp>::type;
  // End of qualifier adding functions


  // Start of qualifier checker function implementations:
  template<class _Tp>
  struct is_const : public false_type { };

  template<class _Tp>
  struct is_const<const _Tp> : public true_type { };

  template<class _Tp>
  inline constexpr bool is_const_v = is_const<_Tp>::value;

  template<class _Tp>
  struct is_volatile : public false_type { };

  template<class _Tp>
  struct is_volatile<volatile _Tp> : public true_type { };

  template<class _Tp>
  inline constexpr bool is_volatile_v = is_volatile<_Tp>::value;

  template<class _Tp>
  struct is_cv : public false_type { };

  template<class _Tp>
  struct is_cv<const _Tp> : public true_type { };

  template<class _Tp>
  struct is_cv<volatile _Tp> : public true_type { };

  template<class _Tp>
  struct is_cv<const volatile _Tp> : public true_type { };

  template<class _Tp>
  inline constexpr bool is_cv_v = is_cv<_Tp>::value;
  // End of qualifier checker functions


  // Start of std::as_const implementation: 
  template<class _Tp>
  [[nodiscard]]
  constexpr add_const_t<_Tp>& as_const(_Tp& __t) noexcept { return __t; }

  template<class _Tp>
  void as_const(const _Tp&&) = delete;
  // End of std::ad_const


  // Start of std::is_enum implementation:
  template<class _Tp>
  struct is_enum : public bool_constant<(__is_enum(_Tp))> { }; // uses compiler intrinsic

  template<class _Tp>
  inline constexpr bool is_enum_v = is_enum<_Tp>::value;
  // End of std::is_enum

  
  // Start of std::is_union implementation:
  template<class _Tp>
  struct is_union : public bool_constant<(__is_union(_Tp))> { }; // uses compiler intrinsic

  template<class _Tp>
  inline constexpr bool is_union_v = is_union<_Tp>::value;
  // End of std::is_union


  // Start of std::is_class implementation:
  namespace __detail { // private namespace for internal implementation

    template<class _Tp>
    bool_constant<!is_union<_Tp>::value> try_is_class(int _Tp::*);

    template<class>
    false_type try_is_class(...);

  } // end of __detail::

  template<class _Tp>
  struct is_class : decltype(__detail::try_is_class<_Tp>(nullptr)) { };

  template<class _Tp>
  inline constexpr bool is_class_v = is_class<_Tp>::value;
  // End of std::is_class


  // Start of std::is_base_of implementation:
  namespace __detail { // private namespace for internal implementation

    template<class _Bc>
    true_type test_ptr_conv(const volatile _Bc*);

    template<class>
    false_type test_ptr_conv(const volatile void*);

    template<class _Bc, class _Dc>
    auto test_is_base_of(int) -> decltype(test_ptr_conv<_Bc>(static_cast<_Dc*>(nullptr)));

    template<class, class>
    auto test_is_base_of(...) -> true_type;

  } // end of __detail::

  template<class _Base, class _Derived>
  struct is_base_of : public bool_constant<is_class<_Base>::value &&
                                           is_class<_Derived>::value &&
                                           decltype(__detail::test_is_base_of<_Base, _Derived>(0))::value> { };

  template<class _Base, class _Derived>
  inline constexpr bool is_base_of_v = is_base_of<_Base, _Derived>::value;
  // End of std::is_base_of


  // Start of std::match_cv_qualifiers implementation:
  namespace __detail { // private namespace for internal implementation

    //template<class _Unqualified, bool __const, bool __vol>
    //struct __cv_selector;

    template<class _Unqualified, bool __const, bool __vol>
    struct __cv_selector;

    template<class _Unqualified>
    struct __cv_selector<_Unqualified, false, false> {
      //typedef _Unqualified __type;
      using __type = _Unqualified;
    };

    template<class _Unqualified>
    struct __cv_selector<_Unqualified, false, true> {
      //typedef volatile _Unqualified __type;
      using __type = volatile _Unqualified;
    };

    template<class _Unqualified>
    struct __cv_selector<_Unqualified, true, false> {
      //typedef const _Unqualified __type;
      using __type = const _Unqualified;
    };

    template<class _Unqualified>
    struct __cv_selector<_Unqualified, true, true> {
      //typedef const volatile _Unqualified __type;
      using __type = const volatile _Unqualified;
    };

  } // end of __detail::

  template<class _Qualified, class _Unqualified>/*, (bool __const = is_const_v<_Qualified>), bool __vol = is_volatile_v<_Qualified>*/
  class match_cv_qualifiers {
    using __match = __detail::__cv_selector<_Unqualified, is_const<_Qualified>::value, is_volatile<_Qualified>::value>;
    public:
      using type = typename __match::__type;
  };

  template<class _Qualified, class _Unqualified> /*, bool __const = is_const<_Qualified>::value, bool __vol = is_volatile<_Qualified>::value*/
  using match_cv_qualifiers_t = typename match_cv_qualifiers<_Qualified, _Unqualified>::type;
  // End of std::match_cv_qualifiers


  // Start of std::remove_reference implementation:
  template<class _Tp>
  struct remove_reference { typedef _Tp type; };

  template<class _Tp>
  struct remove_reference<_Tp&> { typedef _Tp type; };

  template<class _Tp>
  struct remove_reference<_Tp&&> { typedef _Tp type; };

  template<class _Tp>
  using remove_reference_t = typename remove_reference<_Tp>::type;
  // End of std::remove_reference


  // Start of std::add_lvalue_reference and std::add_rvalue_reference implementations:
  /*namespace __detail { // implementation namespace

    template<class _Tp>
    auto try_add_lvalue_reference(int) -> __type_identity<_Tp&>;
    template<class _Tp>
    auto try_add_lvalue_reference(...) -> __type_identity<_Tp>;

    template<class _Tp>
    auto try_add_rvalue_reference(int) -> __type_identity<_Tp&&>;
    template<class _Tp>
    auto try_add_rvalue_reference(...) -> __type_identity<_Tp>;

  } // end of __detail::

  template<class _Tp>
  struct add_lvalue_reference : decltype(__detail::try_add_lvalue_reference<_Tp>(0)) { };

  template<class _Tp>
  struct add_rvalue_reference : decltype(__detail::try_add_rvalue_reference<_Tp>(0)) { };

  template<class _Tp>
  using add_lvalue_reference_t = typename add_lvalue_reference<_Tp>::type;

  template<class _Tp>
  using add_rvalue_reference_t = typename add_rvalue_reference<_Tp>::type;*/
  // Shortened implementation:
  namespace __detail { // start of private implementation namespace

    template<class _Tp, class = void>
    struct __add_rvalue_reference_helper { using type = _Tp; };

    template<class _Tp>
    struct __add_rvalue_reference_helper<_Tp, void_t<_Tp&&>> { using type = _Tp&&; };

    template<class _Tp, class = void>
    struct __add_lvalue_reference_helper { using type = _Tp; };

    template<class _Tp>
    struct __add_lvalue_reference_helper<_Tp, void_t<_Tp&>> { using type = _Tp&; };

    template<class _Tp>
    using __add_rval_ref_t = typename __add_rvalue_reference_helper<_Tp>::type;

    template<class _Tp>
    using __add_lval_ref_t = typename __add_lvalue_reference_helper<_Tp>::type;

  } // end of __detail:: namespace

  template<class _Tp>
  struct add_rvalue_reference {
    using type = typename __detail::__add_rvalue_reference_helper<_Tp>::type;
  };

  template<class _Tp>
  using add_rvalue_reference_t = typename add_rvalue_reference<_Tp>::type;

  template<class _Tp>
  struct add_lvalue_reference {
    using type = typename __detail::__add_lvalue_reference_helper<_Tp>::type;
  };

  template<class _Tp>
  using add_lvalue_reference_t = typename add_lvalue_reference<_Tp>::type;
  // End of std::add_lvalue_reference and std::add_rvalue_reference


  // Start of reference type checker implementations:
  template<class _Tp>
  struct is_lvalue_reference : public false_type { };

  template<class _Tp>
  struct is_lvalue_reference<_Tp&> : public true_type { };

  template<class _Tp>
  inline constexpr bool is_lvalue_reference_v = is_lvalue_reference<_Tp>::value;

  template<class _Tp>
  struct is_rvalue_reference : public false_type { };

  template<class _Tp>
  struct is_rvalue_reference<_Tp&&> : public true_type { };

  template<class _Tp>
  inline constexpr bool is_rvalue_reference_v = is_rvalue_reference<_Tp>::value;
  // End of reference type checkers


  // Start of std::is_member_object_pointer implementation:
  namespace __detail { // private implementation namespace
  
    template<class>
    struct __is_member_object_pointer_helper : public false_type { };

    template<class _Tp, class _Cp>
    struct __is_member_object_pointer_helper<_Tp _Cp::*>
      : public __not_<is_function<_Tp>>::type { };
  
  } // end of __detail:: namespace

  template<class _Tp>
  struct is_member_object_pointer
    : public bool_constant<__detail::__is_member_object_pointer_helper<remove_cv_t<_Tp>>::value> { };

  template<class _Tp>
  inline constexpr bool is_member_object_pointer_v = is_member_object_pointer<_Tp>::value;
  // End of std::is_member_object_pointer


  // Start of std::is_member_function_pointer implementation:
  namespace __detail { // private implementation namespace
  
    template<class>
    struct __is_member_function_pointer_helper : public false_type { };

    template<class _Tp, class _Cp>
    struct __is_member_function_pointer_helper<_Tp _Cp::*>
      : public is_function<_Tp>::type { };
  
  } // end of __detail:: namespace

  template<class _Tp>
  struct is_member_function_pointer
    : public bool_constant<__detail::__is_member_function_pointer_helper<remove_cv_t<_Tp>>::value> { };
  
  template<class _Tp>
  inline constexpr bool is_member_function_pointer_v = is_member_function_pointer<_Tp>::value;
  // End of std::is_member_function_pointer


  // Start of std::remove_cvref implementation:
  // this is the old implementation I used for basic testing
  /*
    template<class _Tp>
    struct remove_cvref {
      typedef remove_cv_t<remove_reference_t<_Tp>> type;
    };

    template<class _Tp>
    using remove_cvref_t = typename remove_cvref<_Tp>::type;
  */
  template<class _Tp>
  struct remove_cvref { using type = typename remove_cv<_Tp>::type; };

  template<class _Tp>
  struct remove_cvref<_Tp&> { using type = typename remove_cv<_Tp>::type; };

  template<class _Tp>
  struct remove_cvref<_Tp&&> { using type = typename remove_cv<_Tp>::type; };

  template<class _Tp>
  using remove_cvref_t = typename remove_cvref<_Tp>::type;
  // End of std::remove_cvref


  // Start of std::add_pointer implementation:
  namespace __detail { // implementation namespace

    template<class _Tp, class = void>
    struct __add_pointer_helper { using type = _Tp; };

    template<class _Tp>
    struct __add_pointer_helper<_Tp, void_t<_Tp*>> { using type = _Tp*; };

  } // end of __detail::

  template<class _Tp>
  struct add_pointer : public __detail::__add_pointer_helper<_Tp> { };

  template<class _Tp>
  struct add_pointer<_Tp&> { using type = _Tp*; };

  template<class _Tp>
  struct add_pointer<_Tp&&> { using type = _Tp*; };

  template<class _Tp>
  using add_pointer_t = typename add_pointer<_Tp>::type;
  // End of std::add_pointer


  // Start of std::remove_pointer implementation
  namespace __detail { // implementation namespace

    template<class _Tp, class>
    struct __remove_pointer_helper { using type = _Tp; };

    template<class _Tp, class _Up>
    struct __remove_pointer_helper<_Tp, _Up*> { using type = _Up; };

  } // end of __detail::

  template<class _Tp>
  struct remove_pointer : public __detail::__remove_pointer_helper<_Tp, remove_cv_t<_Tp>> { };

  template<class _Tp>
  using remove_pointer_t = typename remove_pointer<_Tp>::type;
  // End of std::remove_pointer


  // Start of std::__detail::__cref and std::__detail::__class_or_enum implementations:
  namespace __detail { // using __detail namespace since these are for internal use

    template<class _Tp>
    using __cref = const remove_reference_t<_Tp>&;

    template<class _Tp>
    struct __class_or_enum
      : public bool_constant<is_class<_Tp>::value || is_union<_Tp>::value || is_enum<_Tp>::value> { };

  } // end of __detail::
  // End of std::__detail::__cref and std::__detail::__class_or_enum


  // Start of std::declval() implementation:
  namespace __detail {

    template<class _Tp, class _Up = _Tp&&>
    _Up __declval(int);

    template<class _Tp>
    _Tp __declval(long);

    template<class _Tp>
    struct __declval_protector { static const bool __stop = false; };

  }

  template<class _Tp>
  auto declval() noexcept -> decltype(__detail::__declval<_Tp>(0)) {
    static_assert(__detail::__declval_protector<_Tp>::__stop, 
      "std::declval() must not be used in evaluated expressions!");
    return __detail::__declval<_Tp>(0);
  }

  // This is the original implementation which im leaving commented out incase I switch to it.
  // The implementation I went with above is being used currently because it checks if the function is used in an evaluated expression.
  //template<class _Tp>
  //typename add_rvalue_reference<_Tp>::type declval() noexcept;
  // End of std::declval()


  // Start of std::is_trivial implementation:
  template<class _Tp>
  struct is_trivial : public bool_constant<(__is_trivial(_Tp))> {
    static_assert(__is_complete_or_unbounded(__type_identity<_Tp>{}),
      "std::is_trivial requires the template argument to be a complete class or an unbounded  array!");
  };

  template<class _Tp>
  inline constexpr bool is_trivial_v = __is_trivial(_Tp);

  template<class _Tp>
  struct is_trivially_copyable : public bool_constant<(__is_trivially_copyable(_Tp))> {
    static_assert(__is_complete_or_unbounded(__type_identity<_Tp>{}),
      "std::is_trivially_copyable requires the template argument to be a complete class or an unbounded  array!");
  };

  template<class _Tp>
  inline constexpr bool is_trivially_copyable_v = __is_trivially_copyable(_Tp);
  // End of std::is_trivial


  // Start of class/type property checker function implementations:
  // I'm adding this just incase, but I most likely will never use it which is why it is marked deprecated
  template<class _Tp>
  struct [[deprecated]] is_literal_type 
    : public bool_constant<(__is_literal_type(_Tp))> {
    static_assert(__is_complete_or_unbounded(__type_identity<_Tp>{}),
      "std::is_literal_type requires the template argument to be a complete class or an unbounded  array!");
  };

  template<class _Tp>
  struct is_empty : public bool_constant<(__is_empty(_Tp))> { };

  template<class _Tp>
  struct is_polymorphic : public bool_constant<(__is_polymorphic(_Tp))> { };

  template<class _Tp>
  struct is_final : public bool_constant<(__is_final(_Tp))> { };

  template<class _Tp>
  struct is_abstract : public bool_constant<(__is_abstract(_Tp))> { };
  // End of class/type property checkers


  // Start of std::is_assignable implementation:
  namespace __detail { // internal implementation namespace

    template<class _Tp, class _Up>
    class __is_assignable_helper : public __sfinae_types {
      template<class _Tp1, class _Up1>
      static decltype(declval<_Tp1>() = declval<_Up1>(), __one()) __test(int);

      template<class, class>
      static __two __test(...);

      public:
        static constexpr bool value = sizeof(__test<_Tp, _Up>(0)) == 1;
    };

    template<class _Tp, class _Up> // this is also publicly defined
    struct is_assignable
      : public bool_constant<__is_assignable_helper<_Tp, _Up>::value> { };

    template<class _Tp, bool = is_void<_Tp>::value>
    struct __is_copy_assignable_impl;

    template<class _Tp>
    struct __is_copy_assignable_impl<_Tp, true> : public false_type { };

    template<class _Tp>
    struct __is_copy_assignable_impl<_Tp, false>
      : public is_assignable<_Tp&, const _Tp&> { };

    template<class _Tp, bool = is_void<_Tp>::value>
    struct __is_move_assignable_impl;

    template<class _Tp>
    struct __is_move_assignable_impl<_Tp, true> : public false_type { };

    template<class _Tp>
    struct __is_move_assignable_impl<_Tp, false>
      : public is_assignable<_Tp&, _Tp&&> { };

    template<class _Tp, class _Up>
    struct __is_nt_assignable_impl
      : public bool_constant<noexcept(declval<_Tp>() = declval<_Up>())> { };
    
    template<class _Tp, class _Up> // is also publicly defined
    struct is_nothrow_assignable : public __and_<is_assignable<_Tp, _Up>,
                                                 __is_nt_assignable_impl<_Tp, _Up>>::type { };

    template<class _Tp, bool = is_void<_Tp>::value>
    struct __is_nt_copy_assignable_impl;

    template<class _Tp>
    struct __is_nt_copy_assignable_impl<_Tp, true>
      : public false_type { };

    template<class _Tp>
    struct __is_nt_copy_assignable_impl<_Tp, false>
      : public is_nothrow_assignable<_Tp&, const _Tp&> { };

    template<class _Tp, bool = is_void<_Tp>::value>
    struct __is_nt_move_assignable_impl;

    template<class _Tp>
    struct __is_nt_move_assignable_impl<_Tp, true>
      : public false_type { };
    
    template<class _Tp>
    struct __is_nt_move_assignable_impl<_Tp, false>
      : public is_nothrow_assignable<_Tp&, _Tp&&> { };

  } // end of __detail:: namespace

  template<class _Tp, class _Up>
  struct is_assignable
    : public bool_constant<__detail::__is_assignable_helper<_Tp, _Up>::value> { };
  
  template<class _Tp>
  struct is_copy_assignable
    : public __detail::__is_copy_assignable_impl<_Tp> { };

  template<class _Tp>
  struct is_move_assignable
    : public __detail::__is_move_assignable_impl<_Tp> { };

  template<class _Tp, class _Up>
  struct is_nothrow_assignable : public __and_<is_assignable<_Tp, _Up>,
                                               __detail::__is_nt_assignable_impl<_Tp, _Up>>::type { };
  
  template<class _Tp>
  struct is_nothrow_copy_assignable
    : public __detail::__is_nt_copy_assignable_impl<_Tp> { };

  template<class _Tp>
  struct is_nothrow_move_assignable
    : public __detail::__is_nt_move_assignable_impl<_Tp> { };
    
#if (ALLOW_TEMPLATE_VAR == true) && (ALLOW_INLINE_VAR == true)
  template<class _Tp, class _Up>
  inline constexpr bool is_assignable_v = is_assignable<_Tp, _Up>::value;

  template<class _Tp>
  inline constexpr bool is_copy_assignable_v = is_copy_assignable<_Tp>::value;

  template<class _Tp>
  inline constexpr bool is_move_assignable_v = is_move_assignable<_Tp>::value;
#endif
  // End of std::is_assignable


  // Start of std::conjunction implementation:
  template<class...>
  struct conjunction : public true_type { };

  template<class _Tp>
  struct conjunction<_Tp> : public _Tp { };

  template<class _Tp, class... _Tn>
  struct conjunction<_Tp, _Tn...> 
    : public conditional_t<bool(_Tp::value), conjunction<_Tn...>, _Tp> { };

  template<class... _Tp>
  inline constexpr bool conjunction_v = conjunction<_Tp...>::value;
  // End of std::conjunction


  // Start of std::is_null_pointer implementation:
  template<class _Tp>
  struct is_null_pointer : public is_same<nullptr_t, remove_cv_t<_Tp>> { };

  template<class _Tp>
  inline constexpr bool is_null_pointer_v = is_null_pointer<_Tp>::value;
  // End of std::is_null_pointer


  // Start of std::is_pointer implementation:
  template<class _Tp>
  struct is_pointer : public false_type { };

  template<class _Tp>
  struct is_pointer<_Tp*> : public true_type { }; 

  template<class _Tp>
  struct is_pointer<_Tp* const> : public true_type { };

  template<class _Tp>
  struct is_pointer<_Tp* volatile> : public true_type { };

  template<class _Tp>
  struct is_pointer<_Tp* const volatile> : public true_type { };

  template<class _Tp>
  inline constexpr bool is_pointer_v = is_pointer<_Tp>::value;
  // End of std::is_pointer


  // Start of std::is_member_pointer implementation:
  namespace __detail { // internal implementation namespace

    template<class _Tp>
    struct __is_member_pointer_helper : public false_type { };

    template<class _Tp, class _Up>
    struct __is_member_pointer_helper<_Tp _Up::*> : public true_type { };

  } // end of __detail:: namespace

  template<class _Tp>
  struct is_member_pointer
    : public __detail::__is_member_pointer_helper<remove_cv_t<_Tp>> { };

  template<class _Tp>
  inline constexpr bool is_member_pointer_v = is_member_pointer<_Tp>::value;
  // End of std::is_member_pointer


  // Start of std::is_floating_point implementation:
  template<class _Tp>
  struct is_floating_point : public bool_constant<is_same<float, remove_cv_t<_Tp>>::value ||
                                                  is_same<double, remove_cv_t<_Tp>>::value ||
                                                  is_same<long double, remove_cv_t<_Tp>>::value> { };

  template<class _Tp>
  inline constexpr bool is_floating_point_v = is_floating_point<_Tp>::value;
  // End of std::is_floating_point


  // Start of std::is_integral implementation:
  namespace __detail { // used for private internal implementation

    template<class>
    struct __is_integral_helper : public false_type { };

    template<>
    struct __is_integral_helper<bool> : public true_type { };

    template<>
    struct __is_integral_helper<char> : public true_type { };

    template<>
    struct __is_integral_helper<signed char> : public true_type { };

    template<>
    struct __is_integral_helper<unsigned char> : public true_type { };

    template<>
    struct __is_integral_helper<wchar_t> : public true_type { };

    template<>
    struct __is_integral_helper<char16_t> : public true_type { };

    template<>
    struct __is_integral_helper<char32_t> : public true_type { };

    template<>
    struct __is_integral_helper<short> : public true_type { };

    template<>
    struct __is_integral_helper<unsigned short> : public true_type { };

    template<>
    struct __is_integral_helper<int> : public true_type { };

    template<>
    struct __is_integral_helper<unsigned int> : public true_type { };

    template<>
    struct __is_integral_helper<long> : public true_type { };

    template<>
    struct __is_integral_helper<unsigned long> : public true_type { };

    template<>
    struct __is_integral_helper<long long> : public true_type { };

    template<>
    struct __is_integral_helper<unsigned long long> : public true_type { };

  }// end of __detail:: namespace

  template<class _Tp>
  struct is_integral
    : public bool_constant<__detail::__is_integral_helper<remove_cv_t<_Tp>>::value> { };
  
  template<class _Tp>
  inline constexpr bool is_integral_v = is_integral<_Tp>::value;
  // End of std::is_integral


  // Start of std::is_arithmetic implementation:
  template<class _Tp>
  struct is_arithmetic
    : public bool_constant<is_integral<_Tp>::value || is_floating_point<_Tp>::value> { };
  
  template<class _Tp>
  inline constexpr bool is_arithmetic_v = is_arithmetic<_Tp>::value;
  // End of std::is_arithmetic


  // Start of std::is_scalar implementation:
  template<class _Tp>
  struct is_scalar : public bool_constant<is_arithmetic<_Tp>::value ||
                                          is_enum<_Tp>::value ||
                                          is_pointer<_Tp>::value ||
                                          is_member_pointer<_Tp>::value ||
                                          is_null_pointer<_Tp>::value> { };

  template<class _Tp>
  inline constexpr bool is_scalar_v = is_scalar<_Tp>::value;
  // End of std::is_scalar


  // Start of std::is_signed implementation:
  namespace __detail { // private internal implementation

    template<class _Tp, bool = is_arithmetic<_Tp>::value>
    struct __is_signed_impl : public bool_constant<(_Tp(0) > _Tp(-1))> { };

    template<class _Tp>
    struct __is_signed_impl<_Tp, false> : public false_type { };

  } // end of __detail:: namespace

  template<class _Tp>
  struct is_signed : public __detail::__is_signed_impl<_Tp>::type { };

  template<class _Tp>
  inline constexpr bool is_signed_v = is_signed<_Tp>::value;
  // End of std::is_signed


  // Start of std::is_signed_integral implementation:
  template<class _Tp>
  struct is_signed_integral
    : public bool_constant<is_integral<_Tp>::value && is_signed<_Tp>::value> { };

  template<class _Tp>
  inline constexpr bool is_signed_integral_v = is_signed_integral<_Tp>::value;
  // End of std::is_signed_integral


  // Start of std::is_unsigned implementation:
  namespace __detail { 

    template<class _Tp, bool = is_arithmetic<_Tp>::value>
    struct __is_unsigned_impl : public bool_constant<(_Tp(-1) > _Tp(0))> { };

    template<class _Tp>
    struct __is_unsigned_impl<_Tp, false> : public false_type { };

  } // end of __detail:: namespace

  template<class _Tp>
  struct is_unsigned
    : public __detail::__is_unsigned_impl<_Tp>::type { };

  template<class _Tp>
  inline constexpr bool is_unsigned_v = is_unsigned<_Tp>::value;
  // End of std::is_unsigned


  // Start of std::is_unsigned_integral implementation:
  template<class _Tp>
  struct is_unsigned_integral
    : public bool_constant<is_integral<_Tp>::value && is_unsigned<_Tp>::value> { };

  template<class _Tp>
  inline constexpr bool is_unsigned_integral_v = is_unsigned_integral<_Tp>::value;
  // End of std::is_unsigned_integral


  // Start of std::make_unsigned implementation:
  namespace __detail {

    template<class _Tp>
    struct __make_unsigned { using __type = _Tp; };

    template<>
    struct __make_unsigned<char> { using __type = unsigned char; };

    template<>
    struct __make_unsigned<signed char> { using __type = unsigned char; };

    template<>
    struct __make_unsigned<short> { using __type = unsigned short; };

    template<>
    struct __make_unsigned<int> { using __type = unsigned int; };

    template<>
    struct __make_unsigned<long> { using __type = unsigned long; };

    template<>
    struct __make_unsigned<long long> { using __type = unsigned long long; };

    template<class _Tp, bool _IsInt = is_integral<_Tp>::value,
                        bool _IsEnum = is_enum<_Tp>::value>
    class __make_unsigned_selector;

    template<class _Tp>
    class __make_unsigned_selector<_Tp, true, false> {
      using __unsigned_type = typename __make_unsigned<remove_cv_t<_Tp>>::__type;
      public:
        using __type = typename match_cv_qualifiers<_Tp, __unsigned_type>::type;
    };

    class __make_unsigned_selector_base {
      protected:
        template<class...> struct _List { };

        template<class _Tp, class... _Up>
        struct _List<_Tp, _Up...> : _List<_Up...> {
          static constexpr size_t __size = sizeof(_Tp);
        };

        template<size_t _Sz, class _Tp, bool = (_Sz <= _Tp::__size)>
        struct __select;

        template<size_t _Sz, class _Uint, class... _Uints>
        struct __select<_Sz, _List<_Uint, _Uints...>, true> {
          using __type = _Uint;
        };

        template<size_t _Sz, class _Uint, class... _Uints>
        struct __select<_Sz, _List<_Uint, _Uints...>, false>
          : public __select<_Sz, _List<_Uints...>> { };
    };

    template<class _Tp>
    class __make_unsigned_selector<_Tp, false, true> : public __make_unsigned_selector_base {
      using _Uints = _List<unsigned char, unsigned short, unsigned int,
                           unsigned long, unsigned long long>;
      using __unsigned_type = typename __select<sizeof(_Tp), _Uints>::__type;
      public:
        using __type = typename match_cv_qualifiers<_Tp, __unsigned_type>::type;
    };

    template<>
    struct __make_unsigned<wchar_t> {
      using __type = typename __make_unsigned_selector<wchar_t, false, true>::__type;
    };

    template<>
    struct __make_unsigned<char16_t> {
      using __type = typename __make_unsigned_selector<char16_t, false, true>::__type;
    };

    template<>
    struct __make_unsigned<char32_t> {
      using __type = typename __make_unsigned_selector<char32_t, false, true>::__type;
    };

  } // end of __detail:: namespace

  template<class _Tp>
  struct make_unsigned {
    using type = typename __detail::__make_unsigned_selector<_Tp>::__type;
  };

  template<>
  struct make_unsigned<bool>;

  template<class _Tp>
  using make_unsigned_t = typename make_unsigned<_Tp>::type;
  // End of std::make_unsigned


  // Start of std::make_signed implementation:
  namespace __detail {

    template<class _Tp>
    struct __make_signed { using __type = _Tp; };

    template<>
    struct __make_signed<char> { using __type = signed char; };

    template<>
    struct __make_signed<unsigned char> { using __type = signed char; };

    template<>
    struct __make_signed<unsigned short> { using __type = signed short; };

    template<>
    struct __make_signed<unsigned int> { using __type = signed int; };

    template<>
    struct __make_signed<unsigned long> { using __type = signed long; };

    template<>
    struct __make_signed<unsigned long long> { using __type = signed long long; };

    template<class _Tp, bool _IsInt = is_integral<_Tp>::value,
                        bool _IsEnum = is_enum<_Tp>::value>
    class __make_signed_selector;

    template<class _Tp>
    class __make_signed_selector<_Tp, true, false> {
      using __signed_type = typename __make_signed<remove_cv_t<_Tp>>::__type;
      public:
        using __type = typename match_cv_qualifiers<_Tp, __signed_type>::type;
    };

    template<class _Tp>
    class __make_signed_selector<_Tp, false, true> {
      using __unsigned_type = typename __make_unsigned_selector<_Tp>::__type;
      public:
        using __type = typename __make_signed_selector<__unsigned_type>::__type;
    };

    template<>
    struct __make_signed<wchar_t> {
      using __type = typename __make_signed_selector<wchar_t, false, true>::__type;
    };

    template<>
    struct __make_signed<char16_t> {
      using __type = typename __make_signed_selector<char16_t, false, true>::__type;
    };

    template<>
    struct __make_signed<char32_t> {
      using __type = typename __make_signed_selector<char32_t, false, true>::__type;
    };

  } // end of __detail:: namespace

  template<class _Tp>
  struct make_signed {
    using type = typename __detail::__make_signed_selector<_Tp>::__type;
  };

  template<>
  struct make_signed<bool>;

  template<class _Tp>
  using make_signed_t = typename make_signed<_Tp>::type;
  // End of std::make_signed


  // Start of std::decay implementation:
  namespace __detail {

    template<class _Tp>
    struct __decay_selector : public conditional_t<is_const<const _Tp>::value,
                                                   remove_cv<_Tp>,
                                                   add_pointer<_Tp>> { };

    template<class _Tp, size_t _Nm>
    struct __decay_selector<_Tp[_Nm]> { using type = _Tp*; };

    template<class _Tp>
    struct __decay_selector<_Tp[]> { using type = _Tp*; };

  } // end of __detail:: namespace

  template<class _Tp>
  struct decay { using type = typename __detail::__decay_selector<_Tp>::type; };

  template<class _Tp>
  struct decay<_Tp&> { using type = typename __detail::__decay_selector<_Tp>::type; };

  template<class _Tp>
  struct decay<_Tp&&> { using type = typename __detail::__decay_selector<_Tp>::type; };

  template<class _Tp>
  using decay_t = typename decay<_Tp>::type;

  namespace __detail { // namespace for internal implementations

    template<class _Tp>
    struct __strip_reference_wrapper {
      typedef _Tp __type;
    };

    template<class _Tp>
    struct __strip_reference_wrapper<reference_wrapper<_Tp>> {
      typedef _Tp& __type;
    };

    template<class _Tp>
    using __decay_and_strip = __strip_reference_wrapper<decay_t<_Tp>>;

  } // end of __detail:: namespace
  // End of std::decay


  // Start of std::common_type implementation:
  template<class... _Tp> // might need to comment this out(delete this comment when figured out)
  struct common_type;

  template<>
  struct common_type<> { };

  template<class _Tp>
  struct common_type<_Tp> : public common_type<_Tp, _Tp> { };

  namespace __detail {

    template<class _Tp>
    struct __success_type { using type = _Tp; };

    struct __failure_type { };

    struct __do_common_type_impl {
      template<class _Tp, class _Up>
      using __cond_t = decltype(true ? declval<_Tp>() : declval<_Up>());

      template<class _Tp, class _Up>
      static __success_type<decay_t<__cond_t<_Tp, _Up>>> _S_test(int);

      template<class _Tp, class _Up>
      static __success_type<remove_cvref_t<__cond_t<const _Tp&, const _Up&>>> _S_test_2(int);

      template<class, class>
      static __failure_type _S_test_2(...);

      template<class _Tp, class _Up>
      static decltype(_S_test_2<_Tp, _Up>(0)) _S_test(...);
    };

    template<class _Tp1, class _Tp2,
            class _Dp1 = decay_t<_Tp1>, class _Dp2 = decay_t<_Tp2>>
    struct __common_type_impl { using type = common_type<_Dp1, _Dp2>; };

    template<class _Tp1, class _Tp2>
    struct __common_type_impl<_Tp1, _Tp2, _Tp1, _Tp2> : private __do_common_type_impl {
      using type = decltype(_S_test<_Tp1, _Tp2>(0));
    };

  } // first temp end to __detail:: namespace

  // splitting the internal implementation namespace to add this specialization/overload for the main public function
  template<class _Tp1, class _Tp2> 
  struct common_type<_Tp1, _Tp2> : public __detail::__common_type_impl<_Tp1, _Tp2>::type { };

  namespace __detail { // continuing internal implementation

    template<class...>
    struct __common_type_pack { };

    template<class, class, class = void>
    struct __common_type_fold;

  } // second temp end to __detail:: namespace

  template<class _Tp1, class _Tp2, class... _Rp>
  struct common_type<_Tp1, _Tp2, _Rp...> : public __detail::__common_type_fold<common_type<_Tp1, _Tp2>,
                                                                               __detail::__common_type_pack<_Rp...>> { };

  namespace __detail { // final continuation of internal implementation

    template<class _CTp, class... _Rp>
    struct __common_type_fold<_CTp, __common_type_pack<_Rp...>,
                              void_t<typename _CTp::type>>
      : public common_type<typename _CTp::type, _Rp...> { };
    
    template<class _CTp, class _Rp>
    struct __common_type_fold<_CTp, _Rp, void> { };

  } // final end to __detail:: namespace

  template<class... _Tp>
  using common_type_t = typename common_type<_Tp...>::type;
  // End of std::common_type


  // Start of std::is_object implementation:
  template<class _Tp>
  struct is_object : public bool_constant<is_scalar<_Tp>::value ||
                                          is_array<_Tp>::value ||
                                          is_union<_Tp>::value || 
                                          is_class<_Tp>::value> { };
  
  // Below is an old implementation from GCC 4.8.3. leaving this in the code but commented out incase I decide to switch to it for whatever reason
  //template<class _Tp>
  //struct is_object : public __not_<__or_<is_function<_Tp>, 
  //                                       is_reference<_Tp>, 
  //                                       is_void<_Tp>>>::type { };
  // End of std::is_object


  // Start of std::is_destructible implementation:
  namespace __detail {

    struct __do_is_destructible_impl {
      template<class _Tp, class = decltype(declval<_Tp&>().~_Tp())>
      static true_type __test(int);

      template<class>
      static false_type __test(...);
    };

    template<class _Tp>
    struct __is_destructible_impl : public __do_is_destructible_impl {
      using type = decltype(__test<_Tp>(0));
    };

    template<class _Tp, bool = __or_<is_void<_Tp>, is_array_unknown_bounds<_Tp>, is_function<_Tp>>::value,
                        bool = __or_<is_reference<_Tp>, is_scalar<_Tp>>::value>
    struct __is_destructible_safe;

    template<class _Tp>
    struct __is_destructible_safe<_Tp, false, false>
      : public __is_destructible_impl<typename remove_all_extents<_Tp>::type>::type { };
    
    template<class _Tp>
    struct __is_destructible_safe<_Tp, true, false> : public false_type { };

    template<class _Tp>
    struct __is_destructible_safe<_Tp, false, true> : public true_type { };

  } // first end of __detail:: namespace

  template<class _Tp>
  struct is_destructible
    : public __detail::__is_destructible_safe<_Tp>::type {
    static_assert(__is_complete_or_unbounded(__type_identity<_Tp>{}),
      "std::is_destructible template argument requires a complete class or an unbounded array!");
  };
  
  template<class _Tp>
  struct is_trivially_destructible : public __and_<__detail::__is_destructible_safe<_Tp>,
                                                   bool_constant<__has_trivial_destructor(_Tp)>>::type {
    static_assert(__is_complete_or_unbounded(__type_identity<_Tp>{}),
      "std::is_trivially_destructible template argument requires a complete class or an unbounded array!");
  };
  
  template<class _Tp>
  inline constexpr bool is_destructible_v = is_destructible<_Tp>::value;

  template<class _Tp>
  inline constexpr bool is_trivially_destructible_v = is_trivially_destructible<_Tp>::value;

  namespace __detail { // continued internal implementation for nothrow version of is_destructible

    struct __do_is_nt_destructible_impl {
      template<class _Tp>
      static bool_constant<noexcept(declval<_Tp&>().~_Tp())> __test(int);

      template<class>
      static false_type __test(...);
    };

    template<class _Tp>
    struct __is_nt_destructible_impl : public __do_is_nt_destructible_impl {
      using type = decltype(__test<_Tp>(0));
    };

    template<class _Tp, bool = __or_<is_void<_Tp>,
                                     is_array_unknown_bounds<_Tp>,
                                     is_function<_Tp>>::value,
                        bool = __or_<is_reference<_Tp>,
                                     is_scalar<_Tp>>::value>
    struct __is_nt_destructible_safe;

    template<class _Tp>
    struct __is_nt_destructible_safe<_Tp, false, false>
      : public __is_nt_destructible_impl<typename remove_all_extents<_Tp>::type>::type { };
    
    template<class _Tp>
    struct __is_nt_destructible_safe<_Tp, true, false>
      : public false_type { };

    template<class _Tp>
    struct __is_nt_destructible_safe<_Tp, false, true>
      : public true_type { };

  } // second end of __detail:: namespace

  template<class _Tp>
  struct is_nothrow_destructible
    : public __detail::__is_nt_destructible_safe<_Tp>::type {
    static_assert(__is_complete_or_unbounded(__type_identity<_Tp>{}),
      "std::is_nothrow_destructible template argument is required to be a complete class or an unbounded array!");
  };

  template<class _Tp>
  inline constexpr bool is_nothrow_destructible_v = is_nothrow_destructible<_Tp>::value;
  
  // This implementation for is_trivially_destructible_v was added along with concepts, but I have it commented out since I'm sticking to the typical func_v = func<T>::value notation.
  /*template<class _Tp, class = void>
  inline constexpr bool is_trivially_destructible_v = false;

  template<class _Tp>
  inline constexpr bool is_trivially_destructible_v<_Tp, _Require<__not_<is_reference<_Tp>>,
                                                                  __not_<is_void<decltype(declval<_Tp&>().~_Tp())>>>>
    = __has_trivial_destructor(_Tp);

  template<class _Tp>
  inline constexpr bool is_trivially_destructible_v<_Tp&> = true;

  template<class _Tp>
  inline constexpr bool is_trivially_destructible_v<_Tp&&> = true;

  template<class _Tp, size_t _Nm>
  inline constexpr bool is_trivially_destructible_v<_Tp[_Nm]>
    = is_trivially_destructible_v<_Tp>;*/
  // End of std::is_destructible


  // Start of std::is_constructible and variant function implementations:
  namespace __detail {

    struct __do_is_default_constructible_impl {
      template<class _Tp, class = decltype(_Tp())>
      static true_type __test(int);

      template<class>
      static false_type __test(...);
    };

    template<class _Tp>
    struct __is_default_constructible_impl : public __do_is_default_constructible_impl {
      using type = decltype(__test<_Tp>(0));
    };

    template<class _Tp>
    struct __is_default_constructible_atom
      : public __and_<__not_<is_void<_Tp>>,
                      __is_default_constructible_impl<_Tp>>::type { };

    template<class _Tp, bool = is_array<_Tp>::value>
    struct __is_default_constructible_safe;

    template<class _Tp>
    struct __is_default_constructible_safe<_Tp, true>
      : public __and_<is_array_known_bounds<_Tp>,
                      __is_default_constructible_atom<typename remove_all_extents<_Tp>::type>>::type { };

    template<class _Tp>
    struct __is_default_constructible_safe<_Tp, false>
      : public __is_default_constructible_atom<_Tp>::type { };

    template<class _Tp>
    struct is_default_constructible
      : public bool_constant<__is_default_constructible_safe<_Tp>::value> { };

    struct __do_is_static_castable_impl {
      template<class _From, class _To, class = decltype(static_cast<_To>(declval<_From>()))>
      static true_type __test(int);

      template<class, class>
      static false_type __test(...);
    };

    template<class _From, class _To>
    struct __is_static_castable_impl : public __do_is_static_castable_impl {
      using type = decltype(__test<_From, _To>(0));
    };

    template<class _From, class _To>
    struct __is_static_castable_safe
      : public __is_static_castable_impl<_From, _To>::type { };
    
    template<class _From, class _To>
    struct __is_static_castable
      : public bool_constant<__is_static_castable_safe<_From, _To>::value> { };

    struct __do_is_direct_constructible_impl {
      // This implementation is modified to use the suggestion from issue c++/51222 which recomends adding ::delete before the ::new to avoid creating undeletable dynamic allocations.
      template<class _Tp, class _Arg,
               class = decltype(::delete ::new _Tp(declval<_Arg>()))>
      static true_type __test(int);

      template<class, class>
      static false_type __test(...);
    };

    template<class _Tp, class _Arg>
    struct __is_direct_constructible_impl
      : public __do_is_direct_constructible_impl {
      using type = decltype(__test<_Tp, _Arg>(0));
    };

    template<class _Tp, class _Arg>
    struct __is_direct_constructible_new_safe
      : public __and_<is_destructible<_Tp>,
                      __is_direct_constructible_impl<_Tp, _Arg>>::type { };
    
    template<class _From, class _To,
             bool = __not_<__or_<is_void<_From>, is_function<_From>>>::value>
    struct __is_base_to_derived_ref;

    template<class _From, class _To>
    struct __is_base_to_derived_ref<_From, _To, true> {
      using __src_t = typename remove_cv<typename remove_reference<_From>::type>::type;

      using __dst_t = typename remove_cv<typename remove_reference<_To>::type>::type;

      using type = __and_<__not_<is_same<__src_t, __dst_t>>,
                          is_base_of<__src_t, __dst_t>>;

      static constexpr bool value = type::value;
    };

    template<class _From, class _To>
    struct __is_base_to_derived_ref<_From, _To, false> : public false_type { };

    template<class _From, class _To,
             bool = __and_<is_lvalue_reference<_From>,
                           is_rvalue_reference<_To>>::value>
    struct __is_lvalue_to_rvalue_ref;

    template<class _From, class _To>
    struct __is_lvalue_to_rvalue_ref<_From, _To, true> {
      using __src_t = typename remove_cv<typename remove_reference<_From>::type>::type;

      using __dst_t = typename remove_cv<typename remove_reference<_From>::type>::type;

      using type = __and_<__not_<is_function<__src_t>>,
                          __or_<is_same<__src_t, __dst_t>,
                                is_base_of<__dst_t, __src_t>>>;
      
      static constexpr bool value = type::value;
    };

    template<class _From, class _To>
    struct __is_lvalue_to_rvalue_ref<_From, _To, false> : public false_type { };

    template<class _Tp, class _Arg>
    struct __is_direct_constructible_ref_cast
      : public __and_<__is_static_castable<_Arg, _Tp>,
                      __not_<__or_<__is_base_to_derived_ref<_Arg, _Tp>,
                                   __is_lvalue_to_rvalue_ref<_Arg, _Tp>
                                   >>>::type { };
    
    template<class _Tp, class _Arg>
    struct __is_direct_constructible_new
      : public conditional<is_reference<_Tp>::value,
                           __is_direct_constructible_ref_cast<_Tp, _Arg>,
                           __is_direct_constructible_new_safe<_Tp, _Arg>>::type { };

    template<class _Tp, class _Arg>
    struct __is_direct_constructible
      : public bool_constant<__is_direct_constructible_new<_Tp, _Arg>::value> { };
    
    struct __do_is_nary_constructible_impl {
      template<class _Tp, class... _Args,
               class = decltype(_Tp(declval<_Args>()...))>
      static true_type __test(int);

      template<class, class>
      static false_type __test(...);
    };

    template<class _Tp, class... _Args>
    struct __is_nary_constructible_impl : public __do_is_nary_constructible_impl {
      using type = decltype(__test<_Tp, _Args...>(0));
    };

    template<class _Tp, class... _Args>
    struct __is_nary_constructible
      : public __is_nary_constructible_impl<_Tp, _Args...>::type {
      static_assert(sizeof...(_Args) > 1, "nary_constructible only useful for >1 template arguments");
    };

    template<class _Tp, class... _Args>
    struct __is_constructible_impl
      : public __is_nary_constructible<_Tp, _Args...> { };

    template<class _Tp, class _Arg>
    struct __is_constructible_impl<_Tp, _Arg>
      : public __is_direct_constructible<_Tp, _Arg> { };

    template<class _Tp>
    struct __is_constructible_impl<_Tp>
      : public is_default_constructible<_Tp> { };

    template<class _Tp, class... _Args>
    struct is_constructible
      : public bool_constant<__is_constructible_impl<_Tp, _Args...>::value> { };
    
    template<class _Tp, bool = is_void<_Tp>::value>
    struct __is_copy_constructible_impl;

    template<class _Tp>
    struct __is_copy_constructible_impl<_Tp, true> : public false_type { };

    template<class _Tp>
    struct __is_copy_constructible_impl<_Tp, false>
      : public is_constructible<_Tp, const _Tp&> { };

    template<class _Tp, bool = is_void<_Tp>::value>
    struct __is_move_constructible_impl;

    template<class _Tp>
    struct __is_move_constructible_impl<_Tp, true> : public false_type { };
    
    template<class _Tp>
    struct __is_move_constructible_impl<_Tp, false>
      : public is_constructible<_Tp, _Tp&&> { };

    template<class _Tp>
    struct __is_nt_default_constructible_atom
      : public bool_constant<noexcept(_Tp())> { };

    template<class _Tp, bool = is_array<_Tp>::value>
    struct __is_nt_default_constructible_impl;

    template<class _Tp>
    struct __is_nt_default_constructible_impl<_Tp, true>
      : public __and_<is_array_known_bounds<_Tp>,
                      __is_nt_default_constructible_atom<remove_all_extents_t<_Tp>>>::type { };

    template<class _Tp>
    struct __is_nt_default_constructible_impl<_Tp, false>
      : public __is_nt_default_constructible_atom<_Tp> { };

    template<class _Tp>
    struct is_nothrow_default_constructible
      : public __and_<is_default_constructible<_Tp>,
                      __is_nt_default_constructible_impl<_Tp>>::type { };

    template<class _Tp, class... _Args>
    struct __is_nt_constructible_impl
      : public bool_constant<noexcept(_Tp(declval<_Args>()...))> { };

    template<class _Tp, class _Arg>
    struct __is_nt_constructible_impl<_Tp, _Arg>
      : public bool_constant<noexcept(static_cast<_Tp>(declval<_Arg>()))> { };
    
    template<class _Tp>
    struct __is_nt_constructible_impl<_Tp>
      : public is_nothrow_default_constructible<_Tp> { };
    
    template<class _Tp, class... _Args>
    struct is_nothrow_constructible
      : public __and_<is_constructible<_Tp, _Args...>,
                      __is_nt_constructible_impl<_Tp, _Args...>>::type { };

    template<class _Tp, bool = is_void<_Tp>::value>
    struct __is_nothrow_copy_constructible_impl;

    template<class _Tp>
    struct __is_nothrow_copy_constructible_impl<_Tp, true>
      : public false_type { };

    template<class _Tp>
    struct __is_nothrow_copy_constructible_impl<_Tp, false>
      : public is_nothrow_constructible<_Tp, const _Tp&> { };

    template<class _Tp, bool = is_void<_Tp>::value>
    struct __is_nothrow_move_constructible_impl;

    template<class _Tp>
    struct __is_nothrow_move_constructible_impl<_Tp, true>
      : public false_type { };

    template<class _Tp>
    struct __is_nothrow_move_constructible_impl<_Tp, false>
      : public is_nothrow_constructible<_Tp, _Tp&&> { };

  } // end of __detail:: namespace

  // I may switch these functions to inherit a bool_constant that takes the value of the impl functions instead of directly inheriting the type fromt he impl function
  template<class _From, class _To>
  struct is_static_castable
    : public __detail::__is_static_castable_safe<_From, _To> { };
  
  template<class _From, class _To>
  inline constexpr bool is_static_castable_v = is_static_castable<_From, _To>::value;

  template<class _Tp>
  struct is_default_constructible
    : public __detail::__is_default_constructible_safe<_Tp> { };
  
  template<class _Tp>
  inline constexpr bool is_default_constructible_v = is_default_constructible<_Tp>::value;

  template<class _Tp>
  struct is_nothrow_default_constructible
    : public __and_<is_default_constructible<_Tp>,
                    __detail::__is_nt_default_constructible_impl<_Tp>> { };

  template<class _Tp>
  inline constexpr bool is_nothrow_default_constructible_v = is_nothrow_default_constructible<_Tp>::value;

  template<class _Tp, class... _Args>
  struct is_constructible
    : public __detail::__is_constructible_impl<_Tp, _Args...> { };

  template<class _Tp, class... _Args>
  inline constexpr bool is_constructible_v = is_constructible<_Tp, _Args...>::value;

  template<class _Tp, class... _Args>
  struct is_nothrow_constructible
    : public __and_<is_constructible<_Tp, _Args...>,
                    __detail::__is_nt_constructible_impl<_Tp, _Args...>> { };

  template<class _Tp, class... _Args>
  inline constexpr bool is_nothrow_constructible_v = is_nothrow_constructible<_Tp, _Args...>::value;

  template<class _Tp>
  struct is_copy_constructible
    : public __detail::__is_copy_constructible_impl<_Tp> { };

  template<class _Tp>
  inline constexpr bool is_copy_constructible_v = is_copy_constructible<_Tp>::value;

  template<class _Tp>
  struct is_nothrow_copy_constructible
    : public __detail::__is_nothrow_copy_constructible_impl<_Tp> { };

  template<class _Tp>
  inline constexpr bool is_nothrow_copy_constructible_v = is_nothrow_copy_constructible<_Tp>::value;

  template<class _Tp>
  struct is_move_constructible
    : public __detail::__is_move_constructible_impl<_Tp> { };

  template<class _Tp>
  inline constexpr bool is_move_constructible_v = is_move_constructible<_Tp>::value;

  template<class _Tp>
  struct is_nothrow_move_constructible
    : public __detail::__is_nothrow_move_constructible_impl<_Tp> { };

  template<class _Tp>
  inline constexpr bool is_nothrow_move_constructible_v = is_nothrow_move_constructible<_Tp>::value;
  // End of std::is_constructible and variants


  // Start of std::is_trivially_constructible and std::is_trivially_default_constructible implementation:
  namespace __detail { // namespace for internal implementation

    template<class _Tp, class... _Args>
    using __is_trivially_constructible_impl
      = bool_constant<__is_trivially_constructible(_Tp, _Args...)>;

  } // end of __detail:: namespace

  template<class _Tp, class... _Args>
  struct is_trivially_constructible
    : public __detail::__is_trivially_constructible_impl<_Tp, _Args...> {
    static_assert(__is_complete_or_unbounded(__type_identity<_Tp>{}),
      "std::is_trivially_constructible requires template argument 1 to be a complete class or an unbounded array!");
  };

  template<class _Tp>
  struct is_trivially_default_constructible
    : public __detail::__is_trivially_constructible_impl<_Tp> {
    static_assert(__is_complete_or_unbounded(__type_identity<_Tp>{}),
      "std::is_trivially_default_constructible requires its argument to be a complete class or an unbounded array!");
  };
  // End of std::is_trivially_constructible and std::is_trivially_default_constructible


  // Start of __detail::__is_implicitly_default_constructible implementation:
  namespace __detail { // private namespace for this internal implementation

    inline namespace __H { // helper namespace for private namespace __detail

      struct __do_is_implicitly_default_constructible_impl {
        template<class _Tp>
        static void __helper(const _Tp&);

        template<class _Tp>
        static true_type __test(const _Tp&, decltype(__helper<const _Tp&>({}))* = 0);

        static false_type __test(...);
      };

      template<class _Tp>
      struct __is_implicitly_default_constructible_impl
        : public __do_is_implicitly_default_constructible_impl {
        using type = decltype(__test(declval<_Tp>()));
      };

      template<class _Tp>
      struct __is_implicitly_default_constructible_safe
        : public __is_implicitly_default_constructible_impl<_Tp>::type { };

    } // end of __H:: namespace

    template<class _Tp>
    struct __is_implicitly_default_constructible
      : public __and_<__is_constructible_impl<_Tp>,
                      __is_implicitly_default_constructible_safe<_Tp>>::type { };

  } // end of __detail:: namespace
  // End of __detail::__is_implicitly_default_constructible


  // Start of std::is_trivially_copy_constructible and std::is_trivially_move_constructible implementation:
  template<class _Tp>
  struct is_trivially_copy_constructible
    : public __detail::__is_trivially_constructible_impl<_Tp, __detail::__add_lval_ref_t<const _Tp>> {
    static_assert(__is_complete_or_unbounded(__type_identity<_Tp>{}),
      "std::is_trivially_copy_constructible requires it's argument to be a complete class or an unbounded array!");
  };

  template<class _Tp>
  struct is_trivially_move_constructible
    : public __detail::__is_trivially_constructible_impl<_Tp, __detail::__add_rval_ref_t<_Tp>> {
    static_assert(__is_complete_or_unbounded(__type_identity<_Tp>{}),
      "std::is_trivially_move_constructible requires it's argument to be a complete class or an unbounded array!");
  };
  // End of std::is_trivially_copy_constructible and std::is_trivially_move_constructible


  // Start of std::is_trivially_assignable and variants implementations:
  namespace __detail { // namespace for internal implementation

    template<class _Tp, class _Up>
    using __is_trivially_assignable_impl = bool_constant<__is_trivially_assignable(_Tp, _Up)>;

  } // end of __detail:: namespace

  template<class _Tp, class _Up>
  struct is_trivially_assignable
    : public __detail::__is_trivially_assignable_impl<_Tp, _Up> {
    static_assert(__is_complete_or_unbounded(__type_identity<_Tp>{}),
      "std::is_trivially_assignable requires it's argument to be a complete class or an unbounded array!");
  };

  template<class _Tp>
  struct is_trivially_copy_assignable
    : public __detail::__is_trivially_assignable_impl<__detail::__add_lval_ref_t<_Tp>,
                                                      __detail::__add_lval_ref_t<const _Tp>> {
    static_assert(__is_complete_or_unbounded(__type_identity<_Tp>{}),
      "std::is_trivially_copy_assignable requires it's argument to be a complete class or an unbounded array!");
  };

  template<class _Tp>
  struct is_trivially_move_assignable
    : public __detail::__is_trivially_assignable_impl<__detail::__add_lval_ref_t<_Tp>,
                                                      __detail::__add_rval_ref_t<_Tp>> {
    static_assert(__is_complete_or_unbounded(__type_identity<_Tp>{}),
      "std::is_trivially_move_assignable requires it's argument to be a complete class or an unbounded array!");
  };
  // End of std::is_trivially_assignable and variants


  // Start of std::has_virtual_destructor implementation:
  template<class _Tp>
  struct has_virtual_destructor : public bool_constant<__has_virtual_destructor(_Tp)> {
    static_assert(__is_complete_or_unbounded(__type_identity<_Tp>{}),
      "std::has_virtual_destructor requires its argument to be a complete class or an unbounded array!");
  };
  // End of std::has_virtual_destructor


  // Start of std::is_convertible implementation:
  // This is an old commented out implementation. this implementation works, but I switched to the same implementation that is currently in libstdc++
  // This commented out implementation is from cppreference.com.
  /*namespace __detail {

    template<class _Tp>
    auto test_returnable(int) -> decltype(void(static_cast<_Tp(*)()>(nullptr)), true_type{});

    template<class>
    auto test_returnable(...) -> false_type;

    template<class _From, class _To>
    auto test_implicitly_convertible(int) -> decltype(void(declval<void(&)(_To)>()(declval<_From>())), true_type{});

    template<class>
    auto test_implicitly_convertible(...) -> false_type;

  } // end of __detail:: namespace

  template<class _From, class _To>
  struct is_convertible : public bool_constant<(decltype(__detail::test_returnable<_To>(0))::value &&
                                                decltype(__detail::test_implicitly_convertible<_From, _To>(0))::value) || 
                                                (is_void<_From>::value && is_void<_To>::value)> { };

  template<class _From, class _To>
  inline constexpr bool is_convertible_v = is_convertible<_From, _To>::value;*/

  // This is the new implementation I'm using which includes a new helper function and nothrow variants
  namespace __detail {
    
    template<class _From, class _To,
             bool = __or_<is_void<_From>, is_function<_To>, is_array<_To>>::value>
    struct __is_convertible_helper {
      using type = typename is_void<_To>::type;
    };

    template<class _From, class _To>
    class __is_convertible_helper<_From, _To, false> {
      template<class _To1>
      static void __test_aux(_To1) noexcept;

      template<class _From1, class _To1,
               class = decltype(__test_aux<_To1>(declval<_From1>()))>
      static true_type __test(int);

      template<class, class>
      static false_type __test(...);

      public:
        using type = decltype(__test<_From, _To>(0));
    };

  } // end of __detail:: namespace

  template<class _From, class _To>
  struct is_convertible
    : public __detail::__is_convertible_helper<_From, _To>::type { };

  template<class _ToElementType, class _FromElementType>
  using __is_array_convertible = is_convertible<_FromElementType(*)[], _ToElementType(*)[]>;
  
  namespace __detail { // __detail continued for nothrow implementation

    template<class _From, class _To,
             bool = __or_<is_void<_From>, is_function<_To>, is_array<_To>>::value>
    struct __is_nt_convertible_helper : public is_void<_To> { };

    template<class _From, class _To>
    class __is_nt_convertible_helper<_From, _To, false> {
      template<class _To1>
      static void __test_aux(_To1) noexcept;

      template<class _From1, class _To1>
      static bool_constant<noexcept(__test_aux<_To1>(declval<_From1>()))> __test(int);

      template<class, class>
      static false_type __test(...);

      public:
        using type = decltype(__test<_From, _To>(0));
    };

  } // end of __detail:: namespace

  template<class _From, class _To>
  struct is_nothrow_convertible
    : public __detail::__is_nt_convertible_helper<_From, _To>::type { };

  template<class _From, class _To>
  inline constexpr bool is_nothrow_convertible_v = is_nothrow_convertible<_From, _To>::value;
  // End of std::is_convertible


  // Start of std::common_reference implementation:
  namespace __detail { // start of private implementation namespace

    template<class _From, class _To>
    using __copy_cv = typename match_cv_qualifiers<_From, _To>::type;

    template<class _Xp, class _Yp>
    using __cond_res = decltype(false ? declval<_Xp(&)()>()() : declval<_Yp(&)()>()());

    template<class _Ap, class _Bp, class = void>
    struct __common_ref_impl { };

    template<class _Ap, class _Bp>
    using __common_ref = typename __common_ref_impl<_Ap, _Bp>::type;

    template<class _Xp, class _Yp>
    using __condres_cvref = __cond_res<__copy_cv<_Xp, _Yp>&, __copy_cv<_Yp, _Xp>&>;

    template<class _Xp, class _Yp>
    struct __common_ref_impl<_Xp&, _Yp&, void_t<__condres_cvref<_Xp, _Yp>>>
      : enable_if<is_reference<__condres_cvref<_Xp, _Yp>>::value, __condres_cvref<_Xp, _Yp>> { };

    template<class _Xp, class _Yp>
    using __common_ref_C = remove_reference_t<__common_ref<_Xp&, _Yp&>>&&;

    template<class _Xp, class _Yp>
    struct __common_ref_impl<_Xp&&, _Yp&&, _Require<is_convertible<_Xp&&, __common_ref_C<_Xp, _Yp>>,
                                                    is_convertible<_Yp&&, __common_ref_C<_Xp, _Yp>>>> {
      using type = __common_ref_C<_Xp, _Yp>;
    };

    template<class _Xp, class _Yp>
    using __common_ref_D = __common_ref<const _Xp&, _Yp&>;

    template<class _Xp, class _Yp>
    struct __common_ref_impl<_Xp&&, _Yp&, _Require<is_convertible<_Xp&&, __common_ref_D<_Xp, _Yp>>>> {
      using type = __common_ref_D<_Xp, _Yp>;
    };

    template<class _Xp, class _Yp>
    struct __common_ref_impl<_Xp&, _Yp&&> : __common_ref_impl<_Yp&&, _Xp&> { };

  } // end of __detail:: namespace

  template<class _Tp, class _Up, template<class> class _TQual, template<class> class _UQual>
  struct basic_common_reference { };

  namespace __detail { // continuation of private implementation namespace

    template<class _Tp>
    struct __xref {
      template<class _Up>
      using __type = __copy_cv<_Tp, _Up>;
    };

    template<class _Tp>
    struct __xref<_Tp&> {
      template<class _Up>
      using __type = __copy_cv<_Tp, _Up>&;
    };

    template<class _Tp>
    struct __xref<_Tp&&> {
      template<class _Up>
      using __type = __copy_cv<_Tp, _Up>&&;
    };

    template<class _Tp1, class _Tp2>
    using __basic_common_ref = typename basic_common_reference<remove_cvref_t<_Tp1>,
                                                               remove_cvref_t<_Tp2>,
                                                               __xref<_Tp1>::template __type,
                                                               __xref<_Tp2>::template __type>::type;

  } // end of __detail:: namespace

  template<class... _Tp>
  struct common_reference { };

  template<class... _Tp>
  using common_reference_t = typename common_reference<_Tp...>::type;

  template<>
  struct common_reference<> { };

  template<class _Tp0>
  struct common_reference<_Tp0> { using type = _Tp0; };

  namespace __detail { // continuation of private implementation namespace

    template<class _Tp1, class _Tp2, int _Bullet = 1, typename = void>
    struct __common_reference_impl : __common_reference_impl<_Tp1, _Tp2, _Bullet + 1> { };

  } // end of __detail:: namespace

  template<class _Tp1, class _Tp2>
  struct common_reference<_Tp1, _Tp2> : __detail::__common_reference_impl<_Tp1, _Tp2> { };

  namespace __detail { // continuation of private implementation namespace

    template<class _Tp1, class _Tp2>
    struct __common_reference_impl<_Tp1&, _Tp2&, 1, void_t<__common_ref<_Tp1&, _Tp2&>>> {
      using type = __common_ref<_Tp1&, _Tp2&>;
    };

    template<class _Tp1, class _Tp2>
    struct __common_reference_impl<_Tp1&&, _Tp2&&, 1, void_t<__common_ref<_Tp1&&, _Tp2&&>>> {
      using type = __common_ref<_Tp1&&, _Tp2&&>;
    };

    template<class _Tp1, class _Tp2>
    struct __common_reference_impl<_Tp1&, _Tp2&&, 1, void_t<__common_ref<_Tp1&, _Tp2&&>>> {
      using type = __common_ref<_Tp1&, _Tp2&&>;
    };

    template<class _Tp1, class _Tp2>
    struct __common_reference_impl<_Tp1&&, _Tp2&, 1, void_t<__common_ref<_Tp1&&, _Tp2&>>> {
      using type = __common_ref<_Tp1&&, _Tp2&>;
    };

    template<class _Tp1, class _Tp2>
    struct __common_reference_impl<_Tp1, _Tp2, 2, void_t<__basic_common_ref<_Tp1, _Tp2>>> {
      using type = __basic_common_ref<_Tp1, _Tp2>;
    };

    template<class _Tp1, class _Tp2>
    struct __common_reference_impl<_Tp1, _Tp2, 3, void_t<__cond_res<_Tp1, _Tp2>>> {
      using type = __cond_res<_Tp1, _Tp2>;
    };

    template<class _Tp1, class _Tp2>
    struct __common_reference_impl<_Tp1, _Tp2, 4, void_t<common_type_t<_Tp1, _Tp2>>> {
      using type = common_type_t<_Tp1, _Tp2>;
    };

    template<class _Tp1, class _Tp2>
    struct __common_reference_impl<_Tp1, _Tp2, 5, void> { };

  } // end of __detail:: namespace

  template<class _Tp1, class _Tp2, class... _Rr>
  struct common_reference<_Tp1, _Tp2, _Rr...>
    : public __detail::__common_type_fold<common_reference<_Tp1, _Tp2>,
                                          __detail::__common_type_pack<_Rr...>> { };
  
  namespace __detail { // continuation of private implementation namespace

    template<class _Tp1, class _Tp2, class... _Rr>
    struct __common_type_fold<common_reference<_Tp1, _Tp2>,
                              __common_type_pack<_Rr...>,
                              void_t<common_reference_t<_Tp1, _Tp2>>>
      : public common_reference<common_reference_t<_Tp1, _Tp2>, _Rr...> { };

  } // end of __detail:: namespace
  // End of std::common_reference


  // Start of std::is_swappable implementation:
  namespace __detail { // start of private implementation namespace

    template<class>
    struct __is_swappable;

    template<class>
    struct __is_nothrow_swappable;

    template<class>
    struct __is_tuple_like_impl : public false_type { };

    template<class _Tp>
    struct __is_tuple_like : public __is_tuple_like_impl<remove_cvref_t<_Tp>>::type { };

  } // end of __detail:: namespace

  template<class _Tp>
  constexpr inline _Require<__not_<__detail::__is_tuple_like<_Tp>>,
                            is_move_constructible<_Tp>,
                            is_move_assignable<_Tp>>
  swap(_Tp&, _Tp&) noexcept(__and_<is_nothrow_move_constructible<_Tp>,
                                   is_nothrow_move_assignable<_Tp>>::value);

  template<class _Tp, size_t _Nm>
  constexpr inline enable_if_t<__detail::__is_swappable<_Tp>::value>
  swap(_Tp (&__a)[_Nm], _Tp (&__b)[_Nm]) noexcept(__detail::__is_nothrow_swappable<_Tp>::value);

  namespace __detail { // start of private implementation namespace

    //using std::swap; // most likely wont use so that it doesnt carry this over throught the whole __detail namespace

    struct __do_is_swappable_impl {
      template<class _Tp, class = decltype(std::swap(declval<_Tp&>(), declval<_Tp&>()))>
      static true_type __test(int);

      template<class>
      static false_type __test(...);
    };

    struct __do_is_nothrow_swappable_impl {
      template<class _Tp>
      static bool_constant<noexcept(std::swap(declval<_Tp&>(), declval<_Tp&>()))> __test(int);

      template<class>
      static false_type __test(...);
    };

    template<class _Tp>
    struct __is_swappable_impl : public __do_is_swappable_impl {
      using type = decltype(__test<_Tp>(0));
    };

    template<class _Tp>
    struct __is_nothrow_swappable_impl : public __do_is_nothrow_swappable_impl {
      using type = decltype(__test<_Tp>(0));
    };

    template<class _Tp>
    struct __is_swappable : public __is_swappable_impl<_Tp>::type { };

    template<class _Tp>
    struct __is_nothrow_swappable : public __is_nothrow_swappable_impl<_Tp>::type { };

  } // end of __detail:: namespace

  template<class _Tp>
  struct is_swappable : public __detail::__is_swappable_impl<_Tp>::type {
    static_assert(__is_complete_or_unbounded(__type_identity<_Tp>{}),
      "std::is_swappable requires it's template argument to be a complete class or an unbounded array!");
  };

  template<class _Tp>
  struct is_nothrow_swappable : public __detail::__is_nothrow_swappable_impl<_Tp>::type {
    static_assert(__is_complete_or_unbounded(__type_identity<_Tp>{}),
      "std::is_nothrow_swappable requires it's template argument to be a complete class or an unbounded array!");
  };

  template<class _Tp>
  inline constexpr bool is_swappable_v = is_swappable<_Tp>::value;

  template<class _Tp>
  inline constexpr bool is_nothrow_swappable_v = is_nothrow_swappable<_Tp>::value;
  // End of std::is_swappable


  // Start of std::is_swappable_with implementation:
  namespace __detail { // start of private implementation namespace

    // using std::swap; // commented out for the same reason as the is_swappable implementation

    struct __do_is_swappable_with_impl {
      template<class _Tp, class _Up, class = decltype(std::swap(declval<_Tp>(), declval<_Up>())),
                                     class = decltype(std::swap(declval<_Up>(), declval<_Tp>()))>
      static true_type __test(int);

      template<class, class>
      static false_type __test(...);
    };

    struct __do_is_nothrow_swappable_with_impl {
      template<class _Tp, class _Up>
      static bool_constant<noexcept(std::swap(declval<_Tp>(), declval<_Up>())) &&
                           noexcept(std::swap(declval<_Up>(), declval<_Tp>()))
                          > __test(int);
      
      template<class, class>
      static false_type __test(...);
    };

    template<class _Tp, class _Up>
    struct __is_swappable_with_impl : public __do_is_swappable_with_impl {
      using type = decltype(__test<_Tp, _Up>(0));
    };

    template<class _Tp>
    struct __is_swappable_with_impl<_Tp&, _Tp&>
      : public __do_is_swappable_impl {
      using type = decltype(__test<_Tp&>(0));
    };

    template<class _Tp, class _Up>
    struct __is_nothrow_swappable_with_impl
      : public __do_is_nothrow_swappable_with_impl {
      using type = decltype(__test<_Tp, _Up>(0));
    };

    template<class _Tp>
    struct __is_nothrow_swappable_with_impl<_Tp&, _Tp&>
      : public __do_is_nothrow_swappable_impl {
      using type = decltype(__test<_Tp&>(0));
    };

  } // end of __detail:: namespace

  template<class _Tp, class _Up>
  struct is_swappable_with
    : public __detail::__is_swappable_with_impl<_Tp, _Up>::type {
    static_assert(__is_complete_or_unbounded(__type_identity<_Tp>{}) &&
                  __is_complete_or_unbounded(__type_identity<_Up>{}),
      "std::is_swappable_with requires both of it's template arguments to be a complete class or an unbounded array!");
  };

  template<class _Tp, class _Up>
  struct is_nothrow_swappable_with
    : public __detail::__is_nothrow_swappable_with_impl<_Tp, _Up>::type {
    static_assert(__is_complete_or_unbounded(__type_identity<_Tp>{}) &&
                  __is_complete_or_unbounded(__type_identity<_Up>{}),
      "std::is_nothrow_swappable_with requires both of it's template arguments to be a complete class or an unbounded array!");
  };

  template<class _Tp, class _Up>
  inline constexpr bool is_swappable_with_v = is_swappable_with<_Tp, _Up>::value;

  template<class _Tp, class _Up>
  inline constexpr bool is_nothrow_swappable_with_v = is_nothrow_swappable_with<_Tp, _Up>::value;
  // End of std::is_swappable_with


  // Start of std::result_of implementation:
  template<class _Signature>
  struct result_of; // forward declaration

  namespace __detail { // private implementation namespace

    struct __invoke_memfun_ref { };
    struct __invoke_memfun_deref { };
    struct __invoke_memobj_ref { };
    struct __invoke_memobj_deref { };
    struct __invoke_other { };

    template<class _Tp, class _Tag>
    struct __result_of_success : public __success_type<_Tp> {
      using __invoke_type = _Tag;
    };

    struct __result_of_memfun_ref_impl {
      template<class _Fp, class _Tp1, class... _Args>
      static __result_of_success<decltype((declval<_Tp1>().*declval<_Fp>())(declval<_Args>()...)),
      __invoke_memfun_ref> _S_test(int);

      template<class...>
      static __failure_type _S_test(...);
    };

    template<class _MemPtr, class _Arg, class... _Args>
    struct __result_of_memfun_ref : private __result_of_memfun_ref_impl {
      using type = decltype(_S_test<_MemPtr, _Arg, _Args...>(0));
    };

    struct __result_of_memfun_deref_impl {
      template<class _Fp, class _Tp1, class... _Args>
      static __result_of_success<decltype(((*declval<_Tp1>()).*declval<_Fp>())(declval<_Args>()...)),
      __invoke_memfun_deref> _S_test(int);

      template<class...>
      static __failure_type _S_test(...);
    };

    template<class _MemPtr, class _Arg, class... _Args>
    struct __result_of_memfun_deref : private __result_of_memfun_deref_impl {
      using type = decltype(_S_test<_MemPtr, _Arg, _Args...>(0));
    };

    struct __result_of_memobj_ref_impl {
      template<class _Fp, class _Tp1>
      static __result_of_success<decltype(declval<_Tp1>().*declval<_Fp>()),
      __invoke_memobj_ref> _S_test(int);

      template<class, class>
      static __failure_type _S_test(...);
    };

    template<class _MemPtr, class _Arg>
    struct __result_of_memobj_ref : private __result_of_memobj_ref_impl {
      using type = decltype(_S_test<_MemPtr, _Arg>(0));
    };

    struct __result_of_memobj_deref_impl {
      template<class _Fp, class _Tp1>
      static __result_of_success<decltype((*declval<_Tp1>()).*declval<_Fp>()),
      __invoke_memobj_deref> _S_test(int);

      template<class, class>
      static __failure_type _S_test(...);
    };

    template<class _MemPtr, class _Arg>
    struct __result_of_memobj_deref : private __result_of_memobj_deref_impl {
      using type = decltype(_S_test<_MemPtr, _Arg>(0));
    };

    template<class _MemPtr, class _Arg>
    struct __result_of_memobj;

    template<class _Res, class _Class, class _Arg>
    struct __result_of_memobj<_Res _Class::*, _Arg> {
      using _Argval = remove_cvref_t<_Arg>;
      using _MemPtr = _Res _Class::*;
      using type = typename conditional_t<__or_<is_same<_Argval, _Class>,
                                                is_base_of<_Class, _Argval>>::value,
                                          __result_of_memobj_ref<_MemPtr, _Arg>,
                                          __result_of_memobj_deref<_MemPtr, _Arg>
                                         >::type;
    };

    template<class _MemPtr, class _Arg, class... _Args>
    struct __result_of_memfun;

    template<class _Res, class _Class, class _Arg, class... _Args>
    struct __result_of_memfun<_Res _Class::*, _Arg, _Args...> {
      using _Argval = typename remove_reference<_Arg>::type;
      using _MemPtr = _Res _Class::*;
      using type = typename conditional_t<is_base_of<_Class, _Argval>::value,
                                          __result_of_memfun_ref<_MemPtr, _Arg, _Args...>,
                                          __result_of_memfun_deref<_MemPtr, _Arg, _Args...>
                                         >::type;
    };

    template<class _Tp, class _Up = remove_cvref_t<_Tp>>
    struct __inv_unwrap { using type = _Tp; };

    template<class _Tp, class _Up>
    struct __inv_unwrap<_Tp, reference_wrapper<_Up>> { using type = _Up&; };

    template<bool, bool, class _Functor, class... _ArgTypes>
    struct __result_of_impl { using type = __failure_type; };

    template<class _MemPtr, class _Arg>
    struct __result_of_impl<true, false, _MemPtr, _Arg>
      : public __result_of_memobj<decay_t<_MemPtr>,
                                  typename __inv_unwrap<_Arg>::type> { };

    template<class _MemPtr, class _Arg, class... _Args>
    struct __result_of_impl<false, true, _MemPtr, _Arg, _Args...>
      : public __result_of_memfun<decay_t<_MemPtr>,
                                  typename __inv_unwrap<_Arg>::type, _Args...> { };
    
    struct __result_of_other_impl {
      template<class _Fn, class... _Args>
      static __result_of_success<decltype(declval<_Fn>()(declval<_Args>()...)),
      __invoke_other> _S_test(int);

      template<class...>
      static __failure_type _S_test(...);
    };

    template<class _Functor, class... _ArgTypes>
    struct __result_of_impl<false, false, _Functor, _ArgTypes...>
      : private __result_of_other_impl {
      using type = decltype(_S_test<_Functor, _ArgTypes...>(0));
    };

    template<class _Functor, class... _ArgTypes>
    struct __invoke_result : public __result_of_impl<is_member_object_pointer<typename remove_reference<_Functor>::type>::value,
                                                     is_member_function_pointer<typename remove_reference<_Functor>::type>::value,
                                                     _Functor, _ArgTypes...>::type { };

    template<class _Fn, class... _Args>
    using __invoke_result_t = typename __invoke_result<_Fn, _Args...>::type;

  } // end of __detail:: namespace

  template<class _Functor, class... _ArgTypes>
  struct [[deprecated("std::result_of is deprecated. use std::invoke_result instead.")]] 
    result_of<_Functor(_ArgTypes...)> : public __detail::__invoke_result<_Functor, _ArgTypes...> { };
  // End of std::result_of


  // Start of std::is_invocable and related meta-functions implementation:
  namespace __detail { // private implementation namespace

    template<class _Result, class _Ret,
             bool = is_void<_Ret>::value, class = void>
    struct __is_invocable_impl : public false_type {
      using __nothrow_conv = false_type;
    };

    template<class _Result, class _Ret>
    struct __is_invocable_impl<_Result, _Ret, true, void_t<typename _Result::type>>
      : public true_type {
      using __nothrow_conv = true_type;
    };

#   pragma GCC diagnostic push
#   pragma GCC diagnostic ignored "-Wctor-dtor-privacy"
    template<class _Result, class _Ret>
    struct __is_invocable_impl<_Result, _Ret, false, void_t<typename _Result::type>> {
      private:
        using _Res_t = typename _Result::type;
        
        static _Res_t _S_get() noexcept;

        template<class _Tp>
        static void _S_conv(__type_identity_t<_Tp>) noexcept;

        template<class _Tp, bool _Nothrow = noexcept(_S_conv<_Tp>(_S_get())),
                            class = decltype(_S_conv<_Tp>(_S_get())),
                            bool _Dangle = false> // i most likely will just get rid of _Dangle since arduino IDE doesn't have __reference_converts_from_temporary intrinsic. I will try to find a way to implement this intrinsic as a meta-function.
        static bool_constant<_Nothrow && !_Dangle> _S_test(int);

        template<class _Tp, bool = false>
        static false_type _S_test(...);
      
      public:
        using type = decltype(_S_test<_Ret, true>(1));

        using __nothrow_conv = decltype(_S_test<_Ret>(1));
    };
#   pragma GCC diagnostic pop 

    template<class _Fn, class... _ArgTypes>
    struct __is_invocable
      : public __is_invocable_impl<__invoke_result<_Fn, _ArgTypes...>, void>::type { };

    template<class _Fn, class _Tp, class... _Args>
    constexpr bool __call_is_nt(__invoke_memfun_ref) {
      using _Up = typename __inv_unwrap<_Tp>::type;
      return noexcept((declval<_Up>().*declval<_Fn>())(declval<_Args>()...));
    }

    template<class _Fn, class _Tp, class... _Args>
    constexpr bool __call_is_nt(__invoke_memfun_deref) {
      return noexcept(((*declval<_Tp>()).*declval<_Fn>())(declval<_Args>()...));
    }

    template<class _Fn, class _Tp>
    constexpr bool __call_is_nt(__invoke_memobj_ref) {
      using _Up = typename __inv_unwrap<_Tp>::type;
      return noexcept(declval<_Up>().*declval<_Fn>());
    }

    template<class _Fn, class _Tp>
    constexpr bool __call_is_nt(__invoke_memobj_deref) {
      return noexcept((*declval<_Tp>()).*declval<_Fn>());
    }

    template<class _Fn, class... _Args>
    constexpr bool __call_is_nt(__invoke_other) {
      return noexcept(declval<_Fn>()(declval<_Args>()...));
    }

    template<class _Result, class _Fn, class... _Args>
    struct __call_is_nothrow 
      : public bool_constant<__call_is_nt<_Fn, _Args...>(typename _Result::__invoke_type{})> { };
    
    template<class _Fn, class... _Args>
    using __call_is_nothrow_t = __call_is_nothrow<__invoke_result<_Fn, _Args...>, _Fn, _Args...>;

    template<class _Fn, class... _Args>
    struct __is_nothrow_invocable : public bool_constant<
                                           __and_<__is_invocable<_Fn, _Args...>,
                                                  __call_is_nothrow_t<_Fn, _Args...>
                                                 >::value> { };
    
#   pragma GCC diagnostic push
#   pragma GCC diagnostic ignored "-Wctor-dtor-privacy"
    struct __nonesuchbase { };
    struct __nonesuch : private __nonesuchbase {
      ~__nonesuch() = delete;
      __nonesuch(__nonesuch const&) = delete;
      void operator=(__nonesuch const&) = delete;
    };
#   pragma GCC diagnostic pop

  } // end of __detail:: namespace

  template<class _Functor, class... _ArgTypes>
  struct invoke_result : public __detail::__invoke_result<_Functor, _ArgTypes...> {
    static_assert(__is_complete_or_unbounded(__type_identity<_Functor>{}),
      "std::invoke_result requires the functor type _Functor to be a complete class or an unbounded array!");
    
    static_assert((__is_complete_or_unbounded(__type_identity<_ArgTypes>{}) && ...),
      "std::invoke_result requires each argument type to be a complete class or an unbounded array!");
  };

  template<class _Fn, class... _Args>
  using invoke_result_t = typename invoke_result<_Fn, _Args...>::type;

  template<class _Fn, class... _ArgTypes>
  struct is_invocable
    : public __detail::__is_invocable_impl<__detail::__invoke_result<_Fn, _ArgTypes...>, void>::type {
    static_assert(__is_complete_or_unbounded(__type_identity<_Fn>{}),
      "std::is_invocable requires the functor type _Fn to be a complete class or an unbounded array!");

    static_assert((__is_complete_or_unbounded(__type_identity<_ArgTypes>{}) && ...),
      "std::is_invocable requires that each argument type is a complete class or an unbounded array!");
  };

  template<class _Ret, class _Fn, class... _ArgTypes>
  struct is_invocable_r
    : public __detail::__is_invocable_impl<__detail::__invoke_result<_Fn, _ArgTypes...>, _Ret>::type {
    static_assert(__is_complete_or_unbounded(__type_identity<_Fn>{}),
      "std::is_invocable_r requires the functor type _Fn to be a complete class or an unbounded array!");

    static_assert((__is_complete_or_unbounded(__type_identity<_ArgTypes>{}) && ...),
      "std::is_invocable_r requires that each argument type is a complete class or an unbounded array!");
    
    static_assert(__is_complete_or_unbounded(__type_identity<_Ret>{}),
      "std::is_invocable_r requires the type _Ret to be a complete class or an unbounded array!");
  };

  template<class _Fn, class... _ArgTypes>
  struct is_nothrow_invocable : public bool_constant<
                                       __and_<__detail::__is_invocable_impl<__detail::__invoke_result<_Fn, _ArgTypes...>, void>,
                                              __detail::__call_is_nothrow_t<_Fn, _ArgTypes...>
                                             >::value> {
    static_assert(__is_complete_or_unbounded(__type_identity<_Fn>{}),
      "std::is_nothrow_invocable requires the functor type _Fn to be a complete class or an unbounded array!");

    static_assert((__is_complete_or_unbounded(__type_identity<_ArgTypes>{}) && ...),
      "std::is_nothrow_invocable requires each of the argument types to be a complete class or an unbounded array!");
  };

  namespace __detail { // private namespace for __is_nt_invocable_impl

    template<class _Result, class _Ret>
    using __is_nt_invocable_impl = typename __is_invocable_impl<_Result, _Ret>::__nothrow_conv;

  } // end of __detail:: namespace

  template<class _Ret, class _Fn, class... _ArgTypes>
  struct is_nothrow_invocable_r : public bool_constant<
                                         __and_<__detail::__is_nt_invocable_impl<__detail::__invoke_result<_Fn, _ArgTypes...>, _Ret>,
                                                __detail::__call_is_nothrow_t<_Fn, _ArgTypes...>
                                               >::value> {
    static_assert(__is_complete_or_unbounded(__type_identity<_Fn>{}),
      "std::is_nothrow_invocable_r requires that the functor type _Fn is a complete class or an unbounded array!");

    static_assert((__is_complete_or_unbounded(__type_identity<_ArgTypes>{}) && ...),
      "std::is_nothrow_invocable_r requires that each of the argument types are a complete class or an unbounded array!");

    static_assert(__is_complete_or_unbounded(__type_identity<_Ret>{}),
      "std::is_nothrow_invocable_r requires the type _Ret to be a complete class or an unbounded array!");
  };
  // End of std::is_invocable and related meta-functions

  // Start of _GLIBCXX_HAS_NESTED_TYPE macro function implementation
# define _GLIBCXX_HAS_NESTED_TYPE(_NTYPE)                                     \
  template<class _Tp>                                                         \
  class __has_##_NTYPE##_helper {                                             \
    template<class _Up>                                                       \
    struct _Wrap_type { };                                                    \
    template<class _Up>                                                       \
    static true_type __test(_Wrap_type<typename _Up::_NTYPE>*);               \
    template<class _Up>                                                       \
    static false_type __test(...);                                            \
    public:                                                                   \
      typedef decltype(__test<_Tp>(0)) type;                                  \
  };                                                                          \
  template<class _Tp>                                                         \
  struct __has_##_NTYPE                                                       \
  : public __has_##_NTYPE##_helper<typename remove_cv<_Tp>::type>::type { };  
  // End of _GLIBCXX_HAS_NESTED_TYPE

} // End of std namespace


// Start of implementations from bits/cpp_type_traits

namespace __gnu_cxx _DLIB_VISIBILITY {

  template<class _Iterator, class _Container>
  class __normal_iterator;

}


namespace std _DLIB_VISIBILITY {

  namespace __detail { // namespace for internal implementation

    // pseudo versions of std::true_type and std::false_type
    struct __true_type { };
    struct __false_type { };

    // __std::__detail::__truth_type works like enable_if but for the pseudo true and false types
    template<bool>
    struct __truth_type { typedef __false_type __type; };

    template<>
    struct __truth_type<true> { typedef __true_type __type; };


    template<class _Sp, class _Tp>
    struct __traitor {
      enum { __value = bool(_Sp::__value) || bool(_Tp::__value) };
      typedef typename __truth_type<__value>::__type __type;
    };


    // Start of std::__detail::__are_same implementation:
    template<class, class>
    struct __are_same {
      enum { __value = 0 };
      typedef __false_type __type;
    };

    template<class _Tp>
    struct __are_same<_Tp, _Tp> {
      enum { __value = 1 };
      typedef __true_type __type;
    };
    // End of std::__detail::__are_same


    // Start of std::__detail::__is_void implementation:
    template<class _Tp>
    struct __is_void {
      enum { __value = 0 };
      typedef __false_type __type;
    };

    template<>
    struct __is_void<void> {
      enum { __value = 1 };
      typedef __true_type __type;
    };
    // End of std::__detail::__is_void


    // Start of std::__detail::__is_integer implementation:
    template<class _Tp>
    struct __is_integer {
      enum { __value = 0 };
      typedef __false_type __type;
    };

    template<>
    struct __is_integer<bool> {
      enum { __value = 1 };
      typedef __true_type __type;
    };

    template<>
    struct __is_integer<char> {
      enum { __value = 1 };
      typedef __true_type __type;
    };

    template<>
    struct __is_integer<signed char> {
      enum { __value = 1 };
      typedef __true_type __type;
    };

    template<>
    struct __is_integer<unsigned char> {
      enum { __value = 1 };
      typedef __true_type __type;
    };

    template<>
    struct __is_integer<wchar_t> {
      enum { __value = 1 };
      typedef __true_type __type;
    };

    template<>
    struct __is_integer<char16_t> {
      enum { __value = 1 };
      typedef __true_type __type;
    };

    template<>
    struct __is_integer<char32_t> {
      enum { __value = 1 };
      typedef __true_type __type;
    };

    template<>
    struct __is_integer<short> {
      enum { __value = 1 };
      typedef __true_type __type;
    };

    template<>
    struct __is_integer<unsigned short> {
      enum { __value = 1 };
      typedef __true_type __type;
    };

    template<>
    struct __is_integer<int> {
      enum { __value = 1 };
      typedef __true_type __type;
    };

    template<>
    struct __is_integer<unsigned int> {
      enum { __value = 1 };
      typedef __true_type __type;
    };

    template<>
    struct __is_integer<long> {
      enum { __value = 1 };
      typedef __true_type __type;
    };

    template<>
    struct __is_integer<unsigned long> {
      enum { __value = 1 };
      typedef __true_type __type;
    };

    template<>
    struct __is_integer<long long> {
      enum { __value = 1 };
      typedef __true_type __type;
    };

    template<>
    struct __is_integer<unsigned long long> {
      enum { __value = 1 };
      typedef __true_type __type;
    };
    // End of std::__detail::__is_integer


    // Start of std::__detail::__is_floating implementation:
    template<class _Tp>
    struct __is_floating {
      enum { __value = 0 };
      typedef __false_type __type;
    };

    template<>
    struct __is_floating<float> {
      enum { __value = 1 };
      typedef __true_type __type;
    };

    template<>
    struct __is_floating<double> {
      enum { __value = 1 };
      typedef __true_type __type;
    };

    template<>
    struct __is_floating<long double> {
      enum { __value = 1 };
      typedef __true_type __type;
    };
    // End of std::__detail::__is_floating implementation


    // Start of std::__detail::__is_pointer implementation:
    template<class _Tp>
    struct __is_pointer {
      enum { __value = 0 };
      typedef __false_type __type;
    };

    template<class _Tp>
    struct __is_pointer<_Tp*> {
      enum { __value = 1 };
      typedef __true_type __type;
    };
    // End of std::__detail::__is_pointer


    // Start of std::__detail::__is_normal_iterator implementation:
    template<class _Tp>
    struct __is_normal_iterator {
      enum { __value = 0 };
      typedef __false_type __type;
    };

    template<class _Iterator, class _Container>
    struct __is_normal_iterator<__gnu_cxx::__normal_iterator<_Iterator, _Container>> {
      enum { __value = 1 };
      typedef __true_type __type;
    };
    // End of std::__detail::__is_normal_iterator


    // Start of std::__detail::__is_arithmetic implementation:
    template<class _Tp>
    struct __is_arithmetic
      : public __traitor<__is_integer<_Tp>, __is_floating<_Tp>> { };
    // End of std::__detail::__is_arithmetic


    // Start of std::__detail::__is_scalar implementation:
    template<class _Tp>
    struct __is_scalar
      : public __traitor<__is_arithmetic<_Tp>, __is_pointer<_Tp>> { };
    // End of std::__detail::__is_scalar


    // Start of std::__detail::__is_char implementation:
    template<class _Tp>
    struct __is_char {
      enum { __value = 0 };
      typedef __false_type __type;
    };

    template<>
    struct __is_char<char> {
      enum { __value = 1 };
      typedef __true_type __type;
    };

    template<>
    struct __is_char<wchar_t> {
      enum { __value = 1 };
      typedef __true_type __type;
    };

#   if(TEST_CODE)
    template<>
    struct __is_char<char16_t> {
      enum { __value = 1 };
      typedef __true_type __type;
    };

    template<>
    struct __is_char<char32_t> {
      enum { __value = 1 };
      typedef __true_type __type;
    };
#   endif
    // End of std::__detail::__is_char


    // Start of std::__detail::__is_byte implementation:
    template<class _Tp>
    struct __is_byte {
      enum { __value = 0 };
      typedef __false_type __type;
    };

    template<>
    struct __is_byte<char> {
      enum { __value = 1 };
      typedef __true_type __type;
    };

    template<>
    struct __is_byte<signed char> {
      enum { __value = 1 };
      typedef __true_type __type;
    };

    template<>
    struct __is_byte<unsigned char> {
      enum { __value = 1 };
      typedef __true_type __type;
    };
    // End of std::__detail::__is_byte


    // Start of std::__detail::__is_move_iterator implementation:
    template<class _Tp>
    struct __is_move_iterator {
      enum { __value = 0 };
      typedef __false_type __type;
    };

    template<class _Iterator>
    class move_iterator; // forward declaration

    template<class _Iterator>
    struct __is_move_iterator<move_iterator<_Iterator>> {
      enum { __value = 1 };
      typedef __true_type __type;
    };
    // End of std::__detail::__is_move_iterator

  } // end of __detail:: namespace

} // end of std:: namespace

// End of implementations from bits/cpp_type_traits implementations


// Start of implementations from ext/type_traits.h header from libstdc++:

namespace __gnu_cxx _DLIB_VISIBILITY { // start of gnu extension namespace

  template<bool, class>
  struct __enable_if { };

  template<class _Tp>
  struct __enable_if<true, _Tp> { typedef _Tp __type; };

  template<bool _Cond, class _Iftrue, class _Iffalse>
  struct __conditional_type { typedef _Iftrue __type; };

  template<class _Iftrue, class _Iffalse>
  struct __conditional_type<false, _Iftrue, _Iffalse> { typedef _Iffalse __type; };

  template<class _Tp>
  struct __add_unsigned {
    private:
      typedef __enable_if<std::__detail::__is_integer<_Tp>::__value, _Tp> __if_type;
    public:
      typedef typename __if_type::__type __type;
  };

  template<>
  struct __add_unsigned<char> { typedef unsigned char __type; };

  template<>
  struct __add_unsigned<signed char> { typedef unsigned char __type; };

  template<>
  struct __add_unsigned<short> { typedef unsigned short __type; };

  template<>
  struct __add_unsigned<int> { typedef unsigned int __type; };

  template<>
  struct __add_unsigned<long> { typedef unsigned long __type; };

  template<>
  struct __add_unsigned<long long> { typedef unsigned long long __type; };

  template<>
  struct __add_unsigned<bool>;

  template<>
  struct __add_unsigned<wchar_t>;

# if(TEST_CODE)
  template<>
  struct __add_unsigned<char16_t>;

  template<>
  struct __add_unsigned<char32_t>;
# endif

  template<class _Tp>
  struct __remove_unsigned {
    private:
      typedef __enable_if<std::__detail::__is_integer<_Tp>::__value, _Tp> __if_type;
    public:
      typedef typename __if_type::__type __type;
  };

  template<>
  struct __remove_unsigned<char> { typedef signed char __type; };

  template<>
  struct __remove_unsigned<unsigned char> { typedef signed char __type; };

  template<>
  struct __remove_unsigned<unsigned short> { typedef short __type; };

  template<>
  struct __remove_unsigned<unsigned int> { typedef int __type; };

  template<>
  struct __remove_unsigned<unsigned long> { typedef long __type; };

  template<>
  struct __remove_unsigned<unsigned long long> { typedef long long __type; };

  template<>
  struct __remove_unsigned<bool>;

  template<>
  struct __remove_unsigned<wchar_t>;

# if(TEST_CODE)
  template<>
  struct __remove_unsigned<char16_t>;

  template<>
  struct __remove_unsigned<char32_t>;
# endif

  template<class _Type>
  inline bool __is_null_pointer(_Type* __ptr) { return __ptr == 0; }

  template<class _Type>
  inline bool __is_null_pointer(_Type) { return false; }

  inline bool __is_null_pointer(nullptr_t) { return true; }

  template<class _Tp, bool = std::__detail::__is_integer<_Tp>::__value>
  struct __promote { typedef double __type; };

  template<class _Tp>
  struct __promote<_Tp, false> { }; // undefined for non-integer and non-floating point types

  template<>
  struct __promote<long double> { typedef long double __type; };

  template<>
  struct __promote<double> { typedef double __type; };

  template<>
  struct __promote<float> { typedef float __type; };

  template<class _Tp, class _Up,
           class _Tp2 = typename __promote<_Tp>::__type,
           class _Up2 = typename __promote<_Up>::__type>
  struct __promote_2 { 
    typedef __typeof__(_Tp2() + _Up2()) __type;
  };

  template<class _Tp, class _Up, class _Vp,
           class _Tp2 = typename __promote<_Tp>::__type,
           class _Up2 = typename __promote<_Up>::__type,
           class _Vp2 = typename __promote<_Vp>::__type>
  struct __promote_3 {
    typedef __typeof__(_Tp2() + _Up2() + _Vp2()) __type;
  };

  template<class _Tp, class _Up, class _Vp, class _Wp,
           class _Tp2 = typename __promote<_Tp>::__type,
           class _Up2 = typename __promote<_Up>::__type,
           class _Vp2 = typename __promote<_Vp>::__type,
           class _Wp2 = typename __promote<_Wp>::__type>
  struct __promote_4 {
    typedef __typeof__(_Tp2() + _Up2() + _Vp2() + _Wp2()) __type;
  };

} // end of __gnu_cxx:: namespace

// End of implementations from ext/type_traits.h header from libstdc++


// Start of libstdc++ typeinfo header implementation: (seperate from the implementations above since it uses another namespace along with extern "C++")

namespace std { // namespace for bits/hash_bytes.h implementation:

  namespace __detail { // hash_bytes implementations will be kept internal

    size_t _Hash_bytes(const void* __ptr, size_t __len, size_t __seed);

    size_t _Fnv_hash_bytes(const void* __ptr, size_t __len, size_t __seed);

  } // end of __detail:: namespace

} // end of std:: namespace for bits/hash_bytes.h


#pragma GCC visibility push(default)


extern "C++" {
  namespace __cxxabiv1 {
    class __class_type_info;
  }

  // I can't find any other definitions of __GXX_MERGED_TYPEINFO_NAMES, so I'm not sure in which case this macro should be true(1)
# ifndef __MERGED_TYPEINFO_NAMES
#   if !__GXX_WEAK__
#     define __MERGED_TYPEINFO_NAMES 0
#   else
#     define __MERGED_TYPEINFO_NAMES 1
#   endif
# endif

# ifndef __TYPEINFO_EQUALITY_INLINE
#   if !__GXX_WEAK__
#     define __TYPEINFO_EQUALITY_INLINE 0
#   else
#     define __TYPEINFO_EQUALITY_INLINE 1
#   endif
# endif

  namespace std { // start of std::type_info implementations along with related implementations.

    class type_info {
      public:
        virtual ~type_info();

        const char* name() const noexcept {
          return __name[0] == '*' ? __name + 1 : __name;
        }

#     if !__TYPEINFO_EQUALITY_INLINE
        inline bool before(const type_info& __arg) const noexcept;

        GLIBCXX_CONSTEXPR inline bool operator==(const type_info& __arg) const noexcept;
#     else
#       if !__MERGED_TYPEINFO_NAMES
        inline bool before(const type_info& __arg) const noexcept {
          return (__name[0] == '*' && __arg.__name[0] == '*')
                  ? __name < __arg.__name
                  : __builtin_strcmp(__name, __arg.__name) < 0;
        }

        GLIBCXX_CONSTEXPR inline bool operator==(const type_info& __arg) const noexcept {
          return ((__name == __arg.__name)
                  || (__name[0] != '*' && __builtin_strcmp(__name, __arg.__name) == 0));
        }
#       else
        inline bool before(const type_info& __arg) const noexcept { 
          return __name < __arg.__name; 
        }

        GLIBCXX_CONSTEXPR inline bool operator==(const type_info& __arg) const noexcept { 
          return __name == __arg.__name; 
        }
#       endif
#     endif

        GLIBCXX_CONSTEXPR inline bool operator!=(const type_info& __arg) const noexcept {
          return !operator==(__arg);
        }

        size_t hash_code() const noexcept {
#         if !__MERGED_TYPEINFO_NAMES
          return __detail::_Hash_bytes(name(), __builtin_strlen(name()),
                                       static_cast<size_t>(0xc70f6907UL));
#         else
          return reinterpret_cast<size_t>(__name);
#         endif
        }

        virtual bool __is_pointer_p() const;

        virtual bool __is_function_p() const;

        virtual bool __do_catch(const type_info* __thr_type, void** __thr_obj, unsigned __outer) const;

        virtual bool __do_upcast(const __cxxabiv1::__class_type_info* __target, void** __obj_ptr) const;

      protected:
        const char* __name;

        explicit type_info(const char* __n) : __name(__n) { }
      
      private:
        type_info& operator=(const type_info&) = delete;
        type_info(const type_info&) = delete;
    }; // end of std::type_info


    // class for incorrect typecasting exceptions
    class bad_cast : public std::exception {
      public:
        bad_cast() noexcept { }

        virtual ~bad_cast() noexcept;

        virtual const char* what() const noexcept;
    };


    // class for nullptr typeid expression exception
    class bad_typeid : public std::exception {
      public:
        bad_typeid() noexcept { }

        virtual ~bad_typeid() noexcept;

        virtual const char* what() const noexcept;
    };

  } // end of std:: namespace

} // end of extern "C++"

#pragma GCC visibility pop

// End of libstdc++ typeinfo header implementation


// Start of libstdc++ numeric_traits header implementation(c++14):

namespace __gnu_cxx _DLIB_VISIBILITY { // start of internal GNU extension namespace

  // I might make these macros multi-line to help with readability
  // These macros are temporary helper macros just for the numeric traits implementation, so they cannot be used in other files.
# define __glibcxx_signed(_Tp) ((_Tp)(-1) < 0)
# define __glibcxx_digits(_Tp) (sizeof(_Tp) * __CHAR_BIT__ - __glibcxx_signed(_Tp))
# define __glibcxx_min(_Tp) (__glibcxx_signed(_Tp) ? (_Tp)1 << __glibcxx_digits(_Tp) : (_Tp)0)
# define __glibcxx_max(_Tp) (__glibcxx_signed(_Tp) ? (((((_Tp)1 << (__glibcxx_digits(_Tp) - 1)) - 1) << 1) + 1) : ~(_Tp)0)

  // no __detail:: namespace since this isn't a namespace used by the user
  template<class _Value>
  struct __numeric_traits_integer {
    static const _Value __min = __glibcxx_min(_Value);
    static const _Value __max = __glibcxx_max(_Value);

    static const bool __is_signed = __glibcxx_signed(_Value);
    static const int __digits = __glibcxx_digits(_Value);
  };

  template<class _Value>
  const _Value __numeric_traits_integer<_Value>::__min;

  template<class _Value>
  const _Value __numeric_traits_integer<_Value>::__max;

  template<class _Value>
  const bool __numeric_traits_integer<_Value>::__is_signed;

  template<class _Value>
  const int __numeric_traits_integer<_Value>::__digits;

# undef __glibcxx_signed
# undef __glibcxx_digits
# undef __glibcxx_min
# undef __glibcxx_max

# define __glibcxx_floating(_Tp, _Fval, _Dval, _LDval) \
    (std::__detail::__are_same<_Tp, float>::__value ? _Fval \
    : std::__detail::__are_same<_Tp, double>::__value ? _Dval : _LDval)

# define __glibcxx_max_digits10(_Tp) \
    (2 + __glibcxx_floating(_Tp, __FLT_MANT_DIG__, __DBL_MANT_DIG__, __LDBL_MANT_DIG__) * 643L / 2136)

# define __glibcxx_digits10(_Tp) __glibcxx_floating(_Tp, __FLT_DIG__, __DBL_DIG__, __LDBL_DIG__)

# define __glibcxx_max_exponent10(_Tp) \
    __glibcxx_floating(_Tp, __FLT_MAX_10_EXP__, __DBL_MAX_10_EXP__, __LDBL_MAX_10_EXP__)

  template<class _Value>
  struct __numeric_traits_floating {
    static const int __max_digits10 = __glibcxx_max_digits10(_Value);
    static const bool __is_signed = true;
    static const int __digits10 = __glibcxx_digits10(_Value);
    static const int __max_exponent10 = __glibcxx_max_exponent10(_Value);
  };

  template<class _Value>
  const int __numeric_traits_floating<_Value>::__max_digits10;

  template<class _Value>
  const bool __numeric_traits_floating<_Value>::__is_signed;

  template<class _Value>
  const int __numeric_traits_floating<_Value>::__digits10;

  template<class _Value>
  const int __numeric_traits_floating<_Value>::__max_exponent10;

  template<class _Value>
  struct __numeric_traits
    : public __conditional_type<std::__detail::__is_integer<_Value>::__value,
                                __numeric_traits_integer<_Value>,
                                __numeric_traits_floating<_Value>>::__type { };

# undef __glibcxx_floating
# undef __glibcxx_max_digits10
# undef __glibcxx_digits10
# undef __glibcxx_max_exponent10

} // end of __gnu_cxx:: namespace

// End of libstdc++ numeric_traits header implementation


// Start of libstdc++ bits/ptr_traits header implementation

namespace std _DLIB_VISIBILITY { // continuing std namespace

  namespace __detail { // namespace for internal implementations

    _GLIBCXX_HAS_NESTED_TYPE(element_type)
    _GLIBCXX_HAS_NESTED_TYPE(difference_type)

    template<class _Tp, bool = __has_element_type<_Tp>::value>
    struct __ptrtr_elt_type;

    template<class _Tp>
    struct __ptrtr_elt_type<_Tp, true> {
      typedef typename _Tp::element_type __type;
    };

    template<template<class, class...> class _SomePtr, class _Tp, class... _Args>
    struct __ptrtr_elt_type<_SomePtr<_Tp, _Args...>, false> {
      typedef _Tp __type;
    };

    template<class _Tp, bool = __has_difference_type<_Tp>::value>
    struct __ptrtr_diff_type {
      typedef typename _Tp::difference_type __type;
    };

    template<class _Tp>
    struct __ptrtr_diff_type<_Tp, false> {
      typedef ptrdiff_t __type;
    };

    template<class _Ptr, class _Up>
    class __ptrtr_rebind_helper {
      template<class _Ptr2, class _Up2>
      static constexpr true_type _S_chk(typename _Ptr2::template rebind<_Up2>*);

      template<class, class>
      static constexpr false_type _S_chk(...);

      public:
        using __type = decltype(_S_chk<_Ptr, _Up>(nullptr));
    };

    template<class _Tp, class _Up, bool = __ptrtr_rebind_helper<_Tp, _Up>::__type::value>
    struct __ptrtr_rebind;

    template<class _Tp, class _Up>
    struct __ptrtr_rebind<_Tp, _Up, true> {
      typedef typename _Tp::template rebind<_Up> __type;
    };

    template<template<class, class...> class _SomePtr, class _Up, class _Tp, class... _Args>
    struct __ptrtr_rebind<_SomePtr<_Tp, _Args...>, _Up, false> {
      typedef _SomePtr<_Up, _Args...> __type;
    };

    template<class _Tp, class = typename remove_cv<_Tp>::type>
    struct __ptrtr_not_void {
      typedef _Tp __type;
    };

    template<class _Tp>
    struct __ptrtr_not_void<_Tp, void> {
      struct __type { };
    };

    template<class _Ptr>
    class __ptrtr_pointer_to {
      typedef typename __ptrtr_elt_type<_Ptr>::__type __orig_type;
      typedef typename __ptrtr_not_void<__orig_type>::__type __element_type;

      public:
        static _Ptr pointer_to(__element_type& __e) {
          return _Ptr::pointer_to(__e);
        }
    };

  } // end of __detail:: namespace

  template<class _Ptr>
  struct pointer_traits : public __detail::__ptrtr_pointer_to<_Ptr> {
    typedef _Ptr pointer;
    typedef typename __detail::__ptrtr_elt_type<_Ptr>::__type element_type;
    typedef typename __detail::__ptrtr_diff_type<_Ptr>::__type difference_type;

    template<class _Up>
    using rebind = typename __detail::__ptrtr_rebind<_Ptr, _Up>::__type;
  };

  template<class _Tp>
  struct pointer_traits<_Tp*> {
    typedef _Tp* pointer;
    typedef _Tp element_type;
    typedef ptrdiff_t difference_type;

    template<class _Up>
    using rebind = _Up*;

    static pointer pointer_to(typename __detail::__ptrtr_not_void<element_type>::__type& __r) noexcept {
      return addressof(__r);
    }
  };

} // end of std:: namespace

// End of libstdc++ bits/ptr_traits header implementation


// Start of libstdc++ bits/memoryfwd header implementation:

namespace std _DLIB_VISIBILITY {

  template<class>
  class allocator;

  template<>
  class allocator<void>;

  template<class, class>
  struct uses_allocator;

} // end of std:: namespace

// End of libstdc++ bits/memoryfwd header implementation


#endif // end of Type_Traits.hpp
