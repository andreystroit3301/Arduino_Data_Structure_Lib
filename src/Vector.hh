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
  Vector.hh  [V1.0.0]  (template definition file)
    By: Andrey Stroitelev
  
  ~This file just adds a set of out-of-line definitions for std::vector.
  ~This file is macro enabled and shouldn't be included directly.
  ~This is a slightly modified copy of the vector.tcc file in c++ STL (libstdc++)
  ~Since the arduino IDE doesn't support .tcc files I have to use the .tpp extension instead.
    *The IDE allows .tpp, but text color is all the same unlike the other files which makes me think this extension might not work properly.
    *I'm using .hh for now to get the text style, but I might keep the change permenant.
  
  !WARNING! ~ due to the naming and complexity of certain variables and functions the code in this file is difficult to read.
*/


// Start of vector.hh
#ifndef _DLIB_VECTOR_HH_
#define _DLIB_VECTOR_HH_

// This file should only be included from Vector.hpp. This preprocessor directive will throw an error if this file is included directly anywhere else
#ifndef _DLIB_VECTOR_HPP_
#error __FILE__ should only be included internally by Vector.hpp. Include Vector.hpp or disable template export in config.
#endif

// This file will have its own version independent of the main vector version
#ifndef _DLIB_VECTOR_DEF_VERSION_
#define _DLIB_VECTOR_DEF_VERSION_ 0x010000
#endif


_DLIB_HEADER


// Start of the bits/vector.tcc header implementation from libstdc++:

namespace std _DLIB_VISIBILITY {

  _BEGIN_NAMESPACE_CONTAINER

  template<class _Tp, class _Alloc>
  void vector<_Tp, _Alloc>::reserve(size_type __n) {
    if(__n > this->max_size()) __throw_length_error(__N("vector::reserve"));
    if(this->capacity() < __n) {
      const size_type __old_size = size();
      pointer __tmp = _M_allocate_and_copy(__n,
                                          _GLIBCXX_MAKE_MOVE_IF_NOEXCEPT_ITERATOR(this->_M_impl._M_start),
                                          _GLIBCXX_MAKE_MOVE_IF_NOEXCEPT_ITERATOR(this->_M_impl._M_finish));
      std::__detail::_Destroy(this->_M_impl._M_start, this->_M_impl._M_finish, _M_get_Tp_allocator());
      _M_deallocate(this->_M_impl._M_start,
                    this->_M_impl._M_end_of_storage - this->_M_impl._M_start);
      this->_M_impl._M_start = __tmp;
      this->_M_impl._M_finish = __tmp + __old_size;
      this->_M_impl._M_end_of_storage = this->_M_impl._M_start + __n;
    }
  }


  template<class _Tp, class _Alloc>
  template<class... _Args>
  void vector<_Tp, _Alloc>::emplace_back(_Args&&... __args) {
    if(this->_M_impl._M_finish != this->_M_impl._M_end_of_storage) {
      _Alloc_traits::construct(this->_M_impl, this->_M_impl._M_finish, std::forward<_Args>(__args)...);
      ++this->_M_impl._M_finish;
    } else _M_emplace_back_aux(std::forward<_Args>(__args)...);
  }


  template<class _Tp, class _Alloc>
  typename vector<_Tp, _Alloc>::iterator
  vector<_Tp, _Alloc>::insert(const_iterator __position, const value_type& __x) {
    const size_type __n = __position - begin();
    if(this->_M_impl._M_finish != this->_M_impl._M_end_of_storage && __position == end()) {
      _Alloc_traits::construct(this->_M_impl, this->_M_impl._M_finish, __x);
      ++this->_M_impl._M_finish;
    } else {
      if(this->_M_impl._M_finish != this->_M_impl._M_end_of_storage) {
        _Tp __x_copy = __x;
        _M_insert_aux(__position._M_const_cast(), std::move(__x_copy));
      } else _M_insert_aux(__position._M_const_cast(), __x);
    }
    return iterator(this->_M_impl._M_start + __n);
  }


