#pragma once

#include <cstdio>
#include <cstring>

//#include "MathUtils.hpp"
// SIMD
#if defined(__arm__) || defined(__aarch64__)
#include "SIMD_NEON.hpp"
#else
#include "SIMD_SSE.hpp"
#endif

#include "MathUtils.hpp"

#define USE_SIMD 1

namespace mj2 {

    struct Vector2 {
    public:
        float x;
        float y;

        inline Vector2(){};

        inline Vector2(float fX, float fY)
                : x(fX)
                , y(fY)
        {
        }

        /// Arithmetic operators
        inline Vector2 operator+(const Vector2& other) const
        {
            return Vector2{ x + other.x, y + other.y };
        }

        inline Vector2 operator-(const Vector2& other) const
        {
            return Vector2{ x - other.x, y - other.y };
        }

        inline Vector2 operator*(const Vector2& other) const
        {
            return Vector2{ x * other.x, y * other.y };
        }

        inline Vector2 operator/(const Vector2& other) const
        {
            return Vector2{ x / other.x, y / other.y };
        }

        inline Vector2 operator+(const float bias) const
        {
            return Vector2{ x + bias, y + bias };
        }

        inline Vector2 operator-(const float bias) const
        {
            return Vector2{ x - bias, y - bias };
        }

        inline Vector2 operator*(const float scale) const
        {
            return Vector2{ x * scale, y * scale };
        }

        inline Vector2 operator/(const float scale) const
        {
            return Vector2{ x / scale, y / scale };
        }

        /// Compound assignment operators
        inline Vector2& operator+=(const Vector2& other)
        {
            this->x += other.x;
            this->y += other.y;
            return *this;
        }

        inline Vector2& operator-=(const Vector2& other)
        {
            this->x -= other.x;
            this->y -= other.y;
            return *this;
        }

        inline Vector2& operator+=(const float bias)
        {
            this->x += bias;
            this->y += bias;
            return *this;
        }

        inline Vector2& operator-=(const float bias)
        {
            this->x -= bias;
            this->y -= bias;
            return *this;
        }

        inline Vector2& operator*=(const float scale)
        {
            this->x *= scale;
            this->y *= scale;
            return *this;
        }

        inline Vector2& operator/=(const float scale)
        {
            this->x /= scale;
            this->y /= scale;
            return *this;
        }

        /// dot multiply with other
        inline float operator|(const Vector2& other) const
        {
            return x * other.x + y * other.y;
        }

        inline static float DotProduct(const Vector2& left, const Vector2& right)
        {
            return left | right;
        }
    };

    //-------------------------------------------------------------
    // Vector3
    //-------------------------------------------------------------
    struct Vector3 {
    public:
        float x;
        float y;
        float z;

        inline Vector3(){};
        inline Vector3(float fX, float fY, float fZ);
        inline Vector3(Vector2& v, float fZ);

        inline Vector3 operator-() const
        {
            return Vector3{ -x, -y, -z };
        };

        inline Vector3 operator+(const Vector3& other) const;
        inline Vector3 operator-(const Vector3& other) const;
        inline Vector3 operator*(const Vector3& other) const;
        inline Vector3 operator/(const Vector3& other) const;

        inline Vector3 operator+(const float bias) const;
        inline Vector3 operator-(const float bias) const;
        inline Vector3 operator*(const float Scale) const;
        inline Vector3 operator/(const float Scale) const;
        inline float operator|(const Vector3& other) const;
        inline Vector3 operator^(const Vector3& other) const;

        inline Vector3& operator+=(const Vector3& other);
        inline Vector3& operator-=(const Vector3& other);
        inline Vector3& operator*=(const Vector3& other);
        inline Vector3& operator/=(const Vector3& other);

        inline Vector3& operator+=(const float bias);
        inline Vector3& operator-=(const float bias);
        inline Vector3& operator*=(const float Scale);
        inline Vector3& operator/=(const float Scale);

        inline static Vector3 CrossProduct(const Vector3& Left, const Vector3& Right);
        inline static float DotProduct(const Vector3& Left, const Vector3& Right);

        inline void Normalize();

        void Print();
        void ToString(char* const str, size_t size);
    };

    inline Vector3::Vector3(float fX, float fY, float fZ)
            : x(fX)
            , y(fY)
            , z(fZ)
    {
    }

    inline Vector3::Vector3(Vector2& v, float fZ)
            : x(v.x)
            , y(v.y)
            , z(fZ)
    {
    }

