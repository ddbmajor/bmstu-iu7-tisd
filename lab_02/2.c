#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#define ARG_ERROR 1
#define OPEN_FILE_ERROR 2
#define CHOISE_ERROR 3
#define EMPTY_FILE_ERROR 4
#define INPUT_ERROR 5
#define NOT_FOUND_ERROR 6

#define MAXNAMELEN 30
#define MAXGROUPLEN 10
#define MAXDATELEN 10
#define MAXSTREETLEN 30
#define MAXBUILDLEN 5
#define MAXSTUDENTCOUNT 50
#define MENU "Выберите пункт меню:\n0 - Выйти из программы\n1 - Прочитать файл\n2 - Вывести файл\n3 - Добавить запись\n4 - Удалить запись\n"


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
            }
            else if (rc != 0 && feof(f))
                return 0;
            else
                return rc;
        }
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
    printf("\n");
    for (int i = 0; i < count; i++)
    {
        printf("%d student\n", i + 1);
        printstruct(data[i]);
        printf("\n");
    }
    return 0;
}


int addstruct(student_t data[MAXSTUDENTCOUNT], int *count)
{
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


int mainprocess(char *filename)
{
    int rc;
    char tmp[MAXNAMELEN];
    student_t data[MAXSTUDENTCOUNT];
    int count = 0;
    
    printf(MENU);
    int choise;
    
    rc = 0;
    while (rc == 0)
    {
        rc = scanf("%d", &choise);
            if (rc != 1)
                return CHOISE_ERROR;
        switch (choise)
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
            fgets(tmp, 3, stdin);
            rc = addstruct(data, &count);
            if (rc != 0)
                return rc;
            printf("Done!\n");
            break;
        }
        case 4:
        {
            printf("\nInput surname\n");
            fgets(tmp, 3, stdin);
            fgets(tmp, MAXNAMELEN, stdin);
            tmp[strlen(tmp) - 1] = '\0';
            rc = delstruct(data, &count, tmp);
            if (rc != 0)
                return rc;
            printf("Done!\n");
            break;
        }
        default:
            break;
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