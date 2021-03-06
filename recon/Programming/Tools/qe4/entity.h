
// entity.h
#define ENTKEY_COLOR	"_color"
#define ENTKEY_COLOR_NOUNDERSCORE	"color"


#define	MAX_FLAGS	17

typedef struct eclass_s
{
	struct eclass_s *next;
	char	*name;
	qboolean	fixedsize;
	qboolean	PhysicsModel;	//new pseudo bmodel type
	qboolean	unknown;		// wasn't found in source
	vec3_t	mins, maxs;
	vec3_t	color;
	texdef_t	texdef;
	char	*comments;
	char	flagnames[MAX_FLAGS][32];
} eclass_t;

extern	eclass_t	*eclass;

void Eclass_InitForSourceDirectory (char *path);
eclass_t *Eclass_ForName (char *name, qboolean has_brushes);

//===================================================


typedef struct epair_s
{
	struct epair_s	*next;
	char	*key;
	char	*value;
} epair_t;

typedef struct entity_s
{
	struct	entity_s	*prev, *next;
	brush_t		brushes;	// head/tail of list
	vec3_t		origin;
	eclass_t	*eclass;
	epair_t		*epairs;
} entity_t;

typedef struct entlist_s
{
	char				name[64];
	int					uses;
	struct entlist_s*	prev;
	struct entlist_s*	next;
} entlist_t;

BOOL	KeyExists (entity_t *ent, char *key);
char 	*ValueForKey (entity_t *ent, char *key);
void	SetKeyValue (entity_t *ent, char *key, char *value);
void 	DeleteKey (entity_t *ent, char *key);
float	FloatForKey (entity_t *ent, char *key);
int		IntForKey (entity_t *ent, char *key);
void 	GetVectorForKey (entity_t *ent, char *key, vec3_t vec);

void		Entity_Free (entity_t *e);
entity_t	*Entity_Parse (qboolean onlypairs);
void		Entity_Write (entity_t *e, FILE *f, qboolean use_region);
entity_t	*Entity_Create (eclass_t *c);
entity_t	*Entity_Clone (entity_t *e);

void		Entity_LinkBrush (entity_t *e, brush_t *b);
void		Entity_UnlinkBrush (brush_t *b);
entity_t	*FindEntity(char *pszKey, char *pszValue);
entity_t	 *FindEntityInt(char *pszKey, int iValue);
void		Entity_ListInUse (void);
void		PrintConsole(void);
void		GetPhysicsModelBrushes(vec3_t mins, entity_t *e);
void		Physmod_Parse ( entity_t *e, vec3_t mins);

int GetUniqueTargetId(int iHint);

