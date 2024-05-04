// Libstdc++ and GCC GNU Lincese boilerplate:      (this is required since this code is copyrighted under the GCC compiler. I did not design this myself.)
/*
  Partial Modified Implementation Of Meta-Functions from the <bits/stl_vector | bits/stl_bvector> Libstdc++ header  -*- C++ -*-

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
  Vector.hpp  [V1.0.0]  (template class definition header file)
    By: Andrey Stroitelev

  ~This file includes the libstdc++ implementation of bits/stl_vector and bits/stl_bvector.
  ~This consolidates the vector implementations to keep the library a bit more compact.
    *The out of line definition file(vector.tcc) and the debug vector header will have their own files.
  ~This header file is dedicated to implementing the std::vector class.
  ~The std::vector class is essentially an array without a fixed size.
  ~Since the vector has a dynamic size it dynamically allocates memory.
    *I will use the standard implementation for allocating memory, but I may add macro toggled code that will change how much it will allocate when full and expanding.
    *I may also make a version of this library that will reimplement dynamic data structures such as this to make them static with a fixed size for arduino boards with really low memory.
  ~By default a vector starts with a size of 2 and it doubles in size every time it's full and needs to add another value.
    *When full and pushing another value it will allocate a new block of memory 2x the size of the old one, then it will move all of the values from the old block in RAM including the new value, and lastly it will deallocate the old block of memory.
    *If I add a macro to configure how much a vector increases in capacity, then lowering the rate of increase will reduce memory usage(better RAM preformance), but it will decrease overall preformance since it will need to allocate, move, and deallocate much more often which takes up alot more CPU clock cycles.
    *Lowering the rate of increase will be pretty much necessary for boards with less than 4-8kb of dynamic memory.
    *If you plan on adding a large enough amount of elements to get near max memory usage then using the static version or lowering the rate of capacity increase will be necessary.
    *Another way to conserve memory is the shrink_to_fit() member function which decreases the capacity to match the amount of elements stored.
    **There is also the reserve() function which will set a max size so that the vector never does a reallocation. This will improve preformance alot, but at that point it might be better to just use std::array.
  ~std::vector is contiguous, so it uses a random access iterator allowing it to use all iterator algorithms.
  ~This file also implements bvector(bitset vector). This is a specialization for space-efficient dynamic bitsets.
  ~std::vector is part of the sequence containers in libstdc++ along with std::array, std::deque, std::forward_list, and std::list.

  ~IMPORTANT NOTE: std::vector typically does memory deallocation for you, but if the elements stored are pointers themselves then it will only deallocate the pointer without touching what it points to.
*/


// Start of Vector.hpp
#ifndef _DLIB_VECTOR_HPP_
#define _DLIB_VECTOR_HPP_

#ifndef _DLIB_VECTOR_VERSION_
#define _DLIB_VECTOR_VERSION_ 0x010000
#endif


#include <Arduino.h>
#include "util/Config.h"
#include "util/AlgoBase.hpp"
#include "util/Allocator.hpp"
#include "util/Alloc_Traits.hpp"
#include "util/Utility.hpp"
#include "util/RangeAccess.hpp"
#include "util/Iterator_Funcs.hpp"
#include "util/Stdexcept.hpp"
#include "util/Concept_Check.hpp"
#include "util/Initializer_List.hpp"


_DLIB_HEADER


// Start of bits/stl_vector.h header implementation from libstdc++:

namespace std _DLIB_VISIBILITY {

  _BEGIN_NAMESPACE_CONTAINER // for debugging


  // Start of _Vector_base implementation:
  namespace __detail { // namespace for internal implementations

    template<class _Tp, class _Alloc>
    struct _Vector_base {
      typedef typename __gnu_cxx::__alloc_traits<_Alloc>::template rebind<_Tp>::other _Tp_alloc_type;
      typedef typename __gnu_cxx::__alloc_traits<_Tp_alloc_type>::pointer pointer;

      struct _Vector_impl : public _Tp_alloc_type {
        pointer _M_start;
        pointer _M_finish;
        pointer _M_end_of_storage;

        _Vector_impl() : _Tp_alloc_type(), _M_start(0), _M_finish(0), _M_end_of_storage(0) { }

        _Vector_impl(_Tp_alloc_type const& __a) noexcept
          : _Tp_alloc_type(__a), _M_start(0), _M_finish(0), _M_end_of_storage(0) { }

        _Vector_impl(_Tp_alloc_type&& __a) noexcept
          : _Tp_alloc_type(std::move(__a)), _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
        
        void _M_swap_data(_Vector_impl& __x) noexcept {
          std::swap(_M_start, __x._M_start);
          std::swap(_M_finish, __x._M_finish);
          std::swap(_M_end_of_storage, __x._M_end_of_storage);
        }
      }; // end of _Vector_base::_Vector_impl member struct

      public:
        typedef _Alloc allocator_type;

        _Tp_alloc_type& _M_get_Tp_allocator() noexcept {
          return *static_cast<_Tp_alloc_type*>(&this->_M_impl);
        }

        const _Tp_alloc_type& _M_get_Tp_allocator() const noexcept {
          return *static_cast<const _Tp_alloc_type*>(&this->_M_impl);
        }

        allocator_type get_allocator() const noexcept {
          return allocator_type(_M_get_Tp_allocator());
        }

        _Vector_base() : _M_impl() { }

        _Vector_base(const allocator_type& __a) noexcept : _M_impl(__a) { }

        _Vector_base(size_t __n) : _M_impl() { _M_create_storage(__n); }

        _Vector_base(size_t __n, const allocator_type& __a)
          : _M_impl(__a) { _M_create_storage(__n); }

        _Vector_base(_Tp_alloc_type&& __a) noexcept
          : _M_impl(std::move(__a)) { }

        _Vector_base(_Vector_base&& __x) noexcept
          : _M_impl(std::move(__x._M_get_Tp_allocator())) {
          this->_M_impl._M_swap_data(__x._M_impl);
        }

        _Vector_base(_Vector_base&& __x, const allocator_type& __a) : _M_impl(__a) {
          if(__x.get_allocator() == __a) this->_M_impl._M_swap_data(__x._M_impl);
          else {
            size_t __n = __x._M_impl._M_finish - __x._M_impl._M_start;
            _M_create_storage(__n);
          }
        }

        ~_Vector_base() noexcept {
          _M_deallocate(this->_M_impl._M_start,
                        this->_M_impl._M_end_of_storage - this->_M_impl._M_start);
        }

      public: // not sure why this is repeated in the libstdc++ implementation
        _Vector_impl _M_impl;

        pointer _M_allocate(size_t __n) {
          typedef __gnu_cxx::__alloc_traits<_Tp_alloc_type> _Tr;
          return __n != 0 ? _Tr::allocate(_M_impl, __n) : 0;
        }

        void _M_deallocate(pointer __p, size_t __n) {
          typedef __gnu_cxx::__alloc_traits<_Tp_alloc_type> _Tr;
          if(__p) _Tr::deallocate(_M_impl, __p, __n);
        }

      private:
        void _M_create_storage(size_t __n) {
          this->_M_impl._M_start = this->_M_allocate(__n);
          this->_M_impl._M_finish = this->_M_impl._M_start;
          this->_M_impl._M_end_of_storage = this->_M_impl._M_start + __n;
        }

    }; // end of _Vector_base struct

  } // end of _GLIBCXX_STD_C::__detail:: namespace
  // End of _Vector_base implementation


  // Start of std::vector main implementation:

