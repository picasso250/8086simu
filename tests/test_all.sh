root=$(dirname $(pwd)/$0)
cd $root
for file in $(find $root -iname "*.asm")
do
	echo $file
	bash test_file.sh $file
	echo $?
done
