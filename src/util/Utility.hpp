// Libstdc++ and GCC GNU Lincese boilerplate:      (this is required since this code is copyrighted under the GCC compiler. I did not design this myself.)
/*
  Partial Modified Implementation Of Meta-Functions from the <utility> Libstdc++ header  -*- C++ -*-

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
  Utility.hpp [V1.0.0] (template class header/declaration file)
    By: Andrey Stroitelev

  ~This file adds a bunch of utility functions from the std namespace(libstdc++) that arent available for AVR boards on the arduino IDE
  ~This file was mainly added out of necessity for the implementations of the data structure classes in this library.
  ~This file also adds the std::move() function which allows you to do move semantics in the arduino IDE which are also not normally supported in the arduino IDE
    *You can use r-value references(&&), but it ends up being no different than just using regular references(&) if you don't use std::move()
  ~Some meta-functions that should be in Type_Traits.hpp are in this file instead such as is_enum, is_union, is_class, and is_base_of along with a bunch of other metafunctions.
   I have done this to try to lower the amount of code in type traits by dividing up the functions, and there are some functions in this file that make more sense here but they need a function from Type_Traits.hpp to functions.
    *Since I want this file(Utility.hpp) and Type_Traits.hpp to work properly when used on their own without the other file preset. 
  ~The meta-functions in this file are mainly functions that help with making other meta-functions, and they just help with using templates in general.

  ~I also added a simple function called freeMemory() which calculates the amount of free memory the arduino has left.
    *This function just calculates the distance between the end of the stack and the end of the heap, so it may not detect dynamic memory that was deallocated.
*/


// Start of Utility.hpp
#ifndef _DLIB_UTILITY_HPP_ 
#define _DLIB_UTILITY_HPP_ // adding DLIB to the start of the macro alias to avoid possible conflict since UTILITY_HPP seems common

#ifndef _DLIB_UTILITY_VERSION_
#define _DLIB_UTILITY_VERSION_ 0x010000
#endif


#include <Arduino.h> // including all of the std c++ stuff from the arduino core


// Start of std:: namespace:
namespace std {

  template<class...>
  using void_t = void; // simple utility for templates

  
  // Start of __type_identity implementation: (used internally)
  template<class _Type>
  struct __type_identity { using type = _Type; };

  template<class _Tp>
  using __type_identity_t = typename __type_identity<_Tp>::type;
  // End of __type_identity implementation


  // Start of integral_constant implementation:
  template<class T, T val>
  struct integral_constant {
    static constexpr T value = val;

    using value_type = T;
    using type = integral_constant;

    constexpr operator value_type() const noexcept { return value; }
    constexpr value_type operator()() const noexcept { return value; }
  };

  template<bool B>
  using bool_constant = integral_constant<bool, B>; // often used specialization for boolean constant

  using true_type = bool_constant<true>;
  using false_type = bool_constant<false>;
  // End of integral_constant implementation


  // Start of conditional implementation:
  template<bool _B, class _Tp, class _F>
  struct conditional { using type = _Tp; };

  template<class _Tp, class _F>
  struct conditional<false, _Tp, _F> { using type = _F; };

  template<bool _B, class _Tp, class _F>
  using conditional_t = typename conditional<_B, _Tp, _F>::type;
  // End of conditional implementation


  // Start of helper conditional meta-function implementations:
  template<class...>
  struct __or_;

  template<>
  struct __or_<> : public false_type { };

  template<class _B1>
  struct __or_<_B1> : public _B1 { };

  template<class _B1, class _B2>
  struct __or_<_B1, _B2> : public conditional<_B1::value, _B1, _B2>::type { };

  template<class _B1, class _B2, class _B3, class... _Bn>
  struct __or_<_B1, _B2, _B3, _Bn...> : public conditional<_B1::value, _B1, __or_<_B2, _B3, _Bn...>>::type { };

  template<class...>
  struct __and_ { };

  template<>
  struct __and_<> : public true_type { };

  template<class _B1>
  struct __and_<_B1> : public _B1 { };

  template<class _B1, class _B2>
  struct __and_<_B1, _B2> : public conditional<_B1::value, _B2, _B1>::type { };

  template<class _B1, class _B2, class _B3, class... _Bn>
  struct __and_<_B1, _B2, _B3, _Bn...> : public conditional<_B1::value, __and_<_B2, _B3, _Bn...>, _B1>::type { };

