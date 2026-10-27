
#include "matrix.hpp"

template <typename T>
T calculate_determinant_3x3(const Matrix<T>& matrix) {
    if (matrix.get_rows() != 3 || matrix.get_cols() != 3) {
        throw std::invalid_argument("Determinant can only be calculated for 3x3 matrices.");
    }
    return matrix(0,0)*matrix(1,1)*matrix(2,2) + matrix(0,1)*matrix(1,2)*matrix(2,0) + matrix(0,2)*matrix(1,0)*matrix(2,1) -
           matrix(0,2)*matrix(1,1)*matrix(2,0) - matrix(0,0)*matrix(1,2)*matrix(2,1) - matrix(0,1)*matrix(1,0)*matrix(2,2);
}
