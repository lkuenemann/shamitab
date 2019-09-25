# Shamitab "3mt" format specification

Luc Kuenemann (kuenemann.luc@protonmail.com)


## Introduction

The shamitab format is an attempt to define a shamisen tablature encoding
format. The tablature is encoded in terms of "symbols", where one symbol is a
single element (bar, silence, single note, chord, etc.) of the tablature.
Symbols are atomic units of the shamitab format, encoding all the possible
properties a shamisen tablature element could hold on 32 bits.

In the shamitab format, all symbols on the tablature are encoded one by one in
order, from first to last. A shamitab file begins with a magic number, and ends
with an end of file (EOF) marker (see file markers section).

The shamitab format makes a distinction between normal symbols (notes and
chords), and special symbols (silence, bar, repeat symbol). The encoding is
defined as follows, with the 32 bits broken down into 9 components:

	0000 0000 0000 0000 0000 0000 0000 0000
	AAAB CDDD EFFF PPGH HHHH GHHH HHGH HHHH

* A codes the duration of the symbol
* B codes the presence of a triplet
* C codes the presence of a slide
* D codes the effect type
* E codes the mae bachi play style
* F codes the finger annotation
* P is some padding (2 unused bits set to 0)
* G codes the presence of a note on associated string
* H codes the position of this note on associated string

G and H are repeated 3 times, one for each string of the shamisen. They are, in
order, G1 and H1 for the 1st string (ichi no ito, the lowest string), G2 and H2
for the 2nd string (ni no ito, the middle string), and G3 and H3 for the 3rd
string (san no ito, the highest string).

Each G indicates that a note is played on its associated string. A single G
being positive would mean a single note, multiple Gs being positive would mean a
chord, and no G being positive would mean no note is played. This latter case is
used to encode special symbols.


## Special symbols

Symbols are special symbols when G1, G2, and G3 all together hold the value 0.

The defined special symbols are:
* Silence
* Bar
* Double bar
* Left repeat (start of a repeat section)
* Right repeat (end of a repeat section)

Special symbols are coded by D in the following manner:

D value	| Symbol
-------	| ------
000		| Silence
001		| Bar
010		| Double bar
011		| Left repeat
100		| Right repeat
Other	| Undefined

By convention, all other bits should be set to 0 with the exception of A for a
silence, where it should be encoding its duration using the definition given in
"Normal symbols" section, "A: duration" sub-section.


## Normal symbols

Symbols are normal when at least one of G1, G2, or G3 holds the value 1.

### A: Duration

A encodes the duration of a note or chord. It is defined as follows:

A value | Duration in beats
------- | -----------------
000     | 4
001     | 2
010     | 1
011     | 1/2
100     | 1/4
101     | 1/8
110     | 1/16
111     | 1/32

### B: Triplet

A note is part of a triplet when B is 1. The note is not otherwise.

### C: Slide

A note is part of a slide when C is 1. The note is not otherwise.

### D: Effect

D encodes the type of effect the note (or chord) is played with. 4 effects are
defined:
* Hajiki (pull-off)
* Uchi (hammer-on)
* Sukui (playing by pulling the bachi up)
* Suberi (sliding the bachi onto the string from the lower one)

The encoding is defined as:

D value | Effect
------- | ------
000     | No effect
001     | Hajiki
010     | Uchi
011     | Sukui
100     | Suberi
Other   | Undefined

### E: Mae bachi

A note is played in mae bachi when E is 1. Otherwise, the note is played in
ushiro bachi (assumed as the normal play style here).

### F: Finger

Encodes the indication of which finger to use to play the note. Encoding is
defined as:

F value | Usual marker | Finger
------- | ------------ | ------
000     | None         | No indication
001     | I            | Index finger
010     | II           | Middle finger
011     | III          | Ring finger
100     | IV           | Pinky
Other   | Undefined    | Undefined

### G: Note played

When G is 1, a note is played on the corresponding string, and its position on
the sao (neck) is encoded by the associated H that follows. From MSB (most
significant bit) to LSB (least significant bit), we have in order: G1, H1, G2,
H2, G3, then H3. G1 encodes a note for the 1st string (ichi no ito, lowest), G2
for the 2nd string (ni no ito, middle), and G3 for the 3rd string (san no ito,
highest).

