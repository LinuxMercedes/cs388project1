#ifndef BINARY_H
#define BINARY_H

#include <cmath>
#include <string.h>
#include <algorithm>
#include <sstream>

static unsigned int ZERO = 0; // Only use for optional pass-by-reference parameters

using namespace std;

class Binary {
  public:
    Binary(unsigned int sz, unsigned int dec = 0) {
      size = sz;
      decimal = dec;
      number = new bool[size];
    }

    Binary() {
      size = 0;
      decimal = 0;
      number = NULL;
    }

    ~Binary() {
      if(number != NULL) 
        delete [] number;
    }

    void setDecimal(unsigned int loc) {
      if(loc < size) {
        decimal = loc;
      }
    }

    friend Binary add(const Binary& lhs, const Binary& rhs, bool& overflow, unsigned int& cost) {
      bool carry = false;

    }

    friend Binary sub(const Binary& lhs, const Binary& rhs, bool& carryin, unsigned int& cost);
    friend Binary mul(const Binary& lhs, const Binary& rhs, unsigned int& cost);

    void complement(unsigned int& cost = ZERO) {
      for(unsigned int i = 0; i < size; i++) {
        number[i] = !number[i];
      }

      // Add one, but quickly
      unsigned int i;
      for(i = 0; i < size && number[i] != 0; i++) {
        number[i] = 0;
      }
      number[i] = 1;

      // Add cost
      cost += size;
    }

    /* Deprecated */
    int int_val() {
      int value = 0;

      for(unsigned int i = 0; i < size; i++) {
        if(number[i]) {
          if(i == size - 1) {
            value -= pow(2, i);
          }
          else {
            value += pow(2, i);
          }
        }
      }

      return value;
    }

    const char* char_val() {
      stringstream str;
      for(int i = size - 1; i >= 0; i--) {
        if(decimal == i + 1) {
          str << '.';
        }
        str << number[i];
      }

      return str.str().c_str(); //yuck
    }

    /* Deprecated */
    unsigned int bin_val() {
      unsigned int value = 0;
      for(unsigned int i = 0; i < size; i++) {
        if(number[i]) {
          value += pow(10, i);
        }
      }
      return value;
    }

    Binary& operator= (const Binary& val) {
      if(size != val.size) {
        if(number != NULL) {
          delete [] number;
        }
        number = new bool[val.size];
        size = val.size;
      }

      decimal = val.decimal;
      
      for(unsigned int i = 0; i < size; i++) {
        number[i] = val.number[i];
      }

      return *this;
    }

    /* Assign from an integer */
    Binary& operator= (int val) {
      bool cpl = (val < 0);

      if(cpl) {
        val *= -1;
      }

      for(int i = size - 1; i >= 0; i--) {
        if(val - pow(2, i) >= 0) {
          number[i] = 1;
          val -= pow(2, i);
        }
        else {
          number[i] = 0;
        }
      }

      if(cpl) {
        complement();
      }

      return *this;
    }

    /* 
     * Assign from a character array of binary digits 
     * Note: indexing on the string is reversed so that it works
     * the way most people expect it to.
    */
    Binary& operator= (const char* val) {
      unsigned int i, j;
      bool decset = false;

      // Copy number from character to number array
      for(i = 0, j = 0; i < size && j < strlen(val); i++, j++) {
        //Set the decimal point if we find it
        if(val[strlen(val) - j - 1] == '.') {
          decimal = j;
          i--;
          decset = true;
          continue;
        }

        number[i] = (val[strlen(val) - j - 1] == '1');
      }

      // Catch an edge case where the decimal point isn't set in the previous loop
      if(!decset) {
        for(j = 0; j < strlen(val); j++) {
          if(val[strlen(val) - j - 1] == '.') {
            decimal = j;
            break;
          }
        }
      }

      //Sign extend number stored
      for(i; i < size; i++) {
        number[i] = (val[0] == '1');
      }

      return *this;
    }

  private:
    bool* number;
    unsigned int size;
    unsigned int decimal;

};

#endif
