#ifndef GRAPHICS_CUBE_H
#define GRAPHICS_CUBE_H

#include "../framework/shader.h"
#include "../framework/color.h"

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using std::vector, glm::vec3, glm::mat4;


class Cube {
public:
    Cube(Shader& shader, vec3 pos, vec3 size, struct color color);
    ~Cube();
    void draw(const mat4& model, const mat4& view, const mat4& projection) const;
    void setUniforms(const mat4& model, const mat4& view, const mat4& projection) const;

private:
    Shader shader;
    vec3 pos;
    vec3 size;
    struct color color;

    // Vectors
    vector<float> vertices;
    vector<unsigned int> indices;

    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;

    void initVectors();
    void initVAO();
    void initVBO();
    void initEBO();
};


#endif //GRAPHICS_CUBE_H
