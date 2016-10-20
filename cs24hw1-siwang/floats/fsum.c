#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include "ffunc.h"


/* This function takes an array of single-precision floating point values,
 * and computes a sum in the order of the inputs.  Very simple.
 */
float fsum(FloatArray *floats) {
    float sum = 0;
    int i;

    for (i = 0; i < floats->count; i++)
        sum += floats->values[i];

    return sum;
}


/* TODO:  IMPLEMENT my_fsum() HERE, AND DESCRIBE YOUR APPROACH. */
float my_fsum(FloatArray *floats) {
    /* a compensation term is included into summation
    to account for loss of precision */
    float sum = 0, nextSum = 0, comp = 0; 
    for (int i = 0; i < floats->count; i++) {
        nextSum += floats->values[i]; // adding will result in inaccuracy
        /* nextSum - sums will find the resulting number that was added 
        for the resulting sum. ex: 
            1 + 1.00000000001 = 2, so nextSum = 2, sum = 1, and nextSum - sums = 1
            comp would be 1 - 1.00000000001 = -0.00000000001
        comp will find the compensation term that needs to be subtracted from
        the final summation to find the most accurate result
        */
        comp += (nextSum - sum) - floats->values[i];
        sum = nextSum;
    }
    sum -= comp; // add the compensation to the sum 
    return sum;
}


int main() {
    FloatArray floats;
    float sum1, sum2, sum3, my_sum;

    load_floats(stdin, &floats);
    printf("Loaded %d floats from stdin.\n", floats.count);

    /* Compute a sum, in the order of input. */
    sum1 = fsum(&floats);

    /* Use my_fsum() to compute a sum of the values.  Ideally, your
     * summation function won't be affected by the order of the input floats.
     */
    my_sum = my_fsum(&floats);

    /* Compute a sum, in order of increasing magnitude. */
    sort_incmag(&floats);
    sum2 = fsum(&floats);

    /* Compute a sum, in order of decreasing magnitude. */
    sort_decmag(&floats);
    sum3 = fsum(&floats);

    /* %e prints the floating-point value in full precision,
     * using scientific notation.
     */
    printf("Sum computed in order of input:  %e\n", sum1);
    printf("Sum computed in order of increasing magnitude:  %e\n", sum2);
    printf("Sum computed in order of decreasing magnitude:  %e\n", sum3);

    
    printf("My sum:  %e\n", my_sum);

    return 0;
}

