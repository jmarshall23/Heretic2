// flexmodel.c
//

#include "gl_local.h"
#include "../ref_common/fmodel.h"
#include "../qcommon/flex.h"
#include "../ref_common/m_SkeletalCluster.h"
#include "../qcommon/Skeletons.h"
#include "../qcommon/ArrayedList.h"
#include "../ref_common/m_Skeleton.h"
#include "../ref_common/r_Skeletons.h"

fmblock_t m_fmblocks[] =
{
	{ FM_HEADER_NAME,			FM_BLOCK_HEADER },
	{ FM_SKIN_NAME,				FM_BLOCK_SKIN },
	{ FM_ST_NAME,				FM_BLOCK_ST },
	{ FM_TRI_NAME,				FM_BLOCK_TRIS },
	{ FM_FRAME_NAME	,			FM_BLOCK_FRAMES },
	{ FM_GLCMDS_NAME,			FM_BLOCK_GLCMDS },
	{ FM_MESH_NAME,				FM_BLOCK_MESHNODES },
	{ FM_SHORT_FRAME_NAME,		FM_BLOCK_SHORTFRAMES },
	{ FM_NORMAL_NAME,			FM_BLOCK_NORMAL },
	{ FM_COMP_NAME,				FM_BLOCK_COMP },
	{ FM_SKELETON_NAME,			FM_BLOCK_SKELETON },
	{ FM_REFERENCES_NAME,		FM_BLOCK_REFERENCES },
	{ "",						-1 }
};

extern M_SkeletalCluster_t SkeletalClusters[MAX_ARRAYED_SKELETAL_JOINTS];

/*
=============
Mod_SerializeHeader
=============
*/
void Mod_SerializeHeader(fmdl_t *fmodel, int version, int length, char *buffer)
{
	if (version != FM_HEADER_VER)
	{
		ri.Com_Error(ERR_FATAL, "Invalid header version for block");
		return;
	}

	memcpy(&fmodel->header, buffer, sizeof(fmheader_t));

	if (fmodel->header.num_xyz <= 0)
	{
		ri.Com_Error(ERR_FATAL, "Model has no verts");
		return;
	}

	if (fmodel->header.num_xyz > MAX_FM_VERTS)
	{
		ri.Com_Error(ERR_FATAL, "Model has too many vertices");
		return;
	}

	if (fmodel->header.num_st <= 0)
	{
		ri.Com_Error(ERR_FATAL, "Model has no st verts\n");
		return;
	}

	if (fmodel->header.num_tris <= 0)
	{
		ri.Com_Error(ERR_FATAL, "Model has no tris\n");
		return;
	}

	if (fmodel->header.num_frames <= 0)
	{
		ri.Com_Error(ERR_FATAL, "Model has no frames\n");
		return;
	}
}

/*
=============
Mod_SerializeSkin
=============
*/
void Mod_SerializeSkin(fmdl_t *fmodel, int version, int length, char *buffer)
{
	if (version != FM_SKIN_VER)
	{
		ri.Com_Error(ERR_FATAL, "Invalid skin version!");
		return;
	}

	fmodel->skin_names = buffer;
}

/*
=============
Mod_SerializeSt
=============
*/
void Mod_SerializeSt(fmdl_t *fmodel, int version, int length, char *buffer)
{
	// ignoring data
}

/*
=============
Mod_SerializeTris
=============
*/
void Mod_SerializeTris(fmdl_t *fmodel, int version, int length, char *buffer)
{
	// ignoring data
}

/*
=============
Mod_SerializeFrames
=============
*/
void Mod_SerializeFrames(fmdl_t *fmodel, int version, int length, char *buffer)
{
	int i, j;
	fmaliasframe_t	*pinframe, *poutframe;

	if (fmodel->frames != NULL)
	{
		ri.Com_Error(ERR_FATAL, "Duplicate frames block!\n");
		return;
	}
	if (version != FM_FRAME_VER)
	{
		ri.Com_Error(ERR_FATAL, "Invalid header version for block");
		return;
	}

	fmodel->frames = Hunk_Alloc(fmodel->header.num_frames * fmodel->header.framesize);

	for (i = 0; i < fmodel->header.num_frames; i++)
	{
		pinframe = (fmaliasframe_t *)((byte *)buffer + i * fmodel->header.framesize);
		poutframe = (fmaliasframe_t *)((byte *)fmodel->frames + i * fmodel->header.framesize);

		memcpy(poutframe->name, pinframe->name, sizeof(poutframe->name));
		for (j = 0; j < 3; j++)
		{
			poutframe->scale[j] = pinframe->scale[j];
			poutframe->translate[j] = pinframe->translate[j];
		}

		// verts are all 8 bit, so no swapping needed
		memcpy(poutframe->verts, pinframe->verts, fmodel->header.num_xyz * sizeof(fmtrivertx_t));
	}
}

