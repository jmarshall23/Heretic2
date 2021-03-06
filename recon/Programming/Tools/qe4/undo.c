//  undo.c
//	storage for undo operations
//
#include "qe3.h"
#include "undo.h"

extern HWND s_hwndToolbar;

// NOTICE THAT THIS CODE IS VERY MUCH UNDER DEVELOPMENT!


undo_t undobuffer; 

brush_t dragbrush;	//old data for dragging needs seperate location
brush_t subdata;	//storage space for subtract
pbrush_t sublist;	//this will be a zero terminated list


char NO_UNDO_STRING[] = "Can't Undo";

// GENERIC FUNCTIONS

void UNDO_EnableButton ()
{
    SendMessage(s_hwndToolbar, TB_ENABLEBUTTON, (WPARAM)ID_EDIT_UNDO,
        (LPARAM) (MAKELONG(TRUE, 0))); 
}

void UNDO_DisableButton ()
{
    SendMessage(s_hwndToolbar, TB_ENABLEBUTTON, (WPARAM)ID_EDIT_UNDO,
        (LPARAM) (MAKELONG(FALSE, 0))); 
}

void UNDO_Init ()
{
	undobuffer.brushdata.next = undobuffer.brushdata.prev = &undobuffer.brushdata;
	undobuffer.brushlist.next = 0;
	dragbrush.next = dragbrush.prev = &dragbrush;
	subdata.next = subdata.prev = &subdata;
	sublist.next = NULL;
	undobuffer.title = NO_UNDO_STRING;
}

void UNDO_AddPointer (pbrush_t *list, brush_t *brush)
{
	pbrush_t *pb = qmalloc(sizeof(pbrush_t));
	
	pb->next = list->next;
	list->next = pb;
	pb->b = brush;
}


void UNDO_Reset ()
{
	brush_t *brush;
	pbrush_t *pb;
	undobuffer.type = UNDO_NONE;
	undobuffer.title = NO_UNDO_STRING;

	while (undobuffer.brushdata.next != &undobuffer.brushdata)
	{
		brush = undobuffer.brushdata.next;
		Brush_Free (brush);
	}
	while(undobuffer.brushlist.next)
	{
		pb = undobuffer.brushlist.next->next;
		free(undobuffer.brushlist.next);
		undobuffer.brushlist.next = pb;
	}
	UNDO_DisableButton();
//	undobuffer.brushdata.next = undobuffer.brushdata.prev = &undobuffer.brushdata;
}

void UNDO_DoUndo (HWND hWnd)
{
	switch(undobuffer.type)
	{
	case UNDO_NONE:
		MessageBox (hWnd, "Nothing to undo, why do you bother me?", "NO", MB_OK);
		break;
	case UNDO_NEWBRUSH:
		UNDO_BrushRemove (&undobuffer);
		Sys_UpdateWindows (W_ALL);
		break;
	case UNDO_EDITBRUSH:
	case UNDO_SUBTRACT:
		UNDO_BrushRemove (&undobuffer);
		UNDO_BrushClone (&undobuffer);
		Sys_UpdateWindows (W_ALL);
		break;
	case UNDO_DELETEBRUSH:
		UNDO_BrushClone (&undobuffer);
		Sys_UpdateWindows (W_ALL);
		break;
	default:
		break;
		//not done yet, go away
	}
	undobuffer.type = UNDO_NONE;  //don't allow multiple undos
	undobuffer.title = NO_UNDO_STRING;
	UNDO_DisableButton ();
}

// BRUSH FUNCTIONS

void    Face_Free( face_t *f );

// BRUSH START/FINISH FUNCTIONS


// BrushDrag is basically an abortable version of BrushEdit
// if Finish isn't called, it has no effect on the undo buffer

