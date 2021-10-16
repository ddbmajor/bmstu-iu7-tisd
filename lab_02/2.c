#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <stdlib.h>
#include <inttypes.h>
#include <sys/time.h>


#define ARG_ERROR 1
#define OPEN_FILE_ERROR 2
#define CHOICE_ERROR 3
#define EMPTY_FILE_ERROR 4
#define INPUT_ERROR 5
#define NOT_FOUND_ERROR 6
#define OVERFLOW_ERROR 7

#define MAXNAMELEN 30
#define MAXGROUPLEN 10
#define MAXDATELEN 10
#define MAXSTREETLEN 30
#define MAXBUILDLEN 5
#define MAXSTUDENTCOUNT 1000


typedef enum
{
    male = 0,
    female = 1
} gender_t;

typedef enum
{
    rich = 0,
    poor = 1
} live_kind_t;

typedef struct
{
    char street[MAXSTREETLEN];
    char building[MAXBUILDLEN];
    unsigned short int room;
} home_t;

typedef struct
{
    unsigned short int building;
    unsigned short int room;
} coliving_t;

typedef union
{
    home_t home;
    coliving_t coliving;
}adress_t;


typedef struct
{
    char surname[MAXNAMELEN];
    char name[MAXNAMELEN];
    char group[MAXGROUPLEN];
    gender_t gen;
    unsigned short int age;
    float score;
    char date[MAXDATELEN];
    live_kind_t kind;
    adress_t adress;
} student_t;

typedef struct 
{
    unsigned int position;
    float score;
}key_t;


int namevalidate(char *str)
{
    for (int i = 0; str[i] != '\0'; i++)
    {
        if (isdigit(str[i]))
            return 0;
        if (str[i] == '\n')
            str[i] = '\0';
    }
    return 1;
}


int groupvalidate(char *str)
{  
    for (int i = 0; str[i] != '\0'; i++)
    {
        if (str[i] == '\n')
            str[i] = '\0';
    }
    return 1;
}


int datevalidate(char *str)
{
    int day, month, year;
    int rc;
    for (int i = 0; str[i] != '\0'; i++)
        if (str[i] == '\n')
            str[i] = '\0';
    rc = sscanf(str, "%d.%d.%d", &day, &month, &year);
    if (rc != 3)
        return 0;
    if (month <= 0 || month > 12)
        return 0;
    if (day <= 0)
        return 0;
    switch (month)
    {
    case 1:
        if (day > 31)
            return 0;
        break;
    case 3:
        if (day > 31)
            return 0;
        break;
    case 5:
        if (day > 31)
            return 0;
        break;
    case 7:
        if (day > 31)
            return 0;
        break;
    case 8:
        if (day > 31)
            return 0;
        break;
    case 10:
        if (day > 31)
            return 0;
        break;
    case 12:
        if (day > 31)
            return 0;
        break;
    case 4:
        if (day > 30)
            return 0;
        break;
    case 6:
        if (day > 30)
            return 0;
        break;
    case 9:
        if (day > 30)
            return 0;
        break;
    case 11:
        if (day > 30)
            return 0;
        break;
    case 2:
        if (year % 4 == 0)
        {
            if (day > 29)
                return 0;
        }
        else
        {
            if (day > 28)
                return 0;
        }
        break;
    default:
        return 0;
    }
    return 1;
}


int adressvalidate(char *str)
{
    for (int i = 0; str[i] != '\0'; i++)
    {
        if (str[i] == '\n')
            str[i] = '\0';
    }
    return 1;
}


