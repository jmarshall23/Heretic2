
// window system independent camera view code

typedef enum
{
	cd_wire,
	cd_solid,
	cd_texture,
	cd_blend
} camera_draw_mode;

typedef struct
{
	int		width, height;

	qboolean	timing;

	vec3_t	origin;
	vec3_t	angles;

	camera_draw_mode	draw_mode;

	vec3_t	color;			// background 

	vec3_t	forward, right, up;	// move matrix

	vec3_t	vup, vpn, vright;	// view matrix
} camera_t;

extern	camera_t	camera;

void Cam_Init ();
void Cam_KeyDown (int key);
void Cam_MouseDown (int x, int y, int buttons);
void Cam_MouseUp (int x, int y, int buttons);
void Cam_MouseMoved (int x, int y, int buttons);
void Cam_MouseControl (float dtime);
void Cam_Draw ();

void Cam_HomeView ();
void Cam_ChangeFloor (qboolean up);

