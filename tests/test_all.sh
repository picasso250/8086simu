root=$(dirname $(pwd)/$0)
cd $root
for file in $(find $root -iname "*.asm")
do
	echo $file
	bash test_file.sh $file
	if [ $? -ne 0 ] ; then
		echo test fail
		exit 1
	fi
done