  template<class _Tp, class _Alloc>
  typename vector<_Tp, _Alloc>::iterator
  vector<_Tp, _Alloc>::_M_erase(iterator __position) {
    if(__position + 1 != end()) _GLIBCXX_MOVE3(__position + 1, end(), __position);
    --this->_M_impl._M_finish;
    _Alloc_traits::destroy(this->_M_impl, this->_M_impl._M_finish);
    return __position;
  }


  template<class _Tp, class _Alloc>
  typename vector<_Tp, _Alloc>::iterator
  vector<_Tp, _Alloc>::_M_erase(iterator __first, iterator __last) {
    if(__first != __last) {
      if(__last != end()) _GLIBCXX_MOVE3(__last, end(), __first);
      _M_erase_at_end(__first.base() + (end() - __last));
    }
    return __first;
  }


  template<class _Tp, class _Alloc>
  vector<_Tp, _Alloc>& vector<_Tp, _Alloc>::operator=(const vector<_Tp, _Alloc>& __x) {
    if(&__x != this) {
#     if !(_CXX_OLD_CODE) 
      // c++11 code start
      if(_Alloc_traits::_S_propagate_on_copy_assign()) {
        if(!_Alloc_traits::_S_always_equal() && _M_get_Tp_allocator() != __x._M_get_Tp_allocator()) {
          this->clear();
          _M_deallocate(this->_M_impl._M_start, this->_M_impl._M_end_of_storage - this->_M_impl._M_start);
          this->_M_impl._M_start = nullptr;
          this->_M_impl._M_finish = nullptr;
          this->_M_impl._M_end_of_storage = nullptr;
        }
        std::__detail::__alloc_on_copy(_M_get_Tp_allocator(), __x._M_get_Tp_allocator());
      }
      // c++11 code end
#     endif
      const size_type __xlen = __x.size();
      if(__xlen > capacity()) {
        pointer __tmp = _M_allocate_and_copy(__xlen, __x.begin(), __x.end());
        std::__detail::_Destroy(this->_M_impl._M_start, this->_M_impl._M_finish, _M_get_Tp_allocator());
        _M_deallocate(this->_M_impl._M_start, this->_M_impl._M_end_of_storage - this->_M_impl._M_start);
        this->_M_impl._M_start = __tmp;
        this->_M_impl._M_end_of_storage = this->_M_impl._M_start + __xlen;
      } else if(size() >= __xlen) {
        std::__detail::_Destroy(std::copy(__x.begin(), __x.end(), begin()), end(), _M_get_Tp_allocator());
      } else {
        std::copy(__x._M_impl._M_start, __x._M_impl._M_start + size(), this->_M_impl._M_start);
        std::__detail::__uninitialized_copy_a(__x._M_impl._M_start + size(), __x._M_impl._M_finish,
                                         this->_M_impl._M_finish, _M_get_Tp_allocator());
      }
      this->_M_impl._M_finish = this->_M_impl._M_start + __xlen;
    }
    return *this;
  } // end of vector assignment operator definition


  template<class _Tp, class _Alloc>
  void vector<_Tp, _Alloc>::_M_fill_assign(size_t __n, const value_type& __val) {
    if(__n > capacity()) {
      vector __tmp(__n, __val, _M_get_Tp_allocator());
      __tmp.swap(*this);
    } else if(__n > size()) {
      std::fill(begin(), end(), __val);
      std::__detail::__uninitialized_fill_n_a(this->_M_impl._M_finish, __n - size(), __val,
                                         _M_get_Tp_allocator());
      this->_M_impl._M_finish += __n - size();
    } else {
      _M_erase_at_end(std::fill_n(this->_M_impl._M_start, __n, __val));
    }
  }


  template<class _Tp, class _Alloc>
  template<class _InputIterator>
  void vector<_Tp, _Alloc>::_M_assign_aux(_InputIterator __first, _InputIterator __last, input_iterator_tag) {
    pointer __cur(this->_M_impl._M_start);
    for(; __first != __last && __cur != this->_M_impl._M_finish; ++__cur, ++__first) *__cur = *__first;
    if(__first == __last) _M_erase_at_end(__cur);
    else insert(end(), __first, __last);
  }


