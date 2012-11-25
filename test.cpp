#include "binary.h"
#include <iostream>

using namespace std;

int main() {
  Binary b(5);

  b = "10.10";

  //b.complement();

  cout << b.int_val() << endl;
  cout << b.char_val() << endl;
  cout << b.get_decimal() << endl;

  return 0;
}

