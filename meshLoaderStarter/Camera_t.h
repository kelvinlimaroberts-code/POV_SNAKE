#pragma once
#include "D3DUtil.h"
class Camera_t
{
public:
	void Relocate(DirectX::SimpleMath::Vector3 newPos = DebugPosition, DirectX::SimpleMath::Vector3 newTarget = DebugTarget);
	void SetDebugMode(bool dbMode = true);
	void ToggleDebugMode();
	DirectX::SimpleMath::Vector3& GetPositionRef();
	DirectX::SimpleMath::Vector3& GetTargetRef();
private:
	static constexpr DirectX::SimpleMath::Vector3 DebugPosition = DirectX::SimpleMath::Vector3(2, 38, 0);
	static constexpr DirectX::SimpleMath::Vector3 DebugTarget = DirectX::SimpleMath::Vector3(0, 0, 0);
	DirectX::SimpleMath::Vector3 position = DebugPosition;
	DirectX::SimpleMath::Vector3 target = DebugTarget;
	bool debugMode = false;
};

