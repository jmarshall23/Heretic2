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
H2COMMON_API void SLList_Des(SinglyLinkedList_t *this_ptr)
{
	SinglyLinkedListNode_t *v1; // esi@1
	SinglyLinkedListNode_t *v2; // eax@2
	SinglyLinkedListNode_t *v3; // ST04_4@3

	v1 = this_ptr->front;
	while (v1 != this_ptr->rearSentinel)
	{
		v2 = v1;
		v1 = v1->next;
		ResMngr_AllocateResource(&globalResourceManager, 0);
	}
	v3 = this_ptr->rearSentinel;
	this_ptr->current = this_ptr->rearSentinel;
	ResMngr_AllocateResource(&globalResourceManager, 0);
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
	SinglyLinkedListNode_t *v1;

	v1 = this_ptr->current->next;
	this_ptr->current = v1;
	return v1->value;
}

H2COMMON_API const GenericUnion4_t SLList_PostIncrement(SinglyLinkedList_t *this_ptr)
{
	SinglyLinkedListNode_t *v1; // ecx@1
	GenericUnion4_t result; // eax@1

	v1 = this_ptr->current;
	result = v1->value;
	this_ptr->current = v1->next;
	return result;
}

H2COMMON_API GenericUnion4_t SLList_Front(SinglyLinkedList_t *this_ptr)
{
	SinglyLinkedListNode_t *v1; // eax@1

	v1 = this_ptr->front;
	this_ptr->current = v1;
	return v1->value;
}

H2COMMON_API GenericUnion4_t SLList_ReplaceCurrent(SinglyLinkedList_t *this_ptr, const GenericUnion4_t toReplace)
{
	GenericUnion4_t *v2; // ecx@1
	GenericUnion4_t result; // eax@1

	v2 = (GenericUnion4_t *)this_ptr->current;
	result = *v2;
	*v2 = toReplace;
	return result;
}

H2COMMON_API void SLList_PushEmpty(SinglyLinkedList_t *this_ptr)
{
	SinglyLinkedListNode_t *v1; // eax@1

	v1 = (SinglyLinkedListNode_t *)ResMngr_AllocateResource(&globalResourceManager, 0);
	v1->next = this_ptr->front;
	this_ptr->front = v1;
}

H2COMMON_API void SLList_Push(SinglyLinkedList_t *this_ptr, const GenericUnion4_t toInsert)
{
	SinglyLinkedListNode_t *v2; // eax@1

	v2 = (SinglyLinkedListNode_t *)ResMngr_AllocateResource(&globalResourceManager, 0);
	v2->value = toInsert;
	v2->next = this_ptr->front;
	this_ptr->front = v2;
}

H2COMMON_API GenericUnion4_t SLList_Pop(SinglyLinkedList_t *this_ptr)
{
	SinglyLinkedListNode_t *v1; // eax@1
	SinglyLinkedListNode_t *v2; // esi@1
	SinglyLinkedListNode_t *v3; // edx@1
	GenericUnion4_t v4; // esi@3

	v1 = this_ptr->front;
	v2 = this_ptr->current;
	v3 = v1->next;
	this_ptr->front = v3;
	if (v2 == v1)
		this_ptr->current = v3;
	v4 = v1->value;
	ResMngr_DeallocateResource(&globalResourceManager, v2, 0);
	return v4;
}
H2COMMON_API void SLList_Chop(SinglyLinkedList_t *this_ptr)
{
	SinglyLinkedListNode_t *v1; // esi@1
	SinglyLinkedListNode_t *v2; // eax@2

	v1 = this_ptr->current->next;
	if (v1 == this_ptr->rearSentinel)
	{
		this_ptr->current = this_ptr->rearSentinel;
	}
	else
	{
		do
		{
			v2 = v1;
			v1 = v1->next;
			ResMngr_DeallocateResource(&globalResourceManager, v2, 0);
		} while (v1 != this_ptr->rearSentinel);
		this_ptr->current = this_ptr->rearSentinel;
	}
}

H2COMMON_API void SLList_InsertAfter(SinglyLinkedList_t *this_ptr, const GenericUnion4_t toInsert)
{
	SinglyLinkedListNode_t *v2; // eax@1

	v2 = (SinglyLinkedListNode_t *)ResMngr_AllocateResource(&globalResourceManager, 0);
	v2->value = toInsert;
	v2->next = this_ptr->current->next;
	this_ptr->current->next = v2;
}