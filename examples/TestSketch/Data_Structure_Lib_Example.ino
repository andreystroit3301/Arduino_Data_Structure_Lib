// This main .ino file is just for testing and example functions. The main code is in the other files bundled in this sketch folder.


#include "Data_Structure_lib.hpp"
//#include <inttypes.h>


union TestU { };
class TestC { };
class TestB { };
class TestD : public TestB { };
void test_type_traits(void);

void test_dlib_utility(void);

void test_arduino_util(void);

void test_initializer_list(void);

void test_iterator_traits(void);

void test_iterator(void);

void test_container(void);


void setup() {
  Serial.begin(115200);
  while(!Serial);
  Serial.println(F("\n\nStart of testing!!!\n\n"));
  
  test_type_traits();

  Serial.println(F("\n\n\nEnd Of Testing!!!\n"));
}


void loop() {}


void test_type_traits(void)  {
  Serial.print(F("Is TestU a union?: "));
  Serial.println((std::is_union_v<TestU>) ? F("Yes") : F("No"));
  Serial.print(F("Is TestC a union?: "));
  Serial.println((std::is_union_v<TestC>) ? F("Yes") : F("No"));
  Serial.print(F("Is TestC a class?: "));
  Serial.println((std::is_class_v<TestC>) ? F("Yes") : F("No"));
  Serial.print(F("Is TestB the base class for TestD?: "));
  Serial.println((std::is_base_of_v<TestB, TestD>) ? F("Yes") : F("No"));
}
