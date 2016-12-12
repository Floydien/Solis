#pragma once
#include <string>
#include <vector>

class Shader {
public:
    Shader() {};
    virtual ~Shader() {};

    virtual void bind() const {};

    virtual void addUniform(const std::string &) {};
    virtual void updateUniformMatrix4fv(const std::string &, const glm::mat4 &) const {};

private:
};