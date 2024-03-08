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
  ArduinoUtil.cpp [V1.0.0]  (ArduinoUtil source file for definitions)
    By: Andrey Stroitelev

  ~This is the ArduinoUtil source file to define the functions declared in the ArduinoUtil.h header file
*/


#include "ArduinoUtil.h"


// Start of util:: namespace
namespace util {

  // Start of freeMemory() definition:
  extern int freeMemory(void) {
    char top;
#ifdef __arm__
    return &top - reinterpret_cast<char*>(sbrk(0));
#elif defined(CORE_TEENSY) || (ARDUINO > 103 && ARDUINO != 151)
    return &top - __brkval;
#else
    return __brkval ? &top - __brkval : &top - __malloc_heap_start;
#endif
  }
  // End of freeMemory() definition

} // End of util namespace
