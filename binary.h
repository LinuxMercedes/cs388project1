#ifndef BINARY_H
#define BINARY_H

#include <cmath>
#include <string.h>

static unsigned int ZERO = 0; // Allow optional pass-by-reference parameters
static unsigned int ONE = 1; // Allow optional pass-by-reference parameters

class Binary {
  public:
    Binary(unsigned int sz) {
      size = sz;
      number = new bool[size];
    }

    Binary() {
      size = 0;
      number = NULL;
    }

    ~Binary() {
      if(number != NULL) 
        delete [] number;
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
      unsigned int i;
      for(i = 0; i < size && i < strlen(val); i++) {
        number[i] = (val[strlen(val) - i - 1] == '1');
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

};

#endif
