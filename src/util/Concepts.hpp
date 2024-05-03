// Libstdc++ and GCC GNU Lincese boilerplate:      (this is required since this code is copyrighted under the GCC compiler. I did not design this myself.)
/*
  Partial Modified Implementation Of Meta-Functions from the <concepts> Libstdc++ header  -*- C++ -*-

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
  Concepts.h  (pseudo concept implementation file)
    By: Andrey Stroitelev

  ~This file is for implementing pseudo concepts along with requires to mimic setting constraints.
  ~Since the arduino IDE doesnt support requires and concepts which were added in c++17 I'm using meta-functions to mimic the same functionality
  ~Since these "concepts" are actually meta-functions they resolve to either std::true_type or std::false_type which you can check with either concept::type or concept::value with concept being any of the concepts defined in this file
    *::type will give std::true_type or std::false_type, and ::value will give true or false as a value
    *std::true_type is just std::integral_constant<bool, true>(or std::bool_constant<true>) and std::false_type is just std::integral_constant<bool, false>(or std::bool_constant<false>)
  ~With how these concepts are implemented you cannot use them as template argument restriction as you can with concepts in c++17
    *There is technically a way to achieve the same effect by using template overloads, but that ends up getting pretty convoluded by requiring helper meta-functions
*/
// NOTE: I will most likely get rid of this file as I'm moving towards only using c++11 and c++14(gcc-4.9.0) implementations.


// Start of Concepts.hpp
#ifndef _DLIB_CONCEPTS_HPP
#define _DLIB_CONCEPTS_HPP

#ifndef _DLIB_CONCEPTS_VERSION_
#define _DLIB_CONCEPTS_VERSION_ 0x010000
#endif


#include <Arduino.h>
#include "Type_Traits.hpp"
#include "UtilityBase.hpp"
#include "Invoke.hpp"
#include "Move.hpp"


_DLIB_HEADER


// Start of concepts:
namespace std {

  // Start of std::same_as implementation:
  template<class _Tp, class _Up>
  struct same_as : public __and_<is_same<_Tp, _Up>, is_same<_Up, _Tp>>::type { };
  
  namespace __detail { 

    template<class _Tp, class _Up>
    struct __different_from : public __not_<same_as<remove_cvref_t<_Tp>, remove_cvref_t<_Up>>>::type { };

  }
  // End of std::same_as


  // Start of std::derived_from implementation:
  template<class _Derived, class _Base>
  struct derived_from : public __and_<is_base_of<_Base, _Derived>,
                                      is_convertible<const volatile _Derived*, const volatile _Base*>
                                     >::type { };
  // End of std::derived_from


  // Start of std::convertible_to implementation:
  namespace __detail { // private implementation namespace

    template<class _From, class _To, class = void>
    struct __is_static_convertible_to : public false_type { };

    template<class _From, class _To>
    struct __is_static_convertible_to<_From, _To, void_t<decltype(static_cast<_To>(declval<_From>()))>> : public true_type { };

  } // end of __detail:: namespace

  template<class _From, class _To>
  struct convertible_to : public __and_<is_convertible<_From, _To>,
                                        __detail::__is_static_convertible_to<_From, _To>
                                       >::type { };
  // End of std::convertible_to


  // Start of std::common_reference_with implementation:
  template<class _Tp, class _Up>
  struct common_reference_with : public __and_<same_as<common_reference_t<_Tp, _Up>, common_reference_t<_Up, _Tp>>,
                                               convertible_to<_Tp, common_reference_t<_Tp, _Up>>,
                                               convertible_to<_Up, common_reference_t<_Tp, _Up>>
                                              >::type { };
  // End of std::common_reference_with


  // Start of std::common_with implementation:
  namespace __detail { // private namespace for helper meta-functions

    template<class _Tp, class _Up, class = void>
    struct __common_with_static_cast_helper : public false_type { };

    template<class _Tp, class _Up>
    struct __common_with_static_cast_helper<_Tp, _Up, void_t<decltype(static_cast<common_type_t<_Tp, _Up>>(declval<_Tp>())),
                                                             decltype(static_cast<common_type_t<_Tp, _Up>>(declval<_Up>()))
                                                            >> : public true_type { };

  } // end of __detail:: namespace

