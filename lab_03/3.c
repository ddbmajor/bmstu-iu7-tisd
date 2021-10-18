#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#define DIM_INPUT_ERROR 1
#define MEMORY_ERROR 2
#define POS_INPUT_ERROR 3
#define VAL_INPUT_ERROR 4
#define OPEN_FILE_ERROR 5
#define ZERO_MATRIX_ERROR 6
#define DIMENSION_ERROR 7
#define CHOICE_ERROR 8

typedef struct
{
    int rows;
    int columns;
    int *elements;
} simple_matrix_t;

typedef struct JA_t JA_t;

struct JA_t
{
    int JA;
    JA_t *next;
};

typedef struct
{
    int *A;
    int *IA;
    JA_t *JA_head;
    int rows;
    int columns;
} matrix_t;


JA_t *JA_create(const int value)
{
    JA_t *JA_element = malloc(sizeof(JA_t));

    if (JA_element)
    {
        JA_element->JA = value;
        JA_element->next = NULL;
    }

    return JA_element;
}


void JA_free(JA_t *JA_element)
{
    free(JA_element);
}


void JA_free_all(JA_t *head)
{
    JA_t *next;

    for ( ; head; head = next)
    {
        next = head->next;
        JA_free(head);
    }
}


JA_t *JA_add_end(JA_t *head, JA_t *JA_element)
{
    JA_t *curr = head;

    if(!head)
        return JA_element;
    
    for ( ; curr->next; curr = curr->next)
        ;
    
    curr->next = JA_element;

    return head;
}


void JA_apply(JA_t *head, void (*f)(JA_t*, void*), void *arg)
{
    for ( ; head; head = head->next)
        f(head, arg);
}


void JA_print(JA_t *JA_element, void *arg)
{
    char *fmt = arg;
    printf(fmt, JA_element->JA);
}


JA_t *JA_get_tail(JA_t *head)
{
    JA_t *curr = head;
    for ( ; curr->next; curr = curr->next)
        ;
    return curr;
}


void JA_count(JA_t *JA_element, void *arg)
{
    int *counter = arg;
    (*counter)++;
}


simple_matrix_t *init_matrix()
{
    simple_matrix_t *m = (simple_matrix_t *)malloc(sizeof(simple_matrix_t));
    if (m == NULL)
        return NULL;

    m->rows = 0;
    m->columns = 0;
    m->elements = NULL;
    return m;
}

matrix_t *init_spare_matrix()
{
    matrix_t *m = (matrix_t *)malloc(sizeof(matrix_t));
    if (m == NULL)
        return NULL;
    m->A = NULL;
    m->IA = NULL;
    m->JA_head = NULL;
    m->rows = 0;
    m->columns = 0;
    return m;
}

void clear_matrix(simple_matrix_t *matrix)
{
    free(matrix->elements);
    matrix->elements = NULL;
    matrix->rows = 0;
    matrix->columns = 0;
}

int read_matrix(simple_matrix_t *matrix, FILE *f)
{
    int m, n;
    int value;
    int rc;
    printf("Input dimensions(i,j)\n");
    rc = fscanf(f, "%d %d", &m, &n);
    if (rc != 2 || m <= 0 || n <= 0)
        return DIM_INPUT_ERROR;
    matrix->rows = m;
    matrix->columns = n;
    matrix->elements = (int *)calloc(sizeof(int), n*m);
    if (matrix->elements == NULL)
        return MEMORY_ERROR;
    int i = 0, j = 0;
    while (i >= 0 && j >= 0)
    {
        printf("Input pos(i,j)\n");
        rc = fscanf(f, "%d %d", &i, &j);
        if (rc != 2 || i >= m || j >= n)
        {
            clear_matrix(matrix);
            return POS_INPUT_ERROR;
        }
        if (rc == 2 && i >= 0 && j >= 0)
        {
            printf("Input value\n");
            rc = fscanf(f, "%d", &value);
            if (rc != 1)
            {
                clear_matrix(matrix);
                return VAL_INPUT_ERROR;
            }
            matrix->elements[i*n + j] = value;
        }
    }
    return 0;
}


void print_matrix(simple_matrix_t *matrix)
{
    for (int i = 0; i < matrix->rows; i++)
    {
        for (int j = 0; j < matrix->columns; j++)
            printf("%d ", matrix->elements[i*(matrix->columns) + j]);
        printf("\n");
    }
    printf("\n");
}


