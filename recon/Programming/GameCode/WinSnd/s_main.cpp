// s_main.cpp
//

#include "s_local.h"

sndGlobals_t sndGlobal;

cvar_t *s_musicVolume;

/*
=================
S_Init
=================
*/

bool S_Init(void)
{
	Com_Printf("-------- S_Init -------\n");

	// Open the OpenAL device
	Com_Printf("Opening OpenAL Device...\n");
	sndGlobal.device = alcOpenDevice(NULL);
	if (!sndGlobal.device) {
		Com_Error(ERR_FATAL, "Failed to open OpenAL device...\n");
		return false;
	}

	// Set our OpenAL device context.
	sndGlobal.context = alcCreateContext(sndGlobal.device, NULL);
	if (!alcMakeContextCurrent(sndGlobal.context)) {
		Com_Error(ERR_FATAL, "Failed to OpenAL context!");
		return false;
	}

	if (!alcIsExtensionPresent(alcGetContextsDevice(alcGetCurrentContext()), "ALC_EXT_EFX")) {
		Com_Error(ERR_FATAL, "EFX Extension not present\n");
		return false;
	}

	// Create the voices.
	alGenSources(32, &sndGlobal.voices[0]);

	// Create the music voice
	alGenSources(1, &sndGlobal.music_voice);

	// Load the Cvars
	s_musicVolume = Cvar_Get("s_musicVolume", "50", CVAR_ARCHIVE);

	// Load the reverb effect.
	sndGlobal.reverb_effect = S_LoadReverbEffect();

	return true;
}

/*
=================
S_FindName
=================
*/

sfx_t *S_FindName(char *name, qboolean create) {
	ALenum error;
	char sndName[512];

	// Check to see if the sound is already loaded.
	for(int i = 0; i < sndGlobal.sfxList.size(); i++)
	{
		if(!strcmp(sndGlobal.sfxList[i]->soundName, name))
		{
			return sndGlobal.sfxList[i];
		}
	}

	// Load in the file from the disk.
	void *wavFileBuffer = nullptr;
	if (strstr(name, "music/") == nullptr)
	{
		if (name[0] == '*')
		{
			sprintf(sndName, "sound/player/%s", name + 1);
		}
		else
		{
			sprintf(sndName, "sound/%s", name);
		}
	}
	else
	{
		sprintf(sndName, "%s", name);
	}
	int wavFileSize = FS_LoadFile(sndName, &wavFileBuffer);
	if(wavFileBuffer == nullptr || wavFileSize <= 0) {
		Com_Printf("S_FindName: Failed to find sfx %s\n", name);
		return nullptr;
	}

	sfx_t *sfx = new sfx_t();

	// Load in the wav file.
	ALsizei size, freq;
	ALenum format;
	ALvoid *data;
	ALboolean loop = AL_FALSE;

	// Load Wav file into memory.
	alutLoadWAVMemory((ALbyte *)wavFileBuffer, &format, &data, &size, &freq, &loop);
	
	// Generate the buffer data.
	alGenBuffers(1, &sfx->buffer);
	alBufferData(sfx->buffer, format, data, size, freq);
	error = alGetError();
	if (error != AL_NO_ERROR)
	{
		Com_Error(ERR_FATAL, "S_FindName: Failed to generate buffer data!\n");
	}

	strcpy(sfx->soundName, name);

	FS_FreeFile(wavFileBuffer);

	sndGlobal.sfxList.push_back(sfx);

	return sfx;
}

sfx_t *S_RegisterSound(char *name)
{
	return S_FindName(name, true);
}

void S_Activate(bool active)
{

}

void S_BeginRegistration(void)
{

}

void S_EndRegistration(void)
{

}

void S_Shutdown(void)
{

}

ALuint S_FindFreeVoice(void) {
	ALuint voice = -1;

	// Find a free source buffer.
	for (int i = 0; i < 32; i++)
	{
		ALint source_state;
		alGetSourcei(sndGlobal.voices[i], AL_SOURCE_STATE, &source_state);

		if (source_state != AL_PLAYING)
		{
			voice = sndGlobal.voices[i];
			break;
		}
	}

	return voice;
}

void S_PlayNoPositionSound(ALuint voice, sfx_t *localSound) {
	if (localSound == nullptr)
	{
		return;
	}

	alSourcei(voice, AL_BUFFER, localSound->buffer);
	alSource3i(voice, AL_AUXILIARY_SEND_FILTER, (ALint)sndGlobal.reverb_aux_slot, 0, AL_FILTER_NULL);
	alSourcei(voice, AL_SOURCE_RELATIVE, AL_FALSE);
	alSourcef(voice, AL_GAIN, 1);
	alSourcePlay(voice);
}

