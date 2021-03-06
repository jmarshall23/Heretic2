

typedef struct
{
	int		x, y;
	int		w, h;

	int		redraws;	// clear to 0 when need update

	vec3_t	color;

	vec3_t	origin;
	vec3_t	angles;
	vec3_t	forward, right, up;

	int		draw_filtered;
	int		draw_wire;
	int		draw_solid;
	int		draw_textured;
	int		draw_blend;
} vieworg_t;

extern	vieworg_t	*view_org;
extern	int			keysdown;

void View_Init (void);
void View_Draw (void);
void View_MouseDown (int x, int y, int buttons);
void View_KeyUp (int key);
void View_KeyDown (int key);
void View_Reshape(int w, int h);

