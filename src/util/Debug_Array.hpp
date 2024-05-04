// Libstdc++ and GCC GNU Lincese boilerplate:      (this is required since this code is copyrighted under the GCC compiler. I did not design this myself.)
/*
  Partial Modified Implementation Of Meta-Functions from the <debug/array> Libstdc++ header (gcc-4.9.0)  -*- C++ -*-

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
  debug/Array.hpp  [v1.0.0]  (template class debug header file)
    By: Andrey Stroitelev

  ~This is a second implementation of std::array in the __debug:: namespace to help with debugging.
  ~This is almost an exact copy of the debug/array header in libstdc++.
  ~This file is only used when debugging is enabled
  ~The purpose of this file is to do extra checks to detect any possible runtime errors before they would normally occur.
  *NOTE: currently you would need to initialize the class being debugged with the std::__debug:: namespace as I don't have macros set up yet to disable the std version when debug mode is enabled.
*/


// Start of debug/Array.hpp
#ifndef _DEBUG_ARRAY_HPP_
#define _DEBUG_ARRAY_HPP_

#ifndef _DEBUG_ARRAY_VERSION_
#define _DEBUG_ARRAY_VERSION_ 0x010000
#endif


#include <Arduino.h>
#include "Array.hpp"
#include "Debug.hpp"


_DLIB_HEADER


// start of debug mode std::array implementation:
namespace std _DLIB_VISIBILITY {

  namespace __debug { // namespace for debug implementations

    template<class _Tp, size_t _Nm>
    struct array {
      // standard typedefs for array
      typedef _Tp value_type;
      typedef value_type* pointer;
      typedef const value_type* const_pointer;
      typedef value_type& reference;
      typedef const value_type& const_reference;
      typedef value_type* iterator;
      typedef const value_type& const_iterator;
      typedef size_t size_type;
      typedef ptrdiff_t difference_type;
      typedef std::reverse_iterator<iterator> reverse_iterator;
      typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

      // no change to array type and array type variable
      typedef _GLIBCXX_STD_C::__detail::__array_traits<_Tp, _Nm> _AT_Type;
      typename _AT_Type::_Type _M_elems;

      // Start of debug meta-functions:
      template<size_t _Size>
      struct _Array_check_subscript {
        size_t size() { return _Size; }

        _Array_check_subscript(size_t __index) {
          __glibcxx_check_subscript(__index);
        }
      };

      template<size_t _Size>
      struct _Array_check_nonempty {
        [[nodiscard]] bool empty() { return _Size == 0; }

        _Array_check_nonempty() { __glibcxx_check_nonempty(); }
      };
      // End of debug meta-functions


      // Start of algorithm functions:
      void fill(const value_type& __u) {
        fill_n(begin(), size(), __u);
      }

      void swap(array& __other) noexcept(_AT_Type::_Is_nothrow_swappable::value) {
        swap_ranges(begin(), end(), __other.begin());
      }
      // End of algorithm functions


      // Start of iterator functions:
      iterator begin() noexcept { 
        return iterator(data()); 
      }

      const_iterator begin() const noexcept { 
        return const_iterator(data()); 
      }

      iterator end() noexcept {
        return iterator(data() + _Nm);
      }

      const_iterator end() const noexcept {
        return const_iterator(data() + _Nm);
      }

      reverse_iterator rbegin() noexcept {
        return reverse_iterator(end());
      }

      const_reverse_iterator rbegin() const noexcept {
        return const_reverse_iterator(end());
      }

      reverse_iterator rend() noexcept {
        return reverse_iterator(begin());
      }

      const_reverse_iterator rend() const noexcept {
        return const_reverse_iterator(begin());
      }

      const_iterator cbegin() const noexcept {
        return const_iterator(data());
      }

      const_iterator cend() const noexcept {
        return const_iterator(data() + _Nm);
      }

      const_reverse_iterator crbegin() const noexcept {
        return const_reverse_iterator(end());
      }

      const_reverse_iterator crend() const noexcept {
        return const_reverse_iterator(begin());
      }
      // End of iterator functions


      // Start of capacity functions:
      constexpr size_type size() const noexcept { return _Nm; }

      constexpr size_type max_size() const noexcept { return _Nm; }

      constexpr bool empty() const noexcept { return size() == 0; }
      // End of capacity functions


      // Start of element access functions:
      reference operator[](size_type __n) noexcept {
        __glibcxx_check_subscript(__n);
        return _AT_Type::_S_ref(_M_elems, __n);
      }

      constexpr const_reference operator[](size_type __n) const noexcept {
        return __n < _Nm ? _AT_Type::_S_ref(_M_elems, __n)
                         : (_GLIBCXX_THROW_OR_ABORT(_Array_check_subscript<_Nm>(__n)), 
                            _AT_Type::_S_ref(_M_elems, 0));
      }

      reference at(size_type __n) {
        if(__n >= _Nm) std::__throw_out_of_range_fmt(__N("array::at: __n (which is %zu) "
                                                         ">= _Nm (which is %zu)"),
                                                    __n, _Nm);
        return _AT_Type::_S_ref(_M_elems, __n);
      }

      constexpr const_reference at(size_type __n) const {
        return __n < _Nm ? _AT_Type::_S_ref(_M_elems, __n)
                         : (std::__throw_out_of_range_fmt(__N("array::at: __n (which is %zu) "
                                                              ">= _Nm (which is %zu)"),
                                                          __n, _Nm),
                            _AT_Type::_S_ref(_M_elems, 0));
      }

