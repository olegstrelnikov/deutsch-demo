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

template<typename T> class vector {
public:
	template<typename Container> vector(Container const& u) : data_(std::begin(u), std::end(u)) {}
	template<typename U = T> vector(std::initializer_list<U> const& u) : data_(std::begin(u), std::end(u)) {}
	template<size_t N, typename U = T> vector(U const (&u)[N]) : data_(u, u + N) {}

	vector() = default;
	vector(vector const&) = default;
	vector(vector&&) = default;
	vector& operator=(vector const&) = default;
	vector& operator=(vector&&) = default;
	~vector() = default;

	constexpr auto size() const {
		return data_.size();
	}
	constexpr auto operator[](size_t index) const {
		return data_[index];
	}
	typename std::vector<T>::reference operator[](size_t index) { //todo: return type
		return data_[index];
	}
private:
	std::vector<T> data_;
};

template<typename T, size_t width = 10, char lb = '[', char rb = ']'> std::ostream& print_vector(vector<T> const& v) {
	auto const v_size = v.size();
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
public:
	template<typename Container> matrix(Container const& u) : data_(std::begin(u), std::end(u)) {}
	template<typename U = T> matrix(std::initializer_list<std::initializer_list<U>> const& u) : data_(std::begin(u), std::end(u)) {}
	template<size_t M, size_t N, typename U = T> matrix(U const (&u)[M][N]) {
		data_.reserve(M);
		for (size_t i = 0; i < M; ++i) {
			data_.emplace_back(u[i], u[i] + N);
		}
	}
	template<typename U = T> matrix(vector<U> const& u) {
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
	typename std::vector<std::vector<T>>::reference operator[](size_t index) {
		return data_[index];
	}
private:
	std::vector<std::vector<T>> data_;
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
	std::vector<int> vv{1,2,3};
	float vvv[] = {5.5f,6.5f};
	vector<double> v1({1.0,2.0,3.0,4.0});
	vector<long> v2(vv);
	vector<float> v3(vvv);
	vector<int> v4{10,11,12,13,14,15};
	print_vector(v1) << "\n";
	print_vector(v2) << "\n";
	print_vector(v3) << "\n";
	v4[0] = 25;
	print_vector(v4) << "\n";
	print_vector(vector<char>()) << "\n";
	print_vector<int>({5.0f,6.0f}) << "\n";
	std::vector<std::vector<int>> mm{{1,2,3},{4,5,6}};
	double mmm[4][5] = {{1,2,3,4,5},{2,3,4,5,6},{3,4,5,6,7},{4,5,6,7,8}};
	matrix<double> m1{{1.1,2.2},{3.3,4.4},{5.5,6.6}};
	matrix<int> m2(mm);
	m2[1][2] = 3456;
	matrix<int> m3(mmm);
	matrix<double> m4(v4);
	print_matrix(m1) << "\n";
	print_matrix(m2) << "\n";
	print_matrix(m3) << "\n";
	print_matrix(m4) << "\n";
	print_matrix(matrix<int>({{1,2,3,4,5,6,7,8,9,10}})) << "\n";
	print_matrix(matrix<int>(std::vector<std::vector<int>>(3))) << "\n";
	print_matrix(matrix<int>()) << "\n";
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
