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
  Utility.hpp [V1.0.0] (template class header/declaration file)
    By: Andrey Stroitelev

  ~This file adds a bunch of utility functions from the std namespace(libstdc++) that arent available for AVR boards on the arduino IDE
  ~This file was mainly added out of necessity for the implementations of the data structure classes in this library.
  ~This file also adds the std::move() function which allows you to do move semantics in the arduino IDE which are also not normally supported in the arduino IDE
    *You can use r-value references(&&), but it ends up being no different than just using regular references(&) if you don't use std::move()

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


  // Start of reference type checker implementations:
  template<class T> 
  struct is_lvalue_reference : public false_type { };
  template<class T> 
  struct is_lvalue_reference<T&> : public true_type { };

  template<class T>
  constexpr bool is_lvalue_reference_v = is_lvalue_reference<T>::value;

  template<class T>
  struct is_rvalue_reference : public false_type { };
  template<class T>
  struct is_rvalue_reference<T&&> : public true_type { };

  template<class T>
  constexpr bool is_rvalue_reference_v = is_rvalue_reference<T>::value;
  // End of reference type checker implementations


  // Start of declval() implementation
  template<class T>
  typename add_rvalue_reference<T>::type declval() noexcept;
  // End of declval() implementation


  // Start of move() implementation for move semantics using r-value reference variables(T&&)
  template<class T>
  constexpr remove_reference_t<T>&& move(T&& t) {
    return static_cast<remove_reference_t<T>&&>(t);
  } 
  // End of move() implementation


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


  // Start of is_constructible checker implementations:
  template<class...>
  using void_t = void;

  template<class, class T, class... Args>
  struct is_constructible_ : public false_type { };

  template<class T, class... Args>
  struct is_constructible_<void_t<decltype(T(declval<Args>()...))>, T, Args...> : public true_type { };

  template<class T, class... Args>
  using is_constructible = is_constructible_<void_t<>, T, Args...>;

  template<class T>
  struct is_move_constructible : public is_constructible<T, typename add_rvalue_reference<T>::type> { };
  template<class T>
  constexpr bool is_move_constructible_v = is_move_constructible<T>::value;
  // End of is_constructible checker implementations


  // Start of is_assignable checker implementations:
  template<class T, class U, class = void>
  struct is_assignable : public false_type { };

  template<class T, class U>
  struct is_assignable<T, U, decltype(declval<T>() = declval<U>(), void())> : public true_type { };

  template<class T, class U>
  constexpr bool is_assignable_v = is_assignable<T, U>::value;

  template<class T>
  struct is_move_assignable : is_assignable<typename add_lvalue_reference<T>::type, typename add_lvalue_reference<T>::type> { };

  template<class T>
  constexpr bool is_move_assignable_v = is_move_assignable<T>::value;
  // End of is_assignable checker implementations


  // Start of initial swap() implementation:
  template<class T>
  constexpr void swap(T& a, T& b) noexcept(is_move_constructible<T>::value && is_move_assignable<T>::value) {
    T tmp(move(a));
    a = move(b);
    b = move(tmp);
  }

  template<class ForwardIt1, class ForwardIt2>
  constexpr void iter_swap(ForwardIt1 a, ForwardIt2 b) {
    swap(*a, *b);
  }

  template<class ForwardIt1, class ForwardIt2>
  constexpr void swap_ranges(ForwardIt1 first1, ForwardIt1 last1, ForwardIt2 first2) {
    for(; first1 != last1; ++first1, ++first2) iter_swap(first1, first2);
    //return first2;
  }

  template<class T, size_t N>
  constexpr void swap(T (&a)[N], T (&b)[N]) noexcept(noexcept(swap(*a, *b))) {
    swap_ranges(a, a + N, b);
  }
  // End of initial swap() implementation

} // end of std:: namespace


#endif // end of header
