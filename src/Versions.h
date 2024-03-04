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


// Start of feature version macros:
#define _DATA_STRUCTURE_LIB_VERSION_         0x010000
#define _DATA_STRUCTURE_UTIL_LIB_VERSION_    0x010000
#define _ARDUINO_UTIL_VERSION_               0x010000
#define _DLIB_UTILITY_VERSION_               0x010000
#define _TYPE_TRAITS_VERSION_                0x010000 // 0x 01(V1) 01(.1) 00(.0) == (V1.1.0)
#define _INITIALIZER_LIST_VERSION_           0x010000
#define _ITERATOR_TRATIS_VERSION_            0x010000
#define _DLIB_ITERATOR_VERSION_              0x010000
#define _DLIB_CONTAINER_VERSION_             0x010000
// End of feature version Macros


// Start of feature release date macros:
#define _DATA_STRUCTURE_LIB__FULL_             202402L // no full release yet
#define _DATA_STRUCTURE_LIB__UTIL_LIB_         202402L // no full release yet
#define _ARDUINO_UTIL_VERSION_                 202403L
#define _DATA_STRUCTURE_LIB__UTILITY_          202402L
#define _DATA_STRUCTURE_LIB__TYPE_TRAITS_      202402L
#define _DATA_STRUCTURE_LIB__INITIALIZER_LIST_ 202402L
#define _DATA_STRUCTURE_LIB__ITERATOR_TRAITS_  202402L
#define _DATA_STRUCTURE_LIB__ITERATOR_         202402L
#define _DATA_STRUCTURE_LIB__CONTAINER_        202402L
// End of feature release data macros:


#endif // End of Versions.h
