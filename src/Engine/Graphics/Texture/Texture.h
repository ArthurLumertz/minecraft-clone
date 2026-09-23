#pragma once

#include <string>
#include "Region.h"

class Texture {
private:
	unsigned int m_id = 0;

	int m_width;
	int m_height;
	unsigned char* m_rawPixels;

public:
	Texture(const std::string& filePath, bool flipY = true);
	~Texture();

	void bind(int sampler) const;

	int getWidth() const { return m_width; }
	int getHeight() const { return m_height; }
	unsigned char* getRawPixels() const { return m_rawPixels; }
};