// s_eax.c
//

#include "s_local.h"

/* Effect object functions */
static LPALGENEFFECTS alGenEffects;
static LPALDELETEEFFECTS alDeleteEffects;
static LPALISEFFECT alIsEffect;
static LPALEFFECTI alEffecti;
static LPALEFFECTIV alEffectiv;
static LPALEFFECTF alEffectf;
static LPALEFFECTFV alEffectfv;
static LPALGETEFFECTI alGetEffecti;
static LPALGETEFFECTIV alGetEffectiv;
static LPALGETEFFECTF alGetEffectf;
static LPALGETEFFECTFV alGetEffectfv;

/* Auxiliary Effect Slot object functions */
static LPALGENAUXILIARYEFFECTSLOTS alGenAuxiliaryEffectSlots;
static LPALDELETEAUXILIARYEFFECTSLOTS alDeleteAuxiliaryEffectSlots;
static LPALISAUXILIARYEFFECTSLOT alIsAuxiliaryEffectSlot;
static LPALAUXILIARYEFFECTSLOTI alAuxiliaryEffectSloti;
static LPALAUXILIARYEFFECTSLOTIV alAuxiliaryEffectSlotiv;
static LPALAUXILIARYEFFECTSLOTF alAuxiliaryEffectSlotf;
static LPALAUXILIARYEFFECTSLOTFV alAuxiliaryEffectSlotfv;
static LPALGETAUXILIARYEFFECTSLOTI alGetAuxiliaryEffectSloti;
static LPALGETAUXILIARYEFFECTSLOTIV alGetAuxiliaryEffectSlotiv;
static LPALGETAUXILIARYEFFECTSLOTF alGetAuxiliaryEffectSlotf;
static LPALGETAUXILIARYEFFECTSLOTFV alGetAuxiliaryEffectSlotfv;

