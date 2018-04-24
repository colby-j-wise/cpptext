#include <iostream>
#include <string>
#include <unordered_set>

#include <boost/program_options.hpp>

#include "normalizer.h"
#include "processor.h"
#include "unit_test.h"

// unit test runner
int main(int argc, char *argv[]) {
  std::cout << "Running Unit Tests\n";
  UnitTest::testLowercase("A CAPITALIZED SENTENCE");
}
