#pragma once

#include <imgui.h>

class SimulatorGUI {
public:
	void Init();
	virtual void Update();
	void Render();
	void Shutdown();
};