/*
File: Vector.h

This file contains data structures for 2d and 3d vectors.

*/

#pragma once

struct vec2f
{
	vec2f(float _x = 0.0f, float _y = 0.0f) : x(_x), y(_y){}

	float x;
	float y;
};

struct vec3f
{
	vec3f(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f ) : x(_x), y(_y), z(_z) {}
	float x;
	float y;
	float z;
};

