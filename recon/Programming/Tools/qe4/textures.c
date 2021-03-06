#include "qe3.h"
#include "io.h"

#define	TYP_MIPTEX	68
static unsigned	tex_palette[256];

static qtexture_t	*notexture;

static qboolean	nomips;

#define	FONT_HEIGHT	10

static HGLRC s_hglrcTexture;
static HDC	 s_hdcTexture;

//int		texture_mode = GL_NEAREST;
//int		texture_mode = GL_NEAREST_MIPMAP_NEAREST;
//int		texture_mode = GL_NEAREST_MIPMAP_LINEAR;
//int		texture_mode = GL_LINEAR;
//int		texture_mode = GL_LINEAR_MIPMAP_NEAREST;
int		texture_mode = GL_LINEAR_MIPMAP_LINEAR;

int		texture_extension_number = 1;

// current active texture directory.  if empty, show textures in use
char		texture_directory[32];	// use if texture_showinuse is false
qboolean	texture_showinuse;

// texture layout functions
qtexture_t	*current_texture;
int			current_x, current_y, current_row;

int			texture_nummenus;
#define		MAX_TEXTUREDIRS	100
char		texture_menunames[MAX_TEXTUREDIRS][64];

qboolean	g_dontuse;		// set to true to load the texture but not flag as used

void SelectTexture (int mx, int my);

void	Texture_MouseDown (int x, int y, int buttons);
void	Texture_MouseUp (int x, int y, int buttons);
void	Texture_MouseMoved (int x, int y, int buttons);

//=====================================================

void SortTextures(void)
{	
	qtexture_t	*q, *qtemp, *qhead, *qcur, *qprev;

	// standard insertion sort
	// Take the first texture from the list and
	// add it to our new list
	if ( g_qeglobals.d_qtextures == NULL)
		return;	

	qhead = g_qeglobals.d_qtextures;
	q = g_qeglobals.d_qtextures->next;
	qhead->next = NULL;
	
	// while there are still things on the old
	// list, keep adding them to the new list
	while (q)
	{
		qtemp = q;
		q = q->next;
		
		qprev = NULL;
		qcur = qhead;

		while (qcur)
		{
			// Insert it here?
			if (strcmp(qtemp->name, qcur->name) < 0)
			{
				qtemp->next = qcur;
				if (qprev)
					qprev->next = qtemp;
				else
					qhead = qtemp;
				break;
			}
			
			// Move on

			qprev = qcur;
			qcur = qcur->next;


			// is this one at the end?

			if (qcur == NULL)
			{
				qprev->next = qtemp;
				qtemp->next = NULL;
			}
		}


	}

	g_qeglobals.d_qtextures = qhead;
}

/*
============================================================================

  SCROLLBAR UPDATE AND RESPONSE

============================================================================
*/

void Texture_SetScrollRange (void)
{
	qtexture_t	*q, *curtex;
	int done;
	int curx, cury, currow;

	//start at the top
	curtex = g_qeglobals.d_qtextures;
	curx = 8;
	cury = -12 - FONT_HEIGHT;
	currow = 0;

	//go thru all the textures and figure out how tall the whole list is
	done = false;

	while (curtex)
	{
		while (1)
		{
			q = curtex;
			if (!q)
				done = true;
			else
			{
				curtex = curtex->next;
				if (q->name[0] == '(')	// fake color texture
					continue;
				if (q->inuse)
					break;			// allways show in use
				if (!texture_showinuse && strncmp (q->name, texture_directory, strlen(texture_directory)))
					continue;
			}
			break;
		}

		if (!done)
		{
			if (curx + q->width > g_qeglobals.d_texturewin.width-8 && currow)
			{	// go to the next row unless the texture is the first on the row
				curx = 8;
				cury -= currow + FONT_HEIGHT + 4;
				currow = 0;
			}

			// Is our texture larger than the row? If so, grow the 
			// row height to match it

			if (currow < q->height)
				currow = q->height;

			// never go less than 64, or the names get all crunched up
			curx += q->width < 64 ? 64 : q->width;
			curx += 8;
		}
	}

	g_qeglobals.d_texturewin.scrollheight = -cury + currow - g_qeglobals.d_texturewin.height;

	SetScrollRange(g_qeglobals.d_hwndTexture, SB_VERT, 0,
		g_qeglobals.d_texturewin.scrollheight, true);
}

void Texture_SetScrollPos (void)
{
	SetScrollPos(g_qeglobals.d_hwndTexture, SB_VERT, -g_qeglobals.d_texturewin.originy, true);
}

void Texture_ScrollLine (int ydown)
{
	g_qeglobals.d_texturewin.originy += ydown;
	if (g_qeglobals.d_texturewin.originy > 0)
	{
		g_qeglobals.d_texturewin.originy = 0;
	}
	if (g_qeglobals.d_texturewin.originy < -g_qeglobals.d_texturewin.scrollheight)
	{
		g_qeglobals.d_texturewin.originy = -g_qeglobals.d_texturewin.scrollheight;
	}
	Sys_UpdateWindows (W_TEXTURE);
	Texture_SetScrollPos();
}

void Texture_ScrollRow (int ydown)//warning: i only plan on making this work for values of 1 & -1 --ss
{
	qtexture_t	*q, *curtex;
	int done;
	int curx, cury, currow, lastrow;

	//start at the top
	curtex = g_qeglobals.d_qtextures;
	curx = 8;
	cury = 0;//to make the textures above top row not peek down at top of window, set this to -8 initially
				//(it's set to 0 to let you go to very top of window with scrollbar arrows)
	currow = 0;

	//go thru all the textures until we find the row we want to scroll past
	done = false;
	lastrow = 0;

	while (curtex && !done)
	{
		while (1)
		{
			q = curtex;
			if (!q)
				done = true;
			else
			{
				curtex = curtex->next;
				if (q->name[0] == '(')	// fake color texture
					continue;
				if (q->inuse)
					break;			// allways show in use
				if (!texture_showinuse && strncmp (q->name, texture_directory, strlen(texture_directory)))
					continue;
			}
			break;
		}

		if (!done)
		{
			if (curx + q->width > g_qeglobals.d_texturewin.width-8 && currow)
			{	// go to the next row unless the texture is the first on the row
				curx = 8;

				if ((ydown > 0) && lastrow)//want to scroll down--set origin to cury + the last row
				{
					if (cury > g_qeglobals.d_texturewin.originy)
						g_qeglobals.d_texturewin.originy = cury;
					else
						g_qeglobals.d_texturewin.originy = cury+lastrow+FONT_HEIGHT+4;
					done = true;
				}
				else if ((ydown < 0)&&lastrow)//want to scroll up--set origin to cury
				{
					if (cury < g_qeglobals.d_texturewin.originy)
						g_qeglobals.d_texturewin.originy = cury;
					else
						g_qeglobals.d_texturewin.originy = cury-currow-FONT_HEIGHT-4;

					done = true;
				}
				else
				{
					cury -= currow + FONT_HEIGHT + 4;
					// Is this texture visible?
					if ( (cury-q->height-FONT_HEIGHT < g_qeglobals.d_texturewin.originy)
						&& (cury > g_qeglobals.d_texturewin.originy - g_qeglobals.d_texturewin.height) )
					{
						lastrow = currow;
					}

				}

				if (!done)
				{
					currow = 0;
				}
			}

			// Is our texture larger than the row? If so, grow the 
			// row height to match it

			if (currow < q->height)
				currow = q->height;

			// never go less than 64, or the names get all crunched up
			curx += q->width < 64 ? 64 : q->width;
			curx += 8;
		}
	}

	if (g_qeglobals.d_texturewin.originy > 0)
		g_qeglobals.d_texturewin.originy = 0;
	if (g_qeglobals.d_texturewin.originy < -g_qeglobals.d_texturewin.scrollheight)
		g_qeglobals.d_texturewin.originy = -g_qeglobals.d_texturewin.scrollheight;
	Sys_UpdateWindows (W_TEXTURE);
	Texture_SetScrollPos();

}