  template<class _Pp>
  struct __not_ : public bool_constant<!_Pp::value> { };
  // End of helper conditional meta-function implementations


  // Start of __sfinae_types internal utility implementation:
  struct __sfinae_types {
    typedef char __one;
    typedef struct { char __arr[2]; } __two;
  };
  // End of __sfinae_types implementation


  // Start of is_same implementation:
  template<class _Tp, class _Up>
  struct is_same : public false_type { };

  template<class _Tp>
  struct is_same<_Tp, _Tp> : public true_type { };

  template<class _Tp, class _Up>
  inline constexpr bool is_same_v = is_same<_Tp, _Up>::value;
  // End of is_same implementation


  // Start of same_as implementation:
  // implementing as a meta-function since there are no concepts in c++11
  template<class _Tp, class _Up>
  inline constexpr bool same_as = (is_same_v<_Tp, _Up> && is_same_v<_Up, _Tp>);
  // End of same_as implementation


  // Start of is_void pre-declaration:
  template<class>
  struct is_void;
  // End of is_void pre-declaration


  // Start of is_function implementation:
  template<class>
  struct is_function : public false_type { };

  // start of Standard function checks
  template<class _Ret, class... _Args>
  struct is_function<_Ret(_Args...)> : public true_type { };

  template<class _Ret, class... _Args>
  struct is_function<_Ret(_Args......)> : public true_type { };
  // end of standard function checks

  // start of cv-qualified function checks
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
  // End of cv-qualifed function checks

  // Start of ref-qualified function checks
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
  // End of ref-qualified function checks
  // End of is_function implementation


  // Start of is_reference implementation:
  template<class T>
  struct is_reference : public false_type { };

  template<class T>
  struct is_reference<T&> : public true_type { };

  template<class T>
  struct is_reference<T&&> : public true_type { };

  template<class T>
  inline constexpr bool is_reference_v = is_reference<T>::value;
  // End of is_reference implementation


  // Start of extent and remove_all_extents implementation:
  template<class, unsigned = 0>
  struct extent;

  template<class>
  struct remove_all_extents;

  template<class, unsigned _Uint>
  struct extent : public integral_constant<size_t, 0> { };

  template<class _Tp, unsigned _Uint, size_t _Size>
  struct extent<_Tp[_Size], _Uint> : public integral_constant<size_t, _Uint == 0 ? _Size : extent<_Tp, _Uint - 1>::value> { };

  template<class _Tp, unsigned _Uint>
  struct extent<_Tp[], _Uint> : public integral_constant<size_t, _Uint == 0 ? 0 : extent<_Tp, _Uint - 1>::value> { };

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
  // End of extend and remove_all_extends implementation


  // Start of is_array type checker implementation:
  template<class _Tp>
  struct is_array : public false_type { };

  template<class _Tp>
  struct is_array<_Tp[]> : public true_type { };

  template<class _Tp, size_t N>
  struct is_array<_Tp[N]> : public true_type { };

  template<class _Tp>
  inline constexpr bool is_array_v = is_array<_Tp>::value;

  template<class _Tp>
  struct is_array_known_bounds : public bool_constant<(extent<_Tp>::value > 0)> { };

  template<class _Tp>
  struct is_array_unknown_bounds : public __and_<is_array<_Tp>, __not_<extent<_Tp>>>::type { };
  // End of is_array implementation


  // Start of __is_complete_or_unbounded implementation:
  template<class _Tp, size_t = sizeof(_Tp)>
  constexpr true_type __is_complete_or_unbounded(__type_identity<_Tp>) { return {}; }
  
  template<class _TypeIdentity, class _NestedType = typename _TypeIdentity::type>
  constexpr typename __or_<
                    is_reference<_NestedType>,
                    is_function<_NestedType>,
                    is_void<_NestedType>,
                    is_array_unknown_bounds<_NestedType>
                    >::type __is_complete_or_unbounded(_TypeIdentity) { return {}; }
  // End of __is_complete_or_unbounded implementation


  // Start of qualifier removal helpers(remove_cv, remove_const, and remove_volatile):
  template<class T> 
  struct remove_cv { typedef T type; };
  template<class T> 
  struct remove_cv<const T> { typedef T type; };
  template<class T> 
  struct remove_cv<volatile T> { typedef T type; };
  template<class T> 
  struct remove_cv<const volatile T> { typedef T type; };

