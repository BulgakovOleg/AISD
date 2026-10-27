template <typename T>
class Matrix {
private:
    size_t rows_;
    size_t cols_;
    T* data_;
public:

    Matrix(size_t rows, size_t cols, const T& fill_value = T()) 
        : rows_(rows), cols_(cols), data_(nullptr) {
        if (rows_ == 0 || cols_ == 0) {
            throw std::invalid_argument("Matrix dimensions must be greater than zero.");
        }
        data_ = new T[rows_ * cols_];
        for (size_t i = 0; i < rows_ * cols_; ++i) {
            data_[i] = fill_value;
        }
    }

    Matrix(size_t rows, size_t cols, const T& min_val, const T& max_val) 
        : rows_(rows), cols_(cols), data_(nullptr) {
        if (rows_ == 0 || cols_ == 0) {
            throw std::invalid_argument("Matrix dimensions must be greater than zero.");
        }
        data_ = new T[rows_ * cols_];
        
        std::random_device rd;
        std::mt19937 gen(rd());

        for (size_t i = 0; i < rows_ * cols_; ++i) {
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
        for (size_t i = 0; i < rows_ * cols_; ++i) {
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

    T& operator()(size_t r, size_t c) {
        if (r >= rows_ || c >= cols_) {
            throw std::out_of_range("Index out of bounds.");
        }
        return data_[r * cols_ + c];
    }

    const T& operator()(size_t r, size_t c) const {
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
        for (size_t i = 0; i < rows_; ++i) {
            for (size_t j = 0; j < cols_; ++j) {
                result(i, j) = (*this)(i, j) + other(i, j);
            }
        }
        return result;
    }

    Matrix operator-(const Matrix& other) const {
        if (rows_ != other.rows_ || cols_ != other.cols_) {
            throw std::invalid_argument("Matrix dimensions must match for subtraction.");
        }
        Matrix result(rows_, cols_);
        for (size_t i = 0; i < rows_; ++i) {
            for (size_t j = 0; j < cols_; ++j) {
                result(i, j) = (*this)(i, j) - other(i, j);
            }
        }
        return result;
    }



    

