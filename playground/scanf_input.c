#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    int row, col;
    // 80\t90\n
    printf("input: ");
    scanf("%d\t%d", &row, &col);
    printf("output: %d, %d\n", row, col);
    printf("----------\n");
    int** arr = malloc(row * sizeof(int*));
    for (int i = 0; i < row; i++)
        arr[i] = malloc(col * sizeof(int));
    for (int i = 0; i < row; i++) {
        int a;
        for (int j = 0; j < col; j++) {
            scanf("%d", &a);
            arr[i][j] = a;
        }
    }
    printf("----------\n");
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            printf("%d ", arr[i][j]);
        }
    }
}