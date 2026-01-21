#pragma once
#include <cmath>

namespace GalacticEngine
{

    struct Vector2
    {
        float x, y;

        Vector2() : x(0), y(0) {}
        Vector2(float x, float y) : x(x), y(y) {}

        // Operators
        Vector2 operator+(const Vector2& r) const { return { x + r.x, y + r.y }; }
        Vector2 operator-(const Vector2& r) const { return { x - r.x, y - r.y }; }
        Vector2 operator*(float s) const { return { x * s, y * s }; }
        Vector2 operator/(float s) const { return { x / s, y / s }; }

        Vector2& operator+=(const Vector2& r) { x += r.x; y += r.y; return *this; }
        Vector2& operator-=(const Vector2& r) { x -= r.x; y -= r.y; return *this; }
        Vector2& operator*=(float s) { x *= s; y *= s; return *this; }
        Vector2& operator/=(float s) { x /= s; y /= s; return *this; }
    };

    struct Vector3
    {
        union
        {
            struct { float x, y, z; };
            struct { float r, g, b; };
            float data[3];
        };

        Vector3() : x(0), y(0), z(0) {}
        Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

        // Operators
        Vector3 operator+(const Vector3& r) const { return { x + r.x, y + r.y, z + r.z }; }
        Vector3 operator-(const Vector3& r) const { return { x - r.x, y - r.y, z - r.z }; }
        Vector3 operator*(float s) const { return { x * s, y * s, z * s }; }
        Vector3 operator/(float s) const { return { x / s, y / s, z / s }; }

        Vector3& operator+=(const Vector3& r) { x += r.x; y += r.y; z += r.z; return *this; }
        Vector3& operator-=(const Vector3& r) { x -= r.x; y -= r.y; z -= r.z; return *this; }
        Vector3& operator*=(float s) { x *= s; y *= s; z *= s; return *this; }
        Vector3& operator/=(float s) { x /= s; y /= s; z /= s; return *this; }
    };

    struct Vector4
    {
        union
        {
            struct { float x, y, z, w; };
            struct { float r, g, b, a; };
            float data[4];
        };

        Vector4() : x(0), y(0), z(0), w(0) {}
        Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

        // Operators
        Vector4 operator+(const Vector4& r) const { return { x + r.x, y + r.y, z + r.z, w + r.w }; }
        Vector4 operator-(const Vector4& r) const { return { x - r.x, y - r.y, z - r.z, w - r.w }; }
        Vector4 operator*(float s) const { return { x * s, y * s, z * s, w * s }; }
        Vector4 operator/(float s) const { return { x / s, y / s, z / s, w / s }; }

        Vector4& operator+=(const Vector4& r) { x += r.x; y += r.y; z += r.z; w += r.w; return *this; }
        Vector4& operator-=(const Vector4& r) { x -= r.x; y -= r.y; z -= r.z; w -= r.w; return *this; }
        Vector4& operator*=(float s) { x *= s; y *= s; z *= s; w *= s; return *this; }
        Vector4& operator/=(float s) { x /= s; y /= s; z /= s; w /= s; return *this; }
    };

    struct Vector2Int
    {
        int x, y;

        Vector2Int() : x(0), y(0) {}
        Vector2Int(int x, int y) : x(x), y(y) {}

        // Operators
        Vector2Int operator+(const Vector2Int& r) const { return { x + r.x, y + r.y }; }
        Vector2Int operator-(const Vector2Int& r) const { return { x - r.x, y - r.y }; }
        Vector2Int operator*(int s) const { return { x * s, y * s }; }
        Vector2Int operator/(int s) const { return { x / s, y / s }; }

        Vector2Int& operator+=(const Vector2Int& r) { x += r.x; y += r.y; return *this; }
        Vector2Int& operator-=(const Vector2Int& r) { x -= r.x; y -= r.y; return *this; }
        Vector2Int& operator*=(int s) { x *= s; y *= s; return *this; }
        Vector2Int& operator/=(int s) { x /= s; y /= s; return *this; }
    };

    struct Vector3Int
    {
        union
        {
            struct { int x, y, z; };
            struct { int r, g, b; };
            int data[3];
        };

        Vector3Int() : x(0), y(0), z(0) {}
        Vector3Int(int x, int y, int z) : x(x), y(y), z(z) {}

        // Operators
        Vector3Int operator+(const Vector3Int& r) const { return { x + r.x, y + r.y, z + r.z }; }
        Vector3Int operator-(const Vector3Int& r) const { return { x - r.x, y - r.y, z - r.z }; }
        Vector3Int operator*(int s) const { return { x * s, y * s, z * s }; }
        Vector3Int operator/(int s) const { return { x / s, y / s, z / s }; }

        Vector3Int& operator+=(const Vector3Int& r) { x += r.x; y += r.y; z += r.z; return *this; }
        Vector3Int& operator-=(const Vector3Int& r) { x -= r.x; y -= r.y; z -= r.z; return *this; }
        Vector3Int& operator*=(int s) { x *= s; y *= s; z *= s; return *this; }
        Vector3Int& operator/=(int s) { x /= s; y /= s; z /= s; return *this; }
    };

    struct Vector4Int
    {
        union
        {
            struct { int x, y, z, w; };
            struct { int r, g, b, a; };
            int data[4];
        };

        Vector4Int() : x(0), y(0), z(0), w(0) {}
        Vector4Int(int x, int y, int z, int w) : x(x), y(y), z(z), w(w) {}

