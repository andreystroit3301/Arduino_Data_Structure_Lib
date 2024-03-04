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
  TypeTraits.hpp [V1.0.0] (template class header/declaration file)
    By: Andrey Stroitelev

  ~This header holds a few remade functions from the standard c++ type_traits library.
  ~These functions are simple helpers when working with template types and template functions specializations
  ~The implementation of this library will be very limited as some things I just wont be able to implement, and others I see no use in implementing in regards to this data structure library
  ~Most of these "functions" are actually metafunctions which are structs that use template behavior to produce a dynamic result at compile time.
    *Since the metafunctions are struct objects, when they are used you must explicitly access the value or type in the struct with the scope specifier operator(::type or ::value).
    *Most of the  metafunctions have a helper alias or variable that are named the same with an added _t or _v at the end depending on if the metafunction produces a type or value.
    *Since the types and values are defined by the specialization, every metafunction instance can have a different type or value.
*/


// Start of Type_Traits.hpp
#ifndef _TYPE_TRAITS_HPP_
#define _TYPE_TRAITS_HPP_

#ifndef _TYPE_TRAITS_VERSION_
#define _TYPE_TRAITS_VERSION_ 0x010000
#endif


#include <Arduino.h> // including all of the std c++ stuff from the arduino core
#include "Utility.hpp"


// Start of std:: namespace:
namespace std {

  // Start of is_same implementation:
  template<class T, class U>
  struct is_same : public false_type { };

  template<class T>
  struct is_same<T, T> : public true_type { };

  template<class T, class U>
  constexpr bool is_same_v = is_same<T, U>::value;
  // End of is_same implementation


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


  // Start of conditional implementation:
  template<bool B, class T, class F>
  struct conditional { using type = T; };

  template<class T, class F>
  struct conditional<false, T, F> { using type = F; };

  template<bool B, class T, class F>
  using conditional_t = typename conditional<B, T, F>::type;
  // End of conditional implementation


  // Start of conjunction implementation:
  template<class...>
  struct conjunction : public true_type { };

  template<class T1>
  struct conjunction<T1> : public T1 { };

  template<class T1, class... Tn>
  struct conjunction<T1, Tn...> : public conditional_t<bool(T1::value), conjunction<Tn...>, T1> { };

  template<class... T>
  constexpr bool conjunction_v = conjunction<T...>::value;
  // End of conjunction implementation


  // Start of is_void implementation:
  template<class T>
  struct is_void : public is_same<void, typename remove_cv<T>::type> { };
  // End of is_void implementation 


  // Start of is_null_pointer type checker implementation:
  template<class T>
  struct is_null_pointer : public is_same<nullptr_t, remove_cv_t<T>> { };

  template<class T>
  constexpr bool is_null_pointer_v = is_null_pointer<T>::value;
  // End of is_null_pointer implementation


  // Start of is_pointer type checker implementation:
  template<class T>
  struct is_pointer : public false_type { };

  template<class T>
  struct is_pointer<T*> : public true_type { };

  template<class T>
  struct is_pointer<T* const> : public true_type { };

  template<class T>
  struct is_pointer<T* volatile> : public true_type { };

  template<class T>
  struct is_pointer<T* const volatile> : public true_type { };

  template<class T>
  constexpr bool is_pointer_v = is_pointer<T>::value;
  // End of is_pointer implementation


  // Start of is_member_pointer type checker implementation
  template<class T>
  struct _is_member_pointer_helper_ : false_type { };

  template<class T, class U>
  struct _is_member_pointer_helper_<T U::*> : true_type { };

  template<class T>
  struct is_member_pointer : public _is_member_pointer_helper_<typename remove_cv<T>::type> { };

  template<class T>
  constexpr bool is_member_pointer_v = is_member_pointer<T>::value;
  // End of is_member_pointer implementation


  // Start of is_floating_point type checker implementation:
  template<class T>
  struct is_floating_point : public bool_constant<is_same<float, typename remove_cv<T>::type>::value
                                               || is_same<double, typename remove_cv<T>::type>::value
                                               || is_same<long double, typename remove_cv<T>::type>::value> { };

  template<class T>
  constexpr bool is_floating_point_v = is_floating_point<T>::value;
  // End of is_floating_point implementation


  // Start of is_integral type checker implementation:
  namespace __is_integral_helper_ { // start of is_integral helper namespace

    template<class>
    struct _helper : public false_type { };

    template<>
    struct _helper<bool> : public true_type { };

    template<>
    struct _helper<char> : public true_type { };

    template<>
    struct _helper<signed char> : public true_type { };

    template<>
    struct _helper<unsigned char> : public true_type { };

    template<>
    struct _helper<wchar_t> : public true_type { };

    template<>
    struct _helper<char16_t> : public true_type { };

    template<>
    struct _helper<char32_t> : public true_type { };

    template<>
    struct _helper<short> : public true_type { };

    template<>
    struct _helper<unsigned short> : public true_type { };

    template<>
    struct _helper<int> : public true_type { };

    template<>
    struct _helper<unsigned int> : public true_type { };

    template<>
    struct _helper<long> : public true_type { };

    template<>
    struct _helper<unsigned long> : public true_type { };

    template<>
    struct _helper<long long> : public true_type { };

    template<>
    struct _helper<unsigned long long> : public true_type { };

  } // end of is_integral helper namespace

  template<class T>
  struct is_integral : public bool_constant<__is_integral_helper_::_helper<remove_cv_t<T>>::value> { };

  template<class T>
  constexpr bool is_integral_v = is_integral<T>::value;
  // End of is_integral type checker implementation


