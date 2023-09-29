# 	         Makefile for COMP 40 Exceptions Demonstration
#
#     Author: Noah Mendelsohn (adapted from Norman Ramsey's compile script)
#
#  Maintenance targets:
#
#
#    all         - (default target) make sure everything's compiled
#    clean       - clean out all compiled object and executable files
#    except      - the exceptions demonstration module
#

# Executables to built using "make all"

EXECUTABLES = except triangle_noexcept triangle_except triangle_except2 \
	triangle_assert file_read

# testables are pseudo targets for test cases

TESTABLES = except.testable triangle_noexcept.testable \
        triangle_except.testable triangle_except2.testable \
	triangle_assert.testable file_read.testable

#
#  The following is a compromise. You MUST list all your .h files here.
#  If any .h file changes, all .c files will be recompiled. To do better,
#  we could make an explicit target for each .o, naming only the .h
#  files it really uses.
#
# Add your own .h files to the right side of the assingment below.

INCLUDES = 

# Do all C compies with gcc (at home you could try clang)
GCC = gcc

# Comp 40 directory

COMP40 = /comp/40

# the next two lines enable you to compile and link against CII40
CIIFLAGS = `pkg-config --cflags cii40`
CIILIBS = `pkg-config --libs cii40`

# the next three lines enable you to compile and link against course software
CFLAGS = -I. -I$(COMP40)/include $(CIIFLAGS)
LIBS = $(CIILIBS) -lm    
LFLAGS = -L$(COMP40)/lib64

# these flags max out warnings and debug info
FLAGS = -g -O -Wall -Wextra -Wfatal-errors -std=c99 -pedantic

# 
#    'make all' will build all executables
#
#    Note that "all" is the default target that make will build
#    if nothing is specifically requested
#
all: $(EXECUTABLES)

#
#     'make test' runs all test cases
#
test: $(TESTABLES)

# 
#    'make clean' will remove all object and executable files
#
clean:
	rm -f $(EXECUTABLES) *.o


# 
#    To get any .o, compile the corresponding .c
#
%.o:%.c $(INCLUDES)
	$(GCC) $(FLAGS) $(CFLAGS) -c $<

# 
#    To get any testable, you need to test the corresponding exe
#
%.testable:%
	./testit.sh $<
	rm -f core.*

#
# Individual executables
#
#    Each executable depends on one or more .o files.
#    Those .o files are linked together to build the corresponding
#    executable.
#

except: except.o
	$(GCC) $(FLAGS) $(LFLAGS) -o except  except.o $(LIBS)

triangle_noexcept: triangle_noexcept.o
	$(GCC) $(FLAGS) $(LFLAGS) -o triangle_noexcept  triangle_noexcept.o $(LIBS)

triangle_except: triangle_except.o
	$(GCC) $(FLAGS) $(LFLAGS) -o triangle_except  triangle_except.o $(LIBS)

triangle_except2: triangle_except2.o
	$(GCC) $(FLAGS) $(LFLAGS) -o triangle_except2  triangle_except2.o $(LIBS)

triangle_assert: triangle_assert.o
	$(GCC) $(FLAGS) $(LFLAGS) -o triangle_assert  triangle_assert.o $(LIBS)

file_read: file_read.o
	$(GCC) $(FLAGS) $(LFLAGS) -o file_read  file_read.o $(LIBS)

echo:
	echo "$(CIIFLAGS)"