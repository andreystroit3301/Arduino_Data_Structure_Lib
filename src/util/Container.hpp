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
  Container.hpp [V1.0.0] (Template abstract Base Class header file)
    By: Andrey Stroitelev

  Container Description:
    ~This is an abstract base class which is derived when making a data structure class.
    ~This is based around the Container class in the c++ std library which is inherited by all of the std data structures such as std::array, std::vector, std::map, etc.
    ~This class can't be used on it's own as it is abstract with virtual functions that arent defined.
    ~This class is essentially like a java interface so it lays out some common functions that are required and used by all container classes.
      *for example every container class will have a remove() function which will remove an element it's storing, but every data structure needs it's own implementation based on how the structure stores data. This also means no container sub class can be made without implementing the remove function.
    ~Every container sub class must have all of these functions otherwise you will get an error. This also makes it so you don't need to learn all of the different functions from the different data structures you're using as they will all have common functions that do the same thing but in a different way.
    ~This code is in the std:: namespace for convenience and uniformity

  Container Function Descriptions:
    ~
  
  Container Use-Case Examples:
    ~
*/


// Start of Container.hpp
#ifndef _DLIB_CONTAINER_HPP_
#define _DLIB_CONTAINER_HPP_

#ifndef _DLIB_CONTAINER_VERSION_
#define _DLIB_CONTAINER_VERSION_ 0x010000
#endif


#include <Arduino.h>
#include "Utility.hpp"
#include "Type_Traits.hpp"
#include "Initializer_List.hpp"


// Start of std:: namespace:
namespace std {



} // End of std:: namespace


#endif // End of Container.hpp
