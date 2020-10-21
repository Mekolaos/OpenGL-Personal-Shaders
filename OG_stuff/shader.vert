#version 330 core
layout (location = 0) in vec3 aPos;

// uniform mat4 viewRot;

void main()
{
    gl_Position = vec4(aPos, 1.0);
    gl_Position.w = 1.0;
    
}