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
#define MAX_CALC_STACK 100
#define NUM_THREAD 4
#define BLOCK_SIZE 64

/**
 * @struct  Matrix
 * @brief   Represents a matrix.
 *
 * A matrix struct to help keep track of the matrix size in addition to the matrix value.
 */
struct Matrix {
  int row_length;       // number of rows
  int col_length;       // number of column
  int** data;           // pointer for a 2D array containing the matrix values
};
typedef struct Matrix Matrix;


/**
 * @struct  thread_args
 * @brief   Structure to hold arguments for a thread.
 * 
 * This structure is used to pass arguments to a thread function.
 * It can be used to pass data or parameters that are required by the thread.
 */
struct ThreadArgs {
  int id;               // thread id to determine which part of the matrix to be calculated by the thread
  Matrix* a;            // left operand matrix
  Matrix* b;            // right operand matrix
  Matrix* c;            // result matrix
};
typedef struct ThreadArgs ThreadArgs;

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////FUNCTIONS///////////////////////////////////////

/**
 * Returns the lowest value among the two given integers.
 *
 * @param a First integer to be compared.
 * @param b Second integer to be compared.
 * @return  Integer with the minimum value among the two given parameters.
*/
int min(int a, int b) {
  return a < b ? a : b;
}

/**
 * Calculates the precedence of an operator.
 *
 * @param op  The operator whose precedence is to be calculated.
 * @return    The precedence value of the operator.
 */
int precedence(char op) {
  if (op == '+' || op == '-') return 1;
  if (op == '*') return 2;
  return 0;
}

/**
 * Parses the given expression and converts it into postfix notation.
 *
 * @param exp_input     The input expression to be parsed.
 * @param exp_postfix   The resulting postfix expression.
 * @param matrix_count  The number of matrices in the expression.
 * @return              A pointer to the resulting postfix expression.
 */
char* parseExpression(char* exp_input, char* exp_postfix, unsigned char* matrix_count) {
  int i, k;
  char operation_stack[strlen(exp_input)];
  int top = -1;

  for (i = 0, k = -1; exp_input[i]; ++i) {
    if (exp_input[i] >= 'A' && exp_input[i] <= 'Z') {
      exp_postfix[++k] = exp_input[i];
      (*matrix_count) += 1;
    }
    else {
      while (top != -1 && precedence(exp_input[i]) <= precedence(operation_stack[top]))
        exp_postfix[++k] = operation_stack[top--];
      operation_stack[++top] = exp_input[i];
    }
  }

  while (top != -1)
    exp_postfix[++k] = operation_stack[top--];

  exp_postfix[++k] = '\0';
}

/**
 * Create an empty matrix of size row_length by col_length.
 *
 * @param row_length  length of the matrix row.
 * @param col_length  length of the matrix column.
 * @return            matrix pointer to the generated empty matrix.
*/
Matrix* createMatrix(int row_length, int col_length) {
  struct Matrix* matrix = malloc(sizeof(matrix)); // Allocates memory dynamically
  matrix->row_length = row_length;
  matrix->col_length = col_length;
  int** data = malloc(sizeof(int*) * row_length);
  for (int i = 0; i < row_length; i++)
    data[i] = calloc(col_length, sizeof(int)); // init as 0
  matrix -> data = data;
  return matrix;
}

/**
 * Initializes a matrix by creating an empty matrix, then scans for input data and stores them in a matrix struct.
 *
 * @param row_length  length of the matrix row.
 * @param col_length  length of the matrix column.
 * @return            matrix pointer to the initialized matrix.
*/
Matrix* initMatrix(int row_length, int col_length) {
  struct Matrix* matrix = createMatrix(row_length, col_length);
  for (int i = 0; i < row_length; i++)
    for (int j = 0; j < col_length; j++)
      scanf("%d", &(matrix->data[i][j]));
  return matrix;
}

/**
 * Prints out the given matrix.
 *
 * @param matrix The matrix to be printed out.
*/
void printMatrix(Matrix* matrix) {
  for (int i = 0; i < matrix->row_length; i++) {
    for (int j = 0; j < matrix->col_length; j++)
      printf("%d\t", matrix->data[i][j]);
    printf("\n");
  }
  return;
}

/**
 * Adds two matrices together using threads.
 *
 * @param arg Pointer to the argument struct ThreadArgs.
 */
