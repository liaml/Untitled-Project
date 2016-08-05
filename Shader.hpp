#ifndef Shader_hpp
#define Shader_hpp

#include <GL/glew.h>

class Shader {
    
public:

    GLuint ProgramID;
    
public:
    
    Shader(void) { }
    ~Shader(void) { glDeleteProgram(ProgramID); }
    
    GLuint LoadShaders(const char* vFPath, const char * fFPath);
    
    GLuint GetProgramID(void) { return ProgramID; }
    GLvoid UseProgram(GLuint ProgramID) { glUseProgram(ProgramID); }
    
};

#endif /* Shader_hpp */
