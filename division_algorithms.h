#include <iostream>

using namespace std;

static const int F0 = 1;
static const int ITERLIMIT = 5;

Binary multiplicative_division(const Binary& a, const Binary& b, unsigned int& cost) {
  int size = a.get_size();
  Binary one(size), zero(size);
  for(int i = 0; i < size; i++){
    zero.set_digit(i, 0);
    one.set_digit(i, 1);
  }
  one.set_digit(size-1, 0);
  zero.set_decimal(size-1);
  one.set_decimal(size-1);
  
  Binary f_i = F0;
  Binary a_i = a;
  Binary b_i = b;
  Binary b_i_complement;
  bool unused_overflow;
  
  cout << "b_i == 1 ? " << (b_i == one) << endl;
  cout << "b_i = " << b_i.char_val() << endl;
  cout << "one = " << one.char_val() << endl;

  for(int i = 0; i < ITERLIMIT && b_i != one; i++) {
    cout << "**" << a_i.char_val() << endl;
    f_i = b_i;
    f_i.complement();
    a_i = mul(a_i, f_i, cost);
    b_i = mul(b_i, f_i, cost);
  }

  return a_i;
}
