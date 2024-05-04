// Libstdc++ and GCC GNU Lincese boilerplate:      (this is required since this code is copyrighted under the GCC compiler. I did not design this myself.)
/*
  Partial Modified Implementation Of Meta-Functions from the <bits/stl_algobase> Libstdc++ header  -*- C++ -*-

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
  AlgoBase.hpp  [V1.0.0]  (Base methods and utilities for algorithms header file)
    By: Andrey Stroitelev
  
  ~This file defines/declares all of the base utilities for algorithms.
  ~This file is pretty much an exact copy of bits/stl_algobase.h in libstdc++ (c++14 gcc-4.9.0)
  ~This file is used for more than just algorithms as it's an essential part to a bunch of container classes.
  ~This file most likely wont have everything from bits/stl_algobase.h.
    *And instead of making more files I'm going to implement files that stl_algobase is dependent on in this file together with the stl_algobase implementation
      >I wont implement all of it's dependancies, only the ones required to implement the stuff I'll be using from the original STL source file.
*/


// Start of AlgoBase.hpp
#ifndef _ALGORITHM_BASE_HPP_
#define _ALGORITHM_BASE_HPP_

#ifndef _ALGORITHM_BASE_VERSION_
#define _ALGORITHM_BASE_VERSION_ 0x010000
#endif


#include <Arduino.h>
#include "Config.h"
#include "Type_Traits.hpp"
#include "Pair.hpp"
#include "Iterator_Traits.hpp"
#include "Iterator_Funcs.hpp"
#include "Iterator.hpp"
#include "Concept_Check.hpp"
#include "Debug.hpp"
#include "Move.hpp"
#include "UtilityBase.hpp"


_DLIB_HEADER


// Start base algorithm implementations:
namespace std _DLIB_VISIBILITY {

  // Start of std::iter_swap implementation:
  namespace __detail { // namespace for internal implementation

    template<bool _BoolType>
    struct __iter_swap {
      template<class _ForwardIterator1, class _ForwardIterator2>
      static void iter_swap(_ForwardIterator1 __a, _ForwardIterator2 __b) {
        typedef typename iterator_traits<_ForwardIterator1>::value_type _ValueType1;
        _ValueType1 __tmp = std::move(*__a);
        *__a = std::move(*__b);
        *__b = std::move(__tmp);
      }
    };

    template<>
    struct __iter_swap<true> {
      template<class _ForwardIterator1, class _ForwardIterator2>
      static void iter_swap(_ForwardIterator1 __a, _ForwardIterator2 __b) {
        std::swap(*__a, *__b);
      }
    };

  } // end of __detail:: namespace

  template<class _ForwardIterator1, class _ForwardIterator2>
  inline void iter_swap(_ForwardIterator1 __a, _ForwardIterator2 __b) {
    __glibcxx_function_requires(_Mutable_ForwardIteratorConcept<_ForwardIterator1>)
    __glibcxx_function_requires(_Mutable_ForwardIteratorConcept<_ForwardIterator2>)

    typedef typename iterator_traits<_ForwardIterator1>::value_type _ValueType1;
    typedef typename iterator_traits<_ForwardIterator2>::value_type _ValueType2;

    __glibcxx_function_requires(_ConvertibleConcept<_ValueType1, _ValueType2>)
    __glibcxx_function_requires(_ConvertibleConcept<_ValueType2, _ValueType1>)

    typedef typename iterator_traits<_ForwardIterator1>::reference _ReferenceType1;
    typedef typename iterator_traits<_ForwardIterator2>::reference _ReferenceType2;

    __detail::__iter_swap<__detail::__are_same<_ValueType1, _ValueType2>::__value
                       && __detail::__are_same<_ValueType1&, _ReferenceType1>::__value
                       && __detail::__are_same<_ValueType2&, _ReferenceType2>::__value>::iter_swap(__a, __b);
  }
  // End of std::iter_swap 


  // Start of std::swap_ranges implementation:
  template<class _ForwardIterator1, class _ForwardIterator2>
  _ForwardIterator2 swap_ranges(_ForwardIterator1 __first1, _ForwardIterator1 __last1, _ForwardIterator2 __first2) {
    __glibcxx_function_requires(_Mutable_ForwardIteratorConcept<_ForwardIterator1>)
    __glibcxx_function_requires(_Mutable_ForwardIteratorConcept<_ForwardIterator2>)
    __glibcxx_requires_valid_range(__first1, __last1);

    for(; __first1 != __last1; ++__first1, ++__first2) std::iter_swap(__first1, __first2);
    return __first2;
  }
  // End of std::swap_ranges


