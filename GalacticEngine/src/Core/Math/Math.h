#pragma once
#include <Core/Math/Types.h>

namespace GalacticEngine::Math
{
    float Abs(float v);
    float Sqrt(float v);
    float Round(float v);
    float Floor(float v);
    float Ceil(float v);
    float Radians(float v);

	//Vector3 operations
    Vector3 Normalize(const Vector3& v);
    float Length(const Vector3& v);
    float Dot(const Vector3& a, const Vector3& b);
    Vector3 Cross(const Vector3& a, const Vector3& b);

	//Matrix4 operations
    Matrix4 Identity();
    Matrix4 Translate(const Matrix4& m, const Vector3& v);
    Matrix4 Scale(const Matrix4& m, const Vector3& v);
    Matrix4 Rotate(const Matrix4& m, const Quaternion& q);

    Matrix4 Inverse(const Matrix4& m);
    Matrix4 Transpose(const Matrix4& m);
    Matrix4 perspective(float Fov, float Aspect, float nearPlane, float farPlane);
 

    //Matrix3 operations
    Matrix3 ToMatrix3(const Matrix4& m);
    Quaternion QuaternionFromMatrix3(const Matrix3& m);

    void DecomposeMatrix(const Matrix4& m, Vector3& pos, Quaternion& rot, Vector3& scale);

    

}
