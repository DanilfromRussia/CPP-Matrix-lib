/* Copyright 2022 coruscan School21 Kazan */

#include "s21_matrix_oop.h"

void S21Matrix::set_rows(int row) { this->SetChangeRowsAndCols(row, cols_); }

void S21Matrix::set_cols(int col) { this->SetChangeRowsAndCols(rows_, col); }

S21Matrix::S21Matrix() { Create(rows_, cols_); }

void S21Matrix::Create(int rows, int cols) noexcept {
  if (rows > 0 && cols > 0) {
    matrix_ = new double *[rows];
    for (int i = 0; i < rows; i++) {
      matrix_[i] = new double[cols]{};
    }
    rows_ = rows;
    cols_ = cols;
  }
}

S21Matrix::~S21Matrix() {
  if (this->matrix_ != nullptr && this->rows_ > 0 && this->cols_ > 0) {
    for (int i = 0; i < rows_; i++) {
      if (this->matrix_[i] != nullptr) {
        delete this->matrix_[i];
      }
    }
    delete this->matrix_;
  }
}

S21Matrix::S21Matrix(int rows, int cols) : rows_(rows), cols_(cols) {
  if (rows < 1 || cols < 1) {
    throw std::bad_array_new_length();
  } else {
    Create(rows, cols);
  }
}

bool S21Matrix::EqMatrix(const S21Matrix &other) {
  if (this->matrix_ == nullptr || other.matrix_ == nullptr) {
    throw std::bad_alloc();
  } else if (this->cols_ != other.cols_ || this->rows_ != other.rows_) {
    return false;
  } else {
    if (this->matrix_ != other.matrix_) {
      for (int i = 0; i < this->rows_; i++) {
        for (int j = 0; j < this->cols_; j++) {
          // 0.000001 below means precision, this is "double" realization
          if (fabs(this->matrix_[i][j] - other.matrix_[i][j]) > 0.000001) {
            return false;
          }
        }
      }
    }
    return true;
  }
}

void S21Matrix::SumMatrix(const S21Matrix &other) {
  this->Checker(other);  // throw exceptoions
  for (int i = 0; i < this->rows_; i++) {
    for (int j = 0; j < this->cols_; j++) {
      this->matrix_[i][j] += other.matrix_[i][j];
    }
  }
}

void S21Matrix::SubMatrix(const S21Matrix &other) {
  this->Checker(other);  // throw exceptoions
  for (int i = 0; i < this->rows_; i++) {
    for (int j = 0; j < this->cols_; j++) {
      this->matrix_[i][j] -= other.matrix_[i][j];
    }
  }
}

void S21Matrix::MulNumber(const double num) {
  if (this->matrix_ == nullptr) {
    throw std::bad_alloc();
  } else {
    for (int i = 0; i < this->rows_; i++) {
      for (int j = 0; j < this->cols_; j++) {
        this->matrix_[i][j] *= num;
      }
    }
  }
}

void S21Matrix::Checker(const S21Matrix &other) {
  if (matrix_ == nullptr || other.matrix_ == nullptr) {
    throw std::bad_alloc();
  } else if (rows_ != other.rows_ || cols_ != other.cols_) {
    throw std::logic_error("different dimension");
  }
}

void S21Matrix::MulMatrix(const S21Matrix &other) {
  if (matrix_ == nullptr || other.matrix_ == nullptr)
    throw std::bad_alloc();
  else if (this->cols_ < 1 || this->rows_ < 1 || other.cols_ < 1 ||
           other.rows_ < 1)
    throw std::logic_error("impossible multiplication");
  else if (this->cols_ != other.rows_)
    throw std::logic_error("impossible multiplication");
  else {
    S21Matrix tmp(this->rows_, other.cols_);
    for (int i = 0; i < this->rows_; i++) {
      for (int j = 0; j < other.cols_; j++) {
        for (int k = 0; k < this->cols_; k++) {
          tmp.matrix_[i][j] += this->matrix_[i][k] * other.matrix_[k][j];
        }
      }
    }
    *this = tmp;
  }
}