  // Start of std::min and std::max implementation:
  // I originally had this commented out, but now it is set to be compiled if the min/max macros are not defined or undefined.
  // If the user wants to use these functions instead of the built-in arduino macros for min/max then they would need to undefine the min/max macros before including this library.
  // The #undef directive must be compiled before this file for these functions to be usable.
# if !defined(min) && !defined(max)
  template<class _Tp>
  inline const _Tp& min(const _Tp& __a, const _Tp& __b) {
    __glibcxx_function_requires(_LessThanComparableConcept<_Tp>)
    return ((__b < __a) ? __b : __a);
  }
  
  template<class _Tp>
  inline const _Tp& max(const _Tp& __a, const _Tp& __b) {
    __glibcxx_function_requires(_LessThanComparableConcept<_Tp>)
    return ((__a < __b) ? __b : __a);
  }

  template<class _Tp, class _Compare>
  inline const _Tp& min(const _Tp& __a, const _Tp& __b, _Compare __comp) {
    return __comp(__b, __a) ? __b : __a;
  }

  template<class _Tp, class _Compare>
  inline const _Tp& max(const _Tp& __a, const _Tp& __b, _Compare __comp) {
    return __comp(__a, __b) ? __b : __a;
  }
# endif
  // End of std::min and std::max


  // Start of internal iterator related implementations:
  namespace __detail { // namespace for internal implementations

    template<class _Iterator>
    struct _Niter_base
      : public _Iter_base<_Iterator, __is_normal_iterator<_Iterator>::__value> { };

    template<class _Iterator>
    inline typename _Niter_base<_Iterator>::iterator_type
    __niter_base(_Iterator __it) {
      return _Niter_base<_Iterator>::_S_base(__it);
    }

    template<class _Iterator>
    struct _Miter_base
      : public _Iter_base<_Iterator, __is_move_iterator<_Iterator>::__value> { };

    template<class _Iterator>
    inline typename _Miter_base<_Iterator>::iterator_type
    __miter_base(_Iterator __it) {
      return _Miter_base<_Iterator>::_S_base(__it);
    }

    template<bool, bool, class>
    struct __copy_move {
      template<class _II, class _OI>
      static _OI __copy_m(_II __first, _II __last, _OI __result) {
        for(; __first != __last; ++__result, ++__first) *__result = *__first;
        return __result;
      }
    };

    template<class _Category>
    struct __copy_move<true, false, _Category> {
      template<class _II, class _OI>
      static _OI __copy_m(_II __first, _II __last, _OI __result) {
        for(; __first != __last; ++__result, ++__first) *__result = std::move(*__first);
        return __result;
      }
    };

    template<>
    struct __copy_move<false, false, random_access_iterator_tag> {
      template<class _II, class _OI>
      static _OI __copy_m(_II __first, _II __last, _OI __result) {
        typedef typename iterator_traits<_II>::difference_type _Distance;
        for(_Distance __n = __last - __first; __n > 0; --__n) {
          *__result = *__first;
          ++__first;
          ++__result;
        }
        return __result;
      }
    };

    template<>
    struct __copy_move<true, false, random_access_iterator_tag> {
      template<class _II, class _OI>
      static _OI __copy_m(_II __first, _II __last, _OI __result) {
        typedef typename iterator_traits<_II>::difference_type _Distance;
        for(_Distance __n = __last - __first; __n > 0; --__n) {
          *__result = std::move(*__first);
          ++__first;
          ++__result;
        }
        return __result;
      }
    };

    template<bool _IsMove>
    struct __copy_move<_IsMove, true, random_access_iterator_tag> {
      template<class _Tp>
      static _Tp* __copy_m(const _Tp* __first, const _Tp* __last, _Tp* __result) {
        static_assert(is_copy_assignable<_Tp>::value, "the given type is not assignable");

        const ptrdiff_t _Num = __last - __first;
        if(_Num) __builtin_memmove(__result, __first, sizeof(_Tp) * _Num);
        return __result + _Num;
      }
    };

    template<bool _IsMove, class _II, class _OI>
    inline _OI __copy_move_a(_II __first, _II __last, _OI __result) {
      typedef typename iterator_traits<_II>::value_type _ValueTypeI;
      typedef typename iterator_traits<_OI>::value_type _ValueTypeO;
      typedef typename iterator_traits<_II>::iterator_category _Category;
      const bool __simple = (__is_trivial(_ValueTypeI)
                          && __is_pointer<_II>::__value
                          && __is_pointer<_OI>::__value
                          && __are_same<_ValueTypeI, _ValueTypeO>::__value);
      return __copy_move<_IsMove, __simple, _Category>::__copy_m(__first, __last, __result);
    }

    template<class _CharT>
    struct char_traits;

    template<class _CharT, class _Traits>
    class istreambuf_iterator;

    template<class _CharT, class _Traits>
    class ostreambuf_iterator;

    template<bool _IsMove, class _CharT>
    typename __gnu_cxx::__enable_if<__is_char<_CharT>::__value,
             ostreambuf_iterator<_CharT, char_traits<_CharT>>>::__type
    __copy_move_a2(_CharT*, _CharT*, ostreambuf_iterator<_CharT, char_traits<_CharT>>);

