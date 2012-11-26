#include <iostream>

using namespace std;

static const int ITERLIMIT = 5;

Binary multiplicative_division(const Binary& a, const Binary& b, unsigned int& cost) {
  int size = a.get_size();
  Binary one(size);
  for(int i = 0; i < size; i++){
    one.set_digit(i, 1);
  }
  one.set_digit(size-1, 0);
  one.set_decimal(size-1);
  
  Binary f_i = one;
  Binary a_i = a;
  Binary b_i = b;
  
  for(int i = 0; i < ITERLIMIT && b_i != one; i++) {
    a_i = mul(a_i, f_i, cost).truncate_to_size(size);
    b_i = mul(b_i, f_i, cost).truncate_to_size(size);
    f_i = b_i;
    f_i.complement();
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
Binary divisor_reciprocation(const Binary& aP, const Binary& bP, unsigned int& cost) {

	Binary a = aP;
	Binary b = bP;

	if(b.toDouble() < 0.5 || b.toDouble() >= 1)
	{
//		cout << "WARNING: divisor(B) is " << b << " outside of 1/2 <= B < 1" << endl;
		double bVal = b.toDouble();
		while(bVal < 0.5)
		{
		  b = b << 1;
		  b.decimal++;
		  a = a << 1;
		  a.decimal++;
		  bVal *= 2;
		}

		while(bVal >= 1)
		{
		  b = b >> 1;
		  b.decimal--;
		  a = a >> 1;
		  a.decimal++;
		  bVal /= 2;

		}
	}

	int size = max(a.get_size(), b.get_size());
	Binary x_0(size, b.get_decimal());
	x_0 = a.resize(size);

	Binary a_0 = b.resize(size); //mul(b.resize(size), x_0.resize(size), cost).resize(size);


	Binary x_i = x_0, a_i = a_0;
	Binary TWO(size, b.get_decimal());
	TWO = "010.0";
	TWO = TWO.resize(size);

//
//	std::cout << "Iteration #" << 0 << endl;
//	std::cout << "x_" << 0 << " = " << x_0 << endl;
//	std::cout << "a_" << 0 << " = " << a_0 << endl;

	while(abs(x_i.toDouble() - a.toDouble() / b.toDouble()) >= pow(2, -size + 3) &&
			x_i.get_size() <= size) // Division overflow
	{
		// Perform the operations in parallel
		unsigned int costX = cost, costA = cost;

		Binary sub_x = sub(TWO, a_0, costX);
		Binary sub_a = sub(TWO, a_0, costA);

//		std::cout << "Iteration #" << i + 1 << endl;
//
//		cout << TWO << " - " << a_0 << " = " << sub_x << endl;
		x_i = mul(x_0, sub_x, costX);
//		cout << x_0 << " * " << sub_x << " = " << (x_i) << endl;
		a_i = mul(a_0, sub_a, costA);
//		cout << a_0 << " * " << sub_a << " = " << (a_i) << endl;
		cost = max(costX, costA);

		x_i = x_i.resize(size);
		a_i = a_i.resize(size);

//		std::cout << "x_" << i + 1 << " = " << x_i << endl;
//		std::cout << "a_" << i + 1 << " = " << a_i << endl;

		// x_0 is really x_i-1 for the purposes of this loop.
		x_0 = x_i;
		a_0 = a_i;
	}

	return x_i;
}
