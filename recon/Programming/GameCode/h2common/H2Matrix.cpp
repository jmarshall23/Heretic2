// H2Matrix.c
//

extern "C" {
	#include "../qcommon/H2Common.h"
	#include "../qcommon/ResourceManager.h"
	#include "../qcommon/Matrix.h"
	#include "../qcommon/Vector.h"
};

#include "Math/Matrix.h"

H2COMMON_API_CPLUSPLUS int HACK_Pitch_Adjust = 0; // weak

H2COMMON_API_CPLUSPLUS void CreateIdentityMatrix(matrix4_t matrix)
{
	memset(matrix, 0, sizeof(matrix4_t));
	matrix[0][0] = 1.0f;
	matrix[1][1] = 1.0f;
	matrix[2][2] = 1.0f;
	matrix[3][3] = 1.0f;
}

H2COMMON_API_CPLUSPLUS void TranslateMatrix(matrix4_t matrix, vec3_t xyz)
{
	matrix[3][0] += xyz[0];
	matrix[3][1] += xyz[1];
	matrix[3][2] += xyz[2];
}

H2COMMON_API_CPLUSPLUS void CreatePitchMatrix(matrix3_t matrix, vec_t angles)
{
	memset(&matrix[0], 0, sizeof(matrix3_t));

	matrix[0][0] = cos(-angles);
	matrix[2][2] = matrix[0][0];
	matrix[1][1] = 1;
	matrix[2][0] = sin(-angles);
	matrix[0][2] = -matrix[2][0];
}

H2COMMON_API_CPLUSPLUS void CreateYawMatrix(matrix3_t matrix, vec_t angles)
{
	// Rotation about the z axis
	memset(&matrix[0], 0, sizeof(matrix3_t));

	matrix[0][0] = cos(angles);
	matrix[1][1] = matrix[0][0];
	matrix[0][1] = sin(angles);
	matrix[1][0] = -matrix[0][1];
	matrix[2][2] = 1;
}

H2COMMON_API_CPLUSPLUS void CreateRollMatrix(matrix3_t matrix, vec_t angles)
{
	memset(&matrix[0], 0, sizeof(matrix3_t));

	// Rotation about the x axis
	matrix[0][0] = 1;
	matrix[1][1] = cos(angles);
	matrix[2][2] = matrix[1][1];
	matrix[1][2] = sin(angles);
	matrix[2][1] = -matrix[1][2];
}


H2COMMON_API_CPLUSPLUS void Matrix3MultByMatrix3(matrix3_t A, matrix3_t B, matrix3_t C)
{
	CMatrix::Matrix3MultByMartrix3(A, B, C);
}

H2COMMON_API_CPLUSPLUS void Matrix3FromAngles(vec3_t angles, matrix3_t rotationMatrix)
{
	CMatrix::Matrix3FromAngles(angles, rotationMatrix);
}

H2COMMON_API_CPLUSPLUS void IMatrix3FromAngles(vec3_t angles, matrix3_t rotationMatrix)
{
	matrix3_t yawMatrix;
	matrix3_t pitchMatrix;
	matrix3_t rollMatrix;
	matrix3_t pitchRollMatrix;

#if 1
	memset(rollMatrix, 0, sizeof(rollMatrix));

	// Rotation about the x axis
	rollMatrix[0][0] = 1;
	rollMatrix[1][1] = cos(angles[PITCH]);
	rollMatrix[2][2] = rollMatrix[1][1];
	rollMatrix[1][2] = sin(angles[PITCH]);
	rollMatrix[2][1] = -rollMatrix[1][2];

	// Rotation about the y axis
	memset(pitchMatrix, 0, sizeof(pitchMatrix));

	pitchMatrix[0][0] = cos(angles[YAW]);
	pitchMatrix[2][2] = pitchMatrix[0][0];
	pitchMatrix[1][1] = 1;
	pitchMatrix[2][0] = sin(angles[YAW]);
	pitchMatrix[0][2] = -pitchMatrix[2][0];

	// Rotation about the z axis
	memset(yawMatrix, 0, sizeof(yawMatrix));

	yawMatrix[0][0] = cos(angles[ROLL]);
	yawMatrix[1][1] = yawMatrix[0][0];
	yawMatrix[0][1] = sin(angles[ROLL]);
	yawMatrix[1][0] = -yawMatrix[0][1];
	yawMatrix[2][2] = 1;

	Matrix3MultByMatrix3(pitchMatrix, rollMatrix, pitchRollMatrix);

	Matrix3MultByMatrix3(yawMatrix, pitchRollMatrix, rotationMatrix);
#else
	// Rotation about the y axis
	memset(pitchMatrix, 0, sizeof(pitchMatrix));

	pitchMatrix[0][0] = cos(angles[YAW]);
	pitchMatrix[2][2] = pitchMatrix[0][0];
	pitchMatrix[1][1] = 1;
	pitchMatrix[2][0] = sin(angles[YAW]);
	pitchMatrix[0][2] = -pitchMatrix[2][0];

	memset(rollMatrix, 0, sizeof(rollMatrix));

	// Rotation about the x axis
	rollMatrix[0][0] = 1;
	rollMatrix[1][1] = cos(angles[PITCH]);
	rollMatrix[2][2] = rollMatrix[1][1];
	rollMatrix[1][2] = sin(angles[PITCH]);
	rollMatrix[2][1] = -rollMatrix[1][2];

	// Rotation about the z axis
	memset(yawMatrix, 0, sizeof(yawMatrix));

	yawMatrix[0][0] = cos(angles[ROLL]);
	yawMatrix[1][1] = yawMatrix[0][0];
	yawMatrix[0][1] = sin(angles[ROLL]);
	yawMatrix[1][0] = -yawMatrix[0][1];
	yawMatrix[2][2] = 1;

	Matrix3MultByMartrix3(rollMatrix, pitchMatrix, pitchRollMatrix);

	Matrix3MultByMartrix3(yawMatrix, pitchRollMatrix, rotationMatrix);
#endif
}

H2COMMON_API_CPLUSPLUS void Matrix3MultByVec3(matrix3_t A, vec3_t B, vec3_t C)
{
	CMatrix::Matrix3MultByVec3(A, B, C);
}

H2COMMON_API_CPLUSPLUS double Matricies3FromDirAndUp(vec3_t direction, vec3_t up, matrix3_t toLocal, matrix3_t fromLocal)
{
	return CMatrix::Matricies3FromDirAndUp(direction, up, fromLocal, toLocal);
}

H2COMMON_API_CPLUSPLUS void RotatePointAboutLocalOrigin(matrix3_t rotation, vec3_t origin, vec3_t point)
{
	CMatrix::RotatePointAboutLocalOrigin(rotation, origin, point);
}

H2COMMON_API_CPLUSPLUS void TransformPoint(matrix3_t rotation, vec3_t origin, vec3_t newOrigin, vec3_t point)
{
	vec3_t vec; // [sp+4h] [bp-Ch]@1

	point[0] = point[0] - origin[0];
	point[1] = point[1] - origin[1];
	point[2] = point[2] - origin[2];
	Matrix3MultByVec3(rotation, point, vec);
	point[0] = vec[0] + newOrigin[0];
	point[1] = vec[1] + newOrigin[1];
	point[2] = vec[2] + newOrigin[2];
}