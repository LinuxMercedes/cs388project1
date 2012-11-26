#ifndef BINARY_H
#define BINARY_H

#include <cmath>
#include <string.h>
#include <algorithm>
#include <sstream>
#include <iostream>

static unsigned int ZERO = 0; // Only use for optional pass-by-reference parameters

using namespace std;

bool full_add(const bool lhs, const bool rhs, bool& carry) {
  bool ret;
  if(lhs) {
    if(rhs) {
      ret = carry;
      carry = true;
    }
    else {
      ret = !carry;
    }
  }
  else {
    if(rhs) {
      ret = !carry;
    }
    else {
      ret = carry;
      carry = false;
    }
  }

  return ret;
}

class Binary {
  public:
    Binary(unsigned int sz, unsigned int dec = 0) {
      size = sz;
      decimal = dec;
      number = new bool[size];

      carryin = false;
      overflow = false;
      truncate = false;
    }

    Binary(unsigned int sz, unsigned int dec, int value) {
      size = sz;
      decimal = dec;
      number = new bool[size];

      carryin = false;
      overflow = false;
      truncate = false;

      *this = value;
    }

    Binary() {
      size = 0;
      decimal = 0;
      number = NULL;

      carryin = false;
      overflow = false;
      truncate = false;
    }

    Binary(const Binary& val) {
      number = new bool[val.size];
      size = val.size;
      decimal = val.decimal;

      overflow = val.overflow;
      carryin = val.carryin;
      truncate = val.truncate;

      for(unsigned int i = 0; i < size; i++) {
        number[i] = val.number[i];
      }
    }

    ~Binary() {
      if(number != NULL)
        delete [] number;
    }

    bool has_overflow() {
      return overflow;
    }

    bool has_carryin() {
      return carryin;
    }

    bool has_truncate() {
      return truncate;
    }

    unsigned int get_size() const {
      return size;
    }

    // Set with the number of digits to the right of the decimal point
    void set_decimal(unsigned int loc) {
      if(loc < size) {
        decimal = loc;
      }
    }

    void set_digit(unsigned int loc, bool val) {
      if(loc < size) {
        number[loc] = val;
      }
    }

    unsigned int get_decimal() const {
      return decimal;
    }

    friend Binary add(const Binary& lhs, const Binary& rhs, unsigned int& cost) {
      bool carry = false;
      unsigned int sz = max(lhs.size, rhs.size);
      Binary result(sz);

      // Calculate resulting decimal location
      unsigned int left_ldec = lhs.size - lhs.decimal;
      unsigned int right_ldec = rhs.size - rhs.decimal;
      result.decimal = sz - max(left_ldec, right_ldec);

      // Calculate indices in lhs and rhs to start adding at
      unsigned int l = max(static_cast<int>(lhs.decimal - result.decimal), 0);
      unsigned int r = max(static_cast<int>(rhs.decimal - result.decimal), 0);
      result.truncate = (l > 0) || (r > 0);

      // Calculate which index in result to start adding l and r to
      unsigned int l_start = max(static_cast<int>(result.decimal - lhs.decimal), 0);
      unsigned int r_start = max(static_cast<int>(result.decimal - rhs.decimal), 0);

      // Add
      for(unsigned int i = 0; i < sz; i++) {
        if(i >= l_start && i >= r_start && l < lhs.size && r < rhs.size) {
          result.number[i] = full_add(lhs.number[l++], rhs.number[r++], carry);
        }
        else if(i >= l_start && l < lhs.size) {
          result.number[i] = full_add(lhs.number[l++], false, carry);
        }
        else if(i >= r_start && r < rhs.size) {
          result.number[i] = full_add(rhs.number[r++], false, carry);
        }
        else {
          result.number[i] = full_add(false, false, carry);
        }
      }

      result.overflow = carry;
      result.carryin = carry; // I think this is right for subtraction

      // Update cost
      cost += 3 + 2 * (sz - 1);

      return result;
    }

    friend Binary sub(const Binary& lhs, const Binary& rhs, unsigned int& cost) {
      // Take 2's complement and add!
      Binary r = rhs;
      r.complement();
      return add(lhs, r, cost);
    }

