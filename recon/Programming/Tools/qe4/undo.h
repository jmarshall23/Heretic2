//	undo.h
//	common routines for undo operations

#ifndef __UNDO__
#define __UNDO__

typedef enum
{
	UNDO_NONE,
	UNDO_BUSY,
	UNDO_NEWBRUSH,	
	UNDO_EDITBRUSH,	
	UNDO_DELETEBRUSH,	
	UNDO_SUBTRACT		//needs to record lists differently
} undotypes;

typedef struct pbrush_s
{
	brush_t *b;
	struct pbrush_s *next;
} pbrush_t;



typedef struct undo_s
{
	undotypes type;
	char *title;
	brush_t brushdata;  //stores COPIES of the brush data
	pbrush_t brushlist;  //stores pointers to a brush

} undo_t;

void UNDO_Init (void);
void UNDO_DoUndo (HWND hWnd);
void UNDO_Reset (void);

void UNDO_FinishBrushAdd(char *title);	//needs no start

void UNDO_StartBrushEdit(char *title);
void UNDO_FinishBrushEdit(char *title);

void UNDO_StartBrushDrag(void);  //similar to edit, but uses temp buffer
void UNDO_FinishBrushDrag(char *title);  //similar to edit, but uses temp buffer

void UNDO_StartBrushDelete(char *title);	//needs no finish

//test for and remove brush if in brushlist
void UNDO_ClearSelection(brush_t *brush);

//not sure about the selection choices yet...
void UNDO_StartSubtraction(void);
void UNDO_AddToSubtractData(brush_t *brush);
void UNDO_AddToSubtractList(brush_t *brush);
void UNDO_FinishSubtraction(char *title);

//these routines do the actual undoing
void UNDO_BrushClone (undo_t *buffer);	//copies brushdata into world
void UNDO_BrushRemove (undo_t *buffer);	//removes any brushes in brushlist

extern undo_t undobuffer;

#endif