  template<class _Tp, class _Up>
  struct common_with : public __and_<same_as<common_type_t<_Tp, _Up>, common_type_t<_Up, _Tp>>,
                                     __detail::__common_with_static_cast_helper<_Tp, _Up>,
                                     common_reference_with<add_lvalue_reference_t<const _Tp>, add_lvalue_reference_t<const _Up>>,
                                     common_reference_with<add_lvalue_reference_t<common_type_t<_Tp, _Up>>,
                                                           common_reference_t<add_lvalue_reference_t<const _Tp>,
                                                                              add_lvalue_reference_t<const _Up>>>
                                    >::type { };
  // End of std::common_with


  // Start of std::assignable_from implementation:
  template<class _Lhs, class _Rhs> // might need to revise this
  struct assignable_from : public __and_<is_lvalue_reference<_Lhs>,
                                         common_reference_with<__detail::__cref<_Lhs>, __detail::__cref<_Rhs>>,
                                         same_as<decltype(declval<_Lhs>() = static_cast<_Rhs&&>(declval<_Rhs&&>())), _Lhs>
                                        >::type { };
  // End of std::assignable_from


  // Start of std::constructible_from implementation:
  // I might get rid of this since it does pretty much the same thing as std::is_constructible
  template<class _Tp, class... _Args>
  struct constructible_from : public __and_<is_destructible<_Tp>,
                                            is_constructible<_Tp, _Args...>
                                           >::type { };
  // End of std::constructible_from

  // not implementing the std::destructible concept since it would just call std::is_destructible which is pointless with how these are implemented

  // Start of std::default_initializable implementation:
  namespace __detail { // I need to test these ASAP before continuing

    template<class _Tp, class = void>
    struct __default_init_helper : public false_type { };

    template<class _Tp>
    struct __default_init_helper<_Tp, void_t<decltype(_Tp{}),
                                             decltype((void) ::delete ::new _Tp)> // this expression is originally {(void) ::new _Tp}, but I added the ::delete statement just to be safe.
                                > : public true_type { };

  }

  template<class _Tp>
  struct default_initializable : public __and_<constructible_from<_Tp>,
                                               __detail::__default_init_helper<_Tp>
                                              >::type { };
  // End of std::default_initializable


  // Start of std::move_constructible implementation: (I changed the implementation a bit)
  template<class _Tp>
  struct move_constructible : public __and_<is_move_constructible<_Tp>, // this check isn't in the STL implementation
                                            //constructible_from<_Tp, _Tp>, // I added the line above because I don't think this line will properly check for a move constructor with how std::is_constructible is implemented
                                            convertible_to<_Tp&&, _Tp>
                                           >::type { };
  // End of std::move_constructible


  // Start of std::copy_constructible implementation:
  template<class _Tp>
  struct copy_constructible : public __and_<move_constructible<_Tp>, // I might get rid of this check
                                            constructible_from<_Tp, _Tp&>, convertible_to<_Tp&, _Tp>,
                                            constructible_from<_Tp, const _Tp&>, convertible_to<const _Tp&, _Tp>,
                                            constructible_from<_Tp, const _Tp>, convertible_to<const _Tp, _Tp>
                                           >::type { };
  // End of std::copy_constructible


  // Start of std::swappable and std::swappable_with implementation:
  namespace ranges { // namespace for generalized swap implementation

    namespace __swap { // start of private custom swap implementation

      template<class _Tp>
      void swap(_Tp&, _Tp&) = delete; // ignores std::swap so the following implementation is used instead

      namespace __swap_detail { // private helper implementation namespace

        template<class _Tp, class _Up, class = void>
        struct __adl_swap_helper : public false_type { };

        template<class _Tp, class _Up>
        struct __adl_swap_helper<_Tp, _Up, void_t<decltype(swap(static_cast<_Tp&&>(declval<_Tp&&>()), static_cast<_Up&&>(declval<_Up&&>())))>> 
          : public true_type { };

      } // end of __swap_detail:: namespace

      template<class _Tp, class _Up>
      struct __adl_swap : public __and_<__or_<__detail::__class_or_enum<remove_reference_t<_Tp>>,
                                              __detail::__class_or_enum<remove_reference_t<_Up>>
                                             >,
                                        __swap_detail::__adl_swap_helper<_Tp, _Up>
                                       >::type { };

      struct _Swap {
        private:

