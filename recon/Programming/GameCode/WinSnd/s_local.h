// s_local.h
//

#pragma once

extern "C"
{
#include "../game/q_shared.h"
#include "../qcommon/qcommon.h"
};

#include <vector>

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>
#include <AL/efx.h>
#include <AL/efx-presets.h>

//
// sfx_t
//
struct sfx_t
{
	sfx_t() {
		memset(this, 0, sizeof(sfx_t));
	};

	char soundName[512];
	bool isLoaded;

	ALuint buffer;
};

struct sndGlobals_t {
	ALCdevice *device;
	ALCcontext *context;

	ALuint		voices[32];
	ALuint		music_voice;

	ALuint		reverb_effect;
	ALuint		reverb_aux_slot;

	vec3_t		prev_origin;

	std::vector<sfx_t *> sfxList;
};

#define     SOUND_FULLVOLUME 1.0

extern byte s_currentReverbAmount;

extern sndGlobals_t sndGlobal;

extern "C" {
	ALuint S_LoadReverbEffect(void);
	void SNDEAX_SetEnvironment(int id);
	bool S_Init(void);
	sfx_t* S_FindName(char* name, qboolean create);
	sfx_t* S_RegisterSound(char* name);
	void S_Activate(bool active);
	void S_BeginRegistration(void);
	void S_EndRegistration(void);
	void S_Shutdown(void);
	ALuint S_FindFreeVoice(void);
	void S_PlayNoPositionSound(ALuint voice, sfx_t* localSound);
	void S_StartLocalSound(char* sound);
	void S_StartSound(vec3_t origin, int entnum, int entchannel, sfx_t* sfx, float fvol, int attenuation, float timeofs);
	void S_StopAllSounds(void);
	void S_StopAllSounds_Sounding(void);
	void S_Update(vec3_t quake_origin, vec3_t forward, vec3_t right, vec3_t up);
	void S_PlayMusic(int track, int looping);
};