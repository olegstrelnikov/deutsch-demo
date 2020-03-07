/*
 * matrix-test.cpp
 *
 * Created on: Mar 8, 2020
 * Author: Oleg Strelnikov
 */

#include "matrix-print.h"

void matrix_test() {
	std::vector<double> v({1.0,2.0,3.0,4.0});
	std::vector<int> vv{1,2,3};
	float vvv[] = {5.5f,6.5f};
	std::vector<int> v4{10,11,12,13,14,15};
	print_vector(v) << "\n";
	print_vector(vv) << "\n";
	print_vector(vvv) << "\n";
	print_vector(std::vector<char>()) << "\n";
	print_vector<std::vector<float>>({5.0f,6.0f}) << "\n";
	std::vector<std::vector<int>> mm{{1,2,3},{4,5,6}};
	double mmm[4][5] = {{1,2,3,4,5},{2,3,4,5,6},{3,4,5,6,7},{4,5,6,7,8}};
	std::vector<std::vector<int>> mmmmm{{1,2,3},{4,5,6}};
	matrix<double> m1{{1.1,2.2},{3.3,4.4},{5.5,6.6}};
	matrix<int> m2(mm);
	m2[1][2] = 3456;
	matrix<int> m3(mmm);
	matrix<double> m4(v4);
	matrix<double> m5(mmmmm);
	print_matrix(m1) << "\n";
	print_matrix(m2) << "\n";
	print_matrix(m3) << "\n";
	print_matrix(m4) << "\n";
	print_matrix(matrix<int>({{1,2,3,4,5,6,7,8,9,10}})) << "\n";
	print_matrix(matrix<int>(std::vector<std::vector<int>>(3))) << "\n";
	print_matrix(matrix<int>()) << "\n";
	print_matrix_multiplication(m1, m5) << "\n";
	print_matrix_multiplication(m5, m1) << "\n";
	print_matrix_multiplication(matrix<int>{{1},{2},{3},{4}}, matrix<int>{{1,2,3}}) << "\n";
	print_matrix_multiplication(matrix<int>{{1,2,3}}, matrix<int>{{1},{2},{3}}) << "\n";
}