int readstruct(student_t *student, FILE *f, int isadding)
{
    char tempstr[MAXNAMELEN + 2];
    short int num;
    float tempscore;
    student_t tempstudent;
    
    if (isadding == 1)
        printf("Insert surname(30 symbols)\n");
    if (fgets(tempstr, MAXNAMELEN + 2, f) && tempstr[strlen(tempstr) - 1] == '\n')
    {
        if (namevalidate(tempstr)) // добавить удаление \n
            strcpy(tempstudent.surname, tempstr);
        else
            return INPUT_ERROR;
    }
    else
        return INPUT_ERROR;

    if (isadding == 1)
        printf("Insert name(30 symbols)\n");
    if (fgets(tempstr, MAXNAMELEN + 2, f) && tempstr[strlen(tempstr) -1] == '\n')
    {
        if (namevalidate(tempstr))
            strcpy(tempstudent.name, tempstr);
        else
            return INPUT_ERROR;
    }
    else
        return INPUT_ERROR;

    if (isadding == 1)
        printf("Insert group(10 symbols)\n");
    if (fgets(tempstr, MAXGROUPLEN + 2, f) && tempstr[strlen(tempstr) - 1] == '\n')
    {
        if (groupvalidate(tempstr))
            strcpy(tempstudent.group, tempstr);
        else
            return INPUT_ERROR;
    }
    else
        return INPUT_ERROR;
    
    if (isadding == 1)
        printf("Insert gender(m - male, f - female)\n");
    if (fgets(tempstr, 3, f) && tempstr[strlen(tempstr) - 1] == '\n')
    {
        if (tempstr[0] == 'm')
            tempstudent.gen = male;
        else if (tempstr[0] == 'f')
            tempstudent.gen = female;
        else
            return INPUT_ERROR;
    }
    else
        return INPUT_ERROR;

    if (isadding == 1)
        printf("Insert age\n");
    if (fscanf(f ,"%hi", &num) == 1)
    {
        if (num > 0)
            tempstudent.age = num;
        else
            return INPUT_ERROR;
    }
    else
        return INPUT_ERROR;

    if (isadding == 1)
        printf("Insert average score(>0.0 and <=5.0)\n");
    if (fscanf(f ,"%f", &tempscore) == 1)
    {
        if ((tempscore > 0.0 && tempscore < 5.0) || fabs(tempscore - 5.0) < 1e-8)
            tempstudent.score = tempscore;
        else
            return INPUT_ERROR;
    }
    else
        return INPUT_ERROR;
    
    fgets(tempstr, 2, f);
    
    if (isadding == 1)
        printf("Insert date(dd.mm.yyyy)\n");
    if (fgets(tempstr, MAXDATELEN + 2, f) && tempstr[strlen(tempstr) - 1] == '\n')
    {
        if (datevalidate(tempstr))
            strcpy(tempstudent.date, tempstr);
        else
            return INPUT_ERROR;
    }
    else
        return INPUT_ERROR;
    
    if (isadding == 1)
        printf("Insert type of living(home or coliving)\n");
    if (fgets(tempstr, 20, f) && tempstr[strlen(tempstr) - 1] == '\n')
    {
        if (strcmp(tempstr, "home\n") == 0)
            tempstudent.kind = rich;
        else if (strcmp(tempstr, "coliving\n") == 0)
            tempstudent.kind = poor;
        else
            return INPUT_ERROR;
    }
    else
        return INPUT_ERROR;
    
    if (tempstudent.kind == rich)
    {
        if (isadding == 1)
            printf("Insert street(30 symbols)\n");
        if (fgets(tempstr, MAXSTREETLEN + 2, f) && tempstr[strlen(tempstr) - 1] == '\n')
        {
            if (adressvalidate(tempstr))
               strcpy(tempstudent.adress.home.street, tempstr);
            else
                return INPUT_ERROR;
        }
        else
            return INPUT_ERROR;

        if (isadding == 1)
            printf("Insert buiding(5 symbols)\n");
        if (fgets(tempstr, MAXBUILDLEN + 2, f) && tempstr[strlen(tempstr) - 1] == '\n')
        {
            if (adressvalidate(tempstr))
               strcpy(tempstudent.adress.home.building, tempstr);
            else
                return INPUT_ERROR;
        }
        else
            return INPUT_ERROR;
        
        if (isadding == 1)
            printf("Insert № of apartment\n");
        if (fscanf(f ,"%hi", &num) == 1)
        {
            if (num > 0)
                tempstudent.adress.home.room = num;
            else
                return INPUT_ERROR;
        }
        else
            return INPUT_ERROR;
    }
    else if (tempstudent.kind == poor)
    {
        if (isadding == 1)
            printf("Insert № of coliving\n");
        if (fscanf(f ,"%hi", &num) == 1)
        {
            if (num > 0)
                tempstudent.adress.coliving.building = num;
            else
                return INPUT_ERROR;
        }
        else
            return INPUT_ERROR;

        if (isadding == 1)
            printf("Insert № of room\n");
        if (fscanf(f ,"%hi", &num) == 1)
        {
            if (num > 0)
                tempstudent.adress.coliving.room = num;
            else
                return INPUT_ERROR;
        }
        else
            return INPUT_ERROR;
    }
    fgets(tempstr, 2, f);
    *student = tempstudent;
    return 0;
}


