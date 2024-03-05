# Arduino_Data_Structure_Lib

- INCOMPLETE/WIP

This is a library for the Arduino IDE(V2+) that adds a bunch of templated data structures, functions, and meta-functions from libstdc++ which are not natively available/supported for AVR boards on the Arduino IDE.

This library was developed in Arduino IDE V2.3, so any users should have Arduino IDE V2.3 or newer to make sure the library works properly.

## Description

This library adds an assortment of data structures from the C++ standard template library. This adds most of the well known data structures including but not limited to:
  - std::vector
  - std::map
  - std::array
  - std::forward_list
  - std::list
  - std::pair
  - std::tuple
  - std::set
  - std::queue
  - std::dequeue
  - std::stack

These data structures will use dynamic memory allocation the same way they do in libstdc++. Some containers like std::array do not use dynamic memory allocation since they have a set max size, so these containers
will store data externally via a pointer to a reference. Since the Arduino IDE is using C++11 with some C++14 support there is no concept or requires keywords, so to get around this I implemented the concepts as
abstract pure virtual classes and static assertions which will force the inheriting class to implement all of the required functions and constraints to meet the necessary requirements and constraints such as
being default constructible, copy constructible/assignable, incrementable, is_integral, etc.

Also this library will throw a list of compiler warnings(if you have warnings enabled in the Arduin IDE). These warnings can be ignored. The warnings given are for variable templates which only started being
properly supported in C++14, but the Arduino IDE runs on C++11 so a warning is given. The template variables(which are meta-function helpers) still work correctly and as intended, and they may become officially
supported in future versions of the Arduino IDE which would get rid of the warnings. There is currently no way to add preprocessor directives to ignore specific warnings in the Arduino IDE. If the warnings are
still a bother I may add a seperate version that replaces all of the helper variable instances with their standard meta-function syntax (instead of is_base_of_v<T> you would be required to use is_base_of<T>::value)

### TO DO LIST:
- [ ] Finish Iterator abstract base classes in util/Iterator.hpp
- [ ] Finish Container abstract base class in util/Iterator.hpp
- [ ] Add new test functions to example sketch
- [ ] update keywords.txt with available functions, classes, and constants
- [ ] Finish testing currently implemented functions, classes, and metafunctions
- [ ] Create directory for the container classes implemented in this library
- [ ] Create data structures
- [ ] Add global iterator functions/algorithms
- [ ] Add container utility functions with their respective specializations

### NOTICE/CAUTION:
* This library is made in the Arduino IDE, so I'm not sure if it will work in other IDEs like PlatformIO
* This library is specifically make for AVR boards like the UNO and Mega2560 which dont natively include/support the c++ standard template libraries.
  - Some Microcontrollers supported by the Arduino IDE already have the STL libraries available such as ESP32 board.
* The data structures in this library use dynamic memory allocation which is usually frowned upon when programming microcontrollers or embedded systems.
  - Use caution when using the data structures as they can cause you to run out of memory, and they can cause unrecoverable heap fragmentation depending on what you're storing.
    - The containers that use dynamic memory allocation all have code to properly manage that dynamic memory to avoid memory leaks, so if you make your own container class that dynamically allocates memory make sure its properly deleted.
      - To avoid heap fragmentation try to keep data structures in local scopes so the data is all in one chunck of memory that gets deleted together. This will help prevent empty memory blocks that are too small for the next bit of data to be allocated.
