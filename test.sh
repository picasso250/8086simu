awk '{print "\"" $0 "\"" ","}' $1 > .test.vector.h
echo
awk '
BEGIN {c=256}
/^;assert/ {print "case " c++ ":" $0 ";break;"}
' $1 > .test.int.h
echo

g++ -std=c++11 -o .tests te