### H: Position of the note

The position of the note is directly coded in standard binary from 00000
(position 0, open string) to 11111 (position 31 on the neck), despite a shamisen
neck usually going only up to around 20. H1 encodes the position on the 1st
string, H2 on the 2nd string, and H3 on the 3rd string (similar to G sub-section
above). By convention, when G is 0 (no note played on associated string),
corresponding H should be set to 0.


## File markers

Shamitab files all begin with the magic number 0x334d5421 (ASCII for 3MT!).
Shamitab files are all ending with an end of file (EOF) marker 0xffffffff (all
32 bits set to 1).


## Example of 3mt file

Let us consider the following ASCII tab:

	    m   /   
	|---4---4-||
	|   -   - ||
	|-----0---||
	|     -   ||
	|-0-------||
	  -         

As stated above, a shamitab (3mt) file begins with the magic number `0x334d5421`.
Then, symbols are inserted one after another until the end of the tab.

As seen here, a tablature usually starts by a bar (considered as a "special
sybmbol" in our format), so we put all bits to 0, hence G1, G2, and G3 will be
0, meaning this is indeed a special symbol. The type of special symbol is then
encoded by section D of the symbol, set to 1 (bar):

	0000 0001 0000 0000 0000 0000 0000 0000
	      DDD
          
	Hexadecimal: 0x 01 00 00 00

Following the bar in a "normal symbol", where a 0 (open string) is played on the
first string. The first string is played, so section G1 is set to 1, and the
position of the note played on the first string is 0, so H1 is set to 0.
The note lasts for 1 beat. The duration is encoded by section A, then set to 2
to indicate a 1 beat duration.
The note has no effect, meaning nothing else to set.
In summary, we have:

	0100 0000 0000 0010 0000 0000 0000 0000
	AAA              GH HHHH
	                 1
	Hexadecimal: 0x 40 02 00 00

The second note is a 4 on the third string. Section G3 is set to 1 (3rd string
being played), and H3 is set to 4 (position being 4). The duration is also 1
beat, so A is set to 2 as previously. The note is also played in maebachi, so
section E of the symol is set to 1. This gives us:

	0100 0000 1000 0000 0000 0000 0010 0100
	AAA       E                     GH HHHH
	                                3
	Hexadecimal: 0x 40 80 00 20

The third note is a 0 (open string) again, but played on the second string. G2
is set to 1 (second string being played), and H2 left at 0 (position being 0).
The note is 1 beat long again, meaning A is 2. The note has no other property
(e.g. no effect).

	0100 0000 0000 0000 0000 1000 0000 0000
	AAA                      GHHH HH
	                         2
	Hexadecimal: 0x 40 00 08 00

The last note is a 4 on the 3rd string, similarly to the second note. This means
G3 is 1 (3rd string being played), and H3 is 4 (note position being 4). The note
is also 1 beat long, meaning A is 2. However, this note is played in suberi, and
not in maebachi. Where we had E at 1 earlier, we have E at 0 this time. Instead,
the suberi effect is encoded by section D, and more precisely by D being 4. This
gives:

	0100 0100 0000 0000 0000 0000 0010 0100
	AAA   DDD                       GH HHHH
	                                3
	Hexadecimal: 0x 44 00 00 24

The last symbol is a double bar, another special symbol. All bites are set to 0
except for section D which then encodes the special symbol. In this case, D is 2
for a double bar:

	0000 0010 0000 0000 0000 0000 0000 0000
	      DDD
          
	Hexadecimal: 0x 02 00 00 00

Once all symbols have been added the the file, one after another in the correct
order, the tab file ends with the end of file marker `0xffffffff`.

In summary, our 3mt file should then be:

	0x334d5421	# magic number
	0x01000000	# bar
	0x40020000 	# open 1st string, 1 beat
	0x40800020	# 4 on 3rd string, 1 beat, maebachi
	0x40000800	# open 2nd string, 1 beat
	0x44000024	# 4 on 3rd string, 1 beat, suberi
	0x02000000	# double bar
	0xffffffff	# end of file


## Remarks

If you have any question, correction, or suggestion regarding this document or
the shamitab format, please feel free to send your inquiries to the author.
