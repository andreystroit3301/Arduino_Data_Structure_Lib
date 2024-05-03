// Libstdc++ and GCC GNU Lincese boilerplate:      (this is required since this code is copyrighted under the GCC compiler. I did not design this myself.)
/*
  Partial Modified Implementation Of Meta-Functions from the <iterator/stl_iterator> Libstdc++ header  -*- C++ -*-

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
  Iterator.hpp  [V1.0.0]  (Iterator base class/concept header file)
    By: Andrey Stroitelev

  ~This file defines the iterator base classes/interfaces. 
  ~I will try to make these interfaces work as close to concepts as I can.
  ~These interfaces for the different iterator types should be inherited by the actual iterator class being used to give it iterator traits.
  ~The interfaces are incomplete classes, so all they do is a set of static assertions to make sure the iterator class being made meets the requirements of it's category.
  ~I am currently using this file for both the bits/stl_iterator header and the main stl_iterator header instead of having them seperate.
    *I may end up splitting these implementations into 2 files if I end up running into #include linker issues.
*/


// Start of Iterator.hpp
#ifndef _DLIB_ITERATOR_HPP_
#define _DLIB_ITERATOR_HPP_

#ifndef _DLIB_ITERATOR_VERSION_
#define _DLIB_ITERATOR_VERSION_ 0x010000 
#endif


#include <Arduino.h>
#include "Config.h"
#include "Type_Traits.hpp"
#include "Move.hpp"
#include "Iterator_Traits.hpp"
#include "Iterator_Funcs.hpp"
#include "RangeAccess.hpp"


// Start of bits/stl_iterator header implementation from libstdc++:
namespace std _DLIB_VISIBILITY {

  // start of std::reverse_iterator implementation:
  /*
    reverse_iterator is an adaptor class for bi-directional and stronger iterators.
    This adaptor is used to iterate from end to start instead of start to end.
    This can be done without this adaptor by just decrementing from std::end()
  */
  template<class _Iterator>
  class reverse_iterator
    : public iterator<typename iterator_traits<_Iterator>::iterator_category,
                      typename iterator_traits<_Iterator>::value_type,
                      typename iterator_traits<_Iterator>::difference_type,
                      typename iterator_traits<_Iterator>::pointer,
                      typename iterator_traits<_Iterator>::reference> {
    protected:
      _Iterator current; // iterator being wrapped

      typedef iterator_traits<_Iterator> __traits_type; // for easier iterator_traits usage
    
    public:
      // public typedefs for iterator related functions and implementations
      typedef _Iterator iterator_type;
      typedef typename __traits_type::difference_type difference_type;
      typedef typename __traits_type::pointer pointer;
      typedef typename __traits_type::reference reference;

      // start of std::reverse_iterator constructors:
      reverse_iterator() : current() { }

      explicit reverse_iterator(iterator_type __x) : current(__x) { }

      reverse_iterator(const reverse_iterator& __x) : current(__x.current) { }

      template<class _Iter>
      reverse_iterator(const reverse_iterator<_Iter>& __x) : current(__x.base()) { }
      // end of std::reverse_iterator constructors

      // this function retrieves the iterator this class is wrapping
      iterator_type base() const { return current; }

      // start of std::reverse_iterator operator definitions:
      reference operator*() const {
        _Iterator __tmp = current;
        return *--__tmp;
      }

      pointer operator->() const {
        return &(operator*());
      }

      reverse_iterator& operator++() {
        --current;
        return *this;
      }

      reverse_iterator operator++(int) {
        reverse_iterator __tmp = *this;
        --current;
        return __tmp;
      }

      reverse_iterator& operator--() {
        ++current;
        return *this;
      }

      reverse_iterator operator--(int) {
        reverse_iterator __tmp = *this;
        ++current;
        return __tmp;
      }

      // the rest of these operators require that the wrapped iterator type is a random access iterator
      // If the wrapped iterator type is not a random access iterator the subsequent error may be difficult to read.
      reverse_iterator operator+(difference_type __n) const {
        return reverse_iterator(current - __n);
      }

      reverse_iterator& operator+=(difference_type __n) {
        current -= __n;
        return *this;
      }

      reverse_iterator operator-(difference_type __n) const {
        return reverse_iterator(current + __n);
      }

      reverse_iterator operator-=(difference_type __n) {
        current += __n;
        return *this;
      }

      reference operator[](difference_type __n) const {
        return *(*this + __n);
      }
      // end of std::reverse_iterator operators
  }; // end of std::reverse_iterator class


