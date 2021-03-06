
// window system independent camera view code

typedef struct
{
	int		width, height;

	qboolean	timing;

	vec3_t	origin;			// at center of window
	float	scale;

	float	topclip, bottomclip;

	qboolean d_dirty;
} xy_t;

BOOL XYExcludeBrush(brush_t	*pb);

void XY_Init (void);
void XY_MouseDown (int x, int y, int buttons);
void XY_MouseUp (int x, int y, int buttons);
void XY_MouseMoved (int x, int y, int buttons);
void XY_Draw (void);
void XY_Overlay (void);
void XY_ToGridPoint (int x, int y, vec3_t point);

BOOL FilterBrush(brush_t *pb);