  /*
    std::vector encapsulates/wraps the internal implementation above to create an abstracted interface for the user.
    std::vector meets the requirements of the following:
      *container
      *reversible_container
      *sequence & optional sequence requirements(with the exception of push_front and pop_front)
    This class acts like a dynamic contiguous c-style array with fixed time random access to individual elements.
    std::vector automatically does allocations, reallocations, and deallocations so that the user doesn't have to do it manually.
  */
  template<class _Tp, class _Alloc = std::allocator<_Tp>>
  class vector : protected _GLIBCXX_STD_C::__detail::_Vector_base<_Tp, _Alloc> {
    // concept requirement typedefs:
    typedef typename _Alloc::value_type _Alloc_value_type;
    __glibcxx_class_requires(_Tp, _SGIAssignableConcept)
    __glibcxx_class_requires2(_Tp, _Alloc_value_type, _SameTypeConcept)

    // private typedefs:
    typedef _GLIBCXX_STD_C::__detail::_Vector_base<_Tp, _Alloc> _Base;
    typedef typename _Base::_Tp_alloc_type _Tp_alloc_type;
    typedef __gnu_cxx::__alloc_traits<_Tp_alloc_type> _Alloc_traits;

    public:
      // required public typedefs:
      typedef _Tp value_type;
      typedef typename _Base::pointer pointer;
      typedef typename _Alloc_traits::const_pointer const_pointer;
      typedef typename _Alloc_traits::reference reference;
      typedef typename _Alloc_traits::const_reference const_reference;
      typedef __gnu_cxx::__normal_iterator<pointer, vector> iterator;
      typedef __gnu_cxx::__normal_iterator<const_pointer, vector> const_iterator;
      typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
      typedef std::reverse_iterator<iterator> reverse_iterator;
      typedef size_t size_type;
      typedef ptrdiff_t difference_type;
      typedef _Alloc allocator_type;

    protected:
      // used members from base class:
      using _Base::_M_allocate;
      using _Base::_M_deallocate;
      using _Base::_M_impl;
      using _Base::_M_get_Tp_allocator;

    public:
      // Start of std::vector constructors
      vector() noexcept(is_nothrow_default_constructible<_Alloc>::value)
        : _Base() { }

      explicit vector(const allocator_type& __a) noexcept
        : _Base(__a) { }

#     if !(_CXX_OLD_CODE)
      explicit vector(size_type __n, const allocator_type& __a = allocator_type())
        : _Base(__n, __a) { _M_default_initialize(__n); }
      
      vector(size_type __n, const value_type& __value, const allocator_type& __a = allocator_type())
        : _Base(__n, __a) { _M_fill_initialize(__n, __value); }
#     else // pre c++11 constructor(more compact but uses more memory)
      explicit vector(size_type __n, const value_type& __value = value_type(),
                      const allocator_type& __a = allocator_type())
        : _Base(__n, __a) { _M_fill_initialize(__n, __value); }
#     endif

      vector(const vector& __x)
        : _Base(__x.size(), _Alloc_traits::_S_select_on_copy(__x._M_get_Tp_allocator())) {
        this->_M_impl._M_finish = std::__detail::__uninitialized_copy_a(__x.begin(), __x.end(),
                                                                   this->_M_impl._M_start,
                                                                   _M_get_Tp_allocator());
      }

      vector(vector&& __x) noexcept
        : _Base(std::move(__x)) { }

      vector(const vector& __x, const allocator_type& __a)
        : _Base(__x.size(), __a) {
        this->_M_impl._M_finish = std::__detail::__uninitialized_copy_a(__x.begin(), __x.end(),
                                                                   this->_M_impl._M_start,
                                                                   _M_get_Tp_allocator());
      }

      vector(vector&& __rv, const allocator_type& __m)
      noexcept(_Alloc_traits::_S_always_equal()) : _Base(std::move(__rv), __m) {
        if(__rv.get_allocator() != __m) {
          this->_M_impl._M_finish = std::__detail::__uninitialized_move_a(__rv.begin(), __rv.end(),
                                                                     this->_M_impl._M_start,
                                                                     _M_get_Tp_allocator());
          __rv.clear();
        }
      }

      vector(initializer_list<value_type> __l, 
             const allocator_type& __a = allocator_type()) : _Base(__a) {
        _M_range_initialize(__l.begin(), __l.end(), random_access_iterator_tag());
      }

#     if !(_CXX_OLD_CODE)
      template<class _InputIterator, class = std::__detail::_RequireInputIter<_InputIterator>>
      vector(_InputIterator __first, _InputIterator __last,
             const allocator_type& __a = allocator_type()) : _Base(__a) {
        _M_initialize_dispatch(__first, __last, std::__detail::__false_type());
      }
#     else
      template<class _InputIterator>
      vector(_InputIterator __first, _InputIterator __last,
             const allocator_type& __a = allocator_type()) : _Base(__a) {
        typedef typename std::__detail::__is_integer<_InputIterator>::__type _Integral;
        _M_initialize_dispatch(__first, __last, _Integral());
      }
#     endif
      // End of std::vector constructors


      // std::vector destructor definition:
      ~vector() noexcept {
        std::__detail::_Destroy(this->_M_impl._M_start, this->_M_impl._M_finish,
                           _M_get_Tp_allocator());
      }


      // Start of std::vector assignment operators:
      vector& operator=(const vector& __x);

      vector& operator=(vector&& __x) noexcept(_Alloc_traits::_S_nothrow_move()) {
        constexpr bool __move_storage = _Alloc_traits::_S_propagate_on_move_assign() ||
                                        _Alloc_traits::_S_always_equal();
        _M_move_assign(std::move(__x), bool_constant<__move_storage>());
        return *this;                           
      }

      vector& operator=(initializer_list<value_type> __l) {
        this->assign(__l.begin(), __l.end());
        return *this;
      }
      // End of std::vector assignment operators


      // Start of std::vector assign() implementation:
      void assign(size_type __n, const value_type& __val) {
        _M_fill_assign(__n, __val);
      }

#     if !(_CXX_OLD_CODE)
      template<class _InputIterator, class = std::__detail::_RequireInputIter<_InputIterator>>
      void assign(_InputIterator __first, _InputIterator __last) {
        _M_assign_dispatch(__first, __last, std::__detail::__false_type());
      }
#     else
      template<class _InputIterator>
      void assign(_InputIterator __first, _InputIterator __last) {
        typedef typename std::__detail::__is_integer<_InputIterator>::__type _Integral;
        _M_assign_dispatch(__first, __last, _Integral());
      }
#     endif

      void assign(initializer_list<value_type> __l) {
        this->assign(__l.begin(), __l.end());
      }
      // End of std::vector assign() implementation


      using _Base::get_allocator; // for getting a copy of the allocator


      // Start of std::vector iterator functions:
      inline iterator begin() noexcept {
        return iterator(this->_M_impl._M_start);
      }

      inline const_iterator begin() const noexcept {
        return const_iterator(this->_M_impl._M_start);
      }

      inline iterator end() noexcept {
        return iterator(this->_M_impl._M_finish);
      }

      inline const_iterator end() const noexcept {
        return const_iterator(this->_M_impl._M_finish);
      }

      inline reverse_iterator rbegin() noexcept {
        return reverse_iterator(end());
      }

      inline const_reverse_iterator rbegin() const noexcept {
        return const_reverse_iterator(end());
      }

      inline reverse_iterator rend() noexcept {
        return reverse_iterator(begin());
      }

      inline const_reverse_iterator rend() const noexcept {
        return const_reverse_iterator(begin());
      }

      inline const_iterator cbegin() const noexcept {
        return const_iterator(this->_M_impl._M_start);
      }

      inline const_iterator cend() const noexcept {
        return const_iterator(this->_M_impl._M_finish);
      }

