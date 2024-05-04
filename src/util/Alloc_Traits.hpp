// Libstdc++ and GCC GNU Lincese boilerplate:      (this is required since this code is copyrighted under the GCC compiler. I did not design this myself.)
/*
  Partial Modified Implementation Of Meta-Functions from the <bits/alloc_traits | new | bits/c++allocator | etc...> Libstdc++ header  -*- C++ -*-

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
  Alloc_Traits.hpp  [V1.0.0]  (template header file for allocator traits implementatio)
    By: Andrey Stroitelev

  ~This file is used to consolidate all of the allocator trait headers from libstdc++.
  ~These implementations are mainly just utilities to use with an allocator to simplify things.
*/


// Start of Alloc_Traits.hpp
#ifndef _DLIB_ALLOC_TRAITS_HPP_
#define _DLIB_ALLOC_TRAITS_HPP_

#ifndef _DLIB_ALLOC_TRAITS_VERSION_
#define _DLIB_ALLOC_TRAITS_VERSION_ 0x010000
#endif


#include <Arduino.h>
#include "Config.h"
#include "Allocator.hpp"
#include "Type_Traits.hpp"
#include "Move.hpp"


_DLIB_HEADER


// Start of libstdc++ bits/alloc_traits header implementation:

namespace std _DLIB_VISIBILITY {

  namespace __detail { // namespace for internal implementations

    template<class _Alloc, class _Tp>
    class __alloctr_rebind_helper {
      template<class _Alloc2, class _Tp2>
      static constexpr true_type _S_chk(typename _Alloc2::template rebind<_Tp2>::other*);

      template<class, class>
      static constexpr false_type _S_chk(...);

      public:
        using __type = decltype(_S_chk<_Alloc, _Tp>(nullptr));
    };

    template<class _Alloc, class _Tp,
             bool = __alloctr_rebind_helper<_Alloc, _Tp>::__type::value>
    struct __alloctr_rebind;

    template<class _Alloc, class _Tp>
    struct __alloctr_rebind<_Alloc, _Tp, true> {
      typedef typename _Alloc::template rebind<_Tp>::other __type;
    };

    template<template<class, class...> class _Alloc, class _Tp, class _Up, class... _Args>
    struct __alloctr_rebind<_Alloc<_Up, _Args...>, _Tp, false> {
      typedef _Alloc<_Tp, _Args...> __type;
    };

  } // end of __detail:: namespace


