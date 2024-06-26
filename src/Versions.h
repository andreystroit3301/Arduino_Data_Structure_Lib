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
  Versions.h (version meta data file)
    By: Andrey Stroitelev

  ~This is a simple file to store all of the macros for version numbers and working implement dates.
  ~The version macros expand to a 6-digit hexadecimal number.
    *Examples: 0x0F0F0F == V15.15.15 || 0x01030F == V1.3.15
  ~The feature macros are just simple integer literals that denote the year and the month that the corresponding feature was included in the working version of this library.
    *The date is essentially when the last stable working version of the feature was released. There may be newer versions available, but if the data doesn't correspond with the version then its considered unstable.
    *Once a new stable working version of the feature is released, the date is updated.
    *Example: for 2024-02 it would just be 202402L.
  ~These macros are used to keep track of what exact versions are being used by your project. If someone else tries to run your code you can have preprocessor checks to make sure they have the proper library and function versions.
  ~Deprecated features will also be listed here until they are completely removed, but for convenience I will most likely retain deprecated code.
  ~All of these macros are also defined in their respective files, so If you use just a single file from the library the macros will still be available to you.
*/


// Start of Versions.h
#ifndef _DATA_STRUCTURE_LIB_VERSIONS_H_
#define _DATA_STRUCTURE_LIB_VERSIONS_H_


#define _DATA_STRUCTURE_LIB__VERSIONS_ 202403L // last update to this file


// Example of how version numbers are written: 0x01040A == 0x01(V1) -> 04(.4) -> 0A(.10) == V1.4.10

// Start of feature version macros: (6-digit hexadecimal)
#define _DATA_STRUCTURE_LIB_VERSION_         0x010000 
#define _DATA_STRUCTURE_UTIL_LIB_VERSION_    0x010000
#define _DLIB_CONFIG_VERSION_                0x010000
#define _ARDUINO_UTIL_VERSION_               0x010000
#define _DLIB_MOVE_VERSION_                  0x010000
#define _DLIB_STDEXCEPT_VERSION_             0x010000
#define _DLIB_UTILITY_BASE_VERSION_          0x010000
#define _DLIB_DEBUGGING_VERSION_             0x010000
#define _DLIB_INVOKE_VERSION_                0x010000
#define _RANGE_ACCESS_VERSION_               0x010000
#define _ALGORITHM_BASE_VERSION_             0x010000
#define _DLIB_ARRAY_VERSION_                 0x010000
#define _DEBUG_ARRAY_VERSION_                0x010000
#define _DLIB_PAIR_VERSION_                  0x010000
#define _DLIB_TUPLE_VERSION_                 0x010000
#define _DLIB_UTILITY_VERSION_               0x010000
#define _TYPE_TRAITS_VERSION_                0x010000 
#define _DLIB_CONCEPTS_VERSION_              0x010000 // not sure if I will actually implement this file to keep checker meta-functions seperate
#define _CONCEPT_CHECK_VERSION_              0x010000
#define _INITIALIZER_LIST_VERSION_           0x010000
#define _ITERATOR_TRAITS_VERSION_            0x010000
#define _ITERATOR_FUNCS_VERSION_             0x010000
#define _DLIB_ITERATOR_VERSION_              0x010000
#define _DLIB_ALLOCATOR_VERSION_             0x010000
#define _DLIB_ALLOC_TRAITS_VERSION_          0x010000
#define _DLIB_VECTOR_VERSION_                0x010000
#define _DLIB_VECTOR_DEF_VERSION_            0x010000
#define _DEBUG_VECTOR_VERSION_               0x010000
// End of feature version Macros


// Start of feature release date macros: (integer long value)
// NOTE: These are all arbitrary and will all be updated to whatever date I make the first release of this library
#define _DATA_STRUCTURE_LIB__FULL_             202405L
#define _DATA_STRUCTURE_LIB__UTIL_LIB_         202405L
#define _DATA_STRUCTURE_LIB__CONFIG_           202405L
#define _DATA_STRUCTURE_LIB__ARDUINO_UTIL_     202405L
#define _DATA_STRUCTURE_LIB__MOVE_             202405L
#define _DATA_STRUCTURE_LIB__STDEXCEPT_        202405L
#define _DATA_STRUCTURE_LIB__UTILITY_BASE_     202405L
#define _DATA_STRUCTURE_LIB__DEBUGGING_        202405L
#define _DATA_STRUCTURE_LIB__INVOKE_           202405L
#define _DATA_STRUCTURE_LIB__RANGE_ACCESS_     202405L
#define _DATA_STRUCTURE_LIB__ALGORITHM_BASE_   202405L
#define _DATA_STRUCTURE_LIB__ARRAY_            202405L
#define _DATA_STRUCTURE_LIB__DEBUG_ARRAY_      202405L
#define _DATA_STRUCTURE_LIB__PAIR_             202405L
#define _DATA_STRUCTURE_LIB__TUPLE_            202405L
#define _DATA_STRUCTURE_LIB__UTILITY_          202405L
#define _DATA_STRUCTURE_LIB__TYPE_TRAITS_      202405L
#define _DATA_STRUCTURE_LIB__CONCEPTS_         202405L // will most likely remove
#define _DATA_STRUCTURE_LIB__INITIALIZER_LIST_ 202405L
#define _DATA_STRUCTURE_LIB__ITERATOR_TRAITS_  202405L
#define _DATA_STRUCTURE_LIB__ITERATOR_FUNCS_   202405L
#define _DATA_STRUCTURE_LIB__ITERATOR_         202405L
#define _DATA_STRUCTURE_LIB__CONCEPT_CHECK_    202405L
#define _DATA_STRUCTURE_LIB__ALLOCATOR_        202405L
#define _DATA_STRUCTURE_LIB__ALLOCATOR_TRAITS_ 202405L
#define _DATA_STRUCTURE_LIB__VECTOR_           202405L
#define _DATA_STRUCTURE_LIB__VECTOR_DEF_       202405L
#define _DATA_STRUCTURE_LIB__DEBUG_VECTOR_     202405L
// End of feature release data macros:


#endif // End of Versions.h
