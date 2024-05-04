// Libstdc++ and GCC GNU Lincese boilerplate:      (this is required since this code is copyrighted under the GCC compiler. I did not design this myself.)
/*
  Partial Modified Implementation Of Meta-Functions from the <Array> Libstdc++ header (gcc-4.9.0)  -*- C++ -*-

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
  Array.hpp  [V1.0.0]  (template header file for array wrapper class)
    By: Andrey Stroitelev

  ~This file is for the std::array container class which is just an array wrapper to make using arrays in certain cases much easier.
  ~This file is pretty much an exact copy of the array.h header file in c++ STL (gcc-4.9.0)
  ~This container class wont actually use the container "concept" unlike the rest of the containers.
    *This file will also be with the "utility" files in the util folder.
    *This class along with std::pair and std::tuple are all container classes that dont use the container "concept". std::array is the only one of these 3 that fully meets the requirements of the container concept
*/


// Start of Array.hpp
#ifndef _DLIB_ARRAY_HPP_
#define _DLIB_ARRAY_HPP_

#ifndef _DLIB_ARRAY_VERSION_
#define _DLIB_ARRAY_VERSION_ 0x010000
#endif


#include <Arduino.h>
#include "Config.h"
#include "Stdexcept.hpp"
#include "AlgoBase.hpp"
#include "RangeAccess.hpp"


_DLIB_HEADER


// this is temporary to test bits of code that I'm not sure will work.
#define ARRAY_TEST_CODE true


// start of std::array class implementation from the array.h header in libstdc++:
namespace std _DLIB_VISIBILITY {

  _BEGIN_NAMESPACE_CONTAINER

  // start of array traits implementation:
  namespace __detail { // namespace for internal implementations

    template<class _Tp, size_t _Nm>
    struct __array_traits {
      typedef _Tp _Type[_Nm];

      static constexpr _Tp& _S_ref(const _Type& __t, size_t __n) noexcept {
        return const_cast<_Tp&>(__t[__n]);
      }
    };

    template<class _Tp>
    struct __array_traits<_Tp, 0> {
      struct _Type { };

      static constexpr _Tp& _S_ref(const _Type&, size_t) noexcept {
        return *static_cast<_Tp*>(nullptr);
      }
    };

  } // end of __detail:: namespace
  // end of array traits


  // Start of std::array class implementation:
  /*
    std::array is a basic container used to store a fixed size sequence of elements.
    This is a very basic container that pretty much just wraps a primitive array to extend functionality.
    
    This container is a contiguous sequence and thus supports random access iterators.
    This container is also reversible since it suports random access iterators.
  */
  template<class _Tp, size_t _Nm>
  struct array {
    // container and iterator typedefs for std::array:
    typedef _Tp value_type;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef value_type* iterator;
    typedef const value_type* const_iterator;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    typedef _GLIBCXX_STD_C::__detail::__array_traits<_Tp, _Nm> _AT_Type; // alias for an array of type _Tp and with a size of _Nm
    typename _AT_Type::_Type _M_elems; // internal array to store elements.


    // start of container algorithm functions:
    void fill(const value_type& __u) {
      std::fill_n(begin(), size(), __u);
    }

    void swap(array& __other) noexcept(noexcept(std::swap(declval<_Tp&>(), declval<_Tp&>()))) {
      std::swap_ranges(begin(), end(), __other.begin());
    }
    // end of container algorithm functions


    // start of iterator related member functions:
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
    // end of iterator related functions


    // start of capacity functions:
    constexpr size_type size() const noexcept { return _Nm; }

    constexpr size_type max_size() const noexcept { return _Nm; }

    [[nodiscard]]
    constexpr bool empty() const noexcept { return size() == 0; }
    // end of capacity functions


    // start of element access functions and operators:
    reference operator[](size_type __n) noexcept {
      return _AT_Type::_S_ref(_M_elems, __n);
    }

    constexpr const_reference operator[](size_type __n) const noexcept {
      return _AT_Type::_S_ref(_M_elems, __n);
    }

    reference at(size_type __n) {
#     if(ARRAY_TEST_CODE)
      if(__n >= _Nm) __throw_out_of_range_fmt(__N("array::at: __n (which is %zu) "
                                                            ">= _Nm (which is %zu)"),
                                                        __n, _Nm);
#     endif
      return _AT_Type::_S_ref(_M_elems, __n);
    }