S21Matrix S21Matrix::Transpose() {
  if (matrix_ == nullptr || rows_ < 1 || cols_ < 1) {
    throw std::logic_error("impossible transpose");
  }
  S21Matrix returnable_matrix(cols_, rows_);
  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < cols_; j++) {
      returnable_matrix.matrix_[j][i] = matrix_[i][j];
    }
  }
  return returnable_matrix;
}

// we will get a class object with a ready-made matrix of algebraic additions
S21Matrix S21Matrix::CalcComplements() {
  if (matrix_ == nullptr || rows_ < 1 || cols_ < 1) {
    throw std::logic_error("impossible calculate");
  } else if (this->rows_ != this->cols_) {
    throw std::logic_error("must be square");
  } else {
    S21Matrix returnable_matr(rows_, cols_);
    if (returnable_matr.matrix_ == nullptr) {
      throw std::bad_alloc();
    } else {
      if (rows_ == 1 && matrix_[0][0]) {
        returnable_matr.matrix_[0][0] = 1 / matrix_[0][0];
      } else {
        for (int i = 0; i < rows_; i++) {
          for (int j = 0; j < cols_; j++) {
            if (rows_ == 2) {  // 2x2 matrix below
              returnable_matr.matrix_[i][j] =
                  SecOrdInvHelper(matrix_, i, j) * pow(-1, i + j);
            } else {
              // if matrix 3x3 or huges, multiply matrix[i][j] on (-1)^i+j
              //  will completely automaticly.
              returnable_matr.matrix_[i][j] = this->Determinant('y', i, j);
            }
          }
        }
      }
    }
    return returnable_matr;
  }
}

// If you need to change only 1 value, just duplicate remaining existing one
void S21Matrix::SetChangeRowsAndCols(int new_rows, int new_cols) {
  if (new_rows < 1 || new_cols < 1) throw std::bad_array_new_length();
  if (rows_ != new_rows || cols_ != new_cols) {
    S21Matrix tmp(new_rows, new_cols);
    if (this->matrix_ == nullptr) throw std::bad_alloc();
    int cycle_rows = std::min(new_rows, rows_);
    int cycle_cols = std::min(new_cols, cols_);
    for (int i = 0; i < cycle_rows; i++) {
      for (int j = 0; j < cycle_cols; j++) {
        tmp.matrix_[i][j] = matrix_[i][j];
      }
    }
    *this = tmp;
  }
}

S21Matrix::S21Matrix(S21Matrix &&other) noexcept {
  this->matrix_ = other.matrix_;
  rows_ = other.rows_;
  cols_ = other.cols_;
  other.rows_ = 0;
  other.cols_ = 0;
  other.matrix_ = nullptr;
}

S21Matrix::S21Matrix(const S21Matrix &other) noexcept {
  this->Create(other.rows_, other.cols_);
  this->CopyElem(other);
}

void S21Matrix::CopyElem(const S21Matrix &other) noexcept {
  for (int i = 0; i < other.rows_; i++) {
    for (int j = 0; j < other.cols_; j++) {
      this->matrix_[i][j] = other.matrix_[i][j];
    }
  }
}

// I wanted to write a determinant that can calculate both the entire matrix as
// a whole, using the first row of the matrix to calculate (look for "i <
// rows"), and his overloaded version, calculating matrix[r][c] determinant.
double S21Matrix::Determinant(char flag = 'n', int row = 0, int col = 0) {
  if (matrix_ == nullptr) {
    throw std::bad_alloc();
  } else if (rows_ != cols_) {
    throw std::logic_error("must be square");
  } else {
    if (rows_ == 1) {
      return matrix_[0][0];
    } else if (rows_ == 2) {
      return (matrix_[0][0] * matrix_[1][1]) - (matrix_[0][1] * matrix_[1][0]);
    } else {
      double res{};  // finally sum of determs will be here
      int counter_x = 0, counter_y = 0;
      S21Matrix tmp(rows_ - 1, rows_ - 1);
      while (col < rows_) {  // c - num exception column (default 0)
        for (int j = (row == 0); j < rows_; j++) {  // j - num exception row
          for (int k = 0; k < rows_; k++) {         // k - column num
            if (k != col && j != row) {
              tmp.matrix_[counter_y][counter_x++] = matrix_[j][k];
            }
          }
          if (j == row) counter_y--;
          counter_x = 0;
          counter_y++;
        }
        // below division and multiply by zero protection
        if (flag == 'y' && matrix_[row][col] == 0) {
          res += pow(-1, row + col) * tmp.Determinant();
          return res;
        } else {  // go inside, if flag != y && matrix_[][] != 0
          res += matrix_[row][col] * pow(-1, row + col) * tmp.Determinant();
        }
        if (flag == 'y')  // go inside, if flag == y, but matrix_[][] != 0
          return res /
                 matrix_[row][col];  // division by zero protection work here
        counter_y = 0;
        col++;
      }
      return res;
    }
  }
}

