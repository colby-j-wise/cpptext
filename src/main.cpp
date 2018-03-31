#include <iostream>

int main(int argc, char *argv[]) {

  std::cout << "Printing numbers 1 through 10 in parallel\n";
  #pragma omp parallel for num_threads(4)
  for (int i = 1; i < 11; i++) {
    std::cout << i << "\n";
  }
}
