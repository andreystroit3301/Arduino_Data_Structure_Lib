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
  Config.h  [V1.0.0]  (preprocessor library config file)
    By: Andrey Stroitelev

  ~This file defines a set of macros that are used to configure and change the behavior of the library.
  ~This is mainly used to toggle features that arent actually supported but still usable.
    *enabling some macros can cause the compiler to show a bunch of warnings stating the feature is not available, but it will still compile and run.
  ~The macros are divided into 2 sections.
    *The first section is for the actual toggle macros. These macros are either defined or commented out to toggle the feature they corrolate to.
    *The second section is the macros that are actually used by the library. These macros will always be defined, but what they expand to is set by the toggle macros.
  ~Some combinations of macros being enabled and disabled can cause compiler errors because of how c++11 sees member function signatures.
  
  !Note! ~ A good chunk of these macros will be removed eventually since most of them are for testing/diagnostics, debugging, and experimenting.
*/
// NOTE: The implementation of these macros is not yet complete.
//       I still need to update all the code that these macros affect which will end up taking a while due to the amount of code.


// Start of Config.h
#ifndef _DLIB_CONFIG_H_
#define _DLIB_CONFIG_H_

#ifndef _DLIB_CONFIG_VERSION_
#define _DLIB_CONFIG_VERSION_ 0x010000
#endif


// Start of debugging macros:

// enables debug mode which uses seperate implementations for the container classes which preform more extensive requirement checks to warn the user about something that might cause bugs but not errors.
/*
  Debug Mode Description:
    ~~For this library I'm also implementing the debug mode code from libstdc++.
    ~~Debug mode only really affects the actual data structure implementations,
      so everything in the util/ folder will not be affected.
    ~~Debug mode works by using a macro and a conditionally inlined namespace.
      There is an internal macro that typically expands to nothing, but when you
      enable debug mode the macros create an nested namesapce(std::__cxxstd) which
      encapsulates the main implementation of the container class. Also there is
      the __debug:: namespace also nested in the main std:: namespace which is
      typically a regular nested namespace, but when debug mode is enabled the
      __debug:: namespace is declared inline which makes it assume the scope of the
      parent namespace(std::). when the __debug namespace is inlined you will
      no longer need to explicitly do std::__debug::, so doing just std:: will
      let you access anything in the __debug:: namespace. What this does is make
      the compiler replace the debug versions of the container classes because
      std:: cannot access std::__cxxstd, and instead it will use the debug
      implementation since the __debug namespace will act like it's in the
      std:: namespace.
*/
#define _DLIB_TOGGLE_DEBUGGING // This macro enables debug mode for this library.
//#define _DLIB_TOGGLE_DEBUG_PEDANTIC // enables pedantic debug checks. This macro does nothing if you're not in debug mode.
// End of debugging macros


// Start of toggle macros to configure the library:

// Leaving all of these enabled/defined by default for now.
/*
  There is also a good chance I will downgrade the implementations that are toggled
  by these macros to use the c++11, or most likely c++14(gcc-4.9.0), implementation instead.
  The code toggled by these macros will throw warnings stating the feature is not supported,
  but it will still compile and let you use the code. Inlined and templated variables will
  only work in certain situations, but for the most part they will not work correctly at all.
  I made these macros and the associated implementations purely to experiment with the Arduino IDE.
  I also was originally going to use implementations from the most current version of c++, but I
  was not able to get it working properly(some required code I couldn't implement at all).
  Instead I ended up switching to making everything use c++14(gcc-4.9.0) implementations
  which I have had no problems with implementing in a way that actually works.
*/
#define _DLIB_TOGGLE_INLINE_VARS // enables all inline variables
#define _DLIB_TOGGLE_TEMPLATE_VARS // enables all template variables

