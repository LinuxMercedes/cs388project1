#include "binary.h"
#include "division_algorithms.h"
#include <iostream>

using namespace std;

struct BinaryPair {
  Binary dividend;
  Binary divisor;
};

string DIVIDENDS[8] = {
  "0.11011110", // .DE
  "0.00011001", // .19
  "0.000101010110", // .156
  "0.001000110010", // .232
  "0.0001000100010001", // .1111
  "0.0101101011000010", // .5AC2
  "0.100111001101111000010001", // .9CDE11
  "0.011001110010001100000000" // .672300
};

string DIVISORS[8] = {
  "0.1110", // .E
  "0.0101", // .5
  "0.101110",
  "0.011111",
  "0.00100010", // .22
  "0.01111001", // .79
  "0.101010111100", // .ABC
  "0.110111101111", // .DEF
};

int main() {

  unsigned int cost;
  for(int i = 0; i < 1; i++) {
    Binary dividend(DIVIDENDS[i].size()-1);
    Binary divisor(DIVISORS[i].size()-1);
    dividend = DIVIDENDS[i].c_str();
    divisor = DIVISORS[i].c_str();
    cost = 0;
    cout << "Dividend: " << dividend << endl;
    cout << "Divisor: " << divisor << endl;
    cout << "Quotient: " << multiplicative_division(dividend, divisor, cost) << endl;
    cout << "Cost: " << cost << endl;
  }

  return 0;
}

