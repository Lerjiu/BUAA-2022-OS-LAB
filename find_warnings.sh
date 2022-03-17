#!bin/bash
gcc -Wall $1 -o test 2> warning.txt 
grep warning: warning.txt | sed -n 's/warning: //g' >> result.txt
gcc -Wall $1 -o test 2> warning.txt
a=1
if [ $? -eq 0 ]
then
while [ $a -le $2 ]
do
echo $a | ./test >> result.txt
a=$[$a+1]
done
fi
pwd >> result.txt
