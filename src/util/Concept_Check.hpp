// Libstdc++ and GCC GNU Lincese boilerplate:      (this is required since this code is copyrighted under the GCC compiler. I did not design this myself.)
/*
  Partial Modified Implementation Of Meta-Functions from the <concept_check> Libstdc++ header  -*- C++ -*-

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


  (C) Copyright Jeremy Siek 2000. Permission to copy, use, modify,
   sell and distribute this software is granted provided this
   copyright notice appears in all copies. This software is provided
   "as is" without express or implied warranty, and with no claim as
   to its suitability for any purpose.


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
  Concept_Check.hpp  [V1.0.0]  (header file for c++14 style concept checks)
    By: Andrey Stroitelev

  ~This file implements a series of functions that are used to make sure template arguments meet the given requirements.
  ~These concept checks are mostly used internally.
  ~Unlike the concepts I implemented these concept checks will throw errors when they fail the check instead of the function using them just being ignored.
  ~This file is for internal library use only. The user shouldn't use anything defined here
  ~These concept checks are only meant for some c++14 implementations.
    *These will not be the main method of checking requirements, so instead I'm going to use the implementations in Concepts.hpp.
    *In Concepts.hpp the concepts are implemented like type_trait functions, but these are runtime checks that try to do the requested checks in an evaluated context
    *The Concepts.hpp checks wont cause an error when a check fails unlike the implementations here, but the Concepts.hpp checks require SFINAE which adds implementation sugar
  ~Concepts implemented in this file will mostly do runtime checks instead of compile time checks.
  ~I will try to use these implementations as little as possible since unlike the Concepts.hpp concepts the implementations here are not zero cost abstractions.
    *Since the concepts in this file are not zero cost abstractions and run at runtime they will slow down the main program, and these will use memory.
    *Memory usage from the concept checks here is dependant on the types you're checking requirements for. When checking if an iterator is a forward iterator the concept check will have to instantiate the given iterator class
*/


// Start of Concept_Check.hpp
#ifndef _CONCEPT_CHECK_HPP_
#define _CONCEPT_CHECK_HPP_

#ifndef _CONCEPT_CHECK_VERSION_
#define _CONCEPT_CHECK_VERSION_ 0x010000
#endif


#include <Arduino.h>
#include "Config.h"
#include "Iterator_Traits.hpp"


_DLIB_HEADER


// Start of concept check implementations:
// NOT MEANT TO BE USED BY USER
namespace __gnu_cxx { // private internal namespace for adding to language functionality

# define _IsUnused __attribute__((__unused__))

  // function to make requirement checks in a function
  template<class _Concept>
  inline void __function_requires() {
    void(_Concept::*__x)() _IsUnused = &_Concept::__constraints;
  }

  // these are used to throw errors. They end up being linker errors, so debugging might be difficult.
  void __error_type_must_be_an_integer_type();
  void __error_type_must_be_an_unsigned_integer_type();
  void __error_type_must_be_a_signed_integer_type();