// I will most likely get rid of these constexpr macros
// They were added for the c++23(current) implementation of std::tuple which I'm unable to make with how c++11 compiles.
//#define _DLIB_TOGGLE_CPP20_CONSTEXPR // enables constexpr specifier for methods that only started using it in c++20
#define _DLIB_TOGGLE_GLIBCXX_CONSTEXPR // toggles constexpr specifier on certain methods that need it removed when using c++20 methods

// I might also get rid of the CPP20_METHODS macro since it was also added for the c++23 implementation of std::tuple which I'm not using anymore.
// I will leave this in for now incase I find a use for it again.
//#define _DLIB_TOGGLE_CPP20_METHODS // enables methods added in c++20-23

#define _DLIB_TOGGLE_SYSTEM_HEADERS // enables GCC system_header pragma in files that have it

#define _DLIB_TOGGLE_CPP17_INVOKE_R // This macro is used to switch between the c++17 and the non sfinae friendly c++11/14 implementation of __invoke_r

#define _DLIB_TOGGLE_CPP14_METHODS // This will toggle all c++14 functions(I will most likely remove this)
//#define _DLIB_TOGGLE_USES_ALLOCATOR_CPP23 // toggles between the c++23 and the c++14 bits/uses_allocator.h implementations
#define _DLIB_TOGGLE_USES_ALLOCATOR_CPP14 // toggles on the c++14 bits/uses_allocator.h implementation. If both uses_allocator macros are undefined then bits/uses_allocator wont be implemented at all.

#define _DLIB_TOGGLE_EXPERIMENTAL_FUNC // toggles experimental functions/classes that may not work properly. This can cause bugs and errors
//#define _DLIB_TOGGLE_EXPERIMENTAL_CODE // toggles experimental code such as libstdc++ code suggested in comments, and code that was commented it without a comment stating why. This can cause bugs and errors

#define _DLIB_TOGGLE_TRANSPARENT_OPS // enables transparent operators which are a c++14 feature

// I will most likely get rid of these namespace visibility macros.
//#define _DLIB_TOGGLE_USE_CUSTOM_NAMESPACE_VISIBILITY internal // allows user to set custom namespace visibility instead of none or default
#define _DLIB_TOGGLE_DEFAULT_NAMESPACE_VISIBILITY // (not used and might delete) gives the default visibility attribute to namespaces that use it

/*
  __GXX_RTTI doesn't exist in arduino IDE, so the macro bellow is to manually define it.
  You can change the default compiler flags by changing platform.txt compiler flags for arduino IDE you can enable rtti as it's disabled by default
  Theoretically when enabling RTTI in the compiler flags the __GXX_RTTI macro should automatically get defined,
  so code that requires RTTI will check for my macro AND _GXX_RTTI.
*/
//#define _DLIB_TOGGLE_RTTI_FUNCTIONALITY // this macro toggles any code that requires RTTI.

//#define _DLIB_TOGGLE_PRE_CPP11 // replaces any code from c++11 or newer with pre c++11 implementations for certain functions and implementations. This macro is mainly for testing.

#define _DLIB_TOGGLE_NEW_HEADER // toggles between the implementation I made and the implementation in the <new> header included in the arduino IDE.

//#define _DLIB_TOGGLE_PRE_CPP11_THROW // (will most likely remove) adds throw() qualifier to functions that use it pre c++11

#define _DLIB_DONT_EXPORT_TEMPLATE_DEFINITIONS // This macro toggles whether container classes such as std::vector will use the vector.hh file for out of line template definitions. Otherwise some functions will be declared but not defined. I'm not sure why this is done in libstdc++, so I might just remove this macro and force the include.

// End of toggle macros


// Start of experimental macros(these are either not implemented yet, buggy, or just outright not working):
#define _DLIB_DISABLE_CONTAINER_DYNAMIC_ALLOCATION // (NOT IMPLEMENTED AT ALL) This macro, when defined, disables dynamic allocation in containers that do dynamic memory allocation. In this case you would have to set a max size when initializing which will be the fixed max size.

