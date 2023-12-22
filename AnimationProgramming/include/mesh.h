#pragma once

#include "maths/vector3.hpp"

#include "glad/glad.h"

#include <vector>

#include "gltf/gltf.h"

class Mesh
{
    std::vector<vec3> m_Vertices;
    std::vector<unsigned short> m_Indices;
    bool m_Loaded = false;
    GLuint m_Vbo, m_Ebo, m_Vao;
    GLuint m_TextureId;

    Gltf* m_Gltf = nullptr;
    
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
    void LoadGltfFormat(std::ifstream& file, const std::filesystem::path& filepath);
    void LoadEngineFormat(std::ifstream& file);
};