    constexpr const_reference at(size_type __n) const {
      return __n < _Nm ? _AT_Type::_S_ref(_M_elems, __n)
                       : (__throw_out_of_range_fmt(__N("array::at: __n (which is %zu) "
                                                                 ">= _Nm (which is %zu)"), __n, _Nm),
                                                             _AT_Type::_S_ref(_M_elems, 0));
    }

    reference front() noexcept { return *begin(); }

    constexpr const_reference front() const noexcept {
      return _AT_Type::_S_ref(_M_elems, 0);
    }

    reference back() noexcept {
      return _Nm ? *(end() - 1) : *end();
    }

    constexpr const_reference back() const noexcept {
      return _Nm ? _AT_Type::_S_ref(_M_elems, _Nm - 1)
                 : _AT_Type::_S_ref(_M_elems, 0);
    }

    pointer data() noexcept {
      return std::__detail::__addressof(_AT_Type::_S_ref(_M_elems, 0));
    }

    const_pointer data() const noexcept {
      return std::__detail::__addressof(_AT_Type::_S_ref(_M_elems, 0));
    }
    // end of element access functions and operators
  }; // End of std::array class


  // Start of std::array non-member comparison operators:
  template<class _Tp, size_t _Nm>
  inline bool operator==(const array<_Tp, _Nm>& __lhs, const array<_Tp, _Nm>& __rhs) {
    return std::equal(__lhs.begin(), __lhs.end(), __rhs.begin());
  }

  template<class _Tp, size_t _Nm>
  inline bool operator!=(const array<_Tp, _Nm>& __lhs, const array<_Tp, _Nm>& __rhs) {
    return !(__lhs == __rhs);
  }

  template<class _Tp, size_t _Nm>
  inline bool operator<(const array<_Tp, _Nm>& __lhs, const array<_Tp, _Nm>& __rhs) {
    return lexicographical_compare(__lhs.begin(), __lhs.end(), __rhs.begin(), __rhs.end());
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
  // End of std::array comparison operators


  // Start of algorithm specializations for std::array:
  template<class _Tp, size_t _Nm>
  inline void swap(array<_Tp, _Nm>& __a, array<_Tp, _Nm>* __b)
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
    return std::move(get<_Int>(__arr));
  }

  template<size_t _Int, class _Tp, size_t _Nm>
  constexpr const _Tp& get(const array<_Tp, _Nm>& __arr) noexcept {
    static_assert(_Int < _Nm, "index out of bounds");
    return _GLIBCXX_STD_C::__detail::__array_traits<_Tp, _Nm>::_S_ref(__arr._M_elems, _Int);
  }

  template<size_t _Int, class _Tp, size_t _Nm>
  constexpr const _Tp&& get(const array<_Tp, _Nm>&& __arr) noexcept {
    static_assert(_Int < _Nm, "index out of bounds");
    return std::move(_GLIBCXX_STD_C::get<_Int>(__arr));
  }
  // End of std::array algorithm specializations

  _END_NAMESPACE_CONTAINER

} // end of std:: namespace


namespace std _DLIB_VISIBILITY {

  // Start of tuple interface for std::array:
  template<class _Tp>
  class tuple_size;

  template<class _Tp, size_t _Nm>
  struct tuple_size<_GLIBCXX_STD_C::array<_Tp, _Nm>>
    : public integral_constant<size_t, _Nm> { };

  template<size_t _Int, class _Tp>
  class tuple_element;

  template<size_t _Int, class _Tp, size_t _Nm>
  struct tuple_element<_Int, _GLIBCXX_STD_C::array<_Tp, _Nm>> {
    static_assert(_Int < _Nm, "index is out of bounds");
    typedef _Tp type;
  };

  namespace __detail { // namespace for internal implementations

    // I'm not sure if I'll keep or use this meta-function as it's from 2019 instead of 2014
    template<class _Tp, size_t _Nm>
    struct __is_tuple_like_impl<_GLIBCXX_STD_C::array<_Tp, _Nm>> : true_type { };

  } // end of __detail:: namespace
  // End of tuple interface for std::array

} // end of std:: namespace


// I do not have the debug array file implemented yet, so enabling debug mode will give an error at the moment.
#if (_DEBUG_)
#include "Debug_Array.hpp"
#endif


// I see no need for adding the profile mode to this library since it's not really useful for arduino.
//#if (_PROFILE_)
//# include "profile/Array.hpp"
//#endif


#endif // end of Array.hpp 
