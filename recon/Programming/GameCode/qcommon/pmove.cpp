/*
Copyright (C) 1997-2001 Id Software, Inc.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/

#include "../client/client.h"
#include "g_Physics.h"

#define	STEPSIZE	18
#define OVERCLIP	1.001f

// all of the locals will be zeroed before each
// pmove, just to make damn sure we don't have
// any differences when running on client or server

typedef struct
{
	vec3_t		origin;			// full float precision
	vec3_t		velocity;		// full float precision

	vec3_t		forward, right, up;
	float		frametime;


	trace_t		groundTrace;
	vec3_t		previous_origin;

	qboolean	walking;
	qboolean	groundPlane;

	float impactSpeed;
} pml_t;

pmove_t* pm;
pml_t		pml;


// movement parameters
float	pm_stopspeed = 100;
float	pm_maxspeed = 300;
float	pm_duckspeed = 100;
float	pm_accelerate = 10;
float	pm_airaccelerate = 1;
float	pm_wateraccelerate = 10;
float	pm_friction = 6;
float	pm_waterfriction = 1;
float	pm_waterspeed = 400;

#define MIN_WALK_NORMAL 0.7f 

/*

  walking up a step should kill some velocity

*/

/*
=============
PM_CorrectAllSolid
=============
*/
static int PM_CorrectAllSolid(trace_t* trace) {
	int			i, j, k;
	vec3_t		point;

	// jitter around
	for (i = -1; i <= 1; i++) {
		for (j = -1; j <= 1; j++) {
			for (k = -1; k <= 1; k++) {
				VectorCopy(pml.origin, point);
				point[0] += (float)i;
				point[1] += (float)j;
				point[2] += (float)k;
				pm->trace(point, pm->mins, pm->maxs, point, trace);
				if (!trace->allsolid) {
					point[0] = pml.origin[0];
					point[1] = pml.origin[1];
					point[2] = pml.origin[2] - 0.25;

					pm->trace(pml.origin, pm->mins, pm->maxs, point, trace);
					pml.groundTrace = *trace;
					return true;
				}
			}
		}
	}

	pm->groundentity = NULL;
	pml.groundPlane = false;
	pml.walking = false;

	return false;
}


/*
==================
PM_Friction
Handles both ground friction and water friction
==================
*/
static void PM_Friction(void) {
	vec3_t	vec;
	float* vel;
	float	speed, newspeed, control;
	float	drop;

	vel = pml.velocity;

	VectorCopy(vel, vec);
	if (pml.walking) {
		vec[2] = 0;	// ignore slope movement
	}

	speed = VectorLength(vec);
	if (speed < 1) {
		vel[0] = 0;
		vel[1] = 0;		// allow sinking underwater
		// FIXME: still have z friction underwater?
		return;
	}

	drop = 0;

	// apply ground friction
	if (pm->waterlevel <= 1) {
		if (pml.walking) {
			// if getting knocked back, no friction
			{
				control = speed < pm_stopspeed ? pm_stopspeed : speed;
				drop += control * pm_friction * pml.frametime;
			}
		}
	}

	// apply water friction even if just wading
	if (pm->waterlevel) {
		drop += speed * pm_waterfriction * pm->waterlevel * pml.frametime;
	}

	// apply flying friction
	//if (pm->ps->powerups[PW_FLIGHT]) {
	//	drop += speed * pm_flightfriction * pml.frametime;
	//}
	//
	//if (pm->ps->pm_type == PM_SPECTATOR) {
	//	drop += speed * pm_spectatorfriction * pml.frametime;
	//}

	// scale the velocity
	newspeed = speed - drop;
	if (newspeed < 0) {
		newspeed = 0;
	}
	newspeed /= speed;

	vel[0] = vel[0] * newspeed;
	vel[1] = vel[1] * newspeed;
	vel[2] = vel[2] * newspeed;
}



/*
==================
PM_ClipVelocity
Slide off of the impacting surface
==================
*/
void PM_ClipVelocity(vec3_t in, vec3_t normal, vec3_t out, float overbounce) {
	float	backoff;
	float	change;
	int		i;

	backoff = DotProduct(in, normal);

	if (backoff < 0) {
		backoff *= overbounce;
	}
	else {
		backoff /= overbounce;
	}

	for (i = 0; i < 3; i++) {
		change = normal[i] * backoff;
		out[i] = in[i] - change;
	}
}

