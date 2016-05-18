if [ $# -ne 1 ] ; then
	echo "Usage: $0 file"
fi

root=$(dirname $(pwd)/$0)

awk '{print "\"" $0 "\"" ","}' $1 > $root/.test.vector.h
awk '
BEGIN {c=256}
/^;assert/ {print "case " c++ ":" "printf(\"" $0 ";\\n\")" $0 ";break;"}
' $1 > $root/.test.int.h

cd $root
g++ -std=c++11 -o .tests $root/test_template.cpp
if [ $? -eq 0 ] ; then
	./.tests
fi
