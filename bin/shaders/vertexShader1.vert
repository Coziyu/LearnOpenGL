#version 330 core
layout (location = 0) in vec3 vecPos;
layout (location = 1) in vec3 vecCol;
layout (location = 2) in vec2 vecTexCoord;

out vec3 interpolated_ourColour;
out vec2 texCoord;

uniform mat4 transform;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
void main(){
    interpolated_ourColour = vecCol;
    texCoord = vec2(vecTexCoord.x,vecTexCoord.y);
    gl_Position = projection * view * model * vec4(vecPos, 1.0);
}