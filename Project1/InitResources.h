#pragma once
#include "math3d.h"

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 1024

struct Vertex
{
	Vector3f m_pos;
	Vector2f m_tex;

	Vertex()
	{}

	Vertex(Vector3f pos, Vector2f tex)
	{
		m_pos = pos;
		m_tex = tex;
	}

};