/*
=============
PM_GroundTrace
=============
*/
static void PM_GroundTrace(void) {
	vec3_t		point;
	vec3_t		start;
	trace_t		trace;

	start[0] = pml.origin[0];
	start[1] = pml.origin[1];
	start[2] = pml.origin[2];

	point[0] = pml.origin[0];
	point[1] = pml.origin[1];
	point[2] = pml.origin[2] - 0.5f;

	pm->trace(start, pm->mins, pm->maxs, point, &trace);
	pml.groundTrace = trace;

	// do something corrective if the trace starts in a solid...
	if (trace.allsolid) {
		if (!PM_CorrectAllSolid(&trace))
			return;
	}

	if (trace.fraction == 1.0f)
	{
		re.DrawLine(start, point);
	}
	else
	{
		re.DrawLine(start, trace.endpos);
	}

	pm->groundentity = trace.ent;

	if (trace.allsolid) {
		pml.groundPlane = false;
		pml.walking = false;
		return;
	}

	// slopes that are too steep will not be considered onground
	if (trace.plane.normal[2] < MIN_WALK_NORMAL) {
		pml.groundPlane = true;
		pml.walking = false;
		return;
	}

	pm->numtouch = 0;

	if (pm->numtouch < 31)
	{
		if (pm->groundentity)
		{
			pm->touchents[pm->numtouch] = pm->groundentity;
			pm->numtouch++;
		}
	}

	pml.groundPlane = true;
	pml.walking = true;
}

/*
================
PM_ClampAngles

================
*/
void PM_ClampAngles(void)
{
	short	temp;
	int		i;

	if (pm->s.pm_flags & PMF_TIME_TELEPORT)
	{
		pm->viewangles[YAW] = SHORT2ANGLE(pm->cmd.angles[YAW] + pm->s.delta_angles[YAW]);
		pm->viewangles[PITCH] = 0;
		pm->viewangles[ROLL] = 0;
	}
	else
	{
		// circularly clamp the angles with deltas
		for (i = 0; i < 3; i++)
		{
			temp = pm->cmd.angles[i] + pm->s.delta_angles[i];
			pm->viewangles[i] = SHORT2ANGLE(temp);
		}

		// don't let the player look up or down more than 90 degrees
		if (pm->viewangles[PITCH] > 89 && pm->viewangles[PITCH] < 180)
			pm->viewangles[PITCH] = 89;
		else if (pm->viewangles[PITCH] < 271 && pm->viewangles[PITCH] >= 180)
			pm->viewangles[PITCH] = 271;
	}
	AngleVectors(pm->viewangles, pml.forward, pml.right, pml.up);
}

/*
==============
PM_Accelerate
Handles user intended acceleration
==============
*/
static void PM_Accelerate(vec3_t wishdir, float wishspeed, float accel) {
#if 1
	// q2 style
	int			i;
	float		addspeed, accelspeed, currentspeed;

	vec3_t		newvel;

	newvel[0] = pm->s.velocity[0];
	newvel[1] = pm->s.velocity[1];
	newvel[2] = pm->s.velocity[2];

	currentspeed = DotProduct(newvel, wishdir);
	addspeed = wishspeed - currentspeed;
	if (addspeed <= 0) {
		return;
	}
	accelspeed = accel * pml.frametime * wishspeed;
	if (accelspeed > addspeed) {
		accelspeed = addspeed;
	}

	for (i = 0; i < 3; i++) {
		pml.velocity[i] += accelspeed * wishdir[i];
	}
#else
	// proper way (avoids strafe jump maxspeed bug), but feels bad
	vec3_t		wishVelocity;
	vec3_t		pushDir;
	float		pushLen;
	float		canPush;
	VectorScale(wishdir, wishspeed, wishVelocity);
	VectorSubtract(wishVelocity, pml.velocity, pushDir);
	pushLen = VectorNormalize(pushDir);
	canPush = accel * pml.frametime * wishspeed;
	if (canPush > pushLen) {
		canPush = pushLen;
	}
	VectorMA(pml.velocity, canPush, pushDir, pml.velocity);
#endif
}

