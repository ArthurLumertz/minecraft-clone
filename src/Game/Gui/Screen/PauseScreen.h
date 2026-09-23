#pragma once

#include "Screen.h"

class Gui;

class PauseScreen : public Screen {
public:
	PauseScreen(Gui& gui);

	void init() override;

private:
	Gui& m_gui;

};