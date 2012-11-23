#include "binary.h"
#include <iostream>

using namespace std;

int main() {
  Binary b(5);

  b = "01001";

  cout << b.int_val() << endl;
  cout << b.bin_val() << endl;

  return 0;
}