void* addMatrix(void* arg) {
  // Unpack the struct
  unsigned int n = ((ThreadArgs*)arg)->a->row_length;
  unsigned int m = ((ThreadArgs*)arg)->a->col_length;

  // Calculates p/NUM_THREAD but rounds up instead of truncating. this makes sure that all columns are included.
  unsigned int portion_size = (m + (NUM_THREAD - 1)) / NUM_THREAD;
  unsigned int portion_end = ((((ThreadArgs*)arg)->id + 1) * portion_size);

  if (((((ThreadArgs*)arg)->id) * portion_size) > m)   // if number of threads is larger than number of columns, exit.
    pthread_exit(NULL);

  for (int i = 0; i < n; i++) {
    for (int k = ((((ThreadArgs*)arg)->id) * portion_size); k < min(m, portion_end); k++) {
      ((ThreadArgs*)arg)->c->data[i][k] += ((ThreadArgs*)arg)->a->data[i][k] + ((ThreadArgs*)arg)->b->data[i][k];
    }
  }
}

/**
 * Substract two matrices using threads.
 *
 * @param arg Pointer to the argument struct ThreadArgs.
 */
void* subtractMatrix(void* arg) {
  unsigned int n = ((ThreadArgs*)arg)->a->row_length;
  unsigned int m = ((ThreadArgs*)arg)->a->col_length;

  unsigned int portion_size = (m + (NUM_THREAD - 1)) / NUM_THREAD; // p/NUM_THREAD but rounds up instead of truncating. this makes sure that all columns are included.
  unsigned int portion_end = ((((ThreadArgs*)arg)->id + 1) * portion_size);

  if (((((ThreadArgs*)arg)->id) * portion_size) > m)   // if number of threads is larger than number of columns, exit.
    pthread_exit(NULL);

  for (int i = 0; i < n; i++) {
    for (int k = ((((ThreadArgs*)arg)->id) * portion_size); k < min(m, portion_end); k++) {
      ((ThreadArgs*)arg)->c->data[i][k] += ((ThreadArgs*)arg)->a->data[i][k] - ((ThreadArgs*)arg)->b->data[i][k];
    }
  }
}


/**
 * Multiplies 2 matrices together using thread. Calculations are blocked to the size of BLOCK_SIZE to improve speed.
 *
 * @param arg Pointer to the argument struct ThreadArgs.
*/
void* multiplyMatrix(void* arg) {
  // ROW OF A = N (iterate as i), COLUMN OF A = M (iterate as k)
  // ROW OF B = M (iterate as k), COLUMN OF B = P (iterate as j)

  // n is the row length of matrix A, which is also equal to the row length of the resulting matrix C.
  // m is the column length of matrix A, which is also equal to the row length of matrix B.
  // p is the column length of matrix B, which is also equal to the column length of the resulting matrix C.
  unsigned int n = ((ThreadArgs*)arg)->a->row_length;
  unsigned int m = ((ThreadArgs*)arg)->a->col_length;
  unsigned int p = ((ThreadArgs*)arg)->b->col_length;

  // calculates which portion of the matrix that the current thread is responsible to calculate for.
  // portion_size is basically p divided by the number of threads.
  // However, to ensure that all columns are included, portion_size is rounded up instead of truncated.
  unsigned int portion_size = (p + (NUM_THREAD - 1)) / NUM_THREAD; // divides p by NUM_THREAD and round up the result
  unsigned int portion_end = ((((ThreadArgs*)arg)->id + 1) * portion_size); // end of the this thread's portion

  // Exit the thread if the current thread number is larger than p.
  // For example, when multiplying two 3x3 matrices with 4 threads.
  // We cannot divide the job based on p into more than 3 jobs since p = 3.
  // Thus, the fourth thread will not do anything.
  if (((((ThreadArgs*)arg)->id) * portion_size) > p)
    pthread_exit(NULL);

  // for jj as each column chunk in matrix C...
  for (int jj = ((((ThreadArgs*)arg)->id) * portion_size); jj < portion_end; jj += BLOCK_SIZE) {
    // for kk as each k chunk...
    for (int kk = 0; kk < m; kk += BLOCK_SIZE) {
      // for every row of matrix C
      for (int i = 0; i < n; i++) {
        // for every column in matrix C inside the current iteration's block...
        for (int j = jj; j < min(min(jj + BLOCK_SIZE, portion_end), p); j++) {
          int sum = 0;
          // for every k in the current iteration's block...
          for (int k = kk; k < min(kk + BLOCK_SIZE, m); k++)
            // sum the dot product
            sum += ((ThreadArgs*)arg)->a->data[i][k] * ((ThreadArgs*)arg)->b->data[k][j];
          // add the sum to row i column j of matrix C
          ((ThreadArgs*)arg)->c->data[i][j] += sum;
        }
      }
    }
  }
  pthread_exit(NULL);
}


