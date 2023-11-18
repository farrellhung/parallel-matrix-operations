////////////////////////////////Libraries///////////////////////////////////////
#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/sysinfo.h>
#include <unistd.h>
////////////////////////////////////////////////////////////////////////////////

////////////////////////////GLOBAL VARIABLES/STRUCTS////////////////////////////
struct Matrix {
  int row_length;       // number of rows
  int col_length;       // number of column
  int** data;           // pointer for a 2D array containing the matrix values
};
typedef struct Matrix Matrix;
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////FUNCTIONS///////////////////////////////////////
// number of matrix and operations to calculate
Matrix* create_matrix(int row_length, int col_length) {
  struct Matrix* matrix = malloc(sizeof(matrix));
  matrix->row_length = row_length;
  matrix->col_length = col_length;
  int** data = malloc(sizeof(int*) * row_length);
  for (int i = 0; i < row_length; i++) data[i] = calloc(col_length, sizeof(int));
  matrix->data = data;
  return matrix;
}

Matrix* calculate_matrix(int number, char* operations, int* matrix_array) {
  Matrix C; // result matrix
  return &C;
}

int** matrix_multiplication(int** A, int** B);
int** matrix_addition(int** A, int** B);
int** matrix_subtraction(int** A, int** B);

void output_matrix(int** C) {
  // output the result of matrix
}
////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////MAIN FUNCTION//////////////////////////////////
int main() {

  // read input for matrix calculation
  char* expression;
  char* operation_stack; char* matrix_stack;
  // while index of operations character points null
  for (int i = 0; i < expression.length; i++) {
    // if the index points operation push to operation
    // otherwise push to matrix stack
  }
  // x -> x: ?push operation, pop two from mat_stack and push in reverse order
  // x -> +: ?
  // + -> x: ?
  // + -> +: ?
  char* calculation_queue;
  // read input for number and operations
  int* matrix_operand = NULL;

  // read input for matrixes
  // scanf the input and store the array in the matrix array
  int*** matrix_array;  // max number of matrixes: 10
  int n, m;             // max number of rows/columns: 1,000
  for (int i = 0; i < matrix_stack; i++) {
    // (n, m) = scanf_result;
    // create matrix_temp with size n*m
    for (int j = 0; j < n; j++) {
      for (int k = 0; k < m; k++) {
        // scanf the input and store the array in the matrix array
      }
    }
    // store the pointer of matrix_temp in matrix_array
  }

  // int** result_matrix;
  int** C = calculate_matrix(number, operations);

  output_matrix(C);

  return 0;
}
////////////////////////////////////////////////////////////////////////////////
