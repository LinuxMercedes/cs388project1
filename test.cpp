#include "binary.h"
#include "division_algorithms.h"
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

const bool DELIMITED = true;
const char DELIM = ';';

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

/*
 * Return a binary representation of num as a string
 */
string doubleAsBinary(double num)
{
  stringstream os;
  os << "(" << num << ") ";

  int i = 12;
  while(i != -12)
  {
    if(num >= pow(2, i))
    {
      os << "1";
      num -= pow(2, i);
    }
    else
    {
      os << "0";
    }

    if(i == 0)
      os << ".";

    i--;
  }
  return os.str();
}

int main() {
  
  cout << "Dividend" << DELIM << "Divisor" << DELIM
       << "Multiplicative Division Quotient" << DELIM << "Cost" << DELIM 
       << "Divisor Reciprocation Quotient" << DELIM << "Cost" << DELIM
       << "Correct Value" << endl;

  for(int i = 0; i < sizeof(DIVIDENDS) / sizeof(DIVIDENDS[0]); i++) {
    Binary dividend(DIVIDENDS[i].size()-1);
    Binary divisor(DIVISORS[i].size()-1);
    Binary md_result;
    Binary dr_result;

    unsigned int md_cost = 0;
    unsigned int dr_cost = 0;

    dividend = DIVIDENDS[i].c_str();
    divisor = DIVISORS[i].c_str();

    md_result = multiplicative_division(dividend, divisor, md_cost);
    dr_result = divisor_reciprocation(dividend, divisor, dr_cost);
    if (DELIMITED){
      cout << dividend << DELIM << divisor << DELIM 
           << md_result << DELIM << md_cost << DELIM
           << dr_result << DELIM << dr_cost << DELIM 
           << doubleAsBinary(dividend.toDouble() / divisor.toDouble())
           << endl;
    } else {
      cout << "Dividend: " << dividend << endl
           << "Divisor: " << divisor << endl
           << "MD Quotient: " << md_result << "  Cost: " << md_cost << endl
           << "DR Quotient: " << dr_result << "  Cost: " << dr_cost << endl
           << "Actual Value: " << doubleAsBinary(dividend.toDouble() / divisor.toDouble())
           << endl << endl;
    }

  }

  return 0;
}

