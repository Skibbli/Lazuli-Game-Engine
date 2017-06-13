#ifndef _STDAFX_H_
#define _STDAFX_H_

#include "glew.h"

#include "glm.hpp"
#include "gtc\matrix_transform.hpp"
#include "gtx\transform.hpp"
#include "gtc\type_ptr.hpp"

#include "allegro5/allegro.h"
#include "allegro5/allegro_font.h"
#include "allegro5/allegro_image.h"
#include <allegro5\allegro_audio.h>
#include <allegro5\allegro_acodec.h>
#include <allegro5\allegro_ttf.h>
#include <allegro5\allegro_primitives.h>
#include "allegro5/allegro_opengl.h"

#include "btBulletDynamicsCommon.h"

#include <memory>
#include <vector>
#include <string>

#define weak std::weak_ptr
#define shared std::shared_ptr

#define Vec3 glm::vec3
#define Vec2 glm::vec2
#define Mat4 glm::mat4
#define Quat glm::quat
#define Uint unsigned int

enum AXIS { X_AXIS, Y_AXIS, Z_AXIS };

struct VertexIndex
{
	int vertIndex, texIndex, normIndex;
};

class Mesh;
class Material;

struct ModelPart
{
	ModelPart()
	{
		name = "";
	}

	std::string name;
	weak<Mesh> mesh;
	weak<Material> material;
	Vec3 offset;
};

#endif