char EAX_ReverbTable[432] =
{
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '?',
  ' ',
  '\x1A',
  '¿',
  '?',
  '\0',
  '\0',
  '\0',
  '?',
  '\x01',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '€',
  '>',
  'Í',
  'Ì',
  'Ì',
  '=',
  '\0',
  '\0',
  '\0',
  '\0',
  '\x02',
  '\0',
  '\0',
  '\0',
  '\x06',
  '\x81',
  'Õ',
  '>',
  'Í',
  'Ì',
  'Ì',
  '>',
  'ú',
  '~',
  '*',
  '?',
  '\x03',
  '\0',
  '\0',
  '\0',
  '\x02',
  '+',
  '\'',
  '?',
  ';',
  'ß',
  '¿',
  '?',
  'ç',
  'û',
  ')',
  '>',
  '\x04',
  '\0',
  '\0',
  '\0',
  'ô',
  'ý',
  'T',
  '>',
  'j',
  '¼',
  'ô',
  '>',
  '\0',
  '\0',
  '\0',
  '\0',
  '\x05',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '?',
  '¨',
  'Æ',
  '\x13',
  '@',
  'ø',
  'S',
  'c',
  '?',
  '\x06',
  '\0',
  '\0',
  '\0',
  '\x04',
  'V',
  'Î',
  '>',
  '‘',
  'í',
  'ˆ',
  '@',
  '\0',
  '\0',
  '\0',
  '?',
  '\a',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '?',
  '\x06',
  '\x81',
  '}',
  '@',
  '\0',
  '\0',
  '\0',
  '?',
  '\b',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '?',
  '9',
  '´',
  '8',
  '@',
  'y',
  'é',
  '¦',
  '?',
  '\t',
  '\0',
  '\0',
  '\0',
  'þ',
  'Ô',
  '¸',
  '>',
  '‡',
  '\x16',
  'é',
  '@',
  'ç',
  'û',
  '©',
  '>',
  '\n',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '?',
  '\0',
  '\0',
  ' ',
  'A',
  'š',
  '™',
  '™',
  '>',
  '\v',
  '\0',
  '\0',
  '\0',
  '\b',
  '¬',
  '\x1C',
  '>',
  '¦',
  '›',
  '„',
  '>',
  '\0',
  '\0',
  '\0',
  '@',
  '\f',
  '\0',
  '\0',
  '\0',
  'þ',
  'Ô',
  '¸',
  '>',
  ' ',
  '\x1A',
  '¿',
  '?',
  '\0',
  '\0',
  '\0',
  '\0',
  '\r',
  '\0',
  '\0',
  '\0',
  'ø',
  'S',
  'ã',
  '>',
  '¦',
  '›',
  ',',
  '@',
  'ø',
  'S',
  '#',
  '?',
  '\x0E',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '€',
  '>',
  '‰',
  'A',
  'à',
  '?',
  'ð',
  '§',
  'F',
  '?',
  '\x0F',
  '\0',
  '\0',
  '\0',
  'ø',
  'S',
  'ã',
  '=',
  '®',
  'G',
  'I',
  '@',
  'ü',
  '©',
  'ñ',
  '>',
  '\x10',
  '\0',
  '\0',
  '\0',
  'ø',
  'S',
  'ã',
  '=',
  '‡',
  '\x16',
  '1',
  '@',
  'B',
  '`',
  'e',
  '>',
  '\x11',
  '\0',
  '\0',
  '\0',
  'ð',
  '§',
  'F',
  '>',
  'y',
  'é',
  'ú',
  '@',
  'ü',
  '©',
  'ñ',
  '>',
  '\x12',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '€',
  '?',
  ';',
  'ß',
  '¿',
  '?',
  '\0',
  '\0',
  '\0',
  '?',
  '\x13',
  '\0',
  '\0',
  '\0',
  'ð',
  '§',
  'Æ',
  '=',
  '‡',
  '\x16',
  '1',
  '@',
  'B',
  '`',
  'e',
  '>',
  '\x14',
  '\0',
  '\0',
  '\0',
  'ô',
  'ý',
  'T',
  '>',
  '¼',
  't',
  'Ó',
  '?',
  '\0',
  '\0',
  'À',
  '?',
  '\x15',
  '\0',
  '\0',
  '\0',
  'y',
  'é',
  '&',
  '?',
  '9',
  '´',
  '8',
  '@',
  '\0',
  '\0',
  '€',
  '>',
  '\x16',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '€',
  '?',
  ';',
  'ß',
  '¿',
  '?',
  '\0',
  '\0',
  '\0',
  '\0',
  '\x17',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '`',
  '?',
  '¢',
  'E',
  '\x06',
  'A',
  'ü',
  '©',
  '±',
  '?',
  '\x18',
  '\0',
  '\0',
  '\0',
  '\x04',
  'V',
  '\x0E',
  '>',
  ';',
  'ß',
  '‰',
  'A',
  'ú',
  '~',
  '*',
  '?',
  '\x19',
  '\0',
  '\0',
  '\0',
  'þ',
  'Ô',
  'ø',
  '>',
  '\x19',
  '\x04',
  'ò',
  '@',
  '\x04',
  'V',
  'N',
  '?'
}; // idb

byte s_currentReverbAmount = 0;

void SNDEAX_SetEnvironment(int id) {
	s_currentReverbAmount = EAX_ReverbTable[16 * (id % 27)];
}

/* LoadEffect loads the given reverb properties into a new OpenAL effect
 * object, and returns the new effect ID. */
