#pragma once
#include "math3d.h"

/*
The pipeline class abstracts the details of getting all the transformation required for one single object combined. 
There are currently 3 private member vectors that store the scaling, position in world space and rotation for each 
axis. In addition there are APIs to set them and a function to get the matrix that represent the sum of all these 
transformations.
*/

class Pipeline
{
private:
	Vector3f m_scale;
	Vector3f m_worldPos;
	Vector3f m_rotateInfo;

	struct 
	{
		float FOV; //filed of viev
		float Width; //Aspect ratio width
		float Height; //Aspect ratio height
		float zNear; //near clipping distance
		float zFar; //far clipping distance
	}m_persProj;

	struct
	{
		Vector3f Pos;
		Vector3f Target;
		Vector3f Up;
	}m_camera;

	Matrix4f m_trasformation; //matrix that will be transfomred
public:
	Pipeline()
	{
		m_scale = Vector3f(1.0f, 1.0f, 1.0f);
		m_worldPos = Vector3f(0.0f, 0.0f, 0.0f);
		m_rotateInfo = Vector3f(0.0f, 0.0f, 0.0f);
	}

	void Scale(float scaleX, float scaleY, float scaleZ)
	{
		m_scale.x = scaleX;
		m_scale.y = scaleY;
		m_scale.z = scaleZ;
	}

	void WorldPos(float x, float y, float z)
	{
		m_worldPos.x = x;
		m_worldPos.y = y;
		m_worldPos.z = z;
	}

	void Rotate(float rotateX, float rotateY, float rotateZ)
	{
		m_rotateInfo.x = rotateX;
		m_rotateInfo.y = rotateY;
		m_rotateInfo.z = rotateZ;
	}

	Vector3f getScale()
	{
		return this->m_scale;
	}

	Vector3f getWorldPos()
	{
		return this->m_worldPos;
	}

	Vector3f getRotation()
	{
		return this->m_rotateInfo;
	}


	void SetPerpectiveProj(float FOV, float Width, float Height, float zNear, float zFar)
	{
		m_persProj.FOV = FOV;
		m_persProj.Width = Width;
		m_persProj.Height = Height;
		m_persProj.zNear = zNear;
		m_persProj.zFar = zFar;

	}

	void SetCamera(const Vector3f& Pos, const Vector3f Target, const Vector3f& Up)
	{
		m_camera.Pos = Pos;
		m_camera.Target = Target;
		m_camera.Up = Up;
	}

	const Matrix4f* GetTrans();

};