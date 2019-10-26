cp a.out tests/a.out

cd ./tests

for test in `ls . | grep 'in' | cut -d'_' -f1`;
do
	./a.out < ${test}_in.txt > ${test}_out_my.txt
	diff ${test}_out.txt ${test}_out_my.txt
	if [ $? -eq 0 ]
	then
		echo "Test: ${test} OK"
		rm ${test}_out_my.txt
	else
		echo "Test: ${test} BAD"
	fi;
done

rm a.out

echo "-------------"
echo "Done"