void Texture_ScrollPage (int ydown)//warning: i only plan on making this work for values of 1 & -1 --ss
{
	qtexture_t	*q, *curtex;
	int done;
	int curx, cury, currow, lastrow, curscroll;
	int pagedone, hasscrolled, totalscrolled;

	pagedone = false;
	hasscrolled = false;
	totalscrolled = 0;
	if (!g_qeglobals.d_qtextures)
		return; //JFM: stop the madness

	while ((!pagedone) || (!hasscrolled))
	{
		//start at the top
		curtex = g_qeglobals.d_qtextures;
		curx = 8;
		cury = 0;//to make the textures above top row not peek down at top of window, set this to -8 initially
					//(it's set to 0 to let you go to very top of window with scrollbar arrows)
		currow = 0;

		//go thru all the textures until we find the row we want to scroll past
		done = false;
		lastrow = 0;

		while (curtex && !done)
		{
			while (1)
			{
				q = curtex;
				if (!q)
				{
					pagedone = true;
					hasscrolled = true;
					done = true;
				}
				else
				{
					curtex = curtex->next;
					if (q->name[0] == '(')	// fake color texture
						continue;
					if (q->inuse)
						break;			// allways show in use
					if (!texture_showinuse && strncmp (q->name, texture_directory, strlen(texture_directory)))
						continue;
				}
				break;
			}

			if (!done)
			{
				if (curx + q->width > g_qeglobals.d_texturewin.width-8 && currow)
				{	// go to the next row unless the texture is the first on the row
					curx = 8;

					if ((ydown > 0) && lastrow)//want to scroll down--set origin to cury + the last row
					{
						if (cury > g_qeglobals.d_texturewin.originy)
						{
							curscroll = cury - g_qeglobals.d_texturewin.originy;
						}
						else
						{
							curscroll = cury + lastrow + FONT_HEIGHT + 4 - g_qeglobals.d_texturewin.originy;
						}

						if ((hasscrolled)&&(totalscrolled + curscroll > g_qeglobals.d_texturewin.height))
						{//hey, if i scroll any more, all prev. lines will go totally off the screen, so stop
							pagedone = true;
						}
						else
						{//can still safely scroll some more, so do it
							g_qeglobals.d_texturewin.originy += curscroll;
							totalscrolled += curscroll;
							if (g_qeglobals.d_texturewin.originy > -5)//oop, that was the last one
							{
								pagedone = true;
							}
						}

						hasscrolled = true;
						done = true;
					}
					else if ((ydown < 0)&&lastrow)//want to scroll up--set origin to cury
					{
						if (cury < g_qeglobals.d_texturewin.originy)
						{
							curscroll = g_qeglobals.d_texturewin.originy - cury;
						}
						else
						{
							curscroll = /*g_qeglobals.d_texturewin.originy - cury +*/ lastrow + FONT_HEIGHT + 4;
						}

						if ((hasscrolled)&&((totalscrolled + curscroll) > g_qeglobals.d_texturewin.height))
						{//hey, if i scroll any more, all prev. lines will go totally off the screen, so stop
							pagedone = true;
						}
						else
						{//can still safely scroll some more, so do it
							g_qeglobals.d_texturewin.originy -= curscroll;
							totalscrolled += curscroll;
							if (g_qeglobals.d_texturewin.originy < -g_qeglobals.d_texturewin.scrollheight+5)//oop, that was the last one
			// -g_qeglobals.d_texturewin.scrollheight+5 caused the program to crash...  ^^ this is a temp. hack fix
							{
								pagedone = true;
							}
						}

						hasscrolled = true;
						done = true;
					}
/*					{
						pagedone = true;
						hasscrolled = true;
						if (cury < g_qeglobals.d_texturewin.originy)
						{
							g_qeglobals.d_texturewin.originy = cury;
						}
						else
						{
							g_qeglobals.d_texturewin.originy = cury-currow-FONT_HEIGHT-4;
						}
						done = true;
					}
*/					else
					{
						cury -= currow + FONT_HEIGHT + 4;
						// Is this texture visible?
						if ( (cury-q->height-FONT_HEIGHT < g_qeglobals.d_texturewin.originy)
							&& (cury > g_qeglobals.d_texturewin.originy - g_qeglobals.d_texturewin.height) )
						{
							lastrow = currow;
						}

					}

					if (!done)
					{
						currow = 0;
					}
				}

				// Is our texture larger than the row? If so, grow the 
				// row height to match it

				if (currow < q->height)
				{
					currow = q->height;
				}
				// never go less than 64, or the names get all crunched up
				curx += q->width < 64 ? 64 : q->width;
				curx += 8;
			}
		}

		if (g_qeglobals.d_texturewin.originy > 0)
			g_qeglobals.d_texturewin.originy = 0;
		if (g_qeglobals.d_texturewin.originy < -g_qeglobals.d_texturewin.scrollheight)
			g_qeglobals.d_texturewin.originy = -g_qeglobals.d_texturewin.scrollheight;
	}
	Sys_UpdateWindows (W_TEXTURE);
	Texture_SetScrollPos();

}

void Texture_ScrollTo (int pos)
{
	g_qeglobals.d_texturewin.originy = pos;
	if (g_qeglobals.d_texturewin.originy > 0)
		g_qeglobals.d_texturewin.originy = 0;
	if (g_qeglobals.d_texturewin.originy < -g_qeglobals.d_texturewin.scrollheight)
		g_qeglobals.d_texturewin.originy = -g_qeglobals.d_texturewin.scrollheight;
	Sys_UpdateWindows (W_TEXTURE);
	Texture_SetScrollPos();
}

//=====================================================


