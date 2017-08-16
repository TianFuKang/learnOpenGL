cmake_minimum_required( VERSION 3.4.1 )

project ( mj2math )

set( neon_SRCS SIMD_NEON.cpp )
set_property( SOURCE ${neon_SRCS}
               APPEND_STRING PROPERTY COMPILE_FLAGS " -mfpu=neon -mfloat-abi=hard")

add_library( mj2math STATIC
	Matrix.cpp
	${neon_SRCS}
	SIMD_SSE.cpp
)
