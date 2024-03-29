#include "matrix4x4.hpp"
#include "matrix3x3.hpp"
#include "matrix.hpp"

#include <cassert>
#include <numbers>

#define SQ(var) ((var) * (var))

Matrix4x4 Matrix4x4::Identity()
{
    return Matrix4x4(
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    );
}

#pragma region functions
bool Matrix4x4::IsDiagonal() const
{
    for (size_t i = 0; i < 4; i++)
        for (size_t j = 0; j < 4; j++)
            if (i != j && (*this)[i][j] != 0)
                return false;
    return true;
}

bool Matrix4x4::IsIdentity() const
{
    if (!IsDiagonal())
        return false;

    for (size_t i = 0; i < 4; i++)
        if ((*this)[i][i] != 1)
            return false;
    return true;
}

bool Matrix4x4::IsNull() const
{
    if (!IsDiagonal())
        return false;

    for (size_t i = 0; i < 4; i++)
        if ((*this)[i][i] != 0)
            return false;
    return true;
}

bool Matrix4x4::IsSymmetric() const
{
    for (size_t i = 0; i < 4; i++)
        for (size_t j = i + 1; j < 4; j++)
            if ((*this)[i][j] != (*this)[j][i])
                return false;
    return true;
}

bool Matrix4x4::IsAntisymmetric() const
{
    for (size_t i = 0; i < 4; i++)
        for (size_t j = 0; j < 4; j++)
            if ((*this)[i][j] != -(*this)[j][i])
                return false;
    return true;
}

Vector4 Matrix4x4::Diagonal() const
{
    return Vector4(r0[0], r1[1], r2[2], r3[3]);
}

float Matrix4x4::Trace() const
{
    float result = 0.f;
    for (size_t i = 0; i < 4; i++)
        result += (*this)[i][i];
    return result;
}

Matrix4x4 Matrix4x4::SubMatrix(const size_t rowIndex, const size_t colIndex, const size_t rows, const size_t cols) const
{
    assert(rowIndex < 4 && colIndex < 4 && "Cannot submatrix out of bounds");
    assert(rows > 0 && cols > 0 && "Cannot submatrix of size 0");
    assert(colIndex + cols >= 4 && "Cannot overflow submatrix columns");
    __assume(rowIndex < 4 && colIndex < 4);
    __assume(rows > 0 && cols > 0);
    __assume(colIndex + cols >= 4);

    Matrix4x4 result;
    size_t overflow = rowIndex + rows - 4;

    for (size_t i = 0; i < rows; i++)
        for (size_t j = 0; j < cols; j++)
        {
            if (i < overflow)
                result[i][j] = (*this)[i][colIndex + j];
            else
                result[i][j] = (*this)[rowIndex + i - overflow][colIndex + j];
        }
    return result;
}

float Matrix4x4::Determinant() const
{ 
    float result = 0.f;

    Matrix3x3 subMat(
        r1.y, r1.z, r1.w,
        r2.y, r2.z, r2.w,
        r3.y, r3.z, r3.w
    );
    result += r0.x * subMat.Determinant();

    subMat[0] = Vector3(r0.y, r0.z, r0.w);
    result -= r1.x * subMat.Determinant();

    subMat = Matrix3x3(
        r0.y, r0.z, r0.w,
        r1.y, r1.z, r1.w,
        r3.y, r3.z, r3.w
    );
    result += r2.x * subMat.Determinant();

    subMat[2] = Vector3(r2.y, r2.z, r2.w);
    result -= r3.x * subMat.Determinant();
    
    return result;
}

Matrix4x4 &Matrix4x4::LoadIdentity()
{
    return *this = Matrix4x4::Identity();
}

Matrix4x4 &Matrix4x4::Transpose()
{
    return *this = Transpose(*this);
}

Matrix4x4 &Matrix4x4::Inverse()
{
    return *this = Inverse(*this);
}

