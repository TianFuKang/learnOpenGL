#pragma once

#include "Matrix.h"

// SIMD
#if defined __arm__
#include "SIMD_NEON.h"
#else
#include "SIMD_SSE.h"
#endif

namespace m3d {
    namespace math {
        struct Vector3;
        //		struct Matrix4x4;

        struct alignas(16) Quaternion {
        public:
            float x, y, z, w;

        public:
            inline Quaternion(){};
            inline Quaternion(float fX, float fY, float fZ, float fW);
            inline Quaternion(const Quaternion& other);
            inline explicit Quaternion(const Matrix4x4& mat);
            inline Quaternion(const Vector3& axis, const float angleInRad);

            inline Quaternion Inverse() const;

            inline Quaternion operator=(const Quaternion& other);
            inline Quaternion operator+(const Quaternion& other) const;
            inline Quaternion operator+=(const Quaternion& other);
            inline Quaternion operator-(const Quaternion& other) const;
            inline Quaternion operator-=(const Quaternion& other);
            inline Quaternion operator*(const Quaternion& other) const;
            inline Quaternion operator*=(const Quaternion& other);
            inline float operator|(const Quaternion& other) const; // dot product

            inline Vector3 operator*(const Vector3& v) const;
            inline Matrix4x4 operator*(const Matrix4x4& mat) const;

            inline Quaternion operator*(const float scale) const;
            inline Quaternion operator*=(const float scale);
            inline Quaternion operator/(const float scale) const;
            inline Quaternion operator/=(const float scale);

            inline void ToMatrix(Matrix4x4& mat);
        };

        inline Quaternion::Quaternion(float fX, float fY, float fZ, float fW)
                : x(fX)
                , y(fY)
                , z(fZ)
                , w(fW)
        {
        }

        inline Quaternion::Quaternion(const Quaternion& other)
        {
            x = other.x;
            y = other.y;
            z = other.z;
            w = other.w;
        }

        inline Quaternion::Quaternion(const Vector3& axis, const float angleInRad)
        {
            const float halfAngle = 0.5f * angleInRad;

            float sinOfHalfAngle = std::sin(halfAngle);
            x = sinOfHalfAngle * axis.x;
            y = sinOfHalfAngle * axis.y;
            z = sinOfHalfAngle * axis.z;
            w = std::cos(halfAngle);
        }

        inline Quaternion::Quaternion(const Matrix4x4& mat4x4)
        {
            float s;

            const float trace = mat4x4.m[0][0] + mat4x4.m[1][1] + mat4x4.m[2][2];

            if (trace > 0.0f) {
                float invsqrt = InvSqrt(trace + 1.f);
                w = 0.5f * (1.0f / invsqrt);
                s = 0.5f * invsqrt;

                x = (mat4x4.m[1][2] - mat4x4.m[2][1]) * s;
                y = (mat4x4.m[2][0] - mat4x4.m[0][2]) * s;
                z = (mat4x4.m[0][1] - mat4x4.m[1][0]) * s;
            } else {
                int i = 0;

                if (mat4x4.m[1][1] > mat4x4.m[0][0])
                    i = 1;

                if (mat4x4.m[2][2] > mat4x4.m[i][i])
                    i = 2;

                static const int nxt[3] = { 1, 2, 0 };
                const int j = nxt[i];
                const int k = nxt[j];

                s = mat4x4.m[i][i] - mat4x4.m[j][j] - mat4x4.m[k][k] + 1.0f;

                float invsqrt = InvSqrt(s);

                float qt[4];
                qt[i] = 0.5f * (1.f / invsqrt);

                s = 0.5f * invsqrt;

                qt[3] = (mat4x4.m[j][k] - mat4x4.m[k][j]) * s;
                qt[j] = (mat4x4.m[i][j] + mat4x4.m[j][i]) * s;
                qt[k] = (mat4x4.m[i][k] + mat4x4.m[k][i]) * s;

                x = qt[0];
                y = qt[1];
                z = qt[2];
                w = qt[3];
            }
        }

        inline Quaternion Quaternion::Inverse() const
        {
            return Quaternion(-x, -y, -z, w);
        }

        /* Operators */
        inline Quaternion Quaternion::operator=(const Quaternion& other)
        {
            x = other.x;
            y = other.y;
            z = other.z;
            w = other.w;
            return *this;
        }