    template<bool _IsMove, class _CharT>
    typename __gnu_cxx::__enable_if<__is_char<_CharT>::__value,
                                    ostreambuf_iterator<_CharT, char_traits<_CharT>>>::__type
    __copy_move_a2(const _CharT*, const _CharT*, ostreambuf_iterator<_CharT, char_traits<_CharT>>);

    template<bool _IsMove, class _CharT>
    typename __gnu_cxx::__enable_if<__is_char<_CharT>::__value, _CharT*>::__type
    __copy_move_a2(istreambuf_iterator<_CharT, char_traits<_CharT>>,
                   istreambuf_iterator<_CharT, char_traits<_CharT>>, _CharT*);

    template<bool _IsMove, class _II, class _OI>
    inline _OI __copy_move_a2(_II __first, _II __last, _OI __result) {
      return _OI(__copy_move_a<_IsMove>(__niter_base(__first),
                                        __niter_base(__last),
                                        __niter_base(__result)));
    }

  } // end of __detail:: namespace
  // End of internal iterator related implementations


  // Start of std::copy & std::move implementations:
  template<class _II, class _OI> // might never use in which case I will delete this
  inline _OI copy(_II __first, _II __last, _OI __result) {
    __glibcxx_function_requires(_InputIteratorConcept<_II>)
    __glibcxx_function_requires(_OutputIteratorConcept<_OI, typename iterator_traits<_II>::value_type>)
    __glibcxx_requires_valid_range(__first, __last);
    return (__detail::__copy_move_a2<__detail::__is_move_iterator<_II>::__value>
                                    (__detail::__miter_base(__first), 
                                     __detail::__miter_base(__last), __result));
  }

  template<class _II, class _OI>
  inline _OI move(_II __first, _II __last, _OI __result) {
    __glibcxx_function_requires(_InputIteratorConcept<_II>)
    __glibcxx_function_requires(_OutputIteratorConcept<_OI, typename iterator_traits<_II>::value_type>)
    __glibcxx_requires_valid_range(__first, __last);
    return __detail::__copy_move_a2<true>(__detail::__miter_base(__first),
                                          __detail::__miter_base(__last), __result);
  }

# define _GLIBCXX_MOVE3(_Tp, _Up, _Vp) std::move(_Tp, _Up, _Vp)
  // End of std::copy & std::move implementations


  // Start of internal copy/move backwards internal implementations:
  namespace __detail { // namespace for internal implementations

    template<bool, bool, class>
    struct __copy_move_backward {
      template<class _BI1, class _BI2>
      static _BI2 __copy_move_b(_BI1 __first, _BI1 __last, _BI2 __result) {
        while(__first != __last) *--__result = *--__last;
        return __result;
      }
    };

    template<class _Category>
    struct __copy_move_backward<true, false, _Category> {
      template<class _BI1, class _BI2>
      static _BI2 __copy_move_b(_BI1 __first, _BI1 __last, _BI2 __result) {
        while(__first != __last) *--__result = std::move(*--__last);
        return __result;
      }
    };

    template<>
    struct __copy_move_backward<false, false, random_access_iterator_tag> {
      template<class _BI1, class _BI2>
      static _BI2 __copy_move_b(_BI1 __first, _BI1 __last, _BI2 __result) {
        typename iterator_traits<_BI1>::difference_type __n;
        for(__n = __last - __first; __n > 0; --__n) *--__result = *--__last;
        return __result;
      }
    };

    template<>
    struct __copy_move_backward<true, false, random_access_iterator_tag> {
      template<class _BI1, class _BI2>
      static _BI2 __copy_move_b(_BI1 __first, _BI1 __last, _BI2 __result) {
        typename iterator_traits<_BI1>::difference_type __n;
        for(__n = __last - __first; __n > 0; --__n) *--__result = std::move(*--__last);
        return __result;
      }
    };

    template<bool _IsMove>
    struct __copy_move_backward<_IsMove, true, random_access_iterator_tag> {
      template<class _Tp>
      static _Tp* __copy_move_b(const _Tp* __first, const _Tp* __last, _Tp* __result) {
        static_assert(is_copy_assignable<_Tp>::value, "the given type is not assignable");
        const ptrdiff_t _Num = __last - __first;
        if(_Num) __builtin_memmove(__result - _Num, __first, sizeof(_Tp) * _Num);
        return __result - _Num;
      }
    };

