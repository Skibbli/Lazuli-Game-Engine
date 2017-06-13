#include "stdafx.h"

#include "Maths.h"

glm::mat4 GetTransMat(Vec3 _pos, Vec3 _rot, Vec3 _scale)
{
	glm::mat4 mat(1.0f);

	mat = glm::translate(mat, _pos);

	mat = glm::rotate(mat, glm::radians(_rot.y), glm::vec3(0.0f,1.0f,0.0f));
	mat = glm::rotate(mat, glm::radians(_rot.z), glm::vec3(0.0f,0.0f,1.0f));
	mat = glm::rotate(mat, glm::radians(_rot.x), glm::vec3(1.0f,0.0f,0.0f));

	mat = glm::scale(mat, _scale);

	return mat;
}

glm::mat4 GetTransMat(Vec3 _pos, Quat _rot, Vec3 _scale)
{
	Mat4 translation(1.0f);
	translation = glm::translate(translation, _pos);

	Mat4 rotation = glm::mat4_cast(_rot);

	Mat4 scale(1.0f);
	scale = glm::scale(scale, _scale);

	return (translation * rotation * scale);
}

///Vec3::Vec3()
///{
///
///}
///
///Vec3::Vec3(float _x, float _y, float _z)
///{
///
///}
///
///Vec3::~Vec3()
///{
///
///}
///
///Vec3 Vec3::operator+(const Vec3& _vec)
///{
///	Vec3 vec;
///
///	vec.x = this->x + _vec.x;
///	vec.y = this->y + _vec.y;
///	vec.z = this->z + _vec.z;
///
///	return vec;
///}
///
///Vec3 Vec3::operator-(const Vec3& _vec)
///{
///	Vec3 vec;
///
///	vec.x = this->x - _vec.x;
///	vec.y = this->y - _vec.y;
///	vec.z = this->z - _vec.z;
///
///	return vec;
///}
///
///float Vec3::operator*(const Vec3& _vec)
///{
///	float val;
///
///	val = (this->x * _vec.x) + (this->y * _vec.y) + (this->z * _vec.z);
///
///	return val;
///}
///
///Vec3 Vec3::operator/(const Vec3& _vec)
///{
///	Vec3 vec;
///
///	vec.x = (this->y * _vec.z) - (this->z * _vec.y);
///	vec.y = (this->z * _vec.x) - (this->x * _vec.z);
///	vec.z = (this->x * _vec.y) - (this->y * _vec.x);
///
///	return vec;
///}
///
///
///void Vec3::operator=(const Vec3& _vec)
///{
///	this->x = _vec.x;
///	this->y = _vec.y;
///	this->z = _vec.z;
///}
///
///
///
///Matrix4x4::Matrix4x4()
///{
///
///}
///
///Matrix4x4::Matrix4x4(float _x, float _y, float _z, float _w)
///{
///
///}
///
///Matrix4x4::~Matrix4x4()
///{
///
///}