/*
=============
Mod_SerializeGLCmds
=============
*/
void Mod_SerializeGLCmds(fmdl_t *fmodel, int version, int length, char *buffer)
{
	if (fmodel->glcmds != NULL)
	{
		ri.Com_Error(ERR_FATAL, "Duplicate glCmds block!\n");
		return;
	}

	if (version != FM_GLCMDS_VER)
	{
		ri.Com_Error(ERR_FATAL, "Invalid header version for block");
		return;
	}

	int* poutcmd = (int *)Hunk_Alloc(sizeof(int) * fmodel->header.num_glcmds);
	fmodel->glcmds = (long*)poutcmd;
	int* pincmd = (int *)((byte *)buffer);
	for (int i = 0; i < fmodel->header.num_glcmds; i++)
		poutcmd[i] = (pincmd[i]);
}

/*
=============
Mod_SerializeMeshNodes
=============
*/
void Mod_SerializeMeshNodes(fmdl_t *fmodel, int version, int length, char *buffer)
{
	int				i;
	fmmeshnode_t	*node;

	if (fmodel->mesh_nodes != NULL)
	{
		ri.Com_Error(ERR_FATAL, "Duplicate mesh node block!\n");
		return;
	}

	if (version != FM_MESH_VER)
	{
		ri.Com_Error(ERR_FATAL, "Invalid mesh version");
		return;
	}

	if (fmodel->header.num_mesh_nodes > 0)
	{
		fmodel->mesh_nodes = Hunk_Alloc(sizeof(fmmeshnode_t) * fmodel->header.num_mesh_nodes);
		for (i = 0, node = (fmmeshnode_t *)buffer; i < fmodel->header.num_mesh_nodes; i++, node++)
		{
			memcpy(&fmodel->mesh_nodes[i], node, sizeof(fmmeshnode_t));
		}
	}
}

/*
=============
Mod_SerializeShortFrames
=============
*/
void Mod_SerializeShortFrames(fmdl_t *fmodel, int version, int length, char *buffer)
{
	// ignoring data
}

/*
=============
Mod_SerializeShortFrames
=============
*/
void Mod_SerializeNormal(fmdl_t *fmodel, int version, int length, char *buffer)
{
	if (fmodel->lightnormalindex != NULL)
	{
		ri.Com_Error(ERR_FATAL, "Duplicate light normal block!\n");
		return;
	}
	if (version != FM_NORMAL_VER)
	{
		ri.Com_Error(ERR_FATAL, "Invalid normal version\n");
		return;
	}
	fmodel->lightnormalindex = (byte *)Hunk_Alloc(fmodel->header.num_xyz * sizeof(byte));
	memcpy(fmodel->lightnormalindex, buffer, fmodel->header.num_xyz * sizeof(byte));
}

/*
=============
Mod_SerializeComp
=============
*/
void Mod_SerializeComp(fmdl_t *fmodel, int version, int length, char *buffer)
{
	// ignoring data
}

