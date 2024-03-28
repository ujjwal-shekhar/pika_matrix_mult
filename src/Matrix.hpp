#pragma once

#include <iostream>
#include <vector>
#include <cassert> // Include for assertions

template<typename T>
class Matrix {
public:
    Matrix(int rows, int cols) : rows_(rows), cols_(cols), data_(rows, std::vector<T>(cols)) {}

    int rows() const { return rows_; }
    int cols() const { return cols_; }

    T& operator()(int i, int j) { return data_[i][j]; }
    const T& operator()(int i, int j) const { return data_[i][j]; }

    Matrix<T> operator* (const Matrix<T>& other) const { // Operator for matrix multiplication
        assert(cols_ == other.rows() && "Matrix dimensions mismatch for multiplication");

        int newRows = rows_;
        int newCols = other.cols();
        Matrix<T> result(newRows, newCols);

        for (int i = 0; i < newRows; ++i) {
            for (int j = 0; j < newCols; ++j) {
                result(i, j) = 0;
                for (int k = 0; k < cols_; ++k) {
                    result(i, j) += (*this)(i, k) * other(k, j);
                }
            }
        }

        return result;
    }

    // Custom output and input stream operators
    friend std::ostream& operator<<(std::ostream& os, const Matrix<T>& matrix) {
        for (int i = 0; i < matrix.rows(); ++i) {
            for (int j = 0; j < matrix.cols(); ++j) {
                os << matrix(i, j) << " ";
            }
            os << std::endl;
        }
        return os;
    }

    friend std::istream& operator>>(std::istream& is, Matrix<T>& matrix) {
        for (int i = 0; i < matrix.rows(); ++i) {
            for (int j = 0; j < matrix.cols(); ++j) {
                is >> matrix(i, j);
            }
        }
        return is;
    }

private:
    int rows_;
    int cols_;
    std::vector<std::vector<T>> data_;
};
