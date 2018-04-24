#include <iostream>
#include <string>
#include <unordered_set>

#include <boost/program_options.hpp>

#include "normalizer.h"
#include "processor.h"
#include "unit_test.h"

// unit test runner
int main(int argc, char *argv[]) {
  std::cout << "Running Unit Tests...\n";

  UnitTest::testFilterWhitespaceLines("       ");
  UnitTest::testFilterWhitespaceLines("");
  UnitTest::testLowercase("A CAPITALIZED SENTENCE");
  UnitTest::testLowercase("");
  UnitTest::testLowercase("99 RED BALOONS");
  UnitTest::testLowercase("__C0LUMB1A_IN_NYC_><__=");
  UnitTest::testRemovePunc("N.Y.C.");
  UnitTest::testRemovePunc("<N><Y><C>");
  UnitTest::testRemovePunc("\"What is your name?\" she said, hastily.");
  UnitTest::testRemoveDigits("FINE");
  UnitTest::testRemoveDigits("C0LUMB1A");
  UnitTest::testRemoveDigits("__C0LUMB1A_IN_NYC___=");
  UnitTest::testRemoveDigits("99 RED BALOONS");




}
