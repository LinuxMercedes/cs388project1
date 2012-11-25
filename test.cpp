#include "binary.h"
#include <iostream>

using namespace std;

int main() {
  unsigned int cost = 0;
  Binary b(8);
  Binary a(8);

  a = "0000011";
  b = "0000011";

  Binary c = sub(a, b, cost);
//  b.complement();

  cout << a.char_val() << endl;
  cout << b.char_val() << endl;
  cout << c.char_val() << endl;

  cout << mul(a, b, cost).char_val() << endl;
  return 0;
}

