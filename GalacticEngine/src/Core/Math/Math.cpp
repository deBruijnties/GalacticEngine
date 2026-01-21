#include "pch.h"
#include "Math.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

namespace GalacticEngine::Math
{

    static glm::vec2 ToGLM(const Vector2& v) { return { v.x, v.y}; }
    static Vector2 FromGLM(const glm::vec2& v) { return { v.x, v.y}; }
    // Conversion helpers (PRIVATE)
    static glm::vec3 ToGLM(const Vector3& v) { return { v.x, v.y, v.z }; }
    static Vector3 FromGLM(const glm::vec3& v) { return { v.x, v.y, v.z }; }

    static glm::quat ToGLM(const Quaternion& q) { return { q.w, q.x, q.y, q.z }; }
    static Quaternion FromGLM(const glm::quat& q) { return { q.x, q.y, q.z, q.w }; }

    static glm::mat4 ToGLM(const Matrix4& m)
    {
        glm::mat4 r;
        memcpy(&r, m.m, sizeof(float) * 16);
        return r;
    }

    static Matrix4 FromGLM(const glm::mat4& m)
    {
        Matrix4 r;
        memcpy(r.m, &m, sizeof(float) * 16);
        return r;
    }

    static glm::mat3 ToGLM(const Matrix3& m)
    {
        glm::mat3 r;
        memcpy(&r, m.m, sizeof(float) * 9);
        return r;
    }

    static Matrix3 FromGLM(const glm::mat3& m)
    {
        Matrix3 r;
        memcpy(r.m, &m, sizeof(float) * 9);
        return r;
    }



    float Abs(float v) { return glm::abs(v); }
    float Sqrt(float v) { return glm::sqrt(v); }
    float Round(float v) { return glm::round(v); }
    float Floor(float v) { return glm::floor(v); }
    float Ceil(float v) { return glm::ceil(v); }

    float Radians(float v)
    {
        return glm::radians(v);
    }

    float Lerp(float a, float b, float t)
    {
        return glm::mix(a, b, t);
    }

    float Clamp(float v, float a, float b)
    {
        return glm::clamp(v,a,b);
    }

    Vector3 Radians(Vector3 v)
    {
        return FromGLM(glm::radians(ToGLM(v)));
    }

    Vector3 Normalize(const Vector3& v)
    {
        return FromGLM(glm::normalize(ToGLM(v)));
    }

    float Length(const Vector3& v)
    {
        return glm::length(ToGLM(v));
    }

    float Dot(const Vector3& a, const Vector3& b)
    {
        return glm::dot(ToGLM(a), ToGLM(b));
    }

    float Dot(const Vector2& a, const Vector2& b)
    {
        return glm::dot(ToGLM(a), ToGLM(b));
    }

    Vector3 Cross(const Vector3& a, const Vector3& b)
    {
        return FromGLM(glm::cross(ToGLM(a), ToGLM(b)));
    }

    Vector3 Lerp(Vector3 a, Vector3 b, float t)
    {
        return FromGLM(glm::mix(ToGLM(a), ToGLM(b), t));
    }

    Vector3 Mod(Vector3 a, Vector3 b)
    {
        return FromGLM(glm::mod(ToGLM(a), ToGLM(b)));
    }

    Matrix4 Identity()
    {
        Matrix4 m{};
        auto g = glm::mat4(1.0f);
        memcpy(m.m, &g, sizeof(float) * 16);
        return m;
    }

    Matrix4 Inverse(const Matrix4& m)
    {
        return FromGLM(glm::inverse(ToGLM(m)));
    }

    Matrix4 Transpose(const Matrix4& m)
    {
        return FromGLM(glm::transpose(ToGLM(m)));
    }

    Matrix4 perspective(float fov, float aspect, float nearPlane, float farPlane)
    {
        return FromGLM(glm::perspective(fov, aspect, nearPlane, farPlane));
    }

    Matrix3 ToMatrix3(const Matrix4& m)
    {
        return FromGLM(glm::mat3(ToGLM(m)));
    }

    Quaternion QuaternionFromMatrix3(const Matrix3& m)
    {
        return FromGLM(glm::quat_cast(ToGLM(m)));
    }

    void DecomposeMatrix(const Matrix4& m, Vector3& pos, Quaternion& rot, Vector3& scale)
    {
		glm::mat4 mat = ToGLM(m);
        pos = FromGLM(glm::vec3(mat[3]));

        scale.x = glm::length(glm::vec3(mat[0]));
        scale.y = glm::length(glm::vec3(mat[1]));
        scale.z = glm::length(glm::vec3(mat[2]));

        glm::mat3 rotMat;
        rotMat[0] = glm::vec3(mat[0]) / scale.x;
        rotMat[1] = glm::vec3(mat[1]) / scale.y;
        rotMat[2] = glm::vec3(mat[2]) / scale.z;

        rot = FromGLM(glm::quat_cast(rotMat));
    }

    Matrix4 Translate(const Matrix4& m, const Vector3& v)
    {
        auto r = glm::translate(ToGLM(m), ToGLM(v));
        return FromGLM(r);
    }

    Matrix4 Scale(const Matrix4& m, const Vector3& v)
    {
        auto r = glm::scale(ToGLM(m), ToGLM(v));
        return FromGLM(r);
    }

    Matrix4 Rotate(const Matrix4& m, const Quaternion& q)
    {
        auto r = ToGLM(m) * glm::mat4_cast(ToGLM(q));
        return FromGLM(r);
    }

    Matrix4 Rotate(float angleRadians, const Vector3& axis)
    {
        glm::mat4 r = glm::rotate(glm::mat4(1.0f), angleRadians, ToGLM(axis));
        return FromGLM(r);
    }

}
