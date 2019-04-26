/* Main file of the Movie tree program.
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#include "list_struct.h"
#include "tree_struct.h"

#define MIN_ID 0
#define MIN_RATE 0
#define PLUS_SIZE 1
#define EXTENSION 10
#define MAX_ID 65535
#define MAX_MULT 100000
#define MAX_RATE 2147483647
#define MAX_MULT_RATE 100000000000

typedef struct user User;

char * resize (char *str, size_t size) {

    return realloc(str, (size + EXTENSION) * sizeof(char));

    // We are checking if the memory reallocation was successful.

    if (!str) {
        exit(EXIT_FAILURE);
    }
}

// The two functions that are used to check if the given numbers are in their range.

bool check_ID (int number) {

    if (number >= MIN_ID && number <= MAX_ID)
        return true;
    return false;
}

bool check_RATE (long long number) {
    
    if (number >= MIN_RATE && number <= MAX_RATE)
        return true;
    return false;
}

// This function executes the methods if the numbers are within the range.

void num_check (int num1, int num2, long long num1_rate, int status) {

    switch (status) {
        case 1:
            if (check_ID(num1) && check_ID(num2)) {
                addUser(num2, num1);
            } else {
                fprintf(stderr, "ERROR\n");
            }
           break;
        case 2:
            if (check_ID(num1)) {
                delUser(num1);
            } else {
                fprintf(stderr, "ERROR\n");
            }
             break;
        case 3:
            if (check_ID(num2) && check_RATE(num1_rate)) {
                addMovie(num2, num1_rate);
            } else {
                fprintf(stderr, "ERROR\n");
            }
             break;
        case 4:
            if (check_ID(num2) && check_RATE(num1_rate)) {
                delMovie(num2, num1_rate);
            } else {
                fprintf(stderr, "ERROR\n");
            }
             break;
        default:
            if (check_ID(num2) && check_RATE(num1_rate)) {
                marathon(num2, num1_rate);
            } else {
                fprintf(stderr, "ERROR\n");
            }
    }
}

void command_check (const char *str, size_t size) {

    size_t current_digit = 0;
    long long multiplier = 1;
    int num1 = 0, num2 = 0;

    long long num1_rate = 0;

    // We are checking if the current line is a comment or an empty line.

    if (str[current_digit] == '#' || str[current_digit] == '\0') {

        return;
    }

   // We are looking for keyword.

    int status = 0;
    size_t keyword_size = 0;
    /*
        This is the list of status for different keywords.

        addUser 1
        delUser 2
        addMovie 3
        delMovie 4
        marathon 5

    */

    if (!strncmp("addUser", str, strlen("addUser"))) {
       status = 1;
       keyword_size = strlen("addUser");
    } else if (!strncmp("delUser", str, strlen("delUser"))) {
       status = 2;
       keyword_size = strlen("delUser");
    } else if (!strncmp("addMovie", str, strlen("addMovie"))) {
       status = 3;
       keyword_size = strlen("addMovie");
    } else if (!strncmp("delMovie", str, strlen("delMovie"))) {
       status = 4;
       keyword_size = strlen("delMovie");
    } else if (!strncmp("marathon", str, strlen("marathon"))) {
       status = 5;
       keyword_size = strlen("marathon");
    }
    
    // two variables used to check if the numbers are given

    bool isNum1 = false, isNum2 = false;
    

    // In this 'if' statement we are checking for valid numbers in the given string. 
    if (status) {

        if (isdigit(str[size - PLUS_SIZE])) {

            current_digit = size - PLUS_SIZE;   

            if (status == 1 || status == 2) {

                while (isdigit(str[current_digit])) {

                    isNum1 = true;
                    num1 += ((str[current_digit--] - 48) * multiplier);
                    multiplier *= 10;

                    if (multiplier > MAX_MULT) {

                        isNum1 = false;
                        break;
                    }
                }
            } else {
                
                 while (isdigit(str[current_digit])) {

                    isNum1 = true;
                    num1_rate += ((str[current_digit--] - 48) * multiplier);
                    multiplier *= 10;

                    if (multiplier > MAX_MULT_RATE) {

                        isNum1 = false;
                        break;
                    }
                }
            }

            if (str[current_digit + 1] == '0' && multiplier > 10) {

                isNum1 = false;
            }

            multiplier = 1;

            if (status != 2) {

                if (str[current_digit--] == ' ') {
                    
                    while (isdigit(str[current_digit])) {

                        isNum2 = true;
                        num2 += ((str[current_digit--] - 48) * multiplier);
                        multiplier *= 10;

                        if (multiplier > MAX_MULT) {

                            isNum2 = false;
                            break;
                        }
                    }

                    if (str[current_digit + 1] == '0' && multiplier > 10) {

                        isNum2 = false;
                    }

                }

                if (str[current_digit] == ' ' && current_digit == keyword_size && isNum1 && isNum2) {

                    num_check(num1, num2,num1_rate, status);
                } else {
                    fprintf(stderr, "ERROR\n");
                }
            } else {
                
                if (str[current_digit] == ' ' && current_digit == keyword_size && isNum1) {

                    num_check(num1, num2, num1_rate, status);
                } else {
                    fprintf(stderr, "ERROR\n");
                }
            }
        } else {

            fprintf(stderr, "ERROR\n");
        }
    }
    // If the status is equal to zero it means that the line is incorrect.
    else {

        fprintf(stderr, "ERROR\n");
    }
}

// readLine reads a single line.

const char * readLine (size_t *str_size) {

    int c;
    char *str = malloc((EXTENSION) * sizeof(char));

    // We are checking if the memory allocation was successful.

    if (!str) {
        exit(EXIT_FAILURE);
    }

    size_t size = EXTENSION;
    size_t actual_size = 0;

    while ((c = getchar()) != '\n') {

        if (size == actual_size) {
            str = resize(str, size);
            size += EXTENSION;
        }
        str[actual_size++] = c;
    }

    if (size == actual_size) {
            str = resize(str, size);
            size += EXTENSION;
    }

    str[actual_size] = '\0';
    *str_size = actual_size;

    return str;
}

// In this function we read the input line by line.

void inputRead (void) {

    int c;

    while (true) {

        c = getchar();
        if (c == EOF)
            break;
        else
            ungetc(c, stdin);

        size_t size = 0;
        const char *line = readLine(&size);
        command_check(line, size);

        free((char*)line);
    }
}

int main (void) {

    alloc_tree();

    inputRead();    

    clear_data();

    return 0;
}