/*
==================
PM_SlideMove
Returns qtrue if the velocity was clipped in some way
==================
*/
#define	MAX_CLIP_PLANES	5
qboolean	PM_SlideMove(qboolean gravity) {
	int			bumpcount, numbumps;
	vec3_t		dir;
	float		d;
	int			numplanes;
	vec3_t		planes[MAX_CLIP_PLANES];
	vec3_t		primal_velocity;
	vec3_t		clipVelocity;
	int			i, j, k;
	trace_t	trace;
	vec3_t		end;
	float		time_left;
	float		into;
	vec3_t		endVelocity;
	vec3_t		endClipVelocity;

	numbumps = 4;

	VectorCopy(pml.velocity, primal_velocity);

	if (gravity) {
		VectorCopy(pml.velocity, endVelocity);
		endVelocity[2] -= (GRAVITY * 50) * pml.frametime;
		pml.velocity[2] = (pml.velocity[2] + endVelocity[2]) * 0.5;
		primal_velocity[2] = endVelocity[2];
		if (pml.groundPlane) {
			// slide along the ground plane
			PM_ClipVelocity(pml.velocity, pml.groundTrace.plane.normal,
				pml.velocity, OVERCLIP);
		}
	}

	time_left = pml.frametime;

	// never turn against the ground plane
	if (pml.groundPlane) {
		numplanes = 1;
		VectorCopy(pml.groundTrace.plane.normal, planes[0]);
	}
	else {
		numplanes = 0;
	}

	// never turn against original velocity
	VectorNormalize2(pml.velocity, planes[numplanes]);
	numplanes++;

	for (bumpcount = 0; bumpcount < numbumps; bumpcount++) {

		// calculate position we are trying to move to
		VectorMA(pml.origin, time_left, pml.velocity, end);

		// see if we can make it there
		pm->trace(pml.origin, pm->mins, pm->maxs, end, &trace);

		if (trace.allsolid) {
			// entity is completely trapped in another solid
			pml.velocity[2] = 0;	// don't build up falling damage, but allow sideways acceleration
			return true;
		}

		if (trace.fraction > 0) {
			// actually covered some distance
			VectorCopy(trace.endpos, pml.origin);
		}

		if (trace.fraction == 1) {
			break;		// moved the entire distance
		}

		// save entity for contact
		//PM_AddTouchEnt(trace.entityNum);

		time_left -= time_left * trace.fraction;

		if (numplanes >= MAX_CLIP_PLANES) {
			// this shouldn't really happen
			VectorClear(pml.velocity);
			return true;
		}

		//
		// if this is the same plane we hit before, nudge velocity
		// out along it, which fixes some epsilon issues with
		// non-axial planes
		//
		for (i = 0; i < numplanes; i++) {
			if (DotProduct(trace.plane.normal, planes[i]) > 0.99) {
				VectorAdd(trace.plane.normal, pml.velocity, pml.velocity);
				break;
			}
		}
		if (i < numplanes) {
			continue;
		}
		VectorCopy(trace.plane.normal, planes[numplanes]);
		numplanes++;

		//
		// modify velocity so it parallels all of the clip planes
		//

		// find a plane that it enters
		for (i = 0; i < numplanes; i++) {
			into = DotProduct(pml.velocity, planes[i]);
			if (into >= 0.1) {
				continue;		// move doesn't interact with the plane
			}

			// see how hard we are hitting things
			if (-into > pml.impactSpeed) {
				pml.impactSpeed = -into;
			}

			// slide along the plane
			PM_ClipVelocity(pml.velocity, planes[i], clipVelocity, OVERCLIP);

			// slide along the plane
			PM_ClipVelocity(endVelocity, planes[i], endClipVelocity, OVERCLIP);

			// see if there is a second plane that the new move enters
			for (j = 0; j < numplanes; j++) {
				if (j == i) {
					continue;
				}
				if (DotProduct(clipVelocity, planes[j]) >= 0.1) {
					continue;		// move doesn't interact with the plane
				}

				// try clipping the move to the plane
				PM_ClipVelocity(clipVelocity, planes[j], clipVelocity, OVERCLIP);
				PM_ClipVelocity(endClipVelocity, planes[j], endClipVelocity, OVERCLIP);

				// see if it goes back into the first clip plane
				if (DotProduct(clipVelocity, planes[i]) >= 0) {
					continue;
				}

				// slide the original velocity along the crease
				CrossProduct(planes[i], planes[j], dir);
				VectorNormalize(dir);
				d = DotProduct(dir, pml.velocity);
				VectorScale(dir, d, clipVelocity);

				CrossProduct(planes[i], planes[j], dir);
				VectorNormalize(dir);
				d = DotProduct(dir, endVelocity);
				VectorScale(dir, d, endClipVelocity);

				// see if there is a third plane the the new move enters
				for (k = 0; k < numplanes; k++) {
					if (k == i || k == j) {
						continue;
					}
					if (DotProduct(clipVelocity, planes[k]) >= 0.1) {
						continue;		// move doesn't interact with the plane
					}

					// stop dead at a tripple plane interaction
					VectorClear(pml.velocity);
					return true;
				}
			}

			// if we have fixed all interactions, try another move
			VectorCopy(clipVelocity, pml.velocity);
			VectorCopy(endClipVelocity, endVelocity);
			break;
		}
	}

	if (gravity) {
		VectorCopy(endVelocity, pml.velocity);
	}

	// don't change velocity if in a timer (FIXME: is this correct?)
	//if (pm->ps->pm_time) {
	//	VectorCopy(primal_velocity, pml.velocity);
	//}

	return (bumpcount != 0);
}