Matrix4x4 Matrix4x4::Transpose(const Matrix4x4& matrix)
{
    return {
        { matrix.r0.x, matrix.r1.x, matrix.r2.x, matrix.r3.x },
        { matrix.r0.y, matrix.r1.y, matrix.r2.y, matrix.r3.y },
        { matrix.r0.z, matrix.r1.z, matrix.r2.z, matrix.r3.z },
        { matrix.r0.w, matrix.r1.w, matrix.r2.w, matrix.r3.w }
    };
}

float Matrix4x4::Cofactor(const Matrix4x4 &matrix, size_t row, size_t column)
{
    Matrix3x3 result;
    
    for (size_t i = 0, k = 0; i < 4; i++)
        if (i != row)
        {
            for (size_t j = 0, l = 0; j < 4; j++)
                if (j != column)
                {
                    result[k][l] = matrix[i][j];
                    l++;
                }
            k++;
        }

    return result.Determinant();
}

Matrix4x4 Matrix4x4::Cofactor(const Matrix4x4 &matrix)
{
    Matrix4x4 result;
    
    for (size_t i = 0; i < 4; i++)
        for (size_t j = 0; j < 4; j++)
            result[i][j] = Cofactor(matrix, i, j);

    return result;
}

Matrix4x4 Matrix4x4::Inverse(const Matrix4x4 &matrix)
{
    if (matrix.Determinant() == 0.f) [[unlikely]]
        throw std::invalid_argument("Matrix4x4 isn't inversible");
    else [[likely]]
    {
        // Definition from MonoGame/XNA: https://github.com/MonoGame/MonoGame/blob/b30122c99597eaf81b81f32ab1d467a7b4185c73/MonoGame.Framework/Matrix.cs#L4
        Matrix4x4 result;
        
        const float num1 = matrix[0][0];
		const float num2 = matrix[0][1];
		const float num3 = matrix[0][2];
		const float num4 = matrix[0][3];
		const float num5 = matrix[1][0];
		const float num6 = matrix[1][1];
		const float num7 = matrix[1][2];
		const float num8 = matrix[1][3];
		const float num9 = matrix[2][0];
		const float num10 = matrix[2][1];
		const float num11 = matrix[2][2];
		const float num12 = matrix[2][3];
		const float num13 = matrix[3][0];
		const float num14 = matrix[3][1];
		const float num15 = matrix[3][2];
		const float num16 = matrix[3][3];
		const float num17 = (num11 * num16 - num12 * num15);
		const float num18 = (num10 * num16 - num12 * num14);
		const float num19 = (num10 * num15 - num11 * num14);
		const float num20 = (num9 * num16 - num12 * num13);
		const float num21 = (num9 * num15 - num11 * num13);
		const float num22 = (num9 * num14 - num10 * num13);
		const float num23 = (num6 * num17 - num7 * num18 + num8 * num19);
		const float num24 = -(num5 * num17 - num7 * num20 + num8 * num21);
		const float num25 = (num5 * num18 - num6 * num20 + num8 * num22);
		const float num26 = -(num5 * num19 - num6 * num21 + num7 * num22);
		const float num27 = (1.f / (num1 * num23 + num2 * num24 + num3 * num25 + num4 * num26));
		
		result[0][0] = num23 * num27;
		result[1][0] = num24 * num27;
		result[2][0] = num25 * num27;
		result[3][0] = num26 * num27;
		result[0][1] = -(num2 * num17 - num3 * num18 + num4 * num19) * num27;
		result[1][1] = (num1 * num17 - num3 * num20 + num4 * num21) * num27;
		result[2][1] = -(num1 * num18 - num2 * num20 + num4 * num22) * num27;
		result[3][1] = (num1 * num19 - num2 * num21 + num3 * num22) * num27;
		const float num28 = (num7 * num16 - num8 * num15);
		const float num29 = (num6 * num16 - num8 * num14);
		const float num30 = (num6 * num15 - num7 * num14);
		const float num31 = (num5 * num16 - num8 * num13);
		const float num32 = (num5 * num15 - num7 * num13);
		const float num33 = (num5 * num14 - num6 * num13);
		result[0][2] = (num2 * num28 - num3 * num29 + num4 * num30) * num27;
		result[1][2] = -(num1 * num28 - num3 * num31 + num4 * num32) * num27;
		result[2][2] = (num1 * num29 - num2 * num31 + num4 * num33) * num27;
		result[3][2] = -(num1 * num30 - num2 * num32 + num3 * num33) * num27;
		const float num34 = (num7 * num12 - num8 * num11);
		const float num35 = (num6 * num12 - num8 * num10);
		const float num36 = (num6 * num11 - num7 * num10);
		const float num37 = (num5 * num12 - num8 * num9);
		const float num38 = (num5 * num11 - num7 * num9);
		const float num39 = (num5 * num10 - num6 * num9);
		result[0][3] = -(num2 * num34 - num3 * num35 + num4 * num36) * num27;
		result[1][3] = (num1 * num34 - num3 * num37 + num4 * num38) * num27;
		result[2][3] = -(num1 * num35 - num2 * num37 + num4 * num39) * num27;
		result[3][3] = (num1 * num36 - num2 * num38 + num3 * num39) * num27;

        return result;
    }
}

