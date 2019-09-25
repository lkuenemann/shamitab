# Shamitab

Author:
Luc Kuenemann

## Introduction

Shamitab is a command line software for displaying shamisen tablature in the
eponym format (shamitab, short: 3mt) in ASCII form in a terminal, and converting
ASCII shamisen tablatures to the shamitab/3mt format.

## Compiling

To compile:

	make

To clean everything:

	make clean

## Running

To view a 3mt tablature:

	./shamitab view [path-to-tab.3mt] [bars per line]
	./shamitab v [path-to-tab.3mt] [bars per line]

To convert an ASCII text tablature to 3mt format:

	./shamitab convert [path-to-tab.txt] [path-to-converted-tab.3mt]
	./shamitab c [path-to-tab.txt] [path-to-converted-tab.3mt]

To display a list of all available commands:

	./shamitab help
	./shamitab h

## Todo

* Add test for all supported durations display
* Add test for effects collision avoidance (need to define new way to display all effects)
* Better slide display (can collide with other effects)
* Better suberi display (can collide with other effects)
* Better effects display (can collide with other effects)
* Better finger display (can collide with other effects)
* Better maebachi display (can collide with other effects)
* Better triplets display (can collide with duration display on first string)
* Interactive tab edition mode