  template<class T> 
  struct remove_const { typedef T type; };
  template<class T> 
  struct remove_const<const T> { typedef T type; };

  template<class T> 
  struct remove_volatile { typedef T type; };
  template<class T> 
  struct remove_volatile<volatile T> { typedef T type; };

  template<class T>
  using remove_cv_t = typename remove_cv<T>::type;
  template<class T>
  using remove_const_t = typename remove_const<T>::type;
  template<class T>
  using remove_volatile_t = typename remove_volatile<T>::type;
  // End of qualifier removal helper implementation(remove_cv, remove_const, and remove_volatile)


  // Start of qualifier adding(add_cv, add_const, and add_volatile) meta-function implementation:
  template<class T>
  struct add_cv { typedef const volatile T type; };

  template<class T>
  struct add_const { typedef const T type; };

  template<class T>
  struct add_volatile { typedef volatile T type; };

  template<class T>
  using add_cv_t = typename add_cv<T>::type;

  template<class T>
  using add_const_t = typename add_const<T>::type;

  template<class T>
  using add_volatile_t = typename add_volatile<T>::type;
  // End of add_cv, add_const, and add_volatile implementation


  // Start of qualifier check meta-function implementations:
  template<class T>
  struct is_const : public false_type { };

  template<class T>
  struct is_const<const T> : public true_type { };

  template<class T>
  inline constexpr bool is_const_v = is_const<T>::value;

  template<class T>
  struct is_volatile : public false_type { };

  template<class T>
  struct is_volatile<volatile T> : public true_type { };

  template<class T>
  inline constexpr bool is_volatile_v = is_volatile<T>::value;

  template<class T>
  struct is_cv : public false_type { };

  template<class T>
  struct is_cv<const volatile T> : public true_type { };

  template<class T>
  struct is_cv<const T> : public true_type { };

  template<class T>
  struct is_cv<volatile T> : public true_type { };

  template<class T>
  inline constexpr bool is_cv_v = is_cv<T>::value;
  // End of qualifier check meta-function implementations


  // Start of is_void implementation:
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
  // End of is_void implementation


  // Start of is_enum type checker implementation: (uses GCC(>=V4.3) compiler intrinsics available in the Arduino IDE)
  template<class _Tp>
  struct is_enum : public bool_constant<__is_enum(_Tp)> { };

  template<class _Tp>
  inline constexpr bool is_enum_v = is_enum<_Tp>::value;
  // End of is_enum implementation


  // Start of is_union type checker implementation: (uses GCC(>=V4.3) compiler intrinsics available in the Arduino IDE)
  template<class _Tp>
  struct is_union : public bool_constant<__is_union(_Tp)> { }; // uses GCC compiler intrinsics to detect if a passed type is a union or not

  template<class _Tp>
  inline constexpr bool is_union_v = is_union<_Tp>::value;
  // End of is_union implementation


  // Start of is_class type checker implementation:
  namespace _is_class_ {
    template<class _Tp>
    bool_constant<!is_union<_Tp>::value> test(int _Tp::*);

    template<class>
    false_type test(...);
  }
  
  template<class _Tp>
  struct is_class : decltype(_is_class_::test<_Tp>(nullptr)) { };

  template<class _Tp>
  inline constexpr bool is_class_v = is_class<_Tp>::value;
  // End of is_class implementation


  // Start of is_base_of class inheritance checker implementation:
  namespace _is_base_of_ {
    template<class _B>
    true_type test_ptr_conv(const volatile _B*);
    template<class>
    false_type test_ptr_conv(const volatile void*);

    template<class _B, class _D>
    auto test_is_base_of(int) -> decltype(test_ptr_conv<_B>(static_cast<_D*>(nullptr)));
    template<class, class>
    auto test_is_base_of(...) -> true_type;
  }

  template<class _Base, class _Derived>
  struct is_base_of : public bool_constant<is_class<_Base>::value &&
  is_class<_Derived>::value && decltype(_is_base_of_::test_is_base_of<_Base, _Derived>(0))::value> { };
  
  template<class _Base, class _Derived>
  inline constexpr bool is_base_of_v = is_base_of<_Base, _Derived>::value;
  // End of is_base_of implementation