      inline const_reverse_iterator crbegin() const noexcept {
        return const_reverse_iterator(end());
      }

      inline const_reverse_iterator crend() const noexcept {
        return const_reverse_iterator(begin());
      }
      // End of std::vector iterator functions


      // Start of std::vector capacity functions:
      inline size_type size() const noexcept {
        return size_type(this->_M_impl._M_finish - this->_M_impl._M_start);
      }

      inline size_type max_size() const noexcept {
        return _Alloc_traits::max_size(_M_get_Tp_allocator());
      }

#     if !(_CXX_OLD_CODE)
      void resize(size_type __new_size) {
        if(__new_size > size()) _M_default_append(__new_size - size());
        else if(__new_size < size()) _M_erase_at_end(this->_M_impl._M_start + __new_size);
      }

      void resize(size_type __new_size, const value_type& __x) {
        if(__new_size > size()) insert(end(), __new_size - size(), __x);
        else if(__new_size < size()) _M_erase_at_end(this->_M_impl._M_start + __new_size);
      }
#     else

      void resize(size_type __new_size, value_type __x = value_type()) {
        if(__new_size > size()) insert(end(), __new_size - size(), __x);
        else if(__new_size < size()) _M_erase_at_end(this->_M_impl._M_start + __new_size);
      }
#     endif

      inline void shrink_to_fit() {
        _M_shrink_to_fit();
      }

      inline size_type capacity() const noexcept {
        return size_type(this->_M_impl._M_end_of_storage - this->_M_impl._M_start);
      }

      inline bool empty() const noexcept {
        return begin() == end();
      }

      void reserve(size_type __n);
      // End of std::vector capacity functions


      // Start of std::vector element access:
      inline reference operator[](size_type __n) noexcept {
        return *(this->_M_impl._M_start + __n);
      }

      inline const_reference operator[](size_type __n) const noexcept {
        return *(this->_M_impl._M_start + __n);
      }

    protected:
      void _M_range_check(size_type __n) const {
        if(__n >= this->size())
          __throw_out_of_range_fmt(__N("vector::_M_range_check: __n "
                                       "(which is %zu) >= this->size() "
                                       "(which is %zu)"),
                                       __n, this->size());
      }

    public:
      reference at(size_type __n) {
        _M_range_check(__n);
        return (*this)[__n];
      }

      const_reference at(size_type __n) const {
        _M_range_check(__n);
        return (*this)[__n];
      }

      inline reference front() noexcept {
        return *begin();
      }

      inline const_reference front() const noexcept {
        return *begin();
      }

      inline reference back() noexcept {
        return *(end() - 1);
      }

      inline const_reference back() const noexcept {
        return *(end() - 1);
      }

      inline pointer data() noexcept {
        return _M_data_ptr(this->_M_impl._M_start);
      }

      inline const_pointer data() const noexcept {
        return _M_data_ptr(this->_M_impl._M_start);
      }
      // End of std::vector element access


      // Start of std::vector element modifier functions:
      void push_back(const value_type& __x) {
        if(this->_M_impl._M_finish != this->_M_impl._M_end_of_storage) {
          _Alloc_traits::construct(this->_M_impl, this->_M_impl._M_finish, __x);
          ++this->_M_impl._M_finish;
        } else _M_emplace_back_aux(__x);
      }

      void push_back(value_type&& __x) {
        emplace_back(std::move(__x));
      }

      template<class... _Args>
      void emplace_back(_Args&&... __args);

      void pop_back() noexcept {
        --this->_M_impl._M_finish;
        _Alloc_traits::destroy(this->_M_impl, this->_M_impl._M_finish);
      }

      template<class... _Args>
      iterator emplace(const_iterator __position, _Args&&... __args);

      iterator insert(const_iterator __position, const value_type& __x);

      iterator insert(const_iterator __position, value_type&& __x) {
        return emplace(__position, std::move(__x));
      }

      iterator insert(const_iterator __position, initializer_list<value_type> __l) {
        return this->insert(__position, __l.begin(), __l.end());
      }

#     if !(_CXX_OLD_CODE)
      iterator insert(const_iterator __position, size_type __n, const value_type& __x) {
        difference_type __offset = __position - cbegin();
        _M_fill_insert(__position._M_const_cast(), __n, __x);
        return begin() + __offset;
      }
#     else
      void insert(iterator __position, size_type __n, const value_type& __x) {
        _M_fill_insert(__position, __n, __x);
      }
#     endif

#     if !(_CXX_OLD_CODE)
      template<class _InputIterator, class = std::__detail::_RequireInputIter<_InputIterator>>
      iterator insert(const_iterator __position, _InputIterator __first, _InputIterator __last) {
        difference_type __offset = __position - cbegin();
        _M_insert_dispatch(__position._M_const_cast(), __first,
                           __last, std::__detail::__false_type());
        return begin() + __offset;
      }
#     else
      template<class _InputIterator>
      void insert(iterator __position, _InputIterator __first, _InputIterator __last) {
        typedef typename std::__detail::__is_integer<_InputIterator>::__type _Integral;
        _M_insert_dispatch(__position, __first, __last, _Integral());
      }
#     endif

      iterator erase(const_iterator __position) {
        return _M_erase(__position._M_const_cast());
      }

      iterator erase(const_iterator __first, const_iterator __last) {
        return _M_erase(__first._M_const_cast(), __last._M_const_cast());
      }

      void swap(vector& __x) noexcept(_Alloc_traits::_S_nothrow_swap()) {
        this->_M_impl._M_swap_data(__x._M_impl);
        _Alloc_traits::_S_on_swap(_M_get_Tp_allocator(), __x._M_get_Tp_allocator());
      }

      void clear() noexcept {
        _M_erase_at_end(this->_M_impl._M_start);
      }
      // End of std::vector element modifier functions

    
    protected:
      // Start of std::vector internal implementations:
      template<class _ForwardIterator>
      pointer _M_allocate_and_copy(size_type __n, _ForwardIterator __first, _ForwardIterator __last) {
        pointer __result = this->_M_allocate(__n);
        __try {
          std::__detail::__uninitialized_copy_a(__first, __last, __result, _M_get_Tp_allocator());
          return __result;
        } __catch(...) {
          _M_deallocate(__result, __n);
          __throw_exception_again;
        }
      }

      template<class _Integer>
      void _M_initialize_dispatch(_Integer __n, _Integer __value, std::__detail::__true_type) {
        this->_M_impl._M_start = _M_allocate(static_cast<size_type>(__n));
        this->_M_impl._M_end_of_storage = this->_M_impl._M_start + static_cast<size_type>(__n);
        _M_fill_initialize(static_cast<size_type>(__n), __value);
      }

      template<class _InputIterator>
      void _M_initialize_dispatch(_InputIterator __first, _InputIterator __last, std::__detail::__false_type) {
        typedef typename iterator_traits<_InputIterator>::iterator_category _IterCategory;
        _M_range_initialize(__first, __last, _IterCategory());
      }

      template<class _InputIterator>
      void _M_range_initialize(_InputIterator __first, _InputIterator __last, input_iterator_tag) {
        for(; __first != __last; ++__first) emplace_back(*__first);
      }

      template<class _ForwardIterator>
      void _M_range_initialize(_ForwardIterator __first, _ForwardIterator __last, forward_iterator_tag) {
        const size_type __n = std::distance(__first, __last);
        this->_M_impl._M_start = this->_M_allocate(__n);
        this->_M_impl._M_end_of_storage = this->_M_impl._M_start + __n;
        this->_M_impl._M_finish = std::__detail::__uninitialized_copy_a(__first, __last,
                                                                   this->_M_impl._M_start,
                                                                   _M_get_Tp_allocator());
      }