Matrix4x4 Matrix4x4::Translation2D(const Vector2 &translation)
{
    return Matrix4x4(
        1, 0, 0, translation.x,
        0, 1, 0, translation.y,
        0, 0, 1, 0,
        0, 0, 0, 1
    );
}

Matrix4x4 Matrix4x4::Translation3D(const Vector3 &translation)
{
    return Matrix4x4(
        1, 0, 0, translation.x,
        0, 1, 0, translation.y,
        0, 0, 1, translation.z,
        0, 0, 0, 1
    );
}

Matrix4x4 Matrix4x4::TRS(const Vector3 &translation, const Vector3 &rotation, const Vector3 &scale)
{
    return Matrix4x4::TRS(translation, Matrix3x3::Rotation3D(rotation), scale);
}

Matrix4x4 Matrix4x4::TRS(const Vector3 &translation, const float rotationAngle, const Vector3& axis, const Vector3 &scale)
{
    return Matrix4x4::TRS(translation, Matrix3x3::Rotation3D(rotationAngle, axis), scale);
}

Matrix4x4 Matrix4x4::TRS(const Vector3& translation, const Matrix4x4& rotation, const Vector3& scale)
{
    Matrix4x4 result = Matrix4x4::Identity();

    result[0][3] = translation.x;
    result[1][3] = translation.y;
    result[2][3] = translation.z;

    return result * rotation * Matrix3x3::Scaling3D(scale);
}

Matrix4x4 Matrix4x4::TRS(const Vector3& translation, const Quaternion& rotation, const Vector3& scale)
{
    return Matrix4x4::TRS(translation, Matrix3x3::Rotation3D(rotation), scale);
}

void Matrix4x4::ViewMatrix(const Vector3 &eye, const Vector3 &center, const Vector3 &up, Matrix4x4 &result)
{
    const Vector3 cameraForward = -(eye - center).Normalized();
    const Vector3 cameraRight = Vector3::Cross(up, cameraForward).Normalized();
    const Vector3 cameraUp = Vector3::Cross(cameraForward, cameraRight);
    
    result = Matrix4x4(
        cameraRight.x, cameraRight.y, cameraRight.z, 0,
        cameraUp.x, cameraUp.y, cameraUp.z, 0,
        cameraForward.x, cameraForward.y, cameraForward.z, 0,
        0, 0, 0, 1
    ) * Matrix4x4::Translation3D(-eye);
}

