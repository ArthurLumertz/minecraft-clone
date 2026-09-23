#pragma once

#include "Shader.h"
#include "../Camera/Camera.h"
#include <string>
#include <glm/glm.hpp>

class StaticShader : public Shader {
private:
	int location_projectionViewMatrix;
	int location_modelMatrix;
	int location_cameraPosition;
	int location_uvRegion;
	int location_sampler;
	int location_useSolidColor;
	int location_solidColor;
	int location_useFog;
	int location_sunDirection;
	int location_useLighting;
	int location_fogColor;

protected:
	void bindAttributes() override;
	void getUniformLocations() override;

public:
	StaticShader(const std::string& shaderName);

	void setModelMatrix(const glm::mat4& model);
	void setCamera(Camera& camera);
	void setUVRegion(float u0, float v0, float u1, float v1);
	void setSampler(int sampler);
	void setUseSolidColor(bool value);
	void setSolidColor(float r, float g, float b, float a);
	void setUseFog(bool fog);
	void setSunDirection(glm::vec3& direction);
	void setUseLighting(bool lighting);
	void setFogColor(glm::vec3 fogColor);

};