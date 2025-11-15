#pragma once
#include <vector>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <GL/glew.h>
#include "Shader.h"

#include "Texture.h"


struct Vertex {
    glm::vec3 position;
    glm::vec2 texCoord;
};

class Polygon {
private:
    std::vector<Vertex> m_vertices;
    std::vector<unsigned int> m_indices;

    unsigned int m_VAO = 0, m_VBO = 0, m_EBO = 0;
    Shader* m_shader = nullptr;
    Texture* m_texture = nullptr;

    glm::vec3 m_position = glm::vec3(0.0f);
    glm::vec3 m_rotation = glm::vec3(0.0f);
    glm::vec3 m_scale = glm::vec3(1.0f);
    glm::mat4 m_model = glm::mat4(1.0f);

    glm::vec4 m_color = glm::vec4(1.0f);
    bool m_useTexture = false;

    GLenum m_drawMode = GL_TRIANGLES;

public:
    Polygon(const std::vector<Vertex>& verts,
            const std::vector<unsigned int>& inds,
            Shader* shader,
            GLenum drawMode = GL_TRIANGLES)
            : m_vertices(verts), m_indices(inds), m_shader(shader), m_drawMode(drawMode)
    {
        setupBuffers();
        updateModelMatrix();
    }

    Polygon(const std::vector<Vertex>& verts, Shader* shader, GLenum drawMode = GL_TRIANGLES)
            : m_vertices(verts), m_shader(shader), m_drawMode(drawMode)
    {
        setupBuffers();
        updateModelMatrix();
    }

    ~Polygon() {
        if (m_EBO) glDeleteBuffers(1, &m_EBO);
        if (m_VBO) glDeleteBuffers(1, &m_VBO);
        if (m_VAO) glDeleteVertexArrays(1, &m_VAO);
    }

    void setColor(const glm::vec4& color) {
        m_color = color;
    }

    void setTexture(Texture* tex) {
        m_texture = tex;
        m_useTexture = (tex != nullptr);
    }

    void setPosition(const glm::vec3& pos) {
        m_position = pos;
        updateModelMatrix();
    }

    void setRotation(const glm::vec3& rotDegrees) {
        m_rotation = rotDegrees;
        updateModelMatrix();
    }

    void setScale(const glm::vec3& scale) {
        m_scale = scale;
        updateModelMatrix();
    }

    glm::mat4 getModelMatrix() const { return m_model; }

    void updateModelMatrix() {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, m_position);
        // rotation order: Z, Y, X (common)
        model = glm::rotate(model, glm::radians(m_rotation.z), glm::vec3(0,0,1));
        model = glm::rotate(model, glm::radians(m_rotation.y), glm::vec3(0,1,0));
        model = glm::rotate(model, glm::radians(m_rotation.x), glm::vec3(1,0,0));
        model = glm::scale(model, m_scale);
        m_model = model;
    }

    void draw(const glm::mat4& view, const glm::mat4& proj) {
        if (!m_shader) return;

        m_shader->bind();

        m_shader->setUniformMat4f("u_Model", m_model);
        m_shader->setUniformMat4f("u_View", view);
        m_shader->setUniformMat4f("u_Proj", proj);

        m_shader->setUniformVec4f("u_Color", m_color);
        m_shader->setUniform1i("u_UseTexture", m_useTexture ? 1 : 0);

        if (m_useTexture && m_texture) {
            m_texture->bind(0);
            m_shader->setUniform1i("u_Diffuse", 0); // texture unit 0
        }

        glBindVertexArray(m_VAO);
        if (!m_indices.empty()) {
            glDrawElements(m_drawMode, (GLsizei)m_indices.size(), GL_UNSIGNED_INT, 0);
        } else {
            glDrawArrays(m_drawMode, 0, (GLsizei)m_vertices.size());
        }

        glBindVertexArray(0);
        if (m_useTexture && m_texture) m_texture->unbind();
        m_shader->unbind();
    }

private:
    void setupBuffers() {
        glGenVertexArrays(1, &m_VAO);
        glGenBuffers(1, &m_VBO);
        if (!m_indices.empty()) glGenBuffers(1, &m_EBO);

        glBindVertexArray(m_VAO);

        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), m_vertices.data(), GL_STATIC_DRAW);

        if (!m_indices.empty()) {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), m_indices.data(), GL_STATIC_DRAW);
        }

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));

        glBindVertexArray(0);
    }
};
