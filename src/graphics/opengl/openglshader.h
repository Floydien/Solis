#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <map>
#include <vector>
#include "shader.h"

class UniformStruct {
public:
    UniformStruct(const std::string &name, const std::map<std::string, std::string> &memberMap) : 
        name(name),
        memberMap(memberMap) {};

    inline const std::string &getUniformName() const { return name; };
    inline const std::map<std::string, std::string> &getMembers() { return memberMap; };
private:
    std::string name;
    //Name, type
    std::map<std::string, std::string> memberMap;
};

class OpenGLShader : public Shader {
public:
    OpenGLShader(const std::string&);
    ~OpenGLShader();

    void bind() const;

    void addUniform(const std::string &);
    void updateUniform1i(const std::string &, int) const;
    void updateUniform1f(const std::string &, float) const;
    void updateUniform2i(const std::string &, int, int) const;
    void updateUniform3f(const std::string &, float, float, float) const;
    void updateUniform3fv(const std::string &, const glm::vec3 &) const;
    void updateUniform4f(const std::string &, float, float, float, float) const;
    void updateUniform4fv(const std::string &, const glm::vec4 &) const;
    void updateUniformMatrix4fv(const std::string &, const glm::mat4 &) const;
    // void updateUniformMatrix4fv(const std::string &, const std::vector<glm::mat4> &) const;
    // void updateUniforms(const Transform &, const Material &, const Camera &) const;

private:
    static std::map<std::string, int> resourceMap;

    GLuint program;
    std::string name;
    std::map<std::string, GLint> uniformMap;
    std::map<std::string, std::string> uniformTypeMap;

    std::string loadShader(const std::string &);

    void addVertexShader(const std::string &);
    void addFragmentShader(const std::string &);
    void addProgram(const std::string &, int);

    std::vector<UniformStruct> findUniformStructs(const std::string &);
    void findUniforms(const std::string &);

    void compileShader() const;
};
