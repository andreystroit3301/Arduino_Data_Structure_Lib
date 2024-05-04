// Libstdc++ and GCC GNU Lincese boilerplate:      (this is required since this code is copyrighted under the GCC compiler. I did not design this myself.)
/*
  Partial Modified Implementation Of Meta-Functions from the <tuple> Libstdc++ header  -*- C++ -*-

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
  Tuple.hpp [V1.0.0] (Template Header file for Tuple Class Implementation)
    By: Andrey Stroitelev
  
  ~This file is based on the tuple implementation from libstdc++ gcc-4.9.0 which is the c++14 implementation
    *I originally attempted to use the current tuple implementation, but it required too many things I just couldn't implement without certain things only available in the newer c++ standards such as concepts and fold expressions.
  ~The tuple class works like a pair, but you can set the amount of elements it can hold.
    *a static sized array where you can select the type of each item in the array
    *example: std::tuple<String, int, float, char> student("joe", 15, 4.2, 'A'); // student joe along with his age, gpa, and average letter grade.
  ~The tuple is also a type of data structure, so some containers can be tuple-like such as std::array which means they can all be used with tuple algorithms
    *the tuple class is also used in some of the container class' implementations which ends up making them tuple-like.
*/


// Start of Tuple.hpp
#ifndef _DLIB_TUPLE_HPP_
#define _DLIB_TUPLE_HPP_

#ifndef _DLIB_TUPLE_VERSION_
#define _DLIB_TUPLE_VERSION_ 0x010000
#endif


#include <Arduino.h>
#include "Config.h"
#include "UtilityBase.hpp"
#include "Array.hpp"


_DLIB_HEADER


// Start of c++14 Tuple.hpp implementations:
namespace std _DLIB_VISIBILITY {

  // Start of tuple utilities:
  namespace __detail { // namespace for internal implementations

    template<class _Tp>
    struct __add_c_ref {
      typedef const _Tp& type;
    };

    template<class _Tp>
    struct __add_c_ref<_Tp&> {
      typedef _Tp& type;
    };

    template<class _Tp>
    struct __add_ref {
      typedef _Tp& type;
    };

    template<class _Tp>
    struct __add_ref<_Tp&> {
      typedef _Tp& type;
    };

    template<class _Tp>
    struct __add_r_ref {
      typedef _Tp&& type;
    };

    template<class _Tp>
    struct __add_r_ref<_Tp&> {
      typedef _Tp& type;
    };

    template<size_t _Idx, class _Head, bool _IsEmptyNotFinal>
    struct _Head_base;

    template<size_t _Idx, class _Head>
    struct _Head_base<_Idx, _Head, true> : public _Head {
      constexpr _Head_base() : _Head() { }

      constexpr _Head_base(const _Head& __h) : _Head(__h) { }

      template<class _UHead,
               class = typename enable_if<!is_convertible<_UHead, __uses_alloc_base>::value>::type>
      constexpr _Head_base(_UHead&& __h) : _Head(std::forward<_UHead>(__h)) { }

      _Head_base(__uses_alloc0) : _Head() { }

      template<class _Alloc>
      _Head_base(__uses_alloc1<_Alloc> __a) 
        : _Head(allocator_arg, *__a._M_a) { }

      template<class _Alloc>
      _Head_base(__uses_alloc2<_Alloc> __a) 
        : _Head(*__a._M_a) { }

      template<class _UHead>
      _Head_base(__uses_alloc0, _UHead&& __uhead) 
        : _Head(std::forward<_UHead>(__uhead)) { }

      template<class _Alloc, class _UHead>
      _Head_base(__uses_alloc1<_Alloc> __a, _UHead&& __uhead)
        : _Head(allocator_arg, *__a._M_a, std::forward<_UHead>(__uhead)) { }

      template<class _Alloc, class _UHead>
      _Head_base(__uses_alloc2<_Alloc> __a, _UHead&& __uhead)
        : _Head(std::forward<_UHead>(__uhead), *__a._M_a) { }

      static constexpr _Head& _M_head(_Head_base& __b) noexcept { return __b; }

      static constexpr const _Head& _M_head(const _Head_base& __b) noexcept { return __b; }
    }; // end of _Head_base class

    template<size_t _Idx, class _Head>
    struct _Head_base<_Idx, _Head, false> {
      constexpr _Head_base() : _M_head_impl() { }

      constexpr _Head_base(const _Head& __h) : _M_head_impl(__h) { }

      template<class _UHead,
               class = typename enable_if<!is_convertible<_UHead, __uses_alloc_base>::value>::type>
      constexpr _Head_base(_UHead&& __h) : _M_head_impl(std::forward<_UHead>(__h)) { }

      _Head_base(__uses_alloc0)
        : _M_head_impl() { }

      template<class _Alloc>
      _Head_base(__uses_alloc1<_Alloc> __a)
        : _M_head_impl(allocator_arg, *__a._M_a) { }

      template<class _Alloc>
      _Head_base(__uses_alloc2<_Alloc> __a)
        : _M_head_impl(*__a._M_a) { }

