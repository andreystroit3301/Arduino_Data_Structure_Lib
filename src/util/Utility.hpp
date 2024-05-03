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
*/
/*
  Copyright (c) 1994
  Hewlett-Packard Company

  Permission to use, copy, modify, distribute and sell this software
  and its documentation for any purpose is hereby granted without fee,
  provided that the above copyright notice appear in all copies and
  that both that copyright notice and this permission notice appear
  in supporting documentation.  Hewlett-Packard Company makes no
  representations about the suitability of this software for any
  purpose.  It is provided "as is" without express or implied warranty.


  Copyright (c) 1996,1997
  Silicon Graphics Computer Systems, Inc.

  Permission to use, copy, modify, distribute and sell this software
  and its documentation for any purpose is hereby granted without fee,
  provided that the above copyright notice appear in all copies and
  that both that copyright notice and this permission notice appear
  in supporting documentation.  Silicon Graphics makes no
  representations about the suitability of this software for any
  purpose.  It is provided "as is" without express or implied warranty.
*/
/*
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
  Utility.hpp  [V1.0.0]  (template header file for general utilities)
    By: Andrey Stroitelev

  ~This file is not a copy of the libstdc++ utility header as that is implemented in the UtilityBase.hpp file.
  ~This header consolidates some miscalaneous headers that are essential, but I couldn't think of another file to put them in.
  ~This file contains the implementations of the bits/refwrap, functional, bits/stl_uninitialized, and bits/stl_construct headers from libstdc++.
*/


// Start of Utility.hpp
#ifndef _DLIB_UTILITY_HPP_
#define _DLIB_UTILITY_HPP_

#ifndef _DLIB_UTILITY_VERSION_
#define _DLIB_UTILITY_VERSION_ 0x010000
#endif


#include <Arduino.h>
#include "Config.h"
#include "UtilityBase.hpp"
#include "Invoke.hpp"
#include "Move.hpp"
#include "Alloc_Traits.hpp"
#include "Allocator.hpp"


_DLIB_HEADER


// Start of implementations from the bits/refwrap header in libstdc++:

namespace std _DLIB_VISIBILITY {

  namespace __detail { // namespace for internal implementations

    template<class _Res, class... _ArgTypes>
    struct _Maybe_unary_or_binary_function { };

_USE_DEPRECATED

    template<class _Res, class _T1>
    struct _Maybe_unary_or_binary_function<_Res, _T1>
      : public unary_function<_T1, _Res> { };

    template<class _Res, class _T1, class _T2>
    struct _Maybe_unary_or_binary_function<_Res, _T1, _T2>
      : public binary_function<_T1, _T2, _Res> { };

_END_DEPRECATED

    template<class _Signature>
    struct _Mem_fn_traits;

    template<class _Res, class _Class, class... _ArgTypes>
    struct _Mem_fn_traits_base {
      using __result_type = _Res;
      using __maybe_type = _Maybe_unary_or_binary_function<_Res, _Class*, _ArgTypes...>;
      using __arity = integral_constant<size_t, sizeof...(_ArgTypes)>;
    };

#   define _GLIBCXX_MEM_FN_TRAITS2(_CV, _REF, _LVAL, _RVAL)            \
    template<class _Res, class _Class, class... _ArgTypes>             \
    struct _Mem_fn_traits<_Res (_Class::*)(_ArgTypes...) _CV _REF>     \
    : public _Mem_fn_traits_base<_Res, _CV _Class, _ArgTypes...> {     \
      using __vararg = false_type;                                     \
    };                                                                 \
    template<class _Res, class _Class, class... _ArgTypes>             \
    struct _Mem_fn_traits<_Res (_Class::*)(_ArgTypes... ...) _CV _REF> \
    : public _Mem_fn_traits_base<_Res, _CV _Class, _ArgTypes...> {     \
      using __vararg = true_type;                                      \
    };

#   define _GLIBCXX_MEM_FN_TRAITS(_REF, _LVAL, _RVAL) \
    _GLIBCXX_MEM_FN_TRAITS2(              , _REF, _LVAL, _RVAL) \
    _GLIBCXX_MEM_FN_TRAITS2(const         , _REF, _LVAL, _RVAL) \
    _GLIBCXX_MEM_FN_TRAITS2(volatile      , _REF, _LVAL, _RVAL) \
    _GLIBCXX_MEM_FN_TRAITS2(const volatile, _REF, _LVAL, _RVAL)

    _GLIBCXX_MEM_FN_TRAITS(, true_type, true_type)
    _GLIBCXX_MEM_FN_TRAITS(&, true_type, false_type)
    _GLIBCXX_MEM_FN_TRAITS(&&, false_type, true_type)

#   undef _GLIBCXX_MEM_FN_TRAITS
#   undef _GLIBCXX_MEM_FN_TRAITS2

#   define _NOEXCEPT_PARM //, bool _NE
#   define _NOEXCEPT_QUAL //noexcept(_NE)

