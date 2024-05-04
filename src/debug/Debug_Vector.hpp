// Libstdc++ and GCC GNU Lincese boilerplate:      (this is required since this code is copyrighted under the GCC compiler. I did not design this myself.)
/*
  Partial Modified Implementation Of Meta-Functions from the <bits/vector.tcc> Libstdc++ header  -*- C++ -*-

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
  Debug_Vector.hpp  [V1.0.0]  (template class debug header file)
    By: Andrey Stroitelev

  ~This file is a copy of the libstdc++ debug/vector.h header file
  ~This file is used for the implementation of vector that is used in debug mode(toggled via a macro definition in util/Config.h)
  ~The debug mode classes work the exact same, but they do even more checks(more strict requirements).
    *This is to see if you are using the class in a way that can cause a bug but not an error.
*/
/*
  Possible Changes:
    *I might remove the _CXX_OLD_CODE toggle in the debug mode std::vector.swap()
     function to make the __glibcxx_check_equal_allocs macro checker mandatory.
    *I might make the _M_base() function private since I dont see why it should be public(it's public in the original libstdc++ source code).
     If I make _M_base() private I will also declare comparison operators as friend functions to access private members.
*/


// Start of Debug_Vector.hpp
#ifndef _DEBUG_VECTOR_HPP_
#define _DEBUG_VECTOR_HPP_

#ifndef _DEBUG_VECTOR_VERSION_
#define _DEBUG_VECTOR_VERSION_ 0x010000
#endif


/*
  Including files like this will work in traditional c++, but when in an arduino sketch folder
  including from the parent folder doesn't work due to how the Arduino IDE compiles a sketch.
  I'm not sure if this is the same for libraries since it's an external directory.
  I haven't found much online about this kind of scenario specifically, and I haven't tested
  to see if this works yet, so I'm going to just leave it like this for now to better 
  organize this library.
  If including through a parent folder doesn't end up working then I will have to move
  all of the debug mode container files into either the main src/ folder along with 
  the main container implementation files, or I might just move them into the util/ folder.
  I also dont fully understand yet how to use the "../" in the include statement, so
  there is a decent chance I just have the syntax wrong if this doesn't end up working 
  when doing "../src/"
*/
#include <Arduino.h>
#include "../src/util/Config.h"
#include "../src/util/Utility.hpp"
#include "../src/Vector.hpp"
#include "../src/util/Debug.hpp"


_DLIB_HEADER


// Start of debug mode vector implementation:

namespace std _DLIB_VISIBILITY {

  namespace __debug {