  // start of non-member comparison operators for std::reverse_iterator:
  template<class _Iterator>
  inline bool operator==(const reverse_iterator<_Iterator>& __lhs, 
                         const reverse_iterator<_Iterator>& __rhs) {
    return __lhs.base() == __rhs.base();
  }

  template<class _Iterator>
  inline bool operator<(const reverse_iterator<_Iterator>& __lhs, 
                        const reverse_iterator<_Iterator>& __rhs) {
    return __rhs.base() < __lhs.base();
  }

  template<class _Iterator>
  inline bool operator!=(const reverse_iterator<_Iterator>& __lhs, 
                         const reverse_iterator<_Iterator>& __rhs) {
    return !(__lhs == __rhs);
  }

  template<class _Iterator>
  inline bool operator>(const reverse_iterator<_Iterator>& __lhs, 
                        const reverse_iterator<_Iterator>& __rhs) {
    return __rhs < __lhs; // ends up being __lhs.base() < __rhs.base()
  }

  template<class _Iterator>
  inline bool operator<=(const reverse_iterator<_Iterator>& __lhs, 
                         const reverse_iterator<_Iterator>& __rhs) {
    return !(__rhs < __lhs);
  }

  template<class _Iterator>
  inline bool operator>=(const reverse_iterator<_Iterator>& __lhs, 
                         const reverse_iterator<_Iterator>& __rhs) {
    return !(__lhs < __rhs);
  }
  // end of std::reverse_iterator comparison operators


  // start of non-member std::reverse_iterator arithmetic operators:
  template<class _Iterator>
  inline typename reverse_iterator<_Iterator>::difference_type
  operator-(const reverse_iterator<_Iterator>& __lhs, const reverse_iterator<_Iterator>& __rhs) {
    return __rhs.base() - __lhs.base();
  }

  template<class _Iterator>
  inline reverse_iterator<_Iterator>
  operator+(typename reverse_iterator<_Iterator>::difference_type __n,
            const reverse_iterator<_Iterator>& __x) {
    return reverse_iterator<_Iterator>(__x.base() - __n);
  }
  // end of std::reverse_iterator arithmetic operators


  // continued non-member std::reverse_iterator comparison operators for mismatched types:
  template<class _IteratorL, class _IteratorR>
  inline bool operator==(const reverse_iterator<_IteratorL>& __lhs, 
                         const reverse_iterator<_IteratorR>& __rhs) {
    return __lhs.base() == __rhs.base();
  }

  template<class _IteratorL, class _IteratorR>
  inline bool operator<(const reverse_iterator<_IteratorL>& __lhs,
                        const reverse_iterator<_IteratorR>& __rhs) {
    return __rhs.base() < __lhs.base();
  }

  template<class _IteratorL, class _IteratorR>
  inline bool operator!=(const reverse_iterator<_IteratorL>& __lhs,
                         const reverse_iterator<_IteratorR>& __rhs) {
    return !(__lhs == __rhs);
  }

  template<class _IteratorL, class _IteratorR>
  inline bool operator>(const reverse_iterator<_IteratorL>& __lhs,
                        const reverse_iterator<_IteratorR>& __rhs) {
    return __rhs < __lhs;
  }

  template<class _IteratorL, class _IteratorR>
  inline bool operator<=(const reverse_iterator<_IteratorL>& __lhs,
                         const reverse_iterator<_IteratorR>& __rhs) {
    return !(__rhs < __lhs);
  }

  template<class _IteratorL, class _IteratorR>
  inline bool operator>=(const reverse_iterator<_IteratorL>& __lhs,
                         const reverse_iterator<_IteratorR>& __rhs) {
    return !(__lhs < __rhs);
  }
  // end of continued std::reverse_iterator comparison operators