S21Matrix S21Matrix::InverseMatrix() {
  if (matrix_ == nullptr) {
    throw std::bad_alloc();
  } else if (rows_ != cols_) {
    throw std::logic_error("must be square");
  } else {
    double determ = this->Determinant();
    if (determ == 0) throw std::logic_error("impossible calculate");
    S21Matrix tmp(rows_, cols_);
    tmp = this->Transpose().CalcComplements();
    if (this->rows_ == 1) return tmp;

    for (int i = 0; i < rows_; i++) {
      for (int j = 0; j < cols_; j++) {
        tmp.matrix_[i][j] /= determ;
      }
    }
    return tmp;
  }
}

double S21Matrix::SecOrdInvHelper(double **matr, int row, int col) noexcept {
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 2; j++)
      if (i != row && j != col) return matr[i][j];
  }
  return 0;
}

// OVERLOAD OPERATORS

// copy
S21Matrix &S21Matrix::operator=(const S21Matrix &other) {
  if (this->cols_ != other.cols_ || this->rows_ != other.rows_) {
    this->~S21Matrix();  // throw except - operator is except
    this->rows_ = other.rows_;
    this->cols_ = other.cols_;
    Create(rows_, cols_);
  }
  this->CopyElem(other);
  return *this;
}

// move
S21Matrix &S21Matrix::operator=(S21Matrix &&other) {
  if (this != &other) {
    this->~S21Matrix();  // throw except - operator is except
    rows_ = cols_ = 0;
    matrix_ = other.matrix_;
    rows_ = other.rows_;
    cols_ = other.cols_;
    other.matrix_ = nullptr;
    other.rows_ = other.cols_ = 0;
  }
  return *this;
}

S21Matrix S21Matrix::operator-(const S21Matrix &other) {
  S21Matrix result(*this);
  result.SubMatrix(other);  // throw except - operator is except
  return result;
}

S21Matrix S21Matrix::operator*(const S21Matrix &other) {
  S21Matrix result(*this);
  result.MulMatrix(other);  // throw except - operator is except
  return result;
}

S21Matrix S21Matrix::operator*(const double &num) {
  S21Matrix result(*this);
  result.MulNumber(num);  // throw except - operator is except
  return result;
}

bool S21Matrix::operator==(const S21Matrix &other) {
  return this->EqMatrix(other);  // throw except - operator is except
}

S21Matrix &S21Matrix::operator+=(const S21Matrix &other) {
  this->SumMatrix(other);  // throw except - operator is except
  return *this;
}

S21Matrix &S21Matrix::operator-=(const S21Matrix &other) {
  this->SubMatrix(other);  // throw except - operator is except
  return *this;
}

S21Matrix S21Matrix::operator+(const S21Matrix &other) {
  S21Matrix result(other.rows_,
                   other.cols_);  // throw except - operator is except
  result.SumMatrix(*this);
  result.SumMatrix(other);
  return result;
}

S21Matrix &S21Matrix::operator*=(const S21Matrix &other) {
  this->MulMatrix(other);  // MulMatrix throw except => operator is except
  return *this;
}

S21Matrix &S21Matrix::operator*=(const double &num) {
  this->MulNumber(num);  // MulNumber throw except => operator is except
  return *this;
}

double &S21Matrix::operator()(const int row, const int col) {
  if (rows_ <= row || cols_ <= col || row < 0 || col < 0) {
    throw std::out_of_range("Incorrect Index");
  }
  return matrix_[row][col];
}
