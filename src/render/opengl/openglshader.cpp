/* TODO:
 * -in findUniforms: find the structs in structs
 * -in findUniforms and findUniformStructs: skip comments
 * -add an updateAllUniforms method
 */
#include "openglshader.h"
#include <stdio.h>
#include <fstream>
#include <algorithm>

OpenGLShader::OpenGLShader(const std::string & shaderName) {
    name = shaderName;

    program = glCreateProgram();

    std::string vsText = loadShader(("res/shaders/" + shaderName + ".vs").c_str());
    std::string fsText = loadShader(("res/shaders/" + shaderName + ".fs").c_str());

    addVertexShader(vsText);
    addFragmentShader(fsText);
    compileShader();

    findUniforms(vsText);
    findUniforms(fsText);
}

OpenGLShader::~OpenGLShader() {
    glDeleteProgram(program);
}

void OpenGLShader::addUniform(const std::string &uniformName) {
    auto it = uniformMap.find(uniformName);
    if(it != uniformMap.end()) {
        return;
    }

    GLint uniform = glGetUniformLocation(program, uniformName.c_str());

    /* If the uniform was found */
    if(uniform != GL_INVALID_VALUE)
        uniformMap.insert(std::pair<std::string, GLint>(uniformName, uniform));
    else 
        fprintf(stderr, "WARNING: the uniform %s in %s was not found.\n",
                uniformName.c_str(), name.c_str());
}

void OpenGLShader::updateUniform1i(const std::string &uniformName, int x) const {
    glUniform1i(uniformMap.find(uniformName)->second, x);
}

void OpenGLShader::updateUniform1f(const std::string &uniformName, float x) const {
    glUniform1f(uniformMap.find(uniformName)->second, x);
}

void OpenGLShader::updateUniform3f(const std::string &uniformName, float x, float y, float z) const {
    glUniform3f(uniformMap.find(uniformName)->second, x, y, z);
}

void OpenGLShader::updateUniform3fv(const std::string &uniformName, const glm::vec3 &vec) const {
    glUniform3fv(uniformMap.find(uniformName)->second, 1, glm::value_ptr(vec));
}

void OpenGLShader::updateUniform4f(const std::string &uniformName, float x, float y, float z, float w) const {
    glUniform4f(uniformMap.find(uniformName)->second, x, y, z, w);
}

void OpenGLShader::updateUniform4fv(const std::string &uniformName, const glm::vec4 &vec) const {
    glUniform4fv(uniformMap.find(uniformName)->second, 1, glm::value_ptr(vec));
}

void OpenGLShader::updateUniformMatrix4fv(const std::string &uniformName, const glm::mat4 &matrix) const {
    glUniformMatrix4fv(uniformMap.find(uniformName)->second, 1, GL_FALSE, glm::value_ptr(matrix));
}

/*void OpenGLShader::updateUniformMatrix4fv(const std::string &uniformName, const std::vector<glm::mat4> &matrixlist) const {
    if(matrixlist.size() == 0) {
        glUniformMatrix4fv(uniformMap.find(uniformName)->second, 0, GL_FALSE, 0);
        return;
    }
    float array[matrixlist.size() * 16] = {0.0f};
    const float *source;
    for (size_t i = 0; i < matrixlist.size(); i++) {
        source = (const float *) glm::value_ptr(matrixlist[i]);
        for (size_t k = 0; k < 16; k++) {
            array[k + i * 16] = source[k];
        }
    }
    glUniformMatrix4fv(uniformMap.find(uniformName)->second, matrixlist.size(), GL_FALSE, array);
}*/

void OpenGLShader::addVertexShader(const std::string &text) {
    addProgram(text, GL_VERTEX_SHADER);
}

void OpenGLShader::addFragmentShader(const std::string &text) {
    addProgram(text, GL_FRAGMENT_SHADER);
}

void OpenGLShader::addProgram(const std::string &text, int type) {
    /* Create and load the  shader */
    auto shader = glCreateShader(type);

    const GLchar* ptr[1];
    GLint length[1];

    ptr[0] = text.c_str();
    length[0] = text.length();

    glShaderSource(shader, 1, ptr, length);
    glCompileShader(shader);

    /* Check if compilation was successful */
    GLint success;
    GLchar infoLog[512];

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        fprintf(stderr, "ERROR: compilation of shader %s failed\n %s", name.c_str(), infoLog);
    }

    glAttachShader(program, shader);
    glDeleteShader(shader);
}

