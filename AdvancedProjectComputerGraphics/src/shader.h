#pragma once

#include <string>
#include <unordered_map>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

struct ShaderProgramSource {
	std::string vertexSource;
	std::string fragmentSource;
	std::string geometrySource;
};

class Shader {
private:
	unsigned int rendererId;
	std::unordered_map<std::string, int> uniformLocationCache;
	ShaderProgramSource shaders;

public:
	Shader( const std::string& vertexFilePath, const std::string& fragmentFilePath );
	Shader( const std::string& vertexFilePath, const std::string& fragmentFilePath, const std::string& geometryFilePath );
	~Shader();

	void bind() const;
	void unBind() const;

	// set uniforms
	void setUniform4f( const std::string& name, glm::vec4 vector );

private:
	std::string parseShader( const std::string& filepath );
	unsigned int compileShader( unsigned int type, const std::string& source );
	unsigned int createShader( const std::string& vertexShader, const std::string& fragmentShader );
	unsigned int createShader( const std::string& vertexShader, const std::string& fragmentShader, const std::string& geometryShader );
	unsigned int getUniformLocation( const std::string& name );
};