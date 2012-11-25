static const int F0 = 1;

Binary multiplicative_division(const Binary& a, const Binary& b, unsigned int& cost) {
  Binary one(a.get_size()), zero(a.get_size());
  zero = 0;
  one = 1;
  
  Binary f_i = F0;
  Binary a_i = a;
  Binary b_i = b;
  Binary b_i_complement;
  bool unused_overflow;
  
  while(b_i != one) {
    b_i_complement = b_i;
    b_i_complement.complement(cost);
    f_i = add(b_i_complement, one, unused_overflow, cost);
    a_i = mul(a_i, f_i, cost);
    b_i = mul(b_i, f_i, cost);
  }

  return a_i;
}