/*
=============
Mod_SerializeSkeleton
=============
*/
void Mod_SerializeSkeleton(fmdl_t *fmodel, int version, int length, char *buffer)
{
	int		i, j, k;
	int		*basei;
	int		runningTotalVertices = 0;
	int		indexBase = 0;
	int		m_numClusters = 0;
	float	*basef;

	if (fmodel->skeletons != NULL)
	{
		ri.Com_Error(ERR_FATAL, "Duplicate skeleton block!\n");
		return;
	}
	if (version != FM_SKELETON_VER)
	{
		ri.Com_Error(ERR_FATAL, "Invalid skeleton version\n");
		return;
	}

	basei = (int *)buffer;

	fmodel->skeletalType = *basei;

	m_numClusters = *(++basei);

	fmodel->rootCluster = CreateSkeleton(fmodel->skeletalType);
	

	//m_jointConstraintAngles = new rangeVector_t[m_numClusters];
	////m_modelJointAngles = new D3DVECTOR[m_numClusters];
	//m_skeletalClusters = new M_SkeletalCluster_t[m_numClusters];
	//
	for (i = m_numClusters - 1; i >= 0; --i)
	{
		//m_jointConstraintAngles[i].min.x = 0;
		//m_jointConstraintAngles[i].min.y = 0;
		//m_jointConstraintAngles[i].min.z = 0;
		//m_jointConstraintAngles[i].max.x = 0;
		//m_jointConstraintAngles[i].max.y = 0;
		//m_jointConstraintAngles[i].max.z = 0;
		//m_modelJointAngles[i].x = 0;
		//m_modelJointAngles[i].y = 0;
		//m_modelJointAngles[i].z = 0;
	
		runningTotalVertices += *(++basei);
		//m_skeletalClusters[i].numVerticies = runningTotalVertices;
		//m_skeletalClusters[i].verticies = (int*)malloc(m_skeletalClusters[i].numVerticies * sizeof(int));
	}

	for (j = m_numClusters - 1; j >= 0; --j)
	{
		for (i = indexBase; i < SkeletalClusters[fmodel->rootCluster + j].numVerticies; ++i)
		{
			++basei;

			for (k = 0; k <= j; ++k)
			{
				SkeletalClusters[fmodel->rootCluster + k].verticies[i] = *basei;
			}
		}

		indexBase = SkeletalClusters[fmodel->rootCluster + j].numVerticies;
	}

	if (*(++basei))
	{
		basef = (float *)++basei;

		fmodel->skeletons = (ModelSkeleton_t*)Hunk_Alloc(fmodel->header.num_frames * sizeof(ModelSkeleton_t));

		for (i = 0; i < fmodel->header.num_frames; ++i)
		{
			CreateSkeletonAsHunk(fmodel->skeletalType, &fmodel->skeletons[i]);
			for (j = 0; j < m_numClusters; ++j)
			{
				fmodel->skeletons[i].rootJoint[j].model.origin[0] = *(basef++);
				fmodel->skeletons[i].rootJoint[j].model.origin[1] = *(basef++);
				fmodel->skeletons[i].rootJoint[j].model.origin[2] = *(basef++);

				fmodel->skeletons[i].rootJoint[j].model.direction[0] = *(basef++);
				fmodel->skeletons[i].rootJoint[j].model.direction[1] = *(basef++);
				fmodel->skeletons[i].rootJoint[j].model.direction[2] = *(basef++);

				fmodel->skeletons[i].rootJoint[j].model.up[0] = *(basef++);
				fmodel->skeletons[i].rootJoint[j].model.up[1] = *(basef++);
				fmodel->skeletons[i].rootJoint[j].model.up[2] = *(basef++);
			}
		}

	}
	else
	{
		fmodel->header.num_xyz -= m_numClusters * 3;
	}
}

/*
=============
Mod_SerializeReferences
=============
*/
void Mod_SerializeReferences(fmdl_t *fmodel, int version, int length, char *buffer)
{
	// should be using, not sure why yet .
}


