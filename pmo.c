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
int NUM_THREAD = 4;

/**
 * struct Matrix
 * 
 * A Matrix struct to help keep track of the matrix size in addition to the matrix value.
*/
struct Matrix {
  int row_length;       // number of rows
  int col_length;       // number of column
  int** data;           // pointer for a 2D array containing the matrix values
};
typedef struct Matrix Matrix;
struct thread_args {
  int id;
  Matrix* a;
  Matrix* b;
  Matrix* c;
};
typedef struct thread_args thread_args;

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////FUNCTIONS///////////////////////////////////////

/**
 * min
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

/**
 * create_matrix(int n_rows, int n_cols)
 * 
 * Create an empty matrix of size n_rows by n_cols. Allocates memory dynamically.
 * 
 * Parameters:
 * row_length: length of the matrix row
 * col_length: length of the matrix column
 * 
 * Returns:
 * Matrix* of the generated empty matrix.
*/
Matrix* create_matrix(int row_length, int col_length) {
  struct Matrix* matrix = malloc(sizeof(matrix));
  matrix -> row_length = row_length;
  matrix -> col_length = col_length;
  int** data = malloc(sizeof(int*)*row_length);
  for(int i = 0; i < row_length; i++) data[i] = calloc(col_length, sizeof(int));
  matrix -> data = data;
  return matrix;
}

/**
 * init_matrix(int n_rows, int n_cols)
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
Matrix* init_matrix(int row_length, int col_length) {
  struct Matrix* matrix = create_matrix(row_length, col_length);
  for(int i = 0; i < row_length; i++) {
    for(int j = 0; j < col_length; j++) scanf(" %d", &(matrix -> data[i][j]));
  }
  return matrix;  
}

/**
 * print_matrix(Matrix* matrix)
 * 
 * Prints out the matrix by looping over the data.
 * 
 * Parameters:
 * matrix: the matrix to be printed
*/
void print_matrix(Matrix* matrix) {
  for(int i = 0; i < matrix -> row_length; i++) {
    printf("%s", "\n");
    for(int j = 0; j < matrix -> col_length; j++) printf("%d\t", matrix -> data[i][j]);
  }
  return;
}

