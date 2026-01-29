#pragma once
#include <Core/Math/Types.h>

namespace GalacticEngine::Math
{
    constexpr float PI = 3.14159265358979323846f;
    constexpr float HALF_PI = PI * 0.5f;
    constexpr float TWO_PI = PI * 2.0f;
    constexpr float DEG2RAD = PI / 180.0f;
    constexpr float RAD2DEG = 180.0f / PI;

    float Abs(float v);
    float Sqrt(float v);
    float Round(float v);
    float Floor(float v);
    float Ceil(float v);
    float Radians(float v);
    float Lerp(float a, float b, float t);
    float Clamp(float v, float a, float b);

    //Vector2 operations
    float Dot(const Vector2& a, const Vector2& b);
    Vector2 Lerp(const Vector2& a, const Vector2& b, float t);


	//Vector3 operations
    Vector3 Radians(Vector3 v);
    Vector3 Normalize(const Vector3& v);
    float Length(const Vector3& v);
    float Dot(const Vector3& a, const Vector3& b);
    Vector3 Cross(const Vector3& a, const Vector3& b);
    Vector3 Lerp(Vector3 a, Vector3 b, float t);
    Vector3 Mod(Vector3 a, Vector3 b);

	//Matrix4 operations
    Matrix4 Identity();
    Matrix4 Translate(const Matrix4& m, const Vector3& v);
    Matrix4 Scale(const Matrix4& m, const Vector3& v);
    Matrix4 Rotate(const Matrix4& m, const Quaternion& q);
    Matrix4 Rotate(float angleRadians, const Vector3& axis);

    Matrix4 Inverse(const Matrix4& m);
    Matrix4 Transpose(const Matrix4& m);
    Matrix4 perspective(float Fov, float Aspect, float nearPlane, float farPlane);
    Matrix4 ortho(float left, float right, float bottom, float top, float zNear, float zFar);
    Matrix4 ortho(float left, float right, float bottom, float top);


    //Matrix3 operations
    Matrix3 ToMatrix3(const Matrix4& m);
    Quaternion QuaternionFromMatrix3(const Matrix3& m);

    void DecomposeMatrix(const Matrix4& m, Vector3& pos, Quaternion& rot, Vector3& scale);

    

}
