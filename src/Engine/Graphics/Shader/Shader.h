#pragma once

#include <glm/glm.hpp>
#include <string>

class Shader {
private:
	unsigned int m_programId;

	unsigned int createShader(const std::string& shaderFile, int shaderType);
	const std::string& loadShaderSource(const std::string& shaderFile);

protected:
	int getUniformLocation(const std::string& uniformName);

	void setUniformFloat3f(int location, float x, float y, float z);
	void setUniformFloat4f(int location, float x, float y, float z, float w);
	void setUniformMatrix4f(int location, const glm::mat4& value);
	void setUniformInt1i(int location, int x);

	virtual void bindAttributes() = 0;
	virtual void getUniformLocations() = 0;

	void bindAttribLocation(int index, const std::string& attribName);

public:
	Shader(const std::string& shaderName);
	virtual ~Shader();

	void bind() const;
	void unbind() const;

};