      template<class _UHead>
      _Head_base(__uses_alloc0, _UHead&& __uhead)
        : _M_head_impl(std::forward<_UHead>(__uhead)) { }

      template<class _Alloc, class _UHead>
      _Head_base(__uses_alloc1<_Alloc> __a, _UHead&& __uhead)
        : _M_head_impl(allocator_arg, *__a._M_a, std::forward<_UHead>(__uhead)) { }

      template<class _Alloc, class _UHead>
      _Head_base(__uses_alloc2<_Alloc> __a, _UHead&& __uhead)
        : _M_head_impl(std::forward<_UHead>(__uhead), *__a._M_a) { }

      static constexpr _Head& _M_head(_Head_base& __b) noexcept {
        return __b._M_head_impl;
      }

      static constexpr const _Head& _M_head(const _Head_base& __b) noexcept {
        return __b._M_head_impl;
      }

      _Head _M_head_impl;
    };

    
    // Start of the actual tuple implementation:
    /*
      This implementation is internal since it uses recursive inheritance.
      The public main std::tuple class just encapsulates these implementations.
      Using these implementations directly essentially would just be a huge pain in the ass for the user.
    */
    template<size_t _Idx, class... _Elements>
    struct _Tuple_impl; // base declaration of the template

    template<size_t _Idx>
    struct _Tuple_impl<_Idx> {
      template<size_t, class...>
      friend class _Tuple_impl;

      _Tuple_impl() = default;

      template<class _Alloc>
      _Tuple_impl(allocator_arg_t, const _Alloc&) { }

      template<class _Alloc>
      _Tuple_impl(allocator_arg_t, const _Alloc&, const _Tuple_impl&) { }

      template<class _Alloc>
      _Tuple_impl(allocator_arg_t, const _Alloc&, _Tuple_impl&&) { }

      protected:
        void _M_swap(_Tuple_impl&) noexcept { }
    };

    template<class _Tp>
    struct __is_empty_non_tuple : public is_empty<_Tp> { };

    template<class _E10, class... _E1>
    struct __is_empty_non_tuple<tuple<_E10, _E1...>> : public false_type { };

    template<class _Tp>
    using __empty_not_final = typename conditional<__is_final(_Tp), false_type, __is_empty_non_tuple<_Tp>>::type;