/*
==============
Texture_InitPalette
==============
*/
void Texture_InitPalette (byte *pal)
{
    int		r,g,b,v;
    int		i;
	int		inf;
	byte	gammatable[256];
	float	gamma;

	gamma = g_qeglobals.d_savedinfo.fGamma;

	if (gamma == 1.0)
	{
		for (i=0 ; i<256 ; i++)
			gammatable[i] = i;
	}
	else
	{
		for (i=0 ; i<256 ; i++)
		{
			inf = 255 * pow ( (i+0.5)/255.5 , gamma ) + 0.5;
			if (inf < 0)
				inf = 0;
			if (inf > 255)
				inf = 255;
			gammatable[i] = inf;
		}
	}

    for (i=0 ; i<256 ; i++)
    {
		r = gammatable[pal[0]];
		g = gammatable[pal[1]];
		b = gammatable[pal[2]];
		pal += 3;
		
		v = (r<<24) + (g<<16) + (b<<8) + 255;
		v = BigLong (v);
		
		tex_palette[i] = v;
    }
}

void SetTexParameters (void)
{
	
	switch ( texture_mode )
	{
	case GL_NEAREST:
	case GL_NEAREST_MIPMAP_NEAREST:
	case GL_NEAREST_MIPMAP_LINEAR:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, texture_mode );
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
		break;
	case GL_LINEAR:
	case GL_LINEAR_MIPMAP_NEAREST:
	case GL_LINEAR_MIPMAP_LINEAR:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, texture_mode );
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		break;
	}
}

/*
============
Texture_SetMode
============
*/
void Texture_SetMode(int iMenu)
{
	int	i, iMode;
	HMENU hMenu;
	qboolean texturing = true;

	hMenu = GetMenu(g_qeglobals.d_hwndMain);

	switch(iMenu) {
	case ID_VIEW_NEAREST:					
		iMode = GL_NEAREST;
		break;
	case ID_VIEW_NEARESTMIPMAP:
		iMode = GL_NEAREST_MIPMAP_NEAREST;
		break;
	case ID_VIEW_LINEAR:
		iMode = GL_NEAREST_MIPMAP_LINEAR;
		break;
	case ID_VIEW_BILINEAR:
		iMode = GL_LINEAR;
		break;
	case ID_VIEW_BILINEARMIPMAP:
		iMode = GL_LINEAR_MIPMAP_NEAREST;
		break;
	case ID_VIEW_TRILINEAR:
		iMode = GL_LINEAR_MIPMAP_LINEAR;
		break;

	case ID_TEXTURES_WIREFRAME:
		iMode = 0;
		texturing = false;
		break;

	case ID_TEXTURES_FLATSHADE:
		iMode = 0;
		texturing = false;
		break;

	}

	CheckMenuItem(hMenu, ID_VIEW_NEAREST, MF_BYCOMMAND | MF_UNCHECKED);
	CheckMenuItem(hMenu, ID_VIEW_NEARESTMIPMAP, MF_BYCOMMAND | MF_UNCHECKED);
	CheckMenuItem(hMenu, ID_VIEW_LINEAR, MF_BYCOMMAND | MF_UNCHECKED);
	CheckMenuItem(hMenu, ID_VIEW_BILINEARMIPMAP, MF_BYCOMMAND | MF_UNCHECKED);
	CheckMenuItem(hMenu, ID_VIEW_BILINEAR, MF_BYCOMMAND | MF_UNCHECKED);
	CheckMenuItem(hMenu, ID_VIEW_TRILINEAR, MF_BYCOMMAND | MF_UNCHECKED);
	CheckMenuItem(hMenu, ID_TEXTURES_WIREFRAME, MF_BYCOMMAND | MF_UNCHECKED);
	CheckMenuItem(hMenu, ID_TEXTURES_FLATSHADE, MF_BYCOMMAND | MF_UNCHECKED);

	CheckMenuItem(hMenu, iMenu, MF_BYCOMMAND | MF_CHECKED);

	g_qeglobals.d_savedinfo.iTexMenu = iMenu;
	texture_mode = iMode;
	if ( texturing )
		SetTexParameters ();

	if ( !texturing && iMenu == ID_TEXTURES_WIREFRAME)
	{
		camera.draw_mode = cd_wire;
		Map_BuildBrushData(false);
		Sys_UpdateWindows (W_ALL);
		return;

	} else if ( !texturing && iMenu == ID_TEXTURES_FLATSHADE) {

		camera.draw_mode = cd_solid;
		Map_BuildBrushData(false);
		Sys_UpdateWindows (W_ALL);
		return;
	}

	for (i=1 ; i<texture_extension_number ; i++)
	{
		glBindTexture( GL_TEXTURE_2D, i );
		SetTexParameters ();
	}

	// select the default texture
	glBindTexture( GL_TEXTURE_2D, 0 );

	glFinish();

	if (camera.draw_mode != cd_texture)
	{
		camera.draw_mode = cd_texture;
		Map_BuildBrushData(false);
	}

	Sys_UpdateWindows (W_ALL);
}


/*
=================
Texture_LoadTexture
=================
*/
qtexture_t *Texture_LoadTexture (miptex_t *qtex)
{
    byte		*source;
    unsigned	*dest;
    int			width, height, i, count;
	int			total[3];
    qtexture_t	*q;
    
    q = qmalloc(sizeof(*q));
    width = LittleLong(qtex->width[0]);
    height = LittleLong(qtex->height[0]);

    q->width = width;
    q->height = height;

	q->flags = qtex->flags;
	q->value = qtex->value;
	q->contents = qtex->contents;
	q->scale_x = 1.0;
	q->scale_y = 1.0;

	dest = qmalloc (width*height*4);

    count = width*height;
    source = (byte *)qtex + LittleLong(qtex->offsets[0]);

	// The dib is upside down so we want to copy it into 
	// the buffer bottom up.

	total[0] = total[1] = total[2] = 0;
    for (i=0 ; i<count ; i++)
	{
		dest[i] = qtex->palette[source[i]].b;
		dest[i] <<= 8;
		dest[i] += qtex->palette[source[i]].g;
		dest[i] <<= 8;
		dest[i] += qtex->palette[source[i]].r;

		total[0] += ((byte *)(dest+i))[0];
		total[1] += ((byte *)(dest+i))[1];
		total[2] += ((byte *)(dest+i))[2];
	}

	q->color[0] = (float)total[0]/(count*255);
	q->color[1] = (float)total[1]/(count*255);
	q->color[2] = (float)total[2]/(count*255);

    q->texture_number = texture_extension_number++;

	glBindTexture( GL_TEXTURE_2D, q->texture_number );
	SetTexParameters ();

	if (nomips)
		glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, dest);
	else
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height,GL_RGBA, GL_UNSIGNED_BYTE, dest);

	free (dest);

	glBindTexture( GL_TEXTURE_2D, 0 );

    return q;
}

