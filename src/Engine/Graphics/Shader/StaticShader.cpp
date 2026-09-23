#include "StaticShader.h"

StaticShader::StaticShader(const std::string& shaderName) : Shader(shaderName) {
	bindAttributes();
	getUniformLocations();
}

void StaticShader::bindAttributes() {
	bindAttribLocation(0, "a_pos");
	bindAttribLocation(1, "a_texCoords");
	bindAttribLocation(2, "a_normals");
	bindAttribLocation(3, "a_colors");
}

void StaticShader::getUniformLocations() {
	location_projectionViewMatrix = getUniformLocation("u_projectionViewMatrix");
	location_modelMatrix = getUniformLocation("u_modelMatrix");
	location_cameraPosition = getUniformLocation("u_cameraPosition");
	location_sampler = getUniformLocation("u_sampler");
	location_uvRegion = getUniformLocation("u_uvRegion");
	location_useSolidColor = getUniformLocation("u_useSolidColor");
	location_solidColor = getUniformLocation("u_solidColor");
	location_useFog = getUniformLocation("u_useFog");
	location_sunDirection = getUniformLocation("u_sunDirection");
	location_useLighting = getUniformLocation("u_useLighting");
	location_fogColor = getUniformLocation("u_fogColor");
}

void StaticShader::setModelMatrix(const glm::mat4& model) {
	setUniformMatrix4f(location_modelMatrix, model);
}

void StaticShader::setCamera(Camera& camera) {
	setUniformMatrix4f(location_projectionViewMatrix, camera.getProjectionViewMatrix());
	setUniformFloat3f(
		location_cameraPosition,
		camera.getPosition().x,
		camera.getPosition().y,
		camera.getPosition().z
	);
}

void StaticShader::setUVRegion(float u0, float v0, float u1, float v1) {
	setUniformFloat4f(
		location_uvRegion,
		u0, v0,
		u1, v1
	);
}

void StaticShader::setSampler(int sampler) {
	setUniformInt1i(location_sampler, sampler);
}

void StaticShader::setUseSolidColor(bool value) {
	setUniformInt1i(location_useSolidColor, value);
}

void StaticShader::setSolidColor(float r, float g, float b, float a) {
	setUniformFloat4f(location_solidColor, r, g, b, a);
}

void StaticShader::setUseFog(bool fog) {
	setUniformInt1i(location_useFog, fog);
}

void StaticShader::setSunDirection(glm::vec3& direction) {
	setUniformFloat3f(location_sunDirection, direction.x, direction.y, direction.z);
}


void StaticShader::setUseLighting(bool lighting) {
	setUniformInt1i(location_useLighting, lighting);
}

void StaticShader::setFogColor(glm::vec3 fogColor) {
	setUniformFloat3f(location_fogColor, fogColor.r, fogColor.g, fogColor.b);
}