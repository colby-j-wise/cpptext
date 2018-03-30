#include <iostream>

using namespace std;

int main(int argc, char *argv[]) {

  cout << "Printing numbers 1 through 10 in parallel\n";
  #pragma omp parallel for num_threads(4)
  for (int i = 0; i < 10; ++i) {
    cout << i << endl;
  }
}