    // specialization for the recursive tuple implementation:
    template<size_t _Idx, class _Head, class... _Tail>
    struct _Tuple_impl<_Idx, _Head, _Tail...>
      : public _Tuple_impl<_Idx + 1, _Tail...>,
        private _Head_base<_Idx, _Head, __empty_not_final<_Head>::value> {
      
      template<size_t, class...>
      friend class _Tuple_impl;

      typedef _Tuple_impl<_Idx + 1, _Tail...> _Inherited;
      typedef _Head_base<_Idx, _Head, __empty_not_final<_Head>::value> _Base;

      static constexpr _Head& _M_head(_Tuple_impl& __t) noexcept {
        return _Base::_M_head(__t);
      }

      static constexpr const _Head& _M_head(const _Tuple_impl& __t) noexcept {
        return _Base::_M_head(__t);
      }

      static constexpr _Inherited& _M_tail(_Tuple_impl& __t) noexcept {
        return __t;
      }

      static constexpr const _Inherited& _M_tail(const _Tuple_impl& __t) noexcept {
        return __t;
      }

      constexpr _Tuple_impl() : _Inherited(), _Base() { }

      explicit constexpr _Tuple_impl(const _Head& __head, const _Tail&... __tail)
        : _Inherited(__tail...), _Base(__head) { }
      
      template<class _UHead, class... _UTail,
               class = typename enable_if<sizeof...(_Tail) == sizeof...(_UTail)>::type>
      explicit constexpr _Tuple_impl(_UHead&& __head, _UTail&&... __tail)
        : _Inherited(std::forward<_UTail>(__tail)...),
          _Base(std::forward<_UHead>(__head)) { }
      
      constexpr _Tuple_impl(const _Tuple_impl&) = default;

      constexpr _Tuple_impl(_Tuple_impl&& __in)
      noexcept(__and_<is_nothrow_move_constructible<_Head>, 
                      is_nothrow_move_constructible<_Inherited>>::value)
        : _Inherited(std::move(_M_tail(__in))),
          _Base(std::forward<_Head>(_M_head(__in))) { }
      
      template<class... _UElements>
      constexpr _Tuple_impl(const _Tuple_impl<_Idx, _UElements...>& __in)
        : _Inherited(_Tuple_impl<_Idx, _UElements...>::_M_tail(__in)),
          _Base(_Tuple_impl<_Idx, _UElements...>::_M_head(__in)) { }
      
      template<class _UHead, class... _UTails>
      constexpr _Tuple_impl(_Tuple_impl<_Idx, _UHead, _UTails...>&& __in)
        : _Inherited(std::move(_Tuple_impl<_Idx, _UHead, _UTails...>::_M_tail(__in))),
          _Base(std::forward<_UHead>(_Tuple_impl<_Idx, _UHead, _UTails...>::_M_head(__in))) { }

      template<class _Alloc>
      _Tuple_impl(allocator_arg_t __tag, const _Alloc& __a)
        : _Inherited(__tag, __a), _Base(__use_alloc<_Head>(__a)) { }
      
      template<class _Alloc>
      _Tuple_impl(allocator_arg_t __tag, const _Alloc& __a, const _Head& __head, const _Tail&... __tail)
        : _Inherited(__tag, __a, __tail...), _Base(__use_alloc<_Head, _Alloc, _Head>(__a), __head) { }
      
      template<class _Alloc, class _UHead, class... _UTail,
               class = typename enable_if<sizeof...(_Tail) == sizeof...(_UTail)>::type>
      _Tuple_impl(allocator_arg_t __tag, const _Alloc& __a, _UHead&& __head, _UTail&&... __tail)
        : _Inherited(__tag, __a, std::forward<_UTail>(__tail)...),
          _Base(__use_alloc<_Head, _Alloc, _UHead>(__a), std::forward<_UHead>(__head)) { }
      
      template<class _Alloc>
      _Tuple_impl(allocator_arg_t __tag, const _Alloc& __a, const _Tuple_impl& __in)
        : _Inherited(__tag, __a, _M_tail(__in)),
          _Base(__use_alloc<_Head, _Alloc, _Head>(__a), _M_head(__in)) { }

      template<class _Alloc>
      _Tuple_impl(allocator_arg_t __tag, const _Alloc& __a, _Tuple_impl&& __in)
        : _Inherited(__tag, __a, std::move(_M_tail(__in))),
          _Base(__use_alloc<_Head, _Alloc, _Head>(__a), std::forward<_Head>(_M_head(__in))) { }

      template<class _Alloc, class... _UElements>
      _Tuple_impl(allocator_arg_t __tag, const _Alloc& __a, const _Tuple_impl<_Idx, _UElements...>& __in)
        : _Inherited(__tag, __a, _Tuple_impl<_Idx, _UElements...>::_M_tail(__in)),
          _Base(__use_alloc<_Head, _Alloc, _Head>(__a), _Tuple_impl<_Idx, _UElements...>::_M_head(__in)) { }

      template<class _Alloc, class _UHead, class... _UTails>
      _Tuple_impl(allocator_arg_t __tag, const _Alloc& __a, _Tuple_impl<_Idx, _UHead, _UTails...>&& __in)
        : _Inherited(__tag, __a, std::move(_Tuple_impl<_Idx, _UHead, _UTails...>::_M_tail(__in))),
          _Base(__use_alloc<_Head, _Alloc, _UHead>(__a), std::forward<_UHead>(_Tuple_impl<_Idx, _UHead, _UTails...>::_M_head(__in))) { }
      
      _Tuple_impl& operator=(const _Tuple_impl& __in) {
        _M_head(*this) = _M_head(__in);
        _M_tail(*this) = _M_tail(__in);
        return *this;
      }

      _Tuple_impl& operator=(_Tuple_impl&& __in)
      noexcept(__and_<is_nothrow_move_assignable<_Head>, is_nothrow_move_assignable<_Inherited>>::value) {
        _M_head(*this) = std::forward<_Head>(_M_head(__in));
        _M_tail(*this) = std::move(_M_tail(__in));
        return *this;
      }

      template<class... _UElements>
      _Tuple_impl& operator=(const _Tuple_impl<_Idx, _UElements...>& __in) {
        _M_head(*this) = _Tuple_impl<_Idx, _UElements...>::_M_head(__in);
        _M_tail(*this) = _Tuple_impl<_Idx, _UElements...>::_M_tail(__in);
        return *this;
      }

      template<class _UHead, class... _UTails>
      _Tuple_impl& operator=(_Tuple_impl<_Idx, _UHead, _UTails...>&& __in) {
        _M_head(*this) = std::forward<_UHead>(_Tuple_impl<_Idx, _UHead, _UTails...>::_M_head(__in));
        _M_tail(*this) = std::move(_Tuple_impl<_Idx, _UHead, _UTails...>::_M_tail(__in));
        return *this;
      }

      protected:
        void _M_swap(_Tuple_impl& __in)
        noexcept(noexcept(swap(declval<_Head&>(), declval<_Head&>())) &&
                 noexcept(_M_tail(__in)._M_swap(_M_tail(__in)))) {
          //using std::swap;
          std::swap(_M_head(*this), _M_head(__in));
          _Inherited::_M_swap(_M_tail(__in));
        }
    }; // end of _Tuple_impl class
    // End of internal tuple implementation

  } // end of __detail:: namespace
  // End of tuple utilities


  // Start of primary tuple class implementation:
  /*
    This is the main implementation that encapsulates the internal implementation.
    The internal implementation just stores the elements in sections and this class chains them together kind of like a linked list.
    This also provides the standard functions and specializations for data structure classes
  */
  template<class... _Elements>
  class tuple : public __detail::_Tuple_impl<0, _Elements...> {
    // typedef for inherited internal tuple impl
    typedef __detail::_Tuple_impl<0, _Elements...> _Inherited;

