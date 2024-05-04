// Libstdc++ and GCC GNU Lincese boilerplate:      (this is required since this code is copyrighted under the GCC compiler. I did not design this myself.)
/*
  Partial Modified Implementation Of Meta-Functions from the <bits/stl_iterator_base_funcs> Libstdc++ header  -*- C++ -*-

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
  Iterator_Funcs.hpp  [V1.0.0]  (template implementations for iterator functions)
    By: Andrey Stroitelev
  
  ~This file is to implement the functions defined in the bits/stl_iterator_base_funcs header in libstdc++
  ~This file defines a handful of helpful functions for iterators
*/


// Start of Iterator_Funcs.hpp
#ifndef _ITERATOR_FUNCS_HPP_
#define _ITERATOR_FUNCS_HPP_

#ifndef _ITERATOR_FUNCS_VERSION_
#define _ITERATOR_FUNCS_VERSION_ 0x010000
#endif


#include <Arduino.h>
#include "Config.h"
#include "Concept_Check.hpp"
#include "Debug.hpp"


_DLIB_HEADER


// Start of iterator base function implementations:
namespace std _DLIB_VISIBILITY {

  // Start of std::distance implementation:
  namespace __detail { // namespace for internal implementations

    template<class _InputIterator>
    inline typename iterator_traits<_InputIterator>::difference_type
    __distance(_InputIterator __first, _InputIterator __last, input_iterator_tag) {
      __glibcxx_function_requires(_InputIteratorConcept<_InputIterator>)
      typename iterator_traits<_InputIterator>::difference_type __n = 0;
      while(__first != __last) {
        ++__first;
        ++__n;
      }
      return __n;
    }

    template<class _RandomAccessIterator>
    inline typename iterator_traits<_RandomAccessIterator>::difference_type
    __distance(_RandomAccessIterator __first, _RandomAccessIterator __last, random_access_iterator_tag) {
      __glibcxx_function_requires(_RandomAccessIteratorConcept<_RandomAccessIterator>)
      return __last - __first;
    }

  } // end of __detail:: namespace

  template<class _InputIterator>
  inline typename iterator_traits<_InputIterator>::difference_type
  distance(_InputIterator __first, _InputIterator __last) {
    return __detail::__distance(__first, __last, __detail::__iterator_category(__first));
  }
  // End of std::distance


  // Start of std::advance implementation:
  namespace __detail { // namespace for internal implementations

    template<class _InputIterator, class _Distance>
    inline void __advance(_InputIterator& __i, _Distance __n, input_iterator_tag) {
      __glibcxx_function_requires(_InputIteratorConcept<_InputIterator>)
      _GLIBCXX_DEBUG_ASSERT(__n >= 0);
      while(__n--) ++__i;
    }

    template<class _BidirectionalIterator, class _Distance>
    inline void __advance(_BidirectionalIterator& __i, _Distance __n, bidirectional_iterator_tag) {
      __glibcxx_function_requires(_BidirectionalIteratorConcept<_BidirectionalIterator>)
      if(__n > 0) while(__n--) ++__i;
      else while(__n++) --__i;
    }

    template<class _RandomAccessIterator, class _Distance>
    inline void __advance(_RandomAccessIterator& __i, _Distance __n, random_access_iterator_tag) {
      __glibcxx_function_requires(_RandomAccessIteratorConcept<_RandomAccessIterator>)
      __i += __n;
    }

  } // end of __detail:: namespace

  template<class _InputIterator, class _Distance>
  inline void advance(_InputIterator& __i, _Distance __n) {
    typename iterator_traits<_InputIterator>::difference_type __d = __n;
    __detail::__advance(__i, __d, __detail::__iterator_category(__i));
  }
  // End of std::advance


  // Start of std::next and std::prev implementations:
  template<class _ForwardIterator>
  inline _ForwardIterator next(_ForwardIterator __x,
                               typename iterator_traits<_ForwardIterator>::difference_type __n = 1) {
    advance(__x, __n);
    return __x;
  }

  template<class _BidirectionalIterator>
  inline _BidirectionalIterator prev(_BidirectionalIterator __x,
                                     typename iterator_traits<_BidirectionalIterator>::difference_type __n = 1) {
    advance(__x, -__n);
    return __x;
  }
  // End of std::next and std::prev

} // end of std:: namespace


#endif // end of Iterator_Funcs.hpp