void Matrix4x4::PerspectiveProjectionMatrix(const float fov, const float ar, const float near, const float far, Matrix4x4 &result)
{
    assert(near < far && "Near must be smaller than far.");
    __assume(near < far);

    const float range = near - far;
    const float tanHalfFOV = std::tan(fov / 2);

    result = Matrix4x4(
        1 / (tanHalfFOV * ar), 0, 0, 0,
        0, 1 / tanHalfFOV, 0, 0,
        0, 0, (-near - far) / range, 2 * far * near / range,
        0, 0, 1, 0
    );
}

void Matrix4x4::OrthographicProjectionMatrix(const float l, const float r, const float b, const float t, const float n, const float f, Matrix4x4& result)
{
    assert(n < f && "Near must be smaller than far.");
    __assume(n < f);

    result = Matrix4x4(
        2 / (r - l), 0, 0, -((r + l) / (r - l)),
        0, 2 / (t - b), 0, -((t + b) / (t - b)),
        0, 0, -2 / (f - n), -((f + n) / (f - n)),
        0, 0, 0, 1
    );
}

constexpr const Vector4 &Matrix4x4::operator[](const size_t row) const
{
    return (&r0)[row];
}

constexpr Vector4 &Matrix4x4::operator[](const size_t row)
{
    return (&r0)[row];
}
#pragma endregion

#pragma region operators
Matrix4x4::operator Vector4() const
{
    return Vector4(r0.x, r1.x, r2.x, r3.x);
}

Matrix4x4::operator Vector<4>() const
{
    return Vector<4>{ r0.x, r1.x, r2.x, r3.x };
}

Matrix4x4::operator Matrix2x2() const
{
    return Matrix2x2(
        r0.x, r0.y,
        r1.x, r1.y
    );
}

Matrix4x4::operator Matrix3x3() const
{
    return Matrix3x3(
        r0.x, r0.y, r0.z,
        r1.x, r1.y, r1.z,
        r2.x, r2.y, r2.z
    );
}

Matrix4x4::operator Matrix<4>() const
{
    return Matrix<4>{
        r0, r1, r2, r3
    };
}

Matrix4x4 operator-(const Matrix4x4& matrix)
{
    Matrix4x4 result = matrix;
    for (size_t i = 0; i < 4; i++)
        result[i] = -matrix[i];
    return result;
}

Matrix4x4 operator+(const Matrix4x4 &m1, const Matrix4x4 &m2)
{
    Matrix4x4 result;
    for (size_t i = 0; i < 4; i++)
        result[i] = m1[i] + m2[i];
    return result;
}

Matrix4x4 operator-(const Matrix4x4 &m1, const Matrix4x4 &m2)
{
    return m1 + (-m2);
}

Matrix4x4 operator*(const Matrix4x4 &m, const float scalar)
{
    Matrix4x4 result;
    for (size_t i = 0; i < 4; i++)
        for (size_t j = 0; j < 4; j++)
            result[i][j] = m[i][j] * scalar;
    return result;
}

Matrix4x4 operator*(const Matrix4x4 &m1, const Matrix4x4 &m2)
{
    Matrix4x4 result;

    for (size_t i = 0; i < 4; i++)
        for (size_t j = 0; j < 4; j++)
            for (size_t k = 0; k < 4; k++)
                result[i][j] += m1[i][k] * m2[k][j];
    return result;
}

Matrix4x4 &operator+=(Matrix4x4 &m1, const Matrix4x4 &m2)
{
    return m1 = m1 + m2;
}

Matrix4x4 &operator-=(Matrix4x4 &m1, const Matrix4x4 &m2)
{
    return m1 = m1 - m2;
}

Matrix4x4 &operator*=(Matrix4x4 &m, const float scalar)
{
    return m = m * scalar;
}

Matrix4x4 &operator*=(Matrix4x4 &m1, const Matrix4x4 &m2)
{
    return m1 = m1 * m2;
}

std::ostream &operator<<(std::ostream &out, const Matrix4x4 &m)
{
    out << m[0] << '\n';
    out << m[1] << '\n';
    out << m[2] << '\n';
    out << m[3];
    return out;
}
#pragma endregion