  template<class _Alloc>
  struct allocator_traits {
    typedef _Alloc allocator_type; // allocator type
    typedef typename _Alloc::value_type value_type; // allocated type

#   define _GLIBCXX_ALLOC_TR_NESTED_TYPE(_NTYPE, _ALT) \
    private: \
      template<class _Tp> \
      static typename _Tp::_NTYPE _S_##_NTYPE##_helper(_Tp*); \
      static _ALT _S_##_NTYPE##_helper(...); \
      typedef decltype(_S_##_NTYPE##_helper((_Alloc*)0)) __##_NTYPE; \
    public:

    _GLIBCXX_ALLOC_TR_NESTED_TYPE(pointer, value_type*)
      
      typedef __pointer pointer;
    
    _GLIBCXX_ALLOC_TR_NESTED_TYPE(const_pointer, typename pointer_traits<pointer>::template rebind<const value_type>)

      typedef __const_pointer const_pointer;

    _GLIBCXX_ALLOC_TR_NESTED_TYPE(void_pointer, typename pointer_traits<pointer>::template rebind<void>)

      typedef __void_pointer void_pointer;

    _GLIBCXX_ALLOC_TR_NESTED_TYPE(const_void_pointer, typename pointer_traits<pointer>::template rebind<const void>)

      typedef __const_void_pointer const_void_pointer;

    _GLIBCXX_ALLOC_TR_NESTED_TYPE(difference_type, typename pointer_traits<pointer>::difference_type)

      typedef __difference_type difference_type;

    _GLIBCXX_ALLOC_TR_NESTED_TYPE(size_type, typename make_unsigned<difference_type>::type)

      typedef __size_type size_type;

    _GLIBCXX_ALLOC_TR_NESTED_TYPE(propagate_on_container_copy_assignment, false_type)

      typedef __propagate_on_container_copy_assignment propagate_on_container_copy_assignment;

    _GLIBCXX_ALLOC_TR_NESTED_TYPE(propagate_on_container_move_assignment, false_type)

      typedef __propagate_on_container_move_assignment propagate_on_container_move_assignment;

    _GLIBCXX_ALLOC_TR_NESTED_TYPE(propagate_on_container_swap, false_type)

      typedef __propagate_on_container_swap propagate_on_container_swap;

#   undef _GLIBCXX_ALLOC_TR_NESTED_TYPE
    //public:
      template<class _Tp>
      using rebind_alloc = typename __detail::__alloctr_rebind<_Alloc, _Tp>::__type;

      template<class _Tp>
      using rebind_traits = allocator_traits<rebind_alloc<_Tp>>;

    private:
      template<class _Alloc2>
      struct __allocate_helper {
        template<class _Alloc3, class = decltype(declval<_Alloc3*>()->allocate(declval<size_type>(), declval<const_void_pointer>()))>
        static true_type __test(int);

        template<class>
        static false_type __test(...);

        using type = decltype(__test<_Alloc>(0));
      };

      template<class _Alloc2>
      using __has_allocate = typename __allocate_helper<_Alloc2>::type;

      template<class _Alloc2, class = _Require<__has_allocate<_Alloc2>>>
      static pointer _S_allocate(_Alloc2& __a, size_type __n, const_void_pointer __hint) {
        return __a.allocate(__n, __hint);
      }

      template<class _Alloc2, class _UnusedHint,
               class = _Require<__not_<__has_allocate<_Alloc2>>>>
      static pointer _S_allocate(_Alloc2& __a, size_type __n, _UnusedHint) {
        return __a.allocate(__n);
      }

      template<class _Tp, class... _Args>
      struct __construct_helper {
        template<class _Alloc2, class = decltype(declval<_Alloc2*>()->construct(declval<_Tp*>(), declval<_Args>()...))>
        static true_type __test(int);

        template<class>
        static false_type __test(...);

        using type = decltype(__test<_Alloc>(0));
      };

      template<class _Tp, class... _Args>
      using __has_construct = typename __construct_helper<_Tp, _Args...>::type;

      template<class _Tp, class... _Args>
      static _Require<__has_construct<_Tp, _Args...>>
      _S_construct(_Alloc& __a, _Tp* __p, _Args&&... __args) {
        __a.construct(__p, std::forward<_Args>(__args)...);
      }

      template<class _Tp, class... _Args>
      static _Require<__and_<__not_<__has_construct<_Tp, _Args...>>,
                             is_constructible<_Tp, _Args...>>>
      _S_construct(_Alloc&, _Tp* __p, _Args&&... __args) {
        ::new((void*)__p) _Tp(std::forward<_Args>(__args)...);
      }

      template<class _Tp>
      struct __destroy_helper {
        template<class _Alloc2, class = decltype(declval<_Alloc2*>()->destroy(declval<_Tp*>()))>
        static true_type __test(int);

        template<class>
        static false_type __test(...);

        using type = decltype(__test<_Alloc>(0));
      };

      template<class _Tp>
      using __has_destroy = typename __destroy_helper<_Tp>::type;

      template<class _Tp>
      static _Require<__has_destroy<_Tp>> _S_destroy(_Alloc& __a, _Tp* __p) {
        __a.destroy(__p);
      }

      template<class _Tp>
      static _Require<__not_<__has_destroy<_Tp>>> _S_destroy(_Alloc&, _Tp* __p) {
        __p->~_Tp();
      }

      template<class _Alloc2>
      struct __maxsize_helper {
        template<class _Alloc3, class = decltype(declval<_Alloc3*>()->max_size())>
        static true_type __test(int);

        template<class>
        static false_type __test(...);

        using type = decltype(__test<_Alloc2>(0));
      };

      template<class _Alloc2>
      using __has_max_size = typename __maxsize_helper<_Alloc2>::type;

      template<class _Alloc2, class = _Require<__has_max_size<_Alloc2>>>
      static size_type _S_max_size(_Alloc2& __a, int) {
        return __a.max_size();
      }

      template<class _Alloc2, class = _Require<__not_<__has_max_size<_Alloc2>>>>
      static size_type _S_max_size(_Alloc2&, ...) {
        return __gnu_cxx::__numeric_traits<size_type>::__max;
      }

      template<class _Alloc2>
      struct __select_helper {
        template<class _Alloc3, class = decltype(declval<_Alloc3*>()->select_on_container_copy_construction())>
        static true_type __test(int);

        template<class>
        static false_type __test(...);

        using type = decltype(__test<_Alloc2>(0));
      };

      template<class _Alloc2>
      using __has_soccc = typename __select_helper<_Alloc2>::type;

      template<class _Alloc2, class = _Require<__has_soccc<_Alloc2>>>
      static _Alloc2 _S_select(_Alloc2& __a, int) {
        return __a.select_on_container_copy_construction();
      }

      template<class _Alloc2, class = _Require<__not_<__has_soccc<_Alloc2>>>>
      static _Alloc2 _S_select(_Alloc2& __a, ...) { return __a; }

    public:
      static pointer allocate(_Alloc& __a, size_type __n) {
        return __a.allocate(__n);
      }

      static pointer allocate(_Alloc& __a, size_type __n, const_void_pointer __hint) {
        return _S_allocate(__a, __n, __hint);
      }

      static void deallocate(_Alloc& __a, pointer __p, size_type __n) {
        __a.deallocate(__p, __n);
      }

      template<class _Tp, class... _Args>
      static auto construct(_Alloc& __a, _Tp* __p, _Args&&... __args)
      -> decltype(_S_construct(__a, __p, std::forward<_Args>(__args)...)) {
        _S_construct(__a, __p, std::forward<_Args>(__args)...);
      }

      template<class _Tp>
      static void destroy(_Alloc& __a, _Tp* __p) {
        _S_destroy(__a, __p);
      }

      static size_type max_size(const _Alloc& __a) noexcept {
        return _S_max_size(__a, 0);
      }

      static _Alloc select_on_container_copy_construction(const _Alloc& __rhs) {
        return _S_select(__rhs, 0);
      }
  }; // end of allocator_traits class

  
  namespace __detail { // namespace for internal implementations

