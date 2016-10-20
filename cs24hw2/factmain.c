/* call fact.s and print the result of the factorial */ 
#include <stdio.h>
#include <stdlib.h>
#include "fact.h"

int main(int argc, char const *argv[])
{
    // use the argument as the factorial input
    int n = -1;
    int input = atoi(argv[1]); // convert to integer
    // Check error case before running
    if (argc > 2) {
        printf("Error: There is more than one command-line argument \n");
    } else if (input < 0) {
        printf("Error: The numeric argument is negative \n");
    } else if (input >= 0){
        n = fact(input);
        printf("fact: %d\n", n);
    }
    return 0;
}