    template<class _Functor, class = void_t<>>
    struct _Maybe_get_result_type { };

    template<class _Functor>
    struct _Maybe_get_result_type<_Functor, void_t<typename _Functor::result_type>> {
      typedef typename _Functor::result_type result_type;
    };

    template<class _Functor>
    struct _Weak_result_type_impl 
      : public _Maybe_get_result_type<_Functor> { };

    template<class _Res, class... _ArgTypes _NOEXCEPT_PARM>
    struct _Weak_result_type_impl<_Res(_ArgTypes...) _NOEXCEPT_QUAL> {
      typedef _Res result_type;
    };

    template<class _Res, class... _ArgTypes _NOEXCEPT_PARM>
    struct _Weak_result_type_impl<_Res(_ArgTypes......) _NOEXCEPT_QUAL> {
      typedef _Res result_type;
    };

    template<class _Res, class... _ArgTypes _NOEXCEPT_PARM>
    struct _Weak_result_type_impl<_Res(*)(_ArgTypes...) _NOEXCEPT_QUAL> {
      typedef _Res result_type;
    };

    template<class _Res, class... _ArgTypes _NOEXCEPT_PARM>
    struct _Weak_result_type_impl<_Res(*)(_ArgTypes......) _NOEXCEPT_QUAL> {
      typedef _Res result_type;
    };

    template<class _Functor, bool = is_member_function_pointer<_Functor>::value>
    struct _Weak_result_type_memfun : public _Weak_result_type_impl<_Functor> { };

    template<class _MemFunPtr>
    struct _Weak_result_type_memfun<_MemFunPtr, true> {
      using result_type = typename _Mem_fn_traits<_MemFunPtr>::__result_type;
    };

    template<class _Func, class _Class>
    struct _Weak_result_type_memfun<_Func _Class::*, false> { };

    template<class _Functor>
    struct _Weak_result_type
      : public _Weak_result_type_memfun<typename remove_cv<_Functor>::type> { };

    template<class _Tp, class = void_t<>>
    struct _Refwrap_base_arg1 { };

    template<class _Tp>
    struct _Refwrap_base_arg1<_Tp, void_t<typename _Tp::argument_type>> {
      typedef typename _Tp::argument_type argument_type;
    };

    template<class _Tp, class = void_t<>>
    struct _Refwrap_base_arg2 { };

    template<class _Tp>
    struct _Refwrap_base_arg2<_Tp, void_t<typename _Tp::first_argument_type,
                                          typename _Tp::second_argument_type>> {
      typedef typename _Tp::first_argument_type first_argument_type;
      typedef typename _Tp::second_argument_type second_argument_type;
    };

    template<class _Tp>
    struct _Reference_wrapper_base
      : public _Weak_result_type<_Tp>,
        public _Refwrap_base_arg1<_Tp>,
        public _Refwrap_base_arg2<_Tp> { };

_USE_DEPRECATED

    template<class _Res, class _T1 _NOEXCEPT_PARM>
    struct _Reference_wrapper_base<_Res(_T1) _NOEXCEPT_QUAL>
      : public unary_function<_T1, _Res> { };

    template<class _Res, class _T1>
    struct _Reference_wrapper_base<_Res(_T1) const>
      : public unary_function<_T1, _Res> { };

    template<class _Res, class _T1>
    struct _Reference_wrapper_base<_Res(_T1) volatile>
      : public unary_function<_T1, _Res> { };

    template<class _Res, class _T1>
    struct _Reference_wrapper_base<_Res(_T1) const volatile>
      : public unary_function<_T1, _Res> { };

    template<class _Res, class _T1, class _T2 _NOEXCEPT_PARM>
    struct _Reference_wrapper_base<_Res(_T1, _T2) _NOEXCEPT_QUAL>
      : public binary_function<_T1, _T2, _Res> { };
    
    template<class _Res, class _T1, class _T2>
    struct _Reference_wrapper_base<_Res(_T1, _T2) const>
      : public binary_function<_T1, _T2, _Res> { };

    template<class _Res, class _T1, class _T2>
    struct _Reference_wrapper_base<_Res(_T1, _T2) volatile>
      : public binary_function<_T1, _T2, _Res> { };

    template<class _Res, class _T1, class _T2>
    struct _Reference_wrapper_base<_Res(_T1, _T2) const volatile>
      : public binary_function<_T1, _T2, _Res> { };

    template<class _Res, class _T1 _NOEXCEPT_PARM>
    struct _Reference_wrapper_base<_Res(*)(_T1) _NOEXCEPT_QUAL>
      : public unary_function<_T1, _Res> { };
    
    template<class _Res, class _T1, class _T2 _NOEXCEPT_PARM>
    struct _Reference_wrapper_base<_Res(*)(_T1, _T2) _NOEXCEPT_QUAL>
      : public binary_function<_T1, _T2, _Res> { };

    template<class _Tp, bool = is_member_function_pointer<_Tp>::value>
    struct _Reference_wrapper_base_memfun
      : public _Reference_wrapper_base<_Tp> { };

