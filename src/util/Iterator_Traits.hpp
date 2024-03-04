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
  Iterator_Traits.hpp [V1.0.0] (Template abstract Base Class header file)
    By: Andrey Stroitelev

  Iterator_Traits Description:
    ~This is a type trait class to provide a uniform interface of properties between the Iterator.hpp base classes.
    ~Every Iterator base class will support these type traits as they apply to the legacy_iterator class
    ~This is based on the layout of the libstdc++ iterator library, but it is modified as there is no requires or concept keyword in the arduino IDE currently.
    ~Instead of using concepts this library uses abstract classes and inheritance to achieve a similar outcome
    ~These classes are still defined in the std:: namespace for convenience and to keep things uniform
*/


// Start of Iterator_Traits.hpp
#ifndef _ITERATOR_TRAITS_HPP_
#define _ITERATOR_TRAITS_HPP_

#ifndef _ITERATOR_TRAITS_VERSION_
#define _ITERATOR_TRATIS_VERSION_ 0x010000 
#endif


#include <Arduino.h>
#include "Type_Traits.hpp"


// Start of std:: namespace:
namespace std {

  // Iterator type category tag declarations:
  struct legacy_iterator_tag { };

  struct output_iterator_tag { };

  struct input_iterator_tag { };

  struct forward_iterator_tag : public input_iterator_tag { };

  struct bidirectional_iterator_tag : public forward_iterator_tag { };

  struct random_access_iterator_tag : public bidirectional_iterator_tag { };

  struct contiguous_iterator_tag : public random_access_iterator_tag { };
  // End of iterator type category tags


  // Start of iterator_types base class implementation:
  template<class Category, class T, class Distance = ptrdiff_t, class Pointer = T*, class Reference = T&>
  struct iterator_types {
    using value_type = T;
    using difference_type = Distance;
    using pointer_type = Pointer;
    using reference_type = Reference;
    using iterator_category = Category;
  };
  // End of iterator_types base class implementation


  // Start of iterator_traits base class specialization for iterator_types implementation:
  template<class Iter>
  struct iterator_traits : public iterator_types<typename Iter::iterator_category,
                                                 typename Iter::value_type,
                                                 typename Iter::difference_type,
                                                 typename Iter::pointer_type,
                                                 typename Iter::reference_type> { };

  template<class T>
  struct iterator_traits<T*> : public iterator_types<random_access_iterator_tag, T> { };

  template<class T>
  struct iterator_traits<const  T*> : public iterator_types<random_access_iterator_tag, T, ptrdiff_t, const T*, const T&> { };
  // End of iterator_traits implementation

} // End of std:: namespace


#endif // End of Iterator_Traits.hpp