int readfile(student_t data[MAXSTUDENTCOUNT], int *count, char *filename)
{
    /*
    if (*count != 0)
    {
        printf("Data already exist!!!\n");
        return 0;
    }
    */
    int rc;
    student_t temp;
    FILE *f = fopen(filename, "r");
    if (f == NULL)
        return OPEN_FILE_ERROR;
    else
    {   
        rc = readstruct(&temp, f, 0);
        if (rc != 0 && feof(f))
            return EMPTY_FILE_ERROR;
        else if (rc != 0)
            return rc;
        data[*count] = temp;
        *count += 1;
        while (rc == 0)
        {
            rc = readstruct(&temp, f, 0);
            if (rc == 0)
            {
                data[*count] = temp;
                *count += 1;
                if (*count == MAXSTUDENTCOUNT)
                {
                    rc = readstruct(&temp, f, 0);
                    if (rc != 0 && feof(f))
                        return 0;
                    else if (rc != 0)
                        return rc;
                    else if (rc == 0)
                        return OVERFLOW_ERROR;
                }
            }
            else if (rc != 0 && feof(f))
                return 0;
            else
                return rc;
        }
    }
    return 0;
}


int makekeys(key_t keys[MAXSTUDENTCOUNT], student_t data[MAXSTUDENTCOUNT], int count)
{
    if (count == 0)
        return EMPTY_FILE_ERROR;
    for (int i = 0; i < count; i++)
    {
        keys[i].position = i;
        keys[i].score = data[i].score;
    }
    return 0;
}


void printstruct(student_t student)
{
    printf("%s\n", student.surname);
    printf("%s\n", student.name);
    printf("%s\n", student.group);
    if (student.gen == male)
        printf("m\n");
    else if (student.gen == female)
        printf("f\n");
    printf("%hi\n", student.age);
    printf("%f\n", student.score);
    printf("%s\n", student.date);
    if (student.kind == rich)
    {
        printf("%s\n", student.adress.home.street);
        printf("%s\n", student.adress.home.building);
        printf("%hi\n", student.adress.home.room);
    }
    else if (student.kind == poor)
    {
        printf("%hi\n", student.adress.coliving.building);
        printf("%hi\n", student.adress.coliving.room);
    }
}


int printdata(student_t data[MAXSTUDENTCOUNT], int count)
{
    if (count == 0)
        return EMPTY_FILE_ERROR;
    printf("\n");
    for (int i = 0; i < count; i++)
    {
        printf("%d student:\n", i + 1);
        printstruct(data[i]);
        printf("\n");
    }
    return 0;
}


int printkeys(key_t keys[MAXSTUDENTCOUNT], int count)
{
    if (count == 0)
        return EMPTY_FILE_ERROR;
    printf("\n");
    for (int i = 0; i < count; i++)
    {
        printf("%d student:\n", i);
        printf("pos: %d\tscore: %f", keys[i].position, keys[i].score);
        printf("\n");
    }
    return 0;
}