    template<class _MemFunPtr>
    struct _Reference_wrapper_base_memfun<_MemFunPtr, true>
      : public _Mem_fn_traits<_MemFunPtr>::__maybe_type {
      using result_type = typename _Mem_fn_traits<_MemFunPtr>::__result_type;
    };

_END_DEPRECATED

  } // end of __detail:: namespace


  template<class _Tp>
  class reference_wrapper
    : public __detail::_Reference_wrapper_base_memfun<typename remove_cv<_Tp>::type> {
    _Tp* _M_data;

    constexpr static _Tp* _S_fun(_Tp& __r) noexcept { return __detail::__addressof(__r); }

    static void _S_fun(_Tp&&) = delete;

    template<class _Up, class _Up2 = remove_cvref_t<_Up>>
    using __not_same = typename enable_if<!is_same<reference_wrapper, _Up2>::value>::type;

    public:
      typedef _Tp type;

      template<class _Up, class = __not_same<_Up>,
               class = decltype(reference_wrapper::_S_fun(declval<_Up>()))>
      constexpr reference_wrapper(_Up&& __uref)
      noexcept(noexcept(reference_wrapper::_S_fun(declval<_Up>())))
        : _M_data(reference_wrapper::_S_fun(std::forward<_Up>(__uref))) { }

      reference_wrapper(const reference_wrapper&) = default;

      reference_wrapper& operator=(const reference_wrapper&) = default;

      constexpr operator _Tp&() const noexcept { return this->get(); }

      constexpr _Tp& get() const noexcept { return *_M_data; }

      template<class... _Args>
      constexpr typename __detail::__invoke_result<_Tp&, _Args...>::type
      operator()(_Args&&... __args) const noexcept(is_nothrow_invocable<_Tp&, _Args...>::value) {
        return __detail::__invoke(get(), std::forward<_Args>(__args)...);
      }
  }; // end of reference_wrapper class


  template<class _Tp>
  constexpr inline reference_wrapper<_Tp> ref(_Tp& __t) noexcept {
    return reference_wrapper<_Tp>(__t);
  }

  template<class _Tp>
  constexpr inline reference_wrapper<const _Tp> cref(const _Tp& __t) noexcept {
    return reference_wrapper<const _Tp>(__t);
  }

  template<class _Tp>
  void ref(const _Tp&&) = delete;

  template<class _Tp>
  void cref(const _Tp&&) = delete;

  template<class _Tp>
  constexpr inline reference_wrapper<_Tp> ref(reference_wrapper<_Tp> __t) noexcept {
    return __t;
  }

  template<class _Tp>
  constexpr inline reference_wrapper<const _Tp> cref(reference_wrapper<_Tp> __t) noexcept {
    return { __t.get() };
  }

} // end of std:: namespace

// End of bits/refwrap header implementations


// Start of implementations from the functional header in libstdc++:(incomplete)

namespace std _DLIB_VISIBILITY {

  template<int _Num>
  struct _Placeholder { }; // not sure if I need this yet. I will delete if unused

  template<class _Callable, class... _Args>
  inline constexpr invoke_result_t<_Callable, _Args...>
  invoke(_Callable&& __fn, _Args&&... __args) noexcept(is_nothrow_invocable<_Callable, _Args...>::value) {
    return __detail::__invoke(std::forward<_Callable>(__fn), std::forward<_Args>(__args)...);
  }

  template<class _Res, class _Callable, class... _Args>
  constexpr typename enable_if<is_invocable_r<_Res, _Callable, _Args...>::value, _Res>::type
  invoke_r(_Callable&& __fn, _Args&&... __args) noexcept(is_nothrow_invocable_r<_Res, _Callable, _Args...>::value) {
    return __detail::__invoke_r<_Res>(std::forward<_Callable>(__fn), std::forward<_Args>(__args)...);
  }

  // specialization for when the _Callable type is not able to do invoke_r. 
  // This isn't necessary, but I did this for a cleaner error message when invoke_r fails the is_invocable_r check.
  template<class _Res, class _Callable, class... _Args>
  [[noreturn]]
  constexpr typename enable_if<!is_invocable_r<_Res, _Callable, _Args...>::value, _Res>::type
  invoke_r(_Callable&&, _Args&&...) noexcept {
    static_assert(is_invocable_r<_Res, _Callable, _Args...>::value,
      "std::invoke_r requires that the return type of the invocable type is convertible to the _Res/R type!");
  }

  namespace __detail { // continuing implementation sugar in private namespace

    template<class _MemFunPtr,
            bool __is_mem_fn = is_member_function_pointer<_MemFunPtr>::value>
    class _Mem_fn_base : public _Mem_fn_traits<_MemFunPtr>::__maybe_type {
      using _Traits = _Mem_fn_traits<_MemFunPtr>;

      using _Arity = typename _Traits::__arity;
      using _Varargs = typename _Traits::__vararg;