    public:
      // Start of standard tuple constructors:
      constexpr tuple() : _Inherited() { }

      explicit constexpr tuple(const _Elements&... __elements)
        : _Inherited(__elements...) { }
      
      template<class... _UElements,
               class = typename enable_if<__and_<is_convertible<_UElements, _Elements>...>::value>::type>
      explicit constexpr tuple(_UElements&&... __elements)
        : _Inherited(std::forward<_UElements>(__elements)...) { }
      
      constexpr tuple(const tuple&) = default;
      constexpr tuple(tuple&&) = default;

      template<class... _UElements, 
               class = typename enable_if<__and_<is_convertible<const _UElements&, _Elements>...>::value>::type>
      constexpr tuple(const tuple<_UElements...>& __in)
        : _Inherited(static_cast<const __detail::_Tuple_impl<0, _UElements...>&>(__in)) { }
      
      template<class... _UElements,
               class = typename enable_if<__and_<is_convertible<_UElements, _Elements>...>::value>::type>
      constexpr tuple(tuple<_UElements...>&& __in)
        : _Inherited(static_cast<__detail::_Tuple_impl<0, _UElements...>&&>(__in)) { }
      // End of standard tuple constructors


      // Start of allocator extended constructors:
      template<class _Alloc>
      tuple(allocator_arg_t __tag, const _Alloc& __a)
        : _Inherited(__tag, __a) { }
      
      template<class _Alloc>
      tuple(allocator_arg_t __tag, const _Alloc& __a, const _Elements&... __elements)
        : _Inherited(__tag, __a, __elements...) { }

      template<class _Alloc, class... _UElements,
               class = typename enable_if<sizeof...(_UElements) == sizeof...(_Elements)>::type>
      tuple(allocator_arg_t __tag, const _Alloc& __a, _UElements&&... __elements)
        : _Inherited(__tag, __a, std::forward<_UElements>(__elements)...) { }
      
      template<class _Alloc>
      tuple(allocator_arg_t __tag, const _Alloc& __a, const tuple& __in)
        : _Inherited(__tag, __a, static_cast<const _Inherited&>(__in)) { }

      template<class _Alloc>
      tuple(allocator_arg_t __tag, const _Alloc& __a, tuple&& __in)
        : _Inherited(__tag, __a, static_cast<_Inherited&&>(__in)) { }
      
      template<class _Alloc, class... _UElements,
               class = typename enable_if<sizeof...(_UElements) == sizeof...(_Elements)>::type>
      tuple(allocator_arg_t __tag, const _Alloc& __a, const tuple<_UElements...>& __in)
        : _Inherited(__tag, __a, static_cast<const __detail::_Tuple_impl<0, _UElements...>&>(__in)) { }
      
      template<class _Alloc, class... _UElements,
               class = typename enable_if<sizeof...(_UElements) == sizeof...(_Elements)>::type>
      tuple(allocator_arg_t __tag, const _Alloc& __a, tuple<_UElements...>&& __in)
        : _Inherited(__tag, __a, static_cast<__detail::_Tuple_impl<0, _UElements...>&&>(__in)) { }
      // End of allocator constructors


      // Start of assignment operators:
      tuple& operator=(const tuple& __in) {
        static_cast<_Inherited&>(*this) = __in;
        return *this;
      }

      tuple& operator=(tuple&& __in) noexcept(is_nothrow_move_assignable<_Inherited>::value) {
        static_cast<_Inherited&>(*this) = std::move(__in);
        return *this;
      }

      template<class... _UElements,
               class = typename enable_if<sizeof...(_UElements) == sizeof...(_Elements)>::type>
      tuple& operator=(const tuple<_UElements...>& __in) {
        static_cast<_Inherited&>(*this) = __in;
        return *this;
      }

      template<class... _UElements,
               class = typename enable_if<sizeof...(_UElements) == sizeof...(_Elements)>::type>
      tuple& operator=(tuple<_UElements...>&& __in) {
        static_cast<_Inherited&>(*this) = std::move(__in);
        return *this;
      }
      // End of assignment operators
  }; // end of std::tuple class


  // zero element tuple specialization:
  template<>
  class tuple<> {
    public:
      void swap(tuple&) noexcept { }
  };


  template<class _T1, class _T2>
  class tuple<_T1, _T2> : public __detail::_Tuple_impl<0, _T1, _T2> {
    // typedef for inherited internal implementation
    typedef __detail::_Tuple_impl<0, _T1, _T2> _Inherited;

    public:
      // Start of standard constructors:
      constexpr tuple() : _Inherited() { }

      explicit constexpr tuple(const _T1& __a1, const _T2& __a2)
        : _Inherited(__a1, __a2) { }

      template<class _U1, class _U2,
               class = typename enable_if<__and_<is_convertible<_U1, _T1>, is_convertible<_U2, _T2>>::value>::type>
      explicit constexpr tuple(_U1&& __a1, _U2&& __a2)
        : _Inherited(std::forward<_U1>(__a1), std::forward<_U2>(__a2)) { }
      