    template<class _Tp, class _Allocator = std::allocator<_Tp>>
    class vector : public _GLIBCXX_STD_C::vector<_Tp, _Allocator>,
                   public __gnu_debug::_Safe_sequence<vector<_Tp, _Allocator>> {
      // Debug versions of private typedefs:
      typedef _GLIBCXX_STD_C::vector<_Tp, _Allocator> _Base;

      typedef typename _Base::iterator _Base_iterator;
      typedef typename _Base::const_iterator _Base_const_iterator;
      typedef __gnu_debug::_Equal_to<_Base_const_iterator> _Equal;

      typedef __gnu_debug::_Safe_sequence<vector<_Tp, _Allocator>> _Safe_base;
      typedef __gnu_cxx::__alloc_traits<_Allocator> _Alloc_traits;


      public:
        // Start of container trait typedefs:
        typedef typename _Base::reference reference;
        typedef typename _Base::const_reference const_reference;

        typedef __gnu_debug::_Safe_iterator<_Base_iterator, vector> iterator;
        typedef __gnu_debug::_Safe_iterator<_Base_const_iterator, vector> const_iterator;

        typedef typename _Base::size_type size_type;
        typedef typename _Base::difference_type difference_type;

        typedef _Tp value_type;
        typedef _Allocator allocator_type;
        typedef typename _Base::pointer pointer;
        typedef typename _Base::const_pointer const_pointer;
        typedef std::reverse_iterator<iterator> reverse_iterator;
        typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
        // End of container traits typedefs


        // Start of debug mode std::vector constructors and destructor:
        vector() noexcept : _Base(), _M_guaranteed_capacity(0) { }

        explicit vector(const _Allocator& __a) noexcept
          : _Base(__a), _M_guaranteed_capacity(0) { }
        
#       if !(_CXX_OLD_CODE)
        explicit vector(size_type __n, const _Allocator& __a = _Allocator())
          : _Base(__n, __a), _M_guaranteed_capacity(__n) { }
        
        vector(size_type __n, const _Tp& __value, const _Allocator& __a = _Allocator())
          : _Base(__n, __value, __a), _M_guaranteed_capacity(__n) { }
#       else
        explicit vector(size_type __n, const _Tp& __value = _Tp(), 
                        const _Allocator& __a = _Allocator())
          : _Base(__n, __value, __a), _M_guaranteed_capacity(__n) { }
#       endif

#       if !(_CXX_OLD_CODE)
        template<class _InputIterator, class = std::__detail::_RequireInputIter<_InputIterator>>
#       else
        template<class _InputIterator>
#       endif
        vector(_InputIterator __first, _InputIterator __last, 
               const _Allocator& __a = _Allocator())
          : _Base(__gnu_debug::__base(__gnu_debug::__check_valid_range(__first, __last)),
                  __gnu_debug::__base(__last), __a),
            _M_guaranteed_capacity(0) { _M_guaranteed_capacity(); }
        
        vector(const vector& __x)
          : _Base(__x), _M_guaranteed_capacity(__x.size()) { }

        vector(const _Base& __x)
          : _Base(__x), _M_guaranteed_capacity(__x.size()) { }

        // These next constructors are originally only c++11 and above, 
        // but I removed the _CXX_OLD_CODE check since it's just rvalue and initializer_list constructors.
        vector(vector&& __x) noexcept
          : _Base(std::move(__x)),
            _Safe_base(std::move(__x)),
            _M_guaranteed_capacity(this->size()) {
          __x._M_guaranteed_capacity = 0;
        }

        vector(const vector& __x, const allocator_type& __a)
          : _Base(__x, __a), _M_guaranteed_capacity(__x.size()) { }

        vector(vector&& __x, const allocator_type& __a)
          : _Base(std::move(__x), __a),
            _M_guaranteed_capacity(this->size()) {
          if(__x.get_allocator() == __a) this->_M_swap(__x);
          else __x._M_invalidate_all();
          __x._M_guaranteed_capacity = 0;
        }

        vector(initializer_list<value_type> __l, 
               const allocator_type& __a = allocator_type())
          : _Base(__l, __a), _M_guaranteed_capacity(__l.size()) { }

        ~vector() noexcept {
          //std::__detail::_Destroy(this->_M_impl._M_start, this->_M_impl._M_finish,
          //                        _M_get_Tp_allocator());
        } // not sure why this is empty in libstdc++. seems you need to manually deallocate before destructing.
        // End of debug mode std::vector constructors and destructor


        // Start of debug vector assignment operators:
        vector& operator=(const vector& __x) {
          _M_base() = __x;
          this->_M_invalidate_all();
          _M_update_guaranteed_capacity();
          return *this;
        }

        vector& operator=(vector&& __x) noexcept(_Alloc_traits::_S_nothrow_move()) {
          __glibcxx_check_self_move_assign(__x);
          bool __xfer_memory = _Alloc_traits::_S_propagate_on_move_assign()
                            || __x.get_allocator() == this->get_allocator();
          _M_base() = std::move(__x._M_base());
          if(__xfer_memory) this->_M_swap(__x);
          else this->_M_invalidate_all();
          _M_update_guaranteed_capacity();
          __x._M_invalidate_all();
          __x._M_guaranteed_capacity = 0;
          return *this;
        }

        vector& operator=(initializer_list<value_type> __l) {
          _M_base() = __l;
          this->_M_invalidate_all();
          _M_update_guaranteed_capacity();
          return *this;
        }
        // End of debug vector assignment operators


        // Start of debug vector assign function implementations:
#       if !(_CXX_OLD_CODE)
        template<class _InputIterator, class = std::__detail::_RequireInputIter<_InputIterator>>
#       else
        template<class _InputIterator>
#       endif
        void assign(_InputIterator __first, _InputIterator __last) {
          __glibcxx_check_valid_range(__first, __last);
          _Base::assign(__gnu_debug::__base(__first),
                        __gnu_debug::__base(__last));
          this->_M_invalidate_all();
          _M_update_guaranteed_capacity();
        }

        void assign(size_type __n, const _Tp& __u) {
          _Base::assign(__n, __u);
          this->_M_invalidate_all();
          _M_update_guaranteed_capacity();
        }

        void assign(initializer_list<value_type> __l) {
          _Base::assign(__l);
          this->_M_invalidate_all();
          _M_update_guaranteed_capacity();
        }
        // End of debug vector assign function implementations


        // Start of debug vector iterator functions:
        using _Base::get_allocator;

        iterator begin() noexcept {
          return iterator(_Base::begin(), this);
        }

        const_iterator begin() const noexcept {
          return const_iterator(_Base::begin(), this);
        }

        iterator end() noexcept {
          return iterator(_Base::end(), this);
        }

        const_iterator end() const noexcept {
          return const_iterator(_Base::end(), this);
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
          return const_iterator(_Base::begin(), this);
        }

        const_iterator cend() const noexcept {
          return const_iterator(_Base::end(), this);
        }

        const_reverse_iterator crbegin() const noexcept {
          return const_reverse_iterator(end());
        }

        const_reverse_iterator crend() const noexcept {
          return const_reverse_iterator(begin());
        }
        // End of debug vector iterator functions


        // Start of debug vector size/capacity functions:
        using _Base::size;
        using _Base::max_size;

#       if !(_CXX_OLD_CODE)
        void resize(size_type __sz) {
          bool __realloc = _M_requires_reallocation(__sz);
          if(__sz < this->size()) this->_M_invalidate_after_nth(__sz);
          _Base::resize(__sz);
          if(__realloc) this->_M_invalidate_all();
          _M_update_guaranteed_capacity();
        }

        void resize(size_type __sz, const _Tp& __c) {
          bool __realloc = _M_requires_reallocation(__sz);
          if(__sz < this->size()) this->_M_invalidate_after_nth(__sz);
          _Base::resize(__sz, __c);
          if(__realloc) this->_M_invalidate_all();
          _M_update_guaranteed_capacity();
        }
#       else
        void resize(size_type __sz, _Tp __c = _Tp()) {
          bool __realloc = _M_requires_reallocation(__sz);
          if(__sz < this->size()) this->_M_invalidate_after_nth(__sz);
          _Base::resize(__sz, __c);
          if(__realloc) this->_M_invalidate_all();
          _M_update_guaranteed_capacity();
        }
#       endif

        void shrink_to_fit() {
          if(_Base::_M_shrink_to_fit()) {
            _M_guaranteed_capacity = _Base::capacity();
            this->_M_invalidate_all();
          }
        }

        size_type capacity() const noexcept {
#         ifdef _DEBUG_PEDANTIC_
          return _M_guaranteed_capacity;
#         else
          return _Base::capacity();
#         endif
        }

        using _Base::empty;

        void reserve(size_type __n) {
          bool __realloc = _M_requires_reallocation(__n);
          _Base::reserve(__n);
          if(__n > _M_guaranteed_capacity) _M_guaranteed_capacity = __n;
          if(__realloc) this->_M_invalidate_all();
        }
        // End of debug vector size/capacity functions


        // Start of debug vector element functions:
        reference operator[](size_type __n) noexcept {
          __glibcxx_check_subscript(__n);
          return _M_base()[__n];
        }

        const_reference operator[](size_type __n) const noexcept {
          __glibcxx_check_subscript(__n);
          return _M_base()[__n];
        }

        using _Base::at; // no change to at function

        reference front() noexcept {
          __glibcxx_check_nonempty();
          return _Base::front();
        }

        const_reference front() const noexcept {
          __glibcxx_check_nonempty();
          return _Base::front();
        }

        reference back() noexcept {
          __glibcxx_check_nonempty();
          return _Base::back();
        }

        const_reference back() const noexcept {
          __glibcxx_check_nonempty();
          return _Base::back();
        }

        using _Base::data;

        void push_back(const _Tp& __x) {
          bool __realloc = _M_requires_reallocation(this->size() + 1);
          _Base::push_back(__x);
          if(__realloc) this->_M_invalidate_all();
          _M_update_guaranteed_capacity();
        }

        template<class _Up = _Tp>
        typename __gnu_cxx::__enable_if<!std::__detail::__are_same<_Up, bool>::__value, void>::__type
        push_back(_Tp&& __x) {
          emplace_back(std::move(__x));
        }

        template<class... _Args>
        void emplace_back(_Args&&... __args) {
          bool __realloc = _M_requires_reallocation(this->size() + 1);
          _Base::emplace_back(std::forward<_Args>(__args)...);
          if(__realloc) this->_M_invalidate_all();
          _M_update_guaranteed_capacity();
        }

        void pop_back() noexcept {
          __glibcxx_check_nonempty();
          this->_M_invalidate_if(_Equal(--_Base::end()));
          _Base::pop_back();
        }

        template<class... _Args>
        iterator emplace(const_iterator __position, _Args&&... __args) {
          __glibcxx_check_insert(__position);
          bool __realloc = _M_requires_reallocation(this->size() + 1);
          difference_type __offset = __position.base() - _Base::begin();
          _Base_iterator __res = _Base::emplace(__position.base(),
                                                std::forward<_Args>(__args)...);
          if(__realloc) this->_M_invalidate_all();
          else this->_M_invalidate_after_nth(__offset);
          _M_update_guaranteed_capacity();
          return iterator(__res, this);
        }

        iterator insert(const_iterator __position, const _Tp& __x) {
          __glibcxx_check_insert(__position);
          bool __realloc = _M_requires_reallocation(this->size() + 1);
          difference_type __offset = __position.base() - _Base::begin();
          _Base_iterator __res = _Base::insert(__position.base(), __x);
          if(__realloc) this->_M_invalidate_all();
          else this->_M_invalidate_after_nth(__offset);
          _M_update_guaranteed_capacity();
          return iterator(__res, this);
        }

        template<class _Up = _Tp>
        typename __gnu_cxx::__enable_if<!std::__detail::__are_same<_Up, bool>::__value, iterator>::__type
        insert(const_iterator __position, _Tp&& __x) {
          return emplace(__position, std::move(__x));
        }

        iterator insert(const_iterator __position, initializer_list<value_type> __l) {
          return this->insert(__position, __l.begin(), __l.end());
        }

#       if !(_CXX_OLD_CODE)
        iterator insert(const_iterator __position, size_type __n, const _Tp& __x) {
          __glibcxx_check_insert(__position);
          bool __realloc = _M_requires_reallocation(this->size() + __n);
          difference_type __offset = __position.base() - _Base::cbegin();
          _Base_iterator __res = _Base::insert(__position.base(), __n, __x);
          if(__realloc) this->_M_invalidate_all();
          else this->_M_invalidate_after_nth(__offset);
          _M_update_guaranteed_capacity();
          return iterator(__res, this);
        }
#       else
        void insert(iterator __position, size_type __n, const _Tp& __x) {
          __glibcxx_check_insert(__position);
          bool __realloc = _M_requires_reallocation(this->size() + __n);
          difference_type __offset = __position.base() - _Base::begin();
          _Base::insert(__position.base(), __n, __x);
          if(__realloc) this->_M_invalidate_all();
          else this->_M_invalidate_after_nth(__offset);
          _M_update_guaranteed_capacity();
        }
#       endif

#       if !(_CXX_OLD_CODE)
        template<class _InputIterator, class = std::__detail::_RequireInputIter<_InputIterator>>
        iterator insert(const_iterator __position, _InputIterator __first, _InputIterator __last) {
          __glibcxx_check_insert_range(__position, __first, __last);
          _Base_iterator __old_begin = _M_base().begin();
          difference_type __offset = __position.base() - _Base::cbegin();
          _Base_iterator __res = _Base::insert(__position.base(),
                                               __gnu_debug::__base(__first),
                                               __gnu_debug::__base(__last));
          if(_M_base().begin() != __old_begin) this->_M_invalidate_all();
          else this->_M_invalidate_after_nth(__offset);
          _M_update_guaranteed_capacity();
          return iterator(__res, this);
        }
#       else
        template<class _InputIterator>
        void insert(iterator __position, _InputIterator __first, _InputIterator __last) {
          __glibcxx_check_insert_range(__position, __first, __last);
          _Base_iterator __old_begin = _M_base().begin();
          difference_type __offset = __position.base() - _Base::begin();
          _Base::insert(__position.base(),
                        __gnu_debug::__base(__first),
                        __gnu_debug::__base(__last));
          if(_M_base().begin() != __old_begin) this->_M_invalidate_all();
          else this->_M_invalidate_after_nth(__offset);
          _M_update_guaranteed_capacity();
        }
#       endif

        iterator erase(const_iterator __position) {
          __glibcxx_check_erase(__position);
          difference_type __offset = __position.base() - _Base::begin();
          _Base_iterator __res = _Base::erase(__position.base());
          this->_M_invalidate_after_nth(__offset);
          return iterator(__res, this);
        }

        iterator erase(const_iterator __first, const_iterator __last) {
          __glibcxx_check_erase_range(__first, __last);
          if(__first.base() != __last.base()) {
            difference_type __offset = __first.base() - _Base::begin();
            _Base_iterator __res = _Base::erase(__first.base(), __last.base());
            this->_M_invalidate_after_nth(__offset);
            return iterator(__res, this);
          } else {
#           if !(_CXX_OLD_CODE)
            return iterator(__first.base()._M_const_cast(), this);
#           else
            return __first;
#           endif
          }
        }

        void swap(vector& __x) noexcept(_Alloc_traits::_S_nothrow_swap()) {
#         if !(_CXX_OLD_CODE) // I might get rid of this check
          if(!_Alloc_traits::_S_propagate_on_swap())
            __glibcxx_check_equal_allocs(__x);
#         endif
          _Base::swap(__x);
          this->_M_swap(__x);
          std::swap(_M_guaranteed_capacity, __x._M_guaranteed_capacity);
        }

        void clear() noexcept {
          _Base::clear();
          this->_M_invalidate_all();
          _M_guaranteed_capacity = 0;
        }
        // End of debug vector element functions


        // Start of debug vector internal functions:
        // Not sure why _M_base is public in debug mode, but this is how it was implemented in libstdc++.
        // I might make these private because I don't see a point in making them public.
        // _M_base() is also not present in the regular implementation of std::vector.
        _Base& _M_base() noexcept { return *this; }

        const _Base& _M_base() const noexcept { return *this; }

      private:
        size_type _M_guaranteed_capacity;

        bool _M_requires_reallocation(size_type __elements) noexcept {
          return __elements > this->capacity();
        }

        void _M_update_guaranteed_capacity() noexcept {
          if(this->size() > _M_guaranteed_capacity)
            _M_guaranteed_capacity = this->size();
        }

        void _M_invalidate_after_nth(difference_type __n) noexcept {
          typedef __gnu_debug::_After_nth_from<_Base_const_iterator> _After_nth;
          this->_M_invalidate_if(_After_nth(__n, _Base::begin()));
        }
        // End of debug vector internal functions
    }; // end of std::__debug::vector class definition