      template<class _Func, class... _BoundArgs>
      friend struct _Bind_check_arity;

      _MemFunPtr _M_pmf;

      public:
        using result_type = typename _Traits::__result_type;

        explicit constexpr _Mem_fn_base(_MemFunPtr __pmf) noexcept : _M_pmf(__pmf) { }

        template<class... _Args>
        constexpr auto operator()(_Args&&... __args) const
        noexcept(noexcept(__detail::__invoke(_M_pmf, std::forward<_Args>(__args)...)))
        -> decltype(__detail::__invoke(_M_pmf, std::forward<_Args>(__args)...)) {
          return __detail::__invoke(_M_pmf, std::forward<_Args>(__args)...);
        }
    };

    template<class _MemObjPtr>
    class _Mem_fn_base<_MemObjPtr, false> {
      using _Arity = integral_constant<size_t, 0>;
      using _Varargs = false_type;

      template<class _Func, class... _BoundArgs>
      friend struct _Bind_check_arity;

      _MemObjPtr _M_pm;

      public:
        explicit constexpr _Mem_fn_base(_MemObjPtr __pm) noexcept : _M_pm(__pm) { }

        template<class _Tp>
        constexpr auto operator()(_Tp&& __obj) const
        noexcept(noexcept(__detail::__invoke(_M_pm, std::forward<_Tp>(__obj))))
        -> decltype(__detail::__invoke(_M_pm, std::forward<_Tp>(__obj))) {
          return __detail::__invoke(_M_pm, std::forward<_Tp>(__obj));
        }
    };

    template<class _MemberPointer>
    struct _Mem_fn; // left undefined

    template<class _Res, class _Class>
    struct _Mem_fn<_Res _Class::*> : public _Mem_fn_base<_Res _Class::*> {
      using _Mem_fn_base<_Res _Class::*>::_Mem_fn_base;
    };

  } // end of __detail:: namespace

  template<class _Tp, class _Class>
  constexpr inline __detail::_Mem_fn<_Tp _Class::*>
  mem_fn(_Tp _Class::* __pm) noexcept {
    return __detail::_Mem_fn<_Tp _Class::*>(__pm);
  }

} // end of std:: namespace

// End of functional header implementations(incomplete)


// Start of implementations from the bits/stl_construct header from libstdc++:

namespace std _DLIB_VISIBILITY {

  namespace __detail { // namespace for internal implementations

    template<class _T1, class... _Args>
    inline void _Construct(_T1* __p, _Args&&... __args) {
      ::new(static_cast<void*>(__p)) _T1(std::forward<_Args>(__args)...);
    }

    template<class _Tp>
    inline void _Destroy(_Tp* __pointer) {
      __pointer->~_Tp();
    }

    template<bool>
    struct _Destroy_aux {
      template<class _ForwardIterator>
      static void __destroy(_ForwardIterator __first, _ForwardIterator __last) {
        for(; __first != __last; ++__first)
          _Destroy(__addressof(*__first));
      }
    };

    template<>
    struct _Destroy_aux<true> {
      template<class _ForwardIterator>
      static void __destroy(_ForwardIterator, _ForwardIterator) { }
    };

    template<class _ForwardIterator>
    inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last) {
      typedef typename iterator_traits<_ForwardIterator>::value_type _Value_type;
      _Destroy_aux<__has_trivial_destructor(_Value_type)>::__destroy(__first, __last);
    }

    template<class _ForwardIterator, class _Allocator>
    void _Destroy(_ForwardIterator __first, _ForwardIterator __last, _Allocator& __alloc) {
      typedef __gnu_cxx::__alloc_traits<_Allocator> __traits;
      for(; __first != __last; ++__first)
        __traits::destroy(__alloc, __addressof(*__first));
    }

    template<class _ForwardIterator, class _Tp>
    inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last, allocator<_Tp>&) {
      _Destroy(__first, __last);
    }

  } // end of __detail:: namespace

} // end of std:: namespace

// End of bits/stl_construct header implementations


// Start of implementations from the bits/stl_uninitialized header from libstdc++:

namespace std _DLIB_VISIBILITY {

  namespace __detail { // namespace for internal implementations

    template<bool _TrivialValueTypes>
    struct __uninitialized_copy {
      template<class _InputIterator, class _ForwardIterator>
      static _ForwardIterator __uninit_copy(_InputIterator __first, _InputIterator __last, _ForwardIterator __result) {
        _ForwardIterator __cur = __result;
        __try {
          for(; __first != __last; ++__first, ++__cur)
            _Construct(__addressof(*__cur), *__first);
          return __cur;
        } __catch(...) {
          _Destroy(__result, __cur);
          __throw_exception_again;
        }
      }
    };