void UNDO_StartBrushDrag()
{
	brush_t *from, *data;	

	while (dragbrush.next != &dragbrush)
	{
		from = dragbrush.next;
		Brush_Free (from);
	}

	from = &selected_brushes;
	while (from->next != &selected_brushes)
	{
		from = from->next;
		data = Brush_Clone(from);
		Brush_AddToList(data, &dragbrush);	//store data
	}	

}

void UNDO_FinishBrushDrag(char *title)
{
	brush_t *from;	

	UNDO_Reset ();
	undobuffer.type = UNDO_EDITBRUSH;
	UNDO_EnableButton ();
	if(title)
	{
		undobuffer.title = title;
	}
	else
	{
		undobuffer.title = "&Undo Drag";
	}
	
	if(dragbrush.next != &dragbrush)
	{
		undobuffer.brushdata.next = dragbrush.next;
		undobuffer.brushdata.prev = dragbrush.prev;
		dragbrush.prev->next = &undobuffer.brushdata;
		dragbrush.next->prev = &undobuffer.brushdata;
		dragbrush.prev = dragbrush.next = &dragbrush;
	}

	from = &selected_brushes;
	while (from->next != &selected_brushes)
	{
		from = from->next;
		UNDO_AddPointer(&undobuffer.brushlist, from);	//need to store pointer to original
	}	

}



void UNDO_FinishBrushAdd(char * title)
{
	brush_t *from;	

	UNDO_Reset ();
	UNDO_EnableButton ();
	undobuffer.type = UNDO_NEWBRUSH;
	if(title)
	{
		undobuffer.title = title;
	}
	else
	{
		undobuffer.title = "&Undo Add";
	}

	from = &selected_brushes;
	while (from->next != &selected_brushes)
	{
		from = from->next;
		UNDO_AddPointer(&undobuffer.brushlist, from);	//need to store pointer to original
	}	
}


// the BrushEdit functions assume the selected brushes remain the same
// these are identical to the New and Delete as well, perhaps
void UNDO_StartBrushEdit(char *title)
{
	brush_t *from, *data;	

	UNDO_Reset ();
	undobuffer.type = UNDO_BUSY;
	if(title)
	{
		undobuffer.title = title;
	}
	else
	{
		undobuffer.title = "&Undo Edit";
	}

	from = &selected_brushes;
	while (from->next != &selected_brushes)
	{
		from = from->next;
		data = Brush_Clone(from);
		Brush_AddToList(data, &undobuffer.brushdata);	//store data
	}	
}

void UNDO_FinishBrushEdit(char *title)
{
	brush_t *from;	

	UNDO_EnableButton ();
	undobuffer.type = UNDO_EDITBRUSH;
	if(title)
	{	// this is optional, since we may not know what the drag does at start
		undobuffer.title = title;
	}

	from = &selected_brushes;
	while (from->next != &selected_brushes)
	{
		from = from->next;
		UNDO_AddPointer(&undobuffer.brushlist, from);	//need to store pointer to original
	}	
}

void UNDO_StartBrushDelete(char *title)
{
	brush_t *from, *data;	
	
	if(selected_brushes.next == &selected_brushes)
	{
		return;
	}
	UNDO_Reset ();
	undobuffer.type = UNDO_DELETEBRUSH;
	if(title)
	{
		undobuffer.title = title;
	}
	else
	{
		undobuffer.title = "&Undo Delete";
	}

	from = &selected_brushes;
	while (from->next != &selected_brushes)
	{
		from = from->next;
		data = Brush_Clone(from);
		Brush_AddToList(data, &undobuffer.brushdata);

	}
	UNDO_EnableButton();
}


// needed if selection options remove current brush
void UNDO_ClearSelection (brush_t *brush)
{

	pbrush_t *next;
	qboolean found;

	
	found = FALSE;
	if(!undobuffer.brushlist.next)
	{
		return;
	}
	for(next = undobuffer.brushlist.next; 
		!next->next; 
		next = next->next)
		{
			if(next->b == brush)
			{
				found = TRUE;
				break;
			}
		}
	if(found)
	{
		UNDO_Reset();
	}
}



