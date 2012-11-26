#include "binary.h"
#include "division_algorithms.h"
#include <iostream>

using namespace std;

int main() {
  unsigned int cost = 0;
  Binary b(4);
  Binary a(4);

  a = "1.01";
  b = "1.010";

  Binary c = sub(a, b, cost);
//  b.complement();

  cout << a << endl;
  cout << b << endl;
  cout << c << endl;
  cout << (a == b) << endl;

  cout << mul(a, b, cost).char_val() << endl;

  cout << multiplicative_division(a, b, cost).char_val() << endl;
  return 0;
}

