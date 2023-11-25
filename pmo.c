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
  int stack_pointer = -1;

  for (i = 0, k = -1; exp_input[i]; ++i) {
    if (exp_input[i] >= 'A' && exp_input[i] <= 'Z') {
      exp_postfix[++k] = exp_input[i];
      (*matrix_count) += 1;
    }
    else {
      while (stack_pointer != -1 && precedence(exp_input[i]) <= precedence(operation_stack[stack_pointer]))
        exp_postfix[++k] = operation_stack[stack_pointer--];
      operation_stack[++stack_pointer] = exp_input[i];
    }
  }

  while (stack_pointer != -1)
    exp_postfix[++k] = operation_stack[stack_pointer--];

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
  struct Matrix* matrix = malloc(sizeof(matrix)); // allocates dynamic memory for the matrix struct
  matrix->row_length = row_length;
  matrix->col_length = col_length;
  int** data = malloc(sizeof(int*) * row_length); // allocates dynamic memory for the size of matrix data
  // allocates memory for every cell of the matrix, and initialize it as 0
  for (int i = 0; i < row_length; i++)
    data[i] = calloc(col_length, sizeof(int)); 
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
  // create an empty matrix according to the requested size
  struct Matrix* matrix = createMatrix(row_length, col_length);
  // scans for the input matrix values and update the matrix data accordingly
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
  // print the matrix data by iterating through every rows and columns of the matrix
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
  // Unpack the struct. n is the row of the matrix while m is the column of the matrix.
  unsigned int n = ((ThreadArgs*)arg)->a->row_length;
  unsigned int m = ((ThreadArgs*)arg)->a->col_length;
  unsigned char id = (((ThreadArgs*)arg)->id);

  // calculates which portion of the matrix that the current thread is responsible to calculate for.
  // in the event of non-integer results, portion_size is rounded up instead of truncated.
  unsigned int portion_size = (m + (NUM_THREAD - 1)) / NUM_THREAD;
  unsigned int portion_end = ((id + 1) * portion_size);

  // if the thread ID is larger than the number of columns, there is no more available job. exit right away.
  if ((id * portion_size) > m)
    pthread_exit(NULL);

  // for every cells in this thread's work portion, adds a with b and put the results in c.
  for (int i = 0; i < n; i++)
    for (int j = (id * portion_size); j < min(m, portion_end); j++)
      ((ThreadArgs*)arg)->c->data[i][j] += ((ThreadArgs*)arg)->a->data[i][j] + ((ThreadArgs*)arg)->b->data[i][j];
}

/**
 * Substract two matrices using threads.
 *
 * @param arg Pointer to the argument struct ThreadArgs.
 */
void* subtractMatrix(void* arg) {
  // Unpack the struct. n is the row of the matrix while m is the column of the matrix.
  unsigned int n = ((ThreadArgs*)arg)->a->row_length;
  unsigned int m = ((ThreadArgs*)arg)->a->col_length;
  unsigned char id = (((ThreadArgs*)arg)->id);

  // calculates which portion of the matrix that the current thread is responsible to calculate for.
  // in the event of non-integer results, portion_size is rounded up instead of truncated.
  unsigned int portion_size = (m + (NUM_THREAD - 1)) / NUM_THREAD;
  unsigned int portion_end = ((id + 1) * portion_size);

  // if the thread ID is larger than the number of columns, there is no more available job. exit right away.
  if ((id * portion_size) > m)
    pthread_exit(NULL);

  // for every cells in this thread's work portion, subtracts a with b and put the results in c.
  for (int i = 0; i < n; i++)
    for (int j = (id * portion_size); j < min(m, portion_end); j++)
      ((ThreadArgs*)arg)->c->data[i][j] += ((ThreadArgs*)arg)->a->data[i][j] - ((ThreadArgs*)arg)->b->data[i][j];
}


