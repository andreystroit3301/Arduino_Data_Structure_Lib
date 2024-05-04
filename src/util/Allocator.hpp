// Libstdc++ and GCC GNU Lincese boilerplate:      (this is required since this code is copyrighted under the GCC compiler. I did not design this myself.)
/*
  Partial Modified Implementation Of Meta-Functions from the <new | ext/new_allocator | bits/c++allocator | etc...> Libstdc++ header  -*- C++ -*-

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
  Allocator.hpp  [V1.0.0]  (Template header file for allocator implementations)
    By: Andrey Stroitelev

  ~This file consolidates a bunch of allocator related files from libstdc++ to avoid having a bunch of seperate files that would be included in only one place.
    *If I end up needing to split the file implementations I will do so.
  ~This file just defines the allocator classes and related utilities for memory allocation
*/


// Start of Allocator.hpp
#ifndef _DLIB_ALLOCATOR_HPP_
#define _DLIB_ALLOCATOR_HPP_

#ifndef _DLIB_ALLOCATOR_VERSION_
#define _DLIB_ALLOCATOR_VERSION_ 0x010000
#endif


#define _ALLOC_EXTERN_TEMPLATE true


#include <Arduino.h>
#include "Config.h"
#include "Stdexcept.hpp"
#include "Move.hpp"
#include "Type_Traits.hpp"


_DLIB_HEADER


// Start of libstdc++ <new> header implementation:

#if(_USING_CPP_NEW)
#include <new.h>
#else

#pragma GCC visibility push(default)

extern "C++" {

  namespace std {

    class bad_alloc : public exception {
      public:
        bad_alloc() throw() { }

        virtual ~bad_alloc() throw();

        virtual const char* what() const throw();
    };

    class bad_array_new_length : public bad_alloc {
      public:
        bad_array_new_length() throw() { };

        virtual ~bad_array_new_length() throw();

        virtual const char* what() const throw();
    };

    class bad_array_length : public bad_alloc {
      public:
        bad_array_length() throw() { };

        virtual ~bad_array_length() throw();

        virtual const char* what() const throw();
    };

    struct nothrow_t { };

    extern const nothrow_t nothrow;

    typedef void (*new_handler)();

    new_handler set_new_handler(new_handler) throw();

    new_handler get_new_handler() noexcept;

  } // end of std:: namespace

  void* operator new(size_t) __attribute__((__externally_visible__));

  void* operator new[](size_t) __attribute__((__externally_visible__));

  void operator delete(void*) noexcept __attribute__((__externally_visible__));

  void operator delete[](void*) noexcept __attribute__((__externally_visible__));

  void* operator new(size_t, const std::nothrow_t&) noexcept __attribute__((__externally_visible__));

  void* operator new[](size_t, const std::nothrow_t&) noexcept __attribute__((__externally_visible__));

  void operator delete(void*, const std::nothrow_t&) noexcept __attribute__((__externally_visible__));

  void operator delete[](void*, const std::nothrow_t&) noexcept __attribute__((__externally_visible__));

  inline void* operator new(size_t, void* __p) noexcept { return __p; }

  inline void* operator new[](size_t, void* __p) noexcept { return __p; }

  inline void operator delete(void*, void*) noexcept { }
  inline void operator delete[](void*, void*) noexcept { }

} // end of extern "C++"

#pragma GCC visibility pop

#endif // end of new.h implementation

// End of libstdc++ <new> header implementation


// Start of libstdc++ bits/new_allocator header implementation:
namespace __gnu_cxx _DLIB_VISIBILITY {

  template<class _Tp>
  class new_allocator {
    public:
      typedef size_t size_type;
      typedef ptrdiff_t difference_type;
      typedef _Tp* pointer;
      typedef const _Tp* const_pointer;
      typedef _Tp& reference;
      typedef const _Tp& const_reference;
      typedef _Tp value_type;

      template<class _Tp1>
      struct rebind {
        typedef new_allocator<_Tp1> other;
      };

      typedef std::true_type propagate_on_container_move_assignment;

      new_allocator() noexcept { }

      new_allocator(const new_allocator&) noexcept { }

      template<class _Tp1>
      new_allocator(const new_allocator<_Tp1>&) noexcept { }

      ~new_allocator() noexcept { }

      pointer address(reference __x) const noexcept {
        return std::__detail::__addressof(__x);
      }

      const_pointer address(const_reference __x) const noexcept {
        return std::__detail::__addressof(__x);
      }

      pointer allocate(size_type __n, const void* = 0) {
        if(__n > this->max_size()) std::__throw_bad_alloc();

        return static_cast<_Tp*>(::operator new(__n * sizeof(_Tp)));
      }

      void deallocate(pointer __p, size_type) {
        ::operator delete(__p);
      }

