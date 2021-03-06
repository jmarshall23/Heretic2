// Generic flexible format 

typedef struct
{
	char	ident[32];
	int		version;
	int		size;
} header_t;

void WriteHeader(FILE *, char *, int, int, void *);

// end