#pragma once
#include <vector>
#include "src/glm/glm.hpp"
#include "src/glm/gtc/matrix_transform.hpp"
#include "src/shaders.h"

class celestialBody
{
private:
    unsigned long long mass;
public:
	int id;
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 radius;
    celestialBody(unsigned long long mass, glm::vec3 birthVelocity, glm::vec3 initialPos, glm::vec3 radius, int id);
    void applyVelocity(Shader& shader);
	void applyGravity(std::vector<celestialBody> other);
};
inline std::vector<glm::vec3>circleTemplate;
void generateCircleTemplate(int vCount, float radius);