  // Start of qualifier matching meta-functions implementation:
  namespace _cv_selector_helper { // start of helper namespace

    template<class _Unqualified, bool _IsConst, bool _IsVol>
    struct __cv_selector;

    template<class _Unqualified>
    struct __cv_selector<_Unqualified, false, false> {
      typedef _Unqualified __type;
    };

    template<class _Unqualified>
    struct __cv_selector<_Unqualified, false, true> {
      typedef volatile _Unqualified __type;
    };

    template<class _Unqualified>
    struct __cv_selector<_Unqualified, true, false> {
      typedef const _Unqualified __type;
    };

    template<class _Unqualified>
    struct __cv_selector<_Unqualified, true, true> {
      typedef const volatile _Unqualified __type;
    };

  } // end of helper namespace

  template<class _Qualified, class _Unqualified,
           bool _IsConst = is_const<_Qualified>::value,
           bool _IsVol = is_volatile<_Qualified>::value>
  class match_cv_qualifiers {
    typedef _cv_selector_helper::__cv_selector<_Unqualified, _IsConst, _IsVol> __match;
    public:
      typedef typename __match::__type type;
  };

  template<class _Qualified, class _Unqualified,
           bool _IsConst = is_const<_Qualified>::value,
           bool _IsVol = is_volatile<_Qualified>::value>
  using match_cv_qualifiers_t = typename match_cv_qualifiers<_Qualified, _Unqualified, _IsConst, _IsVol>::type;
  // End of qualifier matching meta-functions implementation


  // Start of remove_refernce implementation:
  template<class T>
  struct remove_reference { typedef T type; };

  template<class T>
  struct remove_reference<T&> { typedef T type; };

  template<class T>
  struct remove_reference<T&&> { typedef T type; };

  template<class T>
  using remove_reference_t = typename remove_reference<T>::type; // Helper alias
  // End of remove_reference implementation


  // Start of add_lvalue_reference and add_rvalue_reference implementation:
  namespace _add_reference_ { // start of helper namespace
    template<class T>
    struct type_identity { using type = T; };

    template<class T>
    auto try_add_lvalue_reference(int) -> type_identity<T&>;
    template<class T>
    auto try_add_lvalue_reference(...) -> type_identity<T>;

    template<class T>
    auto try_add_rvalue_reference(int) -> type_identity<T&&>;
    template<class T>
    auto try_add_rvalue_reference(...) -> type_identity<T>;
  } // end of helper namespace

  template<class T>
  struct add_lvalue_reference : decltype(_add_reference_::try_add_lvalue_reference<T>(0)) { };

  template<class T>
  struct add_rvalue_reference : decltype(_add_reference_::try_add_rvalue_reference<T>(0)) { };

  template<class T>
  using add_lvalue_reference_t = typename add_lvalue_reference<T>::type;
  template<class T>
  using add_rvalue_reference_t = typename add_rvalue_reference<T>::type;
  // End of add_lvalue_reference and add_rvalue_reference


  // Start of remove_cvref implementation:
  template<class T>
  struct remove_cvref {
    typedef remove_cv_t<remove_reference_t<T>> type;
  };

  template<class T>
  using remove_cvref_t = typename remove_cvref<T>::type;
  // End of remove_cvref implementation


  // Start of reference type checker implementations:
  template<class T> 
  struct is_lvalue_reference : public false_type { };
  template<class T> 
  struct is_lvalue_reference<T&> : public true_type { };

  template<class T>
  inline constexpr bool is_lvalue_reference_v = is_lvalue_reference<T>::value;

  template<class T>
  struct is_rvalue_reference : public false_type { };
  template<class T>
  struct is_rvalue_reference<T&&> : public true_type { };

  template<class T>
  inline constexpr bool is_rvalue_reference_v = is_rvalue_reference<T>::value;
  // End of reference type checker implementations


  // Start of remove_pointer implementation:
  template<class _Tp, class>
  struct __remove_pointer_helper { using type = _Tp; };

  template<class _Tp, class _Up>
  struct __remove_pointer_helper<_Tp, _Up*> { using type = _Up; };

  template<class _Tp>
  struct remove_pointer : public __remove_pointer_helper<_Tp, remove_cv_t<_Tp>> { };
  
  template<class _Tp>
  using remove_pointer_t = typename remove_pointer<_Tp>::type;
  // End of remove_pointer implementation


