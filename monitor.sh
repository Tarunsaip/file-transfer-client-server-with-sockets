#!/bin/bash

let level=0

function myFunc () 
{
	level=$2
	child_count=$(($(pgrep --parent $1 | awk '{print $1;}' | wc -l)))
	if [ child_count > 0 ]; then
		touch error_details.txt
		for p in $(pgrep --parent $1)
		do
			let level++
			echo $p     $1      $level >> error_details.txt
			myFunc $p $level
		done
	fi
}

echo Process information at $(date -u) 
for p in $(ps -af | awk 'NR>1 {print  $1 "-" $2 "-" $3 "-" $4 "-" $5 "-" $6 "-" $7 "-" $8}')
do
	echo $p
done
echo -----------------------------------------------------------


for pid in $(ps -af -o pid | awk 'NR>1 {print $1}')
do
	touch error_details.txt
	echo                             >> error_details.txt
	echo $(date -u) >> error_details.txt         
	echo --------------------------- >> error_details.txti
	echo parent: $pid  >> error_details.txt
	echo child  -  parent  -  level  >> error_details.txt
	echo --------------------------- >> error_details.txt
	myFunc $pid $level
	if [ $level -gt 6 ]; then
		touch file1.txt
		cat error_details.txt >> file1.txt
		cat error_details.txt 
	fi
	level=0
	rm -r error_details.txt
done
if [ $(cat file1.txt | wc -l) -gt 0 ]; then
	mail -s "!!ALERT!! SIX DESCENDANT PROCESSES DETECTED" polina@uwindsor.ca < file1.txt
	rm -r file1.txt
fi
