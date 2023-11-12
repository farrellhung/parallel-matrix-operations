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

/**
 * struct Matrix
 * Writen by: Farrell Hung (56691108)
 * 
 * A Matrix struct to help keep track of the matrix size in addition to the matrix value.
*/
struct Matrix {
  int row_length;       // number of rows
  int col_length;       // number of column
  int** data;           // pointer for a 2D array containing the matrix values
};
typedef struct Matrix Matrix;

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////FUNCTIONS///////////////////////////////////////

/**
 * create_matrix(int n_rows, int n_cols)
 * Writen by: Farrell Hung (56691108)
 * 
 * Create an empty matrix of size n_rows by n_cols. Allocates memory dynamically.
 * 
 * Parameters:
 * n_rows: length of the matrix row
 * n_cols: length of the matrix column
 * 
 * Returns:
 * Matrix* of the generated empty matrix.
*/
Matrix* create_matrix(int n_rows, int n_cols) {
  struct Matrix* matrix = malloc(sizeof(matrix));
  matrix -> row_length = n_rows;
  matrix -> col_length = n_cols;
  int** data = malloc(sizeof(int*)*n_rows);
  for(int i = 0; i < n_rows; i++) {
    data[i] = calloc(n_cols, sizeof(int));
  }
  matrix -> data = data;
  return matrix;
}

/**
 * init_matrix(int n_rows, int n_cols)
 * Writen by: Farrell Hung (56691108)
 * 
 * Initialize a matrix by first creating an empty matrix of size n_rows by n_cols, done by calling create_matrix.
 * Then, scan for input and put the value inside matrix -> data.
 * Expects the given matrix size to be consistent with the input given.
 * 
 * Parameters:
 * n_rows: length of the matrix row
 * n_cols: length of the matrix column
 * 
 * Returns:
 * Matrix* of the initialized matrix.
*/
Matrix* init_matrix(int n_rows, int n_cols) {
  struct Matrix* matrix = create_matrix(n_rows, n_cols);
  for(int i = 0; i < n_rows; i++) {
    for(int j = 0; j < n_cols; j++) {
      scanf(" %d", &(matrix -> data[i][j]));
    }
  }
  return matrix;  
}

/**
 * print_matrix(Matrix* matrix)
 * Writen by: Farrell Hung (56691108)
 * 
 * Simply prints out the matrix by looping over the data.
 * 
 * Parameters:
 * matrix: the matrix to be printed
*/
void print_matrix(Matrix* matrix) {
  for(int i = 0; i < matrix -> row_length; i++) {
    printf("%s", "\n");
    for(int j = 0; j < matrix -> col_length; j++) {
      printf("%d\t", matrix -> data[i][j]);
    }
  }
}

/**
 * multiply_matrix(Matrix* a, Matrix* b)
 * Writen by: Farrell Hung (56691108)
 * 
 * Multiply 2 matrices. Only uses block and does not use any blackmagicfuckery algorithms for now.
 * 
 * Parameters:
 * a: left operand matrix
 * b: right operand matrix
 * 
 * Returns:
 * Matrix* of the resulted matrix operation.
*/
Matrix* multiply_matrix(Matrix* a, Matrix* b) {
  // ROW OF A = N (iterate as i), COLUMN OF A = M (iterate as k)
  // ROW OF B = M (iterate as k), COLUMN OF B = P (iterate as j)

  int n = a -> row_length;
  int m = a -> col_length; // Expect a -> col_length to be equal to b -> row_length;
  int p = b -> col_length;

  Matrix* c = create_matrix(n, p);
  int sum;
  int bsize = 2;

  for (int jj = 0; jj < p; jj += bsize) {
    // for (int i = 0; i < n; i++)
    //   for (int j = jj; j < min(jj+bsize,p); j++)
    //     c -> data[i][j] = 0;
    for (int kk = 0; kk < m; kk += bsize) {
      for (int i = 0; i < n; i++) {
        for (int j = jj; j < min(jj+bsize,p); j++) {
          sum = 0;
          for (int k = kk; k < min(kk+bsize,m); k++) {
            sum += a -> data[i][k] * b -> data[k][j];
          }
          (c -> data[i][j]) += sum;
        }
      }
    }
  }
  return c;
}

/**
 * min
 * Writen by: Farrell Hung (56691108)
 * 
 * Returns the lowest value among the two given integers.
 * 
 * Parameters:
 * a: integer to be compared.
 * b: second integer to be compared.
 * 
 * Returns:
 * Integer of the minimum value among the two given parameters.
*/
int min(int a, int b) {
    return a<b ? a : b;
}

////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////MAIN FUNCTION//////////////////////////////////
int main() {
  // while (fgets(buf, sizeof buf, stdin) != NULL) {
  //   if (sscanf(buf, "%31s", buf) != 1) break;
  //   printf(buf);
  // }

  Matrix* a = init_matrix(2,3);
  Matrix* b = init_matrix(3,2);
  Matrix* c = multiply_matrix(a, b);
  print_matrix(c);
  // Tried using 2x2 * 2x2, 2x3 * 3x2, and 3x3 * 3x3 works nicely.
  // Need to try using large matrix later. bsize needs to be optimized according to the server's cache size.

  return 0;
}
////////////////////////////////////////////////////////////////////////////////
