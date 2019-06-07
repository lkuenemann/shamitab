/* Shamitab
 * Luc Kuenemann
 *
 * Implementation of char linked list
 */


#include "charlist.h"


// Free linked list
void freelist(charlist* head)
{
	charlist* tmp;
	while(head!=NULL)
	{
		tmp = head;
		head = head->next;
		free(tmp);
	}
}


// Initialize charlist
charlist* initializelist(charlist* head, char c)
{
	head = malloc(sizeof(charlist));
	head->c = c;
	head->next = NULL;
	return head;
}


// Append new element to charlist linked list
charlist* appendlist(charlist* tail, char c)
{
	charlist* new = malloc(sizeof(charlist));
	new->c = c;
	new->next = NULL;
	tail->next = new;
	return new;
}