  // Start of is_enum type checker implementation: (uses GCC(>=V4.3) compiler intrinsics available in the Arduino IDE)
  template<class _Tp>
  struct is_enum : public bool_constant<__is_enum(_Tp)> { };

  template<class T>
  constexpr bool is_enum_v = is_enum<T>::value;
  // End of is_enum implementation


  // Start of is_union type checker implementation: (uses GCC(>=V4.3) compiler intrinsics available in the Arduino IDE)
  template<class _Tp>
  struct is_union : public bool_constant<__is_union(_Tp)> { }; // uses GCC compiler intrinsics to detect if a passed type is a union or not

  template<class T>
  constexpr bool is_union_v = is_union<T>::value;
  // End of is_union implementation


  // Start of is_class type checker implementation:
  namespace _is_class_ {
    template<class T>
    bool_constant<!is_union<T>::value> test(int T::*);

    template<class>
    false_type test(...);
  }
  
  template<class T>
  struct is_class : decltype(_is_class_::test<T>(nullptr)) { };

  template<class T>
  constexpr bool is_class_v = is_class<T>::value;
  // End of is_class implementation


  // Start of is_arithmetic  type checker implementation:
  template<class T>
  struct is_arithmetic : public bool_constant<is_integral<T>::value || is_floating_point<T>::value> { };

  template<class T>
  constexpr bool is_arithmetic_v = is_arithmetic<T>::value;
  // End of is_arithmetic implementation


  // Start of is_scalar type checker implementation:
  template<class T>
  struct is_scalar : public bool_constant<is_arithmetic<T>::value
                                       || is_enum<T>::value
                                       || is_pointer<T>::value
                                       || is_member_pointer<T>::value
                                       || is_null_pointer<T>::value> { };
  // End of is_scalar implementation


  // Start of is_array type checker implementation:
  template<class T>
  struct is_array : public false_type { };

  template<class T>
  struct is_array<T[]> : public true_type { };

  template<class T, size_t N>
  struct is_array<T[N]> : public true_type { };

  template<class T>
  constexpr bool is_array_v = is_array<T>::value;
  // End of is_array implementation


  // Start of is_object type checker implementation:
  template<class T>
  struct is_object : public bool_constant<is_scalar<T>::value
                                       || is_array<T>::value
                                       || is_union<T>::value
                                       || is_class<T>::value> { };
  // End of is_object implementation


  // Start of is_base_of class inheritance checker implementation:
  namespace _is_base_of_ {
    template<class B>
    true_type test_ptr_conv(const volatile B*);
    template<class>
    false_type test_ptr_conv(const volatile void*);

    template<class B, class D>
    auto test_is_base_of(int) -> decltype(test_ptr_conv<B>(static_cast<D*>(nullptr)));
    template<class, class>
    auto test_is_base_of(...) -> true_type;
  }

  template<class Base, class Derived>
  struct is_base_of : public bool_constant<is_class<Base>::value &&
  is_class<Derived>::value && decltype(_is_base_of_::test_is_base_of<Base, Derived>(0))::value> { };
  
  template<class Base, class Derived>
  constexpr bool is_base_of_v = is_base_of<Base, Derived>::value;
  // End of is_base_of implementation


  // Start of is_convertible type conversion check implementation:
  namespace _is_convertible_ { // start of helper namespace _is_convertible_
    template<class T>
    auto test_returnable(int) -> decltype(void(static_cast<T(*)()>(nullptr)), true_type{});

    template<class>
    auto test_returnable(...) -> false_type;

    template<class From, class To>
    auto test_implicitly_convertible(int) -> decltype(void(declval<void(&)(To)>()(declval<From>())), true_type{});

    template<class, class>
    auto test_implicitly_convertible(...) -> false_type;
  } // end of _is_convertible_ namespace
  
  template<class From, class To>
  struct is_convertible : public bool_constant<(decltype(_is_convertible_::test_returnable<To>(0))::value &&
  decltype(_is_convertible_::test_implicitly_convertible<From, To>(0))::value) ||
  (is_void<From>::value && is_void<To>::value)> { };

  template<class From, class To>
  constexpr bool is_convertible_v = is_convertible<From, To>::value;
  // End of is_convertible implementation


  // Start of derived_from inheritance checker implementation:
  // arduino IDE doesn't support concepts so im implementing this as a constexpr bool that can be used with static_assert to get similar functionality
  template<class Derived, class Base>
  constexpr bool derived_from = is_base_of_v<Base, Derived> && is_convertible_v<const volatile Derived*, const volatile Base*>;
  // End of derived_from inheritance checker implementation


  // Start of enable_if metafunction for SFINAE errors implementation:
  template<bool B, class T = void> // might change the default type to something else to reduce possibility for errors when using this metafunction
  struct enable_if { };

  template<class T>
  struct enable_if<true, T> { typedef T type; };

  template<bool B, class T = void>
  using enable_if_t = typename enable_if<B, T>::type;
  // End of enable_if implementation


  // Start of addressof function implementation to get lvalue address of an object with an overloaded '&' operator:
  template<class T>
  typename enable_if<is_object<T>::value, T*>::type addressof(T& arg) noexcept {
    return reinterpret_cast<T*>( &const_cast<char&>( reinterpret_cast<const volatile char&>(arg) ) );
  }

  template<class T>
  typename enable_if<!is_object<T>::value, T*>::type addressof(T& arg) noexcept {
    return &arg;
  }
  // End of adressof implementation

} // end of std:: namespace


#endif // End of Type_Traits.hpp
