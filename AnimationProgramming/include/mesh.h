#pragma once

#include "maths/vector3.hpp"

#include "glad/glad.h"

#include <vector>

class Mesh
{
    std::vector<vec3> m_Vertices;
    std::vector<unsigned int> m_Indices;
    bool m_Loaded = false;
    GLuint m_Vbo, m_Ebo, m_Vao;
    
public:
    Mesh() = default;
    explicit Mesh(const std::string& filename);
    ~Mesh();

    void Load(const std::string& filename);
    void Unload();

    void Forward();

    void Draw() const;

    _NODISCARD bool IsLoaded() const;
    
private:
    void LoadGltfFormat(std::ifstream& file);
    void LoadEngineFormat(std::ifstream& file);
};
