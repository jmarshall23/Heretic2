// Some useful macros for coding assembler 

#define	cpuid __asm _emit 0fh __asm _emit 0a2h
#define rdtsc __asm _emit 0fh __asm _emit 031h

#define pushall __asm push ebx __asm push ecx __asm push edx __asm push esi __asm push edi __asm push ebp
#define popall __asm pop ebp __asm pop edi __asm pop esi __asm pop edx __asm pop ecx __asm pop ebx

typedef struct int64_s
{
	unsigned int	lo;
	unsigned int	hi;
}	int64_t;

union int64_u
{
	unsigned __int64	full;
	int64_t				half;
};

// end