#ifndef MESSAGE_H
#define MESSAGE_H

#include "SinglyLinkedList.h"

typedef struct MsgQueue_s
{
	SinglyLinkedList_t msgs;
} MsgQueue_t;


size_t SetParms(SinglyLinkedList_t *this_ptr, char *format, va_list marker, qboolean entsAsNames = false);

int GetParms(SinglyLinkedList_t *this_ptr, char *format, va_list marker);
void QueueMessage(MsgQueue_t *this_ptr, void *msg);

#endif