      constexpr tuple(const tuple&) = default;
      constexpr tuple(tuple&&) = default;

      template<class _U1, class _U2,
               class = typename enable_if<__and_<is_convertible<const _U1&, _T1>, is_convertible<const _U2&, _T2>>::value>::type>
      constexpr tuple(const tuple<_U1, _U2>& __in)
        : _Inherited(static_cast<const __detail::_Tuple_impl<0, _U1, _U2>&>(__in)) { }

      template<class _U1, class _U2,
               class = typename enable_if<__and_<is_convertible<_U1, _T1>, is_convertible<_U2, _T2>>::value>::type>
      constexpr tuple(tuple<_U1, _U2>&& __in)
        : _Inherited(static_cast<__detail::_Tuple_impl<0, _U1, _U2>&&>(__in)) { }

      template<class _U1, class _U2,
               class = typename enable_if<__and_<is_convertible<const _U1&, _T1>, is_convertible<const _U2&, _T2>>::value>::type>
      constexpr tuple(const pair<_U1, _U2>& __in)
        : _Inherited(__in.first, __in.second) { }

      template<class _U1, class _U2,
               class = typename enable_if<__and_<is_convertible<_U1, _T1>, is_convertible<_U2, _T2>>::value>::type>
      constexpr tuple(pair<_U1, _U2>&& __in)
        : _Inherited(std::forward<_U1>(__in.first), std::forward<_U2>(__in.second)) { }
      // End of standard constructors


      // Start of allocator extended constructors:
      template<class _Alloc>
      tuple(allocator_arg_t __tag, const _Alloc& __a)
        : _Inherited(__tag, __a) { }

      template<class _Alloc>
      tuple(allocator_arg_t __tag, const _Alloc& __a, const _T1& __a1, const _T2& __a2)
        : _Inherited(__tag, __a, __a1, __a2) { }

      template<class _Alloc, class _U1, class _U2>
      tuple(allocator_arg_t __tag, const _Alloc& __a, _U1&& __a1, _U2&& __a2)
        : _Inherited(__tag, __a, std::forward<_U1>(__a1), std::forward<_U2>(__a2)) { }
      
      template<class _Alloc>
      tuple(allocator_arg_t __tag, const _Alloc& __a, const tuple& __in)
        : _Inherited(__tag, __a, static_cast<const _Inherited&>(__in)) { }
      
      template<class _Alloc>
      tuple(allocator_arg_t __tag, const _Alloc& __a, tuple&& __in)
        : _Inherited(__tag, __a, static_cast<_Inherited&&>(__in)) { }

      template<class _Alloc, class _U1, class _U2>
      tuple(allocator_arg_t __tag, const _Alloc& __a, const tuple<_U1, _U2>& __in)
        : _Inherited(__tag, __a, static_cast<const __detail::_Tuple_impl<0, _U1, _U2>&>(__in)) { }

      template<class _Alloc, class _U1, class _U2>
      tuple(allocator_arg_t __tag, const _Alloc& __a, tuple<_U1, _U2>&& __in)
        : _Inherited(__tag, __a, static_cast<__detail::_Tuple_impl<0, _U1, _U2>&&>(__in)) { }

      template<class _Alloc, class _U1, class _U2>
      tuple(allocator_arg_t __tag, const _Alloc& __a, const pair<_U1, _U2>& __in)
        : _Inherited(__tag, __a, __in.first, __in.second) { }

      template<class _Alloc, class _U1, class _U2>
      tuple(allocator_arg_t __tag, const _Alloc& __a, pair<_U1, _U2>&& __in)
        : _Inherited(__tag, __a, std::forward<_U1>(__in.first), std::forward<_U2>(__in.second)) { }
      // End of allocator constructors


      // Start of assignment operators:
      tuple& operator=(const tuple& __in) {
        static_cast<_Inherited&>(*this) = __in;
        return *this;
      }

      tuple& operator=(tuple&& __in) noexcept(is_nothrow_move_assignable<_Inherited>::value) {
        static_cast<_Inherited&>(*this) = std::move(__in);
        return *this;
      }

      template<class _U1, class _U2>
      tuple& operator=(const tuple<_U1, _U2>& __in) {
        static_cast<_Inherited&>(*this) = __in;
        return *this;
      }

      template<class _U1, class _U2>
      tuple& operator=(tuple<_U1, _U2>&& __in) {
        static_cast<_Inherited&>(*this) = std::move(__in);
        return *this;
      }

      template<class _U1, class _U2>
      tuple& operator=(const pair<_U1, _U2>& __in) {
        this->_M_head(*this) = __in.first;
        this->_M_tail(*this)._M_head(*this) = __in.second;
        return *this;
      }

      template<class _U1, class _U2>
      tuple& operator=(pair<_U1, _U2>&& __in) {
        this->_M_head(*this) = std::forward<_U1>(__in.first);
        this->_M_tail(*this)._M_head(*this) = std::forward<_U2>(__in.second);
        return *this;
      }
      // End of assignment operators


