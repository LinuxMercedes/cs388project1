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
  
  for(int i = 0; i < ITERLIMIT && b_i != one; i++) {
    f_i = b_i;
    f_i.complement();
    a_i = mul(a_i, f_i, cost);
    b_i = mul(b_i, f_i, cost);
  }

  return a_i;
}

/**
 * Performs a / b = ? using divisor reciprocation method
 * @param a [in] Left hand side
 * @param b [in] Right hand side
 * @param cost [in/out] Cost to perform operation
 * @return Binary value with the result
 */
Binary divisor_reciprocation(const Binary& a, const Binary& b, unsigned int& cost) {

//	std::cout << "a: " << a << endl;
//	std::cout << "b: " << b << endl;

	Binary x_0(b.get_size(), b.get_decimal(), 1); // Approximately equal to 1 / B

//	std::cout << "x_" << 0 << " = " << x_0 << endl;

	Binary a_0 = mul(b, x_0, cost);

//	std::cout << "a_" << 0 << " = " << a_0 << endl;

	Binary x_i = x_0, a_i = a_0;
	const Binary TWO(b.get_size(), b.get_decimal(), 2);

//	std::cout << "Iteration #" << 0 << endl;
//	std::cout << "x_" << 0 << " = " << x_0 << endl;
//	std::cout << "a_" << 0 << " = " << a_0 << endl;

	for(int i = 0; i != 2; i++)
	{
		// Perform the operations in parallel
		unsigned int costX = cost, costA = cost;

		Binary sub_x = sub(TWO, a_0, costX);
		Binary sub_a = sub(TWO, a_0, costA);

		x_i = mul(x_0, sub_x, costX);
		a_i = mul(a_0, sub_a, costA);
		cost = max(costX, costA);

//		std::cout << "Iteration #" << i + 1 << endl;
//		std::cout << "subx_" << i + 1<< " = " << sub_x << endl;
//		std::cout << "suba_" << i + 1 << " = " << sub_a << endl;
//		std::cout << "x_" << i + 1<< " = " << x_i << endl;
//		std::cout << "a_" << i + 1 << " = " << a_i << endl;

		// x_0 is really x_i-1 for the purposes of this loop.
		x_0 = x_i;
		a_0 = a_i;
	}

	return x_i;
}
