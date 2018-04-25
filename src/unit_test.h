#pragma once

#include <string>
#include <iostream>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string.hpp>

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
        if (!islower(c) && !isspace(c) && isalpha(c)) {
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
    static void testSetStopwards(std::unordered_set<std::string> stopwords, std::string input, std::string ideal_result) {
      static unsigned int test_number = 1;
      Normalizer normalizer;
      std::string result = normalizer.process_string(input, stopwords);
      if (ideal_result == result) {
        std::cout << "[X] || Set Stopwards Test " << test_number << " Passed\n";
        test_number++;
        return;
      }
      else {
        std::cout << "[ ] || Set Stopwards Test " << test_number << " Failed! || Tested string: \"" << input << "\"\n";
        test_number++;
        return;
      }
    }

    //Regex Test
    static void testRegex(std::string input, std::string regex, std::string replace_with, std::string ideal_result) {
      static unsigned int test_number = 1;
      Normalizer normalizer;
      normalizer.addRegex(regex, replace_with);
      normalizer.runRegex(input);
      std::string result = input;
      if (ideal_result == result) {
        std::cout << "[X] || RegEx Test " << test_number << " Passed\n";
        test_number++;
        return;
      }
      else {
        std::cout << "[ ] || RegEx Test " << test_number << " Failed! || Tested string: \"" << input << "\"\n";
        test_number++;
        return;
      }
    }

  private:
};