    template<bool _IsMove, class _BI1, class _BI2>
    inline _BI2 __copy_move_backward_a(_BI1 __first, _BI1 __last, _BI2 __result) {
      typedef typename iterator_traits<_BI1>::value_type _ValueType1;
      typedef typename iterator_traits<_BI2>::value_type _ValueType2;
      typedef typename iterator_traits<_BI1>::iterator_category _Category;
      const bool __simple = (__is_trivial(_ValueType1)
                          && __is_pointer<_BI1>::__value
                          && __is_pointer<_BI2>::__value
                          && __are_same<_ValueType1, _ValueType2>::__value);
      return __copy_move_backward<_IsMove, __simple, _Category>::__copy_move_b(__first, __last, __result);
    }

    template<bool _IsMove, class _BI1, class _BI2>
    inline _BI2 __copy_move_backward_a2(_BI1 __first, _BI1 __last, _BI2 __result) {
      return _BI2(__copy_move_backward_a<_IsMove>(__niter_base(__first),
                                                  __niter_base(__last),
                                                  __niter_base(__result)));
    }

  } // end of __detail:: namespace
  // End of internal copy/move backwards implementations


  // Start of std::copy_backward and std::move_backward implementation:
  template<class _BI1, class _BI2>
  inline _BI2 copy_backward(_BI1 __first, _BI1 __last, _BI2 __result) {
    __glibcxx_function_requires(_BidirectionalIteratorConcept<_BI1>)
    __glibcxx_function_requires(_Mutable_BidirectionalIteratorConcept<_BI2>)
    __glibcxx_function_requires(_ConvertibleConcept<typename iterator_traits<_BI1>::value_type,
                                                    typename iterator_traits<_BI2>::value_type>)
    __glibcxx_requires_valid_range(__first, __last);
    return (__detail::__copy_move_backward_a2<__detail::__is_move_iterator<_BI1>::__value>(__detail::__miter_base(__first),
                                                                                           __detail::__miter_base(__last),
                                                                                           __result));
  }

  template<class _BI1, class _BI2>
  inline _BI2 move_backward(_BI1 __first, _BI1 __last, _BI2 __result) {
    __glibcxx_function_requires(_BidirectionalIteratorConcept<_BI1>)
    __glibcxx_function_requires(_Mutable_BidirectionalIteratorConcept<_BI2>)
    __glibcxx_function_requires(_ConvertibleConcept<typename iterator_traits<_BI1>::value_type,
                                                    typename iterator_traits<_BI2>::value_type>)
    __glibcxx_requires_valid_range(__first, __last);
    return __detail::__copy_move_backward_a2<true>(__detail::__miter_base(__first),
                                                   __detail::__miter_base(__last),
                                                   __result);
  }

# define _GLIBCXX_MOVE_BACKWARD3(_Tp, _Up, _Vp) std::move_backward(_Tp, _Up, _Vp)
  // End of std::copy_backward and std::move_backward


  // Start of std::fill implementation:
  namespace __detail { // namespace for internal implementations

    template<class _ForwardIterator, class _Tp>
    inline typename __gnu_cxx::__enable_if<!__is_scalar<_Tp>::__value, void>::__type
    __fill_a(_ForwardIterator __first, _ForwardIterator __last, const _Tp& __value) {
      for(; __first != __last; ++__first) *__first = __value;
    }

    template<class _ForwardIterator, class _Tp>
    inline typename __gnu_cxx::__enable_if<__is_scalar<_Tp>::__value, void>::__type
    __fill_a(_ForwardIterator __first, _ForwardIterator __last, const _Tp& __value) {
      const _Tp __tmp = __value;
      for(; __first != __last; ++__first) *__first = __tmp;
    }

    template<class _Tp>
    inline typename __gnu_cxx::__enable_if<__is_byte<_Tp>::__value, void>::__type
    __fill_a(_Tp* __first, _Tp* __last, const _Tp& __c) {
      const _Tp __tmp = __c;
      __builtin_memset(__first, static_cast<unsigned char>(__tmp), __last - __first);
    }

  } // end of __detail:: namespace

  template<class _ForwardIterator, class _Tp>
  inline void fill(_ForwardIterator __first, _ForwardIterator __last, const _Tp& __value) {
    __glibcxx_function_requires(_Mutable_ForwardIteratorConcept<_ForwardIterator>)
    __glibcxx_requires_valid_range(__first, __last);
    __detail::__fill_a(__detail::__niter_base(__first), __detail::__niter_base(__last), __value);
  }

  namespace __detail { // continuing internal implementations

    template<class _OutputIterator, class _Size, class _Tp>
    inline typename __gnu_cxx::__enable_if<!__is_scalar<_Tp>::__value, _OutputIterator>::__type
    __fill_n_a(_OutputIterator __first, _Size __n, const _Tp& __value) {
      for(__decltype(__n + 0) __niter = __n; __niter > 0; --__niter, ++__first) *__first = __value;
      return __first;
    }

