#version 330 core
out vec4 FragColour;

in vec3 interpolated_ourColour;
in vec2 texCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float blend;

void main(){
    //FragColour = vec4(interpolated_ourColour.x, interpolated_ourColour.y, interpolated_ourColour.z, 1.0f);
    FragColour = mix(texture(texture1, texCoord), texture(texture2, texCoord), blend);
}