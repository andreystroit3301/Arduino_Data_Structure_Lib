/*
  Copyright (C) 2024 Andrey Stroitelev <email=andrey.stroitelev3301@gmail.com> (URL=NA)

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
  Iterator.hpp [V1.0.0] (Template abstract Base Class header file)
    By: Andrey Stroitelev

  Iterator Description:
    ~This is also based on the different iterator type concepts in libstdc++ which is not supported for AVR boards on the Arduino IDE
    ~Instead of using concepts again this uses abstract classes with pure virtual functions to ensure deriving classes make the necessary implementations
    ~All of the base classes are again in the std:: namespace for convenience and uniformity.
    ~This header file defines a set of abstract base classes which are derived when making an iterable container and when making a iterator class for that container.
      *Abstract classes starting with "Legacy" are the base classes for the iterable containers' iterator classes
      *Abstract classes not starting with "Legacy" are the base classes for the iterable containers themselves. The name of the container interface and the iterator interace it supports are the same to provide container functions to work with the corresponding iterator type.
        *example: A container that is forward iterable the container itself will inherit public ForwardIterable<T> and the iterator class for the container will inherit public LegacyForwardIterator<T>

  Iterator Function Descriptions:
    ~
  
  Iterator Use-Case Examples:
    ~
*/
/*
  TO DO:
    *make base versions and specializations for std::next, std::prev, std::advance, and std::distance.
    *make specializations and base versions for std::begin and std::end
*/


// Start of Iterator.hpp
#ifndef _DLIB_ITERATOR_HPP_
#define _DLIB_ITERATOR_HPP_

#ifndef _DLIB_ITERATOR_VERSION_
#define _DLIB_ITERATOR_VERSION_ 0x010000
#endif


#include <Arduino.h>
#include "Iterator_Traits.hpp"
#include "Type_Traits.hpp"
#include "Utility.hpp"
//#include "Container.hpp"


// Start of std:: namespace:
namespace std {

  // Start of iterator category type abstract base class implementations:
  template<class T, class Category = legacy_iterator_tag, class Distance = void, class Pointer = void, class Reference = void>
  class legacy_iterator : public virtual iterator_types<Category, T, Distance, Pointer, Reference> {
    public:
      using iter_type = iterator_traits<legacy_iterator>;

      virtual ~legacy_iterator(void) = default; // required virtual dtor
      //virtual legacy_iterator(void) = default; // default constructible
      virtual legacy_iterator(const legacy_iterator& other) = 0; // required copy ctor
      virtual legacy_iterator(legacy_iterator&& rval) = 0;
      virtual legacy_iterator& operator=(const legacy_iterator& other) = 0; // required copy assignment operator
      virtual const legacy_iterator& operator=(const legacy_iterator& other) const = 0;
      virtual legacy_iterator& operator=(legacy_iterator&& rval) = 0;
      virtual const legacy_iterator& operator=(legacy_iterator&& rval) const = 0;

      template<class T1, class C1, class D1, class P1, class R1>
      friend void swap(legacy_iterator<T1, C1, D1, P1, R1>& lhs, legacy_iterator<T1, C1, D1, P1, R1>& rhs); // swap specialization for iterators

      //virtual typename iter_type::reference_typ operator*() const = 0;

      virtual legacy_iterator& operator++(void) = 0; // prefix increment(++it)
      virtual legacy_iterator&& operator++(int); // postifx increment(it++)
  };

  // Start of legacy_iterator swap() specialization
  template<class T, class C, class D, class P, class R>
  void swap(legacy_iterator<T, C, D, P, R>& lhs, legacy_iterator<T, C, D, P, R>& rhs) {
    legacy_iterator<T, C, D, P, R>* tmp(lhs);
    lhs = rhs;
    rhs = *tmp;
  }
  // End of legacy_iterator swap() specialization

  template<class T, class Category = output_iterator_tag, class Distance = size_t, class Pointer = T*, class Reference = T&>
  class legacy_output_iterator : public virtual legacy_iterator<T, Category, Distance, Pointer, Reference> {
    public:
      using iter_type = iterator_traits<legacy_output_iterator>; // alias for readability
      