/*
=================
Texture_LoadTexture
=================
*/
qtexture_t *Texture_LoadTexture32 (miptex32_t *qtex)
{
    unsigned	*source;
    unsigned	*dest;
    int			width, height, i, count;
	int			total[3];
    qtexture_t	*q;

    if (qtex->version != MIP32_VERSION)
	{
		return NULL;
	}

    q = qmalloc(sizeof(*q));
    width = LittleLong(qtex->width[0]);
    height = LittleLong(qtex->height[0]);

    q->width = width;
    q->height = height;

	q->flags = qtex->flags;
	q->value = qtex->value;
	q->contents = qtex->contents;
	q->scale_x = qtex->scale_x;
	q->scale_y = qtex->scale_y;

	dest = qmalloc (width*height*4);

    count = width*height;
    source = (unsigned *)((byte *)qtex + LittleLong(qtex->offsets[0]));

	// The dib is upside down so we want to copy it into 
	// the buffer bottom up.

	total[0] = total[1] = total[2] = 0;
    for (i=0 ; i<count ; i++)
	{
		dest[i] = source[i];

		total[0] += ((byte *)(dest+i))[0];
		total[1] += ((byte *)(dest+i))[1];
		total[2] += ((byte *)(dest+i))[2];
	}

	q->color[0] = (float)total[0]/(count*255);
	q->color[1] = (float)total[1]/(count*255);
	q->color[2] = (float)total[2]/(count*255);

    q->texture_number = texture_extension_number++;

	glBindTexture( GL_TEXTURE_2D, q->texture_number );
	SetTexParameters ();

	if (nomips)
		glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, dest);
	else
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height,GL_RGBA, GL_UNSIGNED_BYTE, dest);

	free (dest);

	glBindTexture( GL_TEXTURE_2D, 0 );

    return q;
}

/*
===============
Texture_CreateSolid

Create a single pixel texture of the apropriate color
===============
*/
qtexture_t *Texture_CreateSolid (char *name)
{
	byte	data[4];
	qtexture_t	*q;

    q = qmalloc(sizeof(*q));
	
	sscanf (name, "(%f %f %f)", &q->color[0], &q->color[1], &q->color[2]);

	data[0] = q->color[0]*255;
	data[1] = q->color[1]*255;
	data[2] = q->color[2]*255;
	data[3] = 255;

	q->width = q->height = 1;
    q->texture_number = texture_extension_number++;
	glBindTexture( GL_TEXTURE_2D, q->texture_number );
	SetTexParameters ();

	q->scale_x = 1.0;
	q->scale_y = 1.0;

	if (nomips)
		glTexImage2D(GL_TEXTURE_2D, 0, 3, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	else
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, 1, 1,GL_RGBA, GL_UNSIGNED_BYTE, data);

	glBindTexture( GL_TEXTURE_2D, 0 );

	return q;
}


/*
=================
Texture_MakeNotexture
=================
*/
void Texture_MakeNotexture (void)
{
    qtexture_t	*q;
    byte		data[4][4];

	notexture = q = qmalloc(sizeof(*q));
	strcpy (q->name, "notexture");
    q->width = q->height = 64;
    
	memset (data, 0, sizeof(data));
	data[0][2] = data[3][2] = 255;

	q->color[0] = 0;
	q->color[1] = 0;
	q->color[2] = 0.5;

	q->scale_x = 1.0;
	q->scale_y = 1.0;

    q->texture_number = texture_extension_number++;
	glBindTexture( GL_TEXTURE_2D, q->texture_number );
	SetTexParameters ();

	if (nomips)
		glTexImage2D(GL_TEXTURE_2D, 0, 3, 2, 2, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	else
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, 2, 2,GL_RGBA, GL_UNSIGNED_BYTE, data);

	glBindTexture( GL_TEXTURE_2D, 0 );
}



/*
===============
Texture_ForName
===============
*/
qtexture_t *Texture_ForName (char *name)
{
    byte    *lump;
	qtexture_t	*q = NULL;
	char	filename[1024];

//return notexture;
	for (q=g_qeglobals.d_qtextures ; q ; q=q->next)
    {
		if (!strcmp(name,  q->name))
		{
			if (!g_dontuse)
				q->inuse = true;
		    return q;
		}
    }

	if (name[0] == '(')
	{
		q = Texture_CreateSolid (name);
		strncpy (q->name, name, sizeof(q->name)-1);
	}
	else
	{
		// load the file
		sprintf (filename, "%s/%s.m32", 
			ValueForKey (g_qeglobals.d_project_entity, "texturepath"),
			name);
		if (TryLoadFile (filename, &lump) == -1)
		{
			sprintf (filename, "%s/%s.m8", 
				ValueForKey (g_qeglobals.d_project_entity, "texturepath"),
				name);
			if (TryLoadFile (filename, &lump) == -1)
			{
				Sys_Printf ("     load failed for %s!\n", name);
				return notexture;
			}
			Sys_Printf ("Loading %s\n", name);
			q = Texture_LoadTexture ((miptex_t *)lump);
		}
		else
		{
			Sys_Printf ("Loading %s\n", name);
			q = Texture_LoadTexture32 ((miptex32_t *)lump);
		}
		free (lump);
		if (!q) 
		{
			return q;
		}
		strncpy (q->name, name, sizeof(q->name)-1);
		StripExtension (q->name);
	}

	if (!g_dontuse)
		q->inuse = true;
	q->next = g_qeglobals.d_qtextures;
	g_qeglobals.d_qtextures = q;

    return q;
}

/*
==================
FillTextureMenu

==================
*/
void FillTextureMenu (void)
{
	HMENU	hmenu;
	int		i;
	struct _finddata_t fileinfo;
	int		handle;
	char	dirstring[1024];
	char	*path;

	hmenu = GetSubMenu (GetMenu(g_qeglobals.d_hwndMain), MENU_TEXTURE);

	// delete everything
	for (i=0 ; i<texture_nummenus ; i++)
		DeleteMenu (hmenu, CMD_TEXTUREWAD+i, MF_BYCOMMAND);

	// add everything
	path = ValueForKey (g_qeglobals.d_project_entity, "texturepath");

	sprintf (dirstring, "%s/*.*", path);

	handle = _findfirst (dirstring, &fileinfo);
	if (handle == -1)
		return;

	do
	{
		if (!(fileinfo.attrib & _A_SUBDIR))
			continue;
		if (fileinfo.name[0] == '.')
			continue;
		// add this directory to the menu
		AppendMenu (hmenu, MF_ENABLED|MF_STRING,
			CMD_TEXTUREWAD+texture_nummenus, (LPCTSTR)fileinfo.name);
		strcpy (texture_menunames[texture_nummenus], fileinfo.name);
		strcat (texture_menunames[texture_nummenus], "/");
		if (++texture_nummenus == MAX_TEXTUREDIRS)
			break;
	} while (_findnext( handle, &fileinfo ) != -1);

	_findclose (handle);
}


/*
==================
Texture_ClearInuse

A new map is being loaded, so clear inuse markers
==================
*/
void Texture_ClearInuse (void)
{
	qtexture_t	*q;

	for (q=g_qeglobals.d_qtextures ; q ; q=q->next)
    {
		q->inuse = false;
	}
}