#define _DLIB_TOGGLE_CUSTOM_REALLOCATION // (NOT IMPLEMENTED AT ALL) This macro when defined will make the containers capacity increase a custom amount during reallocation. The standard in libstdc++ is 2x the old capacity during reallocation, but with the macros below you can either change the multiplier or have the capacity increase at a linear rate by a set amount of elements.
// If the set multiply rate macro is defined it will take priority over the explicit rate macro
// To use a linear rate you must comment out/undefine the set multiply macro completely or set it's value to zero.
// If the custom reallocation macro is undefined/commented out then the 2 set macros will be ignored.
// These macros are for the user to interact with, but the actual macros that will be used are in the config macros section of this file.
#define _DLIB_SET_REALLOCATION_RATE_MULTIPLY 2 // (NOT IMPLEMENTED AT ALL)() This macro sets how many times the old capacity a containers new capacity will be during reallocation. by default the macro value is 2, so when a container reallocates the new capacity will be 2x the old capacity.
#define _DLIB_SET_REALLOCATION_RATE_ADD 10

#define _DLIB_TOGGLE_CUSTOM_ARDUINO_ALLOCATOR // (NOT IMPLEMENTED AT ALL)
#define _DLIB_SET_ARDUINO_ALLOCATOR_LIMIT // (NOT IMPLEMENTED AT ALL)
// End of experimental macros


// Start of config macros used in the library:
// Start of experimental macro config:
#ifdef _DLIB_TOGGLE_CUSTOM_REALLOCATION
#  ifdef _DLIB_SET_REALLOCATION_RATE_MULTIPLY
#    if (_DLIB_SET_REALLOCATION_RATE_MULTIPLY > 1)
#      define _REALLOC_LINEAR false
#      define _REALLOC_RATE _DLIB_SET_REALLOCATION_RATE_MULTIPLY
#    endif
#  elif defined(_DLIB_SET_REALLOCATION_RATE_ADD)
#    if (_DLIB_SET_REALLOCATION_RATE_ADD > 0)
#      define _REALLOC_LINEAR true
#      define _REALLOC_RATE _DLIB_SET_REALLOCATION_RATE_ADD
#    endif
#  endif
#endif

#if !defined(_REALLOC_LINEAR) || !defined(_REALLOC_RATE)
#  undef _REALLOC_LINEAR // just incase
#  undef _REALLOC_RATE // just incase
#  define _REALLOC_LINEAR false
#  define _REALLOC_RATE 2
#endif
// End of experimental macro config

#ifdef _DLIB_DONT_EXPORT_TEMPLATE_DEFINITIONS
#  define _EXPORT_TEMPLATE false
#else
#  define _EXPORT_TEMPLATE true
#endif

// I will most likely remove most of this.
#ifdef _DLIB_TOGGLE_PRE_CPP11_THROW
#  define _GLIBCXX_THROW(_EXC) throw(_EXC)
#  define _GLIBCXX_USE_NOEXCEPT throw()
#  define _GLIBCXX_NOEXCEPT // nothing
#else
#  define _GLIBCXX_THROW(_EXC) // nothing
#  define _GLIBCXX_USE_NOEXCEPT noexcept
#  define _GLIBCXX_NOEXCEPT noexcept
#endif

#ifdef _DLIB_TOGGLE_NEW_HEADER
#  define _USING_CPP_NEW false
#else
#  define _USING_CPP_NEW true
#endif

#ifdef _DLIB_TOGGLE_PRE_CPP11
#  define _CXX_OLD_CODE true
#else
#  define _CXX_OLD_CODE false
#endif

#ifdef _DLIB_TOGGLE_RTTI_FUNCTIONALITY
#  define __DLIB_RTTI true
#else
#  define __DLIB_RTTI false
#endif

