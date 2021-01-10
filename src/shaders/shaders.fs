#version 330 core
out vec4 FragColor;
in vec4 vertexColor;
uniform float ourColor;
void main()
{
   FragColor = vec4(ourColor, 0.0f, 0.0f, 1.0f);
}