void S_StartLocalSound(char *sound)
{
	ALuint voice = S_FindFreeVoice();

	// TODO: Evict old voices?
	if (voice == -1)
		return;

	sfx_t *localSound = S_FindName(sound, true);
	S_PlayNoPositionSound(voice, localSound);
}

void S_StartSound(vec3_t origin, int entnum, int entchannel, sfx_t *sfx, float fvol, int attenuation, float timeofs)
{
	if (sfx == nullptr)
		return;

	ALuint voice = S_FindFreeVoice();
	if(origin == nullptr) {
		S_PlayNoPositionSound(voice, sfx);
		return;
	}

	alSourcei(voice, AL_BUFFER, sfx->buffer);
	alSourcef(voice, AL_GAIN, fvol);
	alSourcef(voice, AL_REFERENCE_DISTANCE, attenuation);

	// https://github.com/yquake2/yquake2/blob/master/src/client/sound/openal.c
	alSourcef(voice, AL_REFERENCE_DISTANCE, SOUND_FULLVOLUME);
	alSourcef(voice, AL_MAX_DISTANCE, 8192);
	alSourcef(voice, AL_ROLLOFF_FACTOR, (attenuation * 0.001f) * (8192 - SOUND_FULLVOLUME) );
	
	alSourcef(voice, AL_ROLLOFF_FACTOR, 1.0);
	alSourcei(voice, AL_SOURCE_RELATIVE, AL_TRUE);
	alSource3i(voice, AL_AUXILIARY_SEND_FILTER, (ALint)sndGlobal.reverb_aux_slot, 0, AL_FILTER_NULL);
	alSource3f(voice, AL_POSITION, -origin[1], origin[2], -origin[0]);

	alSourcePlay(voice);
}

void S_StopAllSounds(void)
{
	// Stop all the sounds
	for (int i = 0; i < 32; i++)
	{
		ALint source_state;
		alGetSourcei(sndGlobal.voices[i], AL_SOURCE_STATE, &source_state);

		if (source_state == AL_PLAYING)
		{
			alSourceStop(sndGlobal.voices[i]);
		}
	}
}

void S_StopAllSounds_Sounding(void)
{

}

void S_Update(vec3_t quake_origin, vec3_t forward, vec3_t right, vec3_t up)
{
	vec3_t velocity;
	vec_t orientation[6];
	vec3_t origin;

	origin[0] = -quake_origin[1];
	origin[1] = quake_origin[2];
	origin[2] = -quake_origin[0];

	orientation[0] = -forward[2];
	orientation[1] = forward[1];
	orientation[2] = -forward[0];
	orientation[3] = -up[2];
	orientation[4] = up[1];
	orientation[5] = -up[0];

	velocity[0] = origin[0] - sndGlobal.prev_origin[0];
	velocity[1] = origin[1] - sndGlobal.prev_origin[1];
	velocity[2] = origin[2] - sndGlobal.prev_origin[2];

	alDistanceModel(AL_LINEAR_DISTANCE_CLAMPED);

	alListener3f(AL_POSITION, origin[0], origin[1], origin[2]);
	alListener3f(AL_VELOCITY, velocity[0], velocity[1], velocity[2]);
	alListenerfv(AL_ORIENTATION, orientation);

	sndGlobal.prev_origin[0] = origin[0];
	sndGlobal.prev_origin[1] = origin[1];
	sndGlobal.prev_origin[2] = origin[2];

}

void S_PlayMusic(int track, int looping)
{
	char filename[512];

	if(track < 10)
		sprintf(filename, "music/Track0%d.wav", track);
	else
		sprintf(filename, "music/Track%d.wav", track);
	
	sfx_t *music = S_FindName(filename, true);
	if (!music)
	{
		Com_Printf("Failed to load music %s\n", filename);
		return;
	}

	alSourcei(sndGlobal.music_voice, AL_BUFFER, music->buffer);
	alSourcei(sndGlobal.music_voice, AL_SOURCE_RELATIVE, AL_FALSE);
	alSourcei(sndGlobal.music_voice, AL_LOOPING, AL_TRUE);

	float volume = s_musicVolume->value / 100.0f;
	alSourcef(sndGlobal.music_voice, AL_GAIN, volume);

	alSourcePlay(sndGlobal.music_voice);
}