void OpenGLShader::compileShader() const {
    glLinkProgram(program);

    GLint success;
    GLchar infoLog[512];
    /* check if the linking was successful */
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(program, 512, NULL, infoLog);
        fprintf(stderr, "ERROR: linking of a shader program failed\n %s", infoLog);
    }
}

std::string OpenGLShader::loadShader(const std::string &fileName) {
    std::ifstream file(fileName.c_str());
    std::string out;
    std::string line;

    if(file.is_open()) {
        while(file.good()) {
            getline(file, line);
            out.append(line + "\n");
        }
    } else {
        fprintf(stderr, "ERROR: failed to load the shader %s\n", fileName.c_str());
    }
    file.close();

    return out;
}

void OpenGLShader::bind() const {
    glUseProgram(program);
}

std::vector<UniformStruct> OpenGLShader::findUniformStructs(const std::string &shaderText) {
    static const std::string STRUCT_KEY = "struct";

    std::vector<UniformStruct> out;
    std::string structName;
    std::map<std::string, std::string> memberMap;

    size_t pos = 0;
    size_t scopeStart = 0;
    size_t scopeEnd = 0;
    while(pos < shaderText.length()) {
        memberMap.clear();
        //First find a struct
        pos = shaderText.find(STRUCT_KEY, scopeEnd);
        if(pos == std::string::npos)
            return out;
        pos += STRUCT_KEY.length() + 1;

        structName = shaderText.substr(pos, shaderText.find(' ', pos + STRUCT_KEY.length()) - pos).c_str();
        scopeStart = shaderText.find('{', pos);
        scopeEnd = shaderText.find('}', pos);
        pos = scopeStart + 1;

        //Find all the members of the struct
        std::string memberType;
        std::string memberName;
        while(pos < scopeEnd) {
            while(isspace(shaderText.at(pos)))
                pos++;
            if(pos >= scopeEnd)
                break;
            memberType = shaderText.substr(pos, shaderText.find(' ', pos) - pos);
            pos += memberType.length() + 1;
            memberName = shaderText.substr(pos, shaderText.find(';', pos ) - pos);
            pos += memberName.length() + 1;
            memberMap.insert(std::pair<std::string, std::string>(memberName, memberType));
        }

        out.push_back(UniformStruct(structName, memberMap));
   }

    return out;
}

void OpenGLShader::findUniforms(const std::string &shaderText) {
    static const std::string UNIFORM_KEY = "uniform";

    auto structs = findUniformStructs(shaderText);

    std::string uniformName;
    std::string uniformType;

    size_t pos = 0;
    size_t semic = 0;
    while(pos < shaderText.length()) {
        pos = shaderText.find(UNIFORM_KEY, semic);
        semic = shaderText.find(';', pos) + 1;

        //reached end of the file
        if(pos == std::string::npos)
            break;

        pos += UNIFORM_KEY.length() + 1;
        uniformType = shaderText.substr(pos, shaderText.find(' ', pos) - pos);
        pos += uniformType.length() + 1;
        uniformName = shaderText.substr(pos, shaderText.find(';', pos ) - pos);

        //Look if the type is a struct
        auto it = std::find_if(structs.begin(), structs.end(), [uniformType](const UniformStruct &uniformStruct) 
            { return uniformType == uniformStruct.getUniformName(); });

        //the type is no struct
        if(it == structs.end()) {
            uniformTypeMap.insert(std::pair<std::string, std::string>(uniformName, uniformType));
        }
        else {
            std::map<std::string, std::string> uniforms;
            auto members = it->getMembers();
            for (auto i = members.begin();
                    i != members.end(); ++i) {
                uniformTypeMap.insert(std::pair<std::string, std::string>(
                    uniformName + "." + i->first, uniformType + "." + i->second));
            }
        }
    }
    for (auto i = uniformTypeMap.begin();
            i != uniformTypeMap.end(); ++i) {
        addUniform(i->first);
    }
}