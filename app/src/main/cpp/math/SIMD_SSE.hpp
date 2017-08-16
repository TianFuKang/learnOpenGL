#pragma once

#include <emmintrin.h> // SSE2

namespace mj2 {
    using VectorSIMD = __m128;

    inline VectorSIMD MakeVectorSIMD(float fX, float fY, float fZ, float fW)
    {
        return _mm_setr_ps(fX, fY, fZ, fW);
    }

#define VectorLoad4f(ptr) _mm_load_ps((float*)(ptr));
#define VectorStore4f(vec, ptr) _mm_store_ps((float*)(ptr), vec)

#define SHUFFLEMASK(A0, A1, B2, B3) ((A0) | ((A1) << 2) | ((B2) << 4) | ((B3) << 6))

#define VectorMultiply(v0, v1) _mm_mul_ps(v0, v1)
#define VectorMultiplyAdd(v0, v1, v2) _mm_add_ps(_mm_mul_ps(v0, v1), v2)
#define VectorReplicate(v, index) _mm_shuffle_ps(v, v, SHUFFLEMASK(index, index, index, index))
#define VectorSwizzle(vec, x, y, z, w) _mm_shuffle_ps(vec, vec, SHUFFLEMASK(x, y, z, w))

    inline void MatrixMultiply(void* result, const void* left, const void* right)
    {
        const VectorSIMD* _left = (const VectorSIMD*)left;
        const VectorSIMD* _right = (const VectorSIMD*)right;
        VectorSIMD* _result = (VectorSIMD*)result;
        VectorSIMD temp, row0, row1, row2, row3;

        // row 0
        temp = VectorMultiply(VectorReplicate(_left[0], 0), _right[0]);
        temp = VectorMultiplyAdd(VectorReplicate(_left[0], 1), _right[1], temp);
        temp = VectorMultiplyAdd(VectorReplicate(_left[0], 2), _right[2], temp);
        row0 = VectorMultiplyAdd(VectorReplicate(_left[0], 3), _right[3], temp);

        // row 1
        temp = VectorMultiply(VectorReplicate(_left[1], 0), _right[0]);
        temp = VectorMultiplyAdd(VectorReplicate(_left[1], 1), _right[1], temp);
        temp = VectorMultiplyAdd(VectorReplicate(_left[1], 2), _right[2], temp);
        row1 = VectorMultiplyAdd(VectorReplicate(_left[1], 3), _right[3], temp);

        // row 2
        temp = VectorMultiply(VectorReplicate(_left[2], 0), _right[0]);
        temp = VectorMultiplyAdd(VectorReplicate(_left[2], 1), _right[1], temp);
        temp = VectorMultiplyAdd(VectorReplicate(_left[2], 2), _right[2], temp);
        row2 = VectorMultiplyAdd(VectorReplicate(_left[2], 3), _right[3], temp);

        // row 3
        temp = VectorMultiply(VectorReplicate(_left[3], 0), _right[0]);
        temp = VectorMultiplyAdd(VectorReplicate(_left[3], 1), _right[1], temp);
        temp = VectorMultiplyAdd(VectorReplicate(_left[3], 2), _right[2], temp);
        row3 = VectorMultiplyAdd(VectorReplicate(_left[3], 3), _right[3], temp);

        _result[0] = row0;
        _result[1] = row1;
        _result[2] = row2;
        _result[3] = row3;
    }

    static const VectorSIMD QMULTI_SIGN_MASK0 = MakeVectorSIMD(1.0f, -1.0f, 1.0f, -1.0f);
    static const VectorSIMD QMULTI_SIGN_MASK1 = MakeVectorSIMD(1.0f, 1.0f, -1.0f, -1.0f);
    static const VectorSIMD QMULTI_SIGN_MASK2 = MakeVectorSIMD(-1.0f, 1.0f, 1.0f, -1.0f);

    inline VectorSIMD VectorQuaternionMultiply2(const VectorSIMD& Quat1, const VectorSIMD& Quat2)
    {
        VectorSIMD resultSIMD = VectorMultiply(VectorReplicate(Quat1, 3), Quat2);
        resultSIMD = VectorMultiplyAdd(VectorMultiply(VectorReplicate(Quat1, 0), VectorSwizzle(Quat2, 3, 2, 1, 0)), QMULTI_SIGN_MASK0, resultSIMD);
        resultSIMD = VectorMultiplyAdd(VectorMultiply(VectorReplicate(Quat1, 1), VectorSwizzle(Quat2, 2, 3, 0, 1)), QMULTI_SIGN_MASK1, resultSIMD);
        resultSIMD = VectorMultiplyAdd(VectorMultiply(VectorReplicate(Quat1, 2), VectorSwizzle(Quat2, 1, 0, 3, 2)), QMULTI_SIGN_MASK2, resultSIMD);

        return resultSIMD;
    }

    inline void QuaternionMultiply(
#ifdef _MSC_VER
    void* __restrict resultSIMD,
        const void* __restrict quat0,
        const void* __restrict quat1
#else
            void* __restrict__ resultSIMD,
            const void* __restrict__ quat0,
            const void* __restrict__ quat1
#endif
    )
    {
        *((VectorSIMD*)resultSIMD) = VectorQuaternionMultiply2(*((const VectorSIMD*)quat0), *((const VectorSIMD*)quat1));
    }
} // end of namespace mj2
