#version 330 core
layout (location = 0) in vec3 vecPos;
layout (location = 1) in vec3 vecCol;

out vec3 interpolated_ourColour;

void main(){
    interpolated_ourColour = vecCol;
    gl_Position = vec4(vecPos.x, vecPos.y, vecPos.z, 1.0);
}