  template<class _Tp, class _Alloc>
  template<class _ForwardIterator>
  void vector<_Tp, _Alloc>::_M_assign_aux(_ForwardIterator __first, _ForwardIterator __last, forward_iterator_tag) {
    const size_type __len = std::distance(__first, __last);
    if(__len > capacity()) {
      pointer __tmp(_M_allocate_and_copy(__len, __first, __last));
      std::__detail::_Destroy(this->_M_impl._M_start, this->_M_impl._M_finish, _M_get_Tp_allocator());
      _M_deallocate(this->_M_impl._M_start, this->_M_impl._M_end_of_storage - this->_M_impl._M_start);
      this->_M_impl._M_start = __tmp;
      this->_M_impl._M_finish = this->_M_impl._M_start + __len;
      this->_M_impl._M_end_of_storage = this->_M_impl._M_finish;
    } else if(size() >= __len) {
      _M_erase_at_end(std::copy(__first, __last, this->_M_impl._M_start));
    } else {
      _ForwardIterator __mid = __first;
      std::advance(__mid, size());
      std::copy(__first, __mid, this->_M_impl._M_start);
      this->_M_impl._M_finish = std::__detail::__uninitialized_copy_a(__mid, __last,
                                                                 this->_M_impl._M_finish,
                                                                 _M_get_Tp_allocator());
    }
  }


//# if !(_CXX_OLD_CODE)
  template<class _Tp, class _Alloc>
  template<class... _Args>
  typename vector<_Tp, _Alloc>::iterator
  vector<_Tp, _Alloc>::emplace(const_iterator __position, _Args&&... __args) {
    const size_type __n = __position - begin();
    if(this->_M_impl._M_finish != this->_M_impl._M_end_of_storage && __position == end()) {
      _Alloc_traits::construct(this->_M_impl, this->_M_impl._M_finish, std::forward<_Args>(__args)...);
      ++this->_M_impl._M_finish;
    } else {
      _M_insert_aux(__position._M_const_cast(), std::forward<_Args>(__args)...);
    }
    return iterator(this->_M_impl._M_start + __n);
  }

# if !(_CXX_OLD_CODE)
  template<class _Tp, class _Alloc>
  template<class... _Args>
  void vector<_Tp, _Alloc>::_M_insert_aux(iterator __position, _Args&&... __args) 
# else
  template<class _Tp, class _Alloc>
  void vector<_Tp, _Alloc>::_M_insert_aux(iterator __position, const _Tp& __x)
# endif
  /*_M_insert_aux:*/ {
    if(this->_M_impl._M_finish != this->_M_impl._M_end_of_storage) {
      _Alloc_traits::construct(this->_M_impl, this->_M_impl._M_finish,
                               _GLIBCXX_MOVE(*(this->_M_impl._M_finish - 1)));
      ++this->_M_impl._M_finish;
#     if (_CXX_OLD_CODE)
      _Tp __x_copy = __x;
#     endif
      _GLIBCXX_MOVE_BACKWARD3(__position.base(),
                              this->_M_impl._M_finish - 2,
                              this->_M_impl._M_finish - 1);
#     if (_CXX_OLD_CODE)
      *__position = __x_copy;
#     else
      *__position = _Tp(std::forward<_Args>(__args)...);
#     endif
    } else {
      const size_type __len = _M_check_len(size_type(1), "vector::_M_insert_aux");
      const size_type __elems_before = __position - begin();
      pointer __new_start(this->_M_allocate(__len));
      pointer __new_finish(__new_start);
      __try {
        _Alloc_traits::construct(this->_M_impl, __new_start + __elems_before,
#                         if !(_CXX_OLD_CODE)
                                 std::forward<_Args>(__args)...);
#                         else
                                 __x);
#                         endif
        __new_finish = 0;
        __new_finish = std::__detail::__uninitialized_move_if_noexcept_a(this->_M_impl._M_start,
                                                                    __position.base(), __new_start,
                                                                    _M_get_Tp_allocator());
        ++__new_finish;
        __new_finish = std::__detail::__uninitialized_move_if_noexcept_a(__position.base(),
                                                                    this->_M_impl._M_finish,
                                                                    __new_finish,
                                                                    _M_get_Tp_allocator());
      } __catch(...) {
        if(!__new_finish) _Alloc_traits::destroy(this->_M_impl, __new_start + __elems_before);
        else std::__detail::_Destroy(__new_start, __new_finish, _M_get_Tp_allocator());
        _M_deallocate(__new_start, __len);
        __throw_exception_again;
      }
      std::__detail::_Destroy(this->_M_impl._M_start, this->_M_impl._M_finish, _M_get_Tp_allocator());
      _M_deallocate(this->_M_impl._M_start, this->_M_impl._M_end_of_storage - this->_M_impl._M_start);
      this->_M_impl._M_start = __new_start;
      this->_M_impl._M_finish = __new_finish;
      this->_M_impl._M_end_of_storage = __new_start + __len;
    }
  }


