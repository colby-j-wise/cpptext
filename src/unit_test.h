#pragma once

#include <string>
#include <iostream>

#include "normalizer.h"

// Basic unit test class
class UnitTest {
  public:

    //Filter Whitespace Lines Test
    static void testFilterWhitespaceLines(std::string input) {
      static unsigned int test_number = 1;
      Normalizer normalizer;
      bool result = normalizer.whitespaceOnly(input);
      if (result == false) {
        std::cout << "[ ] || Filter Whitespace Lines Test " << test_number << " Failed! || Tested string: \"" << input << "\"\n";
        test_number++;
        return;
      }
      std::cout << "[X] || Filter Whitespace Lines Test " << test_number << " Passed\n";
      test_number++;
    }

    //Lowercase Test
    static void testLowercase(std::string input) {
      static unsigned int test_number = 1;
      Normalizer normalizer;
      normalizer.lowercaseFunc(input);
      for (char c : input) {
        if (!islower(c) && !isspace(c)) {
          std::cout << "[ ] || Lowercasing Test " << test_number << " Failed! || Tested string: \"" << input << "\"\n";
          test_number++;
          return;
        }
      }
      std::cout << "[X] || Lowercasing Test " << test_number << " Passed\n";
      test_number++;
    }

    //Remove Punctuation Test
    static void testRemovePunc(std::string input) {
      static unsigned int test_number = 1;
      Normalizer normalizer;
      normalizer.removepuncFunc(input);
      for (char c : input) {
        if(ispunct(static_cast<unsigned char>(c))) {
          std::cout << "[ ] || Remove Punctuation Test " << test_number << " Failed! || Tested string: \"" << input << "\"\n";
          test_number++;
          return;
        }
      }
      std::cout << "[X] || Remove Punctuation Test " << test_number << " Passed\n";
      test_number++;
    }


    //Remove Digits Test
    static void testRemoveDigits(std::string input) {
      static unsigned int test_number = 1;
      Normalizer normalizer;
      normalizer.removedigitFunc(input);
      for (char c : input) {
        if(isdigit(static_cast<unsigned char>(c))) {
          std::cout << "[ ] || Remove Digits Test " << test_number << " Failed! || Tested string: \"" << input << "\"\n";
          test_number++;
          return;
        }
      }
      std::cout << "[X] || Remove Digits Test " << test_number << " Passed\n";
      test_number++;
    }

    //Set Stopwords Test

    //Process Test 


  private:
};
