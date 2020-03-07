/*
 * deutsch-demo.cpp
 *
 *  Created on: Mar 7, 2020
 *      Author: ois
 */


#include <iostream>
#include <vector>
#include <complex>
#include <iterator>
#include <iomanip>

template<typename Container, size_t width = 10, char lb = '[', char rb = ']'> std::ostream& print_vector(Container const& v) {
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

template<typename T> class matrix {

	struct IsScalar {
		typedef int Scalar;
	};
	struct IsVector {
		typedef int Vector;
	};
	template<typename V> struct Is : IsScalar {};
	template<typename V, typename A> struct Is<std::vector<V, A>> : IsVector {};

public:
	template<typename Container, typename Is<typename Container::value_type>::Vector = 0> matrix(Container const& u) {
		data_.reserve(u.size());
		for (auto const& c: u) {
			data_.emplace_back(std::begin(c), std::end(c));
		}
	}
	template<typename U = T> matrix(std::initializer_list<std::initializer_list<U>> const& u) : data_(std::begin(u), std::end(u)) {}
	template<size_t M, size_t N, typename U = T> matrix(U const (&u)[M][N]) {
		data_.reserve(M);
		for (size_t i = 0; i < M; ++i) {
			data_.emplace_back(u[i], u[i] + N);
		}
	}
	template<typename Container, typename Is<typename Container::value_type>::Scalar = 0> matrix(Container const& u) {
		data_.reserve(u.size());
		for (size_t i = 0; i < u.size(); ++i) {
			data_.emplace_back(1, u[i]);
		}
	}
	matrix() = default;
	matrix(matrix const&) = default;
	matrix(matrix&&) = default;
	matrix& operator=(matrix const&) = default;
	matrix& operator=(matrix&&) = default;
	~matrix() = default;

	constexpr auto height() const {
		return data_.size();
	}
	constexpr auto width() const {
		return height() ? data_[0].size() : typename std::vector<T>::size_type();
	}
	constexpr auto operator[](size_t index) const {
		return data_[index];
	}
	typename std::vector<std::vector<T>>::reference operator[](size_t index) { //todo: return type
		return data_[index];
	}

	template<typename R = T, typename U = T> matrix<R> operator*(matrix<U> const& rhs) const {
		if (width() != rhs.height()) {
			throw std::runtime_error("matrix dimensions mismatch");
		}
		matrix<R> result(height());
		for (size_t i = 0; i < height(); ++i) {
			result.data_[i].resize(rhs.width());
			for (size_t j = 0; j < rhs.width(); ++j) {
				for (size_t k = 0; k < width(); ++k) {
					result.data_[i][j] += data_[i][k]*rhs.data_[k][j];
				}
			}
		}
		return result;
	}

private:
	typedef std::vector<T> vector_type;
	typedef std::vector<vector_type> matrix_type;
	matrix(typename matrix_type::size_type h) : data_(h) {}
	matrix_type data_;
};

template<typename T, size_t width = 10, char lb = '[', char rb = ']', char sep = ' '>
std::ostream& print_matrix(matrix<T> const& m) {
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
std::ostream& print_matrix_multiplication(matrix<L> const& lhs, matrix<R> const& rhs) {
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

void test() {
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

int main() {
	test();
	unary funcs[] = {f0, f1, f2, f3};
	for (auto const f: funcs) {
		std::cout << "function [";
		print_function(f) << "] is " << (is_constant(f) ? "" : "not ") << "constant\n";
	}
	return 0;
}
