#pragma once

#include <string>
#include <filesystem>

#include <glad/glad.h>

#include "maths/vector2.hpp"
#include "maths/vector3.hpp"
#include "maths/vector4.hpp"
#include "maths/matrix2x2.hpp"
#include "maths/matrix3x3.hpp"
#include "maths/matrix4x4.hpp"

class Shader
{
public:
    enum class ShaderType : unsigned char
    {
        Vertex,
        Fragment
    };

    Shader() = default;
    explicit Shader(const std::filesystem::path& folder, const std::string& vertexName = "vertex", const std::string& fragmentName = "fragment");
    Shader(const std::filesystem::path& vertexFile, const std::filesystem::path& fragmentFile);
    ~Shader();

    bool LoadVertex(const std::filesystem::path& filepath);
    bool LoadFragment(const std::filesystem::path& filepath);
    bool Link();

    void Use() const;

    __forceinline void SetUniform(const std::string& name, const bool value) const { SetUniform(name, static_cast<int>(value)); }
    __forceinline void SetUniform(const std::string& name, const int value) const { glUniform1i(static_cast<GLint>(GetUniform(name)), value); }
    __forceinline void SetUniform(const std::string& name, const float value) const { glUniform1f(static_cast<GLint>(GetUniform(name)), value); }
    __forceinline void SetUniform(const std::string& name, const Vector2 value) const { glUniform2fv(static_cast<GLint>(GetUniform(name)), 1, &value.x); }
    __forceinline void SetUniform(const std::string& name, const Vector3& value) const { glUniform3fv(static_cast<GLint>(GetUniform(name)), 1, &value.x); }
    __forceinline void SetUniform(const std::string& name, const Vector4& value) const { glUniform4fv(static_cast<GLint>(GetUniform(name)), 1, &value.x); }
    __forceinline void SetUniform(const std::string& name, const Matrix2x2& value) const { glUniformMatrix2fv(static_cast<GLint>(GetUniform(name)), 1, true, &value[0].x); }
    __forceinline void SetUniform(const std::string& name, const Matrix3x3& value) const { glUniformMatrix3fv(static_cast<GLint>(GetUniform(name)), 1, true, &value[0].x); }
    __forceinline void SetUniform(const std::string& name, const Matrix4x4& value) const { glUniformMatrix4fv(static_cast<GLint>(GetUniform(name)), 1, true, &value[0].x); }

private:
    unsigned int m_Vertex = 0, m_Fragment = 0, m_Program = 0;
    std::string m_Source;
    bool m_Linked = false;

    bool LoadShader(const std::filesystem::path& filepath, unsigned int& shader, const ShaderType type);

    // Load shader source
    bool Load(const std::filesystem::path& filepath);
    void Unload();
    static void Unload(unsigned int& shader);

    _NODISCARD unsigned int GetUniform(const std::string& name) const;
};