      // Swap algorithm implementation:
      void swap(tuple& __in) noexcept(noexcept(__in._M_swap(__in))) {
        _Inherited::_M_swap(__in);
      }
  }; // end of tuple class
  // End of std::tuple implementation


  // Start of tuple interface implementations:
  template<size_t __i, class _Tp>
  struct tuple_element;

  template<size_t __i, class _Head, class... _Tail>
  struct tuple_element<__i, tuple<_Head, _Tail...>>
    : public tuple_element<__i - 1, tuple<_Tail...>> { };

  template<class _Head, class... _Tail>
  struct tuple_element<0, tuple<_Head, _Tail...>> {
    typedef _Head type;
  };

  template<size_t __i, class _Tp>
  struct tuple_element<__i, const _Tp> {
    typedef typename add_const<typename tuple_element<__i, _Tp>::type>::type type;
  };

  template<size_t __i, class _Tp>
  struct tuple_element<__i, volatile _Tp> {
    typedef typename add_volatile<typename tuple_element<__i, _Tp>::type>::type type;
  };

  template<size_t __i, class _Tp>
  struct tuple_element<__i, const volatile _Tp> {
    typedef typename add_cv<typename tuple_element<__i, _Tp>::type>::type type;
  };

  template<class _Tp>
  struct tuple_size;

  template<class _Tp>
  struct tuple_size<const _Tp>
    : public integral_constant<typename remove_cv<decltype(tuple_size<_Tp>::value)>::type,
                               tuple_size<_Tp>::value> { };
  
  template<class _Tp>
  struct tuple_size<volatile _Tp>
    : public integral_constant<typename remove_cv<decltype(tuple_size<_Tp>::value)>::type,
                               tuple_size<_Tp>::value> { };

  template<class _Tp>
  struct tuple_size<const volatile _Tp>
    : public integral_constant<typename remove_cv<decltype(tuple_size<_Tp>::value)>::type,
                               tuple_size<_Tp>::value> { };

  template<class... _Elements>
  struct tuple_size<tuple<_Elements...>>
    : public integral_constant<size_t, sizeof...(_Elements)> { };
  
  namespace __detail { // namespace for internal implementations

    template<size_t __i, class _Head, class... _Tail>
    constexpr typename __add_ref<_Head>::type
    __get_helper(_Tuple_impl<__i, _Head, _Tail...>& __t) noexcept {
      return _Tuple_impl<__i, _Head, _Tail...>::_M_head(__t);
    }

    template<size_t __i, class _Head, class... _Tail>
    constexpr typename __add_c_ref<_Head>::type
    __get_helper(const _Tuple_impl<__i, _Head, _Tail...>& __t) noexcept {
      return _Tuple_impl<__i, _Head, _Tail...>::_M_head(__t);
    }

  } // end of __detail:: namespace

  template<size_t __i, class... _Elements>
  constexpr typename __detail::__add_ref<typename tuple_element<__i, tuple<_Elements...>>::type>::type
  get(tuple<_Elements...>& __t) noexcept {
    return __detail::__get_helper<__i>(__t);
  }

  template<size_t __i, class... _Elements>
  constexpr typename __detail::__add_c_ref<typename tuple_element<__i, tuple<_Elements...>>::type>::type
  get(const tuple<_Elements...>& __t) noexcept {
    return __detail::__get_helper<__i>(__t);
  }

  template<size_t __i, class... _Elements>
  constexpr typename __detail::__add_r_ref<typename tuple_element<__i, tuple<_Elements...>>::type>::type
  get(tuple<_Elements...>&& __t) noexcept {
    return std::forward<typename tuple_element<__i, tuple<_Elements...>>::type&&>(get<__i>(__t));
  }

  namespace __detail { // namespace for internal implementations:

    template<class _Head, size_t __i, class... _Tail>
    constexpr typename __add_ref<_Head>::type
    __get_helper2(_Tuple_impl<__i, _Head, _Tail...>& __t) noexcept {
      return _Tuple_impl<__i, _Head, _Tail...>::_M_head(__t);
    }

    template<class _Head, size_t __i, class... _Tail>
    constexpr typename __add_c_ref<_Head>::type
    __get_helper2(const _Tuple_impl<__i, _Head, _Tail...>& __t) noexcept {
      return _Tuple_impl<__i, _Head, _Tail...>::_M_head(__t);
    }

  } // end of __detail:: namespace

  template<class _Tp, class... _Types>
  constexpr _Tp& get(tuple<_Types...>& __t) noexcept {
    return __detail::__get_helper2<_Tp>(__t);
  }

  template<class _Tp, class... _Types>
  constexpr _Tp&& get(tuple<_Types...>&& __t) noexcept {
    return std::move(__detail::__get_helper2<_Tp>(__t));
  }

  template<class _Tp, class... _Types>
  constexpr const _Tp& get(const tuple<_Types...>& __t) noexcept {
    return __detail::__get_helper2<_Tp>(__t);
  }
  // End of tuple interface implementations


  // Start of tuple_compare implementation:
  namespace __detail { // namespace for internal implementation:

