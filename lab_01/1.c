#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>


#define MAX_MANTISSA 65
#define MAX_NUMBER 65
#define MAX_INPUT 30

#define INPUT_ERROR 1
#define MULTIPLICATION_ERROR 2


typedef enum 
{
	plus = 1,
	minus = -1
} my_sign;


typedef	struct
{
	my_sign sign; // знак
	short int mantissa[MAX_MANTISSA]; // мантисса
	short int mantissa_len; // длина мантиссы
	int exponent; // порядок
} my_float;


typedef struct
{
	my_sign sign; // знак
	short int number[MAX_NUMBER]; // числа
	short int number_len; // длина числа
} my_int;


int mantissa_validation(char *str)
{
	bool flag = false;
	for (int i = 0; str[i] != '\0'; i++)
		if (isdigit(str[i]))
			flag = true;
	if (flag == false)
		return INPUT_ERROR;

	// Проверка на валидность всех символов
	for (int i = 0; str[i] != '\0' && str[i] != '\n'; i++)
		if (!(str[i] == '-' || str[i] == '+' || isdigit(str[i]) || str[i] == '.' || str[i] == ','))
			return INPUT_ERROR;

	// Проверка на уникальность точки(запятой)
	short int counter = 0;
	for (int i = 0; str[i] != '\0'; i++)
	{
		if (str[i] == '.' || str[i] == ',')
			counter++;
		if (counter > 1)
			return INPUT_ERROR;
	}
	
	// Проверка на уникальность знака
	for (int i = 0; str[i] != '\0'; i++)
	{
		if ((str[i] == '+' || str[i] == '-') & (i != 0))
			return INPUT_ERROR;
	}

	for (int i = 0; str[i] != '\0'; i++)
		if (str[i] == '\n' || str[i] == '\r')
			str[i] = '\0';

	if (str[0] == '+' || str[0] == '-')
	{
		if (strlen(str) - counter - 1 > MAX_INPUT)
			return INPUT_ERROR;
	}
	else if (strlen(str) - counter > MAX_INPUT)
			return INPUT_ERROR;

	return 0;
}


void mantissa_input(char str[], my_float *number)
{

	if (str[0] == '-')
		number->sign = minus;
	else
		number->sign = plus;

	bool not_zero_flag = false;
	bool point_flag = false;

	for (int i = 0; str[i] != '\0'; i++)
	{
		if (isdigit(str[i]) && str[i] != '0')
			not_zero_flag = true;

		if (isdigit(str[i]) && not_zero_flag == true && point_flag == false)
		{
			(number->mantissa)[number->mantissa_len] = str[i] - '0';
			number->mantissa_len += 1;
		}
		
		if (str[i] == '.' || str[i] == ',')
			point_flag = true;

		if (isdigit(str[i]) && not_zero_flag == true && point_flag == true)
		{
			(number->mantissa)[number->mantissa_len] = str[i] - '0';
			number->mantissa_len += 1;
			number->exponent -= 1;
		}
		if (isdigit(str[i]) && not_zero_flag == false && point_flag == true)
		{
			number->exponent -= 1;
		}
	}
}


int float_input(my_float *number)
{
	char str[MAX_MANTISSA + 4];
	int num;
	char *endptr;

	short int rc;

	printf("Введите мантису в 30 цифр (знак не обязательно, можно точку или запятую)\n");
	printf("%32c|<- (32 символа)\r", ' ');

	fgets(str, sizeof(str), stdin);
	rc = mantissa_validation(str);
	if (rc)
		return rc;

	mantissa_input(str, number);
	if (number->mantissa_len > MAX_MANTISSA)
		return INPUT_ERROR;
	printf("Введите порядок (знак по умолчанию: плюс)\nE = ");
	fgets(str, sizeof(str), stdin);
	if (str[0] == '\n')
		return INPUT_ERROR;
	num = strtol(str, &endptr, 10);
	if (*endptr == '\n' || *endptr == '\0' || *endptr == '\r')
	{
		if (num < -99999 || num > 99999)
			return INPUT_ERROR;
	}
	else
		return INPUT_ERROR;

	number->exponent += num;

	return 0;
}


int int_input(my_int *number)
{
	char str[MAX_NUMBER + 3];

	printf("Введите целое число в 30 цифр\n");
	printf("%31c|<- (31 символ)\r", ' ');

	fgets(str, sizeof(str), stdin);
	if (str[0] == '\n')
		return INPUT_ERROR;

	// Проверка на валидность всех символов
	for (int i = 0; str[i] != '\0' && str[i] != '\n'; i++)
		if (!(str[i] == '-' || str[i] == '+' || isdigit(str[i])))
			return INPUT_ERROR;

	// Проверка на уникальность знака
	for (int i = 0; str[i] != '\0'; i++)
	{
		if ((str[i] == '+' || str[i] == '-') & (i != 0))
			return INPUT_ERROR;
	}

	for (int i = 0; str[i] != '\0'; i++)
		if (str[i] == '\n' || str[i] == '\r')
			str[i] = '\0';

	if (str[0] == '+' || str[0] == '-')
	{
		if (strlen(str) - 1 > MAX_INPUT)
			return INPUT_ERROR;
	}
	else if (strlen(str) > MAX_INPUT)
		return INPUT_ERROR;

	if (str[0] == '-')
		number->sign = minus;
	else
		number->sign = plus;

	bool not_zero_flag = false;
	for (int i = 0; str[i] != '\0'; i++)
	{
		if (isdigit(str[i]) && str[i] != '0')
			not_zero_flag = true;
		if (isdigit(str[i]) && not_zero_flag == true)
		{
			(number->number)[number->number_len] = str[i] - '0';
			number->number_len += 1;
		}
	}
	if (number->number_len > MAX_NUMBER)
		return INPUT_ERROR;

	return 0;
}