    template<class _OutputIterator, class _Size, class _Tp>
    inline typename __gnu_cxx::__enable_if<__is_scalar<_Tp>::__value, _OutputIterator>::__type
    __fill_n_a(_OutputIterator __first, _Size __n, const _Tp& __value) {
      const _Tp __tmp = __value;
      for(__decltype(__n + 0) __niter = __n; __niter > 0; --__niter, ++__first) *__first = __tmp;
      return __first;
    }

    template<class _Size, class _Tp>
    inline typename __gnu_cxx::__enable_if<__is_byte<_Tp>::__value, _Tp*>::__type
    __fill_n_a(_Tp* __first, _Size __n, const _Tp& __c) {
      __fill_a(__first, __first + __n, __c);
      return __first + __n;
    }

  } // end of __detail:: namespace

  template<class _OI, class _Size, class _Tp>
  inline _OI fill_n(_OI __first, _Size __n, const _Tp& __value) {
    __glibcxx_function_requires(_OutputIteratorConcept<_OI, _Tp>)
    return _OI(__detail::__fill_n_a(__detail::__niter_base(__first), __n, __value));
  }
  // End of std::fill


  // Start of internal comparison implementations:
  namespace __detail { // namespace for internal implementations

    template<bool _BoolType>
    struct __equal {
      template<class _II1, class _II2>
      static bool equal(_II1 __first1, _II1 __last1, _II2 __first2) {
        for(; __first1 != __last1; ++__first1, ++__first2) if(!(*__first1 == *__first2)) return false;
        return true;
      }
    };

    template<>
    struct __equal<true> {
      template<class _Tp>
      static bool equal(const _Tp* __first1, const _Tp* __last1, const _Tp* __first2) {
        return !__builtin_memcmp(__first1, __first2, sizeof(_Tp) * (__last1 - __first1));
      }
    };

    template<class _II1, class _II2>
    inline bool __equal_aux(_II1 __first1, _II1 __last1, _II2 __first2) {
      typedef typename iterator_traits<_II1>::value_type _ValueType1;
      typedef typename iterator_traits<_II2>::value_type _ValueType2;
      const bool __simple = ((__is_integer<_ValueType1>::__value || __is_pointer<_ValueType1>::__value)
                          && __is_pointer<_II1>::__value
                          && __is_pointer<_II2>::__value
                          && __are_same<_ValueType1, _ValueType2>::__value);
      return __equal<__simple>::equal(__first1, __last1, __first2);
    }

    template<class, class>
    struct __lc_rai {
      template<class _II1, class _II2>
      static _II1 __newlast1(_II1, _II1 __last1, _II2, _II2) { return __last1; }

      template<class _II>
      static bool __cnd2(_II __first, _II __last) { return __first != __last; }
    };

    template<>
    struct __lc_rai<random_access_iterator_tag, random_access_iterator_tag> {
      template<class _RAI1, class _RAI2>
      static _RAI1 __newlast1(_RAI1 __first1, _RAI1 __last1, _RAI2 __first2, _RAI2 __last2) {
        const typename iterator_traits<_RAI1>::difference_type __diff1 = __last1 - __first1;
        const typename iterator_traits<_RAI2>::difference_type __diff2 = __last2 - __first2;
        return __diff2 < __diff1 ? __first1 + __diff2 : __last1;
      }

      template<class _RAI>
      static bool __cnd2(_RAI, _RAI) { return true; }
    };

    template<class _II1, class _II2, class _Compare>
    bool __lexicographical_compare_impl(_II1 __first1, _II1 __last1, _II2 __first2, _II2 __last2, _Compare __comp) {
      typedef typename iterator_traits<_II1>::iterator_category _Category1;
      typedef typename iterator_traits<_II2>::iterator_category _Category2;
      typedef __lc_rai<_Category1, _Category2> __rai_type;

      __last1 = __rai_type::__newlast1(__first1, __last1, __first2, __last2);
      for(; __first1 != __last1 && __rai_type::__cnd2(__first2, __last2); ++__first1, ++__first2) {
        if(__comp(__first1, __first2)) return true;
        if(__comp(__first2, __first1)) return false;
      }
      return __first1 == __last1 && __first2 != __last2;
    }

    template<bool _BoolType>
    struct __lexicographical_compare {
      template<class _II1, class _II2>
      static bool __lc(_II1, _II1, _II2, _II2);
    };

    template<bool _BoolType>
    template<class _II1, class _II2>
    bool __lexicographical_compare<_BoolType>::__lc(_II1 __first1, _II1 __last1, _II2 __first2, _II2 __last2) {
      return __lexicographical_compare_impl(__first1, __last1, __first2, __last2,
                                            __gnu_cxx::__ops::__iter_less_iter());
    }

