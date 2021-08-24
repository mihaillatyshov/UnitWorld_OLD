#pragma once

#include <unordered_map>
#include <GL/glew.h>
#include <glm.hpp>
#include <TestBoneAnim/math_3d.h>
#include "Material.h"

class ShaderPaths
{
public:
	ShaderPaths(std::string_view vertPath, std::string_view fragPath, std::string_view geomPath = "") 
		: m_VertPath(vertPath), m_FragPath(fragPath), m_GeomPath(geomPath)
	{}

	inline std::string_view GetVertPath() { return m_VertPath; }
	inline std::string_view GetFragPath() { return m_FragPath; }
	inline std::string_view GetGeomPath() { return m_GeomPath; }
protected:
	std::string m_VertPath;
	std::string m_FragPath;
	std::string m_GeomPath = "";
};

class Shader
{
protected:
	GLint GetUniformLocation(const std::string &name) const;
	GLuint Load();
	static std::string LoadShaderFile(std::string_view filepath);
	GLuint CreateShaderByType(std::string_view shaderPath, int shaderType);
public:
	Shader(const ShaderPaths& shaderPaths, bool initArray = true);
	~Shader();

	void Enable() const;
	void Disable() const;

	void Recompile();

	void SetUniform1f(const std::string &name, float value);
	void SetUniform1fv(const std::string &name, float* value, int count);
	void SetUniform1i(const std::string &name, int value);
	void SetUniform1iv(const std::string &name, int* value, int count);
	void SetUniform2f(const std::string &name, const glm::vec2 &vector);
	void SetUniform3f(const std::string &name, const glm::vec3 &vector);
	void SetUniform4f(const std::string &name, const glm::vec4 &vector);
	void SetUniformMat4(const std::string &name, const glm::mat4& matrix);
	void SetUniformMat4(const std::string &name, const Matrix4f& matrix);
	void SetMaterial(const GLuint id, const Material *material, const MaterialInfo *material_info);
protected:
	GLuint m_ShaderID;
	ShaderPaths m_ShaderPaths;
	mutable std::unordered_map<std::string, GLint> m_UniformLocationCache;
};