/*
==================
PM_StepSlideMove
==================
*/
void PM_StepSlideMove(qboolean gravity) {
	vec3_t		start_o, start_v;
	vec3_t		down_o, down_v;
	trace_t		trace;
	//	float		down_dist, up_dist;
	//	vec3_t		delta, delta2;
	vec3_t		up, down;
	float		stepSize;

	VectorCopy(pml.origin, start_o);
	VectorCopy(pml.velocity, start_v);

	if (PM_SlideMove(gravity) == 0) {
		return;		// we got exactly where we wanted to go first try	
	}

	VectorCopy(start_o, down);
	down[2] -= STEPSIZE;
	pm->trace(start_o, pm->mins, pm->maxs, down, &trace);
	VectorSet(up, 0, 0, 1);
	// never step up when you still have up velocity
	if (pml.velocity[2] > 0 && (trace.fraction == 1.0 ||
		DotProduct(trace.plane.normal, up) < 0.7)) {
		return;
	}

	VectorCopy(pml.origin, down_o);
	VectorCopy(pml.velocity, down_v);

	VectorCopy(start_o, up);
	up[2] += STEPSIZE;

	// test the player position if they were a stepheight higher
	pm->trace(start_o, pm->mins, pm->maxs, up, &trace);
	if (trace.allsolid) {
		//if (pm->debugLevel) {
		//	Com_Printf("%i:bend can't step\n", c_pmove);
		//}
		return;		// can't step up
	}

	stepSize = trace.endpos[2] - start_o[2];
	// try slidemove from this position
	VectorCopy(trace.endpos, pml.origin);
	VectorCopy(start_v, pml.velocity);

	PM_SlideMove(gravity);

	// push down the final amount
	VectorCopy(pml.origin, down);
	down[2] -= stepSize;
	pm->trace(pml.origin, pm->mins, pm->maxs, down, &trace);
	if (!trace.allsolid) {
		VectorCopy(trace.endpos, pml.origin);
	}
	if (trace.fraction < 1.0) {
		PM_ClipVelocity(pml.velocity, trace.plane.normal, pml.velocity, OVERCLIP);
	}
}
/*
===================
PM_AirMove
===================
*/
static void PM_AirMove(float fmove, float smove) {
	int			i;
	vec3_t		wishvel;
	vec3_t		wishdir;
	float		wishspeed;
	float		scale;
	usercmd_t	cmd;

	PM_Friction();

	cmd = pm->cmd;
	scale = 1.0f;

	// project moves down to flat plane
	pml.forward[2] = 0;
	pml.right[2] = 0;
	VectorNormalize(pml.forward);
	VectorNormalize(pml.right);

	for (i = 0; i < 2; i++) {
		wishvel[i] = pml.forward[i] * fmove + pml.right[i] * smove;
	}
	wishvel[2] = 0;

	VectorCopy(wishvel, wishdir);
	wishspeed = VectorNormalize(wishdir);
	wishspeed *= scale;

	// not on ground, so little effect on velocity
	PM_Accelerate(wishdir, wishspeed, pm_airaccelerate);

	// we may have a ground plane that is very steep, even
	// though we don't have a groundentity
	// slide along the steep plane
	if (pml.groundPlane) {
		PM_ClipVelocity(pml.velocity, pml.groundTrace.plane.normal,
			pml.velocity, OVERCLIP);
	}

#if 0
	//ZOID:  If we are on the grapple, try stair-stepping
	//this allows a player to use the grapple to pull himself
	//over a ledge
	if (pm->ps->pm_flags & PMF_GRAPPLE_PULL)
		PM_StepSlideMove(qtrue);
	else
		PM_SlideMove(qtrue);
#endif

	PM_StepSlideMove(true);
}