  // Start of add_pointer implementation:
  template<class _Tp, class = void>
  struct __add_pointer_helper { using type = _Tp; };

  template<class _Tp>
  struct __add_pointer_helper<_Tp, void_t<_Tp*>> { using type = _Tp*; };

  template<class _Tp>
  struct add_pointer : public __add_pointer_helper<_Tp> { };

  template<class _Tp>
  struct add_pointer<_Tp&> { using type = _Tp*; };

  template<class _Tp>
  struct add_pointer<_Tp&&> { using type = _Tp*; };

  template<class _Tp>
  using add_pointer_t = typename add_pointer<_Tp>::type;
  // End of add_pointer implementation


  // Start of declval() implementation (NEED TO TEST UPDATED IMPL)
  template<class _Tp, class _Up = _Tp&&>
  _Up __declval(int);

  template<class _Tp>
  _Tp __declval(long);

  template<class _Tp>
  struct __declval_protector { static const bool __stop = false; };

  template<class _Tp>
  auto declval() noexcept -> decltype(__declval<_Tp>(0)) {
    static_assert(__declval_protector<_Tp>::__stop, "declval() must not be used in evaluated expressions!");
    return __declval<_Tp>(0);
  }

  //template<class T>
  //typename add_rvalue_reference<T>::type declval() noexcept;
  // this commented out implementation is what I originally used, but it has no protection for incorrect use.
  // End of declval() implementation


  // Start of move() implementation for move semantics using r-value reference variables(T&&)
  template<class T>
  constexpr remove_reference_t<T>&& move(T&& t) {
    return static_cast<remove_reference_t<T>&&>(t);
  } 
  // End of move() implementation


  // Start of iter_move implementation: (not sure if this will work)
  template<class _Iter>
  constexpr auto iter_move(_Iter&& it) noexcept(noexcept(*it)) -> decltype(move(*it)) {
    return move(*it);
  }
  // End of iter_move implementation


  // Start of forward() implementation for forwarding r-values:
  template<class T>
  constexpr T&& forward(remove_reference_t<T>& t) noexcept {
    return static_cast<T&&>(t);
  }

  template<class T>
  constexpr T&& forward(remove_reference_t<T>&& t) noexcept {
    static_assert(!is_lvalue_reference<T>::value, "Cannot forward an rvalue as an lvalue");
    return static_cast<T&&>(t);
  }
  // End of forward() implementation


  // Start of is_constructible checker implementations: (is_destructible had to be in Type_Traits.hpp since my implementation uses functions from Type_Traits)
  /*
    I'm currently using the current libstdc++ implementation of the is_constructible meta-functions.
    I also have another implementation commented out bellow this one which is another possible implementation, but I haven't been able to test it yet.
    If the second implementation ends up working just as well as the libstdc++ implementation then I will switch to it for less code and better readability.
  */
  // NOTE: is_constructible and is_destructible are currently implemented in Type_Traits.hpp since it uses some type trait meta-functions that I can't move over to this file. the is_destructible implementation in Type_Traits.hpp is the same as in libstdc++
  // second implementation: (wont use until I test it to make sure it works correctly)
  /*
  template<class, class T, class... Args>
  struct is_constructible_ : public false_type { };

  template<class T, class... Args>
  struct is_constructible_<void_t<decltype(T(declval<Args>()...))>, T, Args...> : public true_type { };

  template<class T, class... Args>
  using is_constructible = is_constructible_<void_t<>, T, Args...>;

  template<class T>
  struct is_default_constructible : is_constructible<T> { };
  
  template<class T>
  inline constexpr bool is_default_constructible_v = is_default_constructible<T>::value;

  template<class T>
  struct is_copy_constructible : is_constructible<T, typename add_lvalue_reference<typename add_const<T>::type>::type> { };
  
  template<class T>
  inline constexpr bool is_copy_constructible_v = is_copy_constructible<T>::value;

  template<class T>
  struct is_move_constructible : public is_constructible<T, typename add_rvalue_reference<T>::type> { };
  
  template<class T>
  inline constexpr bool is_move_constructible_v = is_move_constructible<T>::value;
  */
  // End of is_constructible checker implementations


  // Start of is_assignable checker implementations: (changed to be the same as in libstdc++)
  namespace _assignable_helper { // start of private helper namespace
    