    template<>
    struct __lexicographical_compare<true> {
      template<class _Tp, class _Up>
      static bool __lc(const _Tp* __first1, const _Tp* __last1, const _Up* __first2, const _Up* __last2) {
        const size_t __len1 = __last1 - __first1;
        const size_t __len2 = __last2 - __first2;
        const int __result = __builtin_memcmp(__first1, __first2, min(__len1, __len2));
        return __result != 0 ? __result < 0 : __len1 < __len2;
      }
    };

    template<class _II1, class _II2>
    inline bool __lexicographical_compare_aux(_II1 __first1, _II1 __last1, _II2 __first2, _II2 __last2) {
      typedef typename iterator_traits<_II1>::value_type _ValueType1;
      typedef typename iterator_traits<_II2>::value_type _ValueType2;
      const bool __simple = (__is_byte<_ValueType1>::__value && __is_byte<_ValueType2>::__value
                          && !__gnu_cxx::__numeric_traits<_ValueType1>::__is_signed
                          && !__gnu_cxx::__numeric_traits<_ValueType2>::__is_signed
                          && __is_pointer<_II1>::__value
                          && __is_pointer<_II2>::__value);
      return __lexicographical_compare<__simple>::__lc(__first1, __last1, __first2, __last2);
    }

  } // end of __detail:: namespace
  // End of internal comparison implementations


  // Start of std::lower_bound implementation:
  namespace __detail { // namespace for internal implementation

    template<class _ForwardIterator, class _Tp, class _Compare>
    _ForwardIterator __lower_bound(_ForwardIterator __first, _ForwardIterator __last, const _Tp& __val, _Compare __comp) {
      typedef typename iterator_traits<_ForwardIterator>::difference_type _DistanceType;
      _DistanceType __len = distance(__first, __last);
      while(__len > 0) {
        _DistanceType __half = __len >> 1;
        _ForwardIterator __middle = __first;
        std::advance(__middle, __half);
        if(__comp(__middle, __val)) {
          __first = __middle;
          ++__first;
          __len = __len - __half - 1;
        } else __len = __half;
      }
      return __first;
    }

  } // end of __detail:: namespace

  template<class _ForwardIterator, class _Tp>
  inline _ForwardIterator lower_bound(_ForwardIterator __first, _ForwardIterator __last, const _Tp& __val) {
    __glibcxx_function_requires(_ForwardIteratorConcept<_ForwardIterator>)
    __glibcxx_function_requires(_LessThanOpConcept<typename iterator_traits<_ForwardIterator>::value_type, _Tp>)
    __glibcxx_requires_partitioned_lower(__first, __last, __val);
    return __detail::__lower_bound(__first, __last, __val,
                                   __gnu_cxx::__ops::__iter_less_val());
  }
  // End of std::lower_bound


  // Start of internal sort helper implementations:
  namespace __detail { // namespace for internal implementations

    inline GLIBCXX_CONSTEXPR int __lg(int __n) {
      return sizeof(int) * __CHAR_BIT__ - 1 - __builtin_clz(__n);
    }

    inline GLIBCXX_CONSTEXPR unsigned __lg(unsigned __n) {
      return sizeof(int) * __CHAR_BIT__ - 1 - __builtin_clz(__n);
    }

    inline GLIBCXX_CONSTEXPR long __lg(long __n) {
      return sizeof(long) * __CHAR_BIT__ - 1 - __builtin_clzl(__n);
    }

    inline GLIBCXX_CONSTEXPR unsigned long __lg(unsigned long __n) {
      return sizeof(long) * __CHAR_BIT__ - 1 - __builtin_clzl(__n);
    }

    inline GLIBCXX_CONSTEXPR long long __lg(long long __n) {
      return sizeof(long long) * __CHAR_BIT__ - 1 - __builtin_clzll(__n);
    }

    inline GLIBCXX_CONSTEXPR unsigned long long __lg(unsigned long long __n) {
      return sizeof(long long) * __CHAR_BIT__ - 1 - __builtin_clzll(__n);
    }

  } // end of __detail:: namespace
  // End of internal sort helpers

  
  // Start of std::equal implementation:
  template<class _II1, class _II2>
  inline bool equal(_II1 __first1, _II1 __last1, _II2 __first2) {
    __glibcxx_function_requires(_InputIteratorConcept<_II1>)
    __glibcxx_function_requires(_InputIteratorConcept<_II2>)
    __glibcxx_function_requires(_EqualOpConcept<typename iterator_traits<_II1>::value_type,
                                                typename iterator_traits<_II2>::value_type>)
    __glibcxx_requires_valid_range(__first1, __last1);
    return __detail::__equal_aux(__detail::__niter_base(__first1),
                                 __detail::__niter_base(__last1),
                                 __detail::__niter_base(__first2));
  }

