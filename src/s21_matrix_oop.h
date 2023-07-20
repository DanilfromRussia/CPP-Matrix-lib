/* Copyright 2022 coruscan School21 Kazan */

#ifndef SRC_S21_MATRIX_OOP_H_
#define SRC_S21_MATRIX_OOP_H_

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <stdexcept>

// Class S21Matrix contains basic arithmetic matrix functions.
// An example of using each method can be found in "src/s21_test.cc"
class S21Matrix {
 public:
  S21Matrix();
  explicit S21Matrix(int rows, int cols);      // parameterized constructor
  S21Matrix(const S21Matrix& other) noexcept;  // copy constructor
  S21Matrix(S21Matrix&& other) noexcept;       // move constructor
  ~S21Matrix();                                // destructor
  int get_rows() { return rows_; }             // rows accessor
  int get_cols() { return cols_; }             // cols accessor
  void set_rows(int row);
  void set_cols(int col);

  // some operators overloads
  S21Matrix& operator=(const S21Matrix& other);
  S21Matrix& operator=(S21Matrix&& other);
  S21Matrix operator-(const S21Matrix& other);
  S21Matrix operator*(const S21Matrix& other);
  S21Matrix operator*(const double& num);
  bool operator==(const S21Matrix& other);
  S21Matrix& operator+=(const S21Matrix& other);
  S21Matrix& operator-=(const S21Matrix& other);
  S21Matrix operator+(const S21Matrix& other);
  S21Matrix& operator*=(const S21Matrix& other);
  S21Matrix& operator*=(const double& num);
  double& operator()(const int row, const int col);

  // some public methods
  double Determinant(char flag, int row, int col);
  bool EqMatrix(const S21Matrix& other);
  void SumMatrix(const S21Matrix& other);
  void SubMatrix(const S21Matrix& other);
  void MulNumber(const double num);
  void MulMatrix(const S21Matrix& other);
  S21Matrix Transpose();
  S21Matrix CalcComplements();
  S21Matrix InverseMatrix();

 private:
  //  secondary functions
  int rows_ = 0, cols_ = 0;
  double** matrix_ = nullptr;
  void Checker(const S21Matrix& other);
  void SetChangeRowsAndCols(int new_rows, int new_cols);  // alloc mutator
  void Create(int rows, int cols) noexcept;
  double SecOrdInvHelper(double** matr, int row, int col) noexcept;
  void CopyElem(const S21Matrix& other) noexcept;
};

#endif  // SRC_S21_MATRIX_OOP_H_