    template<class _Tp, class _Up>
    class __is_assignable_helper : public __sfinae_types {
      template<class _Tp1, class _Up1>
      static decltype(declval<_Tp1>() = declval<_Up1>(), __one()) __test(int);

      template<class, class>
      static __two __test(...);

      public:
        static constexpr bool value = sizeof(__test<_Tp, _Up>(0)) == 1;
    };

    template<class _Tp, class _Up> // public
    struct is_assignable 
      : public bool_constant<__is_assignable_helper<_Tp, _Up>::value> { };

    template<class _Tp, bool = is_void<_Tp>::value>
    struct __is_copy_assignable_impl;

    template<class _Tp>
    struct __is_copy_assignable_impl<_Tp, true> : public false_type { };

    template<class _Tp>
    struct __is_copy_assignable_impl<_Tp, false>
      : public is_assignable<_Tp&, const _Tp&> { };
    
    template<class _Tp> // public
    struct is_copy_assignable : public __is_copy_assignable_impl<_Tp> { };

    template<class _Tp, bool = is_void<_Tp>::value>
    struct __is_move_assignable_impl;

    template<class _Tp>
    struct __is_move_assignable_impl<_Tp, true> : public false_type { };

    template<class _Tp>
    struct __is_move_assignable_impl<_Tp, false>
      : public is_assignable<_Tp&, _Tp&&> { };

    template<class _Tp>
    struct is_move_assignable : public __is_move_assignable_impl<_Tp> { };

  } // end of helper namespace

  template<class _Tp, class _Up>
  struct is_assignable 
    : public bool_constant<_assignable_helper::__is_assignable_helper<_Tp, _Up>::value> { };

  template<class _Tp>
  struct is_copy_assignable 
    : public _assignable_helper::__is_copy_assignable_impl<_Tp> { };

  template<class _Tp>
  struct is_move_assignable
    : public _assignable_helper::__is_move_assignable_impl<_Tp> { };

  /*
    Original implementation I used, but it doesn't work properly
    leaving this code here and commented out for when I have a chance to better test these functions
    If these end up working as well as the implementation above then I will just revert back to the one bellow.
    Unfortunately I'm not able to use the most modern implementation of is_assignable since libstdc++
    uses a compiler intrinsic(__is_assignable(type)) which is not available in the current GCC version that the arduino IDE uses
  */
  /*
  template<class T, class U, class = void>
  struct is_assignable : public false_type { };

  template<class T, class U>
  struct is_assignable<T, U, decltype(declval<T>() = declval<U>(), void())> : public true_type { };

  template<class T, class U>
  inline constexpr bool is_assignable_v = is_assignable<T, U>::value;

  template<class T>
  struct is_copy_assignable : is_assignable<typename add_lvalue_reference<T>::type, typename add_lvalue_reference<const T>::type> { };

  template<class T>
  inline constexpr bool is_copy_assignable_v = is_copy_assignable<T>::value;

  template<class T>
  struct is_move_assignable : is_assignable<typename add_lvalue_reference<T>::type, typename add_rvalue_reference<T>::type> { };

  template<class T>
  inline constexpr bool is_move_assignable_v = is_move_assignable<T>::value;
  */
  // End of is_assignable checker implementations


  // Start of initial swap() implementation:
  template<class T>
  constexpr inline void swap(T& a, T& b) noexcept(/*is_move_constructible<T>::value &&*/ is_move_assignable<T>::value) {
    T tmp = move(a);
    a = move(b);
    b = move(tmp);
  }

  template<class ForwardIt1, class ForwardIt2>
  constexpr inline void iter_swap(ForwardIt1 a, ForwardIt2 b) {
    swap(*a, *b);
  }

  template<class ForwardIt1, class ForwardIt2>
  constexpr inline void swap_ranges(ForwardIt1 first1, ForwardIt1 last1, ForwardIt2 first2) {
    for(; first1 != last1; ++first1, ++first2) iter_swap(first1, first2);
    //return first2;
  }

  template<class T, size_t N>
  constexpr inline void swap(T (&a)[N], T (&b)[N]) noexcept(noexcept(swap(*a, *b))) {
    swap_ranges(a, a + N, b);
  }
  // End of initial swap() implementation

} // end of std:: namespace


#endif // end of header
