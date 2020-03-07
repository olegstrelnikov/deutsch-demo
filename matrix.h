/*
 * matrix.h
 *
 * Created on: Mar 8, 2020
 * Author: Oleg Strelnikov
 */

#ifndef MATRIX_H_
#define MATRIX_H_

#include <vector>

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

#endif /* MATRIX_H_ */