    template<>
    struct __uninitialized_copy<true> {
      template<class _InputIterator, class _ForwardIterator>
      static _ForwardIterator __uninit_copy(_InputIterator __first, _InputIterator __last, _ForwardIterator __result) {
        return copy(__first, __last, __result);
      }
    };

  } // end of __detail:: namespace

  
  template<class _InputIterator, class _ForwardIterator>
  inline _ForwardIterator uninitialized_copy(_InputIterator __first, _InputIterator __last, _ForwardIterator __result) {
    typedef typename iterator_traits<_InputIterator>::value_type _ValueType1;
    typedef typename iterator_traits<_ForwardIterator>::value_type _ValueType2;
#   if (_CXX_OLD_CODE)
    const bool __assignable = true; // pre c++11
#   else
    typedef typename iterator_traits<_InputIterator>::reference _RefType;
    const bool __assignable = is_assignable<_ValueType1, _RefType>::value;
#   endif
    return __detail::__uninitialized_copy<__is_trivial(_ValueType1)
                                       && __is_trivial(_ValueType2)
                                       && __assignable
                                       >::__uninit_copy(__first, __last, __result);
  }


  namespace __detail { // namespace for internal implementations

    template<bool _TrivialValueType>
    struct __uninitialized_fill {
      template<class _ForwardIterator, class _Tp>
      static void __uninit_fill(_ForwardIterator __first, _ForwardIterator __last, const _Tp& __x) {
        _ForwardIterator __cur = __first;
        __try {
          for(; __cur != __last; ++__cur)
            _Construct(__addressof(*__cur), __x);
        } __catch(...) {
          _Destroy(__first, __cur);
          __throw_exception_again;
        }
      }
    };

    template<>
    struct __uninitialized_fill<true> {
      template<class _ForwardIterator, class _Tp>
      static void __uninit_fill(_ForwardIterator __first, _ForwardIterator __last, const _Tp& __x) {
        std::fill(__first, __last, __x);
      }
    };

  } // end of __detail:: namespace


  template<class _ForwardIterator, class _Tp>
  inline void uninitialized_fill(_ForwardIterator __first, _ForwardIterator __last, const _Tp& __x) {
    typedef typename iterator_traits<_ForwardIterator>::value_type _ValueType;
#   if(_CXX_OLD_CODE)
    const bool __assignable = true;
#   else
    const bool __assignable = is_copy_assignable<_ValueType>::value;
#   endif
    __detail::__uninitialized_fill<__is_trivial(_ValueType) && __assignable>::__uninit_fill(__first, __last, __x);
  }


  namespace __detail { // namespace for internal implementation

    template<bool _TrivialValueType>
    struct __uninitialized_fill_n {
      template<class _ForwardIterator, class _Size, class _Tp>
      static void __uninit_fill_n(_ForwardIterator __first, _Size __n, const _Tp& __x) {
        _ForwardIterator __cur = __first;
        __try {
          for(; __n > 0; --__n, ++__cur)
            _Construct(__addressof(*__cur), __x);
        } __catch(...) {
          _Destroy(__first, __cur);
          __throw_exception_again;
        }
      }
    };

    template<>
    struct __uninitialized_fill_n<true> {
      template<class _ForwardIterator, class _Size, class _Tp>
      static void __uninit_fill_n(_ForwardIterator __first, _Size __n, const _Tp& __x) {
        fill_n(__first, __n, __x);
      }
    };

  } // end of __detail:: namespace


  template<class _ForwardIterator, class _Size, class _Tp>
  inline void uninitialized_fill_n(_ForwardIterator __first, _Size __n, const _Tp& __x) {
    typedef typename iterator_traits<_ForwardIterator>::value_type _ValueType;
#   if (_CXX_OLD_CODE)
    const bool __assignable = true;
#   else
    const bool __assignable = is_copy_assignable<_ValueType>::value;
#   endif
    __detail::__uninitialized_fill_n<__is_trivial(_ValueType) 
                                  && __assignable>::__uninit_fill_n(__first, __n, __x);
  }


  namespace __detail { // namespace for internal implementations

    template<class _InputIterator, class _ForwardIterator, class _Allocator>
    _ForwardIterator __uninitialized_copy_a(_InputIterator __first, _InputIterator __last,
                                            _ForwardIterator __result, _Allocator& __alloc) {
      _ForwardIterator __cur = __result;
      __try {
        typedef __gnu_cxx::__alloc_traits<_Allocator> __traits;
        for(; __first != __last; ++__first, ++__cur)
          __traits::construct(__alloc, __addressof(*__cur), *__first);
        return __cur;
      } __catch(...) {
        _Destroy(__result, __cur, __alloc);
        __throw_exception_again;
      }
    }

    template<class _InputIterator, class _ForwardIterator, class _Tp>
    inline _ForwardIterator __uninitialized_copy_a(_InputIterator __first, _InputIterator __last,
                                                   _ForwardIterator __result, allocator<_Tp>&) {
      return uninitialized_copy(__first, __last, __result);
    }

