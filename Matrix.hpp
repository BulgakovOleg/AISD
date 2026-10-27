template <typename T>
class Matrix {
private:
    size_t rows_;
    size_t cols_;
    T* data_;
public:
    ~Matrix() {
        delete[] data_;
    }
    
