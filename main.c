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
    printf("---------------------->\n");
    printf("f%d <-> f%d\n", row1+1, row2+1);
    print_matrix(m);
    return m;
}

matrix oef2(matrix m, int row, double lambda){
    if (row > m->rows){
        printf("oef2 ERROR: Bad index");
        return m;
    }
    for (int i = 0; i < m->cols; i++) m->data[row][i] *= lambda;
    printf("---------------------->\n");
    printf("f%d = %f * f%d\n", row+1, lambda, row+1);
    print_matrix(m);
    return m;
}

matrix oef3(matrix m, int row1, int row2, double lambda){
    if (row1 > m->rows || row2 > m->rows){
        printf("oef3 ERROR: Bad index");
        return m;
    }
    for (int i = 0; i < m->cols; i++) m->data[row1][i] += lambda * m->data[row2][i];
    printf("---------------------->\n");
    printf("f%d = f%d + %f * f%d\n", row1+1, row1 +1, lambda, row2+1);
    print_matrix(m);
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
        if (m->data[fila_actual][columna] != 1) oef2(m, fila_no_cero, 1/m->data[fila_actual][columna]);

        for(int i = 0; i < m->rows; i++){
            if (m->data[i][columna] != 0 && i != fila_actual) oef3(m, i, fila_actual, -m->data[i][columna]);
        }

        fila_actual++;
    }
    return m;
}

matrix matrix_inv(matrix m){
    if (m->rows != m->cols){
        printf("matrix_inv ERROR: Matrix is not square");
        return m;
    }
    matrix inv = id_matrix(m->rows, m->cols);
    for (int i = 0; i < m->rows; i++){
        for (int j = 0; j < m->cols; j++){
            inv->data[i][j] = m->data[i][j];
        }
    }
    matrix_erf(inv);
    return inv;
}

matrix matrix_mult(matrix m1, matrix m2){
    if (m1->cols != m2->rows){
        printf("matrix_mult ERROR: Bad dimensions");
        return m1;
    }
    matrix mult = create_matrix(m1->rows, m2->cols);
    for (int i = 0; i < m1->rows; i++){
        for (int j = 0; j < m2->cols; j++){
            for (int k = 0; k < m1->cols; k++){
                mult->data[i][j] += m1->data[i][k] * m2->data[k][j];
            }
        }
    }
    return mult;
}

matrix matrix_input(){
    int rows, cols;
    printf("Enter the number of rows: ");
    scanf("%d", &rows);
    printf("Enter the number of columns: ");
    scanf("%d", &cols);
    matrix m = create_matrix(rows, cols);
    printf("Enter the matrix:\n");
    for (int i = 0; i < rows; i++) for (int j = 0; j < cols; j++) scanf("%lf", &m->data[i][j]);
    return m;
}


int main(){
    srand(time(NULL));  // set the seed for rand()
    printf("----------------------\n");
    matrix matriz = matrix_input();
    //for(int i = 0; i < matriz->rows; i++) for (int j = 0; j < matriz->cols; j++) matriz->data[i][j] = rand() % 10;
    print_matrix(matriz);
    
    matrix_erf(matriz);

    delete_matrix(matriz);
    
    return 0;
}

