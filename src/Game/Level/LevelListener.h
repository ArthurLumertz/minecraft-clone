#pragma once

class LevelListener {
public:
	virtual void onBlockChange(int x, int y, int z) = 0;
};