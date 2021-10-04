#include "Shader.h"

#include "GLCall.h"
#include "Debug/ConsoleLog.h"

#include <iostream>
#include <fstream>
#include <string>

Shader::Shader(const ShaderPaths &shaderPaths, bool initArray)
		: m_ShaderPaths(shaderPaths)
{
	m_ShaderID = Load();
	
	//if (initArray)
	//{
	//	GLint texIDs[]{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31 };
	//
	//	enable();
	//	setUniform1iv("textures", texIDs, 32);
	//	disable();
	//}
}


Shader::~Shader()
{
	GLCall(glDeleteProgram(m_ShaderID));
}


GLuint Shader::Load()
{
	GLCall(GLuint program = glCreateProgram());

	GLuint vertex = CreateShaderByType(m_ShaderPaths.GetVertPath(), GL_VERTEX_SHADER);
	GLuint fragment = CreateShaderByType(m_ShaderPaths.GetFragPath(), GL_FRAGMENT_SHADER);
	GLuint geometry = NULL;
	if (m_ShaderPaths.GetGeomPath().size())
	{
		geometry = CreateShaderByType(m_ShaderPaths.GetGeomPath(), GL_GEOMETRY_SHADER);
		if (!geometry)
			return 0;
	}

	if (!vertex || !fragment)
		return 0;

	GLCall(glAttachShader(program, vertex));
	GLCall(glAttachShader(program, fragment));
	if (m_ShaderPaths.GetGeomPath().size())
	{
		GLCall(glAttachShader(program, geometry));
	}

	GLCall(glLinkProgram(program));
	GLCall(glValidateProgram(program));

	GLint result;
	GLCall(glGetProgramiv(program, GL_LINK_STATUS, &result));
	if (result == GL_FALSE) {
		GLint length;
		GLCall(glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length));
		std::vector<char> error(length + 1000);
		GLCall(glGetProgramInfoLog(program, length, &length, &error[0]));
		LOGE("Failed to link program!\n    ", &error[0]);
		GLCall(glDeleteProgram(program));
		return 0;
	}

	GLCall(glDeleteShader(vertex));
	GLCall(glDeleteShader(fragment));
	if (m_ShaderPaths.GetGeomPath().size())
	{
		GLCall(glDeleteShader(geometry));
	}

	return program;

}


void Shader::Enable() const
{
	GLCall(glUseProgram(m_ShaderID));
}


void Shader::Disable() const
{
	GLCall(glUseProgram(0));
}


void Shader::Recompile()
{
	GLuint program = Load();

	if (program)
	{
		LOGI("Recompile ok!");
		m_ShaderID = program;
		return;
	}
	
	LOGE("Recompile failed!");
}

std::string Shader::LoadShaderFile(std::string_view filepath)
{
	std::ifstream stream(filepath.data());
	if (!stream.is_open())
		LOGE("Can't Open Shader file: ", filepath);
	std::string line;
	std::string shader;
		
	while (getline(stream, line)) 
	{
		if (line.find("#texture") < line.size())
		{
			int IDsCount;
			GLCall(glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &IDsCount));
			int MaxTextures;
			GLCall(glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &MaxTextures ));

			const int PackCount = MaxTextures / IDsCount;
			const int NoneSimpleTextures = 1;
			const int TexturesToUse = MaxTextures - NoneSimpleTextures;

			for (int i = 1; i <= PackCount; i++)
			{
				std::string texName = "u_Textures" + std::to_string(i) + "[" + std::to_string(i != PackCount ? IDsCount : IDsCount - NoneSimpleTextures) + "]";
				shader += "uniform sampler2D " + texName + ";\n";
			}

			shader += R"(	
vec4 GetTextureColor(int tid)
{
	if (tid == -1)
		return vec4(1.0f, 1.0f, 1.0f, 1.0f); 

	switch (tid)
	{												)";

			for (int i = 0; i < TexturesToUse; i++)
			{
				std::string caseStr =  "case " + std::to_string(i) + ": \n";
				caseStr += "return texture(u_Textures" + std::to_string(i / IDsCount + 1) + "[" + std::to_string(i % IDsCount) + "], v_UV); \n";
				
				shader += caseStr; //i / IDsCount + 1; [i % IDsCount] ;
			}
			shader += R"(		
	}
})";



			continue;
		}

		shader += line + "\n";
	}

	return shader;
}


GLuint Shader::CreateShaderByType(std::string_view shaderPath, int shaderType)
{
	std::string SourceString = LoadShaderFile(shaderPath);
	const char* Source = SourceString.c_str();

	GLCall(GLuint shader = glCreateShader(shaderType));
	GLCall(glShaderSource(shader, 1, &Source, NULL));
	GLCall(glCompileShader(shader));

	GLint result;
	GLCall(glGetShaderiv(shader, GL_COMPILE_STATUS, &result));
	if (result == GL_FALSE) {
		GLint length;
		GLCall(glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length));
		std::vector<char> error(length + 1000);
		GLCall(glGetShaderInfoLog(shader, length, &length, &error[0]));

		std::string type;
		switch (shaderType) 
		{
		case GL_VERTEX_SHADER:
			type = "vertex";
			break;
		case GL_FRAGMENT_SHADER:
			type = "fragment";
			break;
		case GL_GEOMETRY_SHADER:
			type = "geometry";
			break;
		}
		LOGE("Failed to compile ", type, " shader!\n    ", &error[0]);
		GLCall(glDeleteShader(shader));
		return 0;
	}

	return shader;
}


GLint Shader::GetUniformLocation(const std::string &name) const
{
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
		return m_UniformLocationCache[name];

	GLint location = glGetUniformLocation(m_ShaderID, name.c_str());
	m_UniformLocationCache[name] = location;
	return location;
}


void Shader::SetUniform1f(const std::string &name, float value)
{
	GLCall(glUniform1f(GetUniformLocation(name), value));
}
void Shader::SetUniform1fv(const std::string &name, float* value, int count)
{
	GLCall(glUniform1fv(GetUniformLocation(name), count, value));
}
void Shader::SetUniform1i(const std::string &name, int value)
{
	GLCall(glUniform1i(GetUniformLocation(name), value));
}
void Shader::SetUniform1iv(const std::string &name, int* value, int count)
{
	GLCall(glUniform1iv(GetUniformLocation(name), count, value));
}
void Shader::SetUniform2f(const std::string &name, const glm::vec2 &vector)
{
	GLCall(glUniform2f(GetUniformLocation(name), vector.x, vector.y));
}
void Shader::SetUniform3f(const std::string &name, const glm::vec3 &vector)
{
	GLCall(glUniform3f(GetUniformLocation(name), vector.x, vector.y, vector.z));
}
void Shader::SetUniform4f(const std::string &name, const glm::vec4 &vector)
{
	GLCall(glUniform4f(GetUniformLocation(name), vector.x, vector.y, vector.z, vector.w));
}
void Shader::SetUniformMat4(const std::string &name, const glm::mat4& matrix)
{
	GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

void Shader::SetUniformMat4(const std::string &name, const Matrix4f& matrix)
{
	GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_TRUE, &matrix.m[0][0]));
}

void Shader::SetMaterial(const GLuint id, const Material* material, const MaterialInfo* material_info)
{
	std::string name = "material[" + std::to_string(id) + "]";
	std::string DiffuseTextureName = name + ".DiffuseTID";
	std::string DiffuseTextureColor = name + ".DiffuseColor";

	SetUniform1i(DiffuseTextureName,  material_info->DiffuseTextureSlot);
	SetUniform4f(DiffuseTextureColor, material->DiffuseColor);
}