    template<class _InputIterator, class _ForwardIterator, class _Allocator>
    inline _ForwardIterator __uninitialized_move_a(_InputIterator __first, _InputIterator __last,
                                                   _ForwardIterator __result, _Allocator& __alloc) {
      return __uninitialized_copy_a(_GLIBCXX_MAKE_MOVE_ITERATOR(__first),
                                    _GLIBCXX_MAKE_MOVE_ITERATOR(__last),
                                    __result, __alloc);
    }

    template<class _InputIterator, class _ForwardIterator, class _Allocator>
    inline _ForwardIterator 
    __uninitialized_move_if_noexcept_a(_InputIterator __first, _InputIterator __last,
                                       _ForwardIterator __result, _Allocator& __alloc) {
      return __uninitialized_copy_a(_GLIBCXX_MAKE_MOVE_IF_NOEXCEPT_ITERATOR(__first),
                                    _GLIBCXX_MAKE_MOVE_IF_NOEXCEPT_ITERATOR(__last),
                                    __result, __alloc);
    }

    template<class _ForwardIterator, class _Tp, class _Allocator>
    void __uninitialized_fill_a(_ForwardIterator __first, _ForwardIterator __last,
                                const _Tp& __x, _Allocator& __alloc) {
      _ForwardIterator __cur = __first;
      __try {
        typedef __gnu_cxx::__alloc_traits<_Allocator> __traits;
        for(; __cur != __last; ++__cur)
          __traits::construct(__alloc, __addressof(*__cur), __x);
      } __catch(...) {
        _Destroy(__first, __cur, __alloc);
        __throw_exception_again;
      }
    }

    template<class _ForwardIterator, class _Tp, class _Tp2>
    inline void __uninitialized_fill_a(_ForwardIterator __first, _ForwardIterator __last,
                                       const _Tp& __x, allocator<_Tp2>&) {
      uninitialized_fill(__first, __last, __x);
    }

    template<class _ForwardIterator, class _Size, class _Tp, class _Allocator>
    void __uninitialized_fill_n_a(_ForwardIterator __first, _Size __n,
                                  const _Tp& __x, _Allocator& __alloc) {
      _ForwardIterator __cur = __first;
      __try {
        typedef __gnu_cxx::__alloc_traits<_Allocator> __traits;
        for(; __n > 0; --__n, ++__cur)
          __traits::construct(__alloc, __addressof(*__cur), __x);
      } __catch(...) {
        _Destroy(__first, __cur, __alloc);
        __throw_exception_again;
      }
    }

    template<class _ForwardIterator, class _Size, class _Tp, class _Tp2>
    inline void __uninitialized_fill_n_a(_ForwardIterator __first, _Size __n,
                                         const _Tp& __x, allocator<_Tp2>&) {
      uninitialized_fill_n(__first, __n, __x);
    }

    template<class _InputIterator1, class _InputIterator2,
             class _ForwardIterator, class _Allocator>
    inline _ForwardIterator __uninitialized_copy_move(_InputIterator1 __first1, _InputIterator1 __last1,
                                                      _InputIterator2 __first2, _InputIterator2 __last2,
                                                      _ForwardIterator __result, _Allocator& __alloc) {
      _ForwardIterator __mid = __uninitialized_copy_a(__first1, __last1, __result, __alloc);
      __try {
        return __uninitialized_move_a(__first2, __last2, __mid, __alloc);
      } __catch(...) {
        _Destroy(__result, __mid, __alloc);
        __throw_exception_again;
      }
    }

    template<class _InputIterator1, class _InputIterator2,
             class _ForwardIterator, class _Allocator>
    inline _ForwardIterator __uninitialized_move_copy(_InputIterator1 __first1, _InputIterator1 __last1,
                                                      _InputIterator2 __first2, _InputIterator2 __last2,
                                                      _ForwardIterator __result, _Allocator& __alloc) {
      _ForwardIterator __mid = __uninitialized_move_a(__first1, __last1, __result, __alloc);
      __try {
        return __uninitialized_copy_a(__first2, __last2, __mid, __alloc);
      } __catch(...) {
        _Destroy(__result, __mid, __alloc);
        __throw_exception_again;
      }
    }

    template<class _ForwardIterator, class _Tp, class _InputIterator, class _Allocator>
    inline _ForwardIterator __uninitialized_fill_move(_ForwardIterator __result, _ForwardIterator __mid,
                                                      const _Tp& __x, _InputIterator __first,
                                                      _InputIterator __last, _Allocator& __alloc) {
      __uninitialized_fill_a(__result, __mid, __x, __alloc);
      __try {
        return __uninitialized_move_a(__first, __last, __mid, __alloc);
      } __catch(...) {
        _Destroy(__result, __mid, __alloc);
        __throw_exception_again;
      }
    }

    template<class _InputIterator, class _ForwardIterator, class _Tp, class _Allocator>
    inline void __uninitialized_move_fill(_InputIterator __first1, _InputIterator __last1,
                                          _ForwardIterator __first2, _ForwardIterator __last2,
                                          const _Tp& __x, _Allocator& __alloc) {
      _ForwardIterator __mid2 = __uninitialized_move_a(__first1, __last1, __first2, __alloc);
      __try {
        __uninitialized_fill_a(__mid2, __last2, __x, __alloc);
      } __catch(...) {
        _Destroy(__first2, __mid2, __alloc);
        __throw_exception_again;
      }
    }