/*
=============
Mod_LoadFlexModel
=============
*/
void Mod_LoadFlexModel(struct model_s *mod, void *model_buffer, int filesize)
{
	header_t *header;
	char *buffer = model_buffer;
	int version;
	int size;
	fmdl_t *fmodel;
	int i;

	mod->fmodel = Hunk_Alloc(sizeof(fmdl_t));
	fmodel = mod->fmodel;

	header = (header_t *)buffer;
	while (filesize > 0)
	{
		i = 0;
		char* blockname = buffer;

		while (m_fmblocks[i].blocktype >= 0)
		{
			if (strcmpi(buffer, m_fmblocks[i].blockid) == 0)
			{
				break;
			}
			i++;
		}

		buffer += sizeof(m_fmblocks[0].blockid);
		version = *(int*)buffer;
		buffer += sizeof(version);
		size = *(int*)buffer;
		buffer += sizeof(size);
		filesize = filesize - sizeof(m_fmblocks[0].blockid) - sizeof(version) - sizeof(size);

		switch (m_fmblocks[i].blocktype)
		{
		case FM_BLOCK_HEADER:
			Mod_SerializeHeader(fmodel, version, size, buffer);
			break;
		case FM_BLOCK_SKIN:
			Mod_SerializeSkin(fmodel, version, size, buffer);
			break;
		case FM_BLOCK_ST:
			Mod_SerializeSt(fmodel, version, size, buffer);
			break;
		case FM_BLOCK_TRIS:
			Mod_SerializeTris(fmodel, version, size, buffer);
			break;
		case FM_BLOCK_FRAMES:
			Mod_SerializeFrames(fmodel, version, size, buffer);
			break;
		case FM_BLOCK_GLCMDS:
			Mod_SerializeGLCmds(fmodel, version, size, buffer);
			break;
		case FM_BLOCK_MESHNODES:
			Mod_SerializeMeshNodes(fmodel, version, size, buffer);
			break;
		case FM_BLOCK_SHORTFRAMES:
			Mod_SerializeShortFrames(fmodel, version, size, buffer);
			break;
		case FM_BLOCK_NORMAL:
			Mod_SerializeNormal(fmodel, version, size, buffer);
			break;
		case FM_BLOCK_COMP:
			Mod_SerializeComp(fmodel, version, size, buffer);
			break;
		case FM_BLOCK_SKELETON:
			Mod_SerializeSkeleton(fmodel, version, size, buffer);
			break;
		case FM_BLOCK_REFERENCES:
			Mod_SerializeReferences(fmodel, version, size, buffer);
			break;
		default:
			ri.Com_Error(ERR_FATAL, "Unknown block %s\n", blockname);			
		}
		filesize -= size;
		buffer += size;
	}

	// Load in our skins.
	mod->skins[0] = GL_FindImage(fmodel->skin_names, it_pic);

	mod->type = mod_flex;
	mod->mins[0] = -32;
	mod->mins[1] = -32;
	mod->mins[2] = -32;
	mod->maxs[0] = 32;
	mod->maxs[1] = 32;
	mod->maxs[2] = 32;
}
/*
=============
R_LerpFrameVertexes
=============
*/
void R_LerpFrameVertexes(vec3_t newPoint, vec3_t oldPoint, vec3_t interpolatedPoint, float backlerp)
{
	interpolatedPoint[0] = newPoint[0] + backlerp * (oldPoint[0] - newPoint[0]);
	interpolatedPoint[1] = newPoint[1] + backlerp * (oldPoint[1] - newPoint[1]);
	interpolatedPoint[2] = newPoint[2] + backlerp * (oldPoint[2] - newPoint[2]);
}