      reference front() noexcept {
        __glibcxx_check_nonempty();
        return *begin();
      }

      constexpr const_reference front() const noexcept {
        return _Nm ? _AT_Type::_S_ref(_M_elems, 0)
                   : (_GLIBCXX_THROW_OR_ABORT(_Array_check_nonempty<_Nm>()),
                      _AT_Type::_S_ref(_M_elems, 0));
      }

      reference back() noexcept {
        __glibcxx_check_nonempty();
        return _Nm ? *(end() - 1) : *end();
      }

      constexpr const_reference back() const noexcept {
        return _Nm ? _AT_Type::_S_ref(_M_elems, _Nm - 1)
                   : (_GLIBCXX_THROW_OR_ABORT(_Array_check_nonempty<_Nm>()),
                      _AT_Type::_S_ref(_M_elems, 0));
      }

      pointer data() noexcept {
        return _AT_Type::_S_ptr(_M_elems);
      }

      const_pointer data() const noexcept {
        return _AT_Type::_S_ptr(_M_elems);
      }
      // End of element access functions

    }; // end of __debug::array class


    // Start of array comparison operators:
    template<class _Tp, size_t _Nm>
    inline bool operator==(const array<_Tp, _Nm>& __lhs, const array<_Tp, _Nm>& __rhs) {
      return equal(__lhs.begin(), __lhs.end(), __rhs.begin());
    }

    template<class _Tp, size_t _Nm>
    inline bool operator!=(const array<_Tp, _Nm>& __lhs, const array<_Tp, _Nm>& __rhs) {
      return !(__lhs == __rhs);
    }

    template<class _Tp, size_t _Nm>
    inline bool operator<(const array<_Tp, _Nm>& __lhs, const array<_Tp, _Nm>& __rhs) {
      return lexicographical_compar(__lhs.begin(), __lhs.end(), __rhs.begin(), __rhs.end());
    }

    template<class _Tp, size_t _Nm>
    inline bool operator>(const array<_Tp, _Nm>& __lhs, const array<_Tp, _Nm>& __rhs) {
      return __rhs < __lhs;
    }

    template<class _Tp, size_t _Nm>
    inline bool operator<=(const array<_Tp, _Nm>& __lhs, const array<_Tp, _Nm>& __rhs) {
      return !(__lhs > __rhs);
    }

    template<class _Tp, size_t _Nm>
    inline bool operator>=(const array<_Tp, _Nm>& __lhs, const array<_Tp, _Nm>& __rhs) {
      return !(__lhs < __rhs);
    }
    // End of array comparison operators


    // Start of specialized algorithms for std::array
#   if(ARRAY_TEST_CODE)
    template<class _Tp, size_t _Nm>
    typename enable_if<!_GLIBCXX_STD_C::__detail::__array_traits<_Tp, _Nm>::_Is_swappable::value>::type
    swap(array<_Tp, _Nm>&, array<_Tp, _Nm>&) = delete;
#   endif

    template<class _Tp, size_t _Nm>
    inline void swap(array<_Tp, _Nm>& __a, array<_Tp, _Nm>& __b)
    noexcept(noexcept(__a.swap(__b))) {
      __a.swap(__b);
    }

    template<size_t _Int, class _Tp, size_t _Nm>
    constexpr _Tp& get(array<_Tp, _Nm>& __arr) noexcept {
      static_assert(_Int < _Nm, "index is out of bounds");
      return _GLIBCXX_STD_C::__detail::__array_traits<_Tp, _Nm>::_S_ref(__arr._M_elems, _Int);
    }

    template<size_t _Int, class _Tp, size_t _Nm>
    constexpr _Tp&& get(array<_Tp, _Nm>&& __arr) noexcept {
      static_assert(_Int < _Nm, "index is out of bounds");
      return std::move(__debug::get<_Int>(__arr));
    }

    template<size_t _Int, class _Tp, size_t _Nm>
    constexpr const _Tp& get(const array<_Tp, _Nm>& __arr) noexcept {
      static_assert(_Int < _Nm, "index is out of bounds");
      return _GLIBCXX_STD_C::__detail::__array_traits<_Tp, _Nm>::_S_ref(__arr._M_elems, _Int);
    }

    template<size_t _Int, class _Tp, size_t _Nm>
    constexpr const _Tp&& get(const array<_Tp, _Nm>&& __arr) noexcept {
      static_assert(_Int < _Nm, "index is out of bounds");
      return std::move(__debug::get<_Int>(__arr));
    }
    // End of specialized algorithms

  } // end of debug namespace


  // Start of debug specializations of tuple interface for __debug::arry:
  template<class _Tp, size_t _Nm>
  struct tuple_size<std::__debug::array<_Tp, _Nm>>
    : public integral_constant<size_t, _Nm> { };

  template<size_t _Int, class _Tp, size_t _Nm>
  struct tuple_element<_Int, std::__debug::array<_Tp, _Nm>> {
    static_assert(_Int < _Nm, "index is out of bounds");
    typedef _Tp type;
  };

  namespace __detail { // namespace for internal implementations

    template<class _Tp, size_t _Nm>
    struct __is_tuple_like_impl<std::__debug::array<_Tp, _Nm>> : true_type { };

  } // end of __detail:: namespace
  // End of specialized tuple interface for __debug::array

} // end of std:: namespace


#endif // end of debug/Array.hpp