/*
==============
Texture_ShowDirectory
==============
*/
void	Texture_ShowDirectory (int menunum)
{
	struct _finddata_t fileinfo;
	int		handle;
	char	name[1024];
	char	dirstring[1024];

	texture_showinuse = false;
	strcpy (texture_directory, texture_menunames[menunum-CMD_TEXTUREWAD]);

	g_qeglobals.d_texturewin.originy = 0;
	Sys_Status("loading all textures\n", 0);

	// load all .wal files
	sprintf (dirstring, "%s/textures/%s*.m*", 
		ValueForKey (g_qeglobals.d_project_entity, "basepath"),
		texture_menunames[menunum-CMD_TEXTUREWAD]);

	Sys_Printf ("Scanning %s\n", dirstring);

	handle = _findfirst (dirstring, &fileinfo);
	if (handle == -1)
		return;

	g_dontuse = true;
	do
	{
		sprintf (name, "%s%s", texture_directory, fileinfo.name);
		StripExtension (name);
		Texture_ForName (name);
	} while (_findnext( handle, &fileinfo ) != -1);
	g_dontuse = false;

	_findclose (handle);

	SortTextures();
	SetInspectorMode(W_TEXTURE);
	Sys_UpdateWindows(W_TEXTURE);
	Texture_SetScrollRange();
	Texture_SetScrollPos();

	sprintf (name, "Textures: %s", texture_directory);
	SetWindowText(g_qeglobals.d_hwndEntity, name);

	// select the first texture in the list
	if (!g_qeglobals.d_texturewin.texdef.name[0])
		SelectTexture (16, g_qeglobals.d_texturewin.height -16);
}

/*
==============
Texture_ShowInuse
==============
*/
void	Texture_ShowInuse (void)
{
	char	name[1024];
	face_t	*f;
	brush_t	*b;

	texture_showinuse = true;

	g_qeglobals.d_texturewin.originy = 0;
	Sys_Status("Selecting active textures\n", 0);
	Texture_ClearInuse ();

	for (b=active_brushes.next ; b != NULL && b != &active_brushes ; b=b->next)
		for (f=b->brush_faces ; f ; f=f->next)
			Texture_ForName (f->texdef.name);

	for (b=selected_brushes.next ; b != NULL && b != &selected_brushes ; b=b->next)
		for (f=b->brush_faces ; f ; f=f->next)
			Texture_ForName (f->texdef.name);

	SortTextures();
	SetInspectorMode(W_TEXTURE);
	Sys_UpdateWindows (W_TEXTURE);
	Texture_SetScrollPos();

	sprintf (name, "Textures: in use");
	SetWindowText(g_qeglobals.d_hwndEntity, name);

	// select the first texture in the list
	if (!g_qeglobals.d_texturewin.texdef.name[0])
		SelectTexture (16, g_qeglobals.d_texturewin.height -16);
}

/*
===============
Texture_ListInUse
===============
*/

void Texture_ListInUse(void)
{
	brush_t*	b;
	face_t*		f;
	texlist_t*	texturelist; // the first texture name in the list
	texlist_t*  thistexture; // texture currently read in
	texlist_t*	curTexture;  // texture in the list that we're working with
	texlist_t*  prevTexture; // texture prior to curTexture


	texturelist = NULL;

	SetInspectorMode(W_CONSOLE);
	Sys_ClearPrintf();
	Sys_Printf ("Textures in use:\n");

	for (b=active_brushes.next ; b != NULL && b != &active_brushes ; b=b->next)
	{
		for (f=b->brush_faces ; f ; f=f->next)
		{
			if (f->texdef.name[0] != '(') // we only want texture names, not entities
			{
				thistexture = malloc( sizeof(texlist_t));
				if (thistexture == NULL)
				{
					Error("Not enough memory");
					return;
				}
				thistexture->uses = 0;
				thistexture->name[0] = '\0';
				thistexture->prev = NULL;
				thistexture->next = NULL;
				strcpy(thistexture->name, f->texdef.name);

				if (texturelist == NULL) 
				{
					thistexture->uses++;
					texturelist = thistexture; //put first member on list
				}
				else
				{
					curTexture = texturelist; // grab the list
					prevTexture = curTexture->prev; 
					while (1)
					{
						if (curTexture->name == NULL) // end of list
						{
							thistexture->uses++;
							thistexture->prev = prevTexture; // add texture on
							prevTexture->next = thistexture;
							break;
						}
						if (strcmp(thistexture->name,curTexture->name) == 0) // texture already on list
						{
							curTexture->uses++;
							free (thistexture);
							break;
						}
						if (strcmp(thistexture->name,curTexture->name) < 0) // texture belongs ahead of this one
						{
							if (prevTexture == NULL) // texture goes at front of list
							{
								thistexture->uses++;
								thistexture->next = curTexture;
								curTexture->prev = thistexture;
								texturelist = thistexture; // list now starts here
							}
							else
							{
								thistexture->uses++;
								thistexture->prev = curTexture->prev;
								thistexture->next = curTexture;
								curTexture->prev = thistexture;
								prevTexture->next = thistexture;
							}
							break;
						}
						prevTexture = curTexture;
						curTexture = curTexture->next; // advance to next texture in list
					}
				}
			} // texture name
		} // face
	} // brush

	for (b=selected_brushes.next ; b != NULL && b != &selected_brushes ; b=b->next)
	{
		for (f=b->brush_faces ; f ; f=f->next)
		{
			if (f->texdef.name[0] != '(') // we only want texture names, not entities
			{
				thistexture = malloc( sizeof(texlist_t));
				if (thistexture == NULL)
				{
					Error("Not enough memory");
					return;
				}
				thistexture->uses = 0;
				thistexture->name[0] = '\0';
				thistexture->prev = NULL;
				thistexture->next = NULL;
				strcpy(thistexture->name, f->texdef.name);

				if (texturelist == NULL) 
				{
					thistexture->uses++;
					texturelist = thistexture; //put first member on list
				}
				else
				{
					curTexture = texturelist; // grab the list
					prevTexture = curTexture->prev; 
					while (1)
					{
						if (curTexture->name == NULL) // end of list
						{
							thistexture->uses++;
							thistexture->prev = prevTexture; // add texture on
							prevTexture->next = thistexture;
							break;
						}
						if (strcmp(thistexture->name,curTexture->name) == 0) // texture already on list
						{
							curTexture->uses++;
							free (thistexture);
							break;
						}
						if (strcmp(thistexture->name,curTexture->name) < 0) // texture belongs ahead of this one
						{
							if (prevTexture == NULL) // texture goes at front of list
							{
								thistexture->uses++;
								thistexture->next = curTexture;
								curTexture->prev = thistexture;
								texturelist = thistexture; // list now starts here
							}
							else
							{
								thistexture->uses++;
								thistexture->prev = curTexture->prev;
								thistexture->next = curTexture;
								curTexture->prev = thistexture;
								prevTexture->next = thistexture;
							}
							break;
						}
						prevTexture = curTexture;
						curTexture = curTexture->next; // advance to next texture in list
					}
				}
			} // texture name
		} // face
	} // brush

	for (curTexture = texturelist; curTexture; curTexture=curTexture->next)
	{
		Sys_Printf("%s, Used on %d faces\n",curTexture->name, curTexture->uses);
	}

	while (texturelist != NULL)
	{
		curTexture = texturelist;
		texturelist = curTexture->next;
		free (curTexture);
	}
}