      // Required declarations to make the iterator default constructible, copy assignable, copy constructible, move assignable, and move constructible along with any respective cv-qualifed operator overloads.
      virtual ~legacy_output_iterator(void) = default;
      //virtual legacy_output_iterator(void) = default;
      virtual legacy_output_iterator(const legacy_output_iterator& other) override = 0;
      virtual legacy_output_iterator(legacy_output_iterator&& rval) override = 0;
      virtual legacy_output_iterator& operator=(const legacy_output_iterator& other) override = 0;
      virtual const legacy_output_iterator& operator=(const legacy_output_iterator& other) const override = 0;
      virtual legacy_output_iterator& operator=(legacy_output_iterator&& rval) override = 0;
      virtual const legacy_output_iterator& operator=(legacy_output_iterator&& rval) const override = 0;

      virtual typename iter_type::reference_type operator*(void) const = 0; // allows you to write to the iterator. must use *it = val syntax
      
      virtual legacy_output_iterator& operator++(void) override = 0;
      virtual legacy_output_iterator&& operator++(int) override; // returning r-value reference instead of just normal type to avoid abstract return type error
  };

  template<class T, class Category = input_iterator_tag, class Distance = size_t, class Pointer = const T*, class Reference = const T&>
  class legacy_input_iterator : public virtual legacy_iterator<T, Category, Distance, Pointer, Reference> {
    public:
      using iter_type = iterator_traits<legacy_input_iterator>; // alias for readability

      // Required declarations to make the iterator default constructible, copy assignable, copy constructible, move assignable, and move constructible along with any respective cv-qualifed operator overloads.
      virtual ~legacy_input_iterator(void) = default;
      //virtual legacy_input_iterator(void) = default;
      virtual legacy_input_iterator(const legacy_input_iterator& other) override = 0;
      virtual legacy_input_iterator(legacy_input_iterator&& other) override = 0;
      virtual legacy_input_iterator& operator=(const legacy_input_iterator& other) override = 0;
      virtual const legacy_input_iterator& operator=(const legacy_input_iterator& other) const override = 0;
      virtual legacy_input_iterator& operator=(legacy_input_iterator&& rval) override = 0;
      virtual const legacy_input_iterator& operator=(legacy_input_iterator&& rval) const override = 0;

      // operators to read value pointed to by the input iterator(not modifiable)
      virtual typename iter_type::value_type operator*(void) = 0;
      virtual const typename iter_type::value_type operator*(void) const = 0;
      virtual typename iter_type::pointer_type operator->(void) = 0;
      virtual const typename iter_type::pointer_type operator->(void) const = 0; 

      virtual bool operator==(const legacy_input_iterator& other) const = 0;
      virtual bool operator==(legacy_input_iterator&& rval) const = 0;
      virtual bool operator!=(const legacy_input_iterator& other) const = 0;
      virtual bool operator!=(legacy_input_iterator&& rval) const = 0;
      //template<class T1, class C1, class D1, class P1, class R1>
      //constexpr friend bool operator==(const legacy_input_iterator<T1,C1,D1,P1,R1>& lhs, const legacy_input_iterator<T1,C1,D1,P1,R1>& rhs);
      //template<class T1, class C1, class D1, class P1, class R1>
      //constexpr friend bool operator!=(const legacy_input_iterator<T1,C1,D1,P1,R1>& lhs, const legacy_input_iterator<T1,C1,D1,P1,R1>& rhs);

      virtual legacy_input_iterator& operator++(void) override = 0;
      virtual legacy_input_iterator&& operator++(int) override;
  };

  // Start of legacy_input_iterator specializations:
  /*template<class T, class C, class D, class P, class R>
  constexpr bool operator==(const legacy_input_iterator<T,C,D,P,R>& lhs, const legacy_input_iterator<T,C,D,P,R>& rhs) {
    return (lhs.current_pos == rhs.current_pos) && (*(lhs.current_value) == *(rhs.current_value));
    // might change second half to just (lhs.current_value == rhs.current_value)
  }

  template<class T, class C, class D, class P, class R>
  constexpr bool operator!=(const legacy_input_iterator<T,C,D,P,R>& lhs, const legacy_input_iterator<T,C,D,P,R>& rhs) {
    return !(lhs == rhs);
  }*/
  // End of legacy_input_iterator specializations