# if !(_CXX_OLD_CODE)
  template<class _Tp, class _Alloc>
  template<class... _Args>
  void vector<_Tp, _Alloc>::_M_emplace_back_aux(_Args&&... __args) {
    const size_type __len = _M_check_len(size_type(1), "vector::_M_emplace_back_aux");
    pointer __new_start(this->_M_allocate(__len));
    pointer __new_finish(__new_start);
    __try {
      _Alloc_traits::construct(this->_M_impl, __new_start + size(), std::forward<_Args>(__args)...);
      __new_finish = 0;
      __new_finish = std::__detail::__uninitialized_move_if_noexcept_a(this->_M_impl._M_start,
                                                                  this->_M_impl._M_finish,
                                                                  __new_start, _M_get_Tp_allocator());
      ++__new_finish;
    } __catch(...) {
      if(!__new_finish) _Alloc_traits::destroy(this->_M_impl, __new_start + size());
      else std::__detail::_Destroy(__new_start, __new_finish, _M_get_Tp_allocator());
      _M_deallocate(__new_start, __len);
      __throw_exception_again;
    }
    std::__detail::_Destroy(this->_M_impl._M_start, this->_M_impl._M_finish, _M_get_Tp_allocator());
    _M_deallocate(this->_M_impl._M_start, this->_M_impl._M_end_of_storage - this->_M_impl._M_start);
    this->_M_impl._M_start = __new_start;
    this->_M_impl._M_finish = __new_finish;
    this->_M_impl._M_end_of_storage = __new_start + __len;
  }
