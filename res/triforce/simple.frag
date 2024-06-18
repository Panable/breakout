#version 330 core
in vec3 chosenColor;
out vec4 FragColor;

uniform float additive;

void main()
{
    FragColor = vec4(chosenColor * additive, 1.0f);
}
