#include "Shader.h"
#include <iostream>
#include <stdexcept>
#include <glad/glad.h>
#include <fstream>
#include <sstream>
#include <glm/gtc/type_ptr.hpp>

unsigned int Shader::createShader(const std::string& shaderFile, int shaderType) {
	const std::string& shaderSource = loadShaderSource(shaderFile);

	unsigned int shaderId = glCreateShader(shaderType);
	const char* sourceCStr = shaderSource.c_str();
	glShaderSource(shaderId, 1, &sourceCStr, nullptr);
	glCompileShader(shaderId);
	
	int success;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetShaderInfoLog(shaderId, 512, nullptr, infoLog);

		std::printf(
			"ERROR: Shader Compilation Failed (%s): %s\n", 
			shaderType == GL_VERTEX_SHADER ? "vertex" : "fragment", 
			infoLog
		);
	}

	return shaderId;
}

const std::string& Shader::loadShaderSource(const std::string& shaderFile) {
	static std::string shaderSource;

	std::ifstream file(shaderFile);
	if (!file.is_open()) {
		throw std::runtime_error("Failed to find file: " + shaderFile);
		return shaderSource;
	}

	std::stringstream buffer;
	buffer << file.rdbuf();
	shaderSource = buffer.str();

	file.close();
	return shaderSource;
}

int Shader::getUniformLocation(const std::string& uniformName) {
	int location = glGetUniformLocation(m_programId, uniformName.c_str());
	if (location == -1) {
		std::cout << "Failed to find uniform name: " << uniformName << std::endl;
	}
	return location;
}

Shader::Shader(const std::string& shaderName) {
	std::string vertexFile = "resources/shaders/" + shaderName + ".vert";
	std::string fragmentFile = "resources/shaders/" + shaderName + ".frag";

	unsigned int vertexShader = createShader(vertexFile, GL_VERTEX_SHADER);
	unsigned int fragmentShader = createShader(fragmentFile, GL_FRAGMENT_SHADER);

	m_programId = glCreateProgram();
	glAttachShader(m_programId, vertexShader);
	glAttachShader(m_programId, fragmentShader);
	
	glLinkProgram(m_programId);
	int success;
	glGetProgramiv(m_programId, GL_LINK_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetProgramInfoLog(m_programId, 512, nullptr, infoLog);
		throw std::runtime_error("ERROR: Program Linking Failed: " + std::string(infoLog));
	}

	glValidateProgram(m_programId);
}


void Shader::bindAttribLocation(int index, const std::string& attribName) {
	glBindAttribLocation(m_programId, index, attribName.c_str());
}

Shader::~Shader() {
	glDeleteProgram(m_programId);
}

void Shader::bind() const {
	glUseProgram(m_programId);
}

void Shader::unbind() const {
	glUseProgram(0);
}

void Shader::setUniformFloat3f(int location, float x, float y, float z) {
	glUniform3f(location, x, y, z);
}

void Shader::setUniformFloat4f(int location, float x, float y, float z, float w) {
	glUniform4f(location, x, y, z, w);
}

void Shader::setUniformMatrix4f(int location, const glm::mat4& value) {
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setUniformInt1i(int location, int x) {
	glUniform1i(location, x);
}