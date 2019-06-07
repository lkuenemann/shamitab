#!/bin/bash
set -x

RESOUT='TEST_SUMMARY.txt'

echo "Executed tests summary:" > $RESOUT

for testfile in ./test-*.sh; do
	#printf "$testfile :	" >> $RESOUT
	$testfile >> $RESOUT
done