# endif


  template<class _Tp, class _Alloc>
  void vector<_Tp, _Alloc>::_M_fill_insert(iterator __position, size_type __n, const value_type& __x) {
    if(__n != 0) {
      if(size_type(this->_M_impl._M_end_of_storage - this->_M_impl._M_finish) >= __n) {
        value_type __x_copy = __x;
        const size_type __elems_after = end() - __position;
        pointer __old_finish(this->_M_impl._M_finish);
        if(__elems_after > __n) {
          std::__detail::__uninitialized_move_a(this->_M_impl._M_finish - __n,
                                           this->_M_impl._M_finish,
                                           this->_M_impl._M_finish,
                                           _M_get_Tp_allocator());
          this->_M_impl._M_finish += __n;
          _GLIBCXX_MOVE_BACKWARD3(__position.base(), __old_finish - __n, __old_finish);
          std::fill(__position.base(), __position.base() + __n, __x_copy);
        } else {
          std::__detail::__uninitialized_fill_n_a(this->_M_impl._M_finish,
                                             __n - __elems_after, __x_copy,
                                             _M_get_Tp_allocator());
          this->_M_impl._M_finish += __n - __elems_after;
          std::__detail::__uninitialized_move_a(__position.base(), __old_finish,
                                           this->_M_impl._M_finish, _M_get_Tp_allocator());
          this->_M_impl._M_finish += __elems_after;
          std::fill(__position.base(), __old_finish, __x_copy);
        }
      } else {
        const size_type __len = _M_check_len(__n, "vector::_M_fill_insert");
        const size_type __elems_before = __position - begin();
        pointer __new_start(this->_M_allocate(__len));
        pointer __new_finish(__new_start);
        __try {
          std::__detail::__uninitialized_fill_n_a(__new_start + __elems_before,
                                             __n, __x, _M_get_Tp_allocator());
          __new_finish = 0;
          __new_finish = std::__detail::__uninitialized_move_if_noexcept_a(this->_M_impl._M_start, __position.base(),
                                                                      __new_start, _M_get_Tp_allocator());
          __new_finish += __n;
          __new_finish = std::__detail::__uninitialized_move_if_noexcept_a(__position.base(), this->_M_impl._M_finish,
                                                                      __new_finish, _M_get_Tp_allocator());
        } __catch(...) {
          if(!__new_finish) {
            std::__detail::_Destroy(__new_start + __elems_before,
                               __new_start + __elems_before + __n,
                               _M_get_Tp_allocator());
          } else {
            std::__detail::_Destroy(__new_start, __new_finish, _M_get_Tp_allocator());
          }
          _M_deallocate(__new_start, __len);
          __throw_exception_again;
        }
        std::__detail::_Destroy(this->_M_impl._M_start, this->_M_impl._M_finish, _M_get_Tp_allocator());
        _M_deallocate(this->_M_impl._M_start, this->_M_impl._M_end_of_storage - this->_M_impl._M_start);
        this->_M_impl._M_start = __new_start;
        this->_M_impl._M_finish = __new_finish;
        this->_M_impl._M_end_of_storage = __new_start + __len;
      }
    }
  } // end of _M_fill_insert


  template<class _Tp, class _Alloc>
  void vector<_Tp, _Alloc>::_M_default_append(size_type __n) {
    if(__n != 0) {
      if(size_type(this->_M_impl._M_end_of_storage - this->_M_impl._M_finish) >= __n) {
        std::__detail::__uninitialized_default_n_a(this->_M_impl._M_finish, __n, _M_get_Tp_allocator());
        this->_M_impl._M_finish += __n;
      } else {
        const size_type __len = _M_check_len(__n, "vector::_M_default_append");
        const size_type __old_size = this->size();
        pointer __new_start(this->_M_allocate(__len));
        pointer __new_finish(__new_start);
        __try {
          __new_finish = std::__detail::__uninitialized_move_if_noexcept_a(this->_M_impl._M_start, this->_M_impl._M_finish,
                                                                      __new_start, _M_get_Tp_allocator());
          std::__detail::__uninitialized_default_n_a(__new_finish, __n, _M_get_Tp_allocator());
          __new_finish += __n;
        } __catch(...) {
          std::__detail::_Destroy(__new_start, __new_finish, _M_get_Tp_allocator());
          _M_deallocate(__new_start, __len);
          __throw_exception_again;
        }
        std::__detail::_Destroy(this->_M_impl._M_start, this->_M_impl._M_finish, _M_get_Tp_allocator());
        _M_deallocate(this->_M_impl._M_start, this->_M_impl._M_end_of_storage - this->_M_impl._M_start);
        this->_M_impl._M_start = __new_start;
        this->_M_impl._M_finish = __new_finish;
        this->_M_impl._M_end_of_storage = __new_start + __len;
      }
    }
  }


  template<class _Tp, class _Alloc>
  constexpr bool vector<_Tp, _Alloc>::_M_shrink_to_fit() {
    if(capacity() == size()) return false;
    return std::__detail::__shrink_to_fit_aux<vector>::_S_do_it(*this);
  }


  template<class _Tp, class _Alloc>
  template<class _InputIterator>
  void vector<_Tp, _Alloc>::_M_range_insert(iterator __pos, _InputIterator __first, 
                                            _InputIterator __last, input_iterator_tag) {
    for(; __first != __last; ++__first) {
      __pos = insert(__pos, *__first);
      ++__pos;
    }
  }


  template<class _Tp, class _Alloc>
  template<class _ForwardIterator>
  void vector<_Tp, _Alloc>::_M_range_insert(iterator __position, _ForwardIterator __first, 
                                            _ForwardIterator __last, forward_iterator_tag) {
    if(__first != __last) {
      const size_type __n = std::distance(__first, __last);
      if(size_type(this->_M_impl._M_end_of_storage - this->_M_impl._M_finish) >= __n) {
        const size_type __elems_after = end() - __position;
        pointer __old_finish(this->_M_impl._M_finish);
        if(__elems_after > __n) {
          std::__detail::__uninitialized_move_a(this->_M_impl._M_finish - __n, this->_M_impl._M_finish,
                                           this->_M_impl._M_finish, _M_get_Tp_allocator());
          this->_M_impl._M_finish += __n;
          _GLIBCXX_MOVE_BACKWARD3(__position.base(), __old_finish - __n, __old_finish);
          std::copy(__first, __last, __position);
        } else {
          _ForwardIterator __mid = __first;
          std::advance(__mid, __elems_after);
          std::__detail::__uninitialized_copy_a(__mid, __last, this->_M_impl._M_finish, _M_get_Tp_allocator());
          this->_M_impl._M_finish += __n - __elems_after;
          std::__detail::__uninitialized_move_a(__position.base(), __old_finish, this->_M_impl._M_finish, _M_get_Tp_allocator());
          this->_M_impl._M_finish += __elems_after;
          std::copy(__first, __mid, __position);
        }
      } else {
        const size_type __len = _M_check_len(__n, "vector::_M_range_insert");
        pointer __new_start(this->_M_allocate(__len));
        pointer __new_finish(__new_start);
        __try {
          __new_finish = std::__detail::__uninitialized_move_if_noexcept_a(this->_M_impl._M_start, __position.base(),
                                                                      __new_start, _M_get_Tp_allocator());
          __new_finish = std::__detail::__uninitialized_copy_a(__first, __last, __new_finish, _M_get_Tp_allocator());
          __new_finish = std::__detail::__uninitialized_move_if_noexcept_a(__position.base(), this->_M_impl._M_finish,
                                                                      __new_finish, _M_get_Tp_allocator());
        } __catch(...) {
          std::__detail::_Destroy(__new_start, __new_finish, _M_get_Tp_allocator());
          _M_deallocate(__new_start, __len);
          __throw_exception_again;
        }
        std::__detail::_Destroy(this->_M_impl._M_start, this->_M_impl._M_finish, _M_get_Tp_allocator());
        _M_deallocate(this->_M_impl._M_start, this->_M_impl._M_end_of_storage - this->_M_impl._M_start);
        this->_M_impl._M_start = __new_start;
        this->_M_impl._M_finish = __new_finish;
        this->_M_impl._M_end_of_storage = __new_start + __len;
      }
    }
  }


  // Start of bvector out of line definitions:


  template<class _Alloc>
  void vector<bool, _Alloc>::_M_reallocate(size_type __n) {
    _GLIBCXX_STD_C::__detail::_Bit_type* __q = this->_M_allocate(__n);
    this->_M_impl._M_finish = _M_copy_aligned(begin(), end(), iterator(__q, 0));
    this->_M_deallocate();
    this->_M_impl._M_start = iterator(__q, 0);
    this->_M_impl._M_end_of_storage = __q + _S_nword(__n);
  }


  template<class _Alloc>
  void vector<bool, _Alloc>::_M_fill_insert(iterator __position, size_type __n, bool __x) {
    if(__n == 0) return;
    if(capacity() - size() >= __n) {
      std::copy_backward(__position, end(), this->_M_impl._M_finish + difference_type(__n));
      std::fill(__position, __position + difference_type(__n), __x);
      this->_M_impl._M_finish += difference_type(__n);
    } else {
      const size_type __len = _M_check_len(__n, "vector<bool>::_M_fill_insert");
      _GLIBCXX_STD_C::__detail::_Bit_type* __q = this->_M_allocate(__len);
      iterator __i = _M_copy_aligned(begin(), __position, iterator(__q, 0));
      std::fill(__i, __i + difference_type(__n), __x);
      this->_M_impl._M_finish = std::copy(__position, end(), __i + difference_type(__n));
      this->_M_deallocate();
      this->_M_impl._M_end_of_storage = __q + _S_nword(__len);
      this->_M_impl._M_start = iterator(__q, 0);
    }
  }


  template<class _Alloc>
  template<class _ForwardIterator>
  void vector<bool, _Alloc>::_M_insert_range(iterator __position, _ForwardIterator __first, 
                                             _ForwardIterator __last, forward_iterator_tag) {
    if(__first != __last) {
      size_type __n = std::distance(__first, __last);
      if(capacity() - size() >= __n) {
        std::copy_backward(__position, end(), this->_M_impl._M_finish + difference_type(__n));
        std::copy(__first, __last, __position);
        this->_M_impl._M_finish += difference_type(__n);
      } else {
        const size_type __len = _M_check_len(__n, "vector<bool>::_M_insert_range");
        _GLIBCXX_STD_C::__detail::_Bit_type* __q = this->_M_allocate(__len);
        iterator __i = _M_copy_aligned(begin(), __position, iterator(__q, 0));
        __i = std::copy(__first, __last, __i);
        this->_M_impl._M_finish = std::copy(__position, end(), __i);
        this->_M_deallocate();
        this->_M_impl._M_end_of_storage = __q + _S_nword(__len);
        this->_M_impl._M_start = iterator(__q, 0);
      }
    }
  }


  template<class _Alloc>
  void vector<bool, _Alloc>::_M_insert_aux(iterator __position, bool __x) {
    if(this->_M_impl._M_finish._M_p != this->_M_impl._M_end_of_storage) {
      std::copy_backward(__position, this->_M_impl._M_finish, this->_M_impl._M_finish + 1);
      *__position = __x;
      ++this->_M_impl._M_finish;
    } else {
      const size_type __len = _M_check_len(size_type(1), "vector<bool>::_M_insert_aux");
      _GLIBCXX_STD_C::__detail::_Bit_type* __q = this->_M_allocate(__len);
      iterator __i = _M_copy_aligned(begin(), __position, iterator(__q, 0));
      *__i++ = __x;
      this->_M_impl._M_finish = std::copy(__position, end(), __i);
      this->_M_deallocate();
      this->_M_impl._M_end_of_storage = __q + _S_nword(__len);
      this->_M_impl._M_start = iterator(__q, 0);
    }
  }


  template<class _Alloc>
  typename vector<bool, _Alloc>::iterator
  vector<bool, _Alloc>::_M_erase(iterator __position) {
    if(__position + 1 != end()) std::copy(__position + 1, end(), __position);
    --this->_M_impl._M_finish;
    return __position;
  }


  template<class _Alloc>
  typename vector<bool, _Alloc>::iterator
  vector<bool, _Alloc>::_M_erase(iterator __first, iterator __last) {
    if(__first != __last) _M_erase_at_end(std::copy(__last, end(), __first));
    return __first;
  }


  template<class _Alloc>
  bool vector<bool, _Alloc>::_M_shrink_to_fit() {
    if(capacity() - size() < int(_GLIBCXX_STD_C::__detail::_S_word_bit)) return false;
    __try {
      _M_reallocate(size());
      return true;
    } __catch(...) {
      return false;
    }
  }
   
  _END_NAMESPACE_CONTAINER

} // End of std:: namespace