void clear_sep_matrix(matrix_t *m)
{
    free(m->A);
    free(m->IA);
    JA_free_all(m->JA_head);
    m->rows = 0;
    m->columns = 0;
}


int transform(matrix_t *dst, simple_matrix_t *src)
{
    dst->rows = src->rows;
    int columns = dst->columns = src->columns;
    int elements = 0;

    for (int i = 0; i < src->rows; i++)
        for (int j = 0; j < src->columns; j++)
            if (src->elements[i * columns + j] != 0)
                elements++;

    if (elements == 0)
    {
        clear_sep_matrix(dst);
        return ZERO_MATRIX_ERROR;
    }

    dst->A = (int *)malloc(sizeof(int) * elements);
    dst->IA = (int *)malloc(sizeof(int) * elements);
    if (dst->A == NULL || dst->IA == NULL)
    {
        clear_sep_matrix(dst);
        return MEMORY_ERROR;
    }
    
    int flag;
    elements = 0;
    for (int i = 0; i < src->columns; i++)
    {
        flag = 0;
        for (int j = 0; j < src->rows; j++)
            if (src->elements[j * columns + i] != 0)
            {
                dst->A[elements] = src->elements[j * columns + i];
                dst->IA[elements] = j;
                if (flag == 0)
                {
                    dst->JA_head = JA_add_end(dst->JA_head, JA_create(elements));
                    flag = 1;
                }
                elements++;
            }
        if (flag == 0)
            dst->JA_head = JA_add_end(dst->JA_head, JA_create(elements));
    }
    dst->JA_head = JA_add_end(dst->JA_head, JA_create(elements));

    return 0;
}


void print_sep_matrix(matrix_t *matrix)
{
    if (matrix->rows == 0 || matrix->columns == 0)
        return;
    for (int i = 0; i < JA_get_tail(matrix->JA_head)->JA; i++)
        printf("%d ", matrix->A[i]);
    printf("\n");

    for (int i = 0; i < JA_get_tail(matrix->JA_head)->JA; i++)
        printf("%d ", matrix->IA[i]);
    printf("\n");

    JA_apply(matrix->JA_head, JA_print, "%d ");
    printf("\n");
}


void shuffle(int* arr, int N)
{
    srand(time(NULL));
 
    for (int i = N - 1; i >= 1; i--)
    {
        int j = rand() % (i + 1);
 
        int tmp = arr[j];
        arr[j] = arr[i];
        arr[i] = tmp;
    }
}


int make_random_matrix(simple_matrix_t *matrix, int procent, int rows, int columns)
{
    int count = (int)(((float)procent / 100.0) * (rows * columns));

    matrix->rows = rows;
    matrix->columns = columns;
    matrix->elements = calloc(rows * columns, sizeof(int));
    if (matrix->elements == NULL)
    {
        clear_matrix(matrix);
        return MEMORY_ERROR;
    }
    
    for (int i = 0; i <= count; i++)
        matrix->elements[i] = rand() % 100 - 50;

    shuffle(matrix->elements, rows * columns);

    return 0;
}


simple_matrix_t *simple_mult(simple_matrix_t *matrix, simple_matrix_t *column)
{
    if (matrix->columns != column->rows || column->columns != 1)
    {
        printf("WRONG DIMENSIONS!");
        return NULL;
    }
    simple_matrix_t *result;
    result = init_matrix();
    result->rows = matrix->rows;
    result->columns = column->columns;
    result->elements = (int *)malloc((result->rows * result->columns) * sizeof(int));
    if (result->elements == NULL)
    {
        free(result);
        return NULL;
    }
    int sum = 0;
    for (int i = 0; i < result->rows; i++)
    {
        for (int j = 0; j < result->rows; j++)
        {
            sum += matrix->elements[i * matrix->columns + j] * column->elements[j];
        }
        result->elements[i] = sum;
        sum = 0;
    }
    return result;
}


