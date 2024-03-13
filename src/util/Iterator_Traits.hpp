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
  Iterator_Traits.hpp [V1.0.0] (Template abstract Base Class header file)
    By: Andrey Stroitelev

  Iterator_Traits Description:
    ~This is a type trait class to provide a uniform interface of properties between the Iterator.hpp base classes.
    ~Every Iterator base class will support these type traits as they apply to the legacy_iterator class
    ~This is based on the layout of the libstdc++ iterator library, but it is modified as there is no requires or concept keyword in the arduino IDE currently.
    ~Instead of using concepts this library uses abstract classes and inheritance to achieve a similar outcome
    ~These classes are still defined in the std:: namespace for convenience and to keep things uniform
    ~This library also utilitzes iterator_traits which is deprecated along with the iterator base class that holds the type aliases in the actual C++ STL library.
      *I'm using these deprecated structs and meta-functions due to the lack of concepts, and those were not deprecated in c++11-c++14 which is what the arduino IDE compiles with.
*/



// Start of Iterator_Traits.hpp
#ifndef _ITERATOR_TRAITS_HPP_
#define _ITERATOR_TRAITS_HPP_

#ifndef _ITERATOR_TRAITS_VERSION_
#define _ITERATOR_TRAITS_VERSION_ 0x010000
#endif


#include <Arduino.h>
#include "Utility.hpp"
#include "Type_Traits.hpp"


// Start of std:: namespace:
namespace std {


  // Start of Iterator category tags:
  struct legacy_iterator_tag { };

  struct output_iterator_tag : public legacy_iterator_tag { };

  struct input_iterator_tag : public legacy_iterator_tag { };

  struct forward_iterator_tag : public input_iterator_tag { };

  struct bidirectional_iterator_tag : public forward_iterator_tag { };

  struct random_access_iterator_tag : public bidirectional_iterator_tag { };

  struct contiguous_iterator_tag : public random_access_iterator_tag { };

  struct single_pass_tag { };
  struct multi_pass_tag { };
  // End of Iterator category tags


  // Start of iterator base class and iterator_traits implementation:
  template<class Category, class T, class Distance = ptrdiff_t, class Pointer = T*, class Reference = T&>
  struct _iterator { // this base class is purely to give iterators common type alias names for use in algorithms
    using value_type = T;
    using difference_type = Distance;
    using pointer_type = Pointer;
    using reference_type = Reference;
    using iterator_category = Category; // is an iterator category tag from above
  };

  template<class _Iter> // gets traits from given iterator class
  struct iterator_traits : public _iterator<typename _Iter::iterator_category,
                                            typename _Iter::value_type,
                                            typename _Iter::difference_type,
                                            typename _Iter::pointer_type,
                                            typename _Iter::reference_type> { };
  
  template<class T> // gets traits from pointer being used as an iterator
  struct iterator_traits<T*> : public _iterator<random_access_iterator_tag, T> { };

  template<class T> // gets traits from const pointer being used as an iterator
  struct iterator_traits<const T*> : public _iterator<random_access_iterator_tag, T, ptrdiff_t, const T*, const T&> { };
  // End end of iterator type base classe and iterator_traits type retriever


  // Start of incrementable_traits pre-declarations:
  // End of incrementable_traits pre-declarations


  // Start of single/multi-pass checker implementation:
  template<class Category, class = void>
  struct increment_algorithm {
    using algorithm = single_pass_tag;
  };

  template<class Category>
  struct increment_algorithm<Category, typename enable_if<
                                                          __or_<
                                                                is_same<Category, forward_iterator_tag>(), is_base_of<forward_iterator_tag, Category>() 
                                                                >::value, void>::type> : public true_type {
    using algorithm = multi_pass_tag;
  };
  // End of algorithm type checker implementation


  // Start of common iterator type alias templates:
  template<class _Iter>
  using iter_value_t = typename iterator_traits<remove_cvref_t<_Iter>>::value_type;

  template<class _Iter>
  using iter_reference_t = decltype(*(declval<_Iter&>()));

  template<class _Iter> 
  using iter_const_reference_t = common_reference_t<const iter_value_t<_Iter>&&,
                                                    iter_reference_t<_Iter>>;

  template<class _Iter>
  using iter_difference_t = typename iterator_traits<remove_cvref_t<_Iter>>::difference_type;

  template<class _Iter>
  struct __iter_rvalue_ref {
    static_assert(!same_as<void, iter_move(declval<_Iter&>())>, 
      "std::iter_move(Iter&) should return a valid type that isn't void. Iterators must be movable.");
    
    using type = decltype(iter_move(declval<_Iter&>()));
  };

  template<class _Iter>
  using iter_rvalue_reference_t = typename __iter_rvalue_ref<_Iter>::type;

  template<class _Iter>
  using iter_common_reference_t = common_reference_t<iter_reference_t<_Iter>,
                                                     iter_value_t<_Iter>&>;

  template<class _It>
  using iter_category_t = typename iterator_traits<remove_cvref_t<_It>>::iterator_category;

  template<class _It>
  using iter_algorithm_t = typename increment_algorithm<_It>::algorithm;
  // End of common iterator type alias templates

  
  // Start of is_incrementable checker implementation: (implementing as a meta-function, so no assertions are made)
  template<class T, class = void>
  struct is_weakly_incrementable : public false_type { };

