// Libstdc++ and GCC GNU Lincese boilerplate:      (this is required since this code is copyrighted under the GCC compiler. I did not design this myself.)
/*
  Partial Modified Implementation Of Meta-Functions from the <stdexcept>/<exception> Libstdc++ header  -*- C++ -*-

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
  Stdexcept.hpp  [V1.0.0]  (utility header file)
    By: Andrey Stroitelev

  ~This pretty much a copy of the exception.h and stdexcept.h header files in libstdc++.
    *This file just combines the 2 libstdc++ files into 1 file.
  ~The purpose of this file is to give some exception utilities used in other parts of the util folder.
    *I might get rid of this file once I reimplement the functions that use these implementations since the Arduino IDE has exceptions disabled.
  ~The exception.h and stdexcept.h implementations will be implemented like they are seperate files using their own include guards to divide them.
    *doing this instead of just having the 2 file implementations just in the same namespace seperated by comments since exception.h require certain preprocessor directives that stdexcept.h doesn't use.
*/


// Start of Stdexcept.hpp
#ifndef _DLIB_STDEXCEPT_HPP_
#define _DLIB_STDEXCEPT_HPP_

#ifndef _DLIB_STDEXCEPT_VERSION_
#define _DLIB_STDEXCEPT_VERSION_ 0x010000
#endif


// includes shared between both file implementations
#include <Arduino.h>
//#include <stdio.h>
#include "Config.h"


_DLIB_HEADER


// Start bits/atomic_lockfree_defines.h macro definitions: (c++14 gcc-4.9.0)
#define ATOMIC_BOOL_LOCK_FREE       __GCC_ATOMIC_BOOL_LOCK_FREE
#define ATOMIC_CHAR_LOCK_FREE       __GCC_ATOMIC_CHAR_LOCK_FREE
#define ATOMIC_WCHAR_T_LOCK_FREE    __GCC_ATOMIC_WCHAR_T_LOCK_FREE
#define ATOMIC_CHAR16_T_LOCK_FREE   __GCC_ATOMIC_CHAR16_T_LOCK_FREE
#define ATOMIC_CHAR32_T_LOCK_FREE   __GCC_ATOMIC_CHAR32_T_LOCK_FREE
#define ATOMIC_SHORT_LOCK_FREE      __GCC_ATOMIC_SHORT_LOCK_FREE
#define ATOMIC_INT_LOCK_FREE        __GCC_ATOMIC_INT_LOCK_FREE
#define ATOMIC_LONG_LOCK_FREE       __GCC_ATOMIC_LONG_LOCK_FREE
#define ATOMIC_LLONG_LOCK_FREE      __GCC_ATOMIC_LLONG_LOCK_FREE
#define ATOMIC_POINTER_LOCK_FREE    __GCC_ATOMIC_POINTER_LOCK_FREE
// End of bits/atomic_lockfree_defines.h macro definitions


// Start of exception_defines macro definitions:
// these are the macro definitions when using -fno-exceptions which is used in arduino IDE to disable exceptions
//#define __try       if(true)
//#define __catch     if(false)
//#define __throw_exception_again
// Since you technically can still enable exceptions on arduino IDE I'm adding this check.
// it is greatly advised that exceptions should not be turned on as the arduino AVR processor architecture doesn't support them.
// With exceptions enabled the arduino will just crash when they're thrown causing it to either freeze, or reset itself.
#ifndef __EXCEPTIONS // built-in macro defined by the compiler when exceptions are enabled
# define __try if(true)
# define __catch(_X) if(false)
# define __throw_exception_again // undefined
#else
# define __try try
# define __catch(_X) catch(_X)
# define __throw_exception_again throw
#endif
// End of exception_defines macros


// Start of exception.h from c++ STL implementation:

#ifndef _DLIB_EXCEPTION_
#define _DLIB_EXCEPTION_


#pragma GCC visibility push(default)


extern "C++" {

  namespace std { // start of implementation

    class exception {
      public:
        exception() noexcept { }
        virtual ~exception() noexcept;

        virtual const char* what() const noexcept;
    };

    class bad_exception : public exception {
      public:
        bad_exception() noexcept { }

        virtual ~bad_exception() noexcept;

        virtual const char* what() const noexcept;
    };

    typedef void (*terminate_handler) ();

    typedef void (*unexpected_handler) ();

    terminate_handler set_terminate(terminate_handler) noexcept;

    terminate_handler get_terminate() noexcept;

    void terminate() noexcept __attribute__((__noreturn__));

    unexpected_handler set_unexpected(unexpected_handler) noexcept;

    unexpected_handler get_unexpected() noexcept;

    void unexpected() __attribute__((__noreturn__));

    bool uncaught_exception() noexcept _PURE_;

  } // end of std:: namespace

  namespace __gnu_cxx {

    void __verbose_terminate_handler();

  }

} // end of extern "C++"


