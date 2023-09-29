#! /bin/sh
./restoration /comp/40/bin/images/corruption/$1-corrupt.pgm > $1.pgm 2> $1.err
valgrind ./restoration /comp/40/bin/images/corruption/$1-corrupt.pgm > $1-val.pgm 2> $1-val.err
# echo "===================input file:==================="
# cat /comp/40/bin/images/corruption/$1-corrupt.pgm
#echo -e "\n===================restoration stdout:==================="
#cat $1.pgm
#echo -e "\n===================valgrind restoration stdout:==================="
#cat val-a.out

echo -e "\n===================restoration stderr:==================="
cat $1.err
echo -e "\n===================valgrind restoration stderr:==================="
cat $1-val.err
rm *.err