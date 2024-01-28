// Particle Simulator.cpp : Defines the entry point for the application.
//

#include "ParticleSimulator.h"
#include "SimulatorGUI.h"

using namespace std;

int main()
{
	SimulatorGUI gui;
	gui.Init();

	while (1) {
		gui.Update();
		gui.Render();
	}

	gui.Shutdown();

	return 0;
}
