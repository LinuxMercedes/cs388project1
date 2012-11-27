#ifndef BINARY_H
#define BINARY_H

#include <cmath>
#include <string.h>
#include <algorithm>
#include <sstream>
#include <iostream>

static unsigned int ZERO = 0; // Only use for optional pass-by-reference parameters

using namespace std;


/**
 * Full adder
 * @param lhs [in] Left hand side
 * @param rhs [in] Right hand side
 * @param carry [in/out] Carry bit
 * @return Binary value with the result
 */
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


/**
 * Binary
 * @desc Represents a binary floating point number
 */
class Binary {
  public:

    /**
     * Binary Constructor
     * @param sz [in] number of bits in the number
     * @param dec [in] decimal position (defaults to 0)
     */
    Binary(unsigned int sz, unsigned int dec = 0) {
      size = sz;
      decimal = dec;
      number = new bool[size];

      carryin = false;
      overflow = false;
      truncate = false;
    }

    /**
     * Binary Constructor
     * @param sz [in] number of bits in the number
     * @param dec [in] decimal position (defaults to 0)
     * @param value [in] integer value to assign to the new Binary
     */
    Binary(unsigned int sz, unsigned int dec, int value) {
      size = sz;
      decimal = dec;
      number = new bool[size];

      carryin = false;
      overflow = false;
      truncate = false;

      *this = value;
    }

    /**
     * Binary Default Constructor
     * @desc Creates a binary of size zero
     */
    Binary() {
      size = 0;
      decimal = 0;
      number = NULL;

      carryin = false;
      overflow = false;
      truncate = false;
    }

    /**
     * Binary Copy Constructor
     * @param val [in] the Binary to copy
     */
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

    /**
     * Binary Destructor
     * @desc Deallocates resources used by the Binary
     *       (Don't call this explicitly.)
     */
    ~Binary() {
      if(number != NULL) {
        delete [] number;
      }
    }

    /**
     * has_overflow()
     * @return true if overflow, else false
     */
    bool has_overflow() {
      return overflow;
    }

    /**
     * has_carryin()
     * @return true if carryin, else false
     */
    bool has_carryin() {
      return carryin;
    }

    /**
     * has_truncate()
     * @return true if truncate, else false
     */
    bool has_truncate() {
      return truncate;
    }

    /**
     * get_size()
     * @return returns the number of bits allocated to this Binary
     */
    unsigned int get_size() const {
      return size;
    }

    /**
     * get_decimal()
     * @return returns the decimal position of the Binary
     */
    unsigned int get_decimal() const {
      return decimal;
    }

    /**
     * set_decimal()
     * @param loc [in] the new position of the decimal
     * @desc Set with the number of digits to the right of the decimal point
     */
    void set_decimal(unsigned int loc) {
      if(loc < size) {
        decimal = loc;
      }
    }

    /**
     * set_digit()
     * @param loc [in] the position to change
     * @param val [in] the value of the bit at position loc
     * @desc Set the bit at position "loc" to "val"
     */
    void set_digit(unsigned int loc, bool val) {
      if(loc < size) {
        number[loc] = val;
      }
    }

    /**
     * add()
     * @desc Simulates fast addition using carry lookahead 2 scheme
     * @param lhs [in] the left hand side
     * @param rhs [in] the right hand side
     * @param cost [in/out] Cost to perform operation
     * @return Binary value with the result
     */
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
      cost += 4 * (sz/4) + 1;