int printbykeys(student_t data[MAXSTUDENTCOUNT], key_t keys[MAXSTUDENTCOUNT], int count)
{
    if (count == 0)
        return EMPTY_FILE_ERROR;
    for (int i = 0; i < count; i++)
    {
        printf("%d student:\n", i + 1);
        printstruct(data[keys[i].position]);
        printf("\n");
    }
    return 0;
}


int printbyyear(student_t data[MAXSTUDENTCOUNT], int count, int thatyear)
{
    int flag = 0;
    if (count == 0)
        return EMPTY_FILE_ERROR;
    int day, month, year;
    char tmpstr[MAXDATELEN];

    for (int i = 0; i < count; i++)
        if (data[i].kind == poor)
        {
            strcpy(tmpstr, data[i].date);
            sscanf(data[i].date, "%d.%d.%d", &day, &month, &year);
            if (year == thatyear)
            {
                printf("%d student:\n", flag + 1);
                printstruct(data[i]);
                printf("\n");
                flag += 1;
            }
        }
    if (flag)
        return 0;
    else
    {
        printf("No data\n");
        return 0;
    }
}


int addstruct(student_t data[MAXSTUDENTCOUNT], int *count)
{
    if (*count == MAXSTUDENTCOUNT)
    {
        printf("Array is full!!!\n");
        return 0;
    }
    int rc;
    student_t tmp;
    rc = readstruct(&tmp, stdin, 1);
    if (rc != 0)
        return rc;
    else
    {
        data[*count] = tmp;
        *count += 1;
    }
    return 0;
}


int addkey(key_t keys[MAXSTUDENTCOUNT], int count, student_t student)
{
    if (count == 0)
        return EMPTY_FILE_ERROR;
    keys[count - 1].position = count - 1;
    keys[count - 1].score = student.score;
    return 0;
}


int delstruct(student_t data[MAXSTUDENTCOUNT], int *count, char *key)
{
    short int flag = 0;
    for (int i = 0; i < *count; i++)
    {
        if (strcmp(data[i].surname, key) == 0)
        {
            for (int j = i; j < *count - 1; j++)
                data[j] = data[j + 1];
            *count -= 1;
            flag = 1;
        }
    }
    if (flag)
        return 0;
    else
        return NOT_FOUND_ERROR;
}


int compare_student(const void *first, const void *second)
{
    const student_t *a = first;
    const student_t *b = second;
    if (fabs(a->score - b->score) < 1e-8)
        return 0;
    if (a->score < b->score)
        return -1;
    if (a->score > b->score)
        return 1;
    return 0;
}


int fullquicksort(student_t data[MAXSTUDENTCOUNT], int count)
{
    if (count == 0)
        return EMPTY_FILE_ERROR;
    if (count == 1)
        return 0;
    qsort(data, count, sizeof(student_t), compare_student);
    return 0;
}


void push(void *first, void *second, size_t size)
{
    for (size_t i = 0; i < size; ++i)
        *((char *) first + i) = *((char *) second + i);
}


void mysort(void *base, int num, size_t size, int (*compare)(const void *, const void *))
{
    if (num == 1)
        return;
    char *start = (char *)base;
    char *key = malloc(size);
    if (key == NULL)
    {
        free(key);
        return;
    }

    int j;
    for (int i = 1; i < num; i++)
    {
        push(key, start + i * size, size);
        j = i - 1;
        while (j >= 0 && compare(key, start + j * size) < 0)
        {
            push(start + (j + 1) * size, start + j * size, size);
            j--;
        }
        push(start + (j + 1) * size, key, size);
    }
    free(key);
}


int fullslowsort(student_t data[MAXSTUDENTCOUNT], int count)
{
    if (count == 0)
        return EMPTY_FILE_ERROR;
    if (count == 1)
        return 0;
    mysort(data, count, sizeof(student_t), compare_student);
    return 0;
}


int compare_key(const void *first, const void *second)
{
    const key_t *a = first;
    const key_t *b = second;
    if (fabs(a->score - b->score) < 1e-8)
        return 0;
    if (a->score < b->score)
        return -1;
    if (a->score > b->score)
        return 1;
    return 0;
}