    template<size_t __check_equal_size, size_t __i, size_t __j, class _Tp, class _Up>
    struct __tuple_compare;

    template<size_t __i, size_t __j, class _Tp, class _Up>
    struct __tuple_compare<0, __i, __j, _Tp, _Up> {
      static constexpr bool __eq(const _Tp& __t, const _Up& __u) {
        return (get<__i>(__t) == get<__i>(__u) && __tuple_compare<0, __i + 1, __j, _Tp, _Up>::__eq(__t, __u));
      }

      static constexpr bool __less(const _Tp& __t, const _Up& __u) {
        return ((get<__i>(__t) < get<__i>(__u))
            || !(get<__i>(__u) < get<__i>(__t)) &&
                __tuple_compare<0, __i + 1, __j, _Tp, _Up>::__less(__t, __u));
      }
    };

    template<size_t __i, class _Tp, class _Up>
    struct __tuple_compare<0, __i, __i, _Tp, _Up> {
      static constexpr bool __eq(const _Tp&, const _Up&) { return true; }

      static constexpr bool __less(const _Tp&, const _Up&) { return false; }
    };

  } // end of __detail:: namespace
  // End of tuple_compare implementation


  // Start of std::tuple comparison operators:
  template<class... _TElements, class... _UElements>
  constexpr bool operator==(const tuple<_TElements...>& __t, const tuple<_UElements...>& __u) {
    typedef tuple<_TElements...> _Tp;
    typedef tuple<_UElements...> _Up;
    return bool(__detail::__tuple_compare<tuple_size<_Tp>::value - tuple_size<_Up>::value,
                                          0, tuple_size<_Tp>::value, _Tp, _Up>::__eq(__t, __u));
  }

  template<class... _TElements, class... _UElements>
  constexpr bool operator<(const tuple<_TElements...>& __t, const tuple<_UElements...>& __u) {
    typedef tuple<_TElements...> _Tp;
    typedef tuple<_UElements...> _Up;
    return bool(__detail::__tuple_compare<tuple_size<_Tp>::value - tuple_size<_Up>::value,
                                          0, tuple_size<_Tp>::value, _Tp, _Up>::__less(__t, __u));
  }

  template<class... _TElements, class... _UElements>
  constexpr bool operator!=(const tuple<_TElements...>& __t, const tuple<_UElements...>& __u) {
    return !(__t == __u);
  }

  template<class... _TElements, class... _UElements>
  constexpr bool operator>(const tuple<_TElements...>& __t, const tuple<_UElements...>& __u) {
    return __u < __t;
  }

  template<class... _TElements, class... _UElements>
  constexpr bool operator<=(const tuple<_TElements...>& __t, const tuple<_UElements...>& __u) {
    return !(__u < __t);
  }

  template<class... _TElements, class... _UElements>
  constexpr bool operator>=(const tuple<_TElements...>& __t, const tuple<_UElements...>& __u) {
    return !(__t < __u);
  }
  // End of std::tuple comparison operators


  // Start of tuple utilities and algorithms:
  template<class... _Elements>
  constexpr tuple<typename __detail::__decay_and_strip<_Elements>::__type...>
  make_tuple(_Elements&&... __args) {
    typedef tuple<typename __detail::__decay_and_strip<_Elements>::__type...> __result_type;
    return __result_type(std::forward<_Elements>(__args)...);
  }

  template<class... _Elements>
  tuple<_Elements&&...> forward_as_tuple(_Elements&&... __args) noexcept {
    return tuple<_Elements&&...>(std::forward<_Elements>(__args)...);
  }

  namespace __detail { // namespace for internal implementations

    // already defined in Type_Traits.hpp since I used a newer implementation of type_traits
    //template<class>
    //struct __is_tuple_like_impl : public false_type { };

    template<class... _Tps>
    struct __is_tuple_like_impl<tuple<_Tps...>> : public true_type { };

    template<class _T1, class _T2>
    struct __is_tuple_like_impl<pair<_T1, _T2>> : public true_type { };

    // This was also already specialized in array.hpp
    //template<class _Tp, size_t _Nm>
    //struct __is_tuple_like_impl<array<_Tp, _Nm>> : public true_type { };

    // already defined in the newer implementation of type_traits that I used
    //template<class _Tp>
    //struct __is_tuple_like
    //  : public __is_tuple_like_impl<typename remove_cv<typename remove_reference<_Tp>::type>::type>::type { };

    template<size_t, class, class, size_t>
    struct __make_tuple_impl;

    template<size_t _Idx, class _Tuple, class... _Tp, size_t _Nm>
    struct __make_tuple_impl<_Idx, tuple<_Tp...>, _Tuple, _Nm> {
      typedef typename __make_tuple_impl<_Idx + 1, tuple<_Tp..., typename tuple_element<_Idx, _Tuple>::type>, _Tuple, _Nm>::__type __type;
    };

    template<size_t _Nm, class _Tuple, class... _Tp>
    struct __make_tuple_impl<_Nm, tuple<_Tp...>, _Tuple, _Nm> {
      typedef tuple<_Tp...> __type;
    };