    template<bool _TrivialValueType>
    struct __uninitialized_default_1 {
      template<class _ForwardIterator>
      static void __uninit_default(_ForwardIterator __first, _ForwardIterator __last) {
        _ForwardIterator __cur = __first;
        __try {
          for(; __cur != __last; ++__cur)
            _Construct(__addressof(*__cur));
        } __catch(...) {
          _Destroy(__first, __cur);
          __throw_exception_again;
        }
      }
    };

    template<>
    struct __uninitialized_default_1<true> {
      template<class _ForwardIterator>
      static void __uninit_default(_ForwardIterator __first, _ForwardIterator __last) {
        typedef typename iterator_traits<_ForwardIterator>::value_type _ValueType;
        fill(__first, __last, _ValueType());
      }
    };

    template<bool _TrivialValueType>
    struct __uninitialized_default_n_1 {
      template<class _ForwardIterator, class _Size>
      static void __uninit_default_n(_ForwardIterator __first, _Size __n) {
        _ForwardIterator __cur = __first;
        __try {
          for(; __n > 0; --__n, ++__cur)
            _Construct(__addressof(*__cur));
        } __catch(...) {
          _Destroy(__first, __cur);
          __throw_exception_again;
        }
      }
    };

    template<>
    struct __uninitialized_default_n_1<true> {
      template<class _ForwardIterator, class _Size>
      static void __uninit_default_n(_ForwardIterator __first, _Size __n) {
        typedef typename iterator_traits<_ForwardIterator>::value_type _ValueType;
        fill_n(__first, __n, _ValueType());
      }
    };

    template<class _ForwardIterator>
    inline void __uninitialized_default(_ForwardIterator __first, _ForwardIterator __last) {
      typedef typename iterator_traits<_ForwardIterator>::value_type _ValueType;
      const bool __assignable = is_copy_assignable<_ValueType>::value;
      __uninitialized_default_1<__is_trivial(_ValueType)
                             && __assignable>::__uninit_default(__first, __last);
    }

    template<class _ForwardIterator, class _Size>
    inline void __uninitialized_default_n(_ForwardIterator __first, _Size __n) {
      typedef typename iterator_traits<_ForwardIterator>::value_type _ValueType;
      const bool __assignable = is_copy_assignable<_ValueType>::value;
      __uninitialized_default_n_1<__is_trivial(_ValueType)
                               && __assignable>::__uninit_default_n(__first, __n);
    }

    template<class _ForwardIterator, class _Allocator>
    void __uninitialized_default_a(_ForwardIterator __first, _ForwardIterator __last, _Allocator& __alloc) {
      _ForwardIterator __cur = __first;
      __try {
        typedef __gnu_cxx::__alloc_traits<_Allocator> __traits;
        for(; __cur != __last; ++__cur)
          __traits::construct(__alloc, __addressof(*__cur));
      } __catch(...) {
        _Destroy(__first, __cur, __alloc);
        __throw_exception_again;
      }
    }

    template<class _ForwardIterator, class _Tp>
    inline void __uninitialized_default_a(_ForwardIterator __first, _ForwardIterator __last, allocator<_Tp>&) {
      __uninitialized_default(__first, __last);
    }

    template<class _ForwardIterator, class _Size, class _Allocator>
    void __uninitialized_default_n_a(_ForwardIterator __first, _Size __n, _Allocator& __alloc) {
      _ForwardIterator __cur = __first;
      __try {
        typedef __gnu_cxx::__alloc_traits<_Allocator> __traits;
        for(; __n > 0; --__n, ++__cur)
          __traits::construct(__alloc, __addressof(*__cur));
      } __catch(...) {
        _Destroy(__first, __cur, __alloc);
        __throw_exception_again;
      }
    }

    template<class _ForwardIterator, class _Size, class _Tp>
    inline void __uninitialized_default_n_a(_ForwardIterator __first, _Size __n, allocator<_Tp>&) {
      __uninitialized_default_n(__first, __n);
    }

    template<class _InputIterator, class _Size, class _ForwardIterator>
    _ForwardIterator __uninitialized_copy_n(_InputIterator __first, _Size __n,
                                            _ForwardIterator __result, input_iterator_tag) {
      _ForwardIterator __cur = __result;
      __try {
        for(; __n > 0; --__n, ++__first, ++__cur)
          _Construct(__addressof(*__cur), *__first);
        return __cur;
      } __catch(...) {
        _Destroy(__result, __cur);
        __throw_exception_again;
      }
    }

    template<class _RandomAccessIterator, class _Size, class _ForwardIterator>
    inline _ForwardIterator __uninitialized_copy_n(_RandomAccessIterator __first, _Size __n,
                                                   _ForwardIterator __result, random_access_iterator_tag) {
      return uninitialized_copy(__first, __first + __n, __result);
    }

  } // end of __detail:: namespace