/*
=============
R_RenderFlexModel
=============
*/
void R_RenderFlexModel(fmdl_t *model, int currentFrame, int oldframenum, vec3_t oldorigin, vec3_t origin, vec3_t angles, float backlerp)
{
	fmaliasframe_t *frame;
	fmaliasframe_t *oldframe;
	int i;
	fmtrivertx_t	*v;
	fmtrivertx_t	*ov;
	long* command = model->glcmds;
	int cur_glcmnd = model->header.num_glcmds;

	static vec3_t frame_vertexes[65536];
	static vec3_t old_frame_vertexes[65536];

	frame = (fmaliasframe_t*)((char *)model->frames + model->header.framesize	* currentFrame);
	oldframe = (fmaliasframe_t*)((char *)model->frames + model->header.framesize	* oldframenum);
	for (i = 0; i < model->header.num_xyz; i++)
	{
		v = &frame->verts[i];
		ov = &oldframe->verts[i];

		frame_vertexes[i][0] = v->v[0];
		frame_vertexes[i][1] = v->v[1];
		frame_vertexes[i][2] = v->v[2];

		if (oldframenum != currentFrame)
		{
			old_frame_vertexes[i][0] = ov->v[0];
			old_frame_vertexes[i][1] = ov->v[1];
			old_frame_vertexes[i][2] = ov->v[2];
		}
	}

	if (model->skeletons != NULL)
	{
		int rootJoint = -1;
		RotateModelSegments(&model->skeletons[currentFrame], 0, model->rootCluster, rootJoint, frame_vertexes);
	}

	if (command[0] == 0)
	{
		command = &command[1];
	}
	
	

	while (*command && cur_glcmnd)
	{
		cur_glcmnd--;

		int num_verts;
		if (*command > 0)
		{
			//triangle strip
			num_verts = *command;
			glBegin(GL_TRIANGLE_STRIP);
		}
		else
		{
			//triangle fan
			num_verts = -(*command);
			glBegin(GL_TRIANGLE_FAN);
		}
		command++;

		for (i = 0; i < num_verts; i++)
		{
			vec3_t position;
			vec3_t oldposition;
			vec3_t finalposition;
			vec2_t st;
			int vert_index;

			//grab the floating point s and t
			st[0] = (*((float *)command)) * 1; command++;
			st[1] = (*((float *)command)) * 1; command++;

			vert_index = *command; command++;

			if (oldframenum == currentFrame)
			{
				position[0] = (frame_vertexes[vert_index][0] * frame->scale[0]) + frame->translate[0];
				position[1] = (frame_vertexes[vert_index][1] * frame->scale[1]) + frame->translate[1];
				position[2] = (frame_vertexes[vert_index][2] * frame->scale[2]) + frame->translate[2];

				glTexCoord2f(st[0], st[1]);
				glVertex3fv(position);
			}
			else
			{
				position[0] = (frame_vertexes[vert_index][0] * frame->scale[0]) + frame->translate[0];
				position[1] = (frame_vertexes[vert_index][1] * frame->scale[1]) + frame->translate[1];
				position[2] = (frame_vertexes[vert_index][2] * frame->scale[2]) + frame->translate[2];

				oldposition[0] = (old_frame_vertexes[vert_index][0] * oldframe->scale[0]) + oldframe->translate[0];
				oldposition[1] = (old_frame_vertexes[vert_index][1] * oldframe->scale[1]) + oldframe->translate[1];
				oldposition[2] = (old_frame_vertexes[vert_index][2] * oldframe->scale[2]) + oldframe->translate[2];

				R_LerpFrameVertexes(position, oldposition, finalposition, backlerp);

				glTexCoord2f(st[0], st[1]);
				glVertex3fv(finalposition);
			}
		}
		glEnd();

		if (command[0] == 0)
		{
			command++;
		}
	}
}

/*
=============
R_DrawFlexModel
=============
*/
void R_DrawFlexModel(entity_t *e)
{
	vec3_t		mins, maxs;
	int			i;
	qboolean	rotated;
	model_t		*model;
	vec3_t		ambientLight, directedLight;

	currententity = e;
	gl_state.currenttextures[0] = gl_state.currenttextures[1] = -1;

	if (e->angles[0] || e->angles[1] || e->angles[2])
	{
		rotated = true;
		for (i = 0; i < 3; i++)
		{
			mins[i] = e->origin[i] - currentmodel->radius;
			maxs[i] = e->origin[i] + currentmodel->radius;
		}
	}
	else
	{
		rotated = false;
		VectorAdd(e->origin, currentmodel->mins, mins);
		VectorAdd(e->origin, currentmodel->maxs, maxs);
	}

	if (R_CullBox(mins, maxs))
		return;

	glColor3f(1, 1, 1);


	glPushMatrix();
	e->angles[0] = -e->angles[0];	// stupid quake bug
	e->angles[2] = -e->angles[2];	// stupid quake bug
	R_RotateForEntity(e);
	e->angles[0] = -e->angles[0];	// stupid quake bug
	e->angles[2] = -e->angles[2];	// stupid quake bug

	maxs[0] = e->origin[0] + 10;
	maxs[1] = e->origin[1] + 10;
	maxs[2] = e->origin[2] + 10;

	mins[0] = e->origin[0] - 10;
	mins[1] = e->origin[1] - 10;
	mins[2] = e->origin[2] - 10;

	GL_EnableMultitexture(false);
	GL_SelectTexture(0);
	GL_TexEnv(GL_REPLACE);
	//GL_SelectTexture(GL_TEXTURE1_SGIS);
	//GL_TexEnv(GL_MODULATE);

	GL_Bind(e->model[0]->skins[0]->texnum);
	R_RenderFlexModel(e->model[0]->fmodel, e->frame, e->oldframe, e->oldorigin, e->origin, e->angles, e->backlerp);

	GL_EnableMultitexture(false);

	glPopMatrix();
}