#pragma GCC visibility pop


#if(ATOMIC_INT_LOCK_FREE > 1) // start of implementations for nested and pointer exceptions

#pragma GCC visibility push(default)

#if ATOMIC_INT_LOCK_FREE < 2
#  error This platform does not support exception propagation
#endif

extern "C++" {

  namespace std { // start of exception_ptr implementations

    class type_info;

    namespace __exception_ptr {
      class exception_ptr;
    }

    using __exception_ptr::exception_ptr;

    exception_ptr current_exception() noexcept;

    void rethrow_exception(exception_ptr) __attribute__((__noreturn__));

    namespace __exception_ptr {

      class exception_ptr {
        void* _M_exception_object;

        explicit exception_ptr(void* __e) noexcept;

        void _M_addref() noexcept;
        void _M_release() noexcept;

        void *_M_get() const noexcept _PURE_;

        friend exception_ptr std::current_exception() noexcept;
        friend void std::rethrow_exception(exception_ptr);

        public:
          exception_ptr() noexcept;

          exception_ptr(const exception_ptr&) noexcept;

          exception_ptr(nullptr_t) noexcept
            : _M_exception_object(0) { }

          exception_ptr(exception_ptr&& __o) noexcept
            : _M_exception_object(__o._M_exception_object) {
            __o._M_exception_object = 0;
          }

          typedef void (exception_ptr::*__safe_bool)();

          exception_ptr(__safe_bool) noexcept;

          exception_ptr& operator=(const exception_ptr&) noexcept;

          exception_ptr& operator=(exception_ptr&& __o) noexcept {
            exception_ptr(static_cast<exception_ptr&&>(__o)).swap(*this);
            return *this;
          }

          ~exception_ptr() noexcept;

          void swap(exception_ptr&) noexcept;

          void _M_safe_bool_dummy() noexcept __attribute__((__const__));

          bool operator!() const noexcept _PURE_;

          operator __safe_bool() const noexcept;

          explicit operator bool() const {
            return _M_exception_object;
          }

          friend bool operator==(const exception_ptr&, const exception_ptr&) noexcept _PURE_;

          const class std::type_info* __cxa_exception_type() const noexcept _PURE_;
      }; // end of exception_ptr

      bool operator==(const exception_ptr&, const exception_ptr&) noexcept _PURE_;

      bool operator!=(const exception_ptr&, const exception_ptr&) noexcept _PURE_;

      inline void swap(exception_ptr& __lhs, exception_ptr& __rhs) {
        __lhs.swap(__rhs);
      }

    } // end of __exception_ptr:: namespace

    template<class _Ex>
    exception_ptr make_exception_ptr(_Ex __ex) noexcept {
      __try {
        throw __ex;
      } __catch(...) {
        return current_exception();
      }
    }

    template<class _Ex>
    exception_ptr copy_exception(_Ex __ex) noexcept [[deprecated]];

    template<class _Ex>
    exception_ptr copy_exception(_Ex __ex) noexcept {
      return std::make_exception_ptr<_Ex>(__ex);
    }

  } // end of std:: namespace

} // end of extern "C++"

#pragma GCC visibility pop


#pragma GCC visibility push(default)

#if ATOMIC_INT_LOCK_FREE < 2
#  error This platform does not support exception propagation
#endif

extern "C++" {

  namespace std { // start of nested_exception implementation

    class nested_exception {
      exception_ptr _M_ptr;

      public:
        nested_exception() noexcept : _M_ptr(current_exception()) { }

        nested_exception(const nested_exception&) = default;

        nested_exception& operator=(const nested_exception&) = default;

        virtual ~nested_exception() noexcept;

        void rethrow_nested() const __attribute__((__noreturn__)) {
          rethrow_exception(_M_ptr);
        }

        exception_ptr nested_ptr() const {
          return _M_ptr;
        }
    };

    template<class _Except>
    struct _Nested_exception : public _Except, public nested_exception {
      explicit _Nested_exception(_Except&& __ex)
        : _Except(static_cast<_Except&&>(__ex)) { }
    };

    template<class _Ex>
    struct __get_nested_helper {
      static const nested_exception* _S_get(const _Ex& __ex) {
        return dynamic_cast<const nested_exception*>(&__ex);
      }
    };

    template<class _Ex>
    struct __get_nested_helper<_Ex*> {
      static const nested_exception* _S_get(const _Ex* __ex) {
        return dynamic_cast<const nested_exception*>(__ex);
      }
    };

    template<class _Ex>
    inline const nested_exception* __get_nested_exception(const _Ex& __ex) {
      return __get_nested_helper<_Ex>::_S_get(__Ex);
    }

    template<class _Ex>
    void __throw_with_nested(_Ex&&, const nested_exception* = 0) __attribute__((__noreturn__));

    template<class _Ex>
    void __throw_with_nested(_Ex&&, ...) __attribute__((__noreturn__));

    template<class _Ex>
    inline void __throw_with_nested(_Ex&& __ex, const nested_exception*) { throw __ex; }

    template<class _Ex>
    inline void __throw_with_nested(_Ex&& __ex, ...) {
      throw _Nested_exception<_Ex>(static_cast<_Ex&&>(__ex));
    }

    template<class _Ex>
    void throw_with_nested(_Ex __ex) __attribute__((__noreturn__));

    template<class _Ex>
    inline void throw_with_nested(_Ex __ex) {
      if(__get_nested_exception(__ex)) throw __ex;
      __throw_with_nested(static_cast<_Ex&&>(__ex), &__ex);
    }

    template<class _Ex>
    inline void rethrow_if_nested(const _Ex& __ex) {
      if(const nested_exception* __nested = __get_nested_exception(__ex))
        __nested->rethrow_nested();
    }

    inline void rethrow_if_nested(const nested_exception& __ex) { __ex.rethrow_nested(); }

  } // end of std:: namespace

} // end of extern "C++"