/**
 * Adds or subtracts two matrices without using threads.
 *
 * @param a The first matrix (left operand).
 * @param b The second matrix (right operand).
 * @param f The flag indicating whether to add or subtract the matrices.
 *          Use 1 for addition and -1 for subtraction.
 * @return The resulting matrix after the addition or subtraction operation.
 */
Matrix* naiveAddSubMatrix(Matrix* a, Matrix* b, signed char f) {
  int row = a->row_length;
  int col = a->col_length;

  Matrix* c = createMatrix(row, col);
  for (int i = 0; i < row; i++) {
    for (int j = 0; j < col; j++) {
      c->data[i][j] = a->data[i][j] + f * (b->data[i][j]);
    }
  }
  return c;
}

/**
 * Multiplies 2 matrices using the naive approach, without using threads.
 *
 * @param a Left operand matrix.
 * @param b Right operand matrix.
 * @return  The resulting matrix after the multiplication.
*/
Matrix* naiveMultiplyMatrix(Matrix* a, Matrix* b) {
  // ROW OF A = N (iterate as i), COLUMN OF A = M (iterate as k)
  // ROW OF B = M (iterate as k), COLUMN OF B = P (iterate as j)

  int n = a->row_length;
  int m = a->col_length; // Expect a -> col_length to be equal to b -> row_length;
  int p = b->col_length;

  Matrix* c = createMatrix(n, p);

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < p; j++) {
      int sum = 0;
      for (int k = 0; k < m; k++) sum += a->data[i][k] * b->data[k][j];
      c->data[i][j] += sum;
    }
  }
  return c;
}

////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////MAIN FUNCTION//////////////////////////////////
int main() {
  Matrix* calc_stack[MAX_CALC_STACK];
  int top = -1;

  char exp_input[100];
  scanf("%s", &exp_input);

  int exp_len = strlen(exp_input);
  char exp_postfix[exp_len + 1];

  unsigned char matrix_count = 0;
  parseExpression(exp_input, exp_postfix, &matrix_count);

  Matrix* input_matrices[matrix_count];
  for (int i = 0; i < matrix_count; i++) {
    int r, c;
    scanf("%d\t%d", &r, &c);
    Matrix* matrix = initMatrix(r, c);
    input_matrices[i] = matrix;
  }

  for (int i = 0; i < exp_len; i++) {
    if (exp_postfix[i] >= 'A' && exp_postfix[i] <= 'Z') {
      int idx = exp_postfix[i] - 'A';
      // push operand matrix to calc_stack
      calc_stack[++top] = input_matrices[idx];
    }
    else {
      // pop operand matrix from calc_stack
      Matrix* b = calc_stack[top--]; // b comes first to swap
      Matrix* a = calc_stack[top--];
      Matrix* c = createMatrix(a->row_length, b->col_length);
      ThreadArgs args_pool[NUM_THREAD];
      pthread_t tid[NUM_THREAD];

      switch (exp_postfix[i]) {
      case '+':
        for (int i = 0; i < NUM_THREAD; ++i) {
          ThreadArgs params;
          params.id = i;
          params.a = a;
          params.b = b;
          params.c = c;
          args_pool[i] = params;
          pthread_create(&tid[i], NULL, &addMatrix, (void*)&args_pool[i]);
        }
        break;
      case '-':
        for (int i = 0; i < NUM_THREAD; ++i) {
          ThreadArgs params;
          params.id = i;
          params.a = a;
          params.b = b;
          params.c = c;
          args_pool[i] = params;
          pthread_create(&tid[i], NULL, &subtractMatrix, (void*)&args_pool[i]);
        }
        break;
      case '*':
        for (int i = 0; i < NUM_THREAD; ++i) {
          ThreadArgs params;
          params.id = i;
          params.a = a;
          params.b = b;
          params.c = c;
          args_pool[i] = params;
          pthread_create(&tid[i], NULL, &multiplyMatrix, (void*)&args_pool[i]);
        }
        break;
      }
      for (int i = 0; i < NUM_THREAD; ++i) {
        pthread_join(tid[i], NULL);
      }
      calc_stack[++top] = c;
      free(a);
      free(b);
    }
  }
  printf("%d\t%d\n", calc_stack[top]->row_length, calc_stack[top]->col_length);
  printMatrix(calc_stack[top]);
}
////////////////////////////////////////////////////////////////////////////////