  // std::reverse_iterator arithmetic '-' operator overload for mismatched types
  template<class _IteratorL, class _IteratorR>
  inline auto operator-(const reverse_iterator<_IteratorL>& __lhs,
                        const reverse_iterator<_IteratorR>& __rhs)
  -> decltype(__rhs.base() - __lhs.base()) { return __rhs.base() - __lhs.base(); }
  /*
    alternate signature for the above operator:

    inline typename reverse_iterator<_IteratorL>::difference_type
    operator-(const reverse_iterator<_IteratorL>& __lhs,
              const reverse_iterator<_IteratorR>& __rhs)
  */
  // End of std::reverse_iterator implementation


  // Start of std::back_insert_iterator implementation:
  /*
    This iterator is an output iterator.
    This iterator wraps a container instead of another iterator unlike std::reverse_iterator.
    When assigning a container value type to the iterator the iterator appends that value to the underlying container using push_back.

    There is a helper function std::back_inserter that makes creating these iterators easier.
  */
  template<class _Container>
  class back_insert_iterator
    : public iterator<output_iterator_tag, void, void, void, void> { // only output iterators can have all iterator traits as void except for the category
    protected:
      _Container* container; // pointer to the underlying container that is being iterated through

    public:
      // only provides typedef for container_type
      typedef _Container container_type;

      // the sole constructor for std::back_insert_iterator
      explicit back_insert_iterator(_Container& __x) : container(&__x) { }

      // start of std::back_insert_iterator assignment operators:
#     if !(_CXX_OLD_CODE)
      back_insert_iterator& operator=(typename _Container::const_reference __value) {
        container->push_back(__value);
        return *this;
      }
#     else
      back_insert_iterator& operator=(const typename _Container::value_type& __value) {
        container->push_back(__value);
        return *this;
      }

      back_insert_iterator& operator=(typename _Container::value_type&& __value) {
        container->push_back(std::move(__value));
        return *this;
      }
#     endif
      // end of std::back_insert_iterator assignment operators

      // the rest of the required iterator operators dont do anything for output iterators and just return *this
      // These are required by ALL iterators
      back_insert_iterator& operator*() { return *this; }

      back_insert_iterator& operator++() { return *this; }

      back_insert_iterator operator++(int) { return *this; } 
  }; // end of std::back_insert_iterator class

  // Helper function for creating back insert iterators:
  template<class _Container>
  inline back_insert_iterator<_Container> back_inserter(_Container& __x) {
    return back_insert_iterator<_Container>(__x);
  }
  // End of std::back_insert_iterator implementation


  // Start of std::front_insert_iterator implementation:
  /*
    This iterator is an output iterator just like std::back_insert_iterator.
    This iterator functions like the back insert iterator.
    When assigning values to this iterator is calls push_front instead of push_back for the underlying container.

    Just like for back_insert_iterator there is a helper function for making std::front_insert_iterator's.
    The helper function is std::front_inserter
  */
  template<class _Container>
  class front_insert_iterator
    : public iterator<output_iterator_tag, void, void, void, void> {
    protected:
      _Container* container; // pointer to the underlying container that is being iterated through
    
    public:
      // container_type is the only typedef for output iterators
      typedef _Container container_type;

      // sole constructor for output iterators
      explicit front_insert_iterator(_Container& __x) : container(&__x) { }

#     if !(_CXX_OLD_CODE)
      front_insert_iterator& operator=(typename _Container::const_reference __value) {
        container->push_front(__value);
        return *this;
      }
#     else
      front_insert_iterator& operator=(const typename _Container::value_type& __value) {
        container->push_front(__value);
        return *this;
      }

      front_insert_iterator& operator=(typename _Container::value_type&& __value) {
        container->push_front(std::move(__value));
        return *this;
      }
#     endif

      // defining operators required for all iterator types
      // These will just return *this since they need to be defined, but aren't used.
      front_insert_iterator& operator*() { return *this; }

      front_insert_iterator& operator++() { return *this; }

      front_insert_iterator operator++(int) { return *this; }
  }; // end of std::front_insert_iterator class


  // Helper function for creating front insert iterators for containers:
  template<class _Container>
  inline front_insert_iterator<_Container> front_inserter(_Container& __x) {
    return front_insert_iterator<_Container>(__x);
  }
  // End of std::front_insert_iterator implementation


