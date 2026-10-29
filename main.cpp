#include "matrix.hpp"
#include <iostream>
#include <string>
#include <complex>
#include <stdexcept>

template <typename T>
T calculate_determinant_3x3(const Matrix<T>& matrix) {
    if (matrix.get_rows() != 3 || matrix.get_cols() != 3) {
        throw std::invalid_argument("Determinant can only be calculated for 3x3 matrices.");
    }
    return matrix(0,0)*matrix(1,1)*matrix(2,2) + matrix(0,1)*matrix(1,2)*matrix(2,0) + matrix(0,2)*matrix(1,0)*matrix(2,1) -
           matrix(0,2)*matrix(1,1)*matrix(2,0) - matrix(0,0)*matrix(1,2)*matrix(2,1) - matrix(0,1)*matrix(1,0)*matrix(2,2);
}

template <typename T>
Matrix<T> solve_system_3x3(const Matrix<T>& a_matrix, const Matrix<T>& b_vector) {
    if (a_matrix.get_rows() != 3 || a_matrix.get_cols() != 3) {
        throw std::invalid_argument("Matrix A must be 3x3.");
    }
    if (b_vector.get_rows() != 3 || b_vector.get_cols() != 1) {
        throw std::invalid_argument("Vector b must be 3x1.");
    }

    T det_a = calculate_determinant_3x3(a_matrix);
    
    if (are_elements_equal(det_a, T(0), Matrix<T>::kEps)) {
        throw std::logic_error("The system does not have a unique solution (determinant is zero).");
    }

    Matrix<T> a_x = a_matrix;
    Matrix<T> a_y = a_matrix;
    Matrix<T> a_z = a_matrix;

    for (size_t i = 0; i < 3; ++i) {
        a_x(i, 0) = b_vector(i, 0);
        a_y(i, 1) = b_vector(i, 0);
        a_z(i, 2) = b_vector(i, 0);
    }

    Matrix<T> x_result(3, 1);
    x_result(0, 0) = calculate_determinant_3x3(a_x) / det_a;
    x_result(1, 0) = calculate_determinant_3x3(a_y) / det_a;
    x_result(2, 0) = calculate_determinant_3x3(a_z) / det_a;

    return x_result;
}

template <typename T>
void demonstrate_all_operations(const T& min_val, const T& max_val) {
    const T fill_value = T(2);
    const T scalar = T(2);
    const T written_value = T(7);

    Matrix<T> random_matrix(3, 3, min_val, max_val);
    Matrix<T> other_matrix(3, 3, min_val, max_val);
    Matrix<T> filled_matrix(3, 3, fill_value);

    filled_matrix(0, 1) = written_value;

    Matrix<T> copied_matrix(random_matrix);
    Matrix<T> assigned_matrix(1, 1);
    assigned_matrix = random_matrix;
    
    std::cout << "Matrix A (" << random_matrix.get_rows()
              << "x" << random_matrix.get_cols() << "):\n" << random_matrix;
    std::cout << "Matrix B (" << other_matrix.get_rows()
              << "x" << other_matrix.get_cols() << "):\n" << other_matrix;
    std::cout << "Filled matrix:\n" << filled_matrix;
    std::cout << "Copy of A:\n" << copied_matrix;
    std::cout << "Assigned from A:\n" << assigned_matrix;
    std::cout << "A + B:\n" << (random_matrix + other_matrix);
    std::cout << "A - B:\n" << (random_matrix - other_matrix);
    std::cout << "A * B:\n" << (random_matrix * other_matrix);
    std::cout << "A * 2:\n" << (random_matrix * scalar);
    std::cout << "2 * A:\n" << (scalar * random_matrix);
    std::cout << "A / 2:\n" << (random_matrix / scalar);
    std::cout << "Trace of A: " << random_matrix.calculate_trace() << "\n";
    std::cout << std::boolalpha;
    std::cout << "Copy equals A: " << (copied_matrix == random_matrix) << "\n";
    std::cout << "Assigned equals A: " << (assigned_matrix == random_matrix) << "\n";
    std::cout << "A != B: " << (random_matrix != other_matrix) << "\n";
}

template <typename T>
void run_test_for_type(const std::string& type_name, const T& min_val, const T& max_val) {
    std::cout << "========================================\n";
    std::cout << "TEST FOR DATA TYPE: " << type_name << "\n";
    std::cout << "========================================\n";

    try {
        demonstrate_all_operations<T>(min_val, max_val);

        Matrix<T> a_matrix(3, 3, min_val, max_val);
        Matrix<T> b_vector(3, 1, min_val, max_val);
        std::cout << "Task 1. Matrix A:\n" << a_matrix;
        std::cout << "Task 1. Vector b:\n" << b_vector;

        Matrix<T> x_result = solve_system_3x3(a_matrix, b_vector);
        std::cout << "Task 1. Solution x:\n" << x_result;

        Matrix<T> b_prime = a_matrix * x_result;
        std::cout << "Task 1. Verification A * x:\n" << b_prime;

        if constexpr (std::is_integral_v<T>) {
            std::cout << "[Info]: For int, A * x may differ from b because of integer division.\n";
        }

        if (b_prime == b_vector) {
            std::cout << "Result: SUCCESS (A * x equals b)\n\n";
        } else {
            std::cout << "Result: FAILED (possible for int or a nearly singular matrix)\n\n";
        }
    } catch (const std::exception& e) {
        std::cout << "Exception caught: " << e.what() << "\n\n";
    }
}

int main() {
    run_test_for_type<int>("int", 1, 10);
    run_test_for_type<float>("float", 1.0f, 5.0f);
    run_test_for_type<double>("double", 1.0, 5.0);
    run_test_for_type<std::complex<float>>(
        "std::complex<float>",
        std::complex<float>(1.0f, 1.0f),
        std::complex<float>(4.0f, 4.0f)
    );
    run_test_for_type<std::complex<double>>(
        "std::complex<double>",
        std::complex<double>(1.0, 1.0),
        std::complex<double>(4.0, 4.0)
    );
    return 0;
}
