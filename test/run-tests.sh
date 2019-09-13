#!/bin/bash
set -eu

#How to call shamitab
SHAMITAB='../shamitab'
#List of all testing tabs (no file extension)
TABLIST='all-effects all-fingers all-positions all-special triplets maebachi ringo-bushi-1 ringo-bushi-4 ringo-bushi-7 ringo-bushi-1to11'
#Temporary output for testing
OUTPUT='tmp_out'
DATE=`date '+%Y-%m-%d-%H-%M-%S'`
echo $DATE
OUT_FILE_NAME='TEST_SUMMARY'
SUMMARY=$DATE$OUT_FILE_NAME

#Variables
PASS=0
TOTAL=0

echo "Executed tests summary:" > $SUMMARY

#For each tab in the list
for tab in $TABLIST ; do

	#Check that both the 3mt and txt tab files exist
	if [ ! -f $tab.3mt ]; then
		>&2 echo "ERROR: tab file $tab.3mt not found!";
		exit 1;
	fi
	if [ ! -f $tab.txt ]; then
		>&2 echo "ERROR: tab file $tab.txt not found!";
		exit 1;
	fi

	#Viewing test

	#Call Shamitab on the 3mt tab
	$SHAMITAB view $tab.3mt 4 > $OUTPUT
	TOTAL=$((TOTAL+1))

	#Compare to example txt tab
	if diff --brief $OUTPUT $tab.txt > /dev/null;
		then echo "PASS	view $tab" >> $SUMMARY; PASS=$((PASS+1))
		else echo "FAIL	view $tab" >> $SUMMARY
	fi

	#Conversion test

	#Call Shamitab on the txt tab
	$SHAMITAB convert $tab.txt $OUTPUT
	TOTAL=$((TOTAL+1))

	#Compare to example 3mt tab
	if diff --brief $OUTPUT $tab.3mt > /dev/null;
		then echo "PASS	convert $tab" >> $SUMMARY; PASS=$((PASS+1))
		else echo "FAIL	convert $tab" >> $SUMMARY
	fi

	#Full-circle (conversion then view) test

	#Call shamitab on the txt tab
	$SHAMITAB convert $tab.txt tmp_intermediate
	#Call Shamitab on the 3mt tab
	$SHAMITAB view tmp_intermediate 4 > $OUTPUT
	TOTAL=$((TOTAL+1))

	#Compare to example txt tab
	if diff --brief $OUTPUT $tab.txt > /dev/null;
		then echo "PASS	convert+view $tab" >> $SUMMARY; PASS=$((PASS+1))
		else echo "FAIL	convert+view $tab" >> $SUMMARY
	fi

	#Clean-up
	rm tmp_intermediate
	rm $OUTPUT

done

echo "$PASS/$TOTAL tests passed" >> $SUMMARY
cat $SUMMARY

