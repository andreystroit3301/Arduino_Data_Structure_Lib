#include <stdio.h>
// Libstdc++ and GCC GNU Lincese boilerplate:      (this is required since this code is copyrighted under the GCC compiler. I did not design this myself.)
/*
  Partial Modified Implementation Of Meta-Functions from the <debug/debug.h|macros.h|functions.h|formatter.h> Libstdc++ header  -*- C++ -*-

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
  Debug.hpp  [V1.0.0]  (debugging macro function header)
    By: Andrey Stroitelev

  ~This file is used for debugging the util part of the library.
  ~These macro functions replace some standard behaviors in the library with debug behavior for when a bug occurs and you need more info.
  ~This file is mostly just a copy of 3 files from the debug folder in libstdc++ (debug/macros.g, debug/debug.h, debug/functions.h, and debug/formatter.h)
  ~I may add some of my own code to this file to add debugging macros for the code I made myself once I get to making the data structures for this library.
  ~There is a chance I might just move all of these macros into the Config.h file just to reduce the amount of files there is in util.
  ~For the formatter.h and functions.h implementation here I tried to make whatever functions I could constexpr and/or inline.
    *Many of these functions are originally both not inline or constexpr, and some are just inline.
*/
/*
  TO DO:
    *Figure out if in the macro __glibcxx_check_heap_pred should use std::__detail::__is_heap or std::__is_heap
    *line 887 update attribute to be [[maybe_unused]], [[gnu::unused]] or add macro for __attribute__((__unused__))
*/


// Start of Debug.h
#ifndef _DLIB_DEBUGGING_HPP_
#define _DLIB_DEBUGGING_HPP_

#ifndef _DLIB_DEBUGGING_VERSION_
#define _DLIB_DEBUGGING_VERSION_ 0x010000
#endif


// Start of includes for the actual non-macro function part of this file:
#include <Arduino.h>
#include "Config.h"
#include "Iterator_Traits.hpp"
#include "Type_Traits.hpp"
#include "Move.hpp"
#include "UtilityBase.hpp"
#include "Stdexcept.hpp"
#include "Pair.hpp"
// End of includes


// Creating public debug namespace
namespace std {
  namespace __debug { }
}


// Creating dedicated internal debug namespace
namespace __gnu_debug {
  using namespace std::__debug;
}


// Macro definitions for when debugging is not used:
#if (_DEBUG_ == false) // checks for debug boolean macro in Config.h

#define _GLIBCXX_DEBUG_ASSERT(_Condition)
#define _GLIBCXX_DEBUG_PEDASSERT(_Condition)
#define _GLIBCXX_DEBUG_ONLY(_Statement) ;
#define __glibcxx_requires_cond(_Cond, _Msg)
#define __glibcxx_requires_valid_range(_First, _Last)
#define __glibcxx_requires_non_empty_range(_First, _Last)
#define __glibcxx_requires_sorted(_First, _Last)
#define __glibcxx_requires_sorted_pred(_First, _Last)
#define __glibcxx_requires_sorted_set(_First1, _Last1, _First2)
#define __glibcxx_requires_sorted_set_pred(_First1, _Last1, _First2, _Pred)
#define __glibcxx_requires_partitioned_lower(_First, _Last, _Value)
#define __glibcxx_requires_partitioned_upper(_First, _Last, _Value)
#define __glibcxx_requires_partitioned_lower_pred(_First, _Last, _Value, _Pred)
#define __glibcxx_requires_partitioned_upper_pred(_First, _Last, _Value, _Pred)
#define __glibcxx_requires_heap(_First, _Last)
#define __glibcxx_requires_heap_pred(_First, _Last, _Pred)
#define __glibcxx_requires_nonempty()
#define __glibcxx_requires_string(_String)
#define __glibcxx_requires_string_len(_String, _Len)
#define __glibcxx_requires_subscript(_N)
// End of disabled debugging macro definitions


#else // start definitions for when debug is enabled:


// Start of debug/macros.h file implementation:
#define _GLIBCXX_DEBUG_VERIFY_AT(_Condition, _ErrorMessage, _File, _Line) \
  do { \
    if(!(_Condition)) \
      __gnu_debug::_Error_formatter::_M_at(_File, _Line)._ErrorMessage._M_error(); \
  } while(false)

#define _GLIBCXX_DEBUG_VERIFY(_Condition, _ErrorMessage) \
  _GLIBCXX_DEBUG_VERIFY_AT(_Condition, _ErrorMessage, __FILE__, __LINE__) 
  // __FILE__ gets the path of the file giving the error, and __LINE__ gives the line where the error occurs

