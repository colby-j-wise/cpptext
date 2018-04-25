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
  UnitTest::testRemovePunc("ASCII VALUES 33 to 47 !\"#$%&'()*+,-./	end");
  UnitTest::testRemovePunc("ASCII VALUES 58 to 64 :;<=>?@		end");
  UnitTest::testRemovePunc("ASCII VALUES 91 to 96 []\"^_`		end");
  UnitTest::testRemovePunc("ASCII VALUES 123 to 126 {|}~ end");
  UnitTest::testRemovePunc("\"What is your name?\" she said, hastily.");

  UnitTest::testRemoveDigits("FINE");
  UnitTest::testRemoveDigits("C0LUMB1A");
  UnitTest::testRemoveDigits("__C0LUMB1A_IN_NYC___=");

  //For these, put the stopwords in a set, then the string cointaining everything, and then what the string should look like after stopword removal
  UnitTest::testSetStopwards({"This", "Should"}, "This Should Succeed", "Succeed");
  UnitTest::testSetStopwards({"This", "Should"}, "This Really Should Succeed", "Really Succeed");
  UnitTest::testSetStopwards({"This", "Should"}, "This Should Fail", "Failll");

  //For these, put the string cointaining everything, the regex, what the regex should be replaced with, and the final result of the string
  UnitTest::testRegex("<html>Hello!</html>", "<[^>]+>", "", "Hello!");
  UnitTest::testRegex("<html>What is up</html>", "<[^>]+>", "", "What is up");
  UnitTest::testRegex("Born 2to Run", "(\\+|-)?[[:digit:]]+", "", "Born to Run");
  UnitTest::testRegex("Too 2 many spaces", "(\\+|-)?[[:digit:]]+", "", "Too many spaces");

}