/**
 * multiply_matrix(Matrix* a, Matrix* b)
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
void* multiply_matrix_blocked(void* arg) {
  // ROW OF A = N (iterate as i), COLUMN OF A = M (iterate as k)
  // ROW OF B = M (iterate as k), COLUMN OF B = P (iterate as j)

  int n = ((thread_args*)arg) -> a -> row_length;
  int m = ((thread_args*)arg) -> a -> col_length; // a -> col_length must be equal to b -> row_length. Otherwise mult is impossible!
  int p = ((thread_args*)arg) -> b -> col_length;

  int bsize = 16;     // block size must be smaller than portion_size

  int portion_size = (p + (NUM_THREAD - 1))/NUM_THREAD; // p/NUM_THREAD but rounds up instead of truncating. this makes sure that all columns are included.

  if (((((thread_args*)arg) -> id) * portion_size) > p) {   // if number of threads is larger than number of columns, exit.
    pthread_exit(NULL);
  }

  for (int jj = ((((thread_args*)arg) -> id) * portion_size); jj < ((((thread_args*)arg) -> id + 1) * portion_size); jj += bsize) {
    for (int kk = 0; kk < m; kk += bsize) {
      for (int i = 0; i < n; i++) {
        for (int j = jj; j < min(min(jj+bsize,((((thread_args*)arg) -> id + 1) * portion_size)),p); j++) {
          int sum = 0;
          for (int k = kk; k < min(kk+bsize,m); k++)
            sum += ((thread_args*)arg) -> a -> data[i][k] * ((thread_args*)arg) -> b -> data[k][j];
          ((thread_args*)arg) -> c -> data[i][j] += sum;
        }
      }
    }
  }
  pthread_exit(NULL);
}
// Matrix* multiply_matrix_blocked_column(Matrix* a, Matrix* b, size_t start_col, size_t end_col) {
//   // ROW OF A = N (iterate as i), COLUMN OF A = M (iterate as k)
//   // ROW OF B = M (iterate as k), COLUMN OF B = P (iterate as j)
//   // ROW OF C: N. COL OF C: P.

//   int n = a -> row_length;
//   int m = a -> col_length; // Expect a -> col_length to be equal to b -> row_length;
//   int p = b -> col_length;

//   Matrix* c = create_matrix(n, p);
//   int bsize = 16;
//   int chunk_size = 16;

//   int portion_size = p/NUM_THREAD; // 

//   // For each chunk of columns
//   for (size_t col_chunk = start_col; col_chunk < end_col; col_chunk += chunk_size)     // for each col chunk of C
//     // For each chunk of rows
//     for (size_t row_chunk = 0; row_chunk < n; row_chunk += chunk_size)                 // row chunk of C
//       // For each block of elements in this row of this column chunk
//       for (size_t tile = 0; tile < n; tile += bsize)                                   // for each block           
//         // For apply that tile to each row of the row chunk
//         for (size_t row = 0; row < bsize; row++)
//           // For each row in the tile
//           for (size_t tile_row = 0; tile_row < bsize; tile_row++)
//             // Solve for each element in this tile row
//             for (size_t k = 0; k < bsize; k++)
//               c -> data[]
//               c[(row + row_chunk) * N + col_chunk + idx] +=
//                   a[(row + row_chunk) * N + tile + tile_row] *
//                   b[tile * N + tile_row * N + col_chunk + idx];

//   for (int jj = 0; jj < p; jj += bsize) {
//     for (int kk = 0; kk < m; kk += bsize) {
//       for (int i = 0; i < n; i++) {
//         for (int j = jj; j < min(jj+bsize,p); j++) {
//           int sum = 0;
//           for (int k = kk; k < min(kk+bsize,m); k++) sum += a -> data[i][k] * b -> data[k][j];
//           c -> data[i][j] += sum;
//         }
//       }
//     }
//   }
//   return c;
// }


/**
 * multiply_matrix_naive(Matrix* a, Matrix* b)
 * 
 * Multiply 2 matrices using the naive approach.
 * 
 * Parameters:
 * a: left operand matrix
 * b: right operand matrix
 * 
 * Returns:
 * Matrix* of the resulted matrix operation.
*/
Matrix* multiply_matrix_naive(Matrix* a, Matrix* b) {
  // ROW OF A = N (iterate as i), COLUMN OF A = M (iterate as k)
  // ROW OF B = M (iterate as k), COLUMN OF B = P (iterate as j)

  int n = a -> row_length;
  int m = a -> col_length; // Expect a -> col_length to be equal to b -> row_length;
  int p = b -> col_length;

  Matrix* c = create_matrix(n, p);
  int bsize = 2;

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < p; j++) {
      int sum = 0;
      for (int k = 0; k < m; k++) sum += a -> data[i][k] * b -> data[k][j];
      c -> data[i][j] += sum;
    }
  }
  return c;
}

////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////MAIN FUNCTION//////////////////////////////////
int main() {
  // while (fgets(buf, sizeof buf, stdin) != NULL) {
  //   if (sscanf(buf, "%31s", buf) != 1) break;
  //   printf(buf);
  // }

  Matrix* a = init_matrix(3,3);
  Matrix* b = init_matrix(3,3);

  Matrix* c = create_matrix(a -> row_length, b -> col_length);

  thread_args* threads;
  threads = (thread_args*)malloc( NUM_THREAD * sizeof(thread_args) );

  thread_args* arg_list;
  pthread_t tid[NUM_THREAD];
  
  for (int i = 0; i < NUM_THREAD; ++i ) {
    thread_args params;
    params.id = i;
    params.a = a;
    params.b = b;
    params.c = c;
    pthread_create( &tid[i], NULL, &multiply_matrix_blocked, (void*)&params );
    pthread_join( tid[i], NULL );
  }
  print_matrix(c);
  free(a);
  free(b);
  free(c);

  pthread_exit(NULL);
}
////////////////////////////////////////////////////////////////////////////////