        // Operators
        Vector4Int operator+(const Vector4Int& r) const { return { x + r.x, y + r.y, z + r.z, w + r.w }; }
        Vector4Int operator-(const Vector4Int& r) const { return { x - r.x, y - r.y, z - r.z, w - r.w }; }
        Vector4Int operator*(int s) const { return { x * s, y * s, z * s, w * s }; }
        Vector4Int operator/(int s) const { return { x / s, y / s, z / s, w / s }; }

        Vector4Int& operator+=(const Vector4Int& r) { x += r.x; y += r.y; z += r.z; w += r.w; return *this; }
        Vector4Int& operator-=(const Vector4Int& r) { x -= r.x; y -= r.y; z -= r.z; w -= r.w; return *this; }
        Vector4Int& operator*=(int s) { x *= s; y *= s; z *= s; w *= s; return *this; }
        Vector4Int& operator/=(int s) { x /= s; y /= s; z /= s; w /= s; return *this; }
    };

    struct Quaternion
    {
        float x, y, z, w;

        Quaternion() : x(0), y(0), z(0), w(1) {}
        Quaternion(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

        static Quaternion Identity()
        {
            return Quaternion(0, 0, 0, 1);
        }

        Quaternion Normalized() const
        {
            float len = sqrtf(x * x + y * y + z * z + w * w);
            if (len <= 0.000001f)
                return Identity();

            float inv = 1.0f / len;
            return Quaternion(x * inv, y * inv, z * inv, w * inv);
        }

        static Quaternion FromEuler(const Vector3& euler)
        {
            float cx = cosf(euler.x * 0.5f);
            float sx = sinf(euler.x * 0.5f);
            float cy = cosf(euler.y * 0.5f);
            float sy = sinf(euler.y * 0.5f);
            float cz = cosf(euler.z * 0.5f);
            float sz = sinf(euler.z * 0.5f);

            Quaternion q;
            q.w = cx * cy * cz + sx * sy * sz;
            q.x = sx * cy * cz - cx * sy * sz;
            q.y = cx * sy * cz + sx * cy * sz;
            q.z = cx * cy * sz - sx * sy * cz;

            return q.Normalized();
        }
    };

    struct Matrix3
    {
        // Column-major 3x3
        float m[9];

        float& operator()(int row, int col) { return m[col * 3 + row]; }
        const float& operator()(int row, int col) const { return m[col * 3 + row]; }
    };

    struct Matrix4
    {
        float m[16];

        Matrix4()
        {
            for (int i = 0; i < 16; ++i) m[i] = 0.0f;
            m[0] = m[5] = m[10] = m[15] = 1.0f;
        }

        static Matrix4 Identity()
        {
            return Matrix4();
        }

        static Matrix4 Zero()
        {
            Matrix4 r;
            for (int i = 0; i < 16; ++i) r.m[i] = 0.0f;
            return r;
        }

        inline float& operator()(int row, int col)
        {
            return m[col * 4 + row];
        }

        inline const float& operator()(int row, int col) const
        {
            return m[col * 4 + row];
        }

        // Matrix × Matrix
        Matrix4 operator*(const Matrix4& r) const
        {
            Matrix4 o = Zero();

            for (int c = 0; c < 4; ++c)
            {
                for (int r2 = 0; r2 < 4; ++r2)
                {
                    for (int k = 0; k < 4; ++k)
                    {
                        o(r2, c) += (*this)(r2, k) * r(k, c);
                    }
                }
            }

            return o;
        }

        // Matrix × Vector4
        Vector4 operator*(const Vector4& v) const
        {
            Vector4 r;
            r.x = v.x * m[0] + v.y * m[4] + v.z * m[8] + v.w * m[12];
            r.y = v.x * m[1] + v.y * m[5] + v.z * m[9] + v.w * m[13];
            r.z = v.x * m[2] + v.y * m[6] + v.z * m[10] + v.w * m[14];
            r.w = v.x * m[3] + v.y * m[7] + v.z * m[11] + v.w * m[15];
            return r;
        }

        static Matrix4 Translate(const Vector3& v)
        {
            Matrix4 r;
            r.m[12] = v.x;
            r.m[13] = v.y;
            r.m[14] = v.z;
            return r;
        }

        static Matrix4 Scale(const Vector3& v)
        {
            Matrix4 r;
            r.m[0] = v.x;
            r.m[5] = v.y;
            r.m[10] = v.z;
            return r;
        }

        static Matrix4 Rotate(const Quaternion& q)
        {
            Quaternion nq = q.Normalized();

            float x = nq.x;
            float y = nq.y;
            float z = nq.z;
            float w = nq.w;

            float xx = x * x;
            float yy = y * y;
            float zz = z * z;
            float xy = x * y;
            float xz = x * z;
            float yz = y * z;
            float wx = w * x;
            float wy = w * y;
            float wz = w * z;

            Matrix4 r;

            r.m[0] = 1.0f - 2.0f * (yy + zz);
            r.m[1] = 2.0f * (xy + wz);
            r.m[2] = 2.0f * (xz - wy);
            r.m[3] = 0.0f;

            r.m[4] = 2.0f * (xy - wz);
            r.m[5] = 1.0f - 2.0f * (xx + zz);
            r.m[6] = 2.0f * (yz + wx);
            r.m[7] = 0.0f;

            r.m[8] = 2.0f * (xz + wy);
            r.m[9] = 2.0f * (yz - wx);
            r.m[10] = 1.0f - 2.0f * (xx + yy);
            r.m[11] = 0.0f;

            r.m[12] = 0.0f;
            r.m[13] = 0.0f;
            r.m[14] = 0.0f;
            r.m[15] = 1.0f;

            return r;
        }
    };

}