      return result;
    }

    /**
     * sub()
     * @desc Simulates subtraction
     * @param lhs [in] the left hand side
     * @param rhs [in] the right hand side
     * @param cost [in/out] Cost to perform operation
     * @return Binary value with the result
     */
    friend Binary sub(const Binary& lhs, const Binary& rhs, unsigned int& cost) {
      // Take 2's complement and add!
      Binary r = rhs;

      // This is a workaround because add() adjusts the decimal point
      // in order to avoid dropping the 1.
      // e.g, for 2 - 0.875:
      // Add wants to do this: (2) 010.000000 + (3.125) 11.0010000 = (5.125) 101.001000
      // we *need* to do this: (2) 010.000000 + (3.125) 11.0010000 = (1.125) 01.001000
      Binary l = lhs;

      // Shift lhs left as much as we can, dropping off LSB's that are 0.
      while(l.number[l.size - 1] == false)
      {
        l = l << 1;
        l.decimal++;
      }

      // Sign-extend rhs as much as we need
      while(r.number[r.size - 1] == false && r.decimal < l.decimal)
      {
        r = r << 1;
        r.decimal++;
      }

      while(r.decimal > l.decimal)
      {
        // Losing some data..
        if(r.number[0] == true)
          r.truncate = true;

        r = r >> 1;
        r.decimal--;
      }

      r.complement(cost);


      Binary result = add(l, r, cost);
      return result;
    }

    /**
     * mul()
     * @desc Simulates fast multiplication using full adder tree
     * @param p_b [in] the left hand side
     * @param p_q [in] the right hand side
     * @param cost [in/out] Cost to perform operation
     * @return Binary value with the result
     */
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
        unsigned int i, next = 0, unused_cost;
        for(i = 0; i < results_size/2; i++, next+=2) {
          results[i] = add(results[next], results[next + 1], unused_cost);
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

      // According to full adder tree formula
      cost += 1 + (floor_log2 * 4) + (2 * size - 1) * 4;
      return result;
    }

    /**
     * resize()
     * @desc Changes Binary's size and fixes padding
     * @param new_size [in] the new size for the Binary
     * @return a new Binary with size new_size
     */
    Binary resize(const unsigned int new_size) const {
      Binary q(new_size);

      // copy this->char_val() into a mutable buffer named 'value'
      int buf_size = max(size, (int)new_size);
      char* buffer = new char[buf_size + 1];
      strncpy(buffer, this->char_val().c_str(), buf_size);
      buffer[buf_size] = 0;

      char* value = buffer;

      // Trim from the left of the .
      while(*value != 0 && *value == '0')
        value++;
      if(*(value - 1) == '0') // Pesky sign extension..
        value--;

      // Is there a decimal point to worry about?
      const char* dec = value;
      while(*dec != '.' && *dec != 0)
        dec++;

      // Write null chars from the right to the left
      // until the length of the buffer equals the new size.
      char* end = buffer + buf_size;
      while(end != buffer && (end - value) > new_size + (*dec != 0 ? 1 : 0))
      {
        if(*--end == '.')
          return *this;
        *end = '\0';
      }

      q = value;

      delete[] buffer;

      return q;
    }

    /**
     * pad_to_size()
     * @desc Adds 0's to the right of a decimal Binary
     * @param new_size [in] desired size
     * @return Binary of size new_size, with extra padding on the right
     */
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
        if(i >= size_diff) {
          q.number[i] = number[i-size_diff];
        } else {
          q.number[i] = 0;
        }
      }
      q.decimal = decimal + size_diff;

      return q;
    }

    /**
     * truncate_to_size()
     * @desc Removes digits from the less significant side of a number
     * @param new_size [in] desired size
     * @return Binary truncated to size new_size
     */
    Binary truncate_to_size(const unsigned int new_size) const {
      if (new_size > size) {
        throw "new_size must be smaller than current size";
      }
      if (new_size == size) {
        return Binary(*this);
      }
      Binary q(new_size);
      int size_diff = size - new_size;
      for (int i = (new_size - 1); i >= 0; i--){
        q.number[i] = number[i+size_diff];
      }
      q.decimal = decimal - size_diff;
      return q;
    }

    /**
     * complement()
     * @desc Performs 2's complement of a number in place
     * @param cost [in/out] cost to perform complement
     */
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

    /**
     * char_val()
     * @return a string representation of the Binary
     */
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

    /**
     * operator <<
     * @desc Shifts digits in a Binary to the left WITHOUT moving the decimal.
     * @param val [in] number of places to shift
     * @return a shifted version of the Binary
     */
    Binary operator <<(const unsigned int val) {
      Binary temp(size);
      temp = (*this);
      for(int i = (size - 1); i >= 0; i--) {
        if (i >= val) {
          temp.number[i] = temp.number[i - val];
        } else {
          temp.number[i] = 0;
        }
      }
      return temp;
    }

    /**
     * operator >>
     * @desc Shifts digits in a Binary to the rightWITHOUT moving the decimal.
     * @param val [in] number of places to shift
     * @return a shifted version of the Binary
     */
    Binary operator >>(const unsigned int& val) {
      Binary temp(size);
      temp = (*this);
      for(int i = 0; i < size - 1; i++) {
        if (i < (size-val)) {
          temp.number[i] = temp.number[i + val];
        } else {
          temp.number[i] = temp.number[i - 1];
        }
      }
      return temp;
    }

    /**
     * operator ==
     * @return true if *this == val, else false.
     */
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

    /**
     * operator !=
     * @return true if *this != val, else false.
     */
    bool operator!= (const Binary& val) {
      return !((*this) == val);
    }

    /**
     * operator =
     * @desc allows assignment of a Binary to a Binary
     */
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

    /**
     * operator =
     * @param val [in] integer value to assign to Binary
     * @desc allows assignment an integer to a binary
     */
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

    /**
     * operator =
     * @param val [in] integer value to assign to Binary
     * @desc Assign from a character array of binary digits
     *       Note: indexing on the string is reversed so that it works
     *       the way most people expect it to.
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

    /**
     * toDouble()
     * @return double value of calling Binary
     */
    double toDouble() const {
      double value = 0;
      for(int i = 0; i != size; i++) {
        if(number[i]) {
          value += pow(2, i - (decimal));
        }
      }
      return value;
    }

    int decimal;                // Decimal position

  private:
    bool* number;               // Array of booleans to store number
    int size;                   // Number of bits in boolean array

    bool overflow;              // Whether Binary had overflow
    bool carryin;               // Whether carry occurred
    bool truncate;              // Whether Binary was truncated

};

/*
 * ostream insertion operator for the binary type
 */
ostream& operator <<(ostream &os, const Binary &num) {
    return os << "(" << num.toDouble() << ") " << num.char_val();
}

#endif