ALuint S_LoadReverbEffect( void )
{
	ALuint effect = 0;
	ALenum err;
	EFXEAXREVERBPROPERTIES reverb = EFX_REVERB_PRESET_GENERIC;
	
	/* Define a macro to help load the function pointers. */
#define LOAD_PROC(T, x)  ((x) = (T)alGetProcAddress(#x))
	LOAD_PROC(LPALGENEFFECTS, alGenEffects);
	LOAD_PROC(LPALDELETEEFFECTS, alDeleteEffects);
	LOAD_PROC(LPALISEFFECT, alIsEffect);
	LOAD_PROC(LPALEFFECTI, alEffecti);
	LOAD_PROC(LPALEFFECTIV, alEffectiv);
	LOAD_PROC(LPALEFFECTF, alEffectf);
	LOAD_PROC(LPALEFFECTFV, alEffectfv);
	LOAD_PROC(LPALGETEFFECTI, alGetEffecti);
	LOAD_PROC(LPALGETEFFECTIV, alGetEffectiv);
	LOAD_PROC(LPALGETEFFECTF, alGetEffectf);
	LOAD_PROC(LPALGETEFFECTFV, alGetEffectfv);

	LOAD_PROC(LPALGENAUXILIARYEFFECTSLOTS, alGenAuxiliaryEffectSlots);
	LOAD_PROC(LPALDELETEAUXILIARYEFFECTSLOTS, alDeleteAuxiliaryEffectSlots);
	LOAD_PROC(LPALISAUXILIARYEFFECTSLOT, alIsAuxiliaryEffectSlot);
	LOAD_PROC(LPALAUXILIARYEFFECTSLOTI, alAuxiliaryEffectSloti);
	LOAD_PROC(LPALAUXILIARYEFFECTSLOTIV, alAuxiliaryEffectSlotiv);
	LOAD_PROC(LPALAUXILIARYEFFECTSLOTF, alAuxiliaryEffectSlotf);
	LOAD_PROC(LPALAUXILIARYEFFECTSLOTFV, alAuxiliaryEffectSlotfv);
	LOAD_PROC(LPALGETAUXILIARYEFFECTSLOTI, alGetAuxiliaryEffectSloti);
	LOAD_PROC(LPALGETAUXILIARYEFFECTSLOTIV, alGetAuxiliaryEffectSlotiv);
	LOAD_PROC(LPALGETAUXILIARYEFFECTSLOTF, alGetAuxiliaryEffectSlotf);
	LOAD_PROC(LPALGETAUXILIARYEFFECTSLOTFV, alGetAuxiliaryEffectSlotfv);
#undef LOAD_PROC

	/* Create the effect object and check if we can do EAX reverb. */
	alGenEffects(1, &effect);
	if (alGetEnumValue("AL_EFFECT_EAXREVERB") != 0)
	{
		Com_Printf("Using EAX Reverb\n");

		/* EAX Reverb is available. Set the EAX effect type then load the
		 * reverb properties. */
		alEffecti(effect, AL_EFFECT_TYPE, AL_EFFECT_EAXREVERB);

		alEffectf(effect, AL_EAXREVERB_DENSITY, reverb.flDensity);
		alEffectf(effect, AL_EAXREVERB_DIFFUSION, reverb.flDiffusion);
		alEffectf(effect, AL_EAXREVERB_GAIN, reverb.flGain);
		alEffectf(effect, AL_EAXREVERB_GAINHF, reverb.flGainHF);
		alEffectf(effect, AL_EAXREVERB_GAINLF, reverb.flGainLF);
		alEffectf(effect, AL_EAXREVERB_DECAY_TIME, reverb.flDecayTime);
		alEffectf(effect, AL_EAXREVERB_DECAY_HFRATIO, reverb.flDecayHFRatio);
		alEffectf(effect, AL_EAXREVERB_DECAY_LFRATIO, reverb.flDecayLFRatio);
		alEffectf(effect, AL_EAXREVERB_REFLECTIONS_GAIN, reverb.flReflectionsGain);
		alEffectf(effect, AL_EAXREVERB_REFLECTIONS_DELAY, reverb.flReflectionsDelay);
		alEffectfv(effect, AL_EAXREVERB_REFLECTIONS_PAN, reverb.flReflectionsPan);
		alEffectf(effect, AL_EAXREVERB_LATE_REVERB_GAIN, reverb.flLateReverbGain);
		alEffectf(effect, AL_EAXREVERB_LATE_REVERB_DELAY, reverb.flLateReverbDelay);
		alEffectfv(effect, AL_EAXREVERB_LATE_REVERB_PAN, reverb.flLateReverbPan);
		alEffectf(effect, AL_EAXREVERB_ECHO_TIME, reverb.flEchoTime);
		alEffectf(effect, AL_EAXREVERB_ECHO_DEPTH, reverb.flEchoDepth);
		alEffectf(effect, AL_EAXREVERB_MODULATION_TIME, reverb.flModulationTime);
		alEffectf(effect, AL_EAXREVERB_MODULATION_DEPTH, reverb.flModulationDepth);
		alEffectf(effect, AL_EAXREVERB_AIR_ABSORPTION_GAINHF, reverb.flAirAbsorptionGainHF);
		alEffectf(effect, AL_EAXREVERB_HFREFERENCE, reverb.flHFReference);
		alEffectf(effect, AL_EAXREVERB_LFREFERENCE, reverb.flLFReference);
		alEffectf(effect, AL_EAXREVERB_ROOM_ROLLOFF_FACTOR, reverb.flRoomRolloffFactor);
		alEffecti(effect, AL_EAXREVERB_DECAY_HFLIMIT, reverb.iDecayHFLimit);
	}
	else
	{
		Com_Printf("Using Standard Reverb\n");

		/* No EAX Reverb. Set the standard reverb effect type then load the
		 * available reverb properties. */
		alEffecti(effect, AL_EFFECT_TYPE, AL_EFFECT_REVERB);

		alEffectf(effect, AL_REVERB_DENSITY, reverb.flDensity);
		alEffectf(effect, AL_REVERB_DIFFUSION, reverb.flDiffusion);
		alEffectf(effect, AL_REVERB_GAIN, reverb.flGain);
		alEffectf(effect, AL_REVERB_GAINHF, reverb.flGainHF);
		alEffectf(effect, AL_REVERB_DECAY_TIME, reverb.flDecayTime);
		alEffectf(effect, AL_REVERB_DECAY_HFRATIO, reverb.flDecayHFRatio);
		alEffectf(effect, AL_REVERB_REFLECTIONS_GAIN, reverb.flReflectionsGain);
		alEffectf(effect, AL_REVERB_REFLECTIONS_DELAY, reverb.flReflectionsDelay);
		alEffectf(effect, AL_REVERB_LATE_REVERB_GAIN, reverb.flLateReverbGain);
		alEffectf(effect, AL_REVERB_LATE_REVERB_DELAY, reverb.flLateReverbDelay);
		alEffectf(effect, AL_REVERB_AIR_ABSORPTION_GAINHF, reverb.flAirAbsorptionGainHF);
		alEffectf(effect, AL_REVERB_ROOM_ROLLOFF_FACTOR, reverb.flRoomRolloffFactor);
		alEffecti(effect, AL_REVERB_DECAY_HFLIMIT, reverb.iDecayHFLimit);
	}

	/* Check if an error occured, and clean up if so. */
	err = alGetError();
	if (err != AL_NO_ERROR)
	{
		Com_Error(ERR_FATAL, "OpenAL error: %s\n", alGetString(err));
		if (alIsEffect(effect))
			alDeleteEffects(1, &effect);
		return 0;
	}

	sndGlobal.reverb_aux_slot = 0;
	alGenAuxiliaryEffectSlots(1, &sndGlobal.reverb_aux_slot);

	/* Tell the effect slot to use the loaded effect object. Note that the this
	 * effectively copies the effect properties. You can modify or delete the
	 * effect object afterward without affecting the effect slot.
	 */
	alAuxiliaryEffectSloti(sndGlobal.reverb_aux_slot, AL_EFFECTSLOT_EFFECT, (ALint)effect);
	assert(alGetError() == AL_NO_ERROR && "Failed to set effect slot");

	return effect;
}
