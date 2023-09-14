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
	std::string filePath;
	unsigned int rendererId;
	std::unordered_map<std::string, int> uniformLocationCache;

public: 
	Shader( const std::string& filepath );
	~Shader();

	void bind();
	void unBind();
};