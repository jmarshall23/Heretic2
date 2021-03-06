// entity.h

#define DlgXBorder 5
#define DlgYBorder 5


#define EntList		0
#define EntComment	1
#define EntCheck1	2
#define EntCheck2	3
#define EntCheck3	4
#define EntCheck4	5
#define EntCheck5	6
#define EntCheck6	7
#define EntCheck7	8
#define EntCheck8	9
#define EntCheck9	10
#define EntCheck10	11
#define EntCheck11	12
#define EntCheck12	13

#define EntCheck13	14
#define EntCheck14	15
#define EntCheck15	16
#define EntCheck16	17
#define EntCheck17	18
#define EntCheck18	19
#define EntCheck19	20
#define EntCheck20	21
#define EntCheck21	22

#define EntProps	23
#define	EntDir0		24
#define	EntDir45	25
#define	EntDir90	26
#define	EntDir135	27
#define	EntDir180	28
#define	EntDir225	29
#define	EntDir270	30
#define	EntDir315	31
#define	EntDirUp	32
#define	EntDirDown	33
#define EntDelProp	34
#define	EntKeyLabel	35
#define	EntKeyField	36
#define	EntValueLabel 37
#define	EntValueField 38
#define EntColor      39

#define EntLast		40

#define NUM_SPAWNFLAGS			21
#define LAST_LOW_SPAWNFLAG		8
#define FIRST_HIGH_SPAWNFLAG	13
#define LAST_HIGH_SPAWNFLAG		21

extern HWND hwndEnt[EntLast];

extern int rgIds[EntLast];

int GetEntityColor(int iIndex);