          template<class _Tp, class _Up>
          static constexpr bool _S_noexcept() {
            if constexpr (__adl_swap<_Tp, _Up>::value)
              return noexcept(swap(declval<_Tp>(), declval<_Up>()));
            else
              return (is_nothrow_move_constructible<remove_reference_t<_Tp>>::value &&
                      is_nothrow_move_constructible<remove_reference_t<_Tp>>::value);
          }

          template<class _Tp, class _Up>
          struct __swap_require_helper : public __or_<__adl_swap<_Tp, _Up>,
                                                      __and_<same_as<_Tp, _Up>, is_lvalue_reference<_Tp>,
                                                             move_constructible<remove_reference_t<_Tp>>,
                                                             assignable_from<_Tp, remove_reference_t<_Tp>>
                                                            >
                                                     >::type { };

          template<class _Tp, class _Up, class = void>
          struct __arr_swap_require_helper : public false_type { };

          template<class _Tp, class _Up>
          struct __arr_swap_require_helper<_Tp, _Up, void_t<decltype(declval<const _Swap&>()(declval<_Tp&>(), declval<_Up&>()))>> 
            : public true_type { };
          /*
            The check above is equivalent to doing the following in c++17 and up:
            requires(const _Swap& __swap, _Tp& __e1, _Up& __e2) {
              __swap(__e1, __e2);
            }
          */

        public:
          
          template<class _Tp, class _Up>
          constexpr enable_if_t<__swap_require_helper<_Tp, _Up>::value> 
          operator()(_Tp&& __t, _Up&& __u) const noexcept(_S_noexcept<_Tp, _Up>()) {
            if constexpr (__adl_swap<_Tp, _Up>::value)
              swap(static_cast<_Tp&&>(__t), static_cast<_Up&&>(__u));
            else {
              auto __tmp = static_cast<remove_reference_t<_Tp>&&>(__t);
              __t = static_cast<remove_reference_t<_Tp>&&>(__u);
              __u = static_cast<remove_reference_t<_Tp>&&>(__tmp);
            }
          }

          template<class _Tp, class _Up, size_t _Nm>
          constexpr enable_if_t<__arr_swap_require_helper<_Tp, _Up>::value>
          operator()(_Tp (&__e1)[_Nm], _Up (&__e2)[_Nm]) const noexcept(noexcept(declval<const _Swap&>()(*__e1, *__e2))) {
            for(size_t __n = 0; __n < _Nm; ++__n)
              (*this)(__e1[__n], __e2[__n]);
          }

      }; // end of _Swap class

    } // end of private ranges::__swap:: namespace

    inline namespace _Cpo {
      inline constexpr __swap::_Swap swap{};
    }

  } // end of ranges:: namespace

  template<class _Tp, class = void>
  struct swappable : public false_type { };

  template<class _Tp>
  struct swappable<_Tp, void_t<decltype(ranges::swap(declval<_Tp&>(), declval<_Tp&>()))>>
    : public true_type { };

  namespace __detail { // namespace for private implementation

    template<class _Tp, class _Up, class = void>
    struct __swappable_with_concept_impl : public false_type { };

    template<class _Tp, class _Up>
    struct __swappable_with_concept_impl<_Tp, _Up,
    void_t<decltype(ranges::swap(static_cast<_Tp&&>(declval<_Tp&&>()), static_cast<_Tp&&>(declval<_Tp&&>()))),
           decltype(ranges::swap(static_cast<_Up&&>(declval<_Up&&>()), static_cast<_Up&&>(declval<_Up&&>()))),
           decltype(ranges::swap(static_cast<_Tp&&>(declval<_Tp&&>()), static_cast<_Up&&>(declval<_Up&&>()))),
           decltype(ranges::swap(static_cast<_Up&&>(declval<_Up&&>()), static_cast<_Tp&&>(declval<_Tp&&>())))
          >> : public true_type { };

  } // end of __detail:: namespace

  template<class _Tp, class _Up>
  struct swappable_with : public __and_<common_reference_with<_Tp, _Up>,
                                        __detail::__swappable_with_concept_impl<_Tp, _Up>
                                       >::type { };
  // End of std::swappable and std::swappable_with


