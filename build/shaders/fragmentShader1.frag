#version 330 core
out vec4 FragColour;
in vec3 interpolated_ourColour;
void main(){
    FragColour = vec4(interpolated_ourColour.x, interpolated_ourColour.y, interpolated_ourColour.z, 1.0f);
}