    template<class _Tuple>
    struct __do_make_tuple
      : public __make_tuple_impl<0, tuple<>, _Tuple, tuple_size<_Tuple>::value> { };

    template<class _Tuple>
    struct __make_tuple
      : public __do_make_tuple<typename remove_cv<typename remove_reference<_Tuple>::type>::type> { };

    template<class...>
    struct __combine_tuples;

    template<>
    struct __combine_tuples<> {
      typedef tuple<> __type;
    };

    template<class... _Ts>
    struct __combine_tuples<tuple<_Ts...>> {
      typedef tuple<_Ts...> __type;
    };

    template<class... _T1s, class... _T2s, class... _Rem>
    struct __combine_tuples<tuple<_T1s...>, tuple<_T2s...>, _Rem...> {
      typedef typename __combine_tuples<tuple<_T1s..., _T2s...>, _Rem...>::__type __type;
    };

    template<class... _Tpls>
    struct __tuple_cat_result {
      typedef typename __combine_tuples<typename __make_tuple<_Tpls>::__type...>::__type __type;
    };

    template<class...>
    struct __make_1st_indices;

    template<>
    struct __make_1st_indices<> {
      typedef _Index_tuple<> __type;
    };

    template<class _Tp, class... _Tpls>
    struct __make_1st_indices<_Tp, _Tpls...> {
      typedef typename _Build_index_tuple<tuple_size<typename remove_reference<_Tp>::type>::value>::__type __type;
    };

    template<class _Ret, class _Indices, class... _Tpls>
    struct __tuple_concater;

    template<class _Ret, size_t... _Is, class _Tp, class... _Tpls>
    struct __tuple_concater<_Ret, _Index_tuple<_Is...>, _Tp, _Tpls...> {
      template<class... _Us>
      static constexpr _Ret _S_do(_Tp&& __tp, _Tpls&&... __tps, _Us&&... __us) {
        typedef typename __make_1st_indices<_Tpls...>::__type __idx;
        typedef __tuple_concater<_Ret, __idx, _Tpls...> __next;
        return __next::_S_do(std::forward<_Tpls>(__tps)..., std::forward<_Us>(__us)..., get<_Is>(std::forward<_Tp>(__tp))...);
      }
    };

    template<class _Ret>
    struct __tuple_concater<_Ret, _Index_tuple<>> {
      template<class... _Us>
      static constexpr _Ret _S_do(_Us&&... __us) {
        return _Ret(std::forward<_Us>(__us)...);
      }
    };

  } // end of __detail:: namespace

  template<class... _Tpls,
           class = typename enable_if<__and_<__detail::__is_tuple_like<_Tpls>...>::value>::type>
  constexpr auto tuple_cat(_Tpls&&... __tpls) -> typename __detail::__tuple_cat_result<_Tpls...>::__type {
    typedef typename __detail::__tuple_cat_result<_Tpls...>::__type __ret;
    typedef typename __detail::__make_1st_indices<_Tpls...>::__type __idx;
    typedef __detail::__tuple_concater<__ret, __idx, _Tpls...> __concater;
    return __concater::_S_do(std::forward<_Tpls>(__tpls)...);
  }

  template<class... _Elements>
  inline tuple<_Elements&...> tie(_Elements&... __args) noexcept {
    return tuple<_Elements&...>(__args...);
  }

  template<class... _Elements>
  inline void swap(tuple<_Elements...>& __x, tuple<_Elements...>& __y)
  noexcept(noexcept(__x.swap(__y))) {
    __x.swap(__y);
  }

  namespace __detail { // namespace for internal implementations

    struct _Swallow_assign {
      template<class _Tp>
      const _Swallow_assign& operator=(const _Tp&) const {
        return *this;
      }
    };

    const _Swallow_assign ignore{};

  } // end of __detail:: namespace

  template<class... _Types, class _Alloc>
  struct uses_allocator<tuple<_Types...>, _Alloc> : public true_type { };

  template<class _T1, class _T2>
  template<class... _Args1, class... _Args2>
  inline pair<_T1, _T2>::pair(piecewise_construct_t, tuple<_Args1...> __first, tuple<_Args2...> __second)
    : pair(__first, __second,
           typename __detail::_Build_index_tuple<sizeof...(_Args1)>::__type(),
           typename __detail::_Build_index_tuple<sizeof...(_Args2)>::__type()) { }

  template<class _T1, class _T2>
  template<class... _Args1, size_t... _Indexes1, class... _Args2, size_t... _Indexes2>
  inline pair<_T1, _T2>::pair(tuple<_Args1...>& __tuple1, tuple<_Args2...>& __tuple2,
                              __detail::_Index_tuple<_Indexes1...>, __detail::_Index_tuple<_Indexes2...>)
    : first(std::forward<_Args1>(get<_Indexes1>(__tuple1))...),
      second(std::forward<_Args2>(get<_Indexes2>(__tuple2))...) { }
  // End of tuple utilities and algorithms

} // end of std:: namespace


#endif // End of Tuple.hpp
