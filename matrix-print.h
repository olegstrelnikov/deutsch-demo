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
#include <cassert>

#include "matrix.h"

namespace {

struct RenderedMatrix {
	std::vector<size_t> widths;
	std::vector<std::vector<std::string>> cells;
	size_t max_width;
	char trailer;
	RenderedMatrix(size_t height, size_t width, char t) :
		widths(width), cells(height ? height : 1), max_width(width > 0 ? width + 1 : 2), trailer(t) {
		for (auto& row: cells) {
			row.reserve(width);
		}
	}
};

template<typename T>
inline RenderedMatrix render(matrix<T> const& m, char trailer = ' ') {
	auto const m_height = m.height();
	auto const m_width = m.width();
	RenderedMatrix result(m_height, m_width, trailer);
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


inline std::ostream& print_row(RenderedMatrix const& rr, size_t row_number, const char* ws, char sep, char lb, char rb, char msep, std::ostream& os) {
	if (rr.cells.size() > row_number) {
		print_row(rr.cells[row_number], rr.widths, ws, sep, lb, rb, os);
	} else {
		os.write(ws, rr.max_width); //todo: trailing white spaces
	}
	return os;
}

inline std::ostream& print_rows(std::vector<RenderedMatrix> const& r, size_t row_number, const char* ws, char sep, char lb, char rb, char msep, std::ostream& os) {
	assert(r.size() > 0);
	auto const last_matrix = r.size() - 1;
	for (auto k = decltype(last_matrix){}; k < last_matrix; ++k) {
		char trailers[2] = {r[k].trailer, msep};
		print_row(r[k], row_number, ws, sep, lb, rb, msep, os).put(trailers[row_number > 0]);
	}
	print_row(r[last_matrix], row_number, ws, sep, lb, rb, msep, os);
	return os << '\n';
} //print_rows()

inline std::ostream& print_matrices(std::vector<RenderedMatrix> const& r, char sep, char lb, char rb, char msep, std::ostream& os) {
	if (!r.empty()) {
		auto const m_height = std::max_element(r.begin(), r.end(), [](auto const& lhs, auto const& rhs){return lhs.cells.size() < rhs.cells.size();})->cells.size();
		const std::string ws(std::max_element(r.begin(), r.end(), [](auto const& lhs, auto const& rhs){return lhs.max_width < rhs.max_width;})->max_width, ' ');
		for (auto row_number = decltype(m_height){}; row_number < m_height; ++row_number) {
			print_rows(r, row_number, ws.data(), sep, lb, rb, msep, os);
		}
	}
	return os;
} //print_matrices()

} //namespace

template<typename T>
inline std::ostream& print_matrix(matrix<T> const& m, char sep = ' ', char lb = '[', char rb = ']', std::ostream& os = std::cout) {
	return print_matrices(std::vector<RenderedMatrix>(1, render(m)), sep, lb, rb, '!', os);
} //print_matrix()

template<typename V>
inline std::ostream& print_vector(V const& v, char lb = '[', char rb = ']', std::ostream& os = std::cout) {
	return print_matrix(matrix<typename std::iterator_traits<decltype(std::begin(v))>::value_type>(v), '!', lb, rb, os);
} //print_vector()

template<typename L, typename R>
inline std::ostream& print_matrix_multiplication(matrix<L> const& lhs, matrix<R> const& rhs,
	char sep = ' ', char lb = '[', char rb = ']', char msep = ' ', std::ostream& os = std::cout) {
	return print_matrices({render(lhs, '*'), render(rhs, '='), render(lhs*rhs, '?')}, sep, lb, rb, ' ', os);
} //print_matrix_multiplication()

#endif /* MATRIX_PRINT_H_ */
