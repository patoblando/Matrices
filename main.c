#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

typedef struct _strct_matrix{
    int rows;
    int cols;
    double **data;
} _matrix;
typedef _matrix *matrix; 

matrix create_matrix(int rows, int cols){
    
    matrix m = (matrix)malloc(sizeof(_matrix));
    m->rows = rows;
    m->cols = cols;

    m->data = (double**)malloc(rows * sizeof(double*));

    for(int i=0; i<rows; i++) m->data[i] = (double*)malloc(cols * sizeof(double));
    for (int i=0; i<rows; i++){
        for(int j=0; j<cols; j++){
            m->data[i][j] = 0.0;
        }
    }

    return m;
}

matrix id_matrix(int rows, int cols){
    matrix id = create_matrix(rows, cols);
    for(int i = 0; i < rows; i++) for (int j = 0; j < cols; j++) if (i == j) id->data[i][j] = 1.0;
    return id;
}

void print_matrix(matrix m){
    for(int i=0; i<m->rows; i++){
        for(int j=0; j<m->cols; j++){
            double intpart;
            if(modf(m->data[i][j], &intpart) == 0.0)
                printf("%5d ", (int)m->data[i][j]);
            else
                printf("%5.2f ", m->data[i][j]);
        }
        printf("\n");
    }
}

void delete_matrix(matrix m){
    for(int i = 0; i < m->rows; i++) free(m->data[i]);
    free(m->data);
    free(m);
}

matrix oef1(matrix m, int row1, int row2){
    if (row1 > m->rows || row2 > m->rows){
        printf("oef1 ERROR: Bad index");
        return m;
    }
    double* aux;
    aux = m->data[row1];
    m->data[row1] = m->data[row2];
    m->data[row2] = aux;
    printf("f%d <-> f%d\n", row1+1, row2+1);
    return m;
}

matrix oef2(matrix m, int row, double lambda){
    if (row > m->rows){
        printf("oef2 ERROR: Bad index");
        return m;
    }
    for (int i = 0; i < m->cols; i++) m->data[row][i] *= lambda;
    printf("f%d = %f * f%d\n", row+1, lambda, row+1);
    return m;
}

matrix oef3(matrix m, int row1, int row2, double lambda){
    if (row1 > m->rows || row2 > m->rows){
        printf("oef3 ERROR: Bad index");
        return m;
    }
    for (int i = 0; i < m->cols; i++) m->data[row1][i] += lambda * m->data[row2][i];
    printf("f%d = f%d + %f * f%d\n", row1, row1+1, lambda, row2+1);
    return m;
}

matrix escalon_reducida_filas(matrix m){
    for (int i = 0; i < m->rows; i++){
        for (int j = 0; j < m->cols; j++){
            if (m->data[i][j] != 0){
                oef2(m, i+1, 1/m->data[i][j]);
                for (int k = 0; k < m->rows; k++){
                    if (k != i) oef3(m, k+1, i+1, -m->data[k][j]);
                }
                break;
            }
        }
    }
    return m;
}

matrix matrix_erf(matrix m){
    int fila_actual = 0;
    for (int columna = 0; columna < m->cols; columna++){

        int fila_no_cero = fila_actual;
        while (fila_no_cero < m->rows && m->data[fila_no_cero][columna] == 0) fila_no_cero++;
        if (fila_no_cero == m->rows) continue;

        if (fila_no_cero != fila_actual) oef1(m, fila_no_cero, fila_actual);
        oef2(m, fila_no_cero, 1/m->data[fila_no_cero][columna]);

        for(int i = 0; i < m->rows; i++){
            if (m->data[i][columna] != 0 && i != fila_actual) oef3(m, i, fila_actual, -m->data[i][columna]);
        }

        fila_actual++;
    }
    return m;
}

int main(){
    srand(time(NULL));  // set the seed for rand()

    matrix random = create_matrix(3, 4);
    for(int i = 0; i < random->rows; i++) for (int j = 0; j < random->cols; j++) random->data[i][j] = rand() % 10;
    printf("----------------------\n");
    print_matrix(random);
    printf("----------------------\n");
    matrix_erf(random);
    printf("----------------------\n");
    print_matrix(random);
    delete_matrix(random);
    return 0;
}