  // Start of class type requirement check macro implementations:
# define _GLIBCXX_CLASS_REQUIRES(_type_var, _ns, _concept) \
  typedef void (_ns::_concept <_type_var>::* _func##_type_var##_concept)(); \
  template <_func##_type_var##_concept _Tp1> \
  struct _concept_checking##_type_var##_concept { }; \
  typedef _concept_checking##_type_var##_concept< \
    &_ns::_concept<_type_var>::__constraints> \
    _concept_checking_typedef##_type_var##_concept
  
# define _GLIBCXX_CLASS_REQUIRES2(_type_var1, _type_var2, _ns, _concept) \
  typedef void (_ns::_concept <_type_var1, _type_var2>::* _func##_type_var1##_type_var2##_concept)(); \
  template <_func##_type_var1##_type_var2##_concept _Tp1> \
  struct _concept_checking##_type_var1##_type_var2##_concept { }; \
  typedef _concept_checking##_type_var1##_type_var2##_concept< \
    &_ns::_concept <_type_var1, _type_var2>::__constraints> \
    _concept_checking_typedef##_type_var1##_type_var2##_concept

# define _GLIBCXX_CLASS_REQUIRES3(_type_var1, _type_var2, _type_var3, _ns, _concept) \
  typedef void (_ns::_concept <_type_var1, _type_var2, _type_var3>::* _func##_type_var1##_type_var2##_type_var3##_concept)(); \
  template<_func##_type_var1##_type_var2##_type_var3##_concept _Tp1> \
  struct _concept_checking##_type_var1##_type_var2##_type_var3##_concept { }; \
  typedef _concept_checking##_type_var1##_type_var2##_type_var3##_concept< \
    &_ns::_concept<_type_var1, _type_var2, _type_var3>::__constraints> \
    _concept_checking_typedef##_type_var1##_type_var2##_type_var3##_concept
  
# define _GLIBCXX_CLASS_REQUIRES4(_type_var1, _type_var2, _type_var3, _type_var4, _ns, _concept) \
  typedef void (_ns::_concept<_type_var1, _type_var2, _type_var3, _type_var4>::* _func##_type_var1##_type_var2##_type_var3##_type_var4##_concept)(); \
  template<_func##_type_var1##_type_var2##_type_var3##_type_var4##_concept _Tp1> \
  struct _concept_checking##_type_var1##_type_var2##_type_var3##_type_var4##_concept { }; \
  typedef _concept_checking##_type_var1##_type_var2##_type_var3##_type_var4##_concept< \
    &_ns::_concept<_type_var1, _type_var2, _type_var3, _type_var4>::__constraints> \
    _concept_checking_typedef##_type_var1##_type_var2##_type_var3##_type_var4##_concept
  // End of class requirement check macros


  // Start of type concept helper implementations:
  template<class _Tp1, class _Tp2>
  struct _Aux_require_same { };

  template<class _Tp>
  struct _Aux_require_same<_Tp, _Tp> { typedef _Tp _Type; };

  template<class _Tp1, class _Tp2>
  struct _SameTypeConcept {
    void __constraints() {
      typedef typename _Aux_require_same<_Tp1, _Tp2>::_Type _Required;
    }
  };
  // End of type concept helpers


  // Start of _IntegerConcept implementation:
  template<class _Tp>
  struct _IntegerConcept {
    void __constraints() {
      __error_type_must_be_an_integer_type();
    }
  };

  template<>
  struct _IntegerConcept<short> { void __constraints() {} };

  template<>
  struct _IntegerConcept<unsigned short> { void __constraints() {} };
  
  template<>
  struct _IntegerConcept<int> { void __constraints() {} };

  template<>
  struct _IntegerConcept<unsigned int> { void __constraints() {} };

  template<>
  struct _IntegerConcept<long> { void __constraints() {} };

  template<>
  struct _IntegerConcept<unsigned long> { void __constraints() {} };

  template<>
  struct _IntegerConcept<long long> { void __constraints() {} };

  template<>
  struct _IntegerConcept<unsigned long long> { void __constraints() {} };
  // End of _IntegerConcept


  // Start of _SignedIntegerConcept implementation:
  template<class _Tp>
  struct _SignedIntegerConcept {
    void __constraints() {
      __error_type_must_be_a_signed_integer_type();
    }
  };

  template<>
  struct _SignedIntegerConcept<short> { void __constraints() {} };

  template<>
  struct _SignedIntegerConcept<int> { void __constraints() {} };

  template<>
  struct _SignedIntegerConcept<long> { void __constraints() {} };

  template<>
  struct _SignedIntegerConcept<long long> { void __constraints() {} };
  // End of _SignedIntegerConcept


  // Start of _UnsignedIntegerConcept implementation:
  template<class _Tp>
  struct _UnsignedIntegerConcept {
    void __constraints() {
      __error_type_must_be_an_unsigned_integer_type();
    }
  };

  template<>
  struct _UnsignedIntegerConcept<unsigned short> { void __constraints() {} };

  template<>
  struct _UnsignedIntegerConcept<unsigned int> { void __constraints() {} };

  template<>
  struct _UnsignedIntegerConcept<unsigned long> { void __constraints() {} };

  template<>
  struct _UnsignedIntegerConcept<unsigned long long> { void __constraints() {} };
  // End of _UnsignedIntegerConcept


  // Start of _DefaultConstructibleConcept implementation:
  template<class _Tp>
  struct _DefaultConstructibleConcept {
    void __constraints() {
      _Tp __a _IsUnused;
    }
  };
  // End of _DefaultConstructibleConcept


  // Start of _AssignableConcept implementation:
  template<class _Tp>
  struct _AssignableConcept {
    void __constraints() {
      __a = __a;
      __const_constraints(__a);
    }
    void __const_constraints(const _Tp& __b) {
      __a = __b;
    }
    _Tp __a;
  };
  // End of _AssignableConcept


  // Start of _CopyConstructibleConcept implementation:
  template<class _Tp>
  struct _CopyConstructibleConcept {
    void __constraints() {
      _Tp __a(__b);
      _Tp* __ptr _IsUnused = &__a;
      __const_constraints(__a);
    }
    void __const_constraints(const _Tp& __a) {
      _Tp __c _IsUnused(__a);
      const _Tp* __ptr _IsUnused = &__a;
    }
    _Tp __b;
  };
  // End of _CopyConstructibleConcept


  // Start of _SGIAssignableConcept implementation:
  template<class _Tp>
  struct _SGIAssignableConcept {
    void __constraints() {
      _Tp __b _IsUnused(__a);
      __a = __a;
      __const_constraints(__a);
    }
    void __const_constraints(const _Tp& __b) {
      _Tp __c _IsUnused(__b);
      __a = __b;
    }
    _Tp __a;
  };
  // End of _SGIAssignableConcept


  // Start of _ConvertibleConcept implementation:
  template<class _From, class _To>
  struct _ConvertibleConcept {
    void __constraints() {
      _To __y _IsUnused = __x;
    }
    _From __x;
  };
  // End of _ConvertibleConcept


  // Start of __aux_require_boolean_expr implementation:
  template<class _Tp>
  void __aux_require_boolean_expr(const _Tp& __t) {
    bool __x _IsUnused = __t;
  }
  // End of __aux_require_boolean_expr


  // Start of _EqualityComparableConcept implementation:
  template<class _Tp>
  struct _EqualityComparableConcept {
    void __constraints() {
      __aux_require_boolean_expr(__a == __b);
    }
    _Tp __a, __b;
  };
  // End of _EqualityComparableConcept

  
  // Start of _LessThanComparableConcept implementation:
  template<class _Tp>
  struct _LessThanComparableConcept {
    void __constraints() {
      __aux_require_boolean_expr(__a < __b);
    }
    _Tp __a, __b;
  };
  // End of _LessThanComparableConcept


  // Start of _ComparableConcept implementation:
  template<class _Tp>
  struct _ComparableConcept {
    void __constraints() {
      __aux_require_boolean_expr(__a < __b);
      __aux_require_boolean_expr(__a > __b);
      __aux_require_boolean_expr(__a <= __b);
      __aux_require_boolean_expr(__a >= __b);
    }
    _Tp __a, __b;
  };
  // End of _ComparableConcept implementation


  // Start of binary operator concept implementation:
  // These implementations use macros to cut down on implementation sugar.
  // These macros are used to define a bunch of repetitive concept checks in alot less lines.
  // These macros are undefined outside of the expansions below.
# define _GLIBCXX_DEFINE_BINARY_PREDICATE_OP_CONSTRAINT(_OP, _NAME) \
  template<class _First, class _Second> \
  struct _NAME { \
    void __constraints() { (void)__constraints_(); } \
    bool __constraints_() { \
      return __a _OP __b; \
    } \
    _First __a; \
    _Second __b; \
  }
  
# define _GLIBCXX_DEFINE_BINARY_OPERATOR_CONSTRAINT(_OP, _NAME) \
  template<class _Ret, class _First, class _Second> \
  struct _NAME { \
    void __constraints() { (void)__constraints_(); } \
    _Ret __constraints_() { \
      return __a _OP __b; \
    } \
    _First __a; \
    _Second __b; \
  }

  // expansions to define concepts for binary comparison operators:
  _GLIBCXX_DEFINE_BINARY_PREDICATE_OP_CONSTRAINT(==, _EqualOpConcept);
  _GLIBCXX_DEFINE_BINARY_PREDICATE_OP_CONSTRAINT(!=, _NotEqualOpConcept);
  _GLIBCXX_DEFINE_BINARY_PREDICATE_OP_CONSTRAINT(<, _LessThanOpConcept);
  _GLIBCXX_DEFINE_BINARY_PREDICATE_OP_CONSTRAINT(<=, _LessEqualOpConcept);
  _GLIBCXX_DEFINE_BINARY_PREDICATE_OP_CONSTRAINT(>, _GreaterThanOpConcept);
  _GLIBCXX_DEFINE_BINARY_PREDICATE_OP_CONSTRAINT(>=, _GreaterEqualOpConcept);

  //expansions to define concepts for binary arithmetic operators:
  _GLIBCXX_DEFINE_BINARY_OPERATOR_CONSTRAINT(+, _PlusOpConcept);
  _GLIBCXX_DEFINE_BINARY_OPERATOR_CONSTRAINT(*, _TimesOpConcept);
  _GLIBCXX_DEFINE_BINARY_OPERATOR_CONSTRAINT(/, _DivideOpConcept);
  _GLIBCXX_DEFINE_BINARY_OPERATOR_CONSTRAINT(-, _SubtractOpConcept);
  _GLIBCXX_DEFINE_BINARY_OPERATOR_CONSTRAINT(%, _ModOpConcept);

  // undefining macros since they will no longer be used.
# undef _GLIBCXX_DEFINE_BINARY_PREDICATE_OP_CONSTRAINT
# undef _GLIBCXX_DEFINE_BINARY_OPERATOR_CONSTRAINT
  // End of binary operator concepts


  // Start of _GeneratorConcept implementation:
  template<class _Func, class _Return>
  struct _GeneratorConcept {
    void __constraints() {
      const _Return& __r _IsUnused = __f();
    }
    _Func __f;
  };

  template<class _Func>
  struct _GeneratorConcept<_Func, void> {
    void __constraints() {
      __f();
    }
    _Func __f;
  };
  // End of _GeneratorConcept


  // Start of _UnaryFunctionConcept implementation:
  template<class _Func, class _Return, class _Arg>
  struct _UnaryFunctionConcept {
    void __constraints() {
      __r = __f(__arg);
    }
    _Func __f;
    _Arg __arg;
    _Return __r;
  };

  template<class _Func, class _Arg>
  struct _UnaryFunctionConcept<_Func, void, _Arg> {
    void __constraints() {
      __f(__arg);
    }
    _Func __f;
    _Arg __arg;
  };
  // End of _UnaryFunctionConcept


  // Start of _BinaryFunctionConcept implementation:
  template<class _Func, class _Return, class _First, class _Second>
  struct _BinaryFunctionConcept {
    void __constraints() {
      __r = __f(__first, __second);
    }
    _Func __f;
    _First __first;
    _Second __second;
    _Return __r;
  };

  template<class _Func, class _First, class _Second>
  struct _BinaryFunctionConcept<_Func, void, _First, _Second> {
    void __constraints() {
      __f(__first, __second);
    }
    _Func __f;
    _First __first;
    _Second __second;
  };
  // End of _BinaryFunctionConcept


  // Start of _UnaryPredicateConcept implementation:
  template<class _Func, class _Arg>
  struct _UnaryPredicateConcept {
    void __constraints() {
      __aux_require_boolean_expr(__f(__arg));
    }
    _Func __f;
    _Arg __arg;
  };
  // End of _UnaryPredicateConcept


  // Start of _BinaryPredicateConcept implementation:
  template<class _Func, class _First, class _Second>
  struct _BinaryPredicateConcept {
    void __constraints() {
      __aux_require_boolean_expr(__f(__a, __b));
    }
    _Func __f;
    _First __a;
    _Second __b;
  };

  template<class _Func, class _First, class _Second>
  struct _Const_BinaryPredicateConcept {
    void __constraints() {
      __const_constraints(__f);
    }
    void __const_constraints(const _Func& __fun) {
      __function_requires<_BinaryPredicateConcept<_Func, _First, _Second>>();
      __aux_require_boolean_expr(__fun(__a, __b));
    }
    _Func __f;
    _First __a;
    _Second __b;
  };
  // End of _BinaryPredicateConcept


  // Start of _TrivialIteratorConcept implementation:
  template<class _Tp>
  struct _TrivialIteratorConcept {
    void __constraints() {
#     if(TEST_CODE) 
      __function_requires<_DefaultConstructibleConcept<_Tp>>(); // this is commented out in libstdc++ source code
#     endif
      __function_requires<_AssignableConcept<_Tp>>();
      __function_requires<_EqualityComparableConcept<_Tp>>();
#     if(TEST_CODE)
      typedef typename std::iterator_traits<_Tp>::value_type _V;
#     endif
      (void)* __i;
    }
    _Tp __i;
  };

  template<class _Tp>
  struct _Mutable_TrivialIteratorConcept {
    void __constraints() {
      __function_requires<_TrivialIteratorConcept<_Tp>>();
      *__i = *__j;
    }
    _Tp __i, __j;
  };
  // End of _TrivialIteratorConcept


  // Start of _InputIteratorConcept implementation:
  template<class _Tp>
  struct _InputIteratorConcept {
    void __constraints() {
      __function_requires<_TrivialIteratorConcept<_Tp>>();
      typedef typename std::iterator_traits<_Tp>::difference_type _Diff;
#     if(TEST_CODE)
      __function_requires<_SignedIntegerConcept<_Diff>>();
#     endif
      typedef typename std::iterator_traits<_Tp>::reference _Ref;
      typedef typename std::iterator_traits<_Tp>::pointer _Pt;
      typedef typename std::iterator_traits<_Tp>::iterator_category _Cat;
      __function_requires<_ConvertibleConcept<typename std::iterator_traits<_Tp>::iterator_category,
                                              std::input_iterator_tag>>();
      ++__i;
      __i++;
    }
    _Tp __i;
  };
  // End of _InputIteratorConcept


  // Start of _OutputIteratorConcept implementation:
  template<class _Tp, class _ValueT>
  struct _OutputIteratorConcept {
    void __constraints() {
      __function_requires<_AssignableConcept<_Tp>>();
      ++__i;
      __i++;
      *__i++ = __t;
    }
    _Tp __i;
    _ValueT __t;
  };
  // End of _OutputIteratorConcept


  // Start of _ForwardIteratorConcept implementation:
  template<class _Tp>
  struct _ForwardIteratorConcept {
    void __constraints() {
      __function_requires<_InputIteratorConcept<_Tp>>();
      __function_requires<_DefaultConstructibleConcept<_Tp>>();
      __function_requires<_ConvertibleConcept<typename std::iterator_traits<_Tp>::iterator_category,
                                              std::forward_iterator_tag>>();
      typedef typename std::iterator_traits<_Tp>::reference _Ref;
      _Ref __r _IsUnused = *__i;
    }
    _Tp __i;
  };

  template<class _Tp>
  struct _Mutable_ForwardIteratorConcept {
    void __constraints() {
      __function_requires<_ForwardIteratorConcept<_Tp>>();
      *__i++ = *__i;
    }
    _Tp __i;
  };
  // End of _ForwardIteratorConcept


  // Start of _BidirectionalIteratorConcept implementation:
  template<class _Tp>
  struct _BidirectionalIteratorConcept {
    void __constraints() {
      __function_requires<_ForwardIteratorConcept<_Tp>>();
      __function_requires<_ConvertibleConcept<typename std::iterator_traits<_Tp>::iterator_category,
                                              std::bidirectional_iterator_tag>>();
      --__i;
      __i--;
    }
    _Tp __i;
  };

  template<class _Tp>
  struct _Mutable_BidirectionalIteratorConcept {
    void __constraints() {
      __function_requires<_BidirectionalIteratorConcept<_Tp>>();
      __function_requires<_Mutable_ForwardIteratorConcept<_Tp>>();
      *__i-- = *__i;
    }
    _Tp __i;
  };
  // End of _BidirectionalIteratorConcept


  // Start of _RandomAccessIteratorConcept implementation:
  template<class _Tp>
  struct _RandomAccessIteratorConcept {
    void __constraints() {
      __function_requires<_BidirectionalIteratorConcept<_Tp>>();
      __function_requires<_ComparableConcept<_Tp>>();
      __function_requires<_ConvertibleConcept<typename std::iterator_traits<_Tp>::iterator_category,
                                              std::random_access_iterator_tag>>();
      typedef typename std::iterator_traits<_Tp>::reference _Ref;

      __i += __n;
      __i = __i + __n;
      __i = __n + __i;
      __i -= __n;
      __i = __i - __n;
      __n = __i - __j;
      (void)__i[__n];
    }
    _Tp __a, __b;
    _Tp __i, __j;
    typename std::iterator_traits<_Tp>::difference_type __n;
  };

  template<class _Tp>
  struct _Mutable_RandomAccessIteratorConcept {
    void __constraints() {
      __function_requires<_RandomAccessIteratorConcept<_Tp>>();
      __function_requires<_Mutable_BidirectionalIteratorConcept<_Tp>>();
      __i[__n] = *__i;
    }
    _Tp __i;
    typename std::iterator_traits<_Tp>::difference_type __n;
  };
  // End of _RandomAccessIteratorConcept


  // Start of _ContainerConcept implementation:
  template<class _Container>
  struct _ContainerConcept {
    typedef typename _Container::value_type _Value_type;
    typedef typename _Container::difference_type _Difference_type;
    typedef typename _Container::size_type _Size_type;
    typedef typename _Container::const_reference _Const_reference;
    typedef typename _Container::const_pointer _Const_pointer;
    typedef typename _Container::const_iterator _Const_iterator;

    void __constraints() {
      __function_requires<_InputIteratorConcept<_Const_iterator>>();
      __function_requires<_AssignableConcept<_Container>>();
      const _Container __c;
      __i = __c.begin();
      __i = __c.end();
      __n = __c.size();
      __n = __c.max_size();
      __b = __c.empty();
    }
    bool __b;
    _Const_iterator __i;
    _Size_type __n;
  };

  template<class _Container>
  struct _Mutable_ContainerConcept {
    typedef typename _Container::value_type _Value_type;
    typedef typename _Container::reference _Reference;
    typedef typename _Container::iterator _Iterator;
    typedef typename _Container::pointer _Pointer;

    void __constraints() {
      __function_requires<_ContainerConcept<_Container>>();
      __function_requires<_AssignableConcept<_Value_type>>();
      __function_requires<_InputIteratorConcept<_Iterator>>();

      __i = __c.begin();
      __i = __c.end();
      __c.swap(__c2);
    }
    _Iterator __i;
    _Container __c, __c2;
  };
  // End of _ContainerConcept


  // Start of _ForwardContainerConcept implementation:
  template<class _ForwardContainer>
  struct _ForwardContainerConcept {
    void __constraints() {
      __function_requires<_ContainerConcept<_ForwardContainer>>();
      typedef typename _ForwardContainer::const_iterator _Const_iterator;
      __function_requires<_ForwardIteratorConcept<_Const_iterator>>();
    }
  };

  template<class _ForwardContainer>
  struct _Mutable_ForwardContainerConcept {
    void __constraints() {
      __function_requires<_ForwardContainerConcept<_ForwardContainer>>();
      __function_requires<_Mutable_ContainerConcept<_ForwardContainer>>();
      typedef typename _ForwardContainer::iterator _Iterator;
      __function_requires<_Mutable_ForwardIteratorConcept<_Iterator>>();
    }
  };
  // End of _ForwardContainerConcept


  // Start of _ReversibleContainerConcept implementation:
  template<class _ReversibleContainer>
  struct _ReversibleContainerConcept {
    typedef typename _ReversibleContainer::const_iterator _Const_iterator;
    typedef typename _ReversibleContainer::const_reverse_iterator _Const_reverse_iterator;

    void __constraints() {
      __function_requires<_ForwardContainerConcept<_ReversibleContainer>>();
      __function_requires<_BidirectionalIteratorConcept<_Const_iterator>>();
      __function_requires<_BidirectionalIteratorConcept<_Const_reverse_iterator>>();
      const _ReversibleContainer __c;
      _Const_reverse_iterator __i = __c.rbegin();
      __i = __c.rend();
    }
  };

  template<class _ReversibleContainer>
  struct _Mutable_ReversibleContainerConcept {
    typedef typename _ReversibleContainer::iterator _Iterator;
    typedef typename _ReversibleContainer::reverse_iterator _Reverse_iterator;

    void __constraints() {
      __function_requires<_ReversibleContainerConcept<_ReversibleContainer>>();
      __function_requires<_Mutable_ForwardContainerConcept<_ReversibleContainer>>();
      __function_requires<_Mutable_BidirectionalIteratorConcept<_Iterator>>();
      __function_requires<_Mutable_BidirectionalIteratorConcept<_Reverse_iterator>>();
      _Reverse_iterator __i = __c.rbegin();
      __i = __c.rend();
    }
    _ReversibleContainer __c;
  };
  // End of _ReversibleContainerConcept


  // Start of _RandomAccessContainerConcept implementation:
  template<class _RandomAccessContainer>
  struct _RandomAccessContainerConcept {
    typedef typename _RandomAccessContainer::size_type _Size_type;
    typedef typename _RandomAccessContainer::const_reference _Const_reference;
    typedef typename _RandomAccessContainer::const_iterator _Const_iterator;
    typedef typename _RandomAccessContainer::const_reverse_iterator _Const_reverse_iterator;

    void __constraints() {
      __function_requires<_ReversibleContainerConcept<_RandomAccessContainer>>();
      __function_requires<_RandomAccessIteratorConcept<_Const_iterator>>();
      __function_requires<_RandomAccessIteratorConcept<_Const_reverse_iterator>>();
      const _RandomAccessContainer __c;
      _Const_reference __r _IsUnused = __c[__n];
    }
    _Size_type __n;
  };

  template<class _RandomAccessContainer>
  struct _Mutable_RandomAccessContainerConcept {
    typedef typename _RandomAccessContainer::size_type _Size_type;
    typedef typename _RandomAccessContainer::reference _Reference;
    typedef typename _RandomAccessContainer::iterator _Iterator;
    typedef typename _RandomAccessContainer::reverse_iterator _Reverse_iterator;

    void __constraints() {
      __function_requires<_RandomAccessContainerConcept<_RandomAccessContainer>>();
      __function_requires<_Mutable_ReversibleContainerConcept<_RandomAccessContainer>>();
      __function_requires<_Mutable_RandomAccessIteratorConcept<_Iterator>>();
      __function_requires<_Mutable_RandomAccessIteratorConcept<_Reverse_iterator>>();
      _Reference __r _IsUnused = __c[__i];
    }
    _Size_type __i;
    _RandomAccessContainer __c;
  };
  // End of _RandomAccessContainerConcept

  // NOTE: Sequences are inherently mutable, so all of the concepts below will assume the given sequence is mutable.

  // Start of _SequenceConcept implementation:
  template<class _Sequence>
  struct _SequenceConcept {
    typedef typename _Sequence::reference _Reference;
    typedef typename _Sequence::const_reference _Const_reference;

    void __constraints() {
//#     if(TEST_CODE)
      // This function is commented out in libstdc++ since the concept it's checking is from the container.h header file.
      // I'm leaving this commented out as well since this concept already checks for default construction.
      // Not deleting this if DefaultConstructible from container.h is different from _DefaultConstructibleConcept, but if its not defined or is the same then I will delete this commented code.
      //__function_requires<DefaultConstructible<_Sequence>>();
//#     endif
      __function_requires<_Mutable_ForwardContainerConcept<_Sequence>>();
      __function_requires<_DefaultConstructibleConcept<_Sequence>>();
      // _IsUnused is not a function, it is an attribute, so the parentheses are to construct the _Sequence objects __c and __c2
      _Sequence __c _IsUnused(__n, __t), __c2 _IsUnused(__first, __last);
      __c.insert(__p, __t);
      __c.insert(__p, __n, __t);
      __c.insert(__p, __first, __last);
      __c.erase(__p);
      __c.erase(__p, __q);
      _Reference __r _IsUnused = __c.front();
      __const_constraints(__c);
    }
    void __const_constraints(const _Sequence& __c) {
      _Const_reference __r _IsUnused = __c.front();
    }
    typename _Sequence::value_type __t;
    typename _Sequence::size_type __n;
    typename _Sequence::value_type *__first, *__last;
    typename _Sequence::iterator __p, __q;
  };
  // End of _SequenceConcept


  // Start of _FrontInsertionSequenceConcept implementation:
  template<class _FrontInsertionSequence>
  struct _FrontInsertionSequenceConcept {
    void __constraints() {
      __function_requires<_SequenceConcept<_FrontInsertionSequence>>();
      __c.push_front(__t);
      __c.pop_front();
    }
    _FrontInsertionSequence __c;
    typename _FrontInsertionSequence::value_type __t;
  };
  // End of _FrontInsertionSequenceConcept


  // Start of _BackInsertionSequenceConcept implementation:
  template<class _BackInsertionSequence>
  struct _BackInsertionSequenceConcept {
    typedef typename _BackInsertionSequence::reference _Reference;
    typedef typename _BackInsertionSequence::const_reference _Const_reference;

    void __constraints() {
      __function_requires<_SequenceConcept<_BackInsertionSequence>>();
      __c.push_back(__t);
      __c.pop_back();
      _Reference __r _IsUnused = __c.back();
    }
    void __const_constraints(const _BackInsertionSequence& __c) {
      _Const_reference __r _IsUnused = __c.back();
#   if(TEST_CODE)
    }; // in the libstdc++ source code there is a semicollon at the end of this function for some reason. putting this in as experimental code incase it's on purpose.
#   else
    }
#   endif
    _BackInsertionSequence __c;
    typename _BackInsertionSequence::value_type __t;
  };
  // End of _BackInsertionSequenceConcept

} // end of __gnu_cxx:: namespace
// undefining the unused attribute helper macro as it defined was just for this file
#undef _IsUnused


// Start of main concept_check macros which is what is actually used by functions making the concept checks:
// These macros are global since they are "add-ons" for the language
#define __glibcxx_function_requires(...) __gnu_cxx::__function_requires<__gnu_cxx::__VA_ARGS__>();

#define __glibcxx_class_requires(_a, _C) _GLIBCXX_CLASS_REQUIRES(_a, __gnu_cxx, _C);

#define __glibcxx_class_requires2(_a, _b, _C) _GLIBCXX_CLASS_REQUIRES2(_a, _b, __gnu_cxx, _C);

#define __glibcxx_class_requires3(_a, _b, _c, _C) _GLIBCXX_CLASS_REQUIRES3(_a, _b, _c, __gnu_cxx, _C);

#define __glibcxx_class_requires4(_a, _b, _c, _d, _C) _GLIBCXX_CLASS_REQUIRES4(_a, _b, _c, _d, __gnu_cxx, _C);
// End of concept_check macros


#endif // end of Concept_Check.hpp
