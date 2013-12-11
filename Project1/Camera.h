#pragma once

#include "math3d.h"

class Camera
{
private:
	void Init();
	void Update();

	Vector3f m_pos;
	Vector3f m_target;
	Vector3f m_up;

	int m_windowHeight;
	int m_windowWidth;

	float m_AngleH; //horizontal
	float m_AngleV; //vertical

	bool m_OnUpperEdge;
	bool m_OnLowerEdge;
	bool m_OnLeftEdge;
	bool m_OnRightEdge;

	Vector2i m_MousePos;
public:
	Camera(int WindowWidth, int WindowHeight);
	Camera(int WindowWidht, int WindowHeight, const Vector3f& Pos, const Vector3f& Target, const Vector3f& Up);
	bool OnKeyboard(int key);
	void OnMouse(int x, int y);
	void setAngleH(float angle);
	void setAngleV(float angle);

	void OnRender();

	const Vector3f& GetPos() const
	{
		return m_pos;
	}

	const Vector3f& GetTarget() const
	{
		return m_target;
	}

	const Vector3f& GetUp() const
	{
		return m_up;
	}

};