/*
===================
PM_WalkMove
===================
*/
static void PM_WalkMove(float fmove, float smove) {
	vec3_t wishvel;
	vec3_t wishdir;
	float wishspeed;

	// project moves down to flat plane
	pml.forward[2] = 0;
	pml.right[2] = 0;

	// project the forward and right directions onto the ground plane
	PM_ClipVelocity(pml.forward, pml.groundTrace.plane.normal, pml.forward, OVERCLIP);
	PM_ClipVelocity(pml.right, pml.groundTrace.plane.normal, pml.right, OVERCLIP);
	//
	VectorNormalize(pml.forward);
	VectorNormalize(pml.right);

	for (int i = 0; i < 3; i++) {
		wishvel[i] = pml.forward[i] * fmove + pml.right[i] * smove;
	}

	VectorCopy(wishvel, wishdir);
	wishspeed = VectorNormalize(wishdir);

	PM_Accelerate(wishdir, wishspeed, pm_accelerate);

	float vel = VectorLength(pml.velocity);
	// slide along the ground plane
	PM_ClipVelocity(pml.velocity, pml.groundTrace.plane.normal, pml.velocity, OVERCLIP);

	VectorNormalize(pml.velocity);
	pml.velocity[0] = pml.velocity[0] * vel;
	pml.velocity[1] = pml.velocity[1] * vel;
	pml.velocity[2] = pml.velocity[2] * vel;

	// don't do anything if standing still
	if (!pml.velocity[0] && !pml.velocity[1]) {
		return;
	}

	PM_StepSlideMove(false);
}

/*
================
Pmove

Can be called by either the server or the client
================
*/
void Pmove(pmove_t* pmove, qboolean isServer)
{
	pm = pmove;

	// jmarshall: TODO: Client Prediction
	if (!isServer)
	{
		// Convert it back into nonsense Quake 2 compression.
		pm->s.origin[0] = pml.origin[0] * 8.0f;
		pm->s.origin[1] = pml.origin[1] * 8.0f;
		pm->s.origin[2] = pml.origin[2] * 8.0f;

		pm->s.velocity[0] = pml.velocity[0] * 8.0f;
		pm->s.velocity[1] = pml.velocity[1] * 8.0f;
		pm->s.velocity[2] = pml.velocity[2] * 8.0f;

		PM_ClampAngles();

		return;
	}

	// clear results
	pm->numtouch = 0;
	VectorClear(pm->viewangles);
	pm->groundentity = 0;
	pm->watertype = 0;
	pm->waterlevel = 0;

	// clear all pmove local vars
	memset(&pml, 0, sizeof(pml));

	// convert origin and velocity to float values
	pml.origin[0] = pm->s.origin[0] * 0.125;
	pml.origin[1] = pm->s.origin[1] * 0.125;
	pml.origin[2] = (pm->s.origin[2] * 0.125);

	pml.velocity[0] = pm->s.velocity[0] * 0.125;
	pml.velocity[1] = pm->s.velocity[1] * 0.125;
	pml.velocity[2] = pm->s.velocity[2] * 0.125;

	pml.frametime = pm->cmd.msec * 0.001;

	// save old org in case we get stuck
	//VectorCopy(pm->s.origin, pml.previous_origin);
	pml.previous_origin[0] = pm->s.origin[0];
	pml.previous_origin[1] = pm->s.origin[1];
	pml.previous_origin[2] = pm->s.origin[2];

	PM_ClampAngles();

	AngleVectors(pm->viewangles, pml.forward, pml.right, pml.up);

	if (pm->s.pm_type >= PM_DEAD) {
		pm->cmd.forwardmove = 0;
		pm->cmd.sidemove = 0;
		pm->cmd.upmove = 0;
	}

	PM_GroundTrace();

	if (pml.walking) {
		// walking on ground
		PM_WalkMove(pm->cmd.forwardmove * 8, pm->cmd.sidemove * 8);
	}
	else {
		// airborne
		PM_AirMove(pm->cmd.forwardmove * 8, pm->cmd.sidemove * 8);
	}

	// Convert it back into nonsense Quake 2 compression.
	pm->s.origin[0] = pml.origin[0] * 8.0f;
	pm->s.origin[1] = pml.origin[1] * 8.0f;
	pm->s.origin[2] = pml.origin[2] * 8.0f;

	pm->s.velocity[0] = pml.velocity[0] * 8.0f;
	pm->s.velocity[1] = pml.velocity[1] * 8.0f;
	pm->s.velocity[2] = pml.velocity[2] * 8.0f;

	// jmarshall: TODO: I believe this is used for first person view. 
	pm->cmd.aimangles[0] = pm->cmd.angles[0];
	pm->cmd.aimangles[1] = pm->cmd.angles[1];
	pm->cmd.aimangles[2] = pm->cmd.angles[2];
}