// End of bits/vector.tcc header implementation


// Seperate namespace for hash implementations:
namespace std _DLIB_VISIBILITY {

  template<class _Alloc>
  size_t hash<_GLIBCXX_STD_C::vector<bool, _Alloc>>::operator()(const _GLIBCXX_STD_C::vector<bool, _Alloc>& __b) const noexcept {
    size_t __hash = 0;
    using _GLIBCXX_STD_C::__detail::_S_word_bit;
    using _GLIBCXX_STD_C::__detail::_Bit_type;
    const size_t __words = __b.size() / _GLIBCXX_STD_C::__detail::_S_word_bit; // might need to add __detail::
    if(__words) {
      const size_t __clength = __words * sizeof(_Bit_type);
      __hash = __detail::_Hash_impl::hash(__b._M_impl._M_start._M_p, __clength);
    }
    const size_t __extrabits = __b.size() % _GLIBCXX_STD_C::__detail::_S_word_bit;
    if(__extrabits) {
      _Bit_type __hiword = *__b._M_impl._M_finish._M_p;
      __hiword &= ~((~static_cast<_Bit_type>(0)) << __extrabits);
      const size_t __clength = (__extrabits + __CHAR_BIT__ - 1) / __CHAR_BIT__;
      if(__words) __hash = __detail::_Hash_impl::hash(&__hiword, __clength, __hash);
      else __hash = __detail::_Hash_impl::hash(&__hiword, __clength);
    }
    return __hash;
  }

} // end of std:: namespace
// End of hash implementations


#endif // End of Vector.hh
