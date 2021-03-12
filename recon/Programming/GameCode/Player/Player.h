#ifndef PLAYER_H
#define PLAYER_H

//#ifdef PLAYER_DLL
//
//#define PLAYER_API __declspec(dllexport)
//
//#else
//
//#define PLAYER_API __declspec(dllimport)
//
//#endif	// PLAYER_DLL

#define PLAYER_API

#include "p_types.h"


// **************
// Movement rates
// **************

#define IN_MOVE_CREEP_MIN	16
#define IN_MOVE_CREEP		32
#define IN_MOVE_WALK_MIN	48
#define IN_MOVE_WALK		64
#define IN_MOVE_RUN_MIN		80
#define IN_MOVE_RUN			96

#define IN_MOVE_THRESHOLD	IN_MOVE_CREEP_MIN

#define BUTTON_WALK			0

enum movefwd_e
{
	MOVE_BACK_RUN,
	MOVE_BACK_WALK,
	MOVE_BACK_CREEP,
	MOVE_FWD_NONE,
	MOVE_FWD_CREEP,
	MOVE_FWD_WALK,
	MOVE_FWD_RUN,
	MOVE_FWD_MAX
};

enum moveright_e
{
	MOVE_LEFT_RUN,
	MOVE_LEFT_WALK,
	MOVE_LEFT_CREEP,
	MOVE_RIGHT_NONE,
	MOVE_RIGHT_CREEP,
	MOVE_RIGHT_WALK,
	MOVE_RIGHT_RUN,
	MOVE_RIGHT_MAX
};

enum moveplus_e
{
	MOVE_NORM,
	MOVE_NOFWD,
	MOVE_NOSIDE,
};


#define DEFAULT_PLAYER_LIB "Player"

typedef struct
{
	paceldata_t *PlayerSeqData,*PlayerChickenData;
	int			p_num_items;
	gitem_t		*p_itemlist;
} player_export_t;

void P_Freelib(void);
unsigned int P_Load(char *name);

PLAYER_API void P_Init(void);
PLAYER_API void P_Shutdown(void);

void PlayerReleaseRope(playerinfo_t* playerinfo);
void KnockDownPlayer(playerinfo_t* playerinfo);
void PlayFly(playerinfo_t* playerinfo, float dist);
void PlaySlap(playerinfo_t* playerinfo, float dist);
void PlayScratch(playerinfo_t* playerinfo, float dist);
void PlaySigh(playerinfo_t* playerinfo, float dist);
void SpawnDustPuff(playerinfo_t* playerinfo, float dist);
void PlayerInterruptAction(playerinfo_t* playerinfo);

qboolean BranchCheckDismemberAction(playerinfo_t* playerinfo, int weapon);

void TurnOffPlayerEffects(playerinfo_t* playerinfo);
void AnimUpdateFrame(playerinfo_t* playerinfo);
void PlayerFallingDamage(playerinfo_t* playerinfo);

void PlayerBasicAnimReset(playerinfo_t* playerinfo);
void PlayerAnimReset(playerinfo_t* playerinfo);
void PlayerAnimSetLowerSeq(playerinfo_t* playerinfo, int seq);
void PlayerAnimSetUpperSeq(playerinfo_t* playerinfo, int seq);
void PlayerAnimUpperIdle(playerinfo_t* playerinfo);
void PlayerAnimLowerIdle(playerinfo_t* playerinfo);
void PlayerAnimUpperUpdate(playerinfo_t* playerinfo);
void PlayerAnimLowerUpdate(playerinfo_t* playerinfo);
void PlayerAnimSetVault(playerinfo_t* playerinfo, int seq);
void PlayerPlayPain(playerinfo_t* playerinfo, int type);

void PlayerIntLand(playerinfo_t* playerinfo_t, float landspeed);

void PlayerInit(playerinfo_t* playerinfo, int complete_reset);
void PlayerClearEffects(playerinfo_t* playerinfo);
void PlayerUpdate(playerinfo_t* playerinfo);
void PlayerUpdateCmdFlags(playerinfo_t* playerinfo);
void PlayerUpdateModelAttributes(playerinfo_t* playerinfo);

void Weapon_Ready(playerinfo_t* playerinfo, gitem_t* Weapon);
void Weapon_EquipSpell(playerinfo_t* playerinfo, gitem_t* Weapon);
void Weapon_EquipSwordStaff(playerinfo_t* playerinfo, gitem_t* Weapon);
void Weapon_EquipHellStaff(playerinfo_t* playerinfo, gitem_t* Weapon);
void Weapon_EquipBow(playerinfo_t* playerinfo, gitem_t* Weapon);
void Weapon_EquipArmor(playerinfo_t* playerinfo, gitem_t* Weapon);
int Weapon_CurrentShotsLeft(playerinfo_t* playerinfo);
int Defence_CurrentShotsLeft(playerinfo_t* playerinfo, int intent);



// This is the only function actually exported at the linker level.

typedef	player_export_t	(*GetPlayerAPI_t)(void);

#endif	// PLAYER_H
