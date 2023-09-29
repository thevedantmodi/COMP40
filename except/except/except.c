/*                    except.c
 *
 *         Author: Noah Mendelsohn
 *
 *    Demonstration of Hanson's exceptions
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>    /* islower is defined here */
#include <except.h>

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/*                 EXCEPTION(S)                          */
/*                                                       */
/*     Note that Hanson Exceptions can be RAISE'd to     */
/*     signal an error.                                  */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - */

Except_T Wrong_Arg_Count = { "No arguments or too many arguments provided" };
Except_T Not_LowerCase = { "Argument not lowercase" };
Except_T Not_Four_Chars = { "Argument is not 4 chars long" };


/* 
 *            is_lower_string
 *
 *    Return 1 or 0 according to whether string is or is not lowercase
 */

int
is_lower_string(char *s)
{
        while (islower(*s))
                s++;
        if (*s == '\0')
                return 1;
        else
                return 0;
}

/*
 *           checker
 *
 *     Check input and raise exceptions as appropriate.
 */
char
checker(int argc, char *argv[])
{
        int retval = 0;

        /* Make sure there is one argument (plus command name) */
        if (argc != 2) {
                RAISE(Wrong_Arg_Count);
        }
        printf("calling is_lower_string %s\n", argv[1]);
        if (!is_lower_string(argv[1])) {
                RAISE(Not_LowerCase);
        }
        
        printf("Done with islower\n");

        if (strlen(argv[1]) != 4) {
                RAISE(Not_Four_Chars);
        }
                
        printf("inner dropped through END_TRY, returning %d\n", retval);

        return argv[1][0];   /*return first letter of first argument */
}

/* 
 *              middle
 *
 *      Intermediate function, calls checker, handles Not_Lowercase
 *      but not other exceptions.
 */

char
middle(int argc, char *argv[])
{
        volatile int retval = 0;

        printf("Started middle\n");
        TRY
                printf("middle:in TRY block -- calling checker\n");
                retval = checker(argc, argv);
                printf("middle: returned from checker\n");
                
        EXCEPT(Not_LowerCase);
                printf("middle: in EXCEPT handler for Not_Lowercase\n");
        END_TRY;        

        printf("middle dropped through END_TRY, returning %d\n", retval);

        return retval;
}

/* 
 *              outer
 *
 *      Outer function, calls middle, handles Wrong_Arg_Count
 *      but not other exceptions.
 */

char
outer(int argc, char *argv[])
{
        volatile int retval = 0;

        printf("Started outer\n");
        TRY
                printf("outer is in TRY block -- calling middle\n");
                retval = middle(argc, argv);
                printf("outer returned from middle\n");
                
        EXCEPT(Wrong_Arg_Count);
                printf("outer: in EXCEPT handler for Wrong_Arg_Count\n");
        END_TRY;

        printf("outer dropped through END_TRY, returning %d\n", retval);

        return retval;
}

/* 
 *            main
 */
int
main(int argc, char *argv[])
{
        outer(argc, argv);
}      
