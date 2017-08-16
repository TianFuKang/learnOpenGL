/*
 * Copyright (C) 2009 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// OpenGL ES 2.0 code

#include <jni.h>
#include <android/log.h>

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "math/Matrix.hpp"

#define  LOG_TAG    "libgl2jni"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

static void printGLString(const char *name, GLenum s) {
    const char *v = (const char *) glGetString(s);
    LOGI("GL %s = %s\n", name, v);
}

static void checkGlError(const char* op) {
    for (GLint error = glGetError(); error; error
                                                    = glGetError()) {
        LOGI("after %s() glError (0x%x)\n", op, error);
    }
}

auto gVertexShader =
        "attribute vec4 vPosition;\n"
                "attribute vec4 a_color;\n"
                "varying vec4 v_fragmentColor;\n"
                "uniform mat4 rotationMatrixUniform;\n"
                "void main() {\n"
                "  gl_Position = rotationMatrixUniform * vPosition;\n"
                "  v_fragmentColor = a_color;\n"
                "}\n";

auto gFragmentShader =
        "varying vec4 v_fragmentColor;\n"
                "void main() {\n"
                "  gl_FragColor = v_fragmentColor;\n"
                "}\n";

GLuint loadShader(GLenum shaderType, const char* pSource) {
    GLuint shader = glCreateShader(shaderType);
    if (shader) {
        glShaderSource(shader, 1, &pSource, NULL);
        glCompileShader(shader);
        GLint compiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
        if (!compiled) {
            GLint infoLen = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
            if (infoLen) {
                char* buf = (char*) malloc(infoLen);
                if (buf) {
                    glGetShaderInfoLog(shader, infoLen, NULL, buf);
                    LOGE("Could not compile shader %d:\n%s\n",
                         shaderType, buf);
                    free(buf);
                }
                glDeleteShader(shader);
                shader = 0;
            }
        }
    }
    return shader;
}

GLuint createProgram(const char* pVertexSource, const char* pFragmentSource) {
    GLuint vertexShader = loadShader(GL_VERTEX_SHADER, pVertexSource);
    if (!vertexShader) {
        return 0;
    }

    GLuint pixelShader = loadShader(GL_FRAGMENT_SHADER, pFragmentSource);
    if (!pixelShader) {
        return 0;
    }

    GLuint program = glCreateProgram();
    if (program) {
        glAttachShader(program, vertexShader);
        checkGlError("glAttachShader");
        glAttachShader(program, pixelShader);
        checkGlError("glAttachShader");
        glLinkProgram(program);
        GLint linkStatus = GL_FALSE;
        glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
        if (linkStatus != GL_TRUE) {
            GLint bufLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufLength);
            if (bufLength) {
                char* buf = (char*) malloc(bufLength);
                if (buf) {
                    glGetProgramInfoLog(program, bufLength, NULL, buf);
                    LOGE("Could not link program:\n%s\n", buf);
                    free(buf);
                }
            }
            glDeleteProgram(program);
            program = 0;
        }
    }
    return program;
}

GLuint gProgram;
GLuint a_color;
GLuint vPosition;
GLuint rotationMatrixUniform;
mj2::Matrix4x4 modelMatrix;
mj2::Matrix4x4 rotationMatrix;

bool setupGraphics(int w, int h) {
    modelMatrix.SetIdentity();
    rotationMatrix.SetIdentity();
    rotationMatrix = rotationMatrix.RotationY( 3.14f / 180.0f );


    printGLString("Version", GL_VERSION);
    printGLString("Vendor", GL_VENDOR);
    printGLString("Renderer", GL_RENDERER);
    printGLString("Extensions", GL_EXTENSIONS);

    LOGI("setupGraphics(%d, %d)", w, h);
    gProgram = createProgram(gVertexShader, gFragmentShader);
    if (!gProgram) {
        LOGE("Could not create program.");
        return false;
    }
    vPosition = glGetAttribLocation(gProgram, "vPosition");
    checkGlError("glGetAttribLocation");
    LOGI("glGetAttribLocation(\"vPosition\") = %d\n",
         vPosition);

    a_color = glGetAttribLocation(gProgram, "a_color");
    checkGlError("glGetAttribLocation");
    LOGI("glGetAttribLocation(\"a_color\") = %d\n",
         a_color);

    rotationMatrixUniform = glGetUniformLocation(gProgram, "rotationMatrixUniform");
    checkGlError("glGetUniformLocation");
    LOGI("glGetUniformLocation(\"rotationMatrixUniform\") = %d\n",
         rotationMatrixUniform);

    // create rotation matrix
    //
    //float rotationMatrix[16] = {};

    glViewport(0, 0, w, h);
    checkGlError("glViewport");

    // cull face
    glEnable( GL_CULL_FACE );
    glCullFace( GL_FRONT );
    //逆时针---正面
    //顺时针---背面

    return true;
}

GLfloat _r = 3.1415926f / 18.0f;

GLfloat gTriangleVertices[] = { -0.25f, -0.25f,  -0.25f, 1.0f,
                                0.25f, -0.25f,  -0.25f, 1.0f,
                                0.25f, -0.25f, 0.25f, 1.0f, // 1
                                -0.25f, -0.25f,  -0.25f, 1.0f,
                                0.25f, -0.25f, 0.25f, 1.0f,
                                -0.25f, -0.25f, 0.25f, 1.0f,

                                -0.25f, -0.25f,  0.25f, 1.0f,
                                0.25f, -0.25f,  0.25f, 1.0f,
                                0.25f,  0.25f,  0.25f, 1.0f,// 2
                                -0.25f, -0.25f,  0.25f, 1.0f,
                                0.25f,  0.25f,  0.25f, 1.0f,
                                -0.25f,  0.25f,  0.25f, 1.0f,

                                -0.25f, -0.25f,  -0.25f, 1.0f,
                                -0.25f,  -0.25f,  0.25f, 1.0f,
                                -0.25f,  0.25f, 0.25f, 1.0f, //3
                                -0.25f, -0.25f,  -0.25f, 1.0f,
                                -0.25f,  0.25f, 0.25f, 1.0f,
                                -0.25f, 0.25f, -0.25f, 1.0f,

                                0.25f,  0.25f, -0.25f, 1.0f,
                                -0.25f,  0.25f,  0.25f, 1.0f,
                                0.25f,  0.25f,  0.25f, 1.0f, //4
                                0.25f,  0.25f, -0.25f, 1.0f,
                                -0.25f,  0.25f, -0.25f, 1.0f,
                                -0.25f,  0.25f,  0.25f, 1.0f,

                                0.25f,  0.25f, -0.25f, 1.0f,
                                0.25f,  0.25f,  0.25f, 1.0f,
                                0.25f, -0.25f,  0.25f, 1.0f, //5
                                0.25f,  0.25f, -0.25f, 1.0f,
                                0.25f, -0.25f,  0.25f, 1.0f,
                                0.25f, -0.25f, -0.25f, 1.0f,

                                0.25f,  0.25f, -0.25f, 1.0f,
                                0.25f,  -0.25f, -0.25f, 1.0f,
                                -0.25f, -0.25f, -0.25f, 1.0f, //6
                                0.25f,  0.25f, -0.25f, 1.0f,
                                -0.25f, -0.25f, -0.25f, 1.0f,
                                -0.25f, 0.25f, -0.25f, 1.0f };

const GLfloat gTriangleColors[] = {     0.583f, 0.771f, 0.014f, 1.0f,
                                        0.609f, 0.115f, 0.436f, 1.0f,
                                        0.327f, 0.483f, 0.844f, 1.0f, //1
                                        0.822f, 0.569f, 0.201f, 1.0f,
                                        0.435f, 0.602f, 0.223f, 1.0f,
                                        0.310f, 0.747f, 0.185f, 1.0f,
                                        0.597f, 0.770f, 0.761f, 1.0f,
                                        0.559f, 0.436f, 0.730f, 1.0f,
                                        0.359f, 0.583f, 0.152f, 1.0f, //2
                                        0.483f, 0.596f, 0.789f, 1.0f,
                                        0.559f, 0.861f, 0.639f, 1.0f,
                                        0.195f, 0.548f, 0.859f, 1.0f,
                                        0.014f, 0.184f, 0.576f, 1.0f,
                                        0.771f, 0.328f, 0.970f, 1.0f,
                                        0.406f, 0.615f, 0.116f, 1.0f, //3
                                        0.676f, 0.977f, 0.133f, 1.0f,
                                        0.971f, 0.572f, 0.833f, 1.0f,
                                        0.140f, 0.616f, 0.489f, 1.0f,
                                        0.997f, 0.513f, 0.064f, 1.0f,
                                        0.945f, 0.719f, 0.592f, 1.0f,
                                        0.543f, 0.021f, 0.978f, 1.0f, //4
                                        0.279f, 0.317f, 0.505f, 1.0f,
                                        0.167f, 0.620f, 0.077f, 1.0f,
                                        0.347f, 0.857f, 0.137f, 1.0f,
                                        0.055f, 0.953f, 0.042f, 1.0f,
                                        0.714f, 0.505f, 0.345f, 1.0f,
                                        0.783f, 0.290f, 0.734f, 1.0f, //5
                                        0.722f, 0.645f, 0.174f, 1.0f,
                                        0.302f, 0.455f, 0.848f, 1.0f,
                                        0.225f, 0.587f, 0.040f, 1.0f,
                                        0.517f, 0.713f, 0.338f, 1.0f,
                                        0.053f, 0.959f, 0.120f, 1.0f,
                                        0.393f, 0.621f, 0.362f, 1.0f, //6
                                        0.673f, 0.211f, 0.457f, 1.0f,
                                        0.820f, 0.883f, 0.371f, 1.0f,
                                        0.982f, 0.099f, 0.879f, 1.0f };

//float rotationMatrix[16] = { 1.0f,      0.0f,     0.0f, 0.0f, //X
//                             0.0f,  cosf(_r), sinf(_r), 0.0f,
//                             0.0f, -sinf(_r), cosf(_r), 0.0f,
//                             0.0f,      0.0f,     0.0f, 1.0f };
//const float rotationMatrix[16] = {  cosf(_r), 0.0f, -sinf(_r), 0.0f, //Y
//                                        0.0f, 1.0f,     0.0f, 0.0f,
//                                   sinf(_r), 0.0f, cosf(_r), 0.0f,
//                                        0.0f, 0.0f,     0.0f, 1.0f };
//const float rotationMatrix[16] = {  cosf(_r), 0.0f, -sinf(_r), 0.0f, //Y
//                                         0.0f, 1.0f,      0.0f, 0.0f,
//                                     sinf(_r), 0.0f,  cosf(_r), 0.0f,
//                                         0.0f, 0.0f,      0.0f, 1.0f };

void renderFrame() {

    glClearColor( 1.0f,  1.0f,  1.0f, 1.0f);
    checkGlError("glClearColor");
    glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    checkGlError("glClear");

    glUseProgram(gProgram);
    checkGlError("glUseProgram");

    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, gTriangleVertices);
    checkGlError("glVertexAttribPointer");
    glEnableVertexAttribArray(vPosition);
    checkGlError("glEnableVertexAttribArray");

    glVertexAttribPointer(a_color, 4, GL_FLOAT, GL_FALSE, 0, gTriangleColors);
    checkGlError("glVertexAttribPointer");
    glEnableVertexAttribArray(a_color);
    checkGlError("glEnableVertexAttribArray");

    // set the roatation uniform
    modelMatrix = rotationMatrix * modelMatrix;

    glUniformMatrix4fv(rotationMatrixUniform, 1, GL_FALSE, &modelMatrix.m[0][0]);
    checkGlError("glUniformMatrix4fv");

    glDrawArrays(GL_TRIANGLES, 0, 36);

    checkGlError("glDrawArrays");

}

extern "C" {
JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_init(JNIEnv * env, jobject obj,  jint width, jint height);
JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_step(JNIEnv * env, jobject obj);
};

JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_init(JNIEnv * env, jobject obj,  jint width, jint height)
{
    setupGraphics(width, height);

}

JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_step(JNIEnv * env, jobject obj)
{
    renderFrame();

}


//rotationMatrix[0] = cosf(_r);
//rotationMatrix[2] = -sinf(_r);
//rotationMatrix[8] = sinf(_r);
//rotationMatrix[10] = cosf(_r);
//_r = _r + 0.005f;
//if( _r > 314 ) {
//    _r = 0.0f;
//}