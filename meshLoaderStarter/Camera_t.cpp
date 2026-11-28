#include "Camera_t.h"

void Camera_t::Relocate(DirectX::SimpleMath::Vector3 newPos, DirectX::SimpleMath::Vector3 newTarget)
{
	if (debugMode)
	{
		position = DebugPosition;
		target = DebugTarget;
	}
	else
	{
		position = newPos;
		target = newTarget;
	}
}

void Camera_t::SetDebugMode(bool dbMode)
{
	debugMode = dbMode;
}

void Camera_t::ToggleDebugMode()
{
	debugMode = (!debugMode);
}

DirectX::SimpleMath::Vector3& Camera_t::GetPositionRef()
{
	return position;
}

DirectX::SimpleMath::Vector3& Camera_t::GetTargetRef()
{
	return target;
}