      void _M_fill_initialize(size_type __n, const value_type& __value) {
        std::__detail::__uninitialized_fill_n_a(this->_M_impl._M_start, __n, __value, _M_get_Tp_allocator());
        this->_M_impl._M_finish = this->_M_impl._M_end_of_storage;
      }

      void _M_default_initialize(size_type __n) {
        std::__detail::__uninitialized_default_n_a(this->_M_impl._M_start, __n, _M_get_Tp_allocator());
        this->_M_impl._M_finish = this->_M_impl._M_end_of_storage;
      }

      template<class _Integer>
      void _M_assign_dispatch(_Integer __n, _Integer __val, std::__detail::__true_type) {
        _M_fill_assign(__n, __val);
      }

      template<class _InputIterator>
      void _M_assign_dispatch(_InputIterator __first, _InputIterator __last, std::__detail::__false_type) {
        typedef typename iterator_traits<_InputIterator>::iterator_category _IterCategory;
        _M_assign_aux(__first, __last, _IterCategory());
      }

      template<class _InputIterator>
      void _M_assign_aux(_InputIterator __first, _InputIterator __last, input_iterator_tag);

      template<class _ForwardIterator>
      void _M_assign_aux(_ForwardIterator __first, _ForwardIterator __last, forward_iterator_tag);

      void _M_fill_assign(size_type __n, const value_type& __val);

      template<class _Integer>
      void _M_insert_dispatch(iterator __pos, _Integer __n, _Integer __val, std::__detail::__true_type) {
        _M_fill_insert(__pos, __n, __val);
      }

      template<class _InputIterator>
      void _M_insert_dispatch(iterator __pos, _InputIterator __first, _InputIterator __last, std::__detail::__false_type) {
        typedef typename iterator_traits<_InputIterator>::iterator_category _IterCategory;
        _M_range_insert(__pos, __first, __last, _IterCategory());
      }

      template<class _InputIterator>
      void _M_range_insert(iterator __pos, _InputIterator __first, _InputIterator __last, input_iterator_tag);

      template<class _ForwardIterator>
      void _M_range_insert(iterator __pos, _ForwardIterator __first, _ForwardIterator __last, forward_iterator_tag);

      void _M_fill_insert(iterator __pos, size_type __n, const value_type& __x);

      void _M_default_append(size_type __n);

      constexpr bool _M_shrink_to_fit();

      template<class... _Args>
      void _M_insert_aux(iterator __position, _Args&&... __args);

      template<class... _Args>
      void _M_emplace_back_aux(_Args&&... __args);

      size_type _M_check_len(size_type __n, const char* __s) const {
        if(max_size() - size() < __n) __throw_length_error(__N(__s));
        const size_type __len = size() + max(size(), __n);
        return (__len < size() || __len > max_size()) ? max_size() : __len;
      }

      void _M_erase_at_end(pointer __pos) noexcept {
        std::__detail::_Destroy(__pos, this->_M_impl._M_finish, _M_get_Tp_allocator());
        this->_M_impl._M_finish = __pos;
      }

      iterator _M_erase(iterator __position);

      iterator _M_erase(iterator __first, iterator __last);

    private:

      void _M_move_assign(vector&& __x, std::true_type) noexcept {
        vector __tmp(get_allocator());
        this->_M_impl._M_swap_data(__tmp._M_impl);
        this->_M_impl._M_swap_data(__x._M_impl);
        std::__detail::__alloc_on_move(_M_get_Tp_allocator(), __x._M_get_Tp_allocator());
      }

      void _M_move_assign(vector&& __x, std::false_type) {
        if(__x._M_get_Tp_allocator() == this->_M_get_Tp_allocator()) {
          _M_move_assign(std::move(__x), std::true_type());
        } else {
          this->assign(std::__detail::__make_move_if_noexcept_iterator(__x.begin()),
                       std::__detail::__make_move_if_noexcept_iterator(__x.end()));
          __x.clear();
        }
      }

#     if !(_CXX_OLD_CODE)
      template<class _Up>
      _Up* _M_data_ptr(_Up* __ptr) const { return __ptr; }

      template<class _Ptr>
      typename pointer_traits<_Ptr>::element_type*
      _M_data_ptr(_Ptr __ptr) const {
        return empty() ? nullptr : std::__detail::__addressof(*__ptr);
      }
#     else
      template<class _Ptr>
      _Ptr _M_data_ptr(_Ptr __ptr) const { return __ptr; }
#     endif
      // End of std::vector internal implementations

  }; // End of vector class definition


  // Start of comparison operators for std::vector:
  template<class _Tp, class _Alloc>
  inline bool operator==(const vector<_Tp, _Alloc>& __lhs, const vector<_Tp, _Alloc>& __rhs) {
    return (__lhs.size() == __rhs.size() && std::equal(__lhs.begin(), __lhs.end(), __rhs.begin()));
  }

  template<class _Tp, class _Alloc>
  inline bool operator<(const vector<_Tp, _Alloc>& __lhs, const vector<_Tp, _Alloc>& __rhs) {
    return lexicographical_compare(__lhs.begin(), __lhs.end(), __rhs.begin(), __rhs.end());
  }

  template<class _Tp, class _Alloc>
  inline bool operator!=(const vector<_Tp, _Alloc>& __lhs, const vector<_Tp, _Alloc>& __rhs) {
    return !(__lhs == __rhs);
  }

  template<class _Tp, class _Alloc>
  inline bool operator>(const vector<_Tp, _Alloc>& __lhs, const vector<_Tp, _Alloc>& __rhs) {
    return __rhs < __lhs;
  }

  template<class _Tp, class _Alloc>
  inline bool operator<=(const vector<_Tp, _Alloc>& __lhs, const vector<_Tp, _Alloc>& __rhs) {
    return !(__rhs < __lhs);
  }

  template<class _Tp, class _Alloc>
  inline bool operator>=(const vector<_Tp, _Alloc>& __lhs, const vector<_Tp, _Alloc>& __rhs) {
    return !(__lhs < __rhs);
  }
  // End of std::vector comparison operators


  // swap() specialization for std::vector:
  template<class _Tp, class _Alloc>
  inline void swap(vector<_Tp, _Alloc>& __x, vector<_Tp, _Alloc>& __y) {
    __x.swap(__y);
  }
  // End of std::vector main implementation

  
  _END_NAMESPACE_CONTAINER // for debugging

} // end of std:: namespace

// End of bits/stl_vector.h header implementation


// Start of bits/stl_bvector.h header implementation from libstdc++:

namespace std _DLIB_VISIBILITY {

  _BEGIN_NAMESPACE_CONTAINER // for debugging

  
  // Start of _Bit_reference implementation:
  namespace __detail { // namespace for internal implementations

    typedef unsigned long _Bit_type;
    enum { _S_word_bit = int(__CHAR_BIT__ * sizeof(_Bit_type)) };

    struct _Bit_reference {
      _Bit_type* _M_p;
      _Bit_type _M_mask;

      _Bit_reference(_Bit_type* __x, _Bit_type __y)
        : _M_p(__x), _M_mask(__y) { }

      _Bit_reference() noexcept : _M_p(0), _M_mask(0) { }

      inline operator bool() const noexcept {
        return ~(*_M_p & _M_mask);
      }

      inline _Bit_reference& operator=(bool __x) noexcept {
        if(__x) *_M_p |= _M_mask;
        else *_M_p &= ~_M_mask;
        return *this;
      }

      inline _Bit_reference& operator=(const _Bit_reference& __x) noexcept {
        return *this = bool(__x);
      }

      inline bool operator==(const _Bit_reference& __x) const {
        return bool(*this) == bool(__x);
      }

