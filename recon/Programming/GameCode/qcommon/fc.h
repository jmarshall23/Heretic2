// Definitions required for the flexible cinematic format

#define FC_HEADER_NAME		"cinematic header"
#define FC_HEADER_VERSION	5

#define FC_HUFFBITS_NAME	"cinematic huffbits"
#define FC_HUFFBITS_VERSION	1

#define FC_QUANT_NAME		"cinematic quantisation"
#define FC_QUANT_VERSION	1

#define FC_IMAGE_NAME		"cinematic image"
#define FC_IMAGE_VERSION	1

#define FC_SOUND_11K8M		"sound 11k 8bit mono"
#define FC_SOUND_22K8M		"sound 22k 8bit mono"
#define FC_SOUND_44K8M		"sound 44k 8bit mono"

#define FC_SOUND_11K16M		"sound 11k 16bit mono"
#define FC_SOUND_22K16M		"sound 22k 16bit mono"
#define FC_SOUND_44K16M		"sound 44k 16bit mono"

#define FC_SOUND_VERSION	1

#define FC_SOUND_22KMADPCM	"sound 22k ADPCM mono"
#define FC_ADPCM_VERSION	2

typedef struct CineHead_s
{
	int			NumFrames;
	int			Width;
	int			Height;
	int			SndRate;
	int			SndWidth;
	int			SndChannels;
	int			MaxRleSize;
	int			MaxSndSize;
}	CineHead_t;

// end