        inline Quaternion Quaternion::operator+(const Quaternion& other) const
        {
            return Quaternion(x + other.x, y + other.y, z + other.z, w + other.w);
        }

        inline Quaternion Quaternion::operator+=(const Quaternion& other)
        {
            x += other.x;
            y += other.y;
            z += other.z;
            w += other.w;
            return *this;
        }

        inline Quaternion Quaternion::operator-(const Quaternion& other) const
        {
            return Quaternion(x - other.x, y - other.y, z - other.z, w - other.w);
        }

        inline Quaternion Quaternion::operator-=(const Quaternion& other)
        {
            x -= other.x;
            y -= other.y;
            z -= other.z;
            w -= other.w;
            return *this;
        }

        inline Quaternion Quaternion::operator*(const Quaternion& other) const
        {
            Quaternion result;
            QuaternionMultiply(&result, this, &other);
            return result;
        }

        inline Quaternion Quaternion::operator*=(const Quaternion& other)
        {
            VectorSIMD A = VectorLoad4f(this);
            VectorSIMD B = VectorLoad4f(&other);
            VectorSIMD Result;
            QuaternionMultiply(&Result, &A, &B);
            VectorStore4f(Result, this);

            return *this;
        }

        float Quaternion::operator|(const Quaternion& other) const
        {
            return x * other.x + y * other.y + z * other.z + w * other.w;
        }

        inline Vector3 Quaternion::operator*(const Vector3& v0) const
        {
            const Vector3 v1(x, y, z);
            const Vector3 normal = Vector3::CrossProduct(v1, v0) * 2.0f;
            const Vector3 result = v0 + (normal * w) + Vector3::CrossProduct(v1, normal);
            return result;
        }

        inline Matrix4x4 Quaternion::operator*(const Matrix4x4& mat) const
        {
            Matrix4x4 result;
            Quaternion quat0, quat1;
            Quaternion inverse = Inverse();
            for (int I = 0; I < 4; ++I) {
                Quaternion quat_mat(mat.m[I][0], mat.m[I][1], mat.m[I][2], mat.m[I][3]);
                QuaternionMultiply(&quat0, this, &quat_mat);
                QuaternionMultiply(&quat1, &quat0, &inverse);
                result.m[I][0] = quat1.x;
                result.m[I][1] = quat1.y;
                result.m[I][2] = quat1.z;
                result.m[I][3] = quat1.w;
            }

            return result;
        }

        /* Scale */
        inline Quaternion Quaternion::operator*(const float scale) const
        {
            return Quaternion(x * scale, y * scale, z * scale, w * scale);
        }

        inline Quaternion Quaternion::operator*=(const float scale)
        {
            x *= scale;
            y *= scale;
            z *= scale;
            w *= scale;
            return *this;
        }

        inline Quaternion Quaternion::operator/(const float scale) const
        {
            return Quaternion(x / scale, y / scale, z / scale, w / scale);
        }

        inline Quaternion Quaternion::operator/=(const float scale)
        {
            x /= scale;
            y /= scale;
            z /= scale;
            w /= scale;
            return *this;
        }

        inline void Quaternion::ToMatrix(Matrix4x4& mat)
        {
            const float x2 = x + x;
            const float y2 = y + y;
            const float z2 = z + z;
            const float xx = x * x2;
            const float xy = x * y2;
            const float xz = x * z2;
            const float yy = y * y2;
            const float yz = y * z2;
            const float zz = z * z2;
            const float wx = w * x2;
            const float wy = w * y2;
            const float wz = w * z2;

            mat.m[0][0] = 1.0f - (yy + zz);
            mat.m[1][0] = xy - wz;
            mat.m[2][0] = xz + wy;
            mat.m[3][0] = 0.0f;
            mat.m[0][1] = xy + wz;
            mat.m[1][1] = 1.0f - (xx + zz);
            mat.m[2][1] = yz - wx;
            mat.m[3][1] = 0.0f;
            mat.m[0][2] = xz - wy;
            mat.m[1][2] = yz + wx;
            mat.m[2][2] = 1.0f - (xx + yy);
            mat.m[3][2] = 0.0f;
            mat.m[0][3] = 0.0f;
            mat.m[1][3] = 0.0f;
            mat.m[2][3] = 0.0f;
            mat.m[3][3] = 1.0f;
        }
    }
}