      inline bool operator<(const _Bit_reference& __x) const {
        return !bool(*this) && bool(__x);
      }

      inline void flip() noexcept {
        *_M_p ^= _M_mask;
      }
    }; // end of _Bit_reference class definition

  } // end of std::__detail:: namespace


  // Start of std::swap specializations for std::__detail::_Bit_reference:
  inline void swap(_GLIBCXX_STD_C::__detail::_Bit_reference __x, _GLIBCXX_STD_C::__detail::_Bit_reference __y) noexcept {
    bool __tmp = __x;
    __x = __y;
    __y = __tmp;
  }

  inline void swap(_GLIBCXX_STD_C::__detail::_Bit_reference __x, bool& __y) noexcept {
    bool __tmp = __x;
    __x = __y;
    __y = __tmp;
  }

  inline void swap(bool& __x, _GLIBCXX_STD_C::__detail::_Bit_reference __y) noexcept {
    bool __tmp = __x;
    __x = __y;
    __y = __tmp;
  }
  // End of std::swap specializations for std::__detail::_Bit_reference
  // End of _Bit_reference implementation


  // Start of _Bit_iterator implementation:
  namespace __detail { // namespace for internal implementations

    struct _Bit_iterator_base
      : public std::iterator<random_access_iterator_tag, bool> {
      _Bit_type* _M_p;
      unsigned int _M_offset;

      _Bit_iterator_base(_Bit_type* __x, unsigned int __y)
        : _M_p(__x), _M_offset(__y) { }

      void _M_bump_up() {
        if(_M_offset++ == int(_S_word_bit) - 1) {
          _M_offset = 0;
          ++_M_p;
        }
      }

      void _M_bump_down() {
        if(_M_offset-- == 0) {
          _M_offset = int(_S_word_bit) - 1;
          --_M_p;
        }
      }

      void _M_incr(ptrdiff_t __i) {
        difference_type __n = __i + _M_offset;
        _M_p += __n / int(_S_word_bit);
        __n = __n % int(_S_word_bit);
        if(__n < 0) {
          __n += int(_S_word_bit);
          --_M_p;
        }
        _M_offset = static_cast<unsigned int>(__n);
      }

      inline bool operator==(const _Bit_iterator_base& __i) const {
        return _M_p == __i._M_p && _M_offset == __i._M_offset;
      }

      inline bool operator<(const _Bit_iterator_base& __i) const {
        return _M_p < __i._M_p || (_M_p == __i._M_p && _M_offset < __i._M_offset);
      }

      inline bool operator!=(const _Bit_iterator_base& __i) const {
        return !(*this == __i);
      }

      inline bool operator>(const _Bit_iterator_base& __i) const {
        return __i < *this;
      }

      inline bool operator<=(const _Bit_iterator_base& __i) const {
        return !(__i < *this);
      }

      inline bool operator>=(const _Bit_iterator_base& __i) const {
        return !(*this < __i);
      }
    }; // end of _Bit_iterator_base class definition

  } // end of std::__detail:: namespace

  inline ptrdiff_t operator-(const _GLIBCXX_STD_C::__detail::_Bit_iterator_base& __lhs,
                             const _GLIBCXX_STD_C::__detail::_Bit_iterator_base& __rhs) {
    return (int(_GLIBCXX_STD_C::__detail::_S_word_bit) * (__lhs._M_p - __rhs._M_p) + __lhs._M_offset - __rhs._M_offset);
  }

  namespace __detail { // continuing internal implementation namespace

    struct _Bit_iterator : public _Bit_iterator_base {
      typedef _Bit_reference reference;
      typedef _Bit_reference* pointer;
      typedef _Bit_iterator iterator;

      _Bit_iterator() : _Bit_iterator_base(0, 0) { }

      _Bit_iterator(_Bit_type* __x, unsigned int __y)
        : _Bit_iterator_base(__x, __y) { }

      inline iterator _M_const_cast() const { return *this; }

      inline reference operator*() const {
        return reference(_M_p, 1UL << _M_offset);
      }

      inline iterator& operator++() {
        _M_bump_up();
        return *this;
      }

      inline iterator operator++(int) {
        iterator __tmp = *this;
        _M_bump_up();
        return __tmp;
      }

      inline iterator& operator--() {
        _M_bump_down();
        return *this;
      }

      inline iterator operator--(int) {
        iterator __tmp = *this;
        _M_bump_down();
        return __tmp;
      }

      inline iterator& operator+=(difference_type __i) {
        _M_incr(__i);
        return *this;
      }

      inline iterator& operator-=(difference_type __i) {
        *this += -__i;
        return *this;
      }

      inline iterator operator+(difference_type __i) const {
        iterator __tmp = *this;
        return __tmp += __i;
      }

      inline iterator operator-(difference_type __i) const {
        iterator __tmp = *this;
        return __tmp -= __i;
      }

      inline reference operator[](difference_type __i) const {
        return *(*this + __i);
      }
    }; // end of _Bit_iterator class definition

  } // end of std::__detail:: namespace

  inline _GLIBCXX_STD_C::__detail::_Bit_iterator
  operator+(ptrdiff_t __n, const _GLIBCXX_STD_C::__detail::_Bit_iterator& __x) {
    return __x + __n;
  }

  namespace __detail { // continued internal namespace

    struct _Bit_const_iterator : public _Bit_iterator_base {
      typedef bool reference;
      typedef bool const_reference;
      typedef const bool* pointer;
      typedef _Bit_const_iterator const_iterator;

      _Bit_const_iterator() : _Bit_iterator_base(0, 0) { }

      _Bit_const_iterator(_Bit_type* __x, unsigned int __y)
        : _Bit_iterator_base(__x, __y) { }
      
      _Bit_const_iterator(const _Bit_iterator& __x)
        : _Bit_iterator_base(__x._M_p, __x._M_offset) { }

      inline _Bit_iterator _M_const_cast() const {
        return _Bit_iterator(_M_p, _M_offset);
      }

      inline const_reference operator*() const {
        return _Bit_reference(_M_p, 1UL << _M_offset);
      }

      inline const_iterator& operator++() {
        _M_bump_up();
        return *this;
      }

      inline const_iterator operator++(int) {
        const_iterator __tmp = *this;
        _M_bump_up();
        return __tmp;
      }

      inline const_iterator& operator--() {
        _M_bump_down();
        return *this;
      }

      inline const_iterator operator--(int) {
        const_iterator __tmp = *this;
        _M_bump_down();
        return __tmp;
      }

      inline const_iterator& operator+=(difference_type __i) {
        _M_incr(__i);
        return *this;
      }

      inline const_iterator& operator-=(difference_type __i) {
        *this += -__i;
        return *this;
      }

      inline const_iterator operator+(difference_type __i) const {
        const_iterator __tmp = *this;
        return __tmp += __i;
      }

      inline const_iterator operator-(difference_type __i) const {
        const_iterator __tmp = *this;
        return __tmp -= __i;
      }

      const_reference operator[](difference_type __i) const {
        return *(*this + __i);
      }
    }; // end of _Bit_const_iterator class definition

  } // end of std::__detail:: namespace


  inline _GLIBCXX_STD_C::__detail::_Bit_const_iterator
  operator+(ptrdiff_t __n, const _GLIBCXX_STD_C::__detail::_Bit_const_iterator& __x) {
    return __x + __n;
  }


  namespace __detail { // namespace for internal implementation

    inline void __fill_bvector(_Bit_iterator __first, _Bit_iterator __last, bool __x) {
      for(; __first != __last; ++__first) *__first = __x;
    }

  } // end of std::__detail:: namespace