/*
==============
Texture_Replace
==============
*/

// Start with world_enitity


void Texture_Replace(char *from, char *to, int replaceAll)
{
	brush_t	*b, *startBrush;
	face_t	*f, *startFace;
	qtexture_t	*curTex;
	int			foundTex = 0;
	int			i;

	curTex = g_qeglobals.d_qtextures;

	while(curTex && !foundTex)
	{
		if(!strcmpi(to, curTex->name))
		{	// found it...
			foundTex = 1;
		}
		else
		{
			curTex = curTex->next;
		}
	}

	if(!foundTex)
	{	// looks like the texture entered was invalid
		return;
	}

	if(replaceAll)
	{
		b = world_entity->brushes.onext;
	}
	else
	{
		b = selected_brushes.next;
	}

	startBrush = b;

	while(b != NULL)
	{
		// Search through the face linked list
		f = b->brush_faces;

		startFace = f;

		while(f != NULL)
		{
			// check the stuff
			if (!strcmpi(from, f->texdef.name))
			{
				// replace the things

				memset (&f->texdef, 0, sizeof(f->texdef));
				f->texdef.scale[0] = curTex->scale_x;
				f->texdef.scale[1] = curTex->scale_y;
				f->texdef.flags = curTex->flags;
				f->texdef.value = curTex->value;
				f->texdef.contents = curTex->contents;
				f->texdef.lighting[0] = 1.0;
				f->texdef.lighting[1] = 1.0;
				f->texdef.lighting[2] = 1.0;
				f->texdef.lighting[3] = 1.0;
				strcpy (f->texdef.name, curTex->name);
			}

			f = f->next;

			if(f == startFace)
			{
				f = NULL;
			}
		}

		if(replaceAll)
		{
			b = b->onext;
		}
		else
		{
			b = b->next;
		}
		if(b == startBrush)
		{
			b = NULL; // hit the end of the circular list
		}
	}

	// This is really pretty hacky.  This ensures that the camera view is properly updated....
	// Otherwise the textures don't get update (?)
	if(camera.draw_mode == cd_texture)
	{
		camera.draw_mode = cd_solid;
		Map_BuildBrushData(false);
		Sys_UpdateWindows (W_ALL);

		for (i=1 ; i<texture_extension_number ; i++)
		{
			glBindTexture( GL_TEXTURE_2D, i );
			SetTexParameters ();
		}

		// select the default texture
		glBindTexture( GL_TEXTURE_2D, 0 );

		glFinish();

		if (camera.draw_mode != cd_texture)
		{
			camera.draw_mode = cd_texture;
			Map_BuildBrushData(false);
		}

		Sys_UpdateWindows (W_ALL);
	}
}

/*
============================================================================

TEXTURE LAYOUT

============================================================================
*/

void Texture_StartPos (void)
{
	current_texture = g_qeglobals.d_qtextures;
	current_x = 8;
	current_y = -8;
	current_row = 0;
}

qtexture_t *Texture_NextPos (int *x, int *y)
{
	qtexture_t	*q;

	while (1)
	{
		q = current_texture;
		if (!q)
			return q;
		current_texture = current_texture->next;
		if (q->name[0] == '(')	// fake color texture
			continue;
		if (q->inuse)
			break;			// allways show in use
		if (!texture_showinuse && strncmp (q->name, texture_directory, strlen(texture_directory)))
			continue;
		break;
	}

	if (current_x + q->width > g_qeglobals.d_texturewin.width-8 && current_row)
	{	// go to the next row unless the texture is the first on the row
		current_x = 8;
		current_y -= current_row + FONT_HEIGHT + 4;
		current_row = 0;
	}

	*x = current_x;
	*y = current_y;

	// Is our texture larger than the row? If so, grow the 
	// row height to match it

    if (current_row < q->height)
		current_row = q->height;

	// never go less than 64, or the names get all crunched up
	current_x += q->width < 64 ? 64 : q->width;
	current_x += 8;

	return q;
}


/*
============================================================================

  MOUSE ACTIONS

============================================================================
*/

static	int	textures_cursorx, textures_cursory;


/*
============
Texture_SetTexture

============
*/
void Texture_SetTexture (texdef_t *texdef)
{
	qtexture_t	*q;
	int			x,y;
	char		sz[256];

	if (texdef->name[0] == '(')
	{
		Sys_Status("Can't select an entity texture\n", 0);
		return;
	}
	g_qeglobals.d_texturewin.prevTexDef = g_qeglobals.d_texturewin.texdef;
	g_qeglobals.d_texturewin.texdef = *texdef;

	Sys_UpdateWindows (W_TEXTURE);
	Texture_SetScrollPos();
	sprintf(sz, "Selected texture: %s\n", texdef->name);
	Sys_Status(sz, 0);
	Select_SetTexture(texdef);

// scroll origin so the texture is completely on screen
	Texture_StartPos ();
	while (1)
	{
		q = Texture_NextPos (&x, &y);
		if (!q)
			break;
		if (!strcmpi(texdef->name, q->name))
		{
			if (y > g_qeglobals.d_texturewin.originy)
			{
				g_qeglobals.d_texturewin.originy = y;
				Sys_UpdateWindows (W_TEXTURE);
				Texture_SetScrollPos();
				return;
			}

			if (y-q->height-2*FONT_HEIGHT < g_qeglobals.d_texturewin.originy-g_qeglobals.d_texturewin.height)
			{
				g_qeglobals.d_texturewin.originy = y-q->height-2*FONT_HEIGHT+g_qeglobals.d_texturewin.height;
				Sys_UpdateWindows (W_TEXTURE);
				Texture_SetScrollPos();
				return;
			}

			return;
		}
	}
}


/*
==============
SelectTexture

  By mouse click
==============
*/
void SelectTexture (int mx, int my)
{
	int		x, y;
	qtexture_t	*q;
	texdef_t	tex;

	my += g_qeglobals.d_texturewin.originy-g_qeglobals.d_texturewin.height;
	
	Texture_StartPos ();
	while (1)
	{
		q = Texture_NextPos (&x, &y);
		if (!q)
			break;
		if (mx > x && mx - x < q->width
			&& my < y && y - my < q->height + FONT_HEIGHT)
		{
			memset (&tex, 0, sizeof(tex));
			tex.scale[0] = q->scale_x;
			tex.scale[1] = q->scale_y;
			tex.flags = q->flags;
			tex.value = q->value;
			tex.contents = q->contents;
			strcpy (tex.name, q->name);
			Texture_SetTexture (&tex);
			return;
		}
	}

	Sys_Status("Did not select a texture\n", 0);
}