void UNDO_StartSubtraction()
{	// just clears out brush list
	brush_t *from;
	pbrush_t *pb;

	while (subdata.next != &subdata)
	{
		from = subdata.next;
		Brush_Free (from);
	}
	subdata.next = subdata.prev = &subdata;
	while(sublist.next)
	{
		pb = sublist.next->next;
		free(sublist.next);
		sublist.next = pb;
	}

}

void UNDO_AddToSubtractData(brush_t *brush)
{	
	brush_t *data;
	pbrush_t *pb, *last;
	//if brush is in add list, then remove it and quit
	pb = &sublist;
	while(pb->next)
	{
		if(pb->next->b == brush)
		{
			last = pb->next;
			pb->next = last->next;
			free (last);
			return;
		}
		pb = pb->next;
	}

	data = Brush_Clone(brush);
	Brush_AddToList(data, &subdata);	//store data

}

void UNDO_AddToSubtractList(brush_t *brush)
{
	UNDO_AddPointer(&sublist, brush);
	//add newly created brushes to list
}

void UNDO_FinishSubtraction (char *title)
{
	UNDO_Reset ();

	UNDO_EnableButton ();
	undobuffer.type = UNDO_SUBTRACT;
	if(title)
	{
		undobuffer.title = title;
	}
	else
	{
		undobuffer.title = "&Undo Subtract";
	}

	//copy data from temp buffers
	if(subdata.next != &subdata)
	{
		undobuffer.brushdata.next = subdata.next;
		undobuffer.brushdata.prev = subdata.prev;
		subdata.prev->next = &undobuffer.brushdata;
		subdata.next->prev = &undobuffer.brushdata;
		subdata.prev = subdata.next = &subdata;
	}
	if(sublist.next)
	{
		undobuffer.brushlist.next = sublist.next;
		sublist.next = NULL;
	}
}




// UNDO VERSIONS OF SELECT.C
/*
============
	based on Select_Clone, copies brushdata into selected
============
*/
void UNDO_BrushClone (undo_t *buffer)
{
	brush_t		*b, *b2, *n, *next, *next2;
	entity_t	*e;

	g_qeglobals.d_workcount++;
	g_qeglobals.d_select_mode = sel_brush;

	for (b=buffer->brushdata.next ; b != &buffer->brushdata ; b=next)
	{
		next = b->next;
		// if the brush is a world brush, handle simply
		if (b->owner == world_entity)
		{
			n = Brush_Clone (b);
			Brush_AddToList (n, &active_brushes);
			Entity_LinkBrush (world_entity, n);
			Brush_Build( n );
			Select_Brush(n);
			continue;
		}

		e = Entity_Clone (b->owner);

		// if the brush is a fixed size entity, create a new entity
		if (b->owner->eclass->fixedsize)
		{
			n = Brush_Clone (b);
			Brush_AddToList (n, &active_brushes);

			Entity_LinkBrush (e, n);
			Brush_Build( n );
			Select_Brush(n);
			continue;
		}
        
		// brush is a complex entity, grab all the other ones now

		next = &buffer->brushdata;

		for ( b2 = b ; b2 != &buffer->brushdata ; b2=next2)
		{
			next2 = b2->next;
			if (b2->owner != b->owner)
			{
				if (next == &buffer->brushdata)
					next = b2;
				continue;
			}

			n = Brush_Clone (b2);
			Brush_AddToList (n, &active_brushes);
			Entity_LinkBrush (e, n);
			Brush_Build( n );
			Select_Brush(n);
		}

	}
}

void UNDO_BrushRemove (undo_t *buffer)
{
	pbrush_t *next;
	// this does NOT update the count globals, does it need to?
	while(buffer->brushlist.next)
	{
		next = buffer->brushlist.next->next;
		Brush_Free (buffer->brushlist.next->b);
		free (buffer->brushlist.next);
		buffer->brushlist.next = next;
	}
	buffer->brushlist.next = 0;

}