  template<class T, class Category = forward_iterator_tag, class Distance = size_t, class Pointer = T*, class Reference = T&>
  class legacy_forward_iterator : public virtual legacy_input_iterator<T, Category, Distance, Pointer, Reference> {
    public:
      using iter_type = iterator_traits<legacy_forward_iterator>;

      virtual ~legacy_forward_iterator(void) = default;
      virtual legacy_forward_iterator(void) = default;
      virtual legacy_forward_iterator(const legacy_forward_iterator& other) override = 0;
      virtual legacy_forward_iterator(legacy_forward_iterator&& other) override = 0;
      virtual legacy_forward_iterator& operator=(const legacy_forward_iterator& other) override = 0;
      virtual const legacy_forward_iterator& operator=(const legacy_forward_iterator& other) const override = 0;
      virtual legacy_forward_iterator& operator=(legacy_forward_iterator&& rval) override = 0;
      virtual const legacy_forward_iterator& operator=(legacy_forward_iterator&& rval) const override = 0;

      static_assert(is_convertible<typename iter_type::reference_type, typename iter_type::value_type>(), "reference type must be convertible to value type in forward iterator!");
      virtual typename iter_type::reference_type operator*(void) const override = 0;
      virtual typename iter_type::pointer_type operator->(void) const override = 0;

      virtual legacy_forward_iterator& operator++(void) override = 0;
      virtual legacy_forward_iterator&& operator++(int) override = 0;

      virtual bool operator==(const legacy_forward_iterator& other) const override = 0;
      virtual bool operator==(legacy_forward_iterator&& rval) const override = 0;
      virtual bool operator!=(const legacy_forward_iterator& other) const override = 0;
      virtual bool operator!=(legacy_forward_iterator&& rval) const override = 0;
  };

  template<class T, class Category = bidirectional_iterator_tag, class Distance = ptrdiff_t, class Pointer = T*, class Reference = T&>
  class legacy_bidirectional_iterator : public virtual legacy_forward_iterator<T, Category, Distance, Pointer, Reference> {
    public:
      virtual ~legacy_bidirectional_iterator(void) = default;
  };

  template<class T, class Category = random_access_iterator_tag, class Distance = ptrdiff_t, class Pointer = T*, class Reference = T&>
  class legacy_random_access_iterator : public virtual legacy_bidirectional_iterator<T, Category, Distance, Pointer, Reference> {
    public:
      virtual ~legacy_random_access_iterator(void) = default;
  };

  template<class T, class Category = contiguous_iterator_tag, class Distance = ptrdiff_t, class Pointer = T*, class Reference = T&>
  class legacy_contiguous_iterator : public virtual legacy_random_access_iterator<T, Category, Distance, Pointer, Reference> {
    public:
      virtual ~legacy_contiguous_iterator(void) = default;
  };
  // End of iterator category type base classes.


  // Start of container iterator interface base classes:
  template<class T>
  class output_iterable {
    public:

      virtual ~output_iterable(void) = default;

      virtual void insert(legacy_output_iterator<T>& it) = 0;
      virtual legacy_output_iterator<T> remove(legacy_output_iterator<T>& it) = 0;

      virtual inline legacy_output_iterator<T> begin(void) const noexcept = 0;
      virtual inline legacy_output_iterator<T> end(void) const noexcept = 0;
      virtual inline legacy_output_iterator<T> cbegin(void) const noexcept = 0;
      virtual inline legacy_output_iterator<T> cend(void) const noexcept = 0;

    protected:
  };
  // End of container iterator interface classes

} // End of std:: namespace


// Start of specializations and base versions of global iterator functions:
// End of specializations and base versions of global iterator functions


#endif