  // Start of std::insert_iterator implementation:
  /*
    This iterator class is an output iterator.
    When assigning this iterator it inserts the given value to it's underlying container.
    This iterator will insert a value at its position in the container instead of overwriting like most iterators.

    The helper function std::inserter is used to easily create these iterators for a given container.
  */
  template<class _Container>
  class insert_iterator
    : public iterator<output_iterator_tag, void, void, void, void> {
    protected:
      _Container* container; // pointer to underlying container being iterated
      typename _Container::iterator iter; // iterator for the underlying container class that is used to do the iteration

    public:
      // container_type is the only typedef available for output iterators
      typedef _Container container_type;

      // sole constructor for output iterators, but this one also requires the iterator for the passed container
      insert_iterator(_Container& __x, typename _Container::iterator __i)
        : container(&__x), iter(__i) { }

      // start of assignment operators:
#     if !(_CXX_OLD_CODE)
      insert_iterator& operator=(typename _Container::const_reference __value) {
        iter = container->insert(iter, __value);
        ++iter;
        return *this;
      }
#     else
      insert_iterator& operator=(const typename _Container::value_type& __value) {
        iter = container->insert(iter, __value);
        ++iter;
        return *this;
      }

      insert_iterator& operator=(typename _Container::value_type&& __value) {
        iter = container->insert(iter, std::move(__value));
        ++iter;
        return *this;
      }
#     endif

      // Dummy operator definitions to meet requirements:
      insert_iterator& operator*() { return *this; }

      insert_iterator& operator++() { return *this; }

      insert_iterator operator++(int) { return *this; }
  }; // end of std::insert_iterator class

  // Helper function for creating insert iterators for containers:
  template<class _Container, class _Iterator>
  inline insert_iterator<_Container> inserter(_Container& __x, _Iterator __i) {
    return insert_iterator<_Container>(__x, typename _Container::iterator(__i));
  }
  // End of std::insert_iterator implementation

} // end of std:: namespace


// Start of language extending implementations from bits/stl_iterator
namespace __gnu_cxx _DLIB_VISIBILITY {

  // not using these "using" statements since I keep having issues cause by these expressions
  //using std::iterator_traits;
  //using std::iterator;

  template<class _Iterator, class _Container>
  class __normal_iterator {
    protected:
      _Iterator _M_current;

      typedef std::iterator_traits<_Iterator> __traits_type;

    public:
      typedef _Iterator iterator_type;
      typedef typename __traits_type::iterator_category iterator_category;
      typedef typename __traits_type::value_type value_type;
      typedef typename __traits_type::difference_type difference_type;
      typedef typename __traits_type::reference reference;
      typedef typename __traits_type::pointer pointer;

      GLIBCXX_CONSTEXPR __normal_iterator() noexcept : _M_current(_Iterator()) { }

      explicit __normal_iterator(const _Iterator& __i) noexcept : _M_current(__i) { }

      template<class _Iter>
      __normal_iterator(const __normal_iterator<_Iter,
                        typename __enable_if<(std::__detail::__are_same<_Iter, typename _Container::pointer>::__value),
                                              _Container>::__type>& __i) noexcept : _M_current(__i.base()) { }

#     if !(_CXX_OLD_CODE)
      __normal_iterator<typename _Container::pointer, _Container> _M_const_cast() const noexcept {
        using _PTraits = std::pointer_traits<typename _Container::pointer>;
        return __normal_iterator<typename _Container::pointer, _Container>(_PTraits::pointer_to(const_cast<typename _PTraits::element_type&>(*_M_current)));
      }
#     else
      __normal_iterator _M_const_cast() const { return *this; }
#     endif

      reference operator*() const noexcept {
        return *_M_current;
      }

      pointer operator->() const noexcept {
        return _M_current;
      }

      __normal_iterator& operator++() noexcept {
        ++_M_current;
        return *this;
      }

      __normal_iterator operator++(int) noexcept {
        return __normal_iterator(_M_current++);
      }

      __normal_iterator& operator--() noexcept {
        --_M_current;
        return *this;
      }

      __normal_iterator operator--(int) noexcept {
        return __normal_iterator(_M_current--);
      }

      reference operator[](difference_type __n) const noexcept {
        return _M_current[__n];
      }

      __normal_iterator& operator+=(difference_type __n) noexcept {
        _M_current += __n;
        return *this;
      }

