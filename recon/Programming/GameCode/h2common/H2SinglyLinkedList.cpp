// H2SinglyLinkedList.c
//

#include "../qcommon/H2Common.h"
#include "../qcommon/SinglyLinkedList.h"
#include "../qcommon/ResourceManager.h"
#include "../qcommon/SinglyLinkedList.h"

#include <stdint.h>

ResourceManager_t globalResourceManager;

H2COMMON_API void SLList_DefaultCon(SinglyLinkedList_t *this_ptr)
{
	this_ptr->rearSentinel =  (SinglyLinkedListNode_t *)ResMngr_AllocateResource(&globalResourceManager, 0);
	this_ptr->current = this_ptr->rearSentinel;
	this_ptr->front = this_ptr->rearSentinel;
}
H2COMMON_API void SLList_Des(SinglyLinkedList_t* this_ptr)
{
	SinglyLinkedListNode_t* node; 

	node = this_ptr->front;
	while (node != this_ptr->rearSentinel)
	{
		node = node->next;
		ResMngr_AllocateResource(&globalResourceManager, 0); // jmarshall: why are these here? 
	}
	this_ptr->current = this_ptr->rearSentinel;
	ResMngr_AllocateResource(&globalResourceManager, 0); // jmarshall: why are these here? 
}

H2COMMON_API qboolean SLList_AtEnd(SinglyLinkedList_t *this_ptr)
{
	return this_ptr->current == this_ptr->rearSentinel;
}

H2COMMON_API qboolean SLList_AtLast(SinglyLinkedList_t *this_ptr)
{
	return this_ptr->current->next == this_ptr->rearSentinel;
}

H2COMMON_API qboolean SLList_IsEmpty(SinglyLinkedList_t *this_ptr)
{
	return this_ptr->front == this_ptr->rearSentinel;
}

H2COMMON_API const GenericUnion4_t SLList_Increment(SinglyLinkedList_t *this_ptr)
{
	struct SinglyLinkedListNode_s* nextNode; // ecx
	nextNode = this_ptr->current->next;
	this_ptr->current = nextNode;
	return nextNode->value;
}

H2COMMON_API const GenericUnion4_t SLList_PostIncrement(SinglyLinkedList_t *this_ptr)
{
	GenericUnion4_t value; // [esp+D0h] [ebp-14h]
	SinglyLinkedListNode_t* currentNode; // [esp+DCh] [ebp-8h]

	currentNode = this_ptr->current;
	value = currentNode->value;
	this_ptr->current = currentNode->next;
	return value;
}

H2COMMON_API GenericUnion4_t SLList_Front(SinglyLinkedList_t *this_ptr)
{
	SinglyLinkedListNode_t* frontNode; // ecx

	frontNode = this_ptr->front;
	this_ptr->current = frontNode;
	return frontNode->value;
}

H2COMMON_API GenericUnion4_t SLList_ReplaceCurrent(SinglyLinkedList_t *this_ptr, const GenericUnion4_t toReplace)
{
	GenericUnion4_t oldValue; // [esp+D0h] [ebp-14h]
	SinglyLinkedListNode_t* node; // [esp+DCh] [ebp-8h]

	node = this_ptr->current;
	oldValue = node->value;
	node->value = toReplace;
	return oldValue;
}

H2COMMON_API void SLList_PushEmpty(SinglyLinkedList_t *this_ptr)
{
	SinglyLinkedListNode_t* emptyNode; // [esp+D0h] [ebp-8h]

	emptyNode = (SinglyLinkedListNode_t*)ResMngr_AllocateResource(&globalResourceManager, 0);
	emptyNode->next = this_ptr->front;
	this_ptr->front = emptyNode;
}

H2COMMON_API void SLList_Push(SinglyLinkedList_t *this_ptr, const GenericUnion4_t toInsert)
{
	SinglyLinkedListNode_t* newNode; 

	newNode = (SinglyLinkedListNode_t*)ResMngr_AllocateResource(&globalResourceManager, 0);
	newNode->value = toInsert;
	newNode->next = this_ptr->front;
	this_ptr->front = newNode;
}

H2COMMON_API GenericUnion4_t SLList_Pop(SinglyLinkedList_t *this_ptr)
{
	GenericUnion4_t value; 
	SinglyLinkedListNode_t* nextNode; 
	SinglyLinkedListNode_t* currentNode;
	SinglyLinkedListNode_t* frontNode;

	frontNode = this_ptr->front;
	currentNode = this_ptr->current;
	nextNode = frontNode->next;
	this_ptr->front = nextNode;
	if (currentNode == frontNode)
		this_ptr->current = nextNode;
	value = frontNode->value;
	ResMngr_DeallocateResource(&globalResourceManager, currentNode, 0);
	return value;
}

H2COMMON_API void SLList_Chop(SinglyLinkedList_t *this_ptr)
{
	SinglyLinkedList_t* currentNode; 
	SinglyLinkedList_t* nextNode;

	nextNode = (SinglyLinkedList_t*)this_ptr->current->next;
	if (nextNode == (SinglyLinkedList_t*)this_ptr->rearSentinel)
	{
		this_ptr->current = this_ptr->rearSentinel;
	}
	else
	{
		do
		{
			currentNode = nextNode;
			nextNode = (SinglyLinkedList_t*)nextNode->front;
			ResMngr_DeallocateResource(&globalResourceManager, currentNode, 0);
		} while (nextNode != (SinglyLinkedList_t*)this_ptr->rearSentinel);
		this_ptr->current = this_ptr->rearSentinel;
	}
}

H2COMMON_API void SLList_InsertAfter(SinglyLinkedList_t *this_ptr, const GenericUnion4_t toInsert)
{
	SinglyLinkedListNode_t* newNode; 

	newNode = (SinglyLinkedListNode_t*)ResMngr_AllocateResource(&globalResourceManager, 0);
	newNode->value = toInsert;
	newNode->next = this_ptr->current->next;
	this_ptr->current->next = newNode;
}