int keysquicksort(key_t keys[MAXSTUDENTCOUNT], int count)
{
    if (count == 0)
        return EMPTY_FILE_ERROR;
    if (count == 1)
        return 0;
    qsort(keys, count, sizeof(key_t), compare_key);
    return 0;
}


int keysslowsort(key_t keys[MAXSTUDENTCOUNT], int count)
{
    if (count == 0)
        return EMPTY_FILE_ERROR;
    if (count == 1)
        return 0;
    mysort(keys, count, sizeof(key_t), compare_key);
    return 0;
}


int efficiency(char *filename)
{
    student_t data[MAXSTUDENTCOUNT];
    key_t keys[MAXSTUDENTCOUNT];
    int count;
    readfile(data, &count, filename);
    makekeys(keys, data, count);
    student_t arr[count];
    key_t keyarr[count];

    int64_t fullquicktime;
    int64_t fullslowtime;
    int64_t keysquicktime;
    int64_t keysslowtime;

    int fullquicksize;
    int fullslowsize;
    int keysquicksize;
    int keysslowsize;

    // quick with data
	int n = 100;
	int64_t sum;
	struct timeval tv_start, tv_stop;

	sum = 0;
	for (int i = 0; i < n; i++)
	{
		memmove(arr, data, sizeof(student_t) * count);
		gettimeofday(&tv_start, NULL);
		fullquicksort(arr, count);
		gettimeofday(&tv_stop, NULL);
		sum += (tv_stop.tv_sec - tv_start.tv_sec) * 1000000LL + (tv_stop.tv_usec - tv_start.tv_usec);
	}
	sum /= n;
	fullquicktime = sum;
    fullquicksize = sizeof(student_t) * count;

    // slow with data
	sum = 0;
	for (int i = 0; i < n; i++)
	{
		memmove(arr, data, sizeof(student_t) * count);
		gettimeofday(&tv_start, NULL);
		fullslowsort(arr, count);
		gettimeofday(&tv_stop, NULL);
		sum += (tv_stop.tv_sec - tv_start.tv_sec) * 1000000LL + (tv_stop.tv_usec - tv_start.tv_usec);
	}
	sum /= n;
	fullslowtime = sum;
    fullslowsize = sizeof(student_t) * count;

    // quick with keys
	sum = 0;
	for (int i = 0; i < n; i++)
	{
		memmove(keyarr, keys, sizeof(key_t) * count);
		gettimeofday(&tv_start, NULL);
		keysquicksort(keyarr, count);
		gettimeofday(&tv_stop, NULL);
		sum += (tv_stop.tv_sec - tv_start.tv_sec) * 1000000LL + (tv_stop.tv_usec - tv_start.tv_usec);
	}
	sum /= n;
	keysquicktime = sum;
    keysquicksize = sizeof(key_t) * count;

    // slow with keys
	sum = 0;
	for (int i = 0; i < n; i++)
	{
		memmove(keyarr, keys, sizeof(key_t) * count);
		gettimeofday(&tv_start, NULL);
		keysslowsort(keyarr, count);
		gettimeofday(&tv_stop, NULL);
		sum += (tv_stop.tv_sec - tv_start.tv_sec) * 1000000LL + (tv_stop.tv_usec - tv_start.tv_usec);
	}
	sum /= n;
	keysslowtime = sum;
    keysslowsize = sizeof(key_t) * count;

    printf("------------------------------------------\n");
    printf("|    |fullquick|fullslow|keyquick|keyslow|\n");
    printf("------------------------------------------\n");
    printf("|time|%-9" PRId64 "|%-8" PRId64 "|%-8" PRId64 "|%-7" PRId64 "|\n", fullquicktime, fullslowtime, keysquicktime, keysslowtime);
    printf("------------------------------------------\n");
    printf("|size|%-9d|%-8d|%-8d|%-7d|\n", fullquicksize, fullslowsize, keysquicksize, keysslowsize);
    printf("------------------------------------------\n");

    return 0;
}


