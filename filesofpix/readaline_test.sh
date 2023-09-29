#! /bin/sh
make readaline_test
./readaline_test $1 > $1.out
echo "running diff"
diff $1 $1.out
rm *.out