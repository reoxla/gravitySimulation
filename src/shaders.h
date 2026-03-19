#pragma once
#include "glad.h"
#include "glm/glm.hpp"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
  

class Shader
{
public:
    unsigned int ID;
  
    Shader(const char* vertexPath, const char* fragmentPath);
    void use();
    void unUse();
    void setVec3(const std::string &name, glm::vec3 value) const;  
    void setInt(const std::string &name, int value) const;   
    void setFloat(const std::string &name, float value) const;
	void setMat4(const std::string& name, glm::mat4 value) const;
};