int mainprocess(char *filename)
{
    int rc;
    char tmp[MAXNAMELEN];
    int tmpyear;
    student_t data[MAXSTUDENTCOUNT];
    key_t keys[MAXSTUDENTCOUNT];
    
    int count = 0;
    char *menu = "Выберите пункт меню:\n0 - Выйти из программы\n1 - Прочитать файл\n2 - Вывести данные\n3 - Вывести таблицу ключей\n"
    "4 - Добавить запись\n5 - Удалить запись\n6 - Отсортировать записи по оценке(быстрая, исходная таблица)\n"
    "7 - Отсортировать записи по оценке(медленная исходная таблица)\n8 - Отсортировать таблицу ключей(быстрая)\n"
    "9 - Отсортировать таблицу ключей(медленная)\n10 - Вывести данные по таблице ключей\n"
    "11 - Вывод таблицы эффективности\n12 - Вывести список студентов, указанного года поступления, живущих в общежитии.\n";
    int choice;
    
    rc = 0;
    while (rc == 0)
    {
        printf("%s", menu);
        rc = scanf("%d", &choice);
            if (rc != 1)
                return CHOICE_ERROR;
        switch (choice)
        {
        case 0:
        {
            return 0;
            break;
        }
        case 1:
        {
            rc = readfile(data, &count, filename);
            if (rc != 0)
                return rc;
            rc = makekeys(keys, data, count);
            printf("Done!\n");
            break;
        }
        case 2:
        {
            rc = printdata(data, count);
            if (rc != 0)
                return rc;
            printf("Done!\n");
            break;
        }
        case 3:
        {
            rc = printkeys(keys, count);
            if (rc != 0)
                return rc;
            printf("Done!\n");
            break;
        }
        case 4:
        {
            fgets(tmp, 3, stdin);
            rc = addstruct(data, &count);
            if (rc != 0)
                return rc;
            addkey(keys, count, data[count - 1]);
            printf("Done!\n");
            break;
        }
        case 5:
        {
            printf("\nInput surname\n");
            fgets(tmp, 3, stdin);
            fgets(tmp, MAXNAMELEN, stdin);
            tmp[strlen(tmp) - 1] = '\0';
            rc = delstruct(data, &count, tmp);
            if (rc != 0)
                return rc;
            makekeys(keys, data, count);
            printf("Done!\n");
            break;
        }
        case 6:
        {
            rc = fullquicksort(data, count);
            if (rc != 0)
                return rc;
            printf("Done!\n");
            break;
        }
        case 7:
        {
            rc = fullslowsort(data, count);
            if (rc != 0)
                return rc;
            printf("Done!\n");
            break;

        }
        case 8:
        {
            rc = keysquicksort(keys, count);
            if (rc != 0)
                return rc;
            printf("Done!\n");
            break;
        }
        case 9:
        {
            rc = keysslowsort(keys, count);
            if (rc != 0)
                return rc;
            printf("Done!\n");
            break;
        }
        case 10:
        {
            rc = printbykeys(data, keys, count);
            if (rc != 0)
                return rc;
            printf("Done!\n");
            break;
        }
        case 11:
        {
            rc = efficiency(filename);
            if (rc != 0)
                return rc;
            printf("Done!\n");
            break;
        }
        case 12:
        {
            printf("\nInput year\n");
            scanf("%d", &tmpyear);
            if (tmpyear < 0)
                return INPUT_ERROR;
            fgets(tmp, 3, stdin);
            rc = printbyyear(data, count, tmpyear);
            if (rc != 0)
                return rc;
            printf("Done!\n");
            break;
        }
        default:
        {
            printf("Wrong choice\n");
            break;
        }
        }
    }
    return 0;
}


int main(int argc, char **argv)
{
    int rc;
    if (argc == 2)
    {
        rc = mainprocess(argv[1]);
        if (rc != 0)
            return rc;
    }
    else
        return ARG_ERROR;
    return 0;
}