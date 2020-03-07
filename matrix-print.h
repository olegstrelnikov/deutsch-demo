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
#include <sstream>

#include "matrix.h"

struct PrintedMatrix {
	std::vector<size_t> widths;
	std::vector<std::vector<std::string>> cells;
	size_t max_width;
	PrintedMatrix(size_t height, size_t width) : widths(width), cells(height), max_width() {
		for (auto& row: cells) {
			row.reserve(width);
		}
	}
};

template<typename T>
static inline PrintedMatrix get_widths(matrix<T> const& m) {
	auto const m_height = m.height();
	auto const m_width = m.width();
	PrintedMatrix result(m_height, m_width);
	std::ostringstream oss;
	size_t size;
	for (auto i = decltype(m_height){}; i < m_height; ++i) {
		for (auto j = decltype(m_width){}; j < m_width; ++j) {
			oss.str("");
			oss << m[i][j];
			size = result.cells[i].emplace_back(oss.str()).size();
			if (size > result.widths[j]) {
				result.widths[j] = size;
				if (size > result.max_width) {
					result.max_width = size;
				}
			}
		}
	}
	return result;
}

template<typename T, char lb = '[', char rb = ']', char sep = ' '> inline std::ostream& print_matrix(matrix<T> const& m) {
	std::ostream& os = std::cout;
	auto r = get_widths(m);
	auto const m_height = m.height();
	auto const m_width = m.width();
	const std::string ws(r.max_width, ' ');
	if (m_height > 0) {
		for (auto i = decltype(m_height){}; i < m_height; ++i) {
			os << lb << sep;
			auto& row = r.cells[i];
			for (auto j = decltype(m_width){}; j < m_width; ++j) {
				auto& cell = row[j];
				os.write(ws.data(), r.widths[j] - cell.size()).write(cell.data(), cell.size()) << sep;
			}
			os << rb << '\n';
		}
	} else {
		os << lb << rb << '\n';
	}
	return os;
}

template<typename Container, char lb = '[', char rb = ']', char sep = ' '> inline std::ostream& print_vector(Container const& v) {
	return print_matrix(matrix<typename std::iterator_traits<decltype(std::begin(v))>::value_type>(v));
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