    inline Vector3 Vector3::operator+(const Vector3& other) const
    {
        return Vector3(x + other.x, y + other.y, z + other.z);
    }

    inline Vector3 Vector3::operator-(const Vector3& other) const
    {
        return Vector3(x - other.x, y - other.y, z - other.z);
    }

    inline Vector3 Vector3::operator*(const Vector3& other) const
    {
        return Vector3(x * other.x, y * other.y, z * other.z);
    }

    inline Vector3 Vector3::operator/(const Vector3& other) const
    {
        return Vector3(x / other.x, y / other.y, z / other.z);
    }

    inline Vector3 Vector3::operator+(const float bias) const
    {
        return Vector3(x + bias, y + bias, z + bias);
    }

    inline Vector3 Vector3::operator-(const float bias) const
    {
        return Vector3(x - bias, y - bias, z - bias);
    }

    inline Vector3 Vector3::operator*(const float Scale) const
    {
        return Vector3(x * Scale, y * Scale, z * Scale);
    }

    inline Vector3 Vector3::operator/(const float Scale) const
    {
        return Vector3(x / Scale, y / Scale, z / Scale);
    }

    inline float Vector3::operator|(const Vector3& other) const
    {
        return x * other.x + y * other.y + z * other.z;
    }

    inline Vector3 Vector3::operator^(const Vector3& other) const
    {
        return Vector3(y * other.z - z * other.y,
                       z * other.x - x * other.z,
                       x * other.y - y * other.x);
    }

    // Compound assignment operators
    inline Vector3& Vector3::operator+=(const Vector3& other)
    {
        this->x += other.x;
        this->y += other.y;
        this->z += other.z;
        return *this;
    }
    inline Vector3& Vector3::operator-=(const Vector3& other)
    {
        this->x -= other.x;
        this->y -= other.y;
        this->z -= other.z;
        return *this;
    }

    inline Vector3& Vector3::operator*=(const Vector3& other)
    {
        this->x *= other.x;
        this->y *= other.y;
        this->z *= other.z;
        return *this;
    }

    inline Vector3& Vector3::operator/=(const Vector3& other)
    {
        this->x /= other.x;
        this->y /= other.y;
        this->z /= other.z;
        return *this;
    }

    inline Vector3& Vector3::operator+=(const float bias)
    {
        this->x += bias;
        this->y += bias;
        this->z += bias;
        return *this;
    }
    inline Vector3& Vector3::operator-=(const float bias)
    {
        this->x -= bias;
        this->y -= bias;
        this->z -= bias;
        return *this;
    }
    inline Vector3& Vector3::operator*=(const float Scale)
    {
        this->x *= Scale;
        this->y *= Scale;
        this->z *= Scale;
        return *this;
    }
    inline Vector3& Vector3::operator/=(const float Scale)
    {
        this->x /= Scale;
        this->y /= Scale;
        this->z /= Scale;
        return *this;
    }

    inline float Vector3::DotProduct(const Vector3& Left, const Vector3& Right)
    {
        return Left | Right;
    }

    inline Vector3 Vector3::CrossProduct(const Vector3& Left, const Vector3& Right)
    {
        return Left ^ Right;
    }

    inline void Vector3::Normalize()
    {
        float length = sqrt(x * x + y * y + z * z);
        x /= length;
        y /= length;
        z /= length;
    }

    typedef struct Vector4 {
        union {
            float x;
            float r;
        };
        union {
            float y;
            float g;
        };
        union {
            float z;
            float b;
        };
        union {
            float w;
            float a;
        };
    } Vector4;
    typedef Vector4 Color;

    //-------------------------------------------------------------
    // Matrix4x4
    //-------------------------------------------------------------
    struct Matrix4x4 {
    public:
        alignas(16) float m[4][4];

        inline Matrix4x4();
        inline Matrix4x4(const float* array);

        inline void SetIdentity();

        inline Matrix4x4 operator+(const Matrix4x4& other);
        inline Matrix4x4 operator-(const Matrix4x4& other);
        inline Matrix4x4 operator*(const Matrix4x4& other);
        inline void operator+=(const Matrix4x4& other);
        inline void operator*=(const Matrix4x4& other);

        static inline Matrix4x4 LookAt(const Vector3& eye, const Vector3& at, const Vector3& up);
        static inline Matrix4x4 Perspective(const float halfFOV, const float width, const float height, const float fNear, const float fFar);
        static inline Matrix4x4 Perspective(float fovY, float aspectRatio, float front, float back);
        static inline Matrix4x4 Perspective(float l, float r, float b, float t, float n, float f);
        static inline Matrix4x4 Translation(const Vector3& v);
        static inline Matrix4x4 RotationX(float angleInRad);
        static inline Matrix4x4 RotationY(float angleInRad);
        static inline Matrix4x4 RotationZ(float angleInRad);

