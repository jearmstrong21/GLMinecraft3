//
// Created by Jack Armstrong on 3/24/19.
//

#include "gl.h"

void _gl_check_error(const char*file,int line){
    GLuint code=glGetError();
    switch(code){
        case GL_NO_ERROR:
            break;
        case GL_INVALID_ENUM:
            printf("OPENGL ERROR %s, L%i: Invalid Enum\n",file,line);
            break;
        case GL_INVALID_VALUE:
            printf("OPENGL ERROR %s, L%i: Invalid Value\n",file,line);
            break;
        case GL_INVALID_OPERATION:
            printf("OPENGL ERROR %s, L%i: Invalid Operation\n",file,line);
            break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            printf("OPENGL ERROR %s, L%i: Invalid Framebuffer Operation\n",file,line);
            break;
        case GL_OUT_OF_MEMORY:
            printf("OPENGL ERROR %s, L%i: Out of Memory\n",file,line);
            break;
        default:
            printf("OPENGL ERROR %s, L%i: UNKNOWN CODE %i\n",file,line,code);
//        case :
//            printf
    }
}
