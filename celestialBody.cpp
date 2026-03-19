#include "celestialBody.h"

celestialBody::celestialBody(unsigned long long mass, glm::vec3 birthVelocity, glm::vec3 initialPos, glm::vec3 radius, int id)
{
    this->mass = mass;
    velocity = birthVelocity;
    position = initialPos;
    this->radius = radius;
	this->id = id;
}

void celestialBody::applyVelocity(Shader& shader) {
    position += velocity;
	shader.setMat4("transform", glm::translate(glm::mat4(1.0f), position));
}

void celestialBody::applyGravity(std::vector<celestialBody> other) {
    for each(celestialBody body in other)
    {
        if (body.id == this->id)
        {
            continue;
        }
        else
        {
            glm::vec3 direction = body.position - position;
            float distance = glm::length(direction);
            float gravitationalConstant = 0.001f;
            float forceMagnitude = gravitationalConstant * (mass * body.mass) / (distance * distance);
            glm::vec3 force = glm::normalize(direction) * forceMagnitude;
            velocity += force;
        }
    }
}

void generateCircleTemplate(int vCount, float radius) {
    float angle = 360.0f / vCount;

    int triangleCount = vCount - 2;

    std::vector<glm::vec3> temp;
    // positions
    for (int i = 0; i < vCount; i++)
    {
        float currentAngle = angle * i;
        float x = radius * cos(glm::radians(currentAngle));
        float y = radius * sin(glm::radians(currentAngle));
        float z = 0.0f;

        temp.push_back(glm::vec3(x, y, z));
    }
    // triangles
    for (int i = 0; i < triangleCount; i++)
    {
        circleTemplate.push_back(temp[0]);
        circleTemplate.push_back(glm::vec3(1, 0, 0));
        circleTemplate.push_back(temp[i + 1]);
        circleTemplate.push_back(glm::vec3(1, 0, 0));
        circleTemplate.push_back(temp[i + 2]);
        circleTemplate.push_back(glm::vec3(1, 0, 0));
    }
}