  template<class _IIter1, class _IIter2, class _BinaryPredicate>
  inline bool equal(_IIter1 __first1, _IIter1 __last1, _IIter2 __first2, _BinaryPredicate __binary_pred) {
    __glibcxx_function_requires(_InputIteratorConcept<_IIter1>)
    __glibcxx_function_requires(_InputIteratorConcept<_IIter2>)
    __glibcxx_requires_valid_range(__first1, __last1);
    for(; __first1 != __last1; ++__first1, ++__first2) if(!bool(__binary_pred(*__first1, *__first2))) return false;
    return true;
  }

  template<class _II1, class _II2>
  inline bool equal(_II1 __first1, _II1 __last1, _II2 __first2, _II2 __last2) {
    __glibcxx_function_requires(_InputIteratorConcept<_II1>)
    __glibcxx_function_requires(_InputIteratorConcept<_II2>)
    __glibcxx_function_requires(_EqualOpConcept<typename iterator_traits<_II1>::value_type,
                                                typename iterator_traits<_II2>::value_type>)
    __glibcxx_requires_valid_range(__first1, __last1);
    __glibcxx_requires_valid_range(__first2, __last2);

    using _RATag = random_access_iterator_tag;
    using _Cat1 = typename iterator_traits<_II1>::iterator_category;
    using _Cat2 = typename iterator_traits<_II2>::iterator_category;
    using _RAIters = __and_<is_same<_Cat1, _RATag>, is_same<_Cat2, _RATag>>;
    
    if(_RAIters()) {
      auto __d1 = distance(__first1, __last1);
      auto __d2 = distance(__first2, __last2);
      if(__d1 != __d2) return false;
      return std::equal(__first1, __last1, __first2);
    }

    for(; __first1 != __last1 && __first2 != __last2; ++__first1, ++__first2) 
      if(!(*__first1 == *__first2)) return false;
    return __first1 == __last1 && __first2 == __last2;
  }

  template<class _IIter1, class _IIter2, class _BinaryPredicate>
  inline bool equal(_IIter1 __first1, _IIter1 __last1, _IIter2 __first2, _IIter2 __last2, _BinaryPredicate __binary_pred) {
    __glibcxx_function_requires(_InputIteratorConcept<_IIter1>)
    __glibcxx_function_requires(_InputIteratorConcept<_IIter2>)
    __glibcxx_requires_valid_range(__first1, __last1);
    __glibcxx_requires_valid_range(__first2, __last2);

    using _RATag = random_access_iterator_tag;
    using _Cat1 = typename iterator_traits<_IIter1>::iterator_category;
    using _Cat2 = typename iterator_traits<_IIter2>::iterator_category;
    using _RAIters = __and_<std::is_same<_Cat1, _RATag>, std::is_same<_Cat2, _RATag>>;
    
    if(_RAIters()) {
      auto __d1 = std::distance(__first1, __last1);
      auto __d2 = std::distance(__first2, __last2);
      if(__d1 != __d2) return false;
      return std::equal(__first1, __last1, __first2, __binary_pred);
    }

    for(; __first1 != __last1 && __first2 != __last2; ++__first1, ++__first2) 
      if(!bool(__binary_pred(*__first1, *__first2))) return false;
    return __first1 == __last1 && __first2 == __last2;
  }
  // End of std::equal


  // Start of std::lexicographical_compare implementation:
  template<class _II1, class _II2>
  inline bool lexicographical_compare(_II1 __first1, _II1 __last1, _II2 __first2, _II2 __last2) {
    typedef typename iterator_traits<_II1>::value_type _ValueType1;
    typedef typename iterator_traits<_II2>::value_type _ValueType2;

    __glibcxx_function_requires(_InputIteratorConcept<_II1>)
    __glibcxx_function_requires(_InputIteratorConcept<_II2>)
    __glibcxx_function_requires(_LessThanOpConcept<_ValueType1, _ValueType2>)
    __glibcxx_function_requires(_LessThanOpConcept<_ValueType2, _ValueType1>)
    __glibcxx_requires_valid_range(__first1, __last1);
    __glibcxx_requires_valid_range(__first2, __last2);

    return __detail::__lexicographical_compare_aux(__detail::__niter_base(__first1),
                                                   __detail::__niter_base(__last1),
                                                   __detail::__niter_base(__first2),
                                                   __detail::__niter_base(__last2));
  }

  template<class _II1, class _II2, class _Compare>
  inline bool lexicographical_compare(_II1 __first1, _II1 __last1, _II2 __first2, _II2 __last2, _Compare __comp) {
    __glibcxx_function_requires(_InputIteratorConcept<_II1>)
    __glibcxx_function_requires(_InputIteratorConcept<_II2>)
    __glibcxx_requires_valid_range(__first1, __last1);
    __glibcxx_requires_valid_range(__first2, __last2);

    return __detail::__lexicographical_compare_impl(__first1, __last1, __first2, __last2,
                                                    __gnu_cxx::__ops::__iter_comp_iter(__comp));
  }
  // End of std::lexicographical_compare


