#include "binary.h"
#include <iostream>

using namespace std;

int main() {
  unsigned int cost = 0;
  Binary b(5);
  Binary a(3);

  a = "01.1";
  b = ".11111";

  Binary c = add(a, b, cost);
  //b.complement();

  cout << a.char_val() << endl;
  cout << b.char_val() << endl;
  cout << c.char_val() << endl;

  return 0;
}