    friend Binary mul(const Binary& p_b, const Binary& p_q, unsigned int& cost) {
      Binary b = p_b, q = p_q;
      int size = p_q.get_size();
      if(p_b.get_size() > p_q.get_size()) {
        size = p_b.get_size();
        q = p_q.pad_to_size(size);
      }
      if(p_b.get_size() < p_q.get_size()) {
        b = p_b.pad_to_size(size);
      }
      int floor_log2 = static_cast<int>(floor(log(size)/log(2)));

      // build matrix of summands
      Binary *matrix_of_summands = new Binary[size];
      for(int i = 0; i < size; i++) {
        matrix_of_summands[i] = Binary(2*size-1);
        for(int j = 0; j < (2*size-1); j++) {
          if(i <= j && j < (size + i)) {
            matrix_of_summands[i].number[j] = q.number[i] & b.number[j-i];
          } else {
            matrix_of_summands[i].number[j] = 0;
          }
        }
      }

      Binary *results = new Binary[size];
      for(int i = 0; i < size; i++) {
        results[i] = matrix_of_summands[i];
      }

      int results_size = size;
      while(results_size > 1){
        int i, next = 0;
        for(i = 0; i < results_size/2; i++, next+=2) {
          results[i] = add(results[next], results[next + 1], cost);
        }
        if (results_size % 2) {
          results[i] = results[results_size-1];
        }
        results_size = (results_size + 1) / 2;
      }

      Binary result = results[0];
      result.decimal = b.decimal + q.decimal;

      delete[] matrix_of_summands;
      delete[] results;

      cost = 1 + (floor_log2 * 4) + (2 * size - 1) * 4;
      return result;
    }

    Binary pad_to_size(const unsigned int new_size) const {
      if (new_size < size) {
        throw "new_size must be greater than current size";
      }
      if (new_size == size) {
        return Binary(*this);
      }
      Binary q(new_size);
      int size_diff = new_size - size;
      for (int i = 0; i < new_size; i++){
        if(i > size_diff) {
          q.number[i] = number[i-size_diff];
        } else {
          q.number[i] = 0;
        }
      }
      q.decimal = decimal + size_diff;
      return q;
    }

    void complement(unsigned int& cost = ZERO) {
      for(unsigned int i = 0; i < size; i++) {
        number[i] = !number[i];
      }

      // Add one, but quickly
      unsigned int i;
      for(i = 0; i < size && number[i] != 0; i++) {
        number[i] = 0;
      }
      if(i < size) {
        number[i] = 1;
      }

      // Add cost
      cost += size;
    }

    string char_val() const {
      stringstream str;
      for(int i = size - 1; i >= 0; i--) {
        if(decimal == i + 1) {
          str << '.';
        }
        str << number[i];
      }

      return str.str();
    }

    Binary operator <<(const Binary& val) {
      Binary temp(val.size);
      temp = (*this);
      for(int i = (size - 1); i > 0; i--) {
        temp.number[i] = temp.number[i - 1];
      }
      temp.number[0] = 0;
      return temp;
    }

    Binary operator >>(const Binary& val) {
      Binary temp(val.size);
      temp = (*this);
      for(int i = 0; i < size - 1; i++) {
        temp.number[i] = temp.number[i + 1];
      }
      return temp;
    }

    bool operator== (const Binary& val) {
      int i = 0, j = 0;
      
      //Account for zeros on the right of the number
      if(decimal < val.decimal) {
        while(j < val.decimal - decimal) {
          if(val.number[j++]) {
            return false;
          }
        }
      }
      else {
        while(i < decimal - val.decimal) {
          if(number[i++]) {
            return false;
          }
        }
      }

      // Compare numbers
      while(i < size && j < val.size) {
        if (number[i] != val.number[j]) {
          return false;
        }
        i++;
        j++;
      }
      
      // Check for excess padding
      while(i < size) {
        if(number[i++] != number[size - 1]) {
          return false;
        }
      }
      while(j < val.size) {
        if(val.number[j++] != val.number[val.size - 1]) {
          return false;
        }
      }

      return true;
    }

    bool operator!= (const Binary& val) {
      return !((*this) == val);
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
      overflow = val.overflow;
      carryin = val.carryin;
      truncate = val.truncate;


      for(unsigned int i = 0; i < size; i++) {
        number[i] = val.number[i];
      }

      return *this;
    }

    /* Assign from an integer */
    Binary& operator= (int val) {
      bool cpl = (val < 0);
      decimal = 0;

      if(cpl) {
        val *= -1;
      }

      val <<= decimal;

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
      for(; i < size; i++) {
        number[i] = (val[0] == '1');
      }

      return *this;
    }

  private:
    bool* number;
    unsigned int size;
    unsigned int decimal;

    bool overflow;
    bool carryin;
    bool truncate;

};

/*
 * ostream insertion operator for the binary type
 */
ostream& operator <<(ostream &os, const Binary &num)
{
	return os << num.char_val();
}

#endif