  template<class _InputIterator, class _Size, class _ForwardIterator>
  inline _ForwardIterator uninitialized_copy_n(_InputIterator __first, _Size __n, _ForwardIterator __result) {
    return __detail::__uninitialized_copy_n(__first, __n, __result,
                                            __detail::__iterator_category(__first));
  }

} // end of std:: namespace

// End of bits/stl_uninitialized header implementations


// Start of bits/functional_hash header implementation:
namespace std _DLIB_VISIBILITY {

  namespace __detail { // namespace for internal implementations

    size_t _Hash_bytes(const void* __ptr, size_t __len, size_t __seed);

    size_t _Fnv_hash_bytes(const void* __ptr, size_t __len, size_t __seed);


    template<class _Result, class _Arg>
    struct __hash_base {
      typedef _Result result_type;
      typedef _Arg argument_type;
    };

  } // end of __detail:: namespace


  template<class _Tp>
  struct hash;

  template<class _Tp>
  struct hash<_Tp*> : public __detail::__hash_base<size_t, _Tp*> {
    size_t operator()(_Tp* __p) const noexcept {
      return reinterpret_cast<size_t>(__p);
    }
  };


  // Helper macro for integer specializations
# define _Cxx_hashtable_define_trivial_hash(_Tp) \
  template<> \
  struct hash<_Tp> : public __detail::__hash_base<size_t, _Tp> { \
    size_t operator()(_Tp __val) const noexcept { \
      return static_cast<size_t>(__val); \
    } \
  };

  _Cxx_hashtable_define_trivial_hash(bool)

  _Cxx_hashtable_define_trivial_hash(char)

  _Cxx_hashtable_define_trivial_hash(signed char)

  _Cxx_hashtable_define_trivial_hash(unsigned char)

  _Cxx_hashtable_define_trivial_hash(wchar_t)

  _Cxx_hashtable_define_trivial_hash(char16_t)

  _Cxx_hashtable_define_trivial_hash(char32_t)

  _Cxx_hashtable_define_trivial_hash(short)

  _Cxx_hashtable_define_trivial_hash(int)

  _Cxx_hashtable_define_trivial_hash(long)

  _Cxx_hashtable_define_trivial_hash(long long)

  _Cxx_hashtable_define_trivial_hash(unsigned short)

  _Cxx_hashtable_define_trivial_hash(unsigned int)

  _Cxx_hashtable_define_trivial_hash(unsigned long)

  _Cxx_hashtable_define_trivial_hash(unsigned long long)

  // undefining the no loner needed helper macro
# undef _Cxx_hashtable_define_trivial_hash


  namespace __detail { // continuing internal namespace

    struct _Hash_impl {
      static size_t hash(const void* __ptr, size_t __clength,
                         size_t __seed = static_cast<size_t>(0xC70F6907UL)) {
        return _Hash_bytes(__ptr, __clength, __seed);
      }

      template<class _Tp>
      static size_t hash(const _Tp& __val) {
        return hash(&__val, sizeof(__val));
      }

      template<class _Tp>
      static size_t __hash_combine(const _Tp& __val, size_t __hash) {
        return hash(&__val, sizeof(__val), __hash);
      }
    }; // end of _Hash_impl class

    
    struct _Fnv_hash_impl {
      static size_t hash(const void* __ptr, size_t __clength,
                         size_t __seed = static_cast<size_t>(2166136261UL)) {
        return _Fnv_hash_bytes(__ptr, __clength, __seed);
      }

      template<class _Tp>
      static size_t hash(const _Tp& __val) {
        return hash(&__val, sizeof(__val));
      }

      template<class _Tp>
      static size_t __hash_combine(const _Tp& __val, size_t __hash) {
        return hash(&__val, sizeof(__val), __hash);
      }
    }; // end of _Fnv_hash_impl class

  } // end of __detail:: namespace


  template<>
  struct hash<float> : public __detail::__hash_base<size_t, float> {
    size_t operator()(float __val) const noexcept {
      return __val != 0.0f ? __detail::_Hash_impl::hash(__val) : 0;
    }
  };

  template<>
  struct hash<double> : public __detail::__hash_base<size_t, double> {
    size_t operator()(double __val) const noexcept {
      return __val != 0.0 ? __detail::_Hash_impl::hash(__val) : 0;
    }
  };

  template<>
  struct hash<long double> : public __detail::__hash_base<size_t, long double> {
    _PURE_ size_t operator()(long double __val) const noexcept;
  };


  namespace __detail { // internal "hash trait" internal implementations

    template<class _Hash>
    struct __is_fast_hash : public true_type { };

    template<>
    struct __is_fast_hash<hash<long double>> : public false_type { };

  } // end of __detail:: namespace

} // end of std:: namespace
// End of bits/functional_hash header implementation


#endif // end of Utility.hpp