  // Start of std::mismatch implementation:
  namespace __detail { // namespace for internal implementations

    template<class _InputIterator1, class _InputIterator2, class _BinaryPredicate>
    std::pair<_InputIterator1, _InputIterator2>
    __mismatch(_InputIterator1 __first1, _InputIterator1 __last1,
               _InputIterator2 __first2, _BinaryPredicate __binary_pred) {
      while(__first1 != __last1 && __binary_pred(__first1, __first2)) {
        ++__first1;
        ++__first2;
      }
      return std::pair<_InputIterator1, _InputIterator2>(__first1, __first2);
    }

  } // end of __detail:: namespace

  template<class _InputIterator1, class _InputIterator2>
  inline std::pair<_InputIterator1, _InputIterator2>
  mismatch(_InputIterator1 __first1, _InputIterator1 __last1, _InputIterator2 __first2) {
    __glibcxx_function_requires(_InputIteratorConcept<_InputIterator1>)
    __glibcxx_function_requires(_InputIteratorConcept<_InputIterator2>)
    __glibcxx_function_requires(_EqualOpConcept<typename iterator_traits<_InputIterator1>::value_type,
                                                typename iterator_traits<_InputIterator2>::value_type>)
    __glibcxx_requires_valid_range(__first1, __last1);

    return __detail::__mismatch(__first1, __last1, __first2,
                                __gnu_cxx::__ops::__iter_equal_to_iter());
  }

  template<class _InputIterator1, class _InputIterator2, class _BinaryPredicate>
  inline std::pair<_InputIterator1, _InputIterator2>
  mismatch(_InputIterator1 __first1, _InputIterator1 __last1,
           _InputIterator2 __first2, _BinaryPredicate __binary_pred) {
    __glibcxx_function_requires(_InputIteratorConcept<_InputIterator1>)
    __glibcxx_function_requires(_InputIteratorConcept<_InputIterator2>)
    __glibcxx_requires_valid_range(__first1, __last1);

    return __detail::__mismatch(__first1, __last1, __first2,
                                __gnu_cxx::__ops::__iter_comp_iter(__binary_pred));
  }

  namespace __detail { // continuing internal implementations

    template<class _InputIterator1, class _InputIterator2, class _BinaryPredicate>
    std::pair<_InputIterator1, _InputIterator2>
    __mismatch(_InputIterator1 __first1, _InputIterator1 __last1,
               _InputIterator2 __first2, _InputIterator2 __last2,
               _BinaryPredicate __binary_pred) {
      while(__first1 != __last1 && __first2 != __last2 && __binary_pred(__first1, __first2)) {
        ++__first1;
        ++__first2;
      }
      return std::pair<_InputIterator1, _InputIterator2>(__first1, __first2);
    }

  } // end of __detail:: namespace

  template<class _InputIterator1, class _InputIterator2>
  inline std::pair<_InputIterator1, _InputIterator2>
  mismatch(_InputIterator1 __first1, _InputIterator1 __last1,
           _InputIterator2 __first2, _InputIterator2 __last2) {
    __glibcxx_function_requires(_InputIteratorConcept<_InputIterator1>)
    __glibcxx_function_requires(_InputIteratorConcept<_InputIterator2>)
    __glibcxx_function_requires(_EqualOpConcept<typename iterator_traits<_InputIterator1>::value_type,
                                                typename iterator_traits<_InputIterator2>::value_type>)
    __glibcxx_requires_valid_range(__first1, __last1);
    __glibcxx_requires_valid_range(__first2, __last2);

    return __detail::__mismatch(__first1, __last1, __first2, __last2,
                                __gnu_cxx::__ops::__iter_equal_to_iter());
  }

  template<class _InputIterator1, class _InputIterator2, class _BinaryPredicate>
  inline std::pair<_InputIterator1, _InputIterator2>
  mismatch(_InputIterator1 __first1, _InputIterator1 __last1,
           _InputIterator2 __first2, _InputIterator2 __last2, _BinaryPredicate __binary_pred) {
    __glibcxx_function_requires(_InputIteratorConcept<_InputIterator1>)
    __glibcxx_function_requires(_InputIteratorConcept<_InputIterator2>)
    __glibcxx_requires_valid_range(__first1, __last1);
    __glibcxx_requires_valid_range(__first2, __last2);

    return __detail::__mismatch(__first1, __last1, __first2, __last2,
                                __gnu_cxx::__ops::__iter_comp_iter(__binary_pred));
  }
  // End of std::mismatch

} // end of std:: namespace


// I'm not sure If I should implement this part or anything using _GLIBCXX_PARALLEL
// I never defined this anywhere so this part is just ignored by the compiler for now.
//#ifdef _GLIBCXX_PARALLEL 
//# include "Parallel_AlgoBase.hpp"
//#endif
// I will most likely just delete this


#endif // End of AlgoBase.h
