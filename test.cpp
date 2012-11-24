#include "binary.h"
#include <iostream>

using namespace std;

int main() {
  Binary b(5);

  b = "11.001";

  //b.complement();

  cout << b.int_val() << endl;
  cout << b.bin_val() << endl;
  cout << b.char_val() << endl;

  return 0;
}