      __normal_iterator operator+(difference_type __n) const noexcept {
        return __normal_iterator(_M_current + __n);
      }

      __normal_iterator& operator-=(difference_type __n) noexcept {
        _M_current -= __n;
        return *this;
      }

      __normal_iterator operator-(difference_type __n) const noexcept {
        return __normal_iterator(_M_current - __n);
      }

      const _Iterator& base() const noexcept {
        return _M_current;
      }
  }; // end of __normal_iterator class

  template<class _IteratorL, class _IteratorR, class _Container>
  inline bool operator==
  (const __normal_iterator<_IteratorL, _Container>& __lhs,
   const __normal_iterator<_IteratorR, _Container>& __rhs) noexcept {
    return __lhs.base() == __rhs.base();
  }

  template<class _Iterator, class _Container>
  inline bool operator==
  (const __normal_iterator<_Iterator, _Container>& __lhs,
   const __normal_iterator<_Iterator, _Container>& __rhs) noexcept {
    return __lhs.base() == __rhs.base();
  }

  template<class _IteratorL, class _IteratorR, class _Container>
  inline bool operator!=
  (const __normal_iterator<_IteratorL, _Container>& __lhs,
   const __normal_iterator<_IteratorR, _Container>& __rhs) noexcept {
    return __lhs.base() != __rhs.base();
  }

  template<class _Iterator, class _Container>
  inline bool operator!=
  (const __normal_iterator<_Iterator, _Container>& __lhs,
   const __normal_iterator<_Iterator, _Container>& __rhs) noexcept {
    return __lhs.base() != __rhs.base();
  }

  template<class _IteratorL, class _IteratorR, class _Container>
  inline bool operator<
  (const __normal_iterator<_IteratorL, _Container>& __lhs,
   const __normal_iterator<_IteratorR, _Container>& __rhs) noexcept {
    return __lhs.base() < __rhs.base();
  }

  template<class _Iterator, class _Container>
  inline bool operator<
  (const __normal_iterator<_Iterator, _Container>& __lhs, 
   const __normal_iterator<_Iterator, _Container>& __rhs) noexcept {
    return __lhs.base() < __rhs.base();
  }

  template<class _IteratorL, class _IteratorR, class _Container>
  inline bool operator>
  (const __normal_iterator<_IteratorL, _Container>& __lhs,
   const __normal_iterator<_IteratorR, _Container>& __rhs) noexcept {
    return __lhs.base() > __rhs.base();
  }

  template<class _Iterator, class _Container>
  inline bool operator>
  (const __normal_iterator<_Iterator, _Container>& __lhs,
   const __normal_iterator<_Iterator, _Container>& __rhs) noexcept {
    return __lhs.base() > __rhs.base();
  }

  template<class _IteratorL, class _IteratorR, class _Container>
  inline bool operator<=
  (const __normal_iterator<_IteratorL, _Container>& __lhs,
   const __normal_iterator<_IteratorR, _Container>& __rhs) noexcept {
    return __lhs.base() <= __rhs.base();
  }

  template<class _Iterator, class _Container>
  inline bool operator<=
  (const __normal_iterator<_Iterator, _Container>& __lhs,
   const __normal_iterator<_Iterator, _Container>& __rhs) noexcept {
    return __lhs.base() <= __rhs.base();
  }

  template<class _IteratorL, class _IteratorR, class _Container>
  inline bool operator>=
  (const __normal_iterator<_IteratorL, _Container>& __lhs,
   const __normal_iterator<_IteratorR, _Container>& __rhs) noexcept {
    return __lhs.base() >= __rhs.base();
  }

  template<class _Iterator, class _Container>
  inline bool operator>=
  (const __normal_iterator<_Iterator, _Container>& __lhs,
   const __normal_iterator<_Iterator, _Container>& __rhs) noexcept {
    return __lhs.base() >= __rhs.base();
  }

  template<class _IteratorL, class _IteratorR, class _Container>
  inline auto operator-(const __normal_iterator<_IteratorL, _Container>& __lhs,
                        const __normal_iterator<_IteratorR, _Container>& __rhs)
  noexcept -> decltype(__lhs.base() - __rhs.base()) {
    return __lhs.base() - __rhs.base();
  }

