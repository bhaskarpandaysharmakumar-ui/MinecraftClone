#pragma once

#include <string>

class Shader {
public:
    unsigned int ID;

    Shader(const char* vertexShaderPath, const char* fragmentShaderPath);
    void Bind() const;
    void Unbind() const;
    void SetBool(const std::string &name, bool value) const;
    void SetInt(const std::string &name, int value) const;
    void SetFloat(const std::string &name, float value) const;
};