#define __glibcxx_check_valid_range(_First, _Last) \
  _GLIBCXX_DEBUG_VERIFY(__gnu_debug::__valid_range(_First, _Last), \
                        _M_message(__gnu_debug::__msg_valid_range) \
                        ._M_iterator(_First, #_First) \
                        ._M_iterator(_Last, #_Last))

#define __glibcxx_check_non_empty_range(_First, _Last) \
  _GLIBCXX_DEBUG_VERIFY(_First != _Last, \
                        _M_message(__gnu_debug::__msg_non_empty_range) \
                        ._M_iterator(_First, #_First) \
                        ._M_iterator(_Last, #_Last)))

#define __glibcxx_check_insert(_Position) \
  _GLIBCXX_DEBUG_VERIFY(!_Position._M_singular(), \
                        _M_message(__gnu_debug::__msg_insert_singular) \
                        ._M_sequence(*this, "this") \
                        ._M_iterator(_Position, #_Position)); \
  _GLIBCXX_DEBUG_VERIFY(_Position._M_attached_to(this), \
                        _M_message(__gnu_debug::__msg_insert_different) \
                        ._M_sequence(*this, "this") \
                        ._M_iterator(_Position, #_Position))

#define __glibcxx_check_insert_after(_Position) \
  __glibcxx_check_insert(_Position); \
  _GLIBCXX_DEBUG_VERIFY(!_Position._M_is_end(), \
                        _M_message(__gnu_debug::__msg_insert_after_end) \
                        ._M_sequence(*this, "this") \
                        ._M_iterator(_Position, #_Position))

#define __glibcxx_check_insert_range(_Position, _First, _Last) \
  __glibcxx_check_valid_range(_First, _Last); \
  __glibcxx_check_insert(_Position); \
  _GLIBCXX_DEBUG_VERIFY(__gnu_debug::__foreign_iterator(_Position, _First, _Last), \
                        _M_message(__gnu_debug::__msg_insert_range_from_self) \
                        ._M_iterator(_First, #_First) \
                        ._M_iterator(_Last, #_Last) \
                        ._M_sequence(*this, "this")) 

#define __glibcxx_check_insert_range_after(_Position, _First, _Last) \
  __glibcxx_check_valid_range(_First, _Last); \
  __glibcxx_check_insert_after(_Position); \
  _GLIBCXX_DEBUG_VERIFY(__gnu_debug::__foreign_iterator(_Position, _First, _Last), \
                        _M_message(__gnu_debug::__msg_insert_range_from_self) \
                        ._M_iterator(_First, #_First) \
                        ._M_iterator(_Last, #_Last) \
                        ._M_sequence(*this, "this"))

#define __glibcxx_check_erase(_Position) \
  _GLIBCXX_DEBUG_VERIFY(_Position._M_dereferenceable(), \
                        _M_message(__gnu_debug::__msg_erase_bad) \
                        ._M_sequence(*this, "this") \
                        ._M_iterator(_Position, #_Position)); \
  _GLIBCXX_DEBUG_VERIFY(_Position._M_attached_to(this), \
                        _M_message(__gnu_debug::__msg_erase_different) \
                        ._M_sequence(*this, "this") \
                        ._M_iterator(_Position, #_Position))

#define __glibcxx_check_erase_after(_Position) \
  _GLIBCXX_DEBUG_VERIFY(_Position._M_before_dereferenceable(), \
                        _M_message(__gnu_debug::__msg_erase_after_bad) \
                        ._M_sequence(*this, "this") \
                        ._M_iterator(_Position, #_Position)); \
  _GLIBCXX_DEBUG_VERIFY(_Position._M_attached_to(this), \
                        _M_message(__gnu_debug::__msg_erase_different) \
                        ._M_sequence(*this, "this") \
                        ._M_iterator(_Position, #_Position))

#define __glibcxx_check_erase_range(_First, _Last) \
  __glibcxx_check_valid_range(_First, _Last); \
  _GLIBCXX_DEBUG_VERIFY(_First._M_attached_to(this), \
                        _M_message(__gnu_debug::__msg_erase_different) \
                        ._M_sequence(*this, "this") \
                        ._M_iterator(_First, #_First) \
                        ._M_iterator(_Last, #_Last))

#define __glibcxx_check_erase_range_after(_First, _Last) \
  _GLIBCXX_DEBUG_VERIFY(_First._M_can_compare(_Last), \
                        _M_message(__gnu_debug::__msg_erase_different) \
                        ._M_sequence(*this, "this") \
                        ._M_iterator(_First, #_First) \
                        ._M_iterator(_Last, #_Last)); \
  _GLIBCXX_DEBUG_VERIFY(_First._M_attached_to(this), \
                        _M_message(__gnu_debug::__msg_erase_different) \
                        ._M_sequence(*this, "this") \
                        ._M_iterator(_First, #_First)); \
  _GLIBCXX_DEBUG_VERIFY(_First != _Last, \
                        _M_message(__gnu_debug::__msg_valid_range2) \
                        ._M_sequence(*this, "this") \
                        ._M_iterator(_First, #_First) \
                        ._M_iterator(_Last, #_Last)); \
  _GLIBCXX_DEBUG_VERIFY(_First._M_incrementable(), \
                        _M_message(__gnu_debug::__msg_valid_range2) \
                        ._M_sequence(*this, "this") \
                        ._M_iterator(_First, #_First) \
                        ._M_iterator(_Last, #_Last)); \
  _GLIBCXX_DEBUG_VERIFY(!_Last._M_is_before_begin(), \
                        _M_message(__gnu_debug::__msg_valid_range2) \
                        ._M_sequence(*this, "this") \
                        ._M_iterator(_First, #_First) \
                        ._M_iterator(_Last, #_Last)) // possibly add another backslash here

#define __glibcxx_check_subscript(_N) \
  _GLIBCXX_DEBUG_VERIFY(_N < this->size(), \
                        _M_message(__gnu_debug::__msg_subscript_oob) \
                        ._M_sequence(*this, "this") \
                        ._M_integer(_N, #_N) \
                        ._M_integer(this->size(), "size"))

#define __glibcxx_check_bucket_index(_N) \
  _GLIBCXX_DEBUG_VERIFY(_N < this->bucket_count(), \
                        _M_message(__gnu_debug::__msg_bucket_index_oob) \
                        ._M_sequence(*this, "this") \
                        ._M_integer(_N, #_N) \
                        ._M_integer(this->bucket_count(), "size"))

#define __glibcxx_check_nonempty() \
  _GLIBCXX_DEBUG_VERIFY(!this->empty(), \
                        _M_message(__gnu_debug::__msg_empty) \
                        ._M_sequence(*this, "this"))

#define __glibcxx_check_sorted(_First, _Last) \
  __glibcxx_check_valid_range(_First, _Last); \
  _GLIBCXX_DEBUG_VERIFY(__gnu_debug::__check_sorted( \
                          __gnu_debug::__base(_First), \
                          __gnu_debug::__base(_Last)), \
                        _M_message(__gnu_debug::__msg_unsorted) \
                        ._M_iterator(_First, #_First) \
                        ._M_iterator(_Last, #_Last))

#define __glibcxx_check_sorted_pred(_First, _Last, _Pred) \
  __glibcxx_check_valid_range(_First, _Last); \
  _GLIBCXX_DEBUG_VERIFY(__gnu_debug::__check_sorted(__gnu_debug::__base(_First), \
                                                    __gnu_debug::__base(_Last), _Pred), \
                        _M_message(__gnu_debug::__msg_unsorted_pred) \
                        ._M_iterator(_First, #_First) \
                        ._M_iterator(_Last, #_Last) \
                        ._M_string(#_Pred))

#define __glibcxx_check_sorted_set(_First1, _Last1, _First2) \
  __glibcxx_check_valid_range(_First1, _Last1); \
  _GLIBCXX_DEBUG_VERIFY(__gnu_debug::__check_sorted_set(__gnu_debug::__base(_First1), \
                                                        __gnu_debug::__base(_Last1), _First2), \
                        _M_message(__gnu_debug::__msg_unsorted) \
                        ._M_iterator(_First1, #_First1) \
                        ._M_iterator(_Last1, #_Last1))

#define __glibcxx_check_sorted_set_pred(_First1, _Last1, _First2, _Pred) \
  __glibcxx_check_valid_range(_First1, _Last1); \
  _GLIBCXX_DEBUG_VERIFY(__gnu_debug::__check_sorted_set(__gnu_debug::__base(_First), \
                                                        __gnu_debug::__base(_Last1), \
                                                        _First2, _Pred), \
                        _M_message(__gnu_debug::__msg_unsorted_pred) \
                        ._M_iterator(_First1, #_First1) \
                        ._M_iterator(_Last1, #_Last1) \
                        ._M_string(#_Pred))

#define __glibcxx_check_partitioned_lower(_First, _Last, _Value) \
  __glibcxx_check_valid_range(_First, _Last); \
  _GLIBCXX_DEBUG_VERIFY(__gnu_debug::__check_partitioned_lower(__gnu_debug::__base(_First), \
                                                               __gnu_debug::__base(_Last), _Value), \
                        _M_message(__gnu_debug::__msg_unpartitioned) \
                        ._M_iterator(_First, #_First) \
                        ._M_iterator(_Last, #_Last) \
                        ._M_string(#_Value))

#define __glibcxx_check_partitioned_upper(_First, _Last, _Value) \
  __glibcxx_check_valid_range(_First, _Last); \
  _GLIBCXX_DEBUG_VERIFY(__gnu_debug::__check_partitioned_upper(__gnu_debug::__base(_First), \
                                                               __gnu_debug::__base(_Last), _Value), \
                        _M_message(__gnu_debug::__msg_unpartitioned) \
                        ._M_iterator(_First, #_First) \
                        ._M_iterator(_Last, #_Last) \
                        ._M_string(#_Value))

#define __glibcxx_check_partitioned_lower_pred(_First, _Last, _Value, _Pred) \
  __glibcxx_check_valid_range(_First, _Last); \
  _GLIBCXX_DEBUG_VERIFY(__gnu_debug::__check_partitioned_lower(__gnu_debug::__base(_First), \
                                                               __gnu_debug::__base(_Last), _Value, _Pred), \
                        _M_message(__gnu_debug::__msg_unpartitioned_pred) \
                        ._M_iterator(_First, #_First) \
                        ._M_iterator(_Last, #_Last) \
                        ._M_string(#_Pred) \
                        ._M_string(#_Value))

#define __glibcxx_check_paritioned_upper_pred(_First, _Last, _Value, _Pred) \
  __glibcxx_check_valid_range(_First, _Last); \
  _GLIBCXX_DEBUG_VERIFY(__gnu_debug::__check_partitioned_upper(__gnu_debug::__base(_First), \
                                                               __gnu_debug::__base(_Last), _Value, _Pred), \
                        _M_message(__gnu_debug::__msg_unpartitioned_pred) \
                        ._M_iterator(_First, #_First) \
                        ._M_iterator(_Last, #_Last) \
                        ._M_string(#_Pred) \
                        ._M_string(#_Value))

#define __glibcxx_check_heap(_First, _Last) \
  _GLIBCXX_DEBUG_VERIFY(std::__is_heap(__gnu_debug::__base(_First), \
                                       __gnu_debug::__base(_Last)), \
                        _M_message(__gnu_debug::__msg_not_heap) \
                        ._M_iterator(_First, #_First) \
                        ._M_iterator(_Last, ,#_Last))

#define __glibcxx_check_heap_pred(_First, _Last, _Pred) \
  _GLIBCXX_DEBUG_VERIFY(std::__is_heap(__gnu_debug::__base(_First), \
                                       __gnu_debug::__base(_Last), _Pred), \
                        _M_message(__gnu_debug::__msg_not_heap_pred) \
                        ._M_iterator(_First, #_First) \
                        ._M_iterator(_Last, #_Last) \
                        ._M_string(#_Pred))

#define __glibcxx_check_self_move_assign(_Other) \
  _GLIBCXX_DEBUG_VERIFY(this != &_Other, \
                        _M_message(__gnu_debug::__msg_self_move_assign) \
                        ._M_sequence(*this, "this"))

#define __glibcxx_check_max_load_factor(_F) \
  _GLIBCXX_DEBUG_VERIFY(_F > 0.0f, \
                        _M_message(__gnu_debug::__msg_valid_load_factor) \
                        ._M_sequence(*this, "this"))

// I might get of this equal_allocs macro since im not sure if I'll implement a dedicated allocator class at all
#define __glibcxx_check_equal_allocs(_Other) \
  _GLIBCXX_DEBUG_VERIFY(this->get_allocator() == _Other.get_allocator(), \
                        _M_message(__gnu_debug::__msg_equal_allocs) \
                        ._M_sequence(*this, "this"))

#if (_DEBUG_PEDANTIC_)
#  define __glibcxx_check_string(_String) _GLIBCXX_DEBUG_ASSERT(_String != 0)
#  define __glibcxx_check_string_len(_String, _Len) _GLIBCXX_DEBUG_ASSERT(_String != 0 || _Len == 0)
#else
#  define __glibcxx_check_string(_String)
#  define __glibcxx_check_string_len(_String, _len)
#endif
// End of debug/macros.h file implementation


// Continuation of debug/debug.h implementation:
#define _GLIBCXX_DEBUG_ASSERT(_Condition) __glibcxx_assert(_Condition)

#if (_DEBUG_PEDANTIC_)
#  define _GLIBCXX_DEBUG_PEDASSERT(_Condition) _GLIBCXX_DEBUG_ASSERT(_Condition)
#else
#  define _GLIBCXX_DEBUG_PEDASSERT(_Condition)
#endif // pedantic assertions will only do anything if pedantic debugging is enabled.

#define _GLIBCXX_DEBUG_ONLY(_Statement) _Statement

#define __glibcxx_requires_cond(_Cond, _Msg) _GLIBCXX_DEBUG_VERIFY(_Cond, _Msg)
#define __glibcxx_requires_valid_range(_First, _Last) \
  __glibcxx_check_valid_range(_First, _Last)
#define __glibcxx_requires_non_empty_range(_First, _Last) \
  __glibcxx_check_non_empty_range(_First, _Last)
#define __glibcxx_requires_sorted(_First, _Last) \
  __glibcxx_check_sorted(_First, _last)
#define __glibcxx_requires_sorted_pred(_First, _Last, _Pred) \
  __glibcxx_check_sorted_pred(_First, _Last, _Pred)
#define __glibcxx_requires_sorted_set(_First1, _Last1, _First2) \
  __glibcxx_check_sorted_set(_First1, _Last1, _First2)
#define __glibcxx_requires_sorted_set_pred(_First1, _Last1, _First2, _Pred) \
  __glibcxx_check_sorted_set_pred(_First1, _Last1, _First2, _Pred)
#define __glibcxx_requires_partitioned_lower(_First, _Last, _Value) \
  __glibcxx_check_partitioned_lower(_First, _Last, _Value)
#define __glibcxx_requires_partitioned_upper(_First, _Last, _Value) \
  __glibcxx_check_partitioned_upper(_First, _Last, _Value)
#define __glibcxx_requires_partitioned_lower_pred(_First, _Last, _Value, _Pred) \
  __glibcxx_check_partitioned_lower_pred(_First, _Last, _Value, _Pred)
#define __glibcxx_requires_partitioned_upper_pred(_First, _Last, _Value, _Pred) \
  __glibcxx_check_partitioned_upper_pred(_First, _Last, _Value, _Pred)
#define __glibcxx_requires_heap(_First, _Last) \
  __glibcxx_check_heap(_First, _Last)
#define __glibcxx_requires_heap_pred(_First, _Last, _Pred) \
  __glibcxx_check_heap_pred(_First, _Last, _Pred)
#define __glibcxx_requires_nonempty() __glibcxx_check_nonempty()
#define __glibcxx_requires_string(_String) __glibcxx_check_string(_String)
#define __glibcxx_requires_string_len(_String, _Len) __glibcxx_check_string_len(_String, _Len)
#define __glibcxx_requires_subscript(_N) __glibcxx_check_subscript(_N)
// End of debug/debug.h implementation


// Start of debug/formatter.h implementation:

namespace __gnu_debug { // dedicated internal namespace for debugging
  //using std::type_info; // not using this since arduino IDE acts wierd with using std::<something> and using namespace std
  using type_info = std::type_info; // trying this instead of the above line.

  template<class _Iterator>
  inline bool __check_singular(const _Iterator&);

  class _Safe_sequence_base;

  template<class _Iterator, class _Sequence>
  class _Safe_iterator;

  template<class _Iterator, class _Sequence>
  class _Safe_local_iterator;

  template<class _Iterator>
  class _Safe_sequence;

  enum _Debug_msg_id {
    // General checks
    __msg_valid_range,
    __msg_insert_singular,
    __msg_insert_different,
    __msg_erase_bad,
    __msg_erase_different,
    __msg_subscript_oob,
    __msg_empty,
    __msg_unpartitioned,
    __msg_unpartitioned_pred,
    __msg_unsorted,
    __msg_unsorted_pred,
    __msg_not_heap,
    __msg_not_heap_pred,
    // std::bitset checks
    __msg_bad_bitset_write,
    __msg_bad_bitset_read,
    __msg_bad_bitset_flip,
    // std::list checks
    __msg_self_splice,
    __msg_splice_alloc,
    __msg_splice_bad,
    __msg_splice_other,
    __msg_splice_overlap,
    // iterator checks
    __msg_init_singular,
    __msg_init_copy_singular,
    __msg_init_const_singular,
    __msg_copy_singular,
    __msg_bad_deref,
    __msg_bad_inc,
    __msg_bad_dec,
    __msg_iter_subscript_oob,
    __msg_advance_oob,
    __msg_retreat_oob,
    __msg_iter_compare_bad,
    __msg_compare_different,
    __msg_iter_order_bad,
    __msg_order_different,
    __msg_distance_bad,
    __msg_distance_different,
    // istream_iterator
    __msg_deref_istream,
    __msg_inc_istream,
    // ostream_iterator
    __msg_output_ostream,
    // istreambuf_iterator
    __msg_deref_istreambuf,
    __msg_inc_istreambuf,
    // forward_list
    __msg_insert_after_end,
    __msg_erase_after_bad,
    __msg_valid_range2,
    // unordered container local iterators
    __msg_local_iter_compare_bad,
    __msg_non_empty_range,
    // self move assign
    __msg_self_move_assign,
    // unordered container buckets
    __msg_bucket_index_oob,
    __msg_valid_load_factor,
    // others
    __msg_equal_allocs,
    __msg_insert_range_from_self
  }; // end of enum _Debug_msg_id


  // Start of _Error_formatter implementation
  class _Error_formatter {
    enum _Constness {
      __unknown_constness,
      __const_iterator,
      __mutable_iterator,
      __last_constness
    };

    enum _Iterator_state {
      __unknown_state,
      __singular,
      __begin,
      __middle,
      __end,
      __before_begin,
      __last_state
    };

    struct _Is_iterator { };
    struct _Is_sequence { };

    struct _Parameter {
      enum {
        __unused_param,
        __iterator,
        __sequence,
        __integer,
        __string
      } _M_kind;

      union {
        struct {
          const char* _M_name;
          const void* _M_address;
          const type_info* _M_type;
          _Constness _M_constness;
          _Iterator_state _M_state;
          const void* _M_sequence;
          const type_info* _M_seq_type;
        } _M_iterator;

        struct {
          const char* _M_name;
          const void* _M_address;
          const type_info* _M_type;
        } _M_sequence;

        struct {
          const char* _M_name;
          long _M_value;
        } _M_integer;

        struct {
          const char* _M_name;
          const char* _M_value;
        } _M_string;
      } _M_variant; // end of union in _Parameter

      _Parameter() : _M_kind(__unused_param), _M_variant() { }

      _Parameter(long __value, const char* __name) : _M_kind(__integer), _M_variant() {
        _M_variant._M_integer._M_name = __name;
        _M_variant._M_integer._M_value = __value;
      }

      _Parameter(const char* __value, const char* __name) : _M_kind(__string), _M_variant() {
        _M_variant._M_string._M_name = __name;
        _M_variant._M_string._M_value = __value;
      }

      template<class _Iterator, class _Sequence>
      _Parameter(const _Safe_iterator<_Iterator, _Sequence>& __it, const char* __name, _Is_iterator)
        : _M_kind(__iterator), _M_variant() {
        _M_variant._M_iterator._M_name = __name;
        _M_variant._M_iterator._M_address = &__it;
#       if defined(__GXX_RTTI) || (__DLIB_RTTI)
        _M_variant._M_iterator._M_type = &typeid(__it);
#       else
        _M_variant._M_iterator._M_type = 0;
#       endif
        _M_variant._M_iterator._M_constness 
          = std::__detail::__are_same<_Safe_iterator<_Iterator, _Sequence>,
                                      typename _Sequence::iterator>::
                            __value ? __mutable_iterator : __const_iterator;
        _M_variant._M_iterator._M_sequence = __it._M_get_sequence();
#       if defined(__GXX_RTTI) || (_DLIB_RTTI)
        _M_variant._M_iterator._M_seq_type = &typeid(_Sequence);
#       else
        _M_variant._M_iterator._M_seq_type = 0;
#       endif

        if(__it._M_singular()) {
          _M_variant._M_iterator._M_state = __singular;
        } else {
          if(__it._M_is_before_begin()) _M_variant._M_iterator._M_state = __before_begin;
          else if(__it._M_is_end()) _M_variant._M_iterator._M_state = __end;
          else if(__it._M_is_begin()) _M_variant._M_iterator._M_state = __begin;
          else _M_variant._M_iterator._M_state = __middle;
        }
      }

      template<class _Iterator, class _Sequence>
      _Parameter(const _Safe_local_iterator<_Iterator, _Sequence>& __it, const char* __name, _Is_iterator)
        : _M_kind(__iterator), _M_variant() {
        _M_variant._M_iterator._M_name = __name;
        _M_variant._M_iterator._M_address = &__it;
#       if defined(__GXX_RTTI) || (__DLIB_RTTI)
        _M_variant._M_iterator._M_type = &typeid(__it);
#       else
        _M_variant._M_iterator._M_type = 0;
#       endif
        _M_variant._M_iterator._M_constness
          = std::__detail::__are_same<_Safe_local_iterator<_Iterator, _Sequence>,
                                      typename _Sequence::local_iterator>::
                            __value ? __mutable_iterator : __const_iterator;
        _M_variant._M_iterator._M_sequence = __it._M_get_sequence();
#       if defined(__GXX_RTTI) || (__DLIB_RTTI)
        _M_variant._M_iterator._M_seq_type = &typeid(_Sequence);
#       else
        _M_variant._M_iterator._M_seq_type = 0;
#       endif
        if(__it._M_singular()) {
          _M_variant._M_iterator._M_state = __singular;
        } else {
          if(__it._M_is_end()) _M_variant._M_iterator._M_state = __end;
          else if(__it._M_is_begin()) _M_variant._M_iterator._M_state = __begin;
          else _M_variant._M_iterator._M_state = __middle;
        }
      }

      template<class _Type>
      _Parameter(const _Type*& __it, const char* __name, _Is_iterator) : _M_kind(__iterator), _M_variant() {
        _M_variant._M_iterator._M_name = __name;
        _M_variant._M_iterator._M_address = &__it;
#       if defined(__GXX_RTTI) || (__DLIB_RTTI)
        _M_variant._M_iterator._M_type = &typeid(__it);
#       else
        _M_variant._M_iterator._M_type = 0;
#       endif
        _M_variant._M_iterator._M_constness = __mutable_iterator;
        _M_variant._M_iterator._M_state = __it ? __unknown_state : __singular;
        _M_variant._M_iterator._M_sequence = 0;
        _M_variant._M_iterator._M_seq_type = 0;    
      }

      template<class _Type>
      _Parameter(_Type*& __it, const char* __name, _Is_iterator) : _M_kind(__iterator), _M_variant() {
        _M_variant._M_iterator._M_name = __name;
        _M_variant._M_iterator._M_address = &__it;
#       if defined(__GXX_RTTI) || (__DLIB_RTTI)
        _M_variant._M_iterator._M_type = &typeid(__it);
#       else
        _M_variant._M_iterator._M_type = 0;
#       endif
        _M_variant._M_iterator._M_constness = __const_iterator;
        _M_variant._M_iterator._M_state = __it ? __unknown_state : __singular;
        _M_variant._M_iterator._M_sequence = 0;
        _M_variant._M_iterator._M_seq_type = 0;
      }

      template<class _Iterator>
      _Parameter(const _Iterator& __it, const char* __name, _Is_iterator)
        : _M_kind(__iterator), _M_variant() {
        _M_variant._M_iterator._M_name = __name;
        _M_variant._M_iterator._M_address = &__it;
#       if defined(__GXX_RTTI) || (__DLIB_RTTI)
        _M_variant._M_iterator._M_type = &typeid(__it);
#       else
        _M_variant._M_iterator._M_type = 0;
#       endif
        _M_variant._M_iterator._M_constness = __unknown_constness;
        _M_variant._M_iterator._M_state
          = __gnu_debug::__check_singular(__it) ? __singular : __unknown_state;
        _M_variant._M_iterator._M_sequence = 0;
        _M_variant._M_iterator._M_seq_type = 0;
      }

      template<class _Sequence>
      _Parameter(const _Safe_sequence<_Sequence>& __seq, const char* __name, _Is_sequence)
        : _M_kind(__sequence), _M_variant() {
        _M_variant._M_sequence._M_name = __name;
        _M_variant._M_sequence._M_address = static_cast<const _Sequence*>(&__seq);
#       if defined(__GXX_RTTI) || (__DLIB_RTTI)
        _M_variant._M_sequence._M_type = &typeid(_Sequence);
#       else
        _M_variant._M_sequence._M_type = 0;
#       endif
      }

      template<class _Sequence>
      _Parameter(const _Sequence& __seq, const char* __name, _Is_sequence)
        : _M_kind(__sequence), _M_variant() {
        _M_variant._M_sequence._M_name = __name;
        _M_variant._M_sequence._M_address = &__seq;
#       if defined(__GXX_RTTI) || (__DLIB_RTTI)
        _M_variant._M_sequence._M_type = &typeid(_Sequence);
#       else
        _M_variant._M_sequence._M_type = 0;
#       endif
      }

      void _M_print_field(const _Error_formatter* __formatter, const char* __name) const;

      void _M_print_description(const _Error_formatter* __formatter) const;
    }; // end of member struct _Parameter

    friend struct _Parameter;

    public: // _Error_formatter class
      template<class _Iterator>
      const _Error_formatter& _M_iterator(const _Iterator& __it, const char* __name = 0) const {
        if(_M_num_parameters < size_t(__max_parameters))
          _M_parameters[_M_num_parameters++] = _Parameter(__it, __name, _Is_iterator());

        return *this;
      }

      const _Error_formatter& _M_integer(long __value, const char* __name = 0) const {
        if(_M_num_parameters < size_t(__max_parameters))
          _M_parameters[_M_num_parameters++] = _Parameter(__value, __name);
        
        return *this;
      }

      const _Error_formatter& _M_string(const char* __value, const char* __name = 0) const {
        if(_M_num_parameters < size_t(__max_parameters))
          _M_parameters[_M_num_parameters++] = _Parameter(__value, __name);

        return *this;
      }

      template<class _Sequence>
      const _Error_formatter& _M_sequence(const _Sequence& __seq, const char* __name = 0) const {
        if(_M_num_parameters < size_t(__max_parameters))
          _M_parameters[_M_num_parameters++] = _Parameter(__seq, __name, _Is_sequence());
        
        return *this;
      }

      const _Error_formatter& _M_message(const char* __text) const {
        _M_text = __text;
        return *this;
      }

      const _Error_formatter& _M_message(_Debug_msg_id __id) const throw(); // not sure what this will do with exceptions disabled. I may add a macro to toggle between throw(), noexcept, and nothing. From what I understand throw() does the same as noexcept but calls std::undetermined instead of std::terminate

      [[noreturn]] void _M_error() const;
    
    private:
      _Error_formatter(const char* __file, size_t __line)
        : _M_file(__file), _M_line(__line), _M_num_parameters(0), _M_text(0),
          _M_max_length(78), _M_column(1), _M_first_line(true), _M_wordwrap(false) { _M_get_max_length(); }
      
      template<class _Tp>
      void _M_format_word(char*, int, const char*, _Tp) const throw();

      void _M_print_word(const char* __word) const;

      void _M_print_string(const char* __string) const;

      void _M_get_max_length() const throw();

      enum { __max_parameters = 9 };

      const char*           _M_file;
      size_t                _M_line;
      mutable _Parameter    _M_parameters[__max_parameters];
      mutable size_t        _M_num_parameters;
      mutable const char*   _M_text;
      mutable size_t        _M_max_length;
      enum { _M_indent = 2 };
      mutable size_t        _M_column;
      mutable bool          _M_first_line;
      mutable bool          _M_wordwrap;

    public:
      static _Error_formatter _M_at(const char* __file, size_t __line) {
        return _Error_formatter(__file, __line);
      }

  }; // end of _Error_formatter class

} // end of __gnu_debug:: namespace

// End of debug/formatter.h implementation


// Start of debug/functions.h implementation:

// This namespace is seperate from the one above to keep the file implementations seperate.
namespace __gnu_debug { // start of internal namespace for debugging

  template<class _Iterator, class _Sequence>
  class _Safe_iterator; // this was already declared in formatter.h and is re-declared in functions.h. I'm not sure why this doesn't cause a redeclaration error.

  template<class _Iterator, class _Sequence>
  class _Safe_local_iterator;

  template<class _Sequence>
  struct _Insert_range_from_self_is_safe {
    enum { __value = 0 };
  };

  template<class _Sequence>
  struct _Is_contiguous_sequence : public std::__detail::__false_type { };

  inline bool __check_singular_aux(const void*) { return false; }

  template<class _Iterator>
  inline bool __check_singular(const _Iterator& __x) { return __check_singular_aux(&__x); }

  template<class _Tp>
  inline bool __check_singular(const _Tp* __ptr) { return __ptr == 0; }

  template<class _Iterator>
  inline bool __check_dereferenceable(const _Iterator&) { return true; }

  template<class _Tp>
  inline bool __check_dereferenceable(const _Tp* __ptr) { return __ptr; }

  template<class _Iterator, class _Sequence>
  inline bool __check_dereferenceable(const _Safe_iterator<_Iterator, _Sequence>& __x) {
    return __x._M_dereferenceable();
  }

  template<class _Iterator, class _Sequence>
  inline bool __check_dereferenceable(const _Safe_local_iterator<_Iterator, _Sequence>& __x) {
    return __x._M_dereferenceable();
  }

  template<class _RandomAccessIterator>
  inline bool __valid_range_aux2(const _RandomAccessIterator& __first,
                                 const _RandomAccessIterator& __last,
                                 std::random_access_iterator_tag) {
    return __last - __first >= 0;
  }

  template<class _InputIterator>
  inline bool __valid_range_aux2(const _InputIterator&, const _InputIterator&,
                                 std::input_iterator_tag) { return true; }
  
  template<class _Integral>
  inline bool __valid_range_aux(const _Integral&, const _Integral&, 
                                std::__detail::__true_type) { return true; }
  
  template<class _InputIterator>
  inline bool __valid_range_aux(const _InputIterator& __first,
                                const _InputIterator& __last,
                                std::__detail::__false_type) {
    return __valid_range_aux2(__first, __last, std::__detail::__iterator_category(__first));
  }

  template<class _InputIterator>
  inline bool __valid_range(const _InputIterator& __first, const _InputIterator& __last) {
    typedef typename std::__detail::__is_integer<_InputIterator>::__type _Integral;
    return __valid_range_aux(__first, __last, _Integral());
  }

  template<class _Iterator, class _Sequence>
  inline bool __valid_range(const _Safe_iterator<_Iterator, _Sequence>& __first,
                            const _Safe_iterator<_Iterator, _Sequence>& __last) {
    return __first._M_valid_range(__last);
  }

  template<class _Iterator, class _Sequence>
  inline bool __valid_range(const _Safe_local_iterator<_Iterator, _Sequence>& __first,
                            const _Safe_local_iterator<_Iterator, _Sequence>& __last) {
    return __first._M_valid_range(__last);
  }

  template<class _InputIterator>
  inline _InputIterator __check_valid_range(const _InputIterator& __first,
                                            const _InputIterator& __last _UNUSED_) { 
    __glibcxx_check_valid_range(__first, __last);
    return __first;
  }

  template<class _Iterator, class _Sequence>
  inline bool __foreign_iterator_aux4(const _Safe_iterator<_Iterator, _Sequence>& __it,
                                      const typename _Sequence::value_type* __other) {
    typedef const typename _Sequence::value_type* _PointerType;
    typedef std::less<_PointerType> _Less;
    constexpr _Less __l{};
    const _Sequence* __seq = __it._M_get_sequence();
    const _PointerType __begin = std::__detail::__addressof(*__seq->_M_base().begin());
    const _PointerType __end = std::__detail::__addressof(*(__seq->_M_base().end() - 1));
    return __l(__other, __begin) || __l(__end, __other);
  }

  template<class _Iterator, class _Sequence>
  inline bool __foreign_iterator_aux4(const _Safe_iterator<_Iterator, _Sequence>&, ...) {
    return true;
  }

  template<class _Iterator, class _Sequence, class _InputIterator>
  inline bool __foreign_iterator_aux3(const _Safe_iterator<_Iterator, _Sequence>& __it,
                                      const _InputIterator& __other,
                                      const _InputIterator& __other_end,
                                      std::__detail::__true_type) {
    if(__other == __other_end) return true;
    if(__it._M_get_sequence()->begin() == __it._M_get_sequence()->end()) return true;
    return __foreign_iterator_aux4(__it, std::__detail::__addressof(*__other));
  }

  template<class _Iterator, class _Sequence, class _InputIterator>
  inline bool __foreign_iterator_aux3(const _Safe_iterator<_Iterator, _Sequence>&,
                                      const _InputIterator&, const _InputIterator&,
                                      std::__detail::__false_type) { return true; }
  
  template<class _Iterator, class _Sequence, class _OtherIterator>
  inline bool __foreign_iterator_aux2(const _Safe_iterator<_Iterator, _Sequence>& __it,
                                      const _Safe_iterator<_OtherIterator, _Sequence>& __other,
                                      const _Safe_iterator<_OtherIterator, _Sequence>&) {
    return __it._M_get_sequence() != __other._M_get_sequence();
  }

  template<class _Iterator, class _Sequence, class _OtherIterator, class _OtherSequence>
  inline bool __foreign_iterator_aux2(const _Safe_iterator<_Iterator, _Sequence>& __it,
                                      const _Safe_iterator<_OtherIterator, _OtherSequence>&,
                                      const _Safe_iterator<_OtherIterator, _OtherSequence>&) {
    return true;
  }

  template<class _Iterator, class _Sequence, class _InputIterator>
  inline bool __foreign_iterator_aux2(const _Safe_iterator<_Iterator, _Sequence>& __it,
                                      const _InputIterator& __other,
                                      const _InputIterator& __other_end) {
    return __foreign_iterator_aux3(__it, __other, __other_end, _Is_contiguous_sequence<_Sequence>());
  }

  template<class _Iterator, class _Sequence, class _Integral>
  inline bool __foreign_iterator_aux(const _Safe_iterator<_Iterator, _Sequence>&,
                                     _Integral, _Integral, std::__detail::__true_type) {
    return true;
  }

  template<class _Iterator, class _Sequence, class _InputIterator>
  inline bool __foreign_iterator_aux(const _Safe_iterator<_Iterator, _Sequence>& __it,
                                     _InputIterator __other, _InputIterator __other_end,
                                     std::__detail::__false_type) {
    return _Insert_range_from_self_is_safe<_Sequence>::__value || __foreign_iterator_aux2(__it, __other, __other_end);
  }

  template<class _Iterator, class _Sequence, class _InputIterator>
  inline bool __foreign_iterator(const _Safe_iterator<_Iterator, _Sequence>& __it,
                                 _InputIterator __other, _InputIterator __other_end) {
    typedef typename std::__detail::__is_integer<_InputIterator>::__type _Integral;
    return __foreign_iterator_aux(__it, __other, __other_end, _Integral());
  }

  template<class _CharT, class _Integer>
  inline const _CharT* __check_string(const _CharT* __s, const _Integer& __n _UNUSED_) {
#   if(_DEBUG_PEDANTIC_)
    __glibcxx_assert(__s != 0 || __n == 0);
#   endif
    return __s;
  }

  template<class _CharT>
  inline const _CharT* __check_string(const _CharT* __s) {
#   if(_DEBUG_PEDANTIC_)
    __glibcxx_assert(__s != 0);
#   endif
    return __s;
  }

  template<class _InputIterator>
  inline bool __check_sorted_aux(const _InputIterator&, const _InputIterator&,
                                 std::input_iterator_tag) { return true; }
  
  template<class _ForwardIterator>
  inline bool __check_sorted_aux(_ForwardIterator __first, _ForwardIterator __last,
                                 std::forward_iterator_tag) {
    if(__first == __last) return true;
    _ForwardIterator __next = __first;
    for(++__next; __next != __last; __first = __next, ++__next) {
      if(*__next < *__first) return false;
    }
    return true;
  }

  template<class _InputIterator, class _Predicate>
  inline bool __check_sorted_aux(const _InputIterator&, const _InputIterator&,
                                 _Predicate, std::input_iterator_tag) { return true; }
  
  template<class _ForwardIterator, class _Predicate>
  inline bool __check_sorted_aux(_ForwardIterator __first, _ForwardIterator __last,
                                 _Predicate __pred, std::forward_iterator_tag) {
    if(__first == __last) return true;
    _ForwardIterator __next = __first;
    for(++__next; __next != __last; __first = __next, ++__next) {
      if(__pred(*__next, *__first)) return false;
    }
    return true;
  }

  template<class _InputIterator>
  inline bool __check_sorted(const _InputIterator& __first, const _InputIterator& __last) {
    __glibcxx_assert(__first == __last || !(*__first < *__first));
    return __check_sorted_aux(__first, __last, std::__detail::__iterator_category(__first));
  }

  template<class _InputIterator, class _Predicate>
  inline bool __check_sorted(const _InputIterator& __first, const _InputIterator& __last, _Predicate __pred) {
    __glibcxx_assert(__first == __last || !__pred(*__first, *__first));
    return __check_sorted_aux(__first, __last, __pred, std::__detail::__iterator_category(__first));
  }

  template<class _InputIterator>
  inline bool __check_sorted_set_aux(const _InputIterator& __first,
                                     const _InputIterator& __last,
                                     std::__detail::__true_type) {
    return __check_sorted(__first, __last);
  }

  template<class _InputIterator>
  inline bool __check_sorted_set_aux(const _InputIterator&, const _InputIterator&,
                                     std::__detail::__false_type) { return true; }

  template<class _InputIterator, class _Predicate>
  inline bool __check_sorted_set_aux(const _InputIterator& __first,
                                     const _InputIterator& __last,
                                     _Predicate __pred, std::__detail::__true_type) {
    return __check_sorted(__first, __last, __pred);
  }

  template<class _InputIterator, class _Predicate>
  inline bool __check_sorted_set_aux(const _InputIterator&, const _InputIterator&,
                                     _Predicate, std::__detail::__false_type) { return true; }

  template<class _InputIterator1, class _InputIterator2>
  inline bool __check_sorted_set(const _InputIterator1& __first,
                                 const _InputIterator1& __last,
                                 const _InputIterator2&) {
    typedef typename std::iterator_traits<_InputIterator1>::value_type _ValueType1;
    typedef typename std::iterator_traits<_InputIterator2>::value_type _ValueType2;
    typedef typename std::__detail::__are_same<_ValueType1, _ValueType2>::__type _SameType;
    return __check_sorted_set_aux(__first, __last, _SameType());
  }

  template<class _InputIterator1, class _InputIterator2, class _Predicate>
  inline bool __check_sorted_set(const _InputIterator1& __first,
                                 const _InputIterator1& __last,
                                 const _InputIterator2&, _Predicate __pred) {
    typedef typename std::iterator_traits<_InputIterator1>::value_type _ValueType1;
    typedef typename std::iterator_traits<_InputIterator2>::value_type _ValueType2;
    typedef typename std::__detail::__are_same<_ValueType1, _ValueType2>::__type _SameType;
    return __check_sorted_set_aux(__first, __last, __pred, _SameType());
  }

  template<class _ForwardIterator, class _Tp>
  inline bool __check_partitioned_lower(_ForwardIterator __first, _ForwardIterator __last, const _Tp& __value) {
    while(__first != __last && *__first < __value) ++__first;
    if(__first != __last) {
      ++__first;
      while(__first != __last && !(*__first < __value)) ++__first;
    }
    return __first == __last;
  }

  template<class _ForwardIterator, class _Tp>
  inline bool __check_partitioned_upper(_ForwardIterator __first, _ForwardIterator __last, const _Tp& __value) {
    while(__first != __last && !(__value < *__first)) ++__first;
    if(__first != __last) {
      ++__first;
      while(__first != __last && __value < *__first) ++__first;
    }
    return __first == __last;
  }

  template<class _ForwardIterator, class _Tp, class _Pred>
  inline bool __check_partitioned_lower(_ForwardIterator __first, _ForwardIterator __last,
                                        const _Tp& __value, _Pred __pred) {
    while(__first != __last && bool(__pred(*__first, __value))) ++__first;
    if(__first != __last) {
      ++__first;
      while(__first != __last && !bool(__pred(*__first, __value))) ++__first;
    }
    return __first == __last;
  }

  template<class _ForwardIterator, class _Tp, class _Pred>
  inline bool __check_partitioned_upper(_ForwardIterator __first, _ForwardIterator __last,
                                        const _Tp& __value, _Pred __pred) {
    while(__first != __last && !bool(__pred(__value, *__first))) ++__first;
    if(__first != __last) {
      ++__first;
      while(__first != __last && bool(__pred(__value, *__first))) ++__first;
    }
    return __first == __last;
  }

  template<class _Iterator>
  struct __is_safe_random_iterator {
    enum { __value = 0 };
    typedef std::__detail::__false_type __type;
  };

  template<class _Iterator, class _Sequence>
  struct __is_safe_random_iterator<_Safe_iterator<_Iterator, _Sequence>>
    : public std::__detail::__are_same<std::random_access_iterator_tag,
                                       typename std::iterator_traits<_Iterator>::iterator_category> { };

  template<class _Iterator>
  struct _Siter_base : public std::__detail::_Iter_base<_Iterator, __is_safe_random_iterator<_Iterator>::__value> { };

  template<class _Iterator>
  inline typename _Siter_base<_Iterator>::iterator_type __base(_Iterator __it) {
    return _Siter_base<_Iterator>::_S_base(__it);
  }

} // end of __gnu_debug:: namespace

// End of debug/functions.h implementation


// Start of the libstdc++ debug/safe_base header implementation:
//   NOTE: This implementation has all multi-threading related code removed since the processor architecture in Arduino AVR boards doesn't support multi-threading.

namespace __gnu_debug { // namespace for internal debugging implementations

  class _Safe_sequence_base;

  class _Safe_iterator_base {
    public:
      _Safe_sequence_base* _M_sequence;

      unsigned int _M_version;

      _Safe_iterator_base* _M_prior;

      _Safe_iterator_base* _M_next;
    
    protected:
      _Safe_iterator_base() : _M_sequence(0), _M_version(0), _M_prior(0), _M_next(0) { }

      _Safe_iterator_base(const _Safe_sequence_base* __seq, bool __constant)
        : _M_sequence(0), _M_version(0), _M_prior(0), _M_next(0) {
        this->_M_attach(const_cast<_Safe_sequence_base*>(__seq), __constant);
      }

      _Safe_iterator_base(const _Safe_iterator_base& __x, bool __constant)
        : _M_sequence(0), _M_version(0), _M_prior(0), _M_next(0) {
        this->_M_attach(__x._M_sequence, __constant);    
      }

      _Safe_iterator_base& operator=(const _Safe_iterator_base&);

      explicit _Safe_iterator_base(const _Safe_iterator_base&);

      ~_Safe_iterator_base() { this->_M_detach(); }

      // __gnu_cxx::__mutex& _M_get_mutex throw(); // not implementing this since arduino AVR boards cannot multi-thread.

    public:
      void _M_attach(_Safe_sequence_base* __seq, bool __constant);

      void _M_attach_single(_Safe_sequence_base* __seq, bool __constant) throw();

      void _M_detach();

      void _M_detach_single() throw();

      bool _M_attached_to(const _Safe_sequence_base* __seq) const {
        return _M_sequence == __seq;
      }

      _PURE_ bool _M_singular() const throw();

      _PURE_ bool _M_can_compare(const _Safe_iterator_base& __x) const throw();

      void _M_invalidate() { _M_version = 0; }

      void _M_reset() throw();

      void _M_unlink() throw() {
        if(_M_prior) _M_prior->_M_next = _M_next;
        if(_M_next) _M_next->_M_prior = _M_prior;
      }
  }; // end of _Safe_iterator_base class definition


  class _Safe_sequence_base {
    public:
      _Safe_iterator_base* _M_iterators;

      _Safe_iterator_base* _M_const_iterators;

      mutable unsigned int _M_version;
    
    protected:
      _Safe_sequence_base() : _M_iterators(0), _M_const_iterators(0), _M_version(1) { }

      _Safe_sequence_base(const _Safe_sequence_base&) noexcept : _Safe_sequence_base() { }

      _Safe_sequence_base(_Safe_sequence_base&& __x) noexcept : _Safe_sequence_base() { _M_swap(__x); }

      ~_Safe_sequence_base() { this->_M_detach_all(); }

      void _M_detach_all();

      void _M_detach_singular();

      void _M_revalidate_singular();

      void _M_swap(_Safe_sequence_base& __x);

      // __gnu_cxx::__mutex& _M_get_mutex() throw(); // not implementing this since arduino AVR boards cannot multi-thread.

      public:
        void _M_invalidate_all() const {
          if(++_M_version == 0) _M_version = 1;
        }

        void _M_attach(_Safe_iterator_base* __it, bool __constant);

        void _M_attach_single(_Safe_iterator_base* __it, bool __constant) throw();

        void _M_detach(_Safe_iterator_base* __it);

        void _M_detach_single(_Safe_iterator_base* __it) throw();
  }; // end of _Safe_sequence_base class definition

} // end of __gnu_debug:: namespace

// End of libstdc++ debug/safe_base header implementation


// Start of the libstdc++ debug/safe_sequence header implementation:

namespace __gnu_debug { // namespace for internal debugging implementations

  template<class _Iterator, class _Sequence>
  class _Safe_iterator;


  template<class _Type>
  class _Not_equal_to {
    _Type __value;

    public:
      explicit _Not_equal_to(const _Type& __v) : __value(__v) { }

      bool operator()(const _Type& __x) const {
        return __value != __x;
      }
  }; // end of _Not_equal_to


  template<class _Type>
  class _Equal_to {
    _Type __value;

    public:
      explicit _Equal_to(const _Type& __v) : __value(__v) { }

      bool operator()(const _Type& __x) const {
        return __value == __x;
      }
  }; // end of _Not_equal_to


  template<class _Iterator>
  class _After_nth_from {
    typedef typename std::iterator_traits<_Iterator>::difference_type difference_type;

    _Iterator _M_base;
    difference_type _M_n;

    public:
      _After_nth_from(const difference_type& __n, const _Iterator& __base)
        : _M_base(__base), _M_n(__n) { }
      
      bool operator()(const _Iterator& __x) const {
        return __x - _M_base >= _M_n;
      }
  }; // end of _After_nth_from


  template<class _Sequence>
  class _Safe_sequence : public _Safe_sequence_base {
    public:
      template<class _Predicate>
      void _M_invalidate_if(_Predicate __pred);

      template<class _Predicate>
      void _M_transfer_from_if(_Safe_sequence& __from, _Predicate __pred);
  }; // end of _Safe_sequence class definition

} // End of __gnu_debug:: namespace

// End of libstdc++ debug/safe_sequence header implementation


// Start of libstdc++ debug/safe_sequence.tcc template definition file implementation:
//   NOTE: I commented out all code that is supposed to be included through ext/concurrence.h since I didn't implement it and its required includes.(This would've required 2000-3000 lines of mostly placeholder code)
//         The rest of this code that uses the code included from ext/concurrence will still be implemented, but it will be disabled by a test macro.
//         I will also try to modify the macro toggled test code to work without the ext/concurrence methods and objects such as __mutex.
//     P.S.: I might just be interpreting the file wrong, so there is a chance that the macro enabled code should be left in with only __gnu_cxx:: namespace calls being commented out.
namespace __gnu_debug { // Internal debug namespace for safe_sequence out of line definitions

  template<class _Sequence>
  template<class _Predicate>
  void _Safe_sequence<_Sequence>::_M_invalidate_if(_Predicate __pred) {
    typedef typename _Sequence::iterator iterator;
    typedef typename _Sequence::const_iterator const_iterator;

    //__gnu_cxx::__scoped_lock sentry(this->_M_get_mutex()); // not implemented
    for(_Safe_iterator_base* __iter = _M_iterators; __iter;) {
      iterator* __victim = static_cast<iterator*>(__iter);
      __iter = __iter->_M_next;
      if(!__victim->_M_singular() && __pred(__victim->base())) {
        __victim->_M_invalidate();
      }
    }

    for(_Safe_iterator_base* __iter2 = _M_const_iterators; __iter2;) {
      const_iterator* __victim = static_cast<const_iterator*>(__iter2);
      __iter2 = __iter2->_M_next;
      if(!__victim->_M_singular() && __pred(__victim->base())) {
        __victim->_M_invalidate();
      }
    }
  } // end of _M_invalidate_if


  template<class _Sequence>
  template<class _Predicate>
  void _Safe_sequence<_Sequence>::_M_transfer_from_if(_Safe_sequence& __from, _Predicate __pred) {
    typedef typename _Sequence::iterator iterator;
    typedef typename _Sequence::const_iterator const_iterator;

    _Safe_iterator_base* __transfered_iterators = 0;
    _Safe_iterator_base* __transfered_const_iterators = 0;
    _Safe_iterator_base* __last_iterator = 0;
    _Safe_iterator_base* __last_const_iterator = 0;
    { // lone code block
      
      //__gnu_cxx::__scoped_lock sentry(__from._M_get_mutex()); // not implemented
      for(_Safe_iterator_base* __iter = __from._M_iterators; __iter;) {
        iterator* __victim = static_cast<iterator*>(__iter);
        __iter = __iter->_M_next;
        if(!__victim->_M_singular() && __pred(__victim->base())) {
          __victim->_M_detach_single();
          if(__transfered_iterators) {
            __victim->_M_next = __transfered_iterators;
            __transfered_iterators->_M_prior = __victim;
          } else __last_iterator = __victim;
          __victim->_M_sequence = this;
          __victim->_M_version = this->_M_version;
          __transfered_iterators = __victim;
        }
      }

      for(_Safe_iterator_base* __iter2 = __from._M_const_iterators; __iter2;) {
        const_iterator* __victim = static_cast<const_iterator*>(__iter2);
        __iter2 = __iter2->_M_next;
        if(!__victim->_M_singular() && __pred(__victim->base())) {
          __victim->_M_detach_single();
          if(__transfered_const_iterators) {
            __victim->_M_next = __transfered_const_iterators;
            __transfered_const_iterators->_M_prior = __victim;
          } else __last_const_iterator = __victim;
          __victim->_M_sequence = this;
          __victim->_M_version = this->_M_version;
          __transfered_const_iterators = __victim;
        }
      }

    } // End of lone code block

#   if(_USING_MULTITHREAD)
    if(__last_iterator || __last_const_iterator) {
      // __gnu_cxx::__scoped_lock sentry(this->_M_get_mutex()); // not implemented
      if(__last_iterator) {
        if(this->_M_iterators) {
          this->_M_iterators->_M_prior = __last_iterator;
          __last_iterator->_M_next = this->_M_iterators;
        }
        this->_M_iterators = __transfered_iterators;
      }
      if(__last_const_iterator) {
        if(this->_M_const_iterators) {
          this->_M_const_iterators->_M_prior = __last_const_iterator;
          __last_const_iterator->_M_next = this->_M_const_iterators;
        }
        this->_M_const_iterators = __transfered_const_iterators;
      }
    }
#   endif
  } // end of _M_transfer_from_if

} // End of __gnu_debug:: namespace

// End of libstdc++ debug/safe_sequence.tcc template definition file implementation


// Start of the libstdc++ debug/safe_iterator header implementation:

namespace __gnu_debug { // Start of internal debug namespace

  template<class _Sequence>
  struct _BeforeBeginHelper {
    template<class _Iterator>
    static bool _S_Is(const _Safe_iterator<_Iterator, _Sequence>&) { return false; }

    template<class _Iterator>
    static bool _S_Is_Beginnest(const _Safe_iterator<_Iterator, _Sequence>& __it) {
      return __it.base() == __it._M_get_sequence()->_M_base().begin();
    }
  };


  inline bool __check_singular_aux(const _Safe_iterator_base* __x) {
    return __x->_M_singular();
  }


  enum _Distance_precision {
    __dp_equality,
    __dp_sign,
    __dp_exact
  };


  template<class _Iterator>
  inline std::pair<typename std::iterator_traits<_Iterator>::difference_type, _Distance_precision>
  __get_distance(const _Iterator& __lhs, const _Iterator& __rhs, std::random_access_iterator_tag) {
    return std::make_pair(__rhs - __lhs, __dp_exact);
  }

  template<class _Iterator>
  inline std::pair<typename std::iterator_traits<_Iterator>::difference_type, _Distance_precision>
  __get_distance(const _Iterator& __lhs, const _Iterator& __rhs, std::forward_iterator_tag) {
    return std::make_pair(__lhs == __rhs ? 0 : 1, __dp_equality);
  }

  template<class _Iterator>
  inline std::pair<typename std::iterator_traits<_Iterator>::difference_type, _Distance_precision>
  __get_distance(const _Iterator& __lhs, const _Iterator& __rhs) {
    typedef typename std::iterator_traits<_Iterator>::iterator_category _Category;
    return __get_distance(__lhs, __rhs, _Category());
  }


  template<class _Iterator, class _Sequence>
  class _Safe_iterator : public _Safe_iterator_base {
    typedef _Safe_iterator _Self;
    typedef typename _Sequence::const_iterator _Const_iterator;

    _Iterator _M_current;

    bool _M_constant() const {
      return std::__detail::__are_same<_Const_iterator, _Safe_iterator>::__value;
    }

    typedef std::iterator_traits<_Iterator> _Traits;

    public:
      typedef _Iterator iterator_type;
      typedef typename _Traits::iterator_category iterator_category;
      typedef typename _Traits::value_type value_type;
      typedef typename _Traits::difference_type difference_type;
      typedef typename _Traits::reference reference;
      typedef typename _Traits::pointer pointer;

      _Safe_iterator() noexcept : _M_current() { }

      _Safe_iterator(const _Iterator& __i, const _Sequence* __seq) noexcept
        : _Safe_iterator_base(__seq, _M_constant()), _M_current(__i) {
        _GLIBCXX_DEBUG_VERIFY(!this->_M_singular(),
                              _M_message(__msg_init_singular)
                              ._M_iterator(*this, "this"));
      }

      _Safe_iterator(const _Safe_iterator& __x) noexcept
        : _Safe_iterator_base(__x, _M_constant()), _M_current(__x._M_current) {
        _GLIBCXX_DEBUG_VERIFY(!__x._M_singular()
                              || __x._M_current == _Iterator(),
                              _M_message(__msg_init_copy_singular)
                              ._M_iterator(*this, "this")
                              ._M_iterator(__x, "other"));
      }

      _Safe_iterator(_Safe_iterator&& __x) noexcept : _M_current() {
        _GLIBCXX_DEBUG_VERIFY(!__x._M_singular()
                              || __x._M_current == _Iterator(),
                              _M_message(__msg_init_copy_singular)
                              ._M_iterator(*this, "this")
                              ._M_iterator(__x, "other"));
        std::swap(_M_current, __x._M_current);
        this->_M_attach(__x._M_sequence);
        __x._M_detach();
      }

      template<class _MutableIterator>
      _Safe_iterator(const _Safe_iterator<_MutableIterator,
                                          typename __gnu_cxx::__enable_if<(std::__detail::__are_same<_MutableIterator,
                                                                                                     typename _Sequence::iterator::iterator_type>::__value),
                                          _Sequence>::__type>& __x) noexcept
        : _Safe_iterator_base(__x, _M_constant()), _M_current(__x.base()) {
        _GLIBCXX_DEBUG_VERIFY(!__x._M_singular()
                              || __x.base() == _Iterator(),
                              _M_message(__msg_init_const_singular)
                              ._M_iterator(*this, "this")
                              ._M_iterator(__x, "other"));
      }

      _Safe_iterator& operator=(const _Safe_iterator& __x) noexcept {
        _GLIBCXX_DEBUG_VERIFY(!__x._M_singular()
                              || __x._M_current == _Iterator(),
                              _M_message(__msg_copy_singular)
                              ._M_iterator(*this, "this")
                              ._M_iterator(__x, "other"));
        _M_current = __x._M_current;
        this->_M_attach(__x._M_sequence);
        return *this;
      }

      _Safe_iterator& operator=(_Safe_iterator&& __x) noexcept {
        _GLIBCXX_DEBUG_VERIFY(this != &__x,
                              _M_message(__msg_self_move_assign)
                              ._M_iterator(*this, "this"));
        _GLIBCXX_DEBUG_VERIFY(!__x._M_singular()
                              || __x._M_current == _Iterator(),
                              _M_message(__msg_copy_singular)
                              ._M_iterator(*this, "this")
                              ._M_iterator(__x, "other"));
        _M_current = __x._M_current;
        _M_attach(__x._M_sequence);
        __x._M_detach();
        __x._M_current = _Iterator();
        return *this;
      }

      reference operator*() const noexcept {
        _GLIBCXX_DEBUG_VERIFY(this->_M_dereferenceable(),
                              _M_message(__msg_bad_deref)
                              ._M_iterator(*this, "this"));
        return *_M_current;
      }

      pointer operator->() const noexcept {
        _GLIBCXX_DEBUG_VERIFY(this->_M_dereferenceable(),
                              _M_message(__msg_bad_deref)
                              ._M_iterator(*this, "this"));
        return std::__detail::__addressof(*_M_current);
      }

      _Safe_iterator& operator++() noexcept {
        _GLIBCXX_DEBUG_VERIFY(this->_M_incrementable(),
                              _M_message(__msg_bad_inc)
                              ._M_iterator(*this, "this"));
        ++_M_current;
        return *this;
      }

      _Safe_iterator operator++(int) noexcept {
        _GLIBCXX_DEBUG_VERIFY(this->_M_incrementable(),
                              _M_message(__msg_bad_inc)
                              ._M_iterator(*this, "this"));
        _Safe_iterator __tmp(*this);
        ++_M_current;
        return __tmp;
      }

      _Safe_iterator& operator--() noexcept {
        _GLIBCXX_DEBUG_VERIFY(this->_M_decrementable(),
                              _M_message(__msg_bad_dec)
                              ._M_iterator(*this, "this"));
        --_M_current;
        return *this;
      }

      _Safe_iterator operator--(int) noexcept {
        _GLIBCXX_DEBUG_VERIFY(this->_M_decrementable(),
                              _M_message(__msg_bad_dec)
                              ._M_iterator(*this, "this"));
        _Safe_iterator __tmp(*this);
        --_M_current;
        return __tmp;
      }

      reference operator[](const difference_type& __n) const noexcept {
        _GLIBCXX_DEBUG_VERIFY(this->_M_can_advance(__n)
                              && this->_M_can_advance(__n + 1),
                              _M_message(__msg_iter_subscript_oob)
                              ._M_iterator(*this)._M_integer(__n));
        return _M_current[__n];
      }

      _Safe_iterator& operator+=(const difference_type& __n) noexcept {
        _GLIBCXX_DEBUG_VERIFY(this->_M_can_advance(__n),
                              _M_message(__msg_advance_oob)
                              ._M_iterator(*this)._M_integer(__n));
        _M_current += __n;
        return *this;
      }

      _Safe_iterator operator+(const difference_type& __n) const noexcept {
        _Safe_iterator __tmp(*this);
        __tmp += __n;
        return __tmp;
      }

      _Safe_iterator& operator-=(const difference_type& __n) noexcept {
        _GLIBCXX_DEBUG_VERIFY(this->_M_can_advance(-__n),
                              _M_message(__msg_retreat_oob)
                              ._M_iterator(*this)._M_integer(__n));
        _M_current += -__n;
        return *this;
      }

      _Safe_iterator operator-(const difference_type& __n) const noexcept {
        _Safe_iterator __tmp(*this);
        __tmp -= __n;
        return __tmp;
      }

      _Iterator base() const noexcept { return _M_current; }

      operator _Iterator() const noexcept { return _M_current; }

      void _M_attach(_Safe_sequence_base* __seq) {
        _Safe_iterator_base::_M_attach(__seq, _M_constant());
      }

      void _M_attach_single(_Safe_sequence_base* __seq) {
        _Safe_iterator_base::_M_attach_single(__seq, _M_constant());
      }

      bool _M_dereferenceable() const {
        return !this->_M_singular() && !_M_is_end() && !_M_is_before_begin();
      }

      bool _M_before_dereferenceable() const {
        if(this->_M_incrementable()) {
          _Iterator __base = base();
          return ++__base != _M_get_sequence()->_M_base().end();
        }
        return false;
      }

      bool _M_incrementable() const {
        return !this->_M_singular() && !_M_is_end();
      }

      bool _M_decrementable() const {
        return !_M_singular() && !_M_is_begin();
      }

      bool _M_can_advance(const difference_type& __n) const;

      bool _M_valid_range(const _Safe_iterator& __rhs) const;

      typename __gnu_cxx::__conditional_type<std::__detail::__are_same<_Const_iterator, _Safe_iterator>::__value,
                                             const _Sequence*, _Sequence*>::__type
      _M_get_sequence() const {
        return static_cast<_Sequence*>(_M_sequence);
      }

      bool _M_is_begin() const {
        return base() == _M_get_sequence()->_M_base().begin();
      }

      bool _M_is_end() const {
        return base() == _M_get_sequence()->_M_base().end();
      }

      bool _M_is_before_begin() const {
        return _BeforeBeginHelper<_Sequence>::_S_Is(*this);
      }

      bool _M_is_beginnest() const {
        return _BeforeBeginHelper<_Sequence>::_S_Is_Beginnest(*this);
      }
  }; // end of _Safe_iterator wrapper class


  template<class _IteratorL, class _IteratorR, class _Sequence>
  inline bool operator==(const _Safe_iterator<_IteratorL, _Sequence>& __lhs,
                         const _Safe_iterator<_IteratorR, _Sequence>& __rhs) noexcept {
    _GLIBCXX_DEBUG_VERIFY(!__lhs._M_singular() && !__rhs._M_singular(),
                          _M_message(__msg_iter_compare_bad)
                          ._M_iterator(__lhs, "lhs")
                          ._M_iterator(__rhs, "rhs)"));
    _GLIBCXX_DEBUG_VERIFY(__lhs._M_can_compare(__rhs),
                          _M_message(__msg_compare_different)
                          ._M_iterator(__lhs, "lhs")
                          ._M_iterator(__rhs, "rhs"));
    return __lhs.base() == __rhs.base();
  }

  template<class _Iterator, class _Sequence>
  inline bool operator==(const _Safe_iterator<_Iterator, _Sequence>& __lhs,
                         const _Safe_iterator<_Iterator, _Sequence>& __rhs) noexcept {
    _GLIBCXX_DEBUG_VERIFY(!__lhs._M_singular() && !__rhs._M_singular(),
                          _M_message(__msg_iter_compare_bad)
                          ._M_iterator(__lhs, "lhs")
                          ._M_iterator(__rhs, "rhs"));
    _GLIBCXX_DEBUG_VERIFY(__lhs._M_can_compare(__rhs),
                          _M_message(__msg_compare_different)
                          ._M_iterator(__lhs, "lhs")
                          ._M_iterator(__rhs, "rhs"));
    return __lhs.base() == __rhs.base();
  }


  template<class _IteratorL, class _IteratorR, class _Sequence>
  inline bool operator!=(const _Safe_iterator<_IteratorL, _Sequence>& __lhs,
                         const _Safe_iterator<_IteratorR, _Sequence>& __rhs) noexcept {
    _GLIBCXX_DEBUG_VERIFY(!__lhs._M_singular() && !__rhs._M_singular(),
                          _M_message(__msg_iter_compare_bad)
                          ._M_iterator(__lhs, "lhs")
                          ._M_iterator(__rhs, "rhs"));
    _GLIBCXX_DEBUG_VERIFY(__lhs._M_can_compare(__rhs),
                          _M_message(__msg_compare_different)
                          ._M_iterator(__lhs, "lhs")
                          ._M_iterator(__rhs, "rhs"));
    return __lhs.base() != __rhs.base();
  }

  template<class _Iterator, class _Sequence>
  inline bool operator!=(const _Safe_iterator<_Iterator, _Sequence>& __lhs,
                         const _Safe_iterator<_Iterator, _Sequence>& __rhs) noexcept {
    _GLIBCXX_DEBUG_VERIFY(!__lhs._M_singular() && !__rhs._M_singular(),
                          _M_message(__msg_iter_compare_bad)
                          ._M_iterator(__lhs, "lhs")
                          ._M_iterator(__rhs, "rhs"));
    _GLIBCXX_DEBUG_VERIFY(__lhs._M_can_compare(__rhs),
                          _M_message(__msg_compare_different)
                          ._M_iterator(__lhs, "lhs")
                          ._M_iterator(__rhs, "rhs"));
    return __lhs.base() != __rhs.base();
  }


  template<class _IteratorL, class _IteratorR, class _Sequence>
  inline bool operator<(const _Safe_iterator<_IteratorL, _Sequence>& __lhs,
                        const _Safe_iterator<_IteratorR, _Sequence>& __rhs) noexcept {
    _GLIBCXX_DEBUG_VERIFY(!__lhs._M_singular() && !__rhs._M_singular(),
                          _M_message(__msg_iter_order_bad)
                          ._M_iterator(__lhs, "lhs")
                          ._M_iterator(__rhs, "rhs"));
    _GLIBCXX_DEBUG_VERIFY(__lhs._M_can_compare(__rhs),
                          _M_message(__msg_order_different)
                          ._M_iterator(__lhs, "lhs")
                          ._M_iterator(__rhs, "rhs"));
    return __lhs.base() < __rhs.base();
  }

  template<class _Iterator, class _Sequence>
  inline bool operator<(const _Safe_iterator<_Iterator, _Sequence>& __lhs,
                        const _Safe_iterator<_Iterator, _Sequence>& __rhs) noexcept {
    _GLIBCXX_DEBUG_VERIFY(!__lhs._M_singular() && !__rhs._M_singular(),
                          _M_message(__msg_iter_order_bad)
                          ._M_iterator(__lhs, "lhs")
                          ._M_iterator(__rhs, "rhs"));
    _GLIBCXX_DEBUG_VERIFY(__lhs._M_can_compare(__rhs),
                          _M_message(__msg_order_different)
                          ._M_iterator(__lhs, "lhs")
                          ._M_iterator(__rhs, "rhs"));
    return __lhs.base() < __rhs.base();
  }


  template<class _IteratorL, class _IteratorR, class _Sequence>
  inline bool operator<=(const _Safe_iterator<_IteratorL, _Sequence>& __lhs,
                         const _Safe_iterator<_IteratorR, _Sequence>& __rhs) noexcept {
    _GLIBCXX_DEBUG_VERIFY(!__lhs._M_singular() && !__rhs._M_singular(),
                          _M_message(__msg_iter_order_bad)
                          ._M_iterator(__lhs, "lhs")
                          ._M_iterator(__rhs, "rhs"));
    _GLIBCXX_DEBUG_VERIFY(__lhs._M_can_compare(__rhs),
                          _M_message(__msg_order_different)
                          ._M_iterator(__lhs, "lhs")
                          ._M_iterator(__rhs, "rhs"));
    return __lhs.base() <= __rhs.base();
  }

  template<class _Iterator, class _Sequence>
  inline bool operator<=(const _Safe_iterator<_Iterator, _Sequence>& __lhs,
                         const _Safe_iterator<_Iterator, _Sequence>& __rhs) noexcept {
    _GLIBCXX_DEBUG_VERIFY(!__lhs._M_singular() && !__rhs._M_singular(),
                          _M_message(__msg_iter_order_bad)
                          ._M_iterator(__lhs, "lhs")
                          ._M_iterator(__rhs, "rhs"));
    _GLIBCXX_DEBUG_VERIFY(__lhs._M_can_compare(__rhs),
                          _M_message(__msg_order_different)
                          ._M_iterator(__lhs, "lhs")
                          ._M_iterator(__rhs, "rhs"));
    return __lhs.base() <= __rhs.base();
  }


  template<class _IteratorL, class _IteratorR, class _Sequence>
  inline bool operator>(const _Safe_iterator<_IteratorL, _Sequence>& __lhs,
                        const _Safe_iterator<_IteratorR, _Sequence>& __rhs) noexcept {
    _GLIBCXX_DEBUG_VERIFY(!__lhs._M_singular() && !__rhs._M_singular(),
                          _M_message(__msg_iter_order_bad)
                          ._M_iterator(__lhs, "lhs")
                          ._M_iterator(__rhs, "rhs"));
    _GLIBCXX_DEBUG_VERIFY(__lhs._M_can_compare(__rhs),
                          _M_message(__msg_order_different)
                          ._M_iterator(__lhs, "lhs")
                          ._M_iterator(__rhs, "rhs"));
    return __lhs.base() > __rhs.base();
  }

  template<class _Iterator, class _Sequence>
  inline bool operator>(const _Safe_iterator<_Iterator, _Sequence>& __lhs,
                        const _Safe_iterator<_Iterator, _Sequence>& __rhs) noexcept {
    _GLIBCXX_DEBUG_VERIFY(!__lhs._M_singular() && !__rhs._M_singular(),
                          _M_message(__msg_iter_order_bad)
                          ._M_iterator(__lhs, "lhs")
                          ._M_iterator(__rhs, "rhs"));
    _GLIBCXX_DEBUG_VERIFY(__lhs._M_can_compare(__rhs),
                          _M_message(__msg_order_different)
                          ._M_iterator(__lhs, "lhs")
                          ._M_iterator(__rhs, "rhs"));
    return __lhs.base() > __rhs.base();
  }


  template<class _IteratorL, class _IteratorR, class _Sequence>
  inline bool operator>=(const _Safe_iterator<_IteratorL, _Sequence>& __lhs,
                         const _Safe_iterator<_IteratorR, _Sequence>& __rhs) noexcept {
    _GLIBCXX_DEBUG_VERIFY(!__lhs._M_singular() && !__rhs._M_singular(),
                          _M_message(__msg_iter_order_bad)
                          ._M_iterator(__lhs, "lhs")
                          ._M_iterator(__rhs, "rhs"));
    _GLIBCXX_DEBUG_VERIFY(__lhs._M_can_compare(__rhs),
                          _M_message(__msg_order_different)
                          ._M_iterator(__lhs, "lhs")
                          ._M_iterator(__rhs, "rhs"));
    return __lhs.base() >= __rhs.base();
  }

  template<class _Iterator, class _Sequence>
  inline bool operator>=(const _Safe_iterator<_Iterator, _Sequence>& __lhs,
                         const _Safe_iterator<_Iterator, _Sequence>& __rhs) noexcept {
    _GLIBCXX_DEBUG_VERIFY(!__lhs._M_singular() && !__rhs._M_singular(),
                          _M_message(__msg_iter_order_bad)
                          ._M_iterator(__lhs, "lhs")
                          ._M_iterator(__rhs, "rhs"));
    _GLIBCXX_DEBUG_VERIFY(__lhs._M_can_compare(__rhs),
                          _M_message(__msg_order_different)
                          ._M_iterator(__lhs, "lhs")
                          ._M_iterator(__rhs, "rhs"));
    return __lhs.base() >= __rhs.base();
  }


  template<class _IteratorL, class _IteratorR, class _Sequence>
  inline typename _Safe_iterator<_IteratorL, _Sequence>::difference_type
  operator-(const _Safe_iterator<_IteratorL, _Sequence>& __lhs,
            const _Safe_iterator<_IteratorR, _Sequence>& __rhs) noexcept {
    _GLIBCXX_DEBUG_VERIFY(!__lhs._M_singular() && !__rhs._M_singular(),
                          _M_message(__msg_distance_bad)
                          ._M_iterator(__lhs, "lhs")
                          ._M_iterator(__rhs, "rhs"));
    _GLIBCXX_DEBUG_VERIFY(__lhs._M_can_compare(__rhs),
                          _M_message(__msg_distance_different)
                          ._M_iterator(__lhs, "lhs")
                          ._M_iterator(__rhs, "rhs"));
    return __lhs.base() - __rhs.base();
  }

  template<class _Iterator, class _Sequence>
  inline typename _Safe_iterator<_Iterator, _Sequence>::difference_type
  operator-(const _Safe_iterator<_Iterator, _Sequence>& __lhs,
            const _Safe_iterator<_Iterator, _Sequence>& __rhs) noexcept {
    _GLIBCXX_DEBUG_VERIFY(!__lhs._M_singular() && !__rhs._M_singular(),
                          _M_message(__msg_distance_bad)
                          ._M_iterator(__lhs, "lhs")
                          ._M_iterator(__rhs, "rhs"));
    _GLIBCXX_DEBUG_VERIFY(__lhs._M_can_compare(__rhs),
                          _M_message(__msg_distance_different)
                          ._M_iterator(__lhs, "lhs")
                          ._M_iterator(__rhs, "rhs"));
    return __lhs.base() - __rhs.base();
  }


  template<class _Iterator, class _Sequence>
  inline _Safe_iterator<_Iterator, _Sequence>
  operator+(typename _Safe_iterator<_Iterator, _Sequence>::difference_type __n,
            const _Safe_iterator<_Iterator, _Sequence>& __i) noexcept {
    return __i + __n;
  }

} // End of __gnu_debug:: namespace

// End of libstdc++ debug/safe_iterator header implementation


// Start of libstdc++ debug/safe_iterator.tcc template definition file implementation:

namespace __gnu_debug { // Start of internal debug namespace for safe_iterator out of line definitions

  template<class _Iterator, class _Sequence>
  bool _Safe_iterator<_Iterator, _Sequence>::_M_can_advance(const difference_type& __n) const {
    if(this->_M_singular()) return false;
    if(__n == 0) return true;
    if(__n < 0) {
      std::pair<difference_type, _Distance_precision> __dist
        = __get_distance(_M_get_sequence()->_M_base().begin(), base());
      bool __ok = ((__dist.second == __dp_exact && __dist.first >= -__n)
                || (__dist.second != __dp_exact && __dist.first > 0));
      return __ok;
    } else {
      std::pair<difference_type, _Distance_precision> __dist
        = __get_distance(base(), _M_get_sequence()->_M_base().end());
      bool __ok = ((__dist.second == __dp_exact && __dist.first >= __n)
                || (__dist.second != __dp_exact && __dist.first > 0));
      return __ok;
    }
  }


  template<class _Iterator, class _Sequence>
  bool _Safe_iterator<_Iterator, _Sequence>::_M_valid_range(const _Safe_iterator& __rhs) const {
    if(!_M_can_compare(__rhs)) return false;

    std::pair<difference_type, _Distance_precision> __dist
      = __get_distance(base(), __rhs.base());
    switch(__dist.second) {
      case __dp_equality:
        if(__dist.first == 0) return true;
        break;
      case __dp_sign:
      case __dp_exact:
        return __dist.first >= 0;
    }

    if(_M_is_beginnest() || __rhs._M_is_end()) return true;
    if(_M_is_end() || __rhs._M_is_beginnest()) return false;
    return true;
  }

} // End of __gnu_debug:: namespace

// End of libstdc++ debug/safe_iterator.tcc template definition file implementation


#endif // end of debug mode enabled implementations


#endif // End of Debug.hpp {_DLIB_DEBUGGING_HPP_}
