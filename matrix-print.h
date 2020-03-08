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
#include <algorithm>
#include <numeric>

#include "matrix.h"

namespace {

struct PrintedMatrix {
	std::vector<size_t> widths;
	std::vector<std::vector<std::string>> cells;
	size_t max_width;
	char trailer;
	PrintedMatrix(size_t height, size_t width, char t) :
		widths(width), cells(height ? height : 1), max_width(width > 0 ? width + 1 : 2), trailer(t) {
		for (auto& row: cells) {
			row.reserve(width);
		}
	}
};

template<typename T>
inline PrintedMatrix get_widths(matrix<T> const& m, char trailer = ' ') {
	auto const m_height = m.height();
	auto const m_width = m.width();
	PrintedMatrix result(m_height, m_width, trailer);
	std::ostringstream oss;
	size_t size;
	for (auto i = decltype(m_height){}; i < m_height; ++i) {
		for (auto j = decltype(m_width){}; j < m_width; ++j) {
			oss.str("");
			oss << m[i][j];
			size = result.cells[i].emplace_back(oss.str()).size();
			if (size > result.widths[j]) {
				result.max_width -= result.widths[j];
				result.widths[j] = size;
				result.max_width += size;
			}
		}
	}
	return result;
}

inline std::ostream& print_cell(std::string const& cell, size_t width, const char* ws, std::ostream& os) {
	return os.write(ws, width - cell.size()).write(cell.data(), cell.size());
} //print_cell()

inline std::ostream& print_row(std::vector<std::string> const& row, std::vector<size_t> const& widths,
	const char* ws, char sep, char lb, char rb, std::ostream& os) {
	auto const m_width = row.size();
	os << lb;
	if (m_width > 0) {
		print_cell(row[0], widths[0], ws, os);
		for (auto j = decltype(m_width){1}; j < m_width; ++j) {
			print_cell(row[j], widths[j], ws, os.put(sep));
		}
	}
	return os << rb;
} //print_row()

inline std::ostream& print_rows(std::vector<PrintedMatrix> const& r, size_t i, const char* ws, char sep, char lb, char rb, char msep, std::ostream& os) {
	for (auto const& rr: r) {
		if (rr.cells.size() > i) {
			print_row(rr.cells[i], rr.widths, ws, sep, lb, rb, os);
		} else {
			os.write(ws, rr.max_width);
		}
		os << msep; //todo: trailing msep
	}
	return os << '\n';
}

inline std::ostream& print_matrices(std::vector<PrintedMatrix> const& r, char sep, char lb, char rb, char msep, std::ostream& os) {
	auto const m_height = std::max_element(r.begin(), r.end(), [](auto const& lhs, auto const& rhs){return lhs.cells.size() < rhs.cells.size();})->cells.size();
	const std::string ws(std::max_element(r.begin(), r.end(), [](auto const& lhs, auto const& rhs){return lhs.max_width < rhs.max_width;})->max_width, ' ');
	for (auto const& rr: r) {
		print_row(rr.cells[0], rr.widths, ws.data(), sep, lb, rb, os);
		os << rr.trailer; //todo: trailing msep
	}
	os << '\n';
	for (auto i = decltype(m_height){1}; i < m_height; ++i) {
		print_rows(r, i, ws.data(), sep, lb, rb, msep, os);
	}
	return os;
} //print_matrices()

} //namespace

template<typename T>
inline std::ostream& print_matrix(matrix<T> const& m, char sep = ' ', char lb = '[', char rb = ']', std::ostream& os = std::cout) {
	return print_matrices(std::vector<PrintedMatrix>(1, get_widths(m)), sep, lb, rb, ' ', os);
} //print_matrix()

template<typename V>
inline std::ostream& print_vector(V const& v, char lb = '[', char rb = ']', std::ostream& os = std::cout) {
	return print_matrix(matrix<typename std::iterator_traits<decltype(std::begin(v))>::value_type>(v), '!', lb, rb, os);
} //print_vector()

template<typename L, typename R>
inline std::ostream& print_matrix_multiplication(matrix<L> const& lhs, matrix<R> const& rhs,
	char sep = ' ', char lb = '[', char rb = ']', char msep = ' ', std::ostream& os = std::cout) {
	return print_matrices({get_widths(lhs, '*'), get_widths(rhs, '='), get_widths(lhs*rhs)}, sep, lb, rb, ' ', os);
} //print_matrix_multiplication()

#endif /* MATRIX_PRINT_H_ */