  inline void fill(_GLIBCXX_STD_C::__detail::_Bit_iterator __first, _GLIBCXX_STD_C::__detail::_Bit_iterator __last, const bool& __x) {
    if(__first._M_p != __last._M_p) {
      std::fill(__first._M_p + 1, __last._M_p, __x ? ~0 : 0);
      _GLIBCXX_STD_C::__detail::__fill_bvector(__first, _GLIBCXX_STD_C::__detail::_Bit_iterator(__first._M_p + 1, 0), __x);
      _GLIBCXX_STD_C::__detail::__fill_bvector(_GLIBCXX_STD_C::__detail::_Bit_iterator(__last._M_p, 0), __last, __x);
    } else _GLIBCXX_STD_C::__detail::__fill_bvector(__first, __last, __x);
  }
  // End of _Bit_iterator implementation


  // Start of std::__detail::_Bvector_base internal implementation:
  namespace __detail { // namespace for internal implementations

    template<class _Alloc>
    struct _Bvector_base {
      typedef typename _Alloc::template rebind<_Bit_type>::other _Bit_alloc_type;

      // nested implementation class:
      struct _Bvector_impl : public _Bit_alloc_type {
        _Bit_iterator _M_start;
        _Bit_iterator _M_finish;
        _Bit_type* _M_end_of_storage;

        _Bvector_impl() 
          : _Bit_alloc_type(), _M_start(), _M_finish(), _M_end_of_storage(0) { }

        _Bvector_impl(const _Bit_alloc_type& __a) 
          : _Bit_alloc_type(__a), _M_start(), _M_finish(), _M_end_of_storage(0) { }
        
        _Bvector_impl(_Bit_alloc_type&& __a)
          : _Bit_alloc_type(std::move(__a)), _M_start(), _M_finish(), _M_end_of_storage(0) { }
      }; // end of _Bvector_impl nested class definition

      public:
        typedef _Alloc allocator_type;

        _Bit_alloc_type& _M_get_Bit_allocator() noexcept {
          return *static_cast<_Bit_alloc_type*>(&this->_M_impl);
        }

        const _Bit_alloc_type& _M_get_Bit_allocator() const noexcept {
          return *static_cast<const _Bit_alloc_type*>(&this->_M_impl);
        }

        allocator_type get_allocator() const noexcept {
          return allocator_type(_M_get_Bit_allocator());
        }

        _Bvector_base() : _M_impl() { }

        _Bvector_base(const allocator_type& __a) : _M_impl(__a) { }

        _Bvector_base(_Bvector_base&& __x) noexcept
          : _M_impl(std::move(__x._M_get_Bit_allocator())) {
          this->_M_impl._M_start = __x._M_impl._M_start;
          this->_M_impl._M_finish = __x._M_impl._M_finish;
          this->_M_impl._M_end_of_storage = __x._M_impl._M_end_of_storage;
          __x._M_impl._M_start = _Bit_iterator();
          __x._M_impl._M_finish = _Bit_iterator();
          __x._M_impl._M_end_of_storage = 0;
        }

        ~_Bvector_base() { this->_M_deallocate(); }

      protected:
        _Bvector_impl _M_impl;

        _Bit_type* _M_allocate(size_t __n) {
          return _M_impl.allocate(_S_nword(__n));
        }

        void _M_deallocate() {
          if(_M_impl._M_start._M_p)
            _M_impl.deallocate(_M_impl._M_start._M_p, _M_impl._M_end_of_storage - _M_impl._M_start._M_p);
        }

        static size_t _S_nword(size_t __n) {
          return (__n + int(_S_word_bit) - 1) / int(_S_word_bit);
        }
    }; // end of _Bvector_base class definition

  } // end of std::__detail:: namespace
  // End of std::__detail::_Bvector_base internal implementation


  // Start of main bvector implementation as a std::vector specialization:
  /*
    This class is exactly the same as a regular vector, but some functions have slightly different behavior.
    This specialization is required since a regular vector using booleans would
    not meet the requirements of container in the c++ standard due to how booleans work in c++.
    This class mainly just uses custom internal types to make boolean references and pointers actually act like references and pointers.
  */
  template<class _Alloc>
  class vector<bool, _Alloc> : protected _GLIBCXX_STD_C::__detail::_Bvector_base<_Alloc> {
    // typedef for base type
    typedef _GLIBCXX_STD_C::__detail::_Bvector_base<_Alloc> _Base;

    template<class>
    friend struct hash; // for bits/functional_hash implementation

    public:
      // public container and iterator typedefs
      typedef bool value_type;
      typedef size_t size_type;
      typedef ptrdiff_t difference_type;
      typedef _GLIBCXX_STD_C::__detail::_Bit_reference reference;
      typedef bool const_reference;
      typedef _GLIBCXX_STD_C::__detail::_Bit_reference* pointer;
      typedef const bool* const_pointer;
      typedef _GLIBCXX_STD_C::__detail::_Bit_iterator iterator;
      typedef _GLIBCXX_STD_C::__detail::_Bit_const_iterator const_iterator;
      typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
      typedef std::reverse_iterator<iterator> reverse_iterator;
      typedef _Alloc allocator_type;

      // returns the allocator being used
      allocator_type get_allocator() const {
        return _Base::get_allocator();
      }

    protected:
      // functions inherited from base
      using _Base::_M_allocate;
      using _Base::_M_deallocate;
      using _Base::_S_nword;
      using _Base::_M_get_Bit_allocator;
    
    public:
      // Start of public bvector constructors
      vector() : _Base() { }

      explicit vector(const allocator_type& __a) : _Base(__a) { }

#     if !(_CXX_OLD_CODE)
      explicit vector(size_type __n, const allocator_type& __a = allocator_type())
        : vector(__n, false, __a) { }

      vector(size_type __n, const bool& __value,
             const allocator_type& __a = allocator_type()) : _Base(__a) {
        _M_initialize(__n);
        std::fill(this->_M_impl._M_start._M_p, this->_M_impl._M_end_of_storage,
                  __value ? ~0 : 0);
      }
#     else
      explicit vector(size_type __n, const bool& __value = bool(),
                      const allocator_type& __a = allocator_type()) : _Base(__a) {
        _M_initialize(__n);
        std::fill(this->_M_impl._M_start._M_p, this->_M_impl._M_end_of_storage,
                  __value ? ~0 : 0);
      }
#     endif

      vector(const vector& __x) : _Base(__x._M_get_Bit_allocator()) {
        _M_initialize(__x.size());
        _M_copy_aligned(__x.begin(), __x.end(), this->_M_impl._M_start);
      }

      vector(vector&& __x) noexcept
        : _Base(std::move(__x)) { }
      
      vector(initializer_list<bool> __l,
             const allocator_type& __a = allocator_type()) : _Base(__a) {
        _M_initialize_range(__l.begin(), __l.end(), random_access_iterator_tag());
      }

#     if !(_CXX_OLD_CODE)
      template<class _InputIterator, class = std::__detail::_RequireInputIter<_InputIterator>>
      vector(_InputIterator __first, _InputIterator __last,
             const allocator_type& __a = allocator_type()) : _Base(__a) {
        _M_initialize_dispatch(__first, __last, std::__detail::__false_type());
      }
#     else
      template<class _InputIterator>
      vector(_InputIterator __first, _InputIterator __last,
             const allocator_type& __a = allocator_type()) : _Base(__a) {
        typedef typename std::__detail::__is_integer<_InputIterator>::__type _Integral;
        _M_initialize_dispatch(__first, __last, _Integral());
      }
#     endif
      // End of bvector constructors


      // bvector destructor
      ~vector() noexcept { // empty definition in libstdc++
        //std::__detail::_Destroy(this->_M_impl._M_start, this->_M_impl._M_finish,
        //                   _M_get_Tp_allocator());
      }


