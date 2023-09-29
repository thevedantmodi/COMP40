/*                    triangle_except.c
 *
 *         Author: Noah Mendelsohn
 *
 *    Shows the creation and use of a function third_side, which computes
 *    the second side of a right triangle given the hypotenuse and one side.
 *
 *    This version uses Hanson exceptions to deal with illegal triangles.
 *    
 *    Note that the exception is uncaught, so the first bad triangle
 *    results in the program terminating with the Hanson exception.
 */

#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <except.h>

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/*                 EXCEPTION DEFINITION(S)               */
/*                                                       */
/*     Note that Hanson Exceptions can be RAISE'd to     */
/*     signal an error.                                  */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - */

Except_T Bad_Triangle = { "Triangle is bad" };

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/*                 DATA AND DECLARATIONS                 */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - */

/*
 *         Structure to hold the hypotenuse and one side of a triangle
 */
struct hype_and_side {
        float s1;
        float hyp;
};

/* 
 *        TEST DATA
 *
 *   Three triangles: the middle one is bad.
 */
const struct hype_and_side  TEST_DATA[] = {
        {.s1 = 3, .hyp = 5},
        {.s1 = 5, .hyp = 3},
        {.s1 = 5, .hyp = 13}
};

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/*                       FUNCTIONS                       */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - */

float
third_side(float s1, float hyp)
{
        if (hyp < s1) {
                RAISE(Bad_Triangle);
        }
        return sqrt(hyp * hyp - s1 * s1);
}


/*
 *                  print_triangle
 *
 *       Print all sides of a triangle. Presumes
 *       legal values for the data.
 */
void
print_triangle(float s1, float hyp)
{
        printf("Triangle s1=%.1f s2=%.1f hyp=%.1f \n",
               s1, third_side(s1, hyp), hyp);
}


/* - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/*                         MAIN                          */
/*                                                       */
/*    Call print_triangle on each triangle in test data  */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - */

int
main(int argc, char *argv[])
{
        (void) argc;
        (void) argv;

        unsigned int i;

        for (i = 0; i < sizeof(TEST_DATA) / sizeof(struct hype_and_side); i++) {
                print_triangle(TEST_DATA[i].s1, TEST_DATA[i].hyp);
        }       
        return 0;
}      
