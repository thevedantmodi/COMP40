/*                    triangle_assert.c
 *
 *         Author: Noah Mendelsohn
 *
 *    Shows the creation and use of a function third_side, which computes
 *    the second side of a right triangle given the hypotenuse and one side.
 *
 *    This version uses Hanson asserts to deal with illegal triangles.
 *    (see function "third_side").
 *    
 *    As in triangle_assert the exception is uncaught, so the first bad triangle
 *    results in the program terminating with the Hanson exception.
 *    Indeed, it would be quite unusual, though possible, to catch 
 *    Assertion exceptions; they are intended to be used to signal fatal
 *    errors. (So, it would be inappropriate to use ASSERT to build
 *    something similar to triangle_except2.c, which keeps going
 *    after encountering a bad triangle.
 *
 *    NOTE: we no longer define an Except_T: that's built in for assertions.
 *
 */

#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <assert.h>


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
        assert(s1 < hyp);
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
        printf("Triangle s1 = %.1f; s2 = %.1f; hyp = %.1f \n",
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
