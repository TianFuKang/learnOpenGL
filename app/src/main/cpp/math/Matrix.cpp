#include "Matrix.hpp"

//namespace m3d
//{
namespace mj2
{
    //-------------------------------------------------------------
    // Vector3
    //-------------------------------------------------------------
    void Vector3::Print()
    {
        printf("(%.9g, %.9g, %.9g)\n", x, y, z);
    }

    void Vector3::ToString(char* const str, size_t size)
    {
        snprintf(str, size, "(%f, %f, %f)", x, y, z);
    }

    //-------------------------------------------------------------
    // Matrix4x4
    //-------------------------------------------------------------
    void Matrix4x4::Print()
    {
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                printf("%f,", m[i][j]);
            }
            printf("\n");
        }
    }

    void Matrix4x4::ToString(char* const str, size_t size)
    {
        snprintf(str,
                 size,
                 "{%f, %f, %f, %f\n%f, %f, %f, %f\n%f, %f, %f, %f\n%f, %f, %f, %f}",
                 m[0][0],
                 m[0][1],
                 m[0][2],
                 m[0][3],
                 m[1][0],
                 m[1][1],
                 m[1][2],
                 m[1][3],
                 m[2][0],
                 m[2][1],
                 m[2][2],
                 m[2][3],
                 m[3][0],
                 m[3][1],
                 m[3][2],
                 m[3][3]
        );
    }
}
//}