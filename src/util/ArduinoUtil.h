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
  ArduinoUtil.h [V1.0.0]  (Utility function header file)
    By: Andrey Stroitelev

  ArduinoUtil Description:
    ~This is a simple collection of helpful functions for interacting with the arduino's hardware.
    ~These functions are in a seperate file from the regular utility.hpp file in this library since these functions arent templates, and these are not based on existing libstdc++ functions.
    ~Some of these functions are optimized versions of builtin arduino functions to reduce the time it takes to execute those functions.
    ~Everything here will be under the util:: namespace to seperate it from the built-in arduino IDE global functions.
    ~Since these functions arent templates this will be implemented with a .h/.cpp file pair instead of a single .hpp file, so this file only has declarations.
    ~There will be alot of comments throughout these functions since they use very low level code to directly control the arduino hardware which makes it hard to read. (the built-in arduino functions are very abstracted to avoid such complexity at the cost of preformance)

  ArduinoUtil Functions:
    ~ freeMemory(void) -> this function returns the amount of free memory between the stack and the heap to allow you to keep track of memory usage during runtime.(NOTE: this function cant see any deallocated memory fragments in the heap, so the returned value is usually lower than the actual amount of free memory)

  ArduinoUtil Uses:
    ~
*/


// Start of ArduinoUtil.h
#ifndef _ARDUINO_UTIL_H_
#define _ARDUINO_UTIL_H_

#ifndef _ARDUINO_UTIL_VERSION_
#define _ARDUINO_UTIL_VERSION_ 0x010000
#endif


#include <Arduino.h>


// Start of externally defined variables: (required to be in global scope to work)
#ifdef __arm__
extern "C" char* sbrk(int incr);
#else
extern char *__brkval;
#endif
// End of externally defined variables


// Start of util:: namespace
namespace util {

  // Start of freeMemory() declarations:
  extern int freeMemory(void);
  // End of freeMemory()

} // End of util:: namespace


#endif // End of ArduinoUtil.h