/*
==============
Texture_MouseDown
==============
*/
void Texture_MouseDown (int x, int y, int buttons)
{
	Sys_GetCursorPos (&textures_cursorx, &textures_cursory);

	// lbutton = select texture
	if (buttons == MK_LBUTTON )
	{
		SelectTexture (x, g_qeglobals.d_texturewin.height - 1 - y);
		return;
	}

}

/*
==============
Texture_MouseUp
==============
*/
void Texture_MouseUp (int x, int y, int buttons)
{
}

/*
==============
Texture_MouseMoved
==============
*/
void Texture_MouseMoved (int x, int y, int buttons)
{
	int scale = 1;

	if ( buttons & MK_SHIFT )
		scale = 4;

	// rbutton = drag texture origin
	if (buttons & MK_RBUTTON)
	{
		Sys_GetCursorPos (&x, &y);
		if ( y != textures_cursory)
		{
			Texture_ScrollLine(( y-textures_cursory) * scale);
			Sys_SetCursorPos (textures_cursorx, textures_cursory);

		}
		return;
	}
}


/*
============================================================================

DRAWING

============================================================================
*/

int imax(int iFloor, int i) { if (i>iFloor) return iFloor; return i; }
HFONT ghFont = NULL;

/*
============
Texture_Draw2
============
*/
void Texture_Draw2 (int width, int height)
{
	qtexture_t	*q;
	int			x, y;
	char		*name;

	glClearColor (
		g_qeglobals.d_savedinfo.colors[COLOR_TEXTUREBACK][0],
		g_qeglobals.d_savedinfo.colors[COLOR_TEXTUREBACK][1],
		g_qeglobals.d_savedinfo.colors[COLOR_TEXTUREBACK][2],
		0);
	glViewport (0,0,width,height);
	glClear (GL_COLOR_BUFFER_BIT);
	glDisable (GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity ();
	glOrtho (0, width, g_qeglobals.d_texturewin.originy-height, g_qeglobals.d_texturewin.originy, -100, 100);
	glEnable (GL_TEXTURE_2D);

	glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
	g_qeglobals.d_texturewin.width = width;
	g_qeglobals.d_texturewin.height = height;
	Texture_StartPos ();

	while (1)
	{
		q = Texture_NextPos (&x, &y);
		if (!q)
			break;

		// Is this texture visible?
		if ( (y-q->height-FONT_HEIGHT < g_qeglobals.d_texturewin.originy)
			&& (y > g_qeglobals.d_texturewin.originy - height) )
		{

			// if in use, draw a background
			if (q->inuse && !texture_showinuse)
			{
				glLineWidth (1);
				glColor3f (0.5,1,0.5);
				glDisable (GL_TEXTURE_2D);

				glBegin (GL_LINE_LOOP);
				glVertex2f (x-1,y+1-FONT_HEIGHT);
				glVertex2f (x-1,y-q->height-1-FONT_HEIGHT);
				glVertex2f (x+1+q->width,y-q->height-1-FONT_HEIGHT);
				glVertex2f (x+1+q->width,y+1-FONT_HEIGHT);
				glEnd ();

				glEnable (GL_TEXTURE_2D);
			}

			// Draw the texture
			glColor3f (1,1,1);
			glBindTexture( GL_TEXTURE_2D, q->texture_number );
			glBegin (GL_QUADS);
			glTexCoord2f (0,0);
			glVertex2f (x,y-FONT_HEIGHT);
			glTexCoord2f (1,0);
			glVertex2f (x+q->width,y-FONT_HEIGHT);
			glTexCoord2f (1,1);
			glVertex2f (x+q->width,y-FONT_HEIGHT-q->height);
			glTexCoord2f (0,1);
			glVertex2f (x,y-FONT_HEIGHT-q->height);
			glEnd ();

			// draw the selection border
			if (!strcmpi(g_qeglobals.d_texturewin.texdef.name, q->name))
			{
				glLineWidth (3);
				glColor3f (1,0,0);
				glDisable (GL_TEXTURE_2D);

				glBegin (GL_LINE_LOOP);
				glVertex2f (x-4,y-FONT_HEIGHT+4);
				glVertex2f (x-4,y-FONT_HEIGHT-q->height-4);
				glVertex2f (x+4+q->width,y-FONT_HEIGHT-q->height-4);
				glVertex2f (x+4+q->width,y-FONT_HEIGHT+4);
				glEnd ();

				glEnable (GL_TEXTURE_2D);
				glLineWidth (1);
			}

			// draw the texture name
			glColor3f (0,0,0);
			glRasterPos2f (x, y-FONT_HEIGHT+2);

			// don't draw the directory name
			for (name = q->name ; *name && *name != '/' && *name != '\\' ; name++)
				;
			if (!*name)
				name = q->name;
			else
				name++;
			glCallLists (strlen(name), GL_UNSIGNED_BYTE, name);
		}
	}

	// reset the current texture
	glBindTexture( GL_TEXTURE_2D, 0 );
	glFinish();
}

/*
============
WTexWndProc
============
*/
LONG WINAPI WTex_WndProc (
    HWND    hWnd,
    UINT    uMsg,
    WPARAM  wParam,
    LPARAM  lParam)
{
	int		xPos, yPos;
    RECT	rect;

    GetClientRect(hWnd, &rect);

    switch (uMsg)
    {
	case WM_CREATE:
        s_hdcTexture = GetDC(hWnd);
		QEW_SetupPixelFormat(s_hdcTexture, false);

		if ( ( s_hglrcTexture = wglCreateContext( s_hdcTexture ) ) == 0 )
			Error( "wglCreateContext in WTex_WndProc failed" );

        if (!wglMakeCurrent( s_hdcTexture, s_hglrcTexture ))
			Error ("wglMakeCurrent in WTex_WndProc failed");

		if (!wglShareLists( g_qeglobals.d_hglrcBase, s_hglrcTexture ) )
			Error( "wglShareLists in WTex_WndProc failed" );

		return 0;

	case WM_DESTROY:
		wglMakeCurrent( NULL, NULL );
		wglDeleteContext( s_hglrcTexture );
		ReleaseDC( hWnd, s_hdcTexture );
		return 0;

	case WM_PAINT:
        { 
//		    PAINTSTRUCT	ps;

//		    BeginPaint(hWnd, &ps);

            if ( !wglMakeCurrent( s_hdcTexture, s_hglrcTexture ) )
				Error ("wglMakeCurrent failed");
			Texture_Draw2 (rect.right-rect.left, rect.bottom-rect.top);
			SwapBuffers(s_hdcTexture);

//		    EndPaint(hWnd, &ps);
			ValidateRect(hWnd, NULL);
        }
		return 0;

	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_LBUTTONDOWN:
		SetCapture( g_qeglobals.d_hwndTexture );
		xPos = (short)LOWORD(lParam);  // horizontal position of cursor 
		yPos = (short)HIWORD(lParam);  // vertical position of cursor 
		
		Texture_MouseDown (xPos, yPos, wParam);
		return 0;

	case WM_MBUTTONUP:
	case WM_RBUTTONUP:
	case WM_LBUTTONUP:
		xPos = (short)LOWORD(lParam);  // horizontal position of cursor 
		yPos = (short)HIWORD(lParam);  // vertical position of cursor 
		
		Texture_MouseUp (xPos, yPos, wParam);
		if (! (wParam & (MK_LBUTTON|MK_RBUTTON|MK_MBUTTON)))
			ReleaseCapture ();
		return 0;

	case WM_MOUSEMOVE:
		xPos = (short)LOWORD(lParam);  // horizontal position of cursor 
		yPos = (short)HIWORD(lParam);  // vertical position of cursor 
		
		Texture_MouseMoved (xPos, yPos, wParam);
		return 0;

	case WM_SIZE:
		//i'm not worried about messing this up, since i'll get a paint message soon anyhow
		g_qeglobals.d_texturewin.width = rect.right-rect.left;
		g_qeglobals.d_texturewin.height = rect.bottom-rect.top;
		Texture_SetScrollRange();//this is inefficient way of updating scrollbar?
		return 0;

	case WM_VSCROLL:
		switch(LOWORD(wParam))
		{
		case SB_LINEUP:
			Texture_ScrollRow(1);
			break;
		case SB_LINEDOWN:
			Texture_ScrollRow(-1);
			break;
		case SB_PAGEUP:
			Texture_ScrollPage(1);
			break;
		case SB_PAGEDOWN:
			Texture_ScrollPage(-1);
			break;
		case SB_THUMBTRACK:
			Texture_ScrollTo (-HIWORD(wParam));
			break;
		case SB_THUMBPOSITION:
			Texture_ScrollTo (-HIWORD(wParam));
			break;
		default:
			break;
		}

		return 0;

	case WM_LBUTTONDBLCLK:
		PostMessage (g_qeglobals.d_hwndMain, WM_COMMAND, ID_TEXTURES_INSPECTOR, 0);
		return 0;
	case WM_MOUSEWHEEL:
//		Sys_Printf ("got a wheel!\n");
/*		gcWheelDelta -= (short) HIWORD(wParam);    
		if (abs(gcWheelDelta) >= 
			WHEEL_DELTA && gucWheelScrollLines > 0) 
		{
			texturewin.originy += gucWheelScrollLines*(gcWheelDelta / WHEEL_DELTA);
			if (texturewin.originy > 0)
				texturewin.originy = 0;
			Sys_UpdateWindows (W_TEXTURE);
		}
		break;
		*/
		Texture_ScrollRow(HIWORD(wParam) / WHEEL_DELTA);
		return 0;

	default:
		if (uMsg == g_qeglobals.uMSH_MOUSEWHEEL)
		{
			Texture_ScrollRow(*(int *)&wParam / WHEEL_DELTA);
			return 0;
		}
		break;


    }

    return DefWindowProc (hWnd, uMsg, wParam, lParam);
}



/*
==================
CreateTextureWindow

We need to create a seperate window for the textures
in the inspector window, because we can't share
gl and gdi drawing in a single window
==================
*/
#define	TEXTURE_WINDOW_CLASS	"QTEX"

extern BOOL CALLBACK TexModifyProc (
    HWND hwndDlg,	// handle to dialog box
    UINT uMsg,	// message
    WPARAM wParam,	// first message parameter
    LPARAM lParam 	// second message parameter
   );

HWND CreateTextureWindow (void)
{
    WNDCLASS   wc;
	HWND		hwnd;

    /* Register the camera class */
	memset (&wc, 0, sizeof(wc));

    wc.style         = CS_OWNDC|CS_DBLCLKS;
    wc.lpfnWndProc   = (WNDPROC)WTex_WndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = g_qeglobals.d_hInstance;
    wc.hIcon         = 0;
    wc.hCursor       = LoadCursor (NULL,IDC_ARROW);
    wc.hbrBackground = NULL;
    wc.lpszMenuName  = 0;
    wc.lpszClassName = TEXTURE_WINDOW_CLASS;

    if (!RegisterClass (&wc) )
        Error ("WCam_Register: failed");

	hwnd = CreateWindow (TEXTURE_WINDOW_CLASS ,
		"Texture View",
		WS_BORDER|WS_CHILD|WS_VISIBLE|WS_VSCROLL,
		20,
		20,
		64,
		64,	// size

		g_qeglobals.d_hwndEntity,	// parent window
		0,		// no menu
		g_qeglobals.d_hInstance,
		0);
	if (!hwnd)
		Error ("Couldn't create texturewindow");

	g_qeglobals.d_hwndTexModDlg = CreateDialog(g_qeglobals.d_hInstance, MAKEINTRESOURCE(IDD_TEXMODIFY),
			g_qeglobals.d_hwndEntity, (DLGPROC)TexModifyProc);		
	return hwnd;
}

/*
==================
Texture_Flush
==================
*/
void Texture_Flush (void)
{
}


/*
==================
Texture_Init
==================
*/
void Texture_Init (void)
{
//	char	name[1024];
//	byte	*pal;

	// load the palette
//	sprintf (name, "%s/pics/colormap.pcx", 
//		ValueForKey (g_qeglobals.d_project_entity, "basepath"));
//	Load256Image (name, NULL, &pal, NULL, NULL);
//	if (!pal)
//		Error ("Couldn't load %s", name);
//	Texture_InitPalette (pal);
//	free (pal);

	// create the fallback texture
	Texture_MakeNotexture ();

	g_qeglobals.d_qtextures = NULL;
}


/*
==================
Texture_DefaultMaterial
==================
*/

void Texture_DefaultMaterial(void)
{
	qtexture_t		*curTex;
	brush_t			*b, *startBrush;
	face_t			*f, *startFace;

	b = world_entity->brushes.onext;
	startBrush = b;

	while (b != NULL)
	{
		f = b->brush_faces;
		startFace = f;

		while (f != NULL)
		{
			curTex = g_qeglobals.d_qtextures;
			while(curTex)
			{
				if (!strcmpi(curTex->name, f->texdef.name))
				{
					f->texdef.flags = ((f->texdef.flags & 0x00FFFFFF) | (curTex->flags & 0x0FF000000));
					break;
				}

				curTex = curTex->next;
			}

			f = f->next;
			if (f == startFace)
			{
				f = NULL;
			}
		}

		b = b->onext;
		if (b == startBrush)
		{
			b = NULL;
		}
	}
}