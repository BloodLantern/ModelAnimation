#include "shader.h"

#include <fstream>
#include <iostream>

Shader::Shader(const std::filesystem::path& folder, const std::string& vertexName, const std::string& fragmentName)
{
    const std::filesystem::path newFolder = relative(folder);

    if (!LoadVertex(newFolder.string() + '\\' + vertexName))
        return;
    if (!LoadFragment(newFolder.string() + '\\' + fragmentName))
        return;

    m_Linked = Link();
}

Shader::Shader(const std::filesystem::path& vertexFile, const std::filesystem::path& fragmentFile)
{
    LoadVertex(vertexFile);
    LoadFragment(fragmentFile);
}

Shader::~Shader()
{
    glDeleteProgram(m_Program);
}

bool Shader::LoadVertex(const std::filesystem::path& filepath)
{
    if (m_Vertex != 0)
        Unload(m_Vertex);

    return LoadShader(filepath, m_Vertex, ShaderType::Vertex);
}

bool Shader::LoadFragment(const std::filesystem::path& filepath)
{
    if (m_Fragment != 0)
        Unload(m_Fragment);

    return LoadShader(filepath, m_Fragment, ShaderType::Fragment);
}

bool Shader::LoadShader(const std::filesystem::path &filepath, unsigned int& shader, const ShaderType type)
{
    if (!Load(filepath))
        return false;

    GLenum shaderType = 0;
    switch (type)
    {
        case ShaderType::Vertex:
            shaderType = GL_VERTEX_SHADER;
            break;
            
        case ShaderType::Fragment:
            shaderType = GL_FRAGMENT_SHADER;
            break;
        
        default:
            std::unreachable();
    }

    shader = glCreateShader(shaderType);
    const char* const source = m_Source.c_str();
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cout << "Shader compilation error: " << filepath.string() << "\n" << infoLog << '\n';
        return false;
    }

    return true;
}

bool Shader::Load(const std::filesystem::path &filepath)
{
    std::ifstream file(filepath);

    if (!file.is_open() || !file.good())
    {
        std::cout << "Couldn't open file: " << filepath << '\n';

        return false;
    }

    m_Source = std::string(std::istreambuf_iterator(file), std::istreambuf_iterator<char>());
    
    return true;
}

void Shader::Unload()
{
    m_Source = "";
    
    Unload(m_Vertex);
    Unload(m_Fragment);
}

void Shader::Unload(unsigned int& shader)
{
    glDeleteShader(shader);
    shader = 0;
}

bool Shader::Link()
{
    m_Program = glCreateProgram();
    glAttachShader(m_Program, m_Vertex);
    glAttachShader(m_Program, m_Fragment);
    glLinkProgram(m_Program);

    int success;
    glGetProgramiv(m_Program, GL_LINK_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetProgramInfoLog(m_Program, 512, nullptr, infoLog);
        std::cout << "Shader linking error: " << "\n" << infoLog << '\n';
        return false;
    }

    Unload();

    return true;
}

void Shader::Use() const
{
    glUseProgram(m_Program);
}

unsigned int Shader::GetUniform(const std::string& name) const
{
    return glGetUniformLocation(m_Program, name.c_str());
}
