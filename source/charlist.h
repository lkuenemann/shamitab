/* Shamitab alpha
 * Luc Kuenemann
 *
 * char linked list "charlist" API
 */

#include <stdlib.h>


typedef struct charlist {
	char c;
	struct charlist* next;
} charlist;


void freelist(charlist* head);

charlist* initializelist(charlist* head, char c);

charlist* appendlist(charlist* tail, char c);
