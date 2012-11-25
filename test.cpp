#include "binary.h"
#include <iostream>

using namespace std;

int main() {
  unsigned int cost = 0;
  Binary b(4);
  Binary a(4);

  a = "0.101";
  b = "0.011";

  Binary c = sub(a, b, cost);
//  b.complement();

  cout << a.char_val() << endl;
  cout << b.char_val() << endl;
  cout << c.char_val() << endl;

  cout << mul(a, b, cost).char_val() << endl;
  return 0;
}