  template<class _Iterator, class _Container>
  inline typename __normal_iterator<_Iterator, _Container>::difference_type
  operator-(const __normal_iterator<_Iterator, _Container>& __lhs,
            const __normal_iterator<_Iterator, _Container>& __rhs) noexcept {
    return __lhs.base() - __rhs.base();
  }

  template<class _Iterator, class _Container>
  inline __normal_iterator<_Iterator, _Container>
  operator+(typename __normal_iterator<_Iterator, _Container>::difference_type __n,
            const __normal_iterator<_Iterator, _Container>& __i) noexcept {
    return __normal_iterator<_Iterator, _Container>(__i.base() + __n);
  }

} // end of __gnu_cxx:: namespace


// Continuation of main std:: namespace implementations:
namespace std _DLIB_VISIBILITY {

  // Start of std::move_iterator implementation:
  /*
    This class is an iterator adaptor that assumes the same behavior as its underlying/wrapped iterator.
    move_iterator works the same except it's dereference operator returns an rvalue reference.
    This iterator is used when replacing copying with moving in applicable algorithms.
    The dereference operator implicitly converts the dereference value of the underlying iterator to an rvalue.
  */
  template<class _Iterator>
  class move_iterator {
    protected:
      _Iterator _M_current; // underlying iterator

      typedef iterator_traits<_Iterator> __traits_type; // iterator_traits helper
    
    public:
      // defining iterator traits typedefs
      typedef _Iterator iterator_type;
      typedef typename __traits_type::iterator_category iterator_category;
      typedef typename __traits_type::value_type value_type;
      typedef typename __traits_type::difference_type difference_type;
      typedef _Iterator pointer;
      typedef value_type&& reference;

      // start of std::move_iterator constructors
      move_iterator() : _M_current() { }

      explicit move_iterator(iterator_type __i) : _M_current(__i) { }

      template<class _Iter>
      move_iterator(const move_iterator<_Iter>& __i) : _M_current(__i.base()) { }
      // end of std::move_iterator constructors

      // base() function to retrieve underlying iterator
      iterator_type base() const {
        return _M_current;
      }

      // start of std::move_iterator member access operators:
      reference operator*() const {
        return std::move(*_M_current);
      }

      pointer operator->() const {
        return _M_current;
      }
      // end of std::move_iterator member access operators

      // start of std::move_iterator increment and decrement operators:
      move_iterator& operator++() {
        ++_M_current;
        return *this;
      }

      move_iterator operator++(int) {
        move_iterator __tmp = *this;
        ++_M_current;
        return __tmp;
      }

      move_iterator& operator--() {
        --_M_current;
        return *this;
      }

      move_iterator operator--(int) {
        move_iterator __tmp = *this;
        --_M_current;
        return __tmp;
      }
      // end of std::move_iterator increment and decrement

      // start of std::move_iterator increment/decrement by 'n' operators:
      move_iterator operator+(difference_type __n) const {
        return move_iterator(_M_current + __n);
      }

      move_iterator& operator+=(difference_type __n) {
        _M_current += __n;
        return *this;
      }

      move_iterator operator-(difference_type __n) const {
        return move_iterator(_M_current - __n);
      }

      move_iterator& operator-=(difference_type __n) {
        _M_current -= __n;
        return *this;
      }
      // end of std::move_iterator increment/decrement by 'n' operators

      // std::move_iterator subscript operator for random access iterators
      reference operator[](difference_type __n) const {
        return std::move(_M_current[__n]);
      }
  }; // end of std::move_iterator class

  
  // Start of std::move_iterator non-member comparison operators:
  template<class _IteratorL, class _IteratorR>
  inline bool operator==
  (const move_iterator<_IteratorL>& __lhs, const move_iterator<_IteratorR>& __rhs) {
    return __lhs.base() == __rhs.base();
  }

  template<class _Iterator>
  inline bool operator==
  (const move_iterator<_Iterator>& __lhs, const move_iterator<_Iterator>& __rhs) {
    return __lhs.base() == __rhs.base();
  }

  template<class _IteratorL, class _IteratorR>
  inline bool operator!=
  (const move_iterator<_IteratorL>& __lhs, const move_iterator<_IteratorR>& __rhs) {
    return !(__lhs == __rhs);
  }

