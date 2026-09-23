#include "Texture.h"
#include <glad/glad.h>
#include <stdexcept>
#include "stb_image.h"

Texture::Texture(const std::string& filePath, bool flipY) {
	int channels;

	stbi_set_flip_vertically_on_load(flipY);

	m_rawPixels = stbi_load(filePath.c_str(), &m_width, &m_height, &channels, 0);
	if (!m_rawPixels) {
		throw std::runtime_error("Failed to load texture: " + filePath);
	}

	int mode = GL_RGBA;
	if (channels == 3) {
		mode = GL_RGB;
	}

	glGenTextures(1, &m_id);
	glBindTexture(GL_TEXTURE_2D, m_id);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		mode,
		m_width,
		m_height,
		0,
		mode,
		GL_UNSIGNED_BYTE,
		m_rawPixels
	);
}

Texture::~Texture() {
	glDeleteTextures(1, &m_id);
	stbi_image_free(m_rawPixels);
}

void Texture::bind(int sampler) const {
	glActiveTexture(GL_TEXTURE0 + sampler);
	glBindTexture(GL_TEXTURE_2D, m_id);
}