/**
 * Multiplies 2 matrices together using thread. Calculations are blocked to the size of BLOCK_SIZE to improve speed.
 *
 * @param arg Pointer to the argument struct ThreadArgs.
*/
void* multiplyMatrix(void* arg) {
  // n is the row length of matrix A, which is also equal to the row length of the resulting matrix C.
  // m is the column length of matrix A, which is also equal to the row length of matrix B.
  // p is the column length of matrix B, which is also equal to the column length of the resulting matrix C.
  unsigned int n = ((ThreadArgs*)arg)->a->row_length;
  unsigned int m = ((ThreadArgs*)arg)->a->col_length;
  unsigned int p = ((ThreadArgs*)arg)->b->col_length;
  unsigned char id = (((ThreadArgs*)arg)->id);

  // calculates which portion of the matrix that the current thread is responsible to calculate for.
  // in the event of non-integer results, portion_size is rounded up instead of truncated.
  unsigned int portion_size = (p + (NUM_THREAD - 1)) / NUM_THREAD;
  unsigned int portion_end = ((id + 1) * portion_size);

  // if the thread ID is larger than the number of columns, there is no more available job. exit right away.
  if ((id * portion_size) > p)
    pthread_exit(NULL);

  // in the following iterations, n is iterated as i, m is is iterated as k, and p is iterated as j.
  // explanation for the for-loops are writen in the report to avoid cluttering.
  for (int jj = (id * portion_size); jj < portion_end; jj += BLOCK_SIZE) {
    for (int kk = 0; kk < m; kk += BLOCK_SIZE) {
      for (int i = 0; i < n; i++) {
        for (int k = kk; k < min(kk + BLOCK_SIZE, m); k++)
          for (int j = jj; j < min(min(jj + BLOCK_SIZE, portion_end), p); j++) {
            int sum = 0;
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
 * Adds or subtracts two matrices without using threads. For comparison purposes only (not used).
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
 * Multiplies 2 matrices using the naive approach, without using threads. For comparison purposes only (not used).
 *
 * @param a Left operand matrix.
 * @param b Right operand matrix.
 * @return  The resulting matrix after the multiplication.
*/
Matrix* naiveMultiplyMatrix(Matrix* a, Matrix* b) {
  int n = a->row_length;
  int m = a->col_length;
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
  // read and parse expression
  Matrix* calc_stack[MAX_CALC_STACK];
  int stack_pointer = -1;

  char exp_input[100];
  scanf("%s", &exp_input);

  int exp_len = strlen(exp_input);
  char exp_postfix[exp_len + 1];

  unsigned char matrix_count = 0;
  parseExpression(exp_input, exp_postfix, &matrix_count);

  // read the input matrices
  Matrix* input_matrices[matrix_count];
  for (int i = 0; i < matrix_count; i++) {
    int r, c;
    scanf("%d\t%d", &r, &c);
    Matrix* matrix = initMatrix(r, c);
    input_matrices[i] = matrix;
  }

  // for every char of the expression...
  for (int i = 0; i < exp_len; i++) {
    // if it a matrix operand (denoted by a letter), push it to calc_stack
    if (exp_postfix[i] >= 'A' && exp_postfix[i] <= 'Z') {
      int idx = exp_postfix[i] - 'A';
      calc_stack[++stack_pointer] = input_matrices[idx];
    }
    // otherwise, it is an operator. hence, calculate the operandstack.
    else {
      // pop operand matrix from calc_stack
      Matrix* b = calc_stack[stack_pointer--];    // b comes first to swap
      Matrix* a = calc_stack[stack_pointer--];

      // create a new matrix to store the results
      Matrix* c = createMatrix(a->row_length, b->col_length);

      // args_pool as an array to store different thread arguments
      ThreadArgs args_pool[NUM_THREAD];
      // tid as an array to store different thread IDs
      pthread_t tid[NUM_THREAD];

      // performs operation based on the operator
      switch (exp_postfix[i]) {
      case '+':
        // for the amount of threads we generate...
        c = naiveAddSubMatrix(a, b, 1);
        // for (int i = 0; i < NUM_THREAD; ++i) {
        //   // assign calculation parameters to a ThreadArgs struct and append the struct to args_pool
        //   ThreadArgs params;
        //   params.id = i;
        //   params.a = a;
        //   params.b = b;
        //   params.c = c;
        //   args_pool[i] = params;
        //   // create the thread, passing the argument struct in addition to the thread ID
        //   pthread_create(&tid[i], NULL, &addMatrix, (void*)&args_pool[i]);
        // }
        break;
      case '-':
        // for the amount of threads we generate...
        c = naiveAddSubMatrix(a, b, -1);
        // for (int i = 0; i < NUM_THREAD; ++i) {
        //   // assign calculation parameters to a ThreadArgs struct and append the struct to args_pool
        //   ThreadArgs params;
        //   params.id = i;
        //   params.a = a;
        //   params.b = b;
        //   params.c = c;
        //   args_pool[i] = params;
        //   // create the thread, passing the argument struct in addition to the thread ID
        //   pthread_create(&tid[i], NULL, &subtractMatrix, (void*)&args_pool[i]);
        // }
        break;
      case '*':
        // for the amount of threads we generate...
        for (int i = 0; i < NUM_THREAD; ++i) {
          // assign calculation parameters to a ThreadArgs struct and append the struct to args_pool
          ThreadArgs params;
          params.id = i;
          params.a = a;
          params.b = b;
          params.c = c;
          args_pool[i] = params;
          // create the thread, passing the argument struct in addition to the thread ID
          pthread_create(&tid[i], NULL, &multiplyMatrix, (void*)&args_pool[i]);
        }
        break;
      }

      // wait until all threads finish running
      for (int i = 0; i < NUM_THREAD; ++i) {
        pthread_join(tid[i], NULL);
      }

      // put the result on stack
      calc_stack[++stack_pointer] = c;

      // free the memory of the operand matrices
      free(a);
      free(b);
    }
  }

  // output the results
  printf("%d\t%d\n", calc_stack[stack_pointer]->row_length, calc_stack[stack_pointer]->col_length);
  printMatrix(calc_stack[stack_pointer]);
}
////////////////////////////////////////////////////////////////////////////////
