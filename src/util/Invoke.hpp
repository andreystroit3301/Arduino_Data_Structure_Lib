// Libstdc++ and GCC GNU Lincese boilerplate:      (this is required since this code is copyrighted under the GCC compiler. I did not design this myself.)
/*
  Partial Modified Implementation Of Meta-Functions from the <bits/invoke> Libstdc++ header  -*- C++ -*-

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
  Invoke.hpp [V1.0.0] (Template header file for invoke functions)
    By: Andrey Stroitelev

  ~This file is to implement std::invoke and std::invoke_r
  ~This file is just bits/invoke.h from libstdc++
*/


// Start of Invoke.hpp
#ifndef _DLIB_INVOKE_HPP_
#define _DLIB_INVOKE_HPP_

#ifndef _DLIB_INVOKE_VERSION_
#define _DLIB_INVOKE_VERSION_ 0x010000
#endif


#include <Arduino.h>
#include "Config.h"
#include "Type_Traits.hpp"


_DLIB_HEADER


// Start of Invoke.hpp Implementations:
namespace std {

  namespace __detail { // private namespace for implementation sugar

    /*
      These are added overloads for __invoke_impl which are for the actual std::invoke function.
      The overloads of __invoke_impl in Type_Traits.hpp were pre-defined to make std::is_invocable work.
      Some of the implementations below are not SFINAE friendly.
      These implementations are the main ones that will be used by std::invoke to work properly.
    */ 

    template<class _Tp, class _Up = typename __inv_unwrap<_Tp>::type>
    constexpr _Up&& __invfwd(typename remove_reference<_Tp>::type& __t) noexcept {
      return static_cast<_Up&&>(__t);
    }

    template<class _Res, class _Fn, class... _Args>
    constexpr _Res __invoke_impl(__invoke_other, _Fn&& __f, _Args&&... __args) {
      return forward<_Fn>(__f)(forward<_Args>(__args)...);
    }

    template<class _Res, class _MemFun, class _Tp, class... _Args>
    constexpr _Res __invoke_impl(__invoke_memfun_ref, _MemFun&& __f, _Tp&& __t, _Args&&... __args) {
      return (__invfwd<_Tp>(__t).*__f)(forward<_Args>(__args)...);
    }

    template<class _Res, class _MemFun, class _Tp, class... _Args>
    constexpr _Res __invoke_impl(__invoke_memfun_deref, _MemFun&& __f, _Tp&& __t, _Args&&... __args) {
      return ((*forward<_Tp>(__t)).*__f)(forward<_Args>(__args)...);
    }

    template<class _Res, class _MemPtr, class _Tp>
    constexpr _Res __invoke_impl(__invoke_memobj_ref, _MemPtr&& __f, _Tp&& __t) {
      return __invfwd<_Tp>(__t).*__f;
    }

    template<class _Res, class _MemPtr, class _Tp>
    constexpr _Res __invoke_impl(__invoke_memobj_deref, _MemPtr&& __f, _Tp&& __t) {
      return (*forward<_Tp>(__t)).*__f;
    }

    template<class _Callable, class... _Args> // invocing a callable object
    constexpr typename __invoke_result<_Callable, _Args...>::type
    __invoke(_Callable&& __fn, _Args&&... __args) noexcept(__is_nothrow_invocable<_Callable, _Args...>::value) {
      using __result = __invoke_result<_Callable, _Args...>;
      using __type = typename __result::type;
      using __tag = typename __result::__invoke_type;
      return __invoke_impl<__type>(__tag{}, forward<_Callable>(__fn), forward<_Args>(__args)...);
    }

#ifdef CPP17_INVOKE
    template<class _Res, class _Callable, class... _Args>
    constexpr enable_if_t<is_invocable_r<_Res, _Callable, _Args...>::value, _Res>
    __invoke_r(_Callable&& __fn, _Args&&... __args) noexcept(is_nothrow_invocable_r<_Res, _Callable, _Args...>::value) {
      using __result = __invoke_result<_Callable, _Args...>;
      using __type = typename __result::type;
      using __tag = typename __result::__invoke_type;
      if constexpr (is_void<_Res>::value)
        __invoke_impl<__type>(__tag{}, forward<_Callable>(__fn), forward<_Args>(__args)...);
      else
        return __invoke_impl<__type>(__tag{}, forward<_Callable>(__fn), forward<_Args>(__args)...);
    }

#else // this is the c++11/14 non-sfinae-friendly implementation that is used when CPP17_INVOKE is not defined
    template<class _Res, class _Callable, class... _Args>
    constexpr enable_if_t<!is_void<_Res>::value, _Res>
    __invoke_r(_Callable&& __fn, _Args&&... __args) {
      using __result = __invoke_result<_Callable, _Args...>;
      using __type = typename __result::type;

      // leaving this commented out until I can test it.
      // This does a preprocessor check to see if the intrinsic __reference_converts_from_temporary is defined by the compiler.
      // if this compiler intrinsic is defined then it will make a static assertion to make sure there is no dangling reference.
      // Without the intrinsic errors relating to the dangling reference might be difficult to diagnose.
/*#if __has_builtin(__reference_converts_from_temporary)
      static_assert(!__reference_converts_from_temporary(_Res, __type),
        "std::invoke<R> must not create a dangling reference!");
#endif*/ // end of intrinsic check
      
      using __tag = typename __result::__invoke_type;
      return __invoke_impl<__type>(__tag{}, forward<_Callable>(__fn), forward<_Args>(__args)...);
    }

    template<class _Res, class _Callable, class... _Args>
    constexpr enable_if_t<is_void<_Res>::value, _Res>
    __invoke_r(_Callable&& __fn, _Args&&... __args) {
      using __result = __invoke_result<_Callable, _Args...>;
      using __type = typename __result::type;
      using __tag = typename __result::__invoke_type;
      __invoke_impl<__type>(__tag{}, forward<_Callable>(__fn), forward<_Args>(__args)...);
    }
#endif // end of __invoke_r implementations

  } // end of __detail:: namespace

} // end of std:: namespace


#endif // End of Invoke.hpp
