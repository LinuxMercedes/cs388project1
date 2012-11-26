#include "binary.h"
#include "division_algorithms.h"
#include <iostream>

using namespace std;

struct BinaryPair {
  Binary dividend;
  Binary divisor;
};

string DIVIDENDS[8] = {
  ".11011110", // .DE
  ".00011001", // .19
  ".000101010110", // .156
  ".001000110010", // .232
  ".0001000100010001", // .1111
  ".0101101011000010", // .5AC2
  ".100111001101111000010001", // .9CDE11
  ".011001110010001100000000" // .672300
};

string DIVISORS[8] = {
  ".1110", // .E
  ".0101", // .5
  ".101110",
  ".011111",
  ".00100010", // .22
  ".01111001", // .79
  ".101010111100", // .ABC
  ".110111101111", // .DEF
};

int main() {

  unsigned int cost;
  for(int i = 0; i < 8; i++) {
    Binary dividend(DIVIDENDS[i].size()-1);
    Binary divisor(DIVISORS[i].size()-1);
    dividend = DIVIDENDS[i].c_str();
    divisor = DIVISORS[i].c_str();
    cost = 0;
    cout << "Dividend / Divisor: " << dividend << " / " << divisor << endl;
    cout << "Quotient: " << multiplicative_division(dividend, divisor, cost) << endl;
    cout << "Quotient: " << divisor_reciprocation(dividend, divisor, cost) << endl;
    cout << "Cost: " << cost << endl;
  }

  return 0;
}

