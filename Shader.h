#pragma once
#include <GL/glew.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "glm/glm.hpp"

class Shader {
private:
    struct ShaderSource {
        std::string vertex;
        std::string fragment;
    };

    unsigned int m_ID;

private:
    ShaderSource loadShaders(const std::string& path) {
        std::ifstream file(path);
        std::string line;
        std::stringstream ss[2];
        int mode = -1;

        while (std::getline(file, line)) {
            if (line.find("#shader") != std::string::npos) {
                if (line.find("vertex") != std::string::npos)
                    mode = 0;
                else
                    mode = 1;
            } else {
                if (mode >= 0) ss[mode] << line << "\n";
            }
        }

        return { ss[0].str(), ss[1].str() };
    }

    static void checkShaderErrors(unsigned int shader, const std::string& type) {
        int success;
        char infoLog[1024];
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

        if (!success) {
            glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
            std::cout << type << " SHADER ERROR:\n" << infoLog << "\n";
        }
    }

    static void checkProgramErrors(unsigned int program) {
        int success;
        char infoLog[1024];
        glGetProgramiv(program, GL_LINK_STATUS, &success);

        if (!success) {
            glGetProgramInfoLog(program, 1024, nullptr, infoLog);
            std::cout << "PROGRAM LINK ERROR:\n" << infoLog << "\n";
        }
    }

    unsigned int compileShader(unsigned int type, const std::string& src) {
        unsigned int id = glCreateShader(type);
        const char* csrc = src.c_str();
        glShaderSource(id, 1, &csrc, nullptr);
        glCompileShader(id);

        if (type == GL_VERTEX_SHADER)
            checkShaderErrors(id, "VERTEX");
        else
            checkShaderErrors(id, "FRAGMENT");

        return id;
    }

    unsigned int createProgram(const std::string& v, const std::string& f) {
        unsigned int program = glCreateProgram();
        unsigned int vs = compileShader(GL_VERTEX_SHADER, v);
        unsigned int fs = compileShader(GL_FRAGMENT_SHADER, f);

        glAttachShader(program, vs);
        glAttachShader(program, fs);

        glLinkProgram(program);
        checkProgramErrors(program);

        glValidateProgram(program);

        glDeleteShader(vs);
        glDeleteShader(fs);

        return program;
    }

public:

    Shader(const std::string& filePath) {
        ShaderSource src = loadShaders(filePath);
        m_ID = createProgram(src.vertex, src.fragment);
    }

    ~Shader() {
        glDeleteProgram(m_ID);
    }

    void bind() const {
        glUseProgram(m_ID);
    }

    void unbind() const {
        glUseProgram(0);
    }

    unsigned int getID() const { return m_ID; }


    void setUniformMat4f(const std::string& name, const glm::mat4& mat) const {
        int loc = glGetUniformLocation(m_ID, name.c_str());
        glUniformMatrix4fv(loc, 1, GL_FALSE, &mat[0][0]);
    }

    void setUniformVec4f(const std::string& name, const glm::vec4& v) const {
        int loc = glGetUniformLocation(m_ID, name.c_str());
        glUniform4f(loc, v.x, v.y, v.z, v.w);
    }

    void setUniformVec3f(const std::string& name, const glm::vec3& v) const {
        int loc = glGetUniformLocation(m_ID, name.c_str());
        glUniform3f(loc, v.x, v.y, v.z);
    }

    void setUniform1i(const std::string& name, int value) const {
        int loc = glGetUniformLocation(m_ID, name.c_str());
        glUniform1i(loc, value);
    }

    void setUniform1f(const std::string& name, float value) const {
        int loc = glGetUniformLocation(m_ID, name.c_str());
        glUniform1f(loc, value);
    }
};
