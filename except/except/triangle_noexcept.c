/*                    triangle_noexcept.c
 *
 *         Author: Noah Mendelsohn
 *
 *    Shows the creation and use of a function third_side_or_error, which computes
 *    the second side of a right triangle given the hypotenuse and one side.
 *
 *    The complication is that bad input data may specify a triangle that
 *    has a hypotenuse that's shorter than it's side, which is impossible.
 *    To illustrate how clumsy this can be when exceptions aren't used, the
 *    function also sets a caller-supplied "isbad" error signal.
 *
 *    This source is to be compared with the similar triangle_except and
 *    triangle_except2, which make the code much simpler.
 *
 *    NOTES: The isbad variables illustrate another technique that's 
 *           important to learn for HW1: using pointer-type arguments
 *           to let a called function set a variable owned by the caller.
 */

#include <stdio.h>
#include <math.h>
#include <stdbool.h>

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


/* 
 *                  third_side
 *
 *    Tests for incorrect input, sets isbad, and if data is good computes 
 *    and returns the size of the other side of the triangle.
 */

float
third_side(float s1, float hyp, bool *isbad_p)
{
        /* If hypotenuse is shorter than side, this is not a legal triangle */
        if (hyp < s1) {
                /* Note use of pointer to set variable identified by caller */
                *isbad_p = true;
                return 0;        
        } else {
                *isbad_p = false;
                return sqrt(hyp * hyp - s1 * s1);
        }
}

/*
 *                  print_triangle
 *
 *       Print all sides of a triangle.
 */
void
print_triangle(float s1, float hyp)
{
        float s2;
        bool isbad;

        /* Attempt to compute the size of the other side */
        /* note that following statement will set both s2 and isbad */
        s2 = third_side(s1, hyp, &isbad);

        /* Print result or error message */
        if (!isbad) {
                printf("Triangle s1=%.1f s2=%.1f hyp=%.1f \n",
                       s1, s2, hyp);
        } else {
                printf("Could not compute side for triangle: "
                       "s1 = %.1f; hyp = %.1f\n",
                       s1, hyp);
        }

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
