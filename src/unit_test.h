#pragma once

#include <string>
#include <iostream>

#include "normalizer.h"

// Basic unit test class
class UnitTest {
  public:
    static void testLowercase(std::string input) {
      std::cout << "Testing Lowercasing\n";
      Normalizer normalizer;
      normalizer.lowercaseFunc(input);
      for (char c : input) {
        if (!islower(c) && !isspace(c)) {
          std::cout << "Test Failed!\n";
          return;
        }
      }
      std::cout << "Test Passed\n";
    }
    // Add more tests here, keep them static if possible
  private:
};
