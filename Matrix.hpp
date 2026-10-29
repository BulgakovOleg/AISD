#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <algorithm>
#include <cstddef>
#include <iostream>
#include <random>
#include <stdexcept>
#include <complex>
#include <type_traits>


template <typename T>
struct RandomGenerator {
    static T generate(T min_val, T max_val, std::mt19937& gen) {
        if constexpr (std::is_integral_v<T>) {
            std::uniform_int_distribution<T> dist(min_val, max_val);
            return dist(gen);
        } else {
            std::uniform_real_distribution<T> dist(min_val, max_val);
            return dist(gen);
        }
    }
};


template <typename T>
struct RandomGenerator<std::complex<T>> {
    static std::complex<T> generate(std::complex<T> min_val, std::complex<T> max_val, std::mt19937& gen) {
        if constexpr (std::is_integral_v<T>) {
            std::uniform_int_distribution<T> dist_real(min_val.real(), max_val.real());
            std::uniform_int_distribution<T> dist_imag(min_val.imag(), max_val.imag());
            return std::complex<T>(dist_real(gen), dist_imag(gen));
        } else {
            std::uniform_real_distribution<T> dist_real(min_val.real(), max_val.real());
            std::uniform_real_distribution<T> dist_imag(min_val.imag(), max_val.imag());
            return std::complex<T>(dist_real(gen), dist_imag(gen));
        }
    }
};

template <typename T>
bool are_elements_equal(const T& a, const T& b, double eps) {
    if constexpr (std::is_floating_point_v<T>) {
        const double diff = std::abs(static_cast<double>(a) - static_cast<double>(b));
        const double scale = std::max({1.0, std::abs(static_cast<double>(a)), std::abs(static_cast<double>(b))});
        return diff <= eps * scale;
    } else {
        return a == b;
    }
}


template <typename T>
bool are_elements_equal(const std::complex<T>& a, const std::complex<T>& b, double eps) {
    return are_elements_equal(a.real(), b.real(), eps) && are_elements_equal(a.imag(), b.imag(), eps);
}


template <typename T>
class Matrix {
private:
    std::size_t rows_;
    std::size_t cols_;
    T* data_;

    void fill_with(const T& value) {
        for (std::size_t i = 0; i < rows_ * cols_; ++i) {
            data_[i] = value;
        }
    }
public:
    static constexpr double kEps = 1e-5;

    Matrix(std::size_t rows, std::size_t cols, const T& fill_value = T()) 
        : rows_(rows), cols_(cols), data_(nullptr) {
        if (rows_ == 0 || cols_ == 0) {
            throw std::invalid_argument("Matrix dimensions must be greater than zero.");
        }
        data_ = new T[rows_ * cols_];
        fill_with(fill_value);
    }

    Matrix(std::size_t rows, std::size_t cols, const T& min_val, const T& max_val) 
        : rows_(rows), cols_(cols), data_(nullptr) {
        if (rows_ == 0 || cols_ == 0) {
            throw std::invalid_argument("Matrix dimensions must be greater than zero.");
        }
        data_ = new T[rows_ * cols_];
        
        std::random_device rd;
        std::mt19937 gen(rd());

        for (std::size_t i = 0; i < rows_ * cols_; ++i) {
            data_[i] = RandomGenerator<T>::generate(min_val, max_val, gen);
        }
    }

    ~Matrix() {
        delete[] data_;
    }

    size_t get_rows() const { return rows_; }
    size_t get_cols() const { return cols_; }

    Matrix(const Matrix& other) 
        : rows_(other.rows_), cols_(other.cols_), data_(new T[other.rows_ * other.cols_]) {
        for (std::size_t i = 0; i < rows_ * cols_; ++i) {
            data_[i] = other.data_[i];
        }
    }

    Matrix& operator=(const Matrix& other) {
        if (this != &other) {
            T* new_data = new T[other.rows_ * other.cols_];
            for (size_t i = 0; i < other.rows_ * other.cols_; ++i) {
                new_data[i] = other.data_[i];
            }
            delete[] data_;
            data_ = new_data;
            rows_ = other.rows_;
            cols_ = other.cols_;
        }
        return *this;
    }

    T& operator()(std::size_t r, std::size_t c) {
        if (r >= rows_ || c >= cols_) {
            throw std::out_of_range("Index out of bounds.");
        }
        return data_[r * cols_ + c];
    }

    const T& operator()(std::size_t r, std::size_t c) const {
        if (r >= rows_ || c >= cols_) {
            throw std::out_of_range("Index out of bounds.");
        }
        return data_[r * cols_ + c];
    }

    Matrix operator+(const Matrix& other) const {
        if (rows_ != other.rows_ || cols_ != other.cols_) {
            throw std::invalid_argument("Matrix dimensions must match for addition.");
        }
        Matrix result(rows_, cols_);
        for (std::size_t i = 0; i < rows_; ++i) {
            for (std::size_t j = 0; j < cols_; ++j) {
                result(i, j) = (*this)(i, j) + other(i, j);
            }
        }
        return result;
    }

    Matrix operator-(const Matrix& other) const {
         return *this + other * T(-1);
    }

    Matrix operator*(const T& scalar) const {
        Matrix result(rows_, cols_);
        for (size_t i = 0; i < rows_; ++i) {
            for (size_t j = 0; j < cols_; ++j) {
                result(i, j) = (*this)(i, j) * scalar;
            }
        }
        return result;
    }

    friend Matrix operator*(const T& scalar, const Matrix& matrix) {
        return matrix * scalar;
    }

     Matrix operator*(const Matrix& other) const {
        if (cols_ != other.rows_) {
            throw std::invalid_argument("Matrix dimensions mismatch for multiplication.");
        }
        Matrix result(rows_, other.cols_, T(0));
        for (std::size_t i = 0; i < rows_; ++i) {
            for (std::size_t k = 0; k < cols_; ++k) {
                for (std::size_t j = 0; j < other.cols_; ++j) {
                    result(i, j) += (*this)(i, k) * other(k, j);
                }
            }
        }
        return result;
    }

    Matrix operator/(const T& scalar) const {
       if (are_elements_equal(scalar, T(0), kEps)) {
            throw std::invalid_argument("Division by zero.");
        }
        Matrix result(rows_, cols_);
        for (std::size_t i = 0; i < rows_; ++i) {
            for (std::size_t j = 0; j < cols_; ++j) {
                result(i, j) = (*this)(i, j) / scalar;
            }
        }
        return result;
    }

    bool operator==(const Matrix& other) const {
        if (rows_ != other.rows_ || cols_ != other.cols_) {
            return false;
        }
        for (std::size_t i = 0; i < rows_; ++i) {
            for (std::size_t j = 0; j < cols_; ++j) {
                if (!are_elements_equal((*this)(i, j), other(i, j), kEps)) {
                    return false;
                }
            }
        }
        return true;
    }

    bool operator!=(const Matrix& other) const {
        return !(*this == other);
    }

    T calculate_trace() const {
        if (rows_ != cols_) {
            throw std::logic_error("Trace can only be calculated for square matrices.");
        }
        T total_sum = T(0);
        for (std::size_t i = 0; i < rows_; ++i) {
            total_sum += (*this)(i, i);
        }
        return total_sum;
    }

    friend std::ostream& operator<<(std::ostream& os, const Matrix& matrix) {
        for (std::size_t i = 0; i < matrix.rows_; ++i) {
            for (std::size_t j = 0; j < matrix.cols_; ++j) {
                os << matrix(i, j) << "\t";
            }
            os << "\n";
        }
        return os;
    }
};


#endif



    