    template<class _Alloc>
    inline void __do_alloc_on_copy(_Alloc& __one, const _Alloc& __two, true_type) {
      __one = __two;
    }

    template<class _Alloc>
    inline void __do_alloc_on_copy(_Alloc&, const _Alloc&, false_type) { }

    template<class _Alloc>
    inline void __alloc_on_copy(_Alloc& __one, const _Alloc& __two) {
      typedef allocator_traits<_Alloc> __traits;
      typedef typename __traits::propagate_on_container_copy_assignment __pocca;
      __do_alloc_on_copy(__one, __two, __pocca());
    }

    template<class _Alloc>
    inline _Alloc __alloc_on_copy(const _Alloc& __a) {
      typedef allocator_traits<_Alloc> __traits;
      return __traits::select_on_container_copy_construction(__a);
    }

    template<class _Alloc>
    inline void __do_alloc_on_move(_Alloc& __one, _Alloc& __two, true_type) {
      __one = std::move(__two);
    }

    template<class _Alloc>
    inline void __do_alloc_on_move(_Alloc&, _Alloc&, false_type) { }

    template<class _Alloc>
    inline void __alloc_on_move(_Alloc& __one, _Alloc& __two) {
      typedef allocator_traits<_Alloc> __traits;
      typedef typename __traits::propagate_on_container_move_assignment __pocma;
      __do_alloc_on_move(__one, __two, __pocma());
    }

    template<class _Alloc>
    inline void __do_alloc_on_swap(_Alloc& __one, _Alloc& __two, true_type) {
      std::swap(__one, __two);
    }

    template<class _Alloc>
    inline void __do_alloc_on_swap(_Alloc&, _Alloc&, false_type) { }

    template<class _Alloc>
    inline void __alloc_on_swap(_Alloc& __one, _Alloc& __two) {
      typedef allocator_traits<_Alloc> __traits;
      typedef typename __traits::propagate_on_container_swap __pocs;
      __do_alloc_on_swap(__one, __two, __pocs());
    }

    template<class _Alloc>
    class __is_copy_insertable_impl {
      typedef allocator_traits<_Alloc> _Traits;

      template<class _Up, class = decltype(_Traits::construct(declval<_Alloc&>(),
                                                              declval<_Up*>(),
                                                              declval<const _Up&>()))>
      static true_type _M_select(int);

      template<class _Up>
      static false_type _M_select(...);

      public:
        typedef decltype(_M_select<typename _Alloc::value_type>(0)) type;
    };

    template<class _Alloc>
    struct __is_copy_insertable
      : __is_copy_insertable_impl<_Alloc>::type { };