      size_type max_size() const noexcept {
        return size_t(-1) / sizeof(_Tp);
      }

      template<class _Up, class... _Args>
      void construct(_Up* __p, _Args&&... __args) {
        ::new((void*)__p) _Up(std::forward<_Args>(__args)...);
      }

      template<class _Up>
      void destroy(_Up* __p) {
        __p->~_Up();
      }
  };

  template<class _Tp>
  inline bool operator==(const new_allocator<_Tp>&, const new_allocator<_Tp>&) {
    return true;
  }

  template<class _Tp>
  inline bool operator!=(const new_allocator<_Tp>&, const new_allocator<_Tp>&) {
    return false;
  }

} // end of __gnu_cxx:: namespace
// End of libstdc++ bits/new_allocator header implementation


// Start of libstdc++ bits/c++allocator header implementation:

namespace std {

  namespace __detail {

    template<class _Tp>
    using __allocator_base = __gnu_cxx::new_allocator<_Tp>;

  }

}

// End of libstdc++ bits/c++allocator header implementation


// Start of libstdc++ bits/allocator header implementation:

namespace std _DLIB_VISIBILITY {

  template<>
  class allocator<void> {
    public:
      typedef size_t size_type;
      typedef ptrdiff_t difference_type;
      typedef void* pointer;
      typedef const void* const_pointer;
      typedef void value_type;

      template<class _Tp1>
      struct rebind { typedef allocator<_Tp1> other; };

      typedef true_type propagate_on_container_move_assignment;
  };


  template<class _Tp>
  class allocator : public __detail::__allocator_base<_Tp> {
    public:
      typedef size_t size_type;
      typedef ptrdiff_t difference_type;
      typedef _Tp* pointer;
      typedef const _Tp* const_pointer;
      typedef _Tp& reference;
      typedef const _Tp& const_reference;
      typedef _Tp value_type;

      template<class _Tp1>
      struct rebind { typedef allocator<_Tp1> other; };

      typedef true_type propagate_on_container_move_assignment;

      allocator() throw() { }

      allocator(const allocator& __a) throw() 
        : __detail::__allocator_base<_Tp>(__a) { }

      template<class _Tp1>
      allocator(const allocator<_Tp1>&) throw() { }

      ~allocator() throw() { }
  };


  template<class _T1, class _T2>
  inline bool operator==(const allocator<_T1>&, const allocator<_T2>&) { return true; }

  template<class _Tp>
  inline bool operator==(const allocator<_Tp>&, const allocator<_Tp>&) { return true; }

  template<class _T1, class _T2>
  inline bool operator!=(const allocator<_T1>&, const allocator<_T2>&) { return false; }

  template<class _Tp>
  inline bool operator!=(const allocator<_Tp>&, const allocator<_Tp>&) { return false; }


# if(_ALLOC_EXTERN_TEMPLATE)
  extern template class allocator<char>;
  extern template class allocator<wchar_t>;
# endif

//# undef __allocator_base // pretty sure I dont need this as this macro is only defined for pre cpp11 code which I didn't use

  namespace __detail {

    template<class _Alloc, bool = __is_empty(_Alloc)>
    struct __alloc_swap {
      static void _S_do_it(_Alloc&, _Alloc&) noexcept { }
    };

    template<class _Alloc>
    struct __alloc_swap<_Alloc, false> {
      static void _S_do_it(_Alloc& __one, _Alloc& __two) noexcept {
        if(__one != __two) swap(__one, __two);
      }
    };

    template<class _Alloc, bool = __is_empty(_Alloc)>
    struct __alloc_neq {
      static bool _S_do_it(const _Alloc&, const _Alloc&) { return false; }
    };

    template<class _Alloc>
    struct __alloc_neq<_Alloc, false> {
      static bool _S_do_it(const _Alloc& __one, const _Alloc& __two) {
        return __one != __two;
      }
    };

    template<class _Tp, bool = __or_<is_copy_constructible<typename _Tp::value_type>,
                                    is_nothrow_move_constructible<typename _Tp::value_type>>::value>
    struct __shrink_to_fit_aux {
      static bool _S_do_it(_Tp&) noexcept { return false; }
    };

    template<class _Tp>
    struct __shrink_to_fit_aux<_Tp, true> {
      static bool _S_do_it(_Tp& __c) noexcept {
        __try {
          _Tp(__detail::__make_move_if_noexcept_iterator(__c.begin()),
              __detail::__make_move_if_noexcept_iterator(__c.end()),
              __c.get_allocator()).swap(__c);
        } __catch(...) { return false; }
      }
    };

  } // end of __detail:: namespace

} // end of std:: namespace

// End of libstdc++ bits/allocator header implementation


#endif // End of Allocator.hpp
