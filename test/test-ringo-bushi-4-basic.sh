#!/bin/bash
set -eu

#Define tab viewer ASCII output file and correct expected output file
OUTPUT='../output-tab.txt'
CORRECT='ringo-bushi-4-basic.txt'

#Check that both files exist
if [ ! -f $OUTPUT ]; then
  >&2 echo "ERROR: output file $OUTPUT not found!";
  exit 1;
fi
if [ ! -f $CORRECT ]; then
  >&2 echo "ERROR: correction file $CORRECT not found!";
  exit 1;
fi

#Compare files
if [ "diff --brief $OUTPUT $CORRECT" != "" ];
  then echo "FAIL"
  else echo "PASS"
fi
