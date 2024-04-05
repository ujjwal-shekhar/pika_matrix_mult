#pragma once

#include <pika/execution.hpp>
#include <pika/functional/bind_front.hpp>
#include <pika/init.hpp>
#include <pika/latch.hpp>
#include <pika/thread.hpp>

#include <cstdlib>
#include <functional>
#include <iostream>
#include <utility>
#include <vector>

namespace ex = pika::execution::experimental;
namespace tt = pika::this_thread::experimental;
void wait_for_latch(pika::latch& l) { l.arrive_and_wait(); }

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

        // Initialize the pika runtime
        pika::start(0, nullptr);

        /////////////////////////////////////////////////////////////////////////////////
        //////////////////////////// Parallelizing over rows ////////////////////////////
        // Define a function to compute the product of a row and a column
        auto compute_element = [&](int i, int j) {
            T sum = 0;
            for (int k = 0; k < cols_; ++k) {
                sum += (*this)(i, k) * other(k, j);
            }
            return sum;
        };

        // Define the sender to compute the product of a row and all columns
        auto compute_row_sender = [&](int startRow, int endRow) {
            return ex::just() | ex::then([=, &result, &other]() {
                for (int i = startRow; i < endRow; ++i) {
                    for (int j = 0; j < newCols; ++j) {
                        result(i, j) = compute_element(i, j);
                    }
                }
            });
        };

        // Define the number of threads to use
        const int num_threads = std::min(16, std::min(newRows, newCols));

        for (int i = 0; i < newRows; i += newRows / num_threads) {
            int endRow = std::min(i + newRows / num_threads, newRows);
            auto sender = compute_row_sender(i, endRow);
            ex::start_detached(std::move(sender));
        }

        // Finalize the pika runtime
        pika::finalize();
        pika::stop();

        ///////////////////////////////////////////////////////////////////////////////
        /////////////////////// Parallelizing over sub matrices ///////////////////////
        // const int maxSubMatrices = 16; // Maximum number of sub-matrices

        // // Calculate the number of rows and columns per sub-matrix
        // int rowsPerSubMatrix = (newRows + maxSubMatrices - 1) / maxSubMatrices;
        // int colsPerSubMatrix = (newCols + maxSubMatrices - 1) / maxSubMatrices;

        // // Define a sender to compute a sub-matrix of the result matrix
        // auto compute_sub_matrix_sender = [&](int startRow, int endRow, int startCol, int endCol) {
        //     return ex::just() | ex::then([=, &result, &other]() {
        //         for (int i = startRow; i < endRow; ++i) {
        //             for (int j = startCol; j < endCol; ++j) {
        //                 T sum = 0;
        //                 for (int k = 0; k < cols_; ++k) {
        //                     sum += (*this)(i, k) * other(k, j);
        //                 }
        //                 result(i, j) = sum;
        //             }
        //         }
        //     });
        // };

        // // Create senders for sub-matrices and execute them asynchronously
        // for (int i = 0; i < newRows; i += rowsPerSubMatrix) {
        //     for (int j = 0; j < newCols; j += colsPerSubMatrix) {
        //         int endRow = std::min(i + rowsPerSubMatrix, newRows);
        //         int endCol = std::min(j + colsPerSubMatrix, newCols);
        //         ex::start_detached(compute_sub_matrix_sender(i, endRow, j, endCol));
        //     }
        // }

        // // Wait for all computations to finish
        // pika::this_thread::yield();

        ////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////// Serialized version //////////////////////////////
        // for (int i = 0; i < newRows; ++i) {
        //     for (int j = 0; j < newCols; ++j) {
        //         result(i, j) = 0;
        //         for (int k = 0; k < cols_; ++k) {
        //             result(i, j) += (*this)(i, k) * other(k, j);
        //         }
        //     }
        // }

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
