#include "binary.h"
#include <iostream>

using namespace std;

int main() {
  Binary b(5);

  b = "0000.1";

  //b.complement();

  cout << b.int_val() << endl;
  cout << b.char_val() << endl;

  return 0;
}

