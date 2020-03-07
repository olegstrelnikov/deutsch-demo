/*
 * matrix-print.h
 *
 * Created on: Mar 8, 2020
 * Author: Oleg Strelnikov
 */

#ifndef MATRIX_PRINT_H_
#define MATRIX_PRINT_H_

#include <iostream>
#include <iomanip>

#include "matrix.h"

template<typename Container, size_t width = 10, char lb = '[', char rb = ']'>
inline std::ostream& print_vector(Container const& v) {
	auto const v_size = std::size(v);
	if (v_size > 0) {
		for (auto i = decltype(v_size){}; i < v_size; ++i) {
			std::cout << lb << std::setw(width) << v[i] << rb << '\n';
		}
	} else {
		std::cout << lb << rb << '\n';
	}
	return std::cout;
}

template<typename T, size_t width = 10, char lb = '[', char rb = ']', char sep = ' '>
inline std::ostream& print_matrix(matrix<T> const& m) {
	auto const m_height = m.height();
	auto const m_width = m.width();
	if (m_height > 0) {
		for (auto i = decltype(m_height){}; i < m_height; ++i) {
			std::cout << lb << sep;
			for (auto j = decltype(m_width){}; j < m_width; ++j) {
				std::cout << std::setw(width) << m[i][j] << sep;
			}
			std::cout << rb << '\n';
		}
	} else {
		std::cout << lb << rb << '\n';
	}
	return std::cout;
}

template<typename L, typename R, size_t width = 10, char lb = '[', char rb = ']', char sep = ' '>
inline std::ostream& print_matrix_multiplication(matrix<L> const& lhs, matrix<R> const& rhs) {
	auto const p = lhs*rhs;
	std::ostream& os = std::cout;
	auto const l_height = lhs.height();
	auto const l_width = lhs.width();
	auto const r_height = rhs.height();
	auto const r_width = rhs.width();
	auto const p_height = p.height();
	auto const p_width = p.width();
	auto const height = std::max(l_height, std::max(r_height, p_height));
	if (height > 0) {
		for (auto i = decltype(height){}; i < height; ++i) {
			if (l_height > i) {
				os << lb << sep;
				for (auto j = decltype(l_width){}; j < l_width; ++j) {
					os << std::setw(width) << lhs[i][j] << sep;
				}
				os << rb;
			} else {
				os << std::string(3 + l_width*(width + 1), ' ');
			}
			os << sep;
			if (r_height > i) {
				os << lb << sep;
				for (auto j = decltype(r_width){}; j < r_width; ++j) {
					os << std::setw(width) << rhs[i][j] << sep;
				}
				os << rb;
			} else {
				os << std::string(3 + r_width*(width + 1), ' ');
			}
			os << sep;
			if (p_height > i) {
				os << lb << sep;
				for (auto j = decltype(p_width){}; j < p_width; ++j) {
					os << std::setw(width) << p[i][j] << sep;
				}
				os << rb;
			}
			os << "\n";
		}
	} else {
		os << lb << rb << sep << lb << rb << sep << lb << rb << '\n';
	}
	return os;
} //print_matrix_multiplication()

#endif /* MATRIX_PRINT_H_ */
