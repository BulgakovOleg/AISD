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
    
