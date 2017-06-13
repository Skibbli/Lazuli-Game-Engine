#ifndef MATHS_H
#define MATHS_H

#include "stdafx.h"


/// Returns the translation matrix
glm::mat4 GetTransMat(Vec3 _pos, Vec3 _rot, Vec3 _scale);
/// Returns the translation matrix
glm::mat4 GetTransMat(Vec3 _pos, Quat _rot, Vec3 _scale);

#endif