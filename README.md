# Arduino_Data_Structure_Lib

- INCOMPLETE/WIP

This is a library for the Arduino IDE(V2+) that adds a bunch of templated data structures, functions, and meta-functions from libstdc++ which are not natively available/supported for AVR boards on the Arduino IDE.

This library was developed in Arduino IDE V2.3, so any users should have Arduino IDE V2.3 or newer to make sure the library works properly.

## Description

This library adds most of the data structures/containers found in the C++ STL library. These data structures will dynamically allocate and deallocate data, so they will function as they do for regular C++ 
programming. I may eventually add static versions of the containers that have a constant max size for situations where you have very little SRAM like in a large program, or for when you're using an arduino
board that has very low memory to begin with like the non-IOT boards and compact boards like the Arduino Nano or Arduino Pro Mini. I also added a large chunk of meta-functions from the <utility> and <type_traits>
headers from libstdc++ since some will be required for the container implementations, and I just added some other meta-functions just because I think they could be useful when using the data structures.

This adds most of the well known data structures including but not limited to:
  - std::vector
  - std::map
  - std::array
  - std::list
  - std::pair
  - std::tuple
  - std::set
  - std::queue
  - std::stack

These data structures will use dynamic memory allocation the same way they do in libstdc++. Some containers like std::array do not use dynamic memory allocation since they have a set max size, so these containers
should be used over the dynamic containers when you have low memory to work with. Since the Arduino IDE is using C++11 with some C++14 support there is no concept or requires keywords, so to get around this I
implemented the concepts as bool_constant trait checking meta-functions that will be static_asserted at compile time by the base class using CRTP which will force the inheriting class to implement all of the 
required functions/operators to meet the necessary requirements and constraints such as being default constructible, copy constructible/assignable, incrementable, is_integral, etc,. The Type_Traits.hpp and 
Utility.hpp files dont have the exact same functions as they do in libstdc++, so some type_traits functions are in Utility.hpp and some utility functions are in type_traits.hpp. I also added some of my own 
functions which are mostly concepts being reimplemented as bool_constant metafunctions which will just do a check for a static_assertion somewhere to give the actual compile error. I may eventually make static 
versions of the implemented containers to eliminate the risk of memory leaks and other issues for anyone who wants to use this library for one of the arduinos with really low memory like the UNO. I'm developing 
this library directly in the Arduino IDE, and I'm using an Arduino MEGA(8k SRAM) for functional testing.


Also this library will throw a list of compiler warnings(if you have warnings enabled in the Arduin IDE). These warnings can be ignored. The warnings given are for variable templates which only started being
properly supported in C++14, but the Arduino IDE runs on C++11 so a warning is given. The template variables(which are meta-function helpers) still work correctly and as intended, and they may become officially
supported in future versions of the Arduino IDE which would get rid of the warnings. There is currently no way to add preprocessor directives to ignore specific warnings in the Arduino IDE. If the warnings are
still a bother I may add a seperate version that replaces all of the helper variable instances with their standard meta-function syntax (instead of is_base_of_v<T> you would be required to use is_base_of<T>::value)

## MY TO DO LIST:
- [X] ~~Finish Iterator abstract base classes in util/Iterator.hpp~~(Using CRTP to assert requirements at compile time instead of using interface classes to remove virtual function overhead)
- [X] ~~Finish Container abstract base class in util/Iterator.hpp~~(Same as above)
- [ ] Create CRTP checker class for the different required iterator operators and expressions
    + The requirements for iterators wont be exactly the same as in libstdc++, but the main operators will be required along with the necessary expressions needing to be valid
- [ ] Create the main CRTP style base classes for the different iterator types
- [ ] Create CRTP checker classes for the different required functions for containers
- [ ] Create the main CRTP base class for containers
- [ ] Create general CRTP meta-functions for trait checks such as is_incrementable etc. in util/Type_Traits.hpp
- [ ] Finish adding needed functions and other quality of life functions to util/Utility.hpp
- [ ] Finish adding needed functions and other quality of life functions to util/Type_Traits.hpp
- [ ] Add new test functions to example sketch
- [ ] update keywords.txt with available functions, classes, and constants
- [ ] Finish testing currently implemented functions, classes, and metafunctions
- [ ] Create directory for the container classes implemented in this library
- [ ] Create data structures
    - [ ] implement std::pair
    - [ ] implement std::stack
    - [ ] implement std::vector
    - [ ] implement std::list
    - [ ] implement std::forward_list
    - [ ] implement std::queue
    - [ ] implement std::priority_queue
    - [ ] implement std::dequeue
    - [ ] implement std::array
    - [ ] implement std::map
    - [ ] implement std::unordered_map
    - [ ] implement std::multimap _(might not implement)_
    - [ ] implement std::unordered_multimap _(might not implement)_
    - [ ] implement std::flat_map _(might not implement)_
    - [ ] implement std::flat_multimap _(might not implement)_
    - [ ] implement std::set
    - [ ] implement std::unordered_set
    - [ ] implement std::multiset _(might not implement)_
    - [ ] implement std::unordered_multiset _(might not implement)_
    - [ ] implement std::flat_set _(might not implement)_
    - [ ] implement std::flat_multiset _(might not implement)_
    - [ ] implement std::span _(might not implement)_
    - [ ] implement std::mdspan _(might not implement)_
- [ ] Need to add global iterator functions/algorithms
- [ ] Need to add container utility functions/algorithms with their respective specializations
- [ ] Need to add arduino utility functions for faster IO read/write
- [ ] Possibly will add internal comparator control functions
- [ ] Need to add register/port control functions for abstraction
- [ ] Possibly will add functions for faster pinMode _(if possible)_

## NOTICE/CAUTION:
* This library is made in the Arduino IDE, so I'm not sure if it will work in other IDEs like PlatformIO
* This library is specifically make for AVR boards like the UNO and Mega2560 which dont natively include/support the c++ standard template libraries.
  - Some Microcontrollers supported by the Arduino IDE already have the STL libraries available such as ESP32 board.
* The data structures in this library use dynamic memory allocation which is usually frowned upon when programming microcontrollers or embedded systems.
  - Use caution when using the data structures as they can cause you to run out of memory, and they can cause unrecoverable heap fragmentation depending on what you're storing.
    - The containers that use dynamic memory allocation all have code to properly manage that dynamic memory to avoid memory leaks, so if you make your own container class that dynamically allocates memory make sure its properly deleted.
      - To avoid heap fragmentation try to keep data structures in local scopes so the data is all in one chunck of memory that gets deleted together. This will help prevent empty memory blocks that are too small for the next bit of data to be allocated.
    - NOTE: Currently I'm planning to have dynamic container capacity grow the standard way which is doubling in size every time it reaches max capacity.

### My Info:
  - Email: andrey.stroitelev3301@gmail.com
  - URL: https://github.com/andreystroit3301