int mult(matrix_t *res, matrix_t *m, matrix_t *v)
{
    if (m->columns != v->rows || v->columns != 1)
        return DIMENSION_ERROR;
    res->rows = m->rows;
    res->columns = v->columns;
    res->A = (int *)malloc(res->rows * res->columns * sizeof(int));

    if (res->A == NULL)
    {
        clear_sep_matrix(res);
        return MEMORY_ERROR;
    }
    
    res->IA = (int *)malloc(res->rows * res->columns * sizeof(int));
    if (res->IA == NULL)
    {
        clear_sep_matrix(res);
        return MEMORY_ERROR;
    }
    
    int *IP = (int *)malloc(v->rows * sizeof(int));
    if (IP == NULL)
    {
        clear_sep_matrix(res);
        return MEMORY_ERROR;
    }
    for (int i = 0; i < v->rows; i++)
        IP[i] = -1;
    for (int i = 0; i < JA_get_tail(v->JA_head)->JA; i++)
        IP[v->IA[i]] = i;

    int *IPi = (int *)malloc(v->rows * sizeof(int));
    int *B = (int *)malloc(m->columns * sizeof(int));
    int *JB = (int *)malloc(m->columns * sizeof(int));
    int h, elements;
    if (B == NULL || JB == NULL || IPi == NULL)
    {
        free(B);
        free(JB);
        free(IPi);
        clear_sep_matrix(res);
        return MEMORY_ERROR;
    }
    // сколько строк
    int res_elements = 0;
    for (int i = 0; i < m->rows; i++)
    {
        h = 0;
        elements = 0;
        memmove(IPi, IP, v->rows * sizeof(int));
        // Заполняем вектор-строку
        for (int j = 0; j < JA_get_tail(m->JA_head)->JA; j++)
        {
            if (m->IA[j] == i)
            {
                B[elements] = m->A[j];
                int k = 0;
                JA_t *curr = m->JA_head;
                for (; curr->JA < j; curr = curr->next, k++)
                    ;
                if (curr->JA == j && curr->JA == (*curr->next).JA)
                    JB[elements] = k + 1;
                else if (curr->JA == j)
                    JB[elements] = k;
                else
                    JB[elements] = k - 1;
                elements++;
            }
        }
        // Находим произведение;
        for (int j = 0; j < elements; j++)
            if (IPi[JB[j]] != -1)
                h += B[j] * v->A[JB[j]];
        
        // Отправляем h в res
        if (h != 0)
        {
            res->A[res_elements] = h;
            res->IA[res_elements++] = i;
            res->JA_head = JA_create(0);
        }
    }
    if (res_elements != 0)
        res->JA_head = JA_add_end(res->JA_head, JA_create(res_elements));
    else
        return ZERO_MATRIX_ERROR;
    
    return 0;
}