  template<class _Iterator>
  inline bool operator!=
  (const move_iterator<_Iterator>& __lhs, const move_iterator<_Iterator>& __rhs) {
    return !(__lhs == __rhs);
  }

  template<class _IteratorL, class _IteratorR>
  inline bool operator<
  (const move_iterator<_IteratorL>& __lhs, const move_iterator<_IteratorR>& __rhs) {
    return __lhs.base() < __rhs.base();
  }

  template<class _Iterator>
  inline bool operator<
  (const move_iterator<_Iterator>& __lhs, const move_iterator<_Iterator>& __rhs) {
    return __lhs.base() < __rhs.base();
  }

  template<class _IteratorL, class _IteratorR>
  inline bool operator<=
  (const move_iterator<_IteratorL>& __lhs, const move_iterator<_IteratorR>& __rhs) {
    return !(__rhs < __lhs);
  }

  template<class _Iterator>
  inline bool operator<=
  (const move_iterator<_Iterator>& __lhs, const move_iterator<_Iterator>& __rhs) {
    return !(__rhs < __lhs);
  }

  template<class _IteratorL, class _IteratorR>
  inline bool operator>
  (const move_iterator<_IteratorL>& __lhs, const move_iterator<_IteratorR>& __rhs) {
    return __rhs < __lhs;
  }

  template<class _Iterator>
  inline bool operator>
  (const move_iterator<_Iterator>& __lhs, const move_iterator<_Iterator>& __rhs) {
    return __rhs < __lhs;
  }

  template<class _IteratorL, class _IteratorR>
  inline bool operator>=
  (const move_iterator<_IteratorL>& __lhs, const move_iterator<_IteratorR>& __rhs) {
    return !(__lhs < __rhs);
  }

  template<class _Iterator>
  inline bool operator>=
  (const move_iterator<_Iterator>& __lhs, const move_iterator<_Iterator>& __rhs) {
    return !(__lhs < __rhs);
  }
  // End of std::move_iterator comparison operators


  // start of std::move_iterator non-member arithmetic operators:
  template<class _IteratorL, class _IteratorR>
  inline auto operator-(const move_iterator<_IteratorL>& __lhs,
                        const move_iterator<_IteratorR>& __rhs)
  -> decltype(__lhs.base() - __rhs.base()) {
    return __lhs.base() - __rhs.base();
  }

  template<class _Iterator>
  inline auto operator-(const move_iterator<_Iterator>& __lhs,
                        const move_iterator<_Iterator>& __rhs)
  -> decltype(__lhs.base() - __rhs.base()) {
    return __lhs.base() - __rhs.base();
  }

  template<class _Iterator>
  inline move_iterator<_Iterator> operator+
  (typename move_iterator<_Iterator>::difference_type __n, 
   const move_iterator<_Iterator>& __x) { return __x + __n; }
  // end of std::move_iterator non-member arithmetic operators


  // helper function for creating move iterators:
  template<class _Iterator>
  inline move_iterator<_Iterator> make_move_iterator(_Iterator __i) {
    return move_iterator<_Iterator>(__i);
  }


  namespace __detail { // namespace for internal implementation

    // Internal helper function for making non-throwing move iterators:
    template<class _Iterator,
             class _ReturnType = typename conditional<
                                            __move_if_noexcept_cond<typename iterator_traits<_Iterator>::value_type>::value,
                                            _Iterator, move_iterator<_Iterator>
                                          >::type>
    inline _ReturnType __make_move_if_noexcept_iterator(_Iterator __i) {
      return _ReturnType(__i);
    }

  } // end of __detail:: namespace
  // End of std::move_iterator implementation

} // end of std:: namespace


#define _GLIBCXX_MAKE_MOVE_ITERATOR(_Iter) std::make_move_iterator(_Iter)
#define _GLIBCXX_MAKE_MOVE_IF_NOEXCEPT_ITERATOR(_Iter) std::__detail::__make_move_if_noexcept_iterator(_Iter)
// End of bits/stl_iterator header implementation from libstdc++


// Start of stl_iterator header implementation from libstdc++:
// End of stl_iterator header implementation from libstdc++


#endif // end of Iterator.hpp
