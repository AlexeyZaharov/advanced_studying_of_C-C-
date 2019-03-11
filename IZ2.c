#include <stdio.h>
#include "stdlib.h"
#include "string.h"

#define negative(x) -x

enum {END, BAD_STRING} end;
enum {OK, WAIT_NUMBER} symbol_state = WAIT_NUMBER;
enum {POSITIVE, NEGATIVE} sign = POSITIVE;

size_t num_of_brackets = 0;

void get_str(char** str);

int priority(char op);

double calculate(char* str);

void sub_result (double* numbers, size_t* num_ind, char* operations, size_t* op_ind);

void get_number(char* str, size_t* index, char** str_of_number);

char read(char* str, size_t* index, char** str_of_number);

void other_operation(char elem, char* operations, size_t op_ind);

void minus_operation(char* operations, size_t op_ind, size_t num_ind);

void operation(char elem, char* operations, size_t op_ind, size_t num_ind);

void make_number(char** str_of_number, double* numbers, size_t* num_ind, char* operations, size_t* op_ind);

void in_brackets(char* str, size_t* index, double* numbers, size_t* num_ind, char* operations, size_t* op_ind);

double out_brackets(const double* numbers, size_t num_ind);


int main() {
    char* str = NULL;
    double result = 0;

    get_str(&str);

    if (str == NULL) {
        end = BAD_STRING;
    }
    else {
        result = calculate(str);
    }

    if (end == BAD_STRING) {
        printf("[error]");
    }
    else {
        printf("%.2f", result);
    }

    free(str);

    return 0;
}

void get_str(char** str) {
    char symbol = ' ';
    size_t i = 0;

    while (scanf("%c", &symbol)) {
        *str = realloc(*str, (i+1) * sizeof(char));
        if (*str != NULL) {
            if (symbol != '\n') {
                (*str)[i++] = symbol;
            }
            else {
                (*str)[i] = '\0';
                break;
            }
        }
        else {
            break;
        }
    }
}

int priority(char op) {
    int priority = -1;

    if (op == '+') {
        priority = 0;
    }
    else if (op == '*' || op == '/'){
        priority = 1;
    }

    return priority;
}

void sub_result (double* numbers, size_t* num_ind, char* operations, size_t* op_ind) {
    if (*num_ind == 0) {
        ++(*num_ind);
    }
    else if (*num_ind == 1) {
        if (priority(operations[*op_ind]) == 1) {
            if (operations[*op_ind] == '*') {
                numbers[0] *= numbers[*num_ind];
            } else if (operations[*op_ind] == '/') {
                numbers[0] /= numbers[*num_ind];
            }
        }
        else {
            ++(*num_ind);
            ++(*op_ind);
        }
    }
    else {
        if (priority(operations[*op_ind]) == 1) {
            if (operations[*op_ind] == '*') {
                numbers[*num_ind - 1] *= numbers[*num_ind];
            } else {
                numbers[*num_ind - 1] /= numbers[*num_ind];
            }

        }
        else {
            numbers[0] += numbers[*num_ind-1];

            numbers[*num_ind-1] = numbers[*num_ind];
            operations[*op_ind-1] = operations[*op_ind];
        }
    }
}

void get_number(char* str, size_t* index, char** str_of_number) {
    size_t i = 0;

    while (str[*index] != '\0' && strchr("0123456789.", str[*index])) {
        *str_of_number = realloc(*str_of_number, (i+1) * sizeof(char));

        if(*str_of_number == NULL) {
            end = BAD_STRING;
        }
        else {
            (*str_of_number)[i++] = str[(*index)++];
        }
    }

    (*str_of_number)[i] = '\0';
}

char read(char* str, size_t* index, char** str_of_number) {
    if (str[*index] == '\0') {
        end = END;
        return '\0';
    }

    while(str[*index] == ' '){
        ++(*index);
    }

    switch(str[*index]) {
        case '(':
        case ')':
        case '+':
        case '-':
        case '*':
        case '/': {
            return str[(*index)++];
        }
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9': {
            get_number(str, index, str_of_number);
            return '0';
        }
        default: {
            end = BAD_STRING;
            return '\0';
        }
    }
}

void minus_operation(char* operations, const size_t op_ind, const size_t num_ind) {
    symbol_state = WAIT_NUMBER;
    if (sign == NEGATIVE) {
        sign = POSITIVE;
    }
    else {
        sign = NEGATIVE;
    }

    if(num_ind != 0 ) {
        operations[op_ind] = '+';
    }
}

void other_operation(const char elem, char* operations, const size_t op_ind) {
    if (symbol_state == OK) {
        symbol_state = WAIT_NUMBER;
        operations[op_ind] = elem;
    }
    else {
        end = BAD_STRING;
    }
}

void operation(const char elem, char* operations, const size_t op_ind, const size_t num_ind) {
    if (elem == '-') {
        minus_operation(operations, op_ind, num_ind);
    }
    else {
        other_operation(elem, operations, op_ind);
    }
}

void make_number(char** str_of_number, double* numbers, size_t* num_ind, char* operations, size_t* op_ind) {
    if (symbol_state == WAIT_NUMBER) {
        symbol_state = OK;

        if (sign == POSITIVE) {
            numbers[*num_ind] = atof(*str_of_number);
        }
        else {
            numbers[*num_ind] = negative(atof(*str_of_number));
            sign = POSITIVE;
        }

        sub_result(numbers, num_ind, operations, op_ind);
        free(*str_of_number);
        *str_of_number = NULL;
    }
    else {
        end = BAD_STRING;
    }
}

void in_brackets(char* str, size_t* index, double* numbers, size_t* num_ind, char* operations, size_t* op_ind) {
    ++num_of_brackets;

    numbers[*num_ind] = calculate(&(str[*index]));
    sub_result(numbers, num_ind, operations, op_ind);

    char * offset = strchr(&(str[*index]), ')');

    if(offset == NULL) {
        end = BAD_STRING;
    }

    if (end == BAD_STRING) {
        return;
    }
    else {
        *index += offset - &(str[*index]);
        str[(*index)++] = ' ';
    }
}

double out_brackets(const double* numbers, const size_t num_ind) {
    --num_of_brackets;

    double result = 0;

    if (num_ind == 1){
        result = numbers[0];
    }
    else {
        result = numbers[0] + numbers[1];
    }

    return result;
}

double calculate(char* str) {
    double result = 0;
    size_t index = 0;

    double numbers[3] = {0,0,0};
    size_t num_ind = 0;

    char operations[2] = {'\0', '\0'};
    size_t op_ind = 0;

    char* str_of_number = NULL;
    char elem;

    while( (elem = read(str, &index, &str_of_number)) != '\0') {
        switch (elem) {
            case '(': {
                in_brackets(str, &index, numbers, &num_ind, operations, &op_ind);

                break;
            }
            case ')': {
                return out_brackets(numbers, num_ind);
            }
            case '-':
            case '+':
            case '*':
            case '/': {
                operation(elem, operations, op_ind, num_ind);

                break;
            }
            default: {
                make_number(&str_of_number, numbers, &num_ind, operations, &op_ind);
            }
        }

        if (end == BAD_STRING) {
            break;
        }
    };

    if (symbol_state == WAIT_NUMBER || num_of_brackets != 0) {
        end = BAD_STRING;
        return 0;
    }

    if (end == BAD_STRING) {
        return 0;
    }
    else if (num_ind == 1){
        result = numbers[0];
    }
    else {
        result = numbers[0] + numbers[1];
    }

    return result;
}