    // Start of debug vector comparison operators:
    template<class _Tp, class _Alloc>
    inline bool operator==(const vector<_Tp, _Alloc>& __lhs, const vector<_Tp, _Alloc>& __rhs) {
      return __lhs._M_base() == __rhs._M_base();
    }

    template<class _Tp, class _Alloc>
    inline bool operator!=(const vector<_Tp, _Alloc>& __lhs, const vector<_Tp, _Alloc>& __rhs) {
      return __lhs._M_base() != __rhs._M_base();
    }

    template<class _Tp, class _Alloc>
    inline bool operator<(const vector<_Tp, _Alloc>& __lhs, const vector<_Tp, _Alloc>& __rhs) {
      return __lhs._M_base() < __rhs._M_base();
    }

    template<class _Tp, class _Alloc>
    inline bool operator<=(const vector<_Tp, _Alloc>& __lhs, const vector<_Tp, _Alloc>& __rhs) {
      return __lhs._M_base() <= __rhs._M_base();
    }

    template<class _Tp, class _Alloc>
    inline bool operator>=(const vector<_Tp, _Alloc>& __lhs, const vector<_Tp, _Alloc>& __rhs) {
      return __lhs._M_base() >= __rhs._M_base();
    }

    template<class _Tp, class _Alloc>
    inline bool operator>(const vector<_Tp, _Alloc>& __lhs, const vector<_Tp, _Alloc>& __rhs) {
      return __lhs._M_base() > __rhs._M_base();
    }
    // End of debug vector comparison operators


    // std::swap() specialization for debug vector:
    template<class _Tp, class _Alloc>
    inline void swap(vector<_Tp, _Alloc>& __lhs, vector<_Tp, _Alloc>& __rhs) {
      __lhs.swap(__rhs);
    }

  } // end of __debug:: namespace


  // Debug mode hash specialization for vector:
  template<class _Alloc>
  struct hash<__debug::vector<bool, _Alloc>>
    : public __detail::__hash_base<size_t, __debug::vector<bool, _Alloc>> {
    size_t operator()(const __debug::vector<bool, _Alloc>& __b) const noexcept {
      return std::hash<_GLIBCXX_STD_C::vector<bool, _Alloc>>()(__b._M_base());
    }
  };

} // end of std:: namespace


namespace __gnu_debug {

  template<class _Tp, class _Alloc>
  struct _Is_contiguous_sequence<std::__debug::vector<_Tp, _Alloc>>
    : public std::__detail::__true_type { };

  template<class _Alloc>
  struct _Is_contiguous_sequence<std::__debug::vector<bool, _Alloc>>
    : public std::__detail::__false_type { };

} // end of __gnu_debug:: namespace

// End of debug mode vector implementation


#endif // End of Debug_Vector.hpp