#ifdef _DLIB_TOGGLE_USE_CUSTOM_NAMESPACE_VISIBILITY
#  define _DLIB_VISIBILITY _GXX_NAMESPACE_VISIBILITY(#_DLIB_TOGGLE_USE_CUSTOM_NAMESPACE_VISIBILITY)
#elif defined(_DLIB_TOGGLE_DEFAULT_NAMESPACE_VISIBILITY)
#  define _DLIB_VISIBILITY _GXX_NAMESPACE_VISIBILITY(default)
#else
#  define _DLIB_VISIBILITY // undefined
#endif

#ifdef _DLIB_TOGGLE_EXPERIMENTAL_FUNC
#  define ALLOW_EXPERIMENTAL true
#else
#  define ALLOW_EXPERIMENTAL false
#endif

#ifdef _DLIB_TOGGLE_EXPERIMENTAL_CODE
#  define TEST_CODE true
#else
#  define TEST_CODE false
#endif

#ifdef _DLIB_TOGGLE_DEBUGGING
#  define _DEBUG_ true
#else
#  define _DEBUG_ false
#endif

#if defined(_DLIB_TOGGLE_DEBUGGING) && defined(_DLIB_TOGGLE_DEBUG_PEDANTIC)
#  define _DEBUG_PEDANTIC_ true
#else
#  define _DEBUG_PEDANTIC_ false
#endif

#ifdef _DLIB_TOGGLE_TRANSPARENT_OPS
#  define _TRANSPARENT_OPERATORS_ true
#else
#  define _TRANSPARENT_OPERATORS_ false
#endif

#if defined(_DLIB_TOGGLE_USES_ALLOCATOR_CPP23) && !defined(_DLIB_TOGGLE_USES_ALLOCATOR_CPP14)
#  define USES_ALLOC_CPP23 true
#else
#  define USES_ALLOC_CPP23 false
#endif

#ifdef _DLIB_TOGGLE_USES_ALLOCATOR_CPP14
#  define USES_ALLOC_CPP14 true
#else
#  define USES_ALLOC_CPP14 false
#endif

#ifdef _DLIB_TOGGLE_CPP14_METHODS
#  define USING_CPP14_METHOD true
#else
#  define USING_CPP14_METHOD false
#endif

#ifdef _DLIB_TOGGLE_SYSTEM_HEADERS
#  define USING_SYSTEM_HEADERS true
#  define _DLIB_HEADER _Pragma("GCC system_header")
#else
#  define USING_SYSTEM_HEADERS false
#  define _DLIB_HEADER // does nothing
#endif

#ifdef _DLIB_TOGGLE_CPP20_CONSTEXPR
#  define CPP20CONSTEXPR constexpr
#else
#  define CPP20CONSTEXPR // expands to nothing
#endif

#if defined(_DLIB_TOGGLE_GLIBCXX_CONSTEXPR) && !defined(_DLIB_TOGGLE_CPP20_METHODS)
#  define GLIBCXX_CONSTEXPR constexpr
#else // if glibcxx_constexpr is not used, or if cpp20_methods is used.
#  define GLIBCXX_CONSTEXPR // nothing
#endif

#ifdef _DLIB_TOGGLE_CPP20_METHODS
#define USING_CPP20_METHOD true
#  ifdef _DLIB_TOGGLE_GLIBCXX_CONSTEXPR
#    undef GLIBCXX_CONSTEXPR
#    define GLIBCXX_CONSTEXPR
#  endif
#else
#define USING_CPP20_METHOD false
#endif

#ifdef _DLIB_TOGGLE_TEMPLATE_VARS
#  define ALLOW_TEMPLATE_VAR true
#else
#  define ALLOW_TEMPLATE_VAR false
#endif

#ifdef _DLIB_TOGGLE_INLINE_VARS
#  define ALLOW_INLINE_VAR true
#else
#  define ALLOW_INLINE_VAR false
#endif

#ifdef _DLIB_TOGGLE_CPP17_INVOKE_R
#  define CPP17_INVOKE // This just defines another macro since I already had it implemented.
#endif
// End of config macros