    template<class _Tp>
    struct __is_copy_insertable<allocator<_Tp>>
      : is_copy_constructible<_Tp> { };

  } // end of __detail:: namespace

} // end of std:: namespace

// End of bits/alloc_traits header implementation


// Start of ext/alloc_traits libstdc++ header implementation:

// namespace for extended implementations
namespace __gnu_cxx _DLIB_VISIBILITY {

  template<class _Alloc>
  struct __allocator_always_compares_equal 
    : public std::false_type { };

  template<class _Tp>
  struct __allocator_always_compares_equal<std::allocator<_Tp>> 
    : public std::true_type { };

  template<class, class>
  struct array_allocator;

  template<class _Tp, class _Array>
  struct __allocator_always_compares_equal<array_allocator<_Tp, _Array>>
    : public std::true_type { };

  template<class>
  struct bitmap_allocator;

  template<class _Tp>
  struct __allocator_always_compares_equal<bitmap_allocator<_Tp>>
    : public std::true_type { };

  template<class>
  struct malloc_allocator;

  template<class _Tp>
  struct __allocator_always_compares_equal<malloc_allocator<_Tp>>
    : public std::true_type { };

  template<class>
  struct mt_allocator;

  template<class _Tp>
  struct __allocator_always_compares_equal<mt_allocator<_Tp>>
    : public std::true_type { };

  template<class>
  struct new_allocator;

  template<class _Tp>
  struct __allocator_always_compares_equal<new_allocator<_Tp>>
    : public std::true_type { };

  template<class>
  struct pool_allocator;

  template<class _Tp>
  struct __allocator_always_compares_equal<pool_allocator<_Tp>>
    : public std::true_type { };

  
  template<class _Alloc>
  struct __alloc_traits : public std::allocator_traits<_Alloc> {
    typedef _Alloc allocator_type;
    typedef std::allocator_traits<_Alloc> _Base_type;
    typedef typename _Base_type::value_type value_type;
    typedef typename _Base_type::pointer pointer;
    typedef typename _Base_type::const_pointer const_pointer;
    typedef typename _Base_type::size_type size_type;
    typedef typename _Base_type::difference_type difference_type;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    using _Base_type::allocate;
    using _Base_type::deallocate;
    using _Base_type::construct;
    using _Base_type::destroy;
    using _Base_type::max_size;

    private:
      template<class _Ptr>
      using __is_custom_pointer = std::__and_<std::is_same<pointer, _Ptr>,
                                              std::__not_<std::is_pointer<_Ptr>>>;
    
    public:
      template<class _Ptr, class... _Args>
      static typename std::enable_if<__is_custom_pointer<_Ptr>::value>::type
      construct(_Alloc& __a, _Ptr __p, _Args&&... __args) {
        _Base_type::construct(__a, std::addressof(*__p), std::forward<_Args>(__args)...);
      }

      template<class _Ptr>
      static typename std::enable_if<__is_custom_pointer<_Ptr>::value>::type
      destroy(_Alloc& __a, _Ptr __p) {
        _Base_type::destroy(__a, std::addressof(*__p));
      }

      static _Alloc _S_select_on_copy(const _Alloc& __a) {
        return _Base_type::select_on_container_copy_construction(__a);
      }

      static void _S_on_swap(_Alloc& __a, _Alloc& __b) {
        std::__detail::__alloc_on_swap(__a, __b);
      }

      static constexpr bool _S_propagate_on_copy_assign() {
        return _Base_type::propagate_on_container_copy_assignment::value;
      }

      static constexpr bool _S_propagate_on_move_assign() {
        return _Base_type::propagate_on_container_move_assignment::value;
      }

      static constexpr bool _S_propagate_on_swap() {
        return _Base_type::propagate_on_container_swap::value;
      }

      static constexpr bool _S_always_equal() {
        return __allocator_always_compares_equal<_Alloc>::value;
      }

      static constexpr bool _S_nothrow_move() {
        return _S_propagate_on_move_assign() || _S_always_equal();
      }

      static constexpr bool _S_nothrow_swap() {
        return !_S_propagate_on_swap()
            || noexcept(std::swap(std::declval<_Alloc&>(), std::declval<_Alloc&>()));
      }

      template<class _Tp>
      struct rebind {
        typedef typename _Base_type::template rebind_alloc<_Tp> other;
      };
  };

} // end of __gnu_cxx:: namespace

// End of ext/alloc_traits implementation


#endif // end of Alloc_Traits.hpp