#pragma GCC visibility pop

#endif // end of exception_ptr impl


#endif // end of exception.h impl

// End of exception.h implementation


#ifndef _GLIBCXX_STDEXCEPT
#define _GLIBCXX_STDEXCEPT 1


// Start of main stdexcept.h file implementation:
namespace std {

  class logic_error : public exception {
    String _M_msg;

    public:
      explicit logic_error(const String& __arg);

      virtual ~logic_error() noexcept;

      virtual const char* what() const noexcept;
  }; // end of logic_error class

  class domain_error : public logic_error {
    public:
      explicit domain_error(const String& __arg);
      virtual ~domain_error() noexcept;
  };

  class invalid_argument : public logic_error {
    public:
      explicit invalid_argument(const String& __arg);
      virtual ~invalid_argument() noexcept;
  };

  class length_error : public logic_error {
    public:
      explicit length_error(const String& __arg);
      virtual ~length_error() noexcept;
  };

  class out_of_range : public logic_error {
    public:
      explicit out_of_range(const String& __arg);
      virtual ~out_of_range() noexcept;
  };

  class runtime_error : public exception {
    String _M_msg;

    public:
      explicit runtime_error(const String& __arg);

      virtual ~runtime_error() noexcept;

      virtual const char* what() const noexcept;
  };

  class range_error : public runtime_error {
    public:
      explicit range_error(const String& __arg);
      virtual ~range_error() noexcept;
  };

  class overflow_error : public runtime_error {
    public:
      explicit overflow_error(const String& __arg);
      virtual ~overflow_error() noexcept;
  };

  class underflow_error : public runtime_error {
    public:
      explicit underflow_error(const String& __arg);
      virtual ~underflow_error() noexcept;
  };

} // end of std:: namespace
// End of stdexcept.h implementation


#endif // end of stdexcept impl


// Start of functexcept.h implementation from libstdc++
#ifndef _DLIB_FUNCTEXCEPT_
#define _DLIB_FUNCTEXCEPT_ 1


// Start of std:: namespace for functexcept implementation:
namespace std _DLIB_VISIBILITY {

  [[noreturn]]
  void __throw_bad_exception(void);

  [[noreturn]]
  void __throw_bad_alloc(void);

  [[noreturn]]
  void __throw_bad_cast(void);

  [[noreturn]]
  void __throw_bad_typeid(void);

  [[noreturn]]
  void __throw_logic_error(const char*);

  [[noreturn]]
  void __throw_domain_error(const char*);

  [[noreturn]]
  void __throw_invalid_argument(const char*);

  [[noreturn]]
  void __throw_length_error(const char*);

  [[noreturn]]
  void __throw_out_of_range(const char*);

  // not sure if the __printf__ will work properly
  void __throw_out_of_range_fmt(const char*, ...) __attribute__((__noreturn__)) 
    __attribute__((__format__(__printf__, 1, 2)));

  [[noreturn]]
  void __throw_runtime_error(const char*);

  [[noreturn]]
  void __throw_range_error(const char*);

  [[noreturn]]
  void __throw_overflow_error(const char*);

  [[noreturn]]
  void __throw_underflow_error(const char*);

  [[noreturn]]
  void __throw_ios_failure(const char*);

  [[noreturn]]
  void __throw_system_error(int);

  [[noreturn]]
  void __throw_future_error(int);

  [[noreturn]]
  void __throw_bad_function_call();

} // end of std:: namespace


#endif // end of functexcept.h impl


#endif // end of file