  // Start of std::movable implementation:
  template<class _Tp>
  struct movable : public __and_<is_object<_Tp>,
                                 move_constructible<_Tp>,
                                 assignable_from<_Tp&, _Tp>,
                                 swappable<_Tp>
                                >::type { };
  // End of std::movable


  // Start of std::copyable implementation:
  template<class _Tp>
  struct copyable : public __and_<copy_constructible<_Tp>,
                                  movable<_Tp>,
                                  assignable_from<_Tp&, _Tp&>,
                                  assignable_from<_Tp&, const _Tp&>,
                                  assignable_from<_Tp&, const _Tp>
                                 >::type { };
  // End of std::copyable


  // Start of std::semiregular implementation: (I might not need this, and if that is the case I will delete this)
  template<class _Tp>
  struct semiregular : public __and_<copyable<_Tp>,
                                     default_initializable<_Tp>
                                    >::type { };
  // End of std::semiregular


  // Start of std::boolean_testable implementation: this meta-function is made public here, but is originaly private in the __detail:: namespace
  namespace __detail { // namespace for private implementation

    template<class _Tp>
    struct __boolean_testable_impl : public convertible_to<_Tp, bool>::type { };

  } // end of __detail:: namespace

  template<class _Tp>
  struct boolean_testable : public __and_<__detail::__boolean_testable_impl<_Tp>,
                                          __detail::__boolean_testable_impl<decltype((!static_cast<_Tp&&>(declval<_Tp&&>())))>
                                         >::type { };
  // End of std::boolean_testable


  // Start of __detail::__weakly_eq_cmp_with implementation:
  namespace __detail { // private implementation namespace

    template<class _Tp, class _Up>
    struct __weakly_eq_cmp_with : public __and_<boolean_testable<decltype(declval<__detail::__cref<_Tp>>() == declval<__detail::__cref<_Up>>())>,
                                                boolean_testable<decltype(declval<__detail::__cref<_Tp>>() != declval<__detail::__cref<_Up>>())>,
                                                boolean_testable<decltype(declval<__detail::__cref<_Up>>() == declval<__detail::__cref<_Tp>>())>,
                                                boolean_testable<decltype(declval<__detail::__cref<_Up>>() != declval<__detail::__cref<_Tp>>())>
                                               >::type { };

  } // end of __detail:: namespace
  // End of __detail::__weakly_eq_cmp_with


  // Start of std::equality_comparable and std::equality_comparable_with implementation:
  template<class _Tp>
  struct equality_comparable : public __detail::__weakly_eq_cmp_with<_Tp, _Tp>::type { };

  template<class _Tp, class _Up>
  struct equality_comparable_with : public __and_<equality_comparable<_Tp>,
                                                  equality_comparable<_Up>,
                                                  common_reference_with<__detail::__cref<_Tp>, __detail::__cref<_Up>>,
                                                  equality_comparable<common_reference_t<__detail::__cref<_Tp>, __detail::__cref<_Up>>>,
                                                  __detail::__weakly_eq_cmp_with<_Tp, _Up>
                                                 >::type { };
  // End of std::equality_comparable and std::equality_comparable_with


  // Start of __detail::__partially_ordered_with implementation:
  namespace __detail { // private implementation namespace

    template<class _Tp, class _Up>
    struct __partially_ordered_with : public bool_constant<
                                             __and_<boolean_testable<decltype(declval<const remove_reference_t<_Tp>&>() < declval<const remove_reference_t<_Up>&>())>,
                                                    boolean_testable<decltype(declval<const remove_reference_t<_Tp>&>() > declval<const remove_reference_t<_Up>&>())>,
                                                    boolean_testable<decltype(declval<const remove_reference_t<_Tp>&>() <= declval<const remove_reference_t<_Up>&>())>,
                                                    boolean_testable<decltype(declval<const remove_reference_t<_Tp>&>() >= declval<const remove_reference_t<_Up>&>())>,
                                                    boolean_testable<decltype(declval<const remove_reference_t<_Up>&>() < declval<const remove_reference_t<_Tp>&>())>,
                                                    boolean_testable<decltype(declval<const remove_reference_t<_Up>&>() > declval<const remove_reference_t<_Tp>&>())>,
                                                    boolean_testable<decltype(declval<const remove_reference_t<_Up>&>() <= declval<const remove_reference_t<_Tp>&>())>,
                                                    boolean_testable<decltype(declval<const remove_reference_t<_Up>&>() >= declval<const remove_reference_t<_Tp>&>())>
                                                   >::value>::type { };

  } // end of __detail:: namespace
  // End of __detail::__partially_ordered_with


