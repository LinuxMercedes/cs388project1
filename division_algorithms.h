Binary multiplicative_division(const Binary& a, const Binary& b, unsigned int& cost) {
  Binary ONE(a.size);
  ONE = 1;

  Binary f_i = F0;
  Binary a_i = a;
  Binary b_i = b;
  Binary b_i_complement;

  while(b_i != ONE) {
	b_i_complement = b_i;
	b_i_complement.complement();
	f_i = add(b_i_complement, ONE);
	a_i = mul(a_i, f_i);
	b_i = mul(b_i, f_i);
  }

  return a_i;
}