  template<class T> // just checks for pre and post increment operator
  struct is_weakly_incrementable<T, typename enable_if<__and_< is_convertible<T&, decltype(++(declval<T>()))>,
                                                                is_convertible<T, decltype(declval<T>()++)>>::value,
                                                                void>::type> : public true_type { };
  
  template<class _Tp, class = void>
  struct is_incrementable : public false_type { };

  template<class _Tp> // for forward iterators and up as they are multi-pass and have more requirements for increment behavior
  struct is_incrementable<_Tp, typename enable_if<__and_<
                                                  is_weakly_incrementable<_Tp>,
                                                  equality_comparable<_Tp>,
                                                  bool_constant<(*(declval<_Tp>()) == *(declval<_Tp>())) && (*(++(declval<_Tp>())) == *(++(declval<_Tp>())))>>::value, // must be equal initially, and must be equal after they both increment once
                                                  void>::type> : public true_type { };
  // End of incrementable checker implementation


  // Start of iterator dereference checker implementation:
  template<class _Iter, class = void>
  struct dereferenceable_iter : public false_type { };

  template<class _Iter>
  struct dereferenceable_iter<_Iter, typename enable_if<__or_<
                                        is_convertible<decltype(*(declval<_Iter>())), typename iterator_traits<_Iter>::value_type&>,
                                        is_convertible<decltype(*(declval<_Iter>())), typename iterator_traits<_Iter>::value_type>,
                                        is_convertible<decltype(*(declval<_Iter>())), typename iterator_traits<_Iter>::reference_type>>::value,
                                        void>::type> : public true_type { };
  // End of iterator dereference checker implementation


  // Start of iterator pointer to member checker implementation:
  template<class _Iter, class = void>
  struct pointable_iter : public false_type { };

  template<class _Iter>
  struct pointable_iter<_Iter, typename enable_if<__and_<
                                                     dereferenceable_iter<_Iter>,
                                                     __or_<is_convertible<decltype(declval<_Iter>().operator->()), typename iterator_traits<_Iter>::pointer_type>,
                                                           is_convertible<decltype(declval<_Iter>().operator->()), typename iterator_traits<_Iter>::pointer_type&>,
                                                           is_convertible<decltype(declval<_Iter>().operator->()), typename iterator_traits<_Iter>::value_type*>,
                                                           is_convertible<decltype(declval<_Iter>().operator->()), typename iterator_traits<_Iter>::value_type*&>>>::value,
                                                           void>::type> : public true_type { };
  // End of iterator pointer to member checker implementation


  // Start of writable iterator checker implementation:
  template<class _It, class = void>
  struct writable_iter : public false_type { };

  template<class _It> // might not use this
  struct writable_iter<_It, typename enable_if<__or_<
                                              is_assignable<decltype((*declval<_It>())++), iter_value_t<_It>>,
                                              __and_<
                                                     is_reference<iter_reference_t<_It>>,
                                                     is_same<typename iterator_traits<_It>::reference_type, typename iterator_traits<_It>::value_type&>>>::value,
                                              void>::type> : public true_type { };
  // End of writable iterator checker implementation


  // Start of readable iterator checker implementation:
  template<class _It, class = void>
  struct readable_iter : public false_type { };
  // End of readable iterator checker implementation


  // Start of has_signed_difference_type type checker implementation:
  template<class _Iter, class = void>
  struct has_signed_difference_type : public false_type { };

  template<class _Iter>
  struct has_signed_difference_type<_Iter, typename enable_if<
                                          is_signed_integral<typename iterator_traits<_Iter>::difference_type>::value,
                                          void>::type> : public true_type { };
  // End of has_signed_difference_type type checker implementation


  // Start of *it++ expression checker implementation:
  // End of *it++ expression checker implementation


  // Start of has_valid_iterator_category implementation:
  template<class _It, class = void>
  struct has_valid_iter_category : public false_type { };

  template<class _It>
  struct has_valid_iter_category<_It, typename enable_if<__and_<
                                                            __not_<is_void<typename iterator_traits<_It>::iterator_category>>,
                                                            is_base_of<legacy_iterator_tag, typename iterator_traits<_It>::iterator_category>,
                                                            __not_<is_same<legacy_iterator_tag, typename iterator_traits<_It>::iterator_category>>,
                                                            __not_<is_base_of<contiguous_iterator_tag, typename iterator_traits<_It>::iterator_category>>>::value,
                                                            void>::type> : public true_type { };
  // End of has_valid_iterator_category implementation


  // Start of has_valid_value_type implementation:
  // End of has_valid_value_type implementation


  // Start of has_valid_reference_type implementation:
  // End of has_valid_reference_type implementation


  // Start of has_valid_pointer_type implementation:
  // End of has_valid_pointer_type implementation


  // Start of valid types checker implementation:
  template<class _It, class = void>
  struct has_valid_iter_types : public false_type { };

  template<class _It> // incomplete
  struct has_valid_iter_types<_It, typename enable_if<__and_<
                                                      has_signed_difference_type<_It>,
                                                      has_valid_iter_category<_It>>::value,
                                                      void>::type> : public true_type { };
  // End of valid types checker implementation


  // Start of const iterator checker implementation:
  // End of const iterator checker implementation


  // Start of category specific requirement checker implementations:
  // End of category specific requirement checker implementations

} // End of std:: namespace


#endif // End of Iterator_Traits.hpp
