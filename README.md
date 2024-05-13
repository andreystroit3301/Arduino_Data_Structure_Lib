# Arduino_Data_Structure_Lib

- STATUS: INCOMPLETE/WIP -- No full release yet

## Description

This is a library for the Arduino IDE(V2+) that adds a bunch of templated data structures, functions, and meta-functions from libstdc++(c++ STL) which are not natively available/supported for AVR boards on the 
Arduino IDE. Most of this code is copied from various versions of GCC libstdc++, but primarily I used gcc-4.9.0 implementations. The purpose of this library is to just take the parts of c++ STL that are needed 
for the container classes along with the actual container classes and putting it all into a simple arduino library for AVR board users to use. Along with the data structures I'm implementing, I am also implementing the STL allocator, iterator, type traits, and a few general utilities and algorithms to go along with the data structure like std::swap, std::begin, and std::lexicographical_compare.

This library was developed in Arduino IDE V2.3, so any users should have Arduino IDE V2.3 or newer to make sure the library works properly.

### Note:

This library adds most of the data structures/containers found in the C++ STL library. These data structures will dynamically allocate and deallocate data, so they will function as they do for regular C++ 
programming. I may eventually add static versions of the containers that have a constant max size for situations where you have very little SRAM like in a large program, or for when you're using an arduino
board that has very low memory to begin with like the non-IOT boards and compact boards like the Arduino Nano or Arduino Pro Mini. I also added a large chunk of meta-functions from headers such as
<utility> and <type_traits> from libstdc++ since some will be required for the container implementations, and I just added some other meta-functions just because I think they could be useful when using the 
data structures.

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


### Custom Reallocation Feature _(Will implement in the future)_

The Containers in this library are just slightly modified copies of the original source code in libstdc++. Since it's a copy the container classes that have a dynamic capacity reallocate at the same
rate as they do in libstdc++. By default whenever you exceed the capacity on a dynamic container such as std::vector it will allocate a new block of memory that is 2x the size of the old capacity, 
so on an embedded system like an arduino the memory can get used up really fast when working with hundreds to thousands of elements. These containers can still be used in moderation perfectly fine, but 
I am going to implement a set of config macros that will customize how these containers do reallocation. There will be a couple of macros relating to this. To help preserve available memory the user
will be able to set how much the capacity multiplies during reallocation, or the user can make reallocation linear in which case the capacity will increase by a set amount of elements during reallocation.
Currently this library can only use the default libstdc++ method of reallocation which doubles the capacity each time despite the macros for this feature being defined. I will most likely not start
implementing this feature until I get alot more container classes finished since the way libstdc++ handles reallocation is alot more complicated that what is typically seen.
Also this library implements the allocators defined in libstdc++ which all dynamic sized containers will use by default unless the user gives a custom allocator, but this doesn't affect how much the
capacity of the container increases during reallocation.


## MY TO DO LIST:
- [ ] Test the current file structure to make sure the include statements work correctly
- [ ] Optimize whatever I can to futher reduce preformance cost for the Arduino
- [ ] Update the main License of the repo to include the additional contributors from libstdc++ for the new files added to this library
- [ ] Update license boilerplates on any files that need it
- [ ] Clean up macros in the Config file
- [ ] Clean up comments
- [ ] Clean up code to help avoid bugs
- [ ] Possibly add profile mode and associated implementations for performance analysis and optimization. _(I might not implement this mode, but I most likely will)_
- [ ] Create proper documentation aside from just the keywords.txt file
- [ ] Add test functions to example sketch
- [ ] Test everything that is currently implemented
- [ ] update keywords.txt with available functions, classes, and constants
- [ ] Create data structures
    - [X] ~~implement std::pair~~
    - [X] ~~implement std::tuple~~
    - [X] ~~implement std::stack~~
    - [X] ~~implement std::vector~~
    - [X] ~~implement std::list~~
    - [X] ~~implement std::forward_list~~
    - [X] ~~implement std::queue~~
    - [X] ~~implement std::priority_queue~~
    - [X] ~~implement std::dequeue~~
    - [X] ~~implement std::array~~
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
- [X] ~~Need to add global iterator functions/algorithms~~
- [X] ~~Need to add container utility functions/algorithms with their respective specializations~~
- [ ] Implement the customizable reallocation functionality
- [ ] Need to add arduino utility functions for faster IO read/write
- [ ] Possibly will add internal comparator control functions
- [ ] Need to add register/port control functions for abstraction
- [ ] Possibly will add functions for faster pinMode _(if possible)_


## What I'm currently working on:
- [ ] Implement std::stack
- [ ] Implement std::queue
- [ ] Implement std::dequeue
- [ ] Update main license along with NOTICE file
- [ ] Update boilerplates on files that need it
- [ ] Test the current file structure
- [ ] Possibly add the <bits/stl_heap> header from libstdc++

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