      // Start of bvector assignment operators:
      vector& operator=(const vector& __x) {
        if(&__x == this) return *this;
        if(__x.size() > capacity()) {
          this->_M_deallocate();
          _M_initialize(__x.size());
        }
        this->_M_impl._M_finish = _M_copy_aligned(__x.begin(), __x.end(), begin());
        return *this;
      }

      vector& operator=(vector&& __x) {
        this->clear();
        this->swap(__x);
        return *this;
      }

      vector& operator=(initializer_list<bool> __l) {
        this->assign(__l.begin(), __l.end());
        return *this;
      }
      // End of bvector assignment operators


      // Start of bvector assign() method implementation:
      inline void assign(size_type __n, const bool& __x) {
        _M_fill_assign(__n, __x);
      }

#     if !(_CXX_OLD_CODE)
      template<class _InputIterator, class = std::__detail::_RequireInputIter<_InputIterator>>
      void assign(_InputIterator __first, _InputIterator __last) {
        _M_assign_dispatch(__first, __last, std::__detail::__false_type());
      }
#     else
      template<class _InputIterator>
      void assign(_InputIterator __first, _InputIterator __last) {
        typedef typename std::__detail::__is_integer<_InputIterator>::__type _Integral;
        _M_assign_dispatch(__first, __last, _Integral());
      }
#     endif

      inline void assign(initializer_list<bool> __l) {
        this->assign(__l.begin(), __l.end());
      }
      // End of bvector assign() implementation


      // Start of bvector iterator functions:
      inline iterator begin() noexcept {
        return this->_M_impl._M_start;
      }

      constexpr const_iterator begin() const noexcept {
        return this->_M_impl._M_start;
      }

      inline iterator end() noexcept {
        return this->_M_impl._M_finish;
      }

      constexpr const_iterator end() const noexcept {
        return this->_M_impl._M_finish;
      }

      inline reverse_iterator rbegin() noexcept {
        return reverse_iterator(end());
      }

      constexpr const_reverse_iterator rbegin() const noexcept {
        return const_reverse_iterator(end());
      }

      inline reverse_iterator rend() noexcept {
        return reverse_iterator(begin());
      }

      constexpr const_reverse_iterator rend() const noexcept {
        return const_reverse_iterator(begin());
      }

      constexpr const_iterator cbegin() const noexcept {
        return this->_M_impl._M_start;
      }

      constexpr const_iterator cend() const noexcept {
        return this->_M_impl._M_finish;
      }

      constexpr const_reverse_iterator crbegin() const noexcept {
        return const_reverse_iterator(end());
      }

      constexpr const_reverse_iterator crend() const noexcept {
        return const_reverse_iterator(begin());
      }
      // End of bvector iterator functions

      // Start of bvector capacity functions:
      inline size_type size() const noexcept {
        return size_type(end() - begin());
      }

      constexpr size_type max_size() const noexcept {
        const size_type __isize 
          = __gnu_cxx::__numeric_traits<difference_type>::__max - int(_GLIBCXX_STD_C::__detail::_S_word_bit) + 1;
        const size_type __asize = _M_get_Bit_allocator().max_size();
        return (__asize <= __isize / int(_GLIBCXX_STD_C::__detail::_S_word_bit) 
              ? __asize * int(_GLIBCXX_STD_C::__detail::_S_word_bit) 
              : __isize);
      }

      size_type capacity() const noexcept {
        return size_type(const_iterator(this->_M_impl._M_end_of_storage, 0) - begin());
      }

      constexpr bool empty() const noexcept {
        return begin() == end();
      }
      // End of bvector capacity functions


      // Start of bvector subscript operator implementation:
      inline reference operator[](size_type __n) {
        return *iterator(this->_M_impl._M_start._M_p + __n / int(_GLIBCXX_STD_C::__detail::_S_word_bit),
                         __n % int(_GLIBCXX_STD_C::__detail::_S_word_bit));
      }

      constexpr const_reference operator[](size_type __n) const {
        return *const_iterator(this->_M_impl._M_start._M_p + __n / int(_GLIBCXX_STD_C::__detail::_S_word_bit),
                               __n % int(_GLIBCXX_STD_C::__detail::_S_word_bit));
      }
      // End of bvector subscript operator implementation

      // Internal range check function:
    protected:
      constexpr void _M_range_check(size_type __n) const {
        if(__n >= this->size())
          __throw_out_of_range_fmt(__N("vector<bool>::_M_range_check: __n "
                                       "(which is %zu) >= this->size() "
                                       "(which is %zu)"),
                                       __n, this->size());
      }


    public:
      // Start of bvector element access functions:
      inline reference at(size_type __n) {
        _M_range_check(__n);
        return (*this)[__n];
      }

      constexpr const_reference at(size_type __n) const {
        _M_range_check(__n);
        return (*this)[__n];
      }

      void reserve(size_type __n) { // not member access, but its defined with them
        if(__n > max_size()) __throw_length_error(__N("vector::reserve"));
        if(capacity() < __n) _M_reallocate(__n);
      }

      inline reference front() {
        return *begin();
      }

      constexpr const_reference front() const {
        return *begin();
      }

      inline reference back() {
        return *(end() - 1);
      }

      constexpr const_reference back() const {
        return *(end() - 1);
      }

      void data() noexcept { }

      void push_back(bool __x) {
        if(this->_M_impl._M_finish._M_p != this->_M_impl._M_end_of_storage) *this->_M_impl._M_finish++ = __x;
        else _M_insert_aux(end(), __x);
      }

      void swap(vector& __x) {
        std::swap(this->_M_impl._M_start, __x._M_impl._M_start);
        std::swap(this->_M_impl._M_finish, __x._M_impl._M_finish);
        std::swap(this->_M_impl._M_end_of_storage, __x._M_impl._M_end_of_storage);
        std::__detail::__alloc_swap<typename _Base::_Bit_alloc_type>::_S_do_it(_M_get_Bit_allocator(), 
                                                                          __x._M_get_Bit_allocator());
      }

      static void swap(reference __x, reference __y) noexcept {
        bool __tmp = __x;
        __x = __y;
        __y = __tmp;
      }

      iterator insert(const_iterator __position, const bool& __x = bool()) {
        const difference_type __n = __position - begin();
        if(this->_M_impl._M_finish._M_p != this->_M_impl._M_end_of_storage
          && __position == end()) *this->_M_impl._M_finish++ = __x;
        else _M_insert_aux(__position._M_const_cast(), __x);
        return begin() + __n;
      }

#     if !(_CXX_OLD_CODE)
      template<class _InputIterator, class = std::__detail::_RequireInputIter<_InputIterator>>
      iterator insert(const_iterator __position, _InputIterator __first, _InputIterator __last) {
        difference_type __offset = __position - cbegin();
        _M_insert_dispatch(__position._M_const_cast(), __first, __last, std::__detail::__false_type());
        return begin() + __offset;
      }
#     else
      template<class _InputIterator>
      inline void insert(iterator __position, _InputIterator __first, _InputIterator __last) {
        typedef typename std::__detail::__is_integer<_InputIterator>::__type _Integral;
        _M_insert_dispatch(__position, __first, __last, _Integral());
      }
#     endif

#     if !(_CXX_OLD_CODE)
      iterator insert(const_iterator __position, size_type __n, const bool& __x) {
        difference_type __offset = __position - cbegin();
        _M_fill_insert(__position._M_const_cast(), __n, __x);
        return begin() + __offset;
      }
#     else
      inline void insert(iterator __position, size_type __n, const bool& __x) {
        _M_fill_insert(__position, __n, __x);
      }
#     endif