void init_float(my_float *num)
{
	num->sign = plus;
	num->mantissa_len = 0;
	num->exponent = 0;
	for (int i = 0; i < MAX_MANTISSA; i++)
		(num->mantissa)[i] = 0;
}


void init_int(my_int *num)
{
	num->sign = plus;
	num->number_len = 0;
	for (int i = 0; i < MAX_NUMBER; i++)
		(num->number)[i] = 0;
}


void mul_op(my_int *res, my_float first, my_int second, int i)
{
	short int digit = second.number[i];
	short int tmp = 0;
	short int digit_in_mind = 0;
	for (int j = first.mantissa_len - 1; j >= 0; j--)
	{
		tmp = first.mantissa[j] * digit + digit_in_mind;
		res->number[0] = tmp % 10;
		digit_in_mind = tmp / 10;
		res->number_len += 1;
		for (int k = res->number_len; k > 0 && j != 0; k--)
		{
			res->number[k] = res->number[k - 1];
		}
	}
	if (digit_in_mind != 0)
	{
		for (int k = res->number_len; k > 0; k--)
		{
			res->number[k] = res->number[k - 1];
		}
		res->number[0] = digit_in_mind;
		res->number_len++;
	}
	for (int j = 0; j < second.number_len - i - 1; j++)
	{
		res->number[res->number_len] = 0;
		res->number_len++;
	}
}

void add_op(my_float *result, my_int num)
{
	int amount;
	short int tmp = 0;
	short int first_digit = 0;
	short int second_digit = 0;
	short int digit_in_mind = 0;
	my_int res;
	init_int(&res);

	if (result->mantissa_len > num.number_len)
		amount = result->mantissa_len;
	else
		amount = num.number_len;
	
	for (int i = 0; i < amount; i++)
	{
		if (result->mantissa_len - i - 1 >= 0)
			first_digit = result->mantissa[result->mantissa_len - i - 1];
		else
			first_digit = 0;

		if (num.number_len - i - 1 >= 0)
			second_digit = num.number[num.number_len - i - 1];
		else
			second_digit = 0;

		tmp = first_digit + second_digit + digit_in_mind;
		res.number[0] = tmp % 10;
		res.number_len += 1;
		// Сдвиг вправо кроме последнего раза
		for (int k = res.number_len; k > 0 && i != amount - 1; k--) 
		{
			res.number[k] = res.number[k - 1];
		}
		digit_in_mind = tmp / 10;
	}
	if (digit_in_mind != 0)
	{
		for (int k = res.number_len; k > 0; k--)
		{
			res.number[k] = res.number[k - 1];
		}
		res.number[0] = digit_in_mind;
	}

	for (int i = 0; i < res.number_len; i++)
	{
		result->mantissa[i] = res.number[i];
	}
	result->mantissa_len = res.number_len;

}

void extra_zero_del(my_float *number);

int multiplication(my_float *result, my_float first, my_int second)
{
	my_int tmp;

	init_int(&tmp);

	if (first.sign == second.sign)
		result->sign = plus;
	else
		result->sign = minus;
	
	for (int i = second.number_len - 1; i >= 0 ; i--)
	{
		mul_op(&tmp, first, second, i);
		add_op(result, tmp);
		init_int(&tmp);
	}

	result->exponent += first.exponent;
	if (result->exponent > 99999 || result->exponent < -99999)
		return MULTIPLICATION_ERROR;

	tmp.number[0] = 1;
	tmp.number_len = 1;

	if (result->mantissa_len > 30 && result->mantissa[30] >= 5)
	{
		result->exponent = result->mantissa_len - 30;
		result->mantissa_len = 30;
		add_op(result, tmp);
		extra_zero_del(result);
	}
	else if (result->mantissa_len > 30 && result->mantissa[31] < 5)
		result->mantissa_len = 30;
	
	return 0;
}


int print_float(my_float number)
{
	if (number.exponent + number.mantissa_len > 99999 || number.exponent + number.mantissa_len < -99999)
		{
			printf("Переполнение порядка");
			return MULTIPLICATION_ERROR;
		}
	
	if (number.mantissa_len == 0)
	{
		printf("+0.0 E 0");
		return 0;
	}
	if (number.sign == plus)
		printf("+");
	else
		printf("-");

	printf("0.");

	for (int i = 0; i < number.mantissa_len; i++)
	{
		printf("%d", number.mantissa[i]);
	}
	printf(" E %d", number.exponent + number.mantissa_len);

	return 0;
}


void extra_zero_del(my_float *number)
{
	if (number->mantissa_len == 0)
		return;
	for (int i = number->mantissa_len - 1; number->mantissa[i] == 0; i--)
	{
		number->mantissa_len--;
		number->exponent++;
	}
}


int main(void)
{
	my_float first;
	my_int second;
	short int rc;
	
	init_float(&first);
	init_int(&second);

	rc = float_input(&first);
	if (rc != 0)
	{
		printf("float ошибка ввода");
		return rc;
	}

	extra_zero_del(&first);

	rc = int_input(&second);
	if (rc != 0)
	{
		printf("int ошибка ввода");
		return rc;
	}


	my_float result;
	init_float(&result);
	rc = multiplication(&result, first, second);
	if (rc != 0)
	{
		printf("Переполнение порядка");
		return rc;
	}

	extra_zero_del(&result);
	
	rc = print_float(result);
	if (rc != 0)
		return rc;

	return 0;
}