#version 330 core
layout (location = 0) in vec2 Pos;

void main()
{
    gl_Position = vec4(Pos, 0.0f, 1.0f);
}