  // Start of std::totally_ordered and std::totally_ordered_with implementation:
  template<class _Tp>
  struct totally_ordered : public bool_constant<
                                  __and_<equality_comparable<_Tp>,
                                         __detail::__partially_ordered_with<_Tp, _Tp>
                                        >::value>::type { };
  
  template<class _Tp, class _Up>
  struct totally_ordered_with : public bool_constant<
                                       __and_<totally_ordered<_Tp>,
                                              totally_ordered<_Up>,
                                              equality_comparable_with<_Tp, _Up>,
                                              totally_ordered<common_reference_t<__detail::__cref<_Tp>,
                                                                                 __detail::__cref<_Up>>>,
                                              __detail::__partially_ordered_with<_Tp, _Up>
                                             >::value>::type { };
  // End of std::totally_ordered and std::totally_ordered_with

  // Everything below this line I might not use and thus I might get rid of.

  // Start of std::regular implementation: (might delete if not used)
  template<class _Tp>
  struct regular : public bool_constant<
                          __and_<semiregular<_Tp>,
                                 equality_comparable<_Tp>
                                >::value>::type { };
  // End of std::regular


  // Start of std::invocable and std::regular_invocable implementation:
  namespace __detail { // private helper namespace for invocable

    struct __do_invocable_concept_impl {
      template<class _Fn, class... _Args,
               class = decltype(invoke(forward<_Fn>(declval<_Fn&&>()), forward<_Args>(declval<_Args&&>())...))>
      static true_type __test(int);

      template<class, class>
      static false_type __test(...);
    };

    template<class _Fn, class... _Args>
    struct __invocable_concept_impl : public __do_invocable_concept_impl {
      using type = decltype(__test<_Fn, _Args...>(0));
    };

  } // end of __detail:: namespace

  template<class _Fn, class... _Args>
  struct invocable // this meta-function works as intended
    : public __detail::__invocable_concept_impl<_Fn, _Args...>::type { };

  /*
    I have been unable to figure out how to implement std::regular_invocable.
    This function checks if a Functor is invocable with the given arguments, but in libstdc++
    they use the requires expression to check for equality preservation. On the page for this
    function on cppreference.com they state that the requires expression automatically checks for equality preservation,
    but I've seen people say that it's purely semantics so the function never actually checks if the function is equality preserving.

    Since I can't figure out how to make it check for equality preservation
    I am going to just go with using it as a semantic requirement. In this case the user can still pass
    a non equality preserving function to regular_invocable, but whatever function checks for regular_invocable will most likely
    throw an error since it relies on the functors and its parameters to stay unchanged.
    Since this is semantic it is also going to be difficult at times to diagnose errors caused when failing this requirement.
  */
  template<class _Fn, class... _Args> 
  struct regular_invocable : public invocable<_Fn, _Args...> { };
  // End of std::invocable and std::regular_invocable


  // Start of std::predicate implementation:
  template<class _Fn, class... _Args>
  struct predicate : public bool_constant<
                            __and_<regular_invocable<_Fn, _Args...>,
                                   boolean_testable<invoke_result_t<_Fn, _Args...>>
                                  >::value> { };
  // End of std::predicate


  // Start of std::relation and std::equivalence_relation implementation:
  template<class _Rel, class _Tp, class _Up>
  struct relation : public bool_constant<
                           __and_<predicate<_Rel, _Tp, _Tp>,
                                  predicate<_Rel, _Up, _Up>,
                                  predicate<_Rel, _Tp, _Up>,
                                  predicate<_Rel, _Up, _Tp>
                                 >::value> { };
  
  template<class _Rel, class _Tp, class _Up>
  struct equivalence_relation 
    : public bool_constant<relation<_Rel, _Tp, _Up>::value> { };
  // End of std::relation and std::equivalence_relation


  // Start of std::strict_weak_order implementation:
  template<class _Rel, class _Tp, class _Up>
  struct strict_weak_order : public bool_constant<relation<_Rel, _Tp, _Up>::value> { };
  // End of std::strict_weak_order

} // End of main std:: namespace


#endif // End of Concepts.hpp