// Start of internal util macros:
#ifndef _DLIB_TOGGLE_DEBUGGING
#  define __glibcxx_assert(_Condition)
#else
namespace std {
  namespace __detail {

    inline void __replacement_assert(const char* __file, int __line,
                                     const char* __function, const char* __condition) {
      if(Serial) {
        // I need to test these because if either of them work this function would use alot less resources.
        //Serial.print(__file + __line + __function + " Assertion '" + __condition + "' failed.\n");
        //Serial.print(__file, __line, __function, " Assertion '", __condition, "' failed.\n");
        Serial.print(__file);
        Serial.print(__line);
        Serial.print(__function);
        Serial.print(" Assertion '");
        Serial.print(__condition);
        Serial.print("' failed.\n");
      }
      exit(0); // terminates program(Arduino will just stop and freeze until reset or code is fixed)
    } // modified for arduino

  }
}
#  define __glibcxx_assert(_Condition) \
   do { \
     if(!(_Condition)) \
       std::__detail::__replacement_assert(__FILE__, __LINE__, __PRETTY_FUNCTION__, \
                                           #_Condition); \
   } while(false)
#endif

#define _USE_DEPRECATED \
_Pragma("GCC diagnostic push") \
_Pragma("GCC diagnostic ignored \"-Wdeprecated-declarations\"")

#define _END_DEPRECATED \
_Pragma("GCC diagnostic pop")

#define _GXX_NAMESPACE_VISIBILITY(_V) __attribute__((__visibility__(#_V)))

#define _UNUSED_  __attribute__((__unused__))

#define _PURE_  __attribute__((__pure__))

namespace std {
# if (_DEBUG_ == true)
  inline namespace __debug { }
# endif
}

#if (_DEBUG_ == true)
#  define _GLIBCXX_STD_C __cxxstd
#  define _BEGIN_NAMESPACE_CONTAINER \
  namespace _GLIBCXX_STD_C {
#  define _END_NAMESPACE_CONTAINER }
#else
#  define _GLIBCXX_STD_C std
#  define _BEGIN_NAMESPACE_CONTAINER
#  define _END_NAMESPACE_CONTAINER
#endif

#define _DO_PRAGMA(_X) _Pragma(#_X)

#define _SUPPRESS_WARNINGS_START \
_Pragma("GCC diagnostic push")

#define _SUPPRESS_WARNING(_W) \
_DO_PRAGMA(GCC diagnostic ignored _W)

#define _SUPPRESS_WARNINGS_END \
_Pragma("GCC diagnostic pop")

#define _SUPPRESS_WARNING_END \
_Pragma("GCC diagnostic pop")

// macro for when suppressing a single warning.
// This just combines a single _SUPPRESS_WARNING with a _SUPPRESS_WARNINGS_START
#define _SUPPRESS_WARNING_AND_START(_W) \
_SUPPRESS_WARNINGS_START \
_DO_PRAGMA(GCC diagnostic ignored _W)

// redefining size_t typedef since arduino IDE shows that all files except .ino files see size_t as an int instead of an unsigned int.
// I'm not sure why but it seems the arduino IDE ignores its internal size_t definition in header files(.h & .hpp).
// Redefining this makes headers properly see size_t as an unsigned int.
// With this redefinition you don't need to explicitly type std::size_t as the compiler correctly deduces size_t as unsigned int even without the scope specifier.
namespace std _DLIB_VISIBILITY {
  typedef unsigned int size_t;
  
}

#ifndef _GLIBCXX_THROW_OR_ABORT
#  if (__EXCEPTIONS)
#    define _GLIBCXX_THROW_OR_ABORT(_EXC) (throw(_EXC))
#  else
#    define _GLIBCXX_THROW_OR_ABORT(_EXC) (__builtin_abort())
#  endif
#endif

// End of internal util macros


#endif // End of Config.h
