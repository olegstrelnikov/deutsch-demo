/*
 * deutsch-demo.cpp
 *
 * Created on: Mar 7, 2020
 * Author: Oleg Strelnikov
 */

#include <iostream>
#include <vector>
#include <complex>
#include <iterator>

#include "matrix.h"

bool f0(bool) {
	return false;
}

bool f1(bool) {
	return true;
}

bool f2(bool x) {
	return x;
}

bool f3(bool x) {
	return !x;
}

using unary = bool (*)(bool);

std::ostream& print_function(unary f) {
	return std::cout << false << "->" << f(false) << " " << true << "->" << f(true);
}

bool is_constant(unary f) {
	return f(false) == f(true);
}


void matrix_test();

int main() {
	matrix_test();
	unary funcs[] = {f0, f1, f2, f3};
	for (auto const f: funcs) {
		std::cout << "function [";
		print_function(f) << "] is " << (is_constant(f) ? "" : "not ") << "constant\n";
	}
	return 0;
}