int main(int argc, char**argv)
{
    int rc;
    FILE *f;
    if (argc == 2)
    {
        f = fopen(argv[1], "r");
        if (f == NULL)
            return OPEN_FILE_ERROR;
    }

    int choice;
    rc = scanf("%d", &choice);
    if (rc != 1)
        return CHOICE_ERROR;

    simple_matrix_t *simple_matrix;
    simple_matrix_t *simple_vec;
    simple_matrix_t *res;
    matrix_t *matrix;
    matrix_t *vec;
    matrix_t *result;
    switch (choice)
    {
        case 1:
        {
            printf("Введите матрицу 1(координаты, значение)\n");
            simple_matrix = init_matrix();
            rc = read_matrix(simple_matrix, stdin);
            if (rc != 0)
                return rc;
            if (simple_matrix->rows <= 12 && simple_matrix->columns <= 12)
            {
                printf("Матрица 1\n");
                print_matrix(simple_matrix);
            }
            simple_vec = init_matrix();
            printf("Введите матрицу 2(координаты, значение)\n");
            rc = read_matrix(simple_vec, stdin);
            if (rc != 0)
                return rc;
            if (simple_vec->rows <= 12 && simple_vec->columns <= 12)
            {
                printf("Матрица 2\n");
                print_matrix(simple_vec);
            }
            res = init_matrix();
            res = simple_mult(simple_matrix, simple_vec);
            if (res == NULL)
                return MEMORY_ERROR;
            if (res->rows <= 12 && res->columns <= 12)
            {
                printf("Результат\n");
                print_matrix(res);
            }
            matrix = init_spare_matrix();
            rc = transform(matrix, simple_matrix);
            if (rc != 0)
            {
                if (rc == ZERO_MATRIX_ERROR)
                    printf("Результат - вектор из нулей");
                return rc;
            }
            vec = init_spare_matrix();
            rc = transform(vec, simple_vec);
            if (rc != 0)
            {
                if (rc == ZERO_MATRIX_ERROR)
                    printf("Результат - вектор из нулей");
                return rc;
            }
            result = init_spare_matrix();
            rc = mult(result, matrix, vec);
            if (rc != 0)
            {
                if (rc == ZERO_MATRIX_ERROR)
                    printf("Результат - вектор из нулей");
                return rc;
            }
            print_sep_matrix(result);
            break;
        }
        case 2:
        {
            // printf("Введите матрицу 1(координаты, значение)\n");
            simple_matrix = init_matrix();
            rc = read_matrix(simple_matrix, f);
            if (rc != 0)
                return rc;
            if (simple_matrix->rows <= 12 && simple_matrix->columns <= 12)
            {
                printf("Матрица 1\n");
                print_matrix(simple_matrix);
            }
            simple_vec = init_matrix();
            // printf("Введите матрицу 2(координаты, значение)\n");
            rc = read_matrix(simple_vec, f);
            if (rc != 0)
                return rc;
            if (simple_vec->rows <= 12 && simple_vec->columns <= 12)
            {
                printf("Матрица 2\n");
                print_matrix(simple_vec);
            }
            res = init_matrix();
            res = simple_mult(simple_matrix, simple_vec);
            if (res == NULL)
                return MEMORY_ERROR;
            if (res->rows <= 12 && res->columns <= 12)
            {
                printf("Результат\n");
                print_matrix(res);
            }
            matrix = init_spare_matrix();
            rc = transform(matrix, simple_matrix);
            if (rc != 0)
            {
                if (rc == ZERO_MATRIX_ERROR)
                    printf("Результат - вектор из нулей");
                return rc;
            }
            vec = init_spare_matrix();
            rc = transform(vec, simple_vec);
            if (rc != 0)
            {
                if (rc == ZERO_MATRIX_ERROR)
                    printf("Результат - вектор из нулей");
                return rc;
            }
            result = init_spare_matrix();
            rc = mult(result, matrix, vec);
            if (rc != 0)
            {
                if (rc == ZERO_MATRIX_ERROR)
                    printf("Результат - вектор из нулей");
                return rc;
            }
            print_sep_matrix(result);
            break;
        }
        case 3:
        {
            printf("Введите процент, количество строк и столбцов\n");
            int rows, columns, proc;
            rc = scanf("%d %d %d", &proc, &rows, &columns);
            if (rc != 3)
                return CHOICE_ERROR;
            simple_matrix = init_matrix();
            rc = make_random_matrix(simple_matrix, proc, rows, columns);
            if (rc != 0)
                return rc;
            if (simple_matrix->rows <= 12 && simple_matrix->columns <= 12)
            {
                printf("Матрица 1\n");
                print_matrix(simple_matrix);
            }
            simple_vec = init_matrix();
            printf("Введите процент, количество строк и столбцов\n");
            rc = scanf("%d %d %d", &proc, &rows, &columns);
            if (rc != 3)
                return CHOICE_ERROR;
            rc = make_random_matrix(simple_vec, proc, rows, columns);
            if (rc != 0)
                return rc;
            if (simple_vec->rows <= 12 && simple_vec->columns <= 12)
            {
                printf("Матрица 2\n");
                print_matrix(simple_vec);
            }
            res = init_matrix();
            res = simple_mult(simple_matrix, simple_vec);
            if (res == NULL)
                return MEMORY_ERROR;
            if (res->rows <= 12 && res->columns <= 12)
            {
                printf("Результат\n");
                print_matrix(res);
            }
            matrix = init_spare_matrix();
            rc = transform(matrix, simple_matrix);
            if (rc != 0)
            {
                if (rc == ZERO_MATRIX_ERROR)
                    printf("Результат - вектор из нулей");
                return rc;
            }
            vec = init_spare_matrix();
            rc = transform(vec, simple_vec);
            if (rc != 0)
            {
                if (rc == ZERO_MATRIX_ERROR)
                    printf("Результат - вектор из нулей");
                return rc;
            }
            result = init_spare_matrix();
            rc = mult(result, matrix, vec);
            if (rc != 0)
            {
                if (rc == ZERO_MATRIX_ERROR)
                    printf("Результат - вектор из нулей");
                return rc;
            }
            print_sep_matrix(result);
            break;
        }
        default:
            break;
    }

    return 0;
}