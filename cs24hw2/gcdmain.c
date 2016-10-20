/* call gcd.s and print the result of the greatest common denominator */ 
#include <stdio.h>
#include <stdlib.h>
#include "gcd.h"

int main(int argc, char const *argv[])
{
    // use the arguments as the GCD inputs
    int n = -1;
    int in1 = atoi(argv[1]); // convert to integer
    int in2 = atoi(argv[2]); 
    // swap arguments, make sure in1  > in2
    if (in1 < in2) {
        int temp = in1;
        in1 = in2;
        in2 = temp;
    }
    // Check error case before running
    if (argc > 3) {
        printf("Error: There are more than two command-line arguments \n");
    } else if (in1 < 0 || in2 < 0) {
        printf("Error: The numeric argument(s) are negative \n");
    } else if (in1 >= 0){
        n = gcd(in1, in2);
        printf("Greatest Common Denominator: %d\n", n);
    }
    return 0;
}