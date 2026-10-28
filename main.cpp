#include "matrix.hpp"


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
