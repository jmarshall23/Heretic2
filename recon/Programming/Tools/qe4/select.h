
typedef enum
{
	sel_brush,
	// sel_sticky_brush,
	// sel_face,
	sel_vertex,
	sel_edge
} select_t;

typedef struct
{
	brush_t		*brush;
	face_t		*face;
	float		dist;
	qboolean	selected;
} trace_t;

#define	SF_SELECTED_ONLY	1
#define	SF_ENTITIES_FIRST	2
#define	SF_SINGLEFACE		4


trace_t Test_Ray (vec3_t origin, vec3_t dir, int flags);

void Select_GetBounds (vec3_t mins, vec3_t maxs);
void Select_Brush (brush_t *b);
void Select_Ray (vec3_t origin, vec3_t dir, int flags);
void Select_Delete (qboolean undo);
void Select_Deselect (void);
void Select_Clone (void);
void Select_Move (vec3_t delta);
void Select_SetTexture (texdef_t *texdef);
void Select_FlipAxis (int axis);
void Select_RotateAxis (int axis, float deg);
void Select_CompleteTall (void);
void Select_PartialTall (void);
void Select_Touching (void);
void Select_Inside (void);
void Select_MakeStructural (void);
void Select_MakeDetail (void);
void TextureAxisFromPlane(plane_t *pln, vec3_t xv, vec3_t yv);