      inline iterator insert(const_iterator __p, initializer_list<bool> __l) {
        return this->insert(__p, __l.begin(), __l.end());
      }

      constexpr void pop_back() {
        --this->_M_impl._M_finish;
      }

      inline iterator erase(const_iterator __position) {
        return _M_erase(__position._M_const_cast());
      }

      inline iterator erase(const_iterator __first, const_iterator __last) {
        return _M_erase(__first._M_const_cast(), __last._M_const_cast());
      }

      void resize(size_type __new_size, bool __x = bool()) {
        if(__new_size < size()) _M_erase_at_end(begin() + difference_type(__new_size));
        else insert(end(), __new_size - size(), __x);
      }

      constexpr void shrink_to_fit() { _M_shrink_to_fit(); }

      void flip() noexcept {
        for(_GLIBCXX_STD_C::__detail::_Bit_type* __p = this->_M_impl._M_start._M_p; __p != this->_M_impl._M_end_of_storage; ++__p)
          *__p = ~*__p;
      }

      constexpr void clear() noexcept {
        _M_erase_at_end(begin());
      }

      template<class... _Args>
      inline void emplace_back(_Args&&... __args) {
        push_back(bool(__args...));
      }

      template<class... _Args>
      inline iterator emplace(const_iterator __pos, _Args&&... __args) {
        return insert(__pos, bool(__args...));
      }
      // End of bvector element access functions


    protected:
      // Start of internal implementation functions for bvector:
      iterator _M_copy_aligned(const_iterator __first, const_iterator __last, iterator __result) {
        _GLIBCXX_STD_C::__detail::_Bit_type* __q = std::copy(__first._M_p, __last._M_p, __result._M_p);
        return std::copy(const_iterator(__last._M_p, 0), __last, iterator(__q, 0));
      }

      void _M_initialize(size_type __n) {
        _GLIBCXX_STD_C::__detail::_Bit_type* __q = this->_M_allocate(__n);
        this->_M_impl._M_end_of_storage = __q + _S_nword(__n);
        this->_M_impl._M_start = iterator(__q, 0);
        this->_M_impl._M_finish = this->_M_impl._M_start + difference_type(__n);
      }

      void _M_reallocate(size_type __n);

      bool _M_shrink_to_fit();

      template<class _Integer>
      void _M_initialize_dispatch(_Integer __n, _Integer __x, std::__detail::__true_type) {
        _M_initialize(static_cast<size_type>(__n));
        std::fill(this->_M_impl._M_start._M_p, this->_M_impl._M_end_of_storage, __x ? ~0 : 0);
      }

      template<class _InputIterator>
      inline void _M_initialize_dispatch(_InputIterator __first, _InputIterator __last, std::__detail::__false_type) {
        _M_initialize_range(__first, __last, std::__detail::__iterator_category(__first));
      }

      template<class _InputIterator>
      void _M_initialize_range(_InputIterator __first, _InputIterator __last, input_iterator_tag) {
        for(; __first != __last; ++__first) push_back(*__first);
      }

      template<class _ForwardIterator>
      void _M_initialize_range(_ForwardIterator __first, _ForwardIterator __last, forward_iterator_tag) {
        const size_type __n = std::distance(__first, __last);
        _M_initialize(__n);
        std::copy(__first, __last, this->_M_impl._M_start);
      }

      template<class _Integer>
      inline void _M_assign_dispatch(_Integer __n, _Integer __val, std::__detail::__true_type) {
        _M_fill_assign(__n, __val);
      }

      template<class _InputIterator>
      inline void _M_assign_dispatch(_InputIterator __first, _InputIterator __last, std::__detail::__false_type) {
        _M_assign_aux(__first, __last, std::__detail::__iterator_category(__first));
      }

      void _M_fill_assign(size_t __n, bool __x) {
        if(__n > size()) {
          std::fill(this->_M_impl._M_start._M_p,
                    this->_M_impl._M_end_of_storage,
                    __x ? ~0 : 0);
          insert(end(), __n - size(), __x);
        } else {
          _M_erase_at_end(begin() + __n);
          std::fill(this->_M_impl._M_start._M_p,
                    this->_M_impl._M_end_of_storage,
                    __x ? ~0 : 0);
        }
      }

      template<class _InputIterator>
      void _M_assign_aux(_InputIterator __first, _InputIterator __last, input_iterator_tag) {
        iterator __cur = begin();
        for(; __first != __last && __cur != end(); ++__cur, ++__first) *__cur = *__first;
        if(__first == __last) _M_erase_at_end(__cur);
        else insert(end(), __first, __last);
      }

      template<class _ForwardIterator>
      void _M_assign_aux(_ForwardIterator __first, _ForwardIterator __last, forward_iterator_tag) {
        const size_type __len = std::distance(__first, __last);
        if(__len < size()) _M_erase_at_end(std::copy(__first, __last, begin()));
        else {
          _ForwardIterator __mid = __first;
          std::advance(__mid, size());
          std::copy(__first, __mid, begin());
          insert(end(), __mid, __last);
        }
      }

      template<class _Integer>
      inline void _M_insert_dispatch(iterator __pos, _Integer __n, _Integer __x, std::__detail::__true_type) {
        _M_fill_insert(__pos, __n, __x);
      }

      template<class _InputIterator>
      inline void _M_insert_dispatch(iterator __pos, _InputIterator __first,
                                     _InputIterator __last, std::__detail::__false_type) {
        _M_insert_range(__pos, __first, __last, std::__detail::__iterator_category(__first));
      }

      void _M_fill_insert(iterator __position, size_type __n, bool __x);

      template<class _InputIterator>
      void _M_insert_range(iterator __pos, _InputIterator __first,
                           _InputIterator __last, input_iterator_tag) {
        for(; __first != __last; ++__first) {
          __pos = insert(__pos, *__first);
          ++__pos;
        }
      }

      template<class _ForwardIterator>
      void _M_insert_range(iterator __position, _ForwardIterator __first,
                           _ForwardIterator __last, forward_iterator_tag);
      
      void _M_insert_aux(iterator __position, bool __x);

      size_type _M_check_len(size_type __n, const char* __s) const {
        if(max_size() - size() < __n) __throw_length_error(__N(__s));
        const size_type __len = size() + max(size(), __n);
        return (__len < size() || __len > max_size()) ? max_size() : __len;
      }

      void _M_erase_at_end(iterator __pos) {
        this->_M_impl._M_finish = __pos;
      }

      iterator _M_erase(iterator __pos);

      iterator _M_erase(iterator __first, iterator __last);
      // End of bvector internal functions

  }; // end of vector class specialization
  // End of main bvector implementation


  _END_NAMESPACE_CONTAINER // for debugging

} // end of std:: namespace

// End of bits/stl_bvector.h header implementation


// seperate std namespace for hash implementation outside of the container namespace
namespace std _DLIB_VISIBILITY {

  template<class _Alloc>
  struct hash<_GLIBCXX_STD_C::vector<bool, _Alloc>>
    : public __detail::__hash_base<size_t, _GLIBCXX_STD_C::vector<bool, _Alloc>> {
    size_t operator()(const _GLIBCXX_STD_C::vector<bool, _Alloc>&) const noexcept;
  };

} // end of std:: namespace


// Start Conditional includes for debugging and out of line definitions:
#if !(_EXPORT_TEMPLATE)
// contains the definitions for the functions that were defined but not declared.
// I might just get rid of the export template macro, and I will move the code in Vector.hh to this file.
#include "Vector.hh" 
#endif

#if (_DEBUG_)
#include "debug/Debug_Vector.hpp"
#endif
// End of conditional includes


#endif // End of Vector.hpp