        void Print();
        void ToString(char* const str, size_t size);
    };

    inline void Matrix4x4::SetIdentity()
    {
        m[0][0] = 1;
        m[0][1] = 0;
        m[0][2] = 0;
        m[0][3] = 0;
        m[1][0] = 0;
        m[1][1] = 1;
        m[1][2] = 0;
        m[1][3] = 0;
        m[2][0] = 0;
        m[2][1] = 0;
        m[2][2] = 1;
        m[2][3] = 0;
        m[3][0] = 0;
        m[3][1] = 0;
        m[3][2] = 0;
        m[3][3] = 1;
    }

    inline Matrix4x4::Matrix4x4()
    {
        SetIdentity();
    }

    inline Matrix4x4::Matrix4x4(const float* array)
    {
        std::memcpy(m, array, 16 * sizeof(float));
    }

    inline Matrix4x4 Matrix4x4::operator+(const Matrix4x4& other)
    {
        Matrix4x4 result;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                result.m[i][j] = m[i][j] + other.m[i][j];
            }
        }
        return result;
    }

    inline Matrix4x4 Matrix4x4::operator-(const Matrix4x4& other)
    {
        Matrix4x4 result;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                result.m[i][j] = m[i][j] - other.m[i][j];
            }
        }
        return result;
    }

    inline Matrix4x4 Matrix4x4::operator*(const Matrix4x4& other)
    {
        Matrix4x4 result;
#if USE_SIMD
        MatrixMultiply(&result, this, &other);
#else

        for (int i = 0; i < 4; i++) {
            float accumulator;
            for (int j = 0; j < 4; j++) {
                for (int k = 0; k < 4; k++) {
                    accumulator += m[i][k] * other.m[k][j];
                }

                result.m[i][j] = accumulator;
            }
        }
#endif
        return result;
    }

    inline void Matrix4x4::operator+=(const Matrix4x4& other)
    {
        *this = *this + other;
    }

    inline void Matrix4x4::operator*=(const Matrix4x4& other)
    {
#if USE_SIMD
        MatrixMultiply(this, this, &other);
#else
        *this = *this * other;
#endif
    }

    Matrix4x4 Matrix4x4::LookAt(const Vector3& eye, const Vector3& at, const Vector3& up)
    {
        Matrix4x4 result;

        Vector3 zAxis = (at - eye);
        zAxis.Normalize();
        Vector3 xAxis = (up ^ zAxis);
        xAxis.Normalize();
        Vector3 yAxis = zAxis ^ xAxis;

        for (int row = 0; row < 3; row++) {
            result.m[row][0] = (&xAxis.x)[row];
            result.m[row][1] = (&yAxis.x)[row];
            result.m[row][2] = (&zAxis.x)[row];
            result.m[row][3] = 0.0f;
        }

        result.m[3][0] = -eye | xAxis;
        result.m[3][1] = -eye | yAxis;
        result.m[3][2] = -eye | zAxis;
        result.m[3][3] = 1.0f;

        return result;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // set a perspective frustum with 6 params similar to glFrustum()
    // (left, right, bottom, top, near, far)
    // Note: this is for row-major notation. OpenGL needs transpose it
    ///////////////////////////////////////////////////////////////////////////////
    Matrix4x4 Matrix4x4::Perspective(float l, float r, float b, float t, float n, float f)
    {
        Matrix4x4 result;

        result.m[0][0] = 2 * n / (r - l);
        result.m[0][2] = (r + l) / (r - l);
        result.m[1][1] = 2 * n / (t - b);
        result.m[1][2] = (t + b) / (t - b);
        result.m[2][2] = -(f + n) / (f - n);
        result.m[2][3] = -(2 * f * n) / (f - n);
        result.m[3][2] = -1;
        result.m[3][3] = 0;

        return result;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // set a symmetric perspective frustum with 4 params similar to gluPerspective
    // (vertical field of view, aspect ratio, near, far)
    ///////////////////////////////////////////////////////////////////////////////
    Matrix4x4 Matrix4x4::Perspective(float fovY, float aspectRatio, float front, float back)
    {
        float tangent = tanf(fovY / 2 * 3.141593f / 180.0f); // tangent of half fovY
        float height = front * tangent; // half height of near plane
        float width = height * aspectRatio; // half width of near plane

        // params: left, right, bottom, top, near, far
        return Perspective(-width, width, -height, height, front, back);
    }

    Matrix4x4 Matrix4x4::Perspective(const float fov, const float width, const float height, const float zNear, const float zFar)
    {
        Matrix4x4 result;
        // set the basic projection matrix
        float scale = 1.0f / tanf(fov * 0.5f * PI_F / 180.0f);
        result.m[0][0] = scale; // scale the x coordinates of the projected point
        result.m[1][1] = scale; // scale the y coordinates of the projected point
        result.m[2][2] = -zFar / (zFar - zNear); // used to remap z to [0,1]
        result.m[3][2] = -zFar * zNear / (zFar - zNear); // used to remap z [0,1]
        result.m[2][3] = -1; // set w = -z
        result.m[3][3] = 0;

        return result;
    }

    Matrix4x4 Matrix4x4::Translation(const Vector3& v)
    {
        Matrix4x4 result;

        result.m[0][0] = 1.0f;
        result.m[0][1] = 0.0f;
        result.m[0][2] = 0.0f;
        result.m[0][3] = v.x;
        result.m[1][0] = 0.0f;
        result.m[1][1] = 1.0f;
        result.m[1][2] = 0.0f;
        result.m[1][3] = v.y;
        result.m[2][0] = 0.0f;
        result.m[2][1] = 0.0f;
        result.m[2][2] = 1.0f;
        result.m[2][3] = v.z;
        result.m[3][0] = 0.0f;
        result.m[3][1] = 0.0f;
        result.m[3][2] = 0.0f;
        result.m[3][3] = 1.0f;

        return result;
    }

    Matrix4x4 Matrix4x4::RotationX(float angleInRad)
    {
        Matrix4x4 result;

        float fCosine, fSine;

        fCosine = cosf(angleInRad);
        fSine = sinf(angleInRad);

        result.m[0][0] = 1.0f;
        result.m[0][1] = 0.0f;
        result.m[0][2] = 0.0f;
        result.m[0][3] = 0.0f;
        result.m[1][0] = 0.0f;
        result.m[1][1] = fCosine;
        result.m[1][2] = fSine;
        result.m[1][3] = 0.0f;
        result.m[2][0] = 0.0f;
        result.m[2][1] = -fSine;
        result.m[2][2] = fCosine;
        result.m[2][3] = 0.0f;
        result.m[3][0] = 0.0f;
        result.m[3][1] = 0.0f;
        result.m[3][2] = 0.0f;
        result.m[3][3] = 1.0f;

        return result;
    }

    Matrix4x4 Matrix4x4::RotationY(float angleInRad)
    {
        Matrix4x4 result;

        float fCosine, fSine;

        fCosine = cosf(angleInRad);
        fSine = sinf(angleInRad);

        result.m[0][0] = fCosine;
        result.m[0][1] = 0.0f;
        result.m[0][2] = -fSine;
        result.m[0][3] = 0.0f;
        result.m[1][0] = 0.0f;
        result.m[1][1] = 1.0f;
        result.m[1][2] = 0.0f;
        result.m[1][3] = 0.0f;
        result.m[2][0] = fSine;
        result.m[2][1] = 0.0f;
        result.m[2][2] = fCosine;
        result.m[2][3] = 0.0f;
        result.m[3][0] = 0.0f;
        result.m[3][1] = 0.0f;
        result.m[3][2] = 0.0f;
        result.m[3][3] = 1.0f;

        return result;
    }

    Matrix4x4 Matrix4x4::RotationZ(float angleInRad)
    {
        Matrix4x4 result;

        float fCosine, fSine;

        fCosine = cosf(angleInRad);
        fSine = sinf(angleInRad);

        result.m[0][0] = fCosine;
        result.m[0][1] = fSine;
        result.m[0][2] = 0.0f;
        result.m[0][3] = 0.0f;
        result.m[1][0] = -fSine;
        result.m[1][1] = fCosine;
        result.m[1][2] = 0.0f;
        result.m[1][3] = 0.0f;
        result.m[2][0] = 0.0f;
        result.m[2][1] = 0.0f;
        result.m[2][2] = 1.0f;
        result.m[2][3] = 0.0f;
        result.m[3][0] = 0.0f;
        result.m[3][1] = 0.0f;
        result.m[3][2] = 0.0f;
        result.m[3][3] = 1.0f;

        return result;
    }

} // namespace mj2
