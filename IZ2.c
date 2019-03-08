#include <stdio.h>
#include "stdlib.h"
#include "string.h"

enum {END, BAD_STRING} end;
//enum {GOOD_BRACKETS, WAIT_BRACKET} brackets_state = GOOD_BRACKETS;
size_t  num_of_brakets = 0;
enum {OK, WAIT_NUMBER} symbol_state = WAIT_NUMBER;
enum {POSITIVE, NEGATIVE} sign = POSITIVE;

char str_of_number[80] = "";


int priority(char op) {

    if (op == '+') {
        return 0;
    }
    else {
        return 1;
    }
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
            if (operations[*op_ind])
                numbers[0] += numbers[*num_ind-1];

            numbers[*num_ind-1] = numbers[*num_ind];
            operations[*op_ind-1] = operations[*op_ind];
        }
    }
}

void get_number(char* str, size_t* index) {
    size_t i = 0;

    while (str[*index] != '\0' && strchr("0123456789.", str[*index])) {
        str_of_number[i++] = str[(*index)++];
    }

    str_of_number[i] = '\0';
}

char read(char* str, size_t* index) {
    if (str[*index] == '\0') {
        end = END;
        return '\0';
    }

    while(str[*index] == ' '){
        ++(*index);
    } //skip spaces

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
            get_number(str, index);
            return '0';
        }
        default: {
            end = BAD_STRING;
            return '\0';
        }
    }
}

double calculate(char* str) {
    double result = 0;
    size_t index = 0;

    double numbers[3] = {0,0,0};
    size_t num_ind = 0;

    char operations[2] = {'\0', '\0'};
    size_t op_ind = 0;

    char elem;

    while( (elem = read(str, &index)) != '\0') {
        switch (elem) {
            case '(': {
                //if (symbol_state == WAIT_NUMBER) {
                //    brackets_state = WAIT_BRACKET;
                ++num_of_brakets;

                    numbers[num_ind] = calculate(&(str[index]));
                    sub_result(numbers, &num_ind, operations, &op_ind);

                    char * offset = strchr(&(str[index]), ')');

                    if (end == BAD_STRING || offset == NULL) {
                        return 0;
                    }
                    else {
                        index += offset - &(str[index]);
                        str[index++] = ' ';
                    }
                //}
                //else {
                //    end = BAD_STRING;
                //    return 0 ;
                //}


                break;
            }
            case ')': {
                /*if (brackets_state == WAIT_BRACKET) {
                    brackets_state = GOOD_BRACKETS;
                }
                else {
                    end = BAD_STRING;
                    return 0;
                }*/
                --num_of_brakets;

                if (num_ind == 1){
                    result = numbers[0];
                }
                else {
                    result = numbers[0] + numbers[1];
                }

                return result;
            }
            case '-': {
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

                break;
            }
            case '+':
            case '*':
            case '/': {
                if (symbol_state == OK) {
                    symbol_state = WAIT_NUMBER;
                }
                else {
                    end = BAD_STRING;
                    return 0;
                }

                operations[op_ind] = elem;

                break;
            }
            default: {
                if (symbol_state == WAIT_NUMBER) {
                    symbol_state = OK;
                }
                else {
                    end = BAD_STRING;
                    return 0;
                }

                if (sign == POSITIVE) {
                    numbers[num_ind] = atof(str_of_number);
                }
                else {
                    numbers[num_ind] = -1 * atof(str_of_number);
                    sign = POSITIVE;
                }

                sub_result(numbers, &num_ind, operations, &op_ind);
            }
        }
    };

    if (symbol_state == WAIT_NUMBER || num_of_brakets != 0) {
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

int main() {
    char buf[1024];
    gets(buf);
    double result = calculate(buf);

    if (end == BAD_STRING) {
        printf("[error]");
    }
    else {
        printf("%.2f", result);
    }

    return 0;
}
