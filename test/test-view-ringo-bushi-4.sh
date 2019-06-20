#!/bin/bash
set -eu

#Define tab viewer ASCII output file and correct expected output file
SHAMITAB='../shamitab view'
TAB='ringo-bushi-4.3mt'
OUTPUT='output-tab.txt'
CORRECT='ringo-bushi-4.txt'

#Check that both files exist
if [ ! -f $TAB ]; then
  >&2 echo "ERROR: tab file $TAB not found!";
  exit 1;
fi
if [ ! -f $CORRECT ]; then
  >&2 echo "ERROR: correction file $CORRECT not found!";
  exit 1;
fi

#Call Shamitab on the tab
$SHAMITAB $TAB > $OUTPUT

#Compare files
if diff --brief $OUTPUT $CORRECT > /dev/null;
  then echo "PASS"
  else echo "FAIL"
fi

rm $OUTPUT
