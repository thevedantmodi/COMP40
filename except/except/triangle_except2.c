/*                    triangle_except2.c
 *
 *         Author: Noah Mendelsohn
 *
 *    Shows the creation and use of a function third_side, which computes
 *    the second side of a right triangle given the hypotenuse and one side.
 *
 *    This version uses Hanson exceptions to deal with illegal triangles.
 *    
 *    TRY/EXCEPT is used to "catch" bad triangle exceptions, so 
 *    we can keep going on to other test cases.
 */

#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <except.h>

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/*                 EXCEPTION(S)                          */
/*                                                       */
/*     Note that Hanson Exceptions can be RAISE'd to     */
/*     signal an error.                                  */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - */

/*
 * Define three exceptions
 */

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
 *
 *       Note that the call to third_side that's made as an argument
 *       to printf can RAISE Bad_Triangle, which is caught by the
 *       EXCEPT block.
 */
void
print_triangle(float s1, float hyp)
{
        TRY
                printf("Triangle s1=%.1f s2=%.1f hyp=%.1f \n",
                       s1, third_side(s1, hyp), hyp);
        EXCEPT(Bad_Triangle);
                printf("Could not compute side for triangle:  "
                       "s1 = %.1f; hyp = %.1f\n",
                       s1, hyp);
        END_TRY;
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
