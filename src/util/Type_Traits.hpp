// Libstdc++ and GCC GNU Lincese boilerplate:      (this is required since this code is copyrighted under the GCC compiler. I did not design this myself.)
/*
  Partial Modified Implementation Of Meta-Functions from the <type_traits> Libstdc++ header  -*- C++ -*-

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

  // Start of conjunction implementation:
  template<class...>
  struct conjunction : public true_type { };

  template<class T1>
  struct conjunction<T1> : public T1 { };

  template<class T1, class... Tn>
  struct conjunction<T1, Tn...> : public conditional_t<bool(T1::value), conjunction<Tn...>, T1> { };

  template<class... _Tp>
  inline constexpr bool conjunction_v = conjunction<_Tp...>::value;
  // End of conjunction implementation


  // Start of is_null_pointer type checker implementation:
  template<class _Tp>
  struct is_null_pointer : public is_same<nullptr_t, remove_cv_t<_Tp>> { };

  template<class _Tp>
  inline constexpr bool is_null_pointer_v = is_null_pointer<_Tp>::value;
  // End of is_null_pointer implementation


  // Start of is_pointer type checker implementation:
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
  // End of is_pointer implementation


  // Start of is_member_pointer type checker implementation
  template<class _Tp>
  struct _is_member_pointer_helper_ : false_type { };

  template<class _Tp, class _Up>
  struct _is_member_pointer_helper_<_Tp _Up::*> : true_type { };

  template<class _Tp>
  struct is_member_pointer : public _is_member_pointer_helper_<typename remove_cv<_Tp>::type> { };

  template<class _Tp>
  inline constexpr bool is_member_pointer_v = is_member_pointer<_Tp>::value;
  // End of is_member_pointer implementation


  // Start of is_floating_point type checker implementation:
  template<class _Tp>
  struct is_floating_point : public bool_constant<is_same<float, typename remove_cv<_Tp>::type>::value
                                               || is_same<double, typename remove_cv<_Tp>::type>::value
                                               || is_same<long double, typename remove_cv<_Tp>::type>::value> { };

  template<class _Tp>
  inline constexpr bool is_floating_point_v = is_floating_point<_Tp>::value;
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

  template<class _Tp>
  struct is_integral : public bool_constant<__is_integral_helper_::_helper<remove_cv_t<_Tp>>::value> { };

  template<class _Tp>
  inline constexpr bool is_integral_v = is_integral<_Tp>::value;
  // End of is_integral type checker implementation


  // Start of is_arithmetic  type checker implementation:
  template<class _Tp>
  struct is_arithmetic : public bool_constant<is_integral<_Tp>::value || is_floating_point<_Tp>::value> { };

  template<class _Tp>
  inline constexpr bool is_arithmetic_v = is_arithmetic<_Tp>::value;
  // End of is_arithmetic implementation


  // Start of is_scalar type checker implementation:
  template<class _Tp>
  struct is_scalar : public bool_constant<is_arithmetic<_Tp>::value
                                       || is_enum<_Tp>::value
                                       || is_pointer<_Tp>::value
                                       || is_member_pointer<_Tp>::value
                                       || is_null_pointer<_Tp>::value> { };
  // End of is_scalar implementation


  // Start of is_signed implementation:
  namespace _is_signed_ { // Start of helper namespace
    
    template<class _Tp, bool = is_arithmetic<_Tp>::value>
    struct is_signed : public bool_constant<(_Tp(0) > _Tp(-1))> { }; // using this implementation for now because I know it will work
    //struct is_signed : public bool_constant<declval<_Tp>(0) > declval<_Tp>(-1)> { }; // might switch to this if possible
    
    template<class _Tp>
    struct is_signed<_Tp, false> : public false_type { };
  
  } // end of helper namespace

  template<class _Tp>
  struct is_signed : public _is_signed_::is_signed<_Tp>::type { };

  template<class _Tp>
  inline constexpr bool is_signed_v = is_signed<_Tp>::value;
  // End of is_signed implementation


  // Start of is_signed_integral implementation:
  template<class _Tp>
  struct is_signed_integral
    : public bool_constant<is_integral<_Tp>::value && is_signed<_Tp>::value> { };

  template<class _Tp>
  inline constexpr bool is_signed_integral_v = is_signed_integral<_Tp>::value;
  // End of is_signed_integral implementation


  // Start of is_unsigned implementation:
  namespace _is_unsigned_ { // start of helper namespace

    template<class _Tp, bool = is_arithmetic<_Tp>::value>
    struct is_unsigned : public bool_constant<(_Tp(-1) > _Tp(0))> { };

    template<class _Tp>
    struct is_unsigned<_Tp, false> : public false_type { };

  } // start of helper namespace

  template<class _Tp>
  struct is_unsigned : _is_unsigned_::is_unsigned<_Tp>::type { };

  template<class _Tp>
  inline constexpr bool is_unsigned_v = is_unsigned<_Tp>::value;
  // End of is_unsigned implementation


  // Start of is_unsigned_integral implementation:
  template<class _Tp>
  struct is_unsigned_integral
    : public bool_constant<is_integral<_Tp>::value && is_unsigned<_Tp>::value> { };

  template<class _Tp>
  inline constexpr bool is_unsigned_integral_v = is_unsigned_integral<_Tp>::value;
  // End of is_unsigned_integral implementation


  // Start of make_unsigned implementation:
  namespace _unsign_helper { // start of helper namespace

    template<class _Tp>
    struct __make_unsigned { typedef _Tp __type; };

    template<>
    struct __make_unsigned<char> { typedef unsigned char __type; };

    template<>
    struct __make_unsigned<signed char> { typedef unsigned char __type; };

    template<>
    struct __make_unsigned<short> { typedef unsigned short __type; };

    template<>
    struct __make_unsigned<int> { typedef unsigned int __type; };

    template<>
    struct __make_unsigned<long> { typedef unsigned long __type; };

    template<>
    struct __make_unsigned<long long> { typedef unsigned long long __type; };

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

  } // end of helper namespace

  template<class _Tp>
  struct make_unsigned {
    typedef typename _unsign_helper::__make_unsigned_selector<_Tp>::__type type;
  };

  template<>
  struct make_unsigned<bool>; // undefined to prevent using bool because it would let you

  template<class _Tp>
  using make_unsigned_t = typename make_unsigned<_Tp>::type;
  // End of make_unsigned implementation


  // Start of make_signed implementation:
  namespace _sign_helper { // start of helper namespace

    template<class _Tp>
    struct __make_signed { typedef _Tp __type; };

    template<>
    struct __make_signed<char> { typedef signed char __type; };

    template<>
    struct __make_signed<unsigned char> { typedef signed char __type; };

    template<>
    struct __make_signed<unsigned short> { typedef signed short __type; };

    template<>
    struct __make_signed<unsigned int> { typedef signed int __type; };

    template<>
    struct __make_signed<unsigned long> { typedef signed long __type; };

    template<>
    struct __make_signed<unsigned long long> { typedef signed long long __type; };

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
      typedef typename _unsign_helper::__make_unsigned_selector<_Tp>::__type  __unsigned_type;
      public:
        typedef typename __make_signed_selector<__unsigned_type>::__type __type;
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

  } // end of helper namespace

  template<class _Tp>
  struct make_signed {
    typedef typename _sign_helper::__make_signed_selector<_Tp>::__type type;
  };

  template<>
  struct make_signed<bool>; // undefining this specialization

  template<class _Tp>
  using make_signed_t = typename make_signed<_Tp>::type;
  // End of make_signed implementation


  // Start of rank implementation
  template<class>
  struct rank : public integral_constant<size_t, 0> { };

  template<class _Tp, size_t _Size>
  struct rank<_Tp[_Size]> : public integral_constant<size_t, 1 + rank<_Tp>::value> { };

  template<class _Tp>
  struct rank<_Tp[]> : public integral_constant<size_t, 1 + rank<_Tp>::value> { };
  // End of rank implementation


  // Start of decay implementation:
  template<class _Up>
  struct __decay_selector : public conditional_t<is_const<const _Up>::value,
                                                 remove_cv<_Up>,
                                                 add_pointer<_Up>> { };

  template<class _Up, size_t _Nm>
  struct __decay_selector<_Up[_Nm]> { using type = _Up*; };

  template<class _Up>
  struct __decay_selector<_Up[]> { using type = _Up*; };

  template<class _Tp>
  struct decay { using type = typename __decay_selector<_Tp>::type; };

  template<class _Tp>
  struct decay<_Tp&> { using type = typename __decay_selector<_Tp>::type; };

  template<class _Tp>
  struct decay<_Tp&&> { using type = typename __decay_selector<_Tp>::type; };

  template<class _Tp>
  using decay_t = typename decay<_Tp>::type;
  // End of decay implementation


  // Start of common_type implementation:
  /*template<class...>
  struct common_type { };

  template<class _Tp>
  struct common_type<_Tp> : public common_type<_Tp, _Tp> { };

  namespace _common_type_helper { // start of private helper namespace

    template<class _Tp1, class _Tp2>
    using conditional_result_t = decltype(false ? declval<_Tp1>() : declval<_Tp2>());

    template<class, class, class = void>
    struct decay_conditional_result { };

    template<class _Tp1, class _Tp2>
    struct decay_conditional_result<_Tp1, _Tp2, void_t<conditional_result_t<_Tp1, _Tp2>>>
      : public decay<conditional_result_t<_Tp1, _Tp2>> { };

    template<class _Tp1, class _Tp2, class = void>
    struct common_type_2_impl : decay_conditional_result<const _Tp1&, const _Tp2&> { };

    template<class _Tp1, class _Tp2>
    struct common_type_2_impl<_Tp1, _Tp2, void_t<conditional_result_t<_Tp1, _Tp2>>>
      : decay_conditional_result<_Tp1, _Tp2> { };

  } // helper namespace to be continued

  template<class _Tp1, class _Tp2>
  struct common_type<_Tp1, _Tp2> : public conditional<is_same<_Tp1, typename decay<_Tp1>::type>::value &&
                                                      is_same<_Tp2, typename decay<_Tp2>::type>::value,
                                                      _common_type_helper::common_type_2_impl<_Tp1, _Tp2>,
                                                      common_type<typename decay<_Tp1>::type,
                                                                  typename decay<_Tp2>::type>>::type { };

  namespace _common_type_helper { // continued private helper namespace
    
    template<class _AV, class _Tp1, class _Tp2, class... _Rp>
    struct common_type_multi_impl { };

    template<class _Tp1, class _Tp2, class... _Rp>
    struct common_type_multi_impl<void_t<typename common_type<_Tp1, _Tp2>::type>, _Tp1, _Tp2, _Rp...>
      : public common_type<typename common_type<_Tp1, _Tp2>::type, _Rp...> { };

  } // end of private helper namespace

  template<class _Tp1, class _Tp2, class... _Rp>
  struct common_type<_Tp1, _Tp2, _Rp...>
    : public _common_type_helper::common_type_multi_impl<void, _Tp1, _Tp2, _Rp...> { };
  
  template<class... _Tp>
  using common_type_t = typename common_type<_Tp...>::type;*/
  template<class... _Tp>
  struct common_type;

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

  template<>
  struct common_type<> { };

  template<class _Tp0>
  struct common_type<_Tp0> : public common_type<_Tp0, _Tp0> { };

  template<class _Tp1, class _Tp2,
           class _Dp1 = decay_t<_Tp1>, class _Dp2 = decay_t<_Tp2>>
  struct __common_type_impl { using type = common_type<_Dp1, _Dp2>; };

  template<class _Tp1, class _Tp2>
  struct __common_type_impl<_Tp1, _Tp2, _Tp1, _Tp2> : private __do_common_type_impl {
    using type = decltype(_S_test<_Tp1,_Tp2>(0));
  };

  template<class _Tp1, class _Tp2>
  struct common_type<_Tp1, _Tp2> : public __common_type_impl<_Tp1, _Tp2>::type { };

  template<class...>
  struct __common_type_pack { };

  template<class, class, class = void>
  struct __common_type_fold;

  template<class _Tp1, class _Tp2, class... _Rp>
  struct common_type<_Tp1, _Tp2, _Rp...> : public __common_type_fold<common_type<_Tp1, _Tp2>, 
                                                                    __common_type_pack<_Rp...>> { };

  template<class _CTp, class... _Rp>
  struct __common_type_fold<_CTp, __common_type_pack<_Rp...>,
                            void_t<typename _CTp::type>>
    : public common_type<typename _CTp::type, _Rp...> { };

  template<class _CTp, class _Rp>
  struct __common_type_fold<_CTp, _Rp, void> { };

  template<class... _Tp>
  using common_type_t = typename common_type<_Tp...>::type;
  // End of common_type implementation


  // Start of is_object type checker implementation:
  template<class _Tp>
  struct is_object : public bool_constant<is_scalar<_Tp>::value
                                       || is_array<_Tp>::value
                                       || is_union<_Tp>::value
                                       || is_class<_Tp>::value> { };

  // Old style implementation which may work better:
  //template<class _Tp>
  //struct is_object : public __not_<__or_<is_function<_Tp>, is_reference<_Tp>, is_void<_Tp>>>::type { };  
  // End of is_object implementation


  // Start of is_destructible implementation
  namespace _is_destructible_ { // start of is_destructible helper namespace
    
    struct __do_is_destructible_impl {
      template<class _Tp, class = decltype(declval<_Tp&>().~_Tp())>
      static true_type __test(int);

      template<class>
      static false_type __test(...);
    };

    template<class _Tp>
    struct __is_destructible_impl : public __do_is_destructible_impl {
      typedef decltype(__test<_Tp>(0)) type;
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

  }

  template<class _Tp>
  struct is_destructible : public _is_destructible_::__is_destructible_safe<_Tp>::type { };

  template<class _Tp>
  struct is_trivially_destructible : public __and_<_is_destructible_::__is_destructible_safe<_Tp>,
                                                   bool_constant<__has_trivial_destructor(_Tp)>>::type { };

  template<class _Tp>
  inline constexpr bool is_destructible_v = is_destructible<_Tp>::value;
  // End of is_destructible implementation


  // Start of is_constructible, is_default_constructible, is_copy_constructible, and is_move_constructible implementations:
  namespace _construct_helper { // start of private helper namespace

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
      // in libstdc++ the implementation for the decltype statement is decltype(::new _Tp(declval<_Arg>())) without the ::delete.
      // Just to be safe for now I added the ::delete as suggested in the libstdc++ comment mentioning c++/51222.
      // I may remove this if it causes issues, but I also may remove the ::new if it ends up actually dynamically allocating memory which cannot be deleted.
      // I can't find anything online that mentions if the decltype() function will automatically destrory objects created in it's parameters.
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
    
    //template<class, class>
    //struct is_same;
    //template<class, class>
    //struct is_base_of;
    //template<class>
    //struct remove_reference;

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
      
      using __dst_t = typename remove_cv<typename remove_reference<_To>::type>::type;

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
      static_assert(sizeof...(_Args) > 1, "Only useful for >1 arguments");
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

  } // end of private helper namespace
  // duplicate definition for the std:: namespace. The original is in std::_construct_helper:: since its required for further implementations in the private namespace.
  // I mainly did this to prevent having to split up the helper namespace into multiple seperate sections, but I may do so anyway to kind of organize the code. For now this is just more convenient.

  template<class _From, class _To>
  struct __is_static_castable // made this accessible outside of the helper incase I need it for other things, if not then I will just delete this
    : public bool_constant<_construct_helper::__is_static_castable_safe<_From, _To>::value> { };

  template<class _Tp>
  struct is_default_constructible 
    : public bool_constant<_construct_helper::__is_default_constructible_safe<_Tp>::value> { };
  
  template<class _Tp, class... _Args>
  struct is_constructible
    : public bool_constant<_construct_helper::__is_constructible_impl<_Tp, _Args...>::value> { };

  template<class _Tp>
  struct is_copy_constructible
    : public _construct_helper::__is_copy_constructible_impl<_Tp> { };

  template<class _Tp>
  struct is_move_constructible
    : public _construct_helper::__is_move_constructible_impl<_Tp> { };
  // End of is_constructible and variants implementations


  // Start of is_convertible type conversion check implementation:
  namespace _is_convertible_ { // start of helper namespace _is_convertible_
    template<class _Tp>
    auto test_returnable(int) -> decltype(void(static_cast<_Tp(*)()>(nullptr)), true_type{});

    template<class>
    auto test_returnable(...) -> false_type;

    template<class _From, class _To>
    auto test_implicitly_convertible(int) -> decltype(void(declval<void(&)(_To)>()(declval<_From>())), true_type{});

    template<class, class>
    auto test_implicitly_convertible(...) -> false_type;
  } // end of _is_convertible_ namespace
  
  template<class _From, class _To>
  struct is_convertible : public bool_constant<(decltype(_is_convertible_::test_returnable<_To>(0))::value &&
  decltype(_is_convertible_::test_implicitly_convertible<_From, _To>(0))::value) ||
  (is_void<_From>::value && is_void<_To>::value)> { };

  template<class _From, class _To>
  inline constexpr bool is_convertible_v = is_convertible<_From, _To>::value;
  // End of is_convertible implementation


  // Start of derived_from inheritance checker implementation:
  // arduino IDE doesn't support concepts so im implementing this as a constexpr bool that can be used with static_assert to get similar functionality
  template<class _Derived, class _Base>
  struct derived_from : bool_constant<is_base_of<_Base, _Derived>::value && is_convertible<const volatile _Derived*, const volatile _Base*>::value> { };

  template<class _Derived, class _Base>
  inline constexpr bool derived_from_v = derived_from<_Derived, _Base>::value;
  // End of derived_from inheritance checker implementation


  // Start of enable_if metafunction for SFINAE errors implementation:
  template<bool _B, class _Tp = void>
  struct enable_if { };

  template<class _Tp>
  struct enable_if<true, _Tp> { typedef _Tp type; };

  template<bool _B, class _Tp = void>
  using enable_if_t = typename enable_if<_B, _Tp>::type;

  template<class... _Cond>
  using _Require = enable_if_t<__and_<_Cond...>::value>;
  // End of enable_if implementation


  // Start of addressof function implementation to get lvalue address of an object with an overloaded '&' operator:
  template<class _Tp>
  typename enable_if<is_object<_Tp>::value, _Tp*>::type addressof(_Tp& arg) noexcept {
    return reinterpret_cast<_Tp*>( &const_cast<char&>( reinterpret_cast<const volatile char&>(arg) ) );
  }

  template<class _Tp>
  typename enable_if<!is_object<_Tp>::value, _Tp*>::type addressof(_Tp& arg) noexcept {
    return &arg;
  }
  // End of adressof implementation


  // Start of common_reference implementation: (no helper namespace to isolate the private functions since this implementation specializes other functions in this file)
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

  template<class _Tp, class _Up, template<class> class _TQual, template<class> class _UQual> 
  struct basic_common_reference { }; // May need to make a publicly accessible version of this meta-function

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
  
  template<class... _Tp>
  struct common_reference { }; // make publicly accessible. this is just a pre-declaration

  template<class... _Tp>
  using common_reference_t = typename common_reference<_Tp...>::type;

  template<> // if no variables are recieved the member "type" wont be defined
  struct common_reference<> { };

  template<class _Tp0>
  struct common_reference<_Tp0> { using type = _Tp0; };

  template<class _Tp1, class _Tp2, int _Bullet = 1, typename = void>
  struct __common_reference_impl : __common_reference_impl<_Tp1, _Tp2, _Bullet + 1> { };

  template<class _Tp1, class _Tp2>
  struct common_reference<_Tp1, _Tp2> : __common_reference_impl<_Tp1, _Tp2> { };

  template<class _Tp1, class _Tp2>
  struct __common_reference_impl<_Tp1&, _Tp2&, 1,
                                  void_t<__common_ref<_Tp1&, _Tp2&>>> {
    using type = __common_ref<_Tp1&, _Tp2&>;
  };

  template<class _Tp1, class _Tp2>
  struct __common_reference_impl<_Tp1&&, _Tp2&&, 1,
                                  void_t<__common_ref<_Tp1&&, _Tp2&&>>> {
    using type = __common_ref<_Tp1&&, _Tp2&&>;
  };

  template<class _Tp1, class _Tp2>
  struct __common_reference_impl<_Tp1&, _Tp2&&, 1,
                                  void_t<__common_ref<_Tp1&, _Tp2&&>>> {
    using type = __common_ref<_Tp1&, _Tp2&&>;
  };

  template<class _Tp1, class _Tp2>
  struct __common_reference_impl<_Tp1&&, _Tp2&, 1,
                                  void_t<__common_ref<_Tp1&&, _Tp2&>>> {
    using type = __common_ref<_Tp1&&, _Tp2&>;
  };

  template<class _Tp1, class _Tp2>
  struct __common_reference_impl<_Tp1, _Tp2, 2,
                                  void_t<__basic_common_ref<_Tp1, _Tp2>>> {
    using type = __basic_common_ref<_Tp1, _Tp2>;
  };

  template<class _Tp1, class _Tp2>
  struct __common_reference_impl<_Tp1, _Tp2, 3,
                                  void_t<__cond_res<_Tp1, _Tp2>>> {
    using type = __cond_res<_Tp1, _Tp2>;
  };

  template<class _Tp1, class _Tp2>
  struct __common_reference_impl<_Tp1, _Tp2, 4,
                                  void_t<common_type_t<_Tp1, _Tp2>>> {
    using type = common_type_t<_Tp1, _Tp2>;
  };

  template<class _Tp1, class _Tp2>
  struct __common_reference_impl<_Tp1, _Tp2, 5, void> { };

  template<class _Tp1, class _Tp2, class... _Rr>
  struct common_reference<_Tp1, _Tp2, _Rr...>
    : public __common_type_fold<common_reference<_Tp1, _Tp2>,
                                __common_type_pack<_Rr...>> { };

  template<class _Tp1, class _Tp2, class... _Rr>
  struct __common_type_fold<common_reference<_Tp1, _Tp2>,
                            __common_type_pack<_Rr...>,
                            void_t<common_reference_t<_Tp1, _Tp2>>>
    : public common_reference<common_reference_t<_Tp1, _Tp2>, _Rr...> { };
  // End of common_reference implementation


  // Start of is_swappable implementation:
  template<class _Tp>
  struct __is_swappable;

  template<class>
  struct __is_tuple_like_impl : public false_type { };

  template<class _Tp>
  struct __is_tuple_like : public __is_tuple_like_impl<remove_cvref_t<_Tp>>::type { };

  template<class _Tp>
  constexpr inline _Require<__not_<__is_tuple_like<_Tp>>,
                            is_move_constructible<_Tp>,
                            is_move_assignable<_Tp>>
  swap(_Tp&, _Tp&) noexcept(__and_<is_move_constructible<_Tp>, is_move_assignable<_Tp>>::value);

  template<class _Tp, size_t _Nm>
  constexpr inline enable_if_t<__is_swappable<_Tp>::value> 
                    swap(_Tp (&_a)[_Nm], _Tp (&_b)[_Nm]) noexcept(__is_swappable<_Tp>::value);

  namespace _swappable_helper { // start of private helper namespace

    struct __do_is_swappable_impl {
      template<class _Tp, class = decltype(swap(declval<_Tp&>(), declval<_Tp&>()))>
      static true_type __test(int);

      template<class>
      static false_type __test(...);
    };

    template<class _Tp>
    struct __is_swappable_impl : public __do_is_swappable_impl {
      using type = decltype(__test<_Tp>(0));
    };

    template<class _Tp>
    struct __is_swappable : public __is_swappable_impl<_Tp>::type { };

  } // end of private helper namespace for is_swappable implementation

  template<class _Tp>
  struct is_swappable : public _swappable_helper::__is_swappable_impl<_Tp>::type {
    static_assert(__is_complete_or_unbounded(__type_identity<_Tp>{}),
      "template argument must be a complete class or an unbounded array!");
  };

  template<class _Tp>
  inline constexpr bool is_swappable_v = is_swappable<_Tp>::value;

  namespace _swappable_helper { // start of is_swappable_with private helper namespace

    struct __do_is_swappable_with_impl {
      template<class _Tp, class _Up, 
        class = decltype(swap(declval<_Tp>(), declval<_Up>())),
        class = decltype(swap(declval<_Up>(), declval<_Tp>()))>
      static true_type __test(int);

      template<class, class>
      static false_type __test(...);
    };

    template<class _Tp, class _Up>
    struct __is_swappable_with_impl : public __do_is_swappable_with_impl {
      using type = decltype(__test<_Tp, _Up>(0));
    };

    template<class _Tp>
    struct __is_swappable_with_impl<_Tp&, _Tp&> : public __do_is_swappable_impl {
      using type = decltype(__test<_Tp&>(0));
    };

  } // End of is_swappable_with helper namespace

  template<class _Tp, class _Up>
  struct is_swappable_with : public _swappable_helper::__is_swappable_with_impl<_Tp, _Up>::type {
    static_assert(__is_complete_or_unbounded(__type_identity<_Tp>{}),
      "first template argument must be a complete class or an unbounded array!");
    static_assert(__is_complete_or_unbounded(__type_identity<_Up>{}),
      "second template argument must be a complete class or an unbounded array!");
  };

  template<class _Tp, class _Up>
  inline constexpr bool is_swappable_with_v = is_swappable_with<_Tp, _Up>::value;
  // End of is_swappable implementation


  // Start of equality_comparable and equality_comparable_with implementation: (not an extact remake of these concepts in libstdc++)
  template<class _Tp, class _Up, class = void>
  struct __weakly_equality_comparable_with : public false_type { };

  template<class _Tp, class _Up>
  struct __weakly_equality_comparable_with<_Tp, _Up, typename enable_if<__and_<
                                                                    is_same<bool, decltype(declval<_Tp>() == declval<_Up>())>,
                                                                    is_same<bool, decltype(declval<_Up>() == declval<_Tp>())>,
                                                                    is_same<bool, decltype(declval<_Tp>() != declval<_Up>())>,
                                                                    is_same<bool, decltype(declval<_Up>() != declval<_Tp>())> >::value,
                                                                    void>::type> : public true_type { };
  
  template<class _Tp>
  struct equality_comparable : public __weakly_equality_comparable_with<_Tp, _Tp> { };

  template<class _Tp>
  inline constexpr bool equality_comparable_v = equality_comparable<_Tp>::value;


  template<class _Tp, class _Up, class = void>
  struct equality_comparable_with : public false_type { };

  template<class _Tp, class _Up>
  struct equality_comparable_with<_Tp, _Up, typename enable_if<__and_<
                                                           equality_comparable<_Tp>,
                                                           equality_comparable<_Up>,
                                                           __weakly_equality_comparable_with<_Tp, _Up>>::value,
                                                           void>::type> : public true_type { };

  // End of equality_comparable and equality_comparable_with implementation

} // end of std:: namespace


#endif // End of Type_Traits.hpp
