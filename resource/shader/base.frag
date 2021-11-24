#version 330 core

in vec2 texPos;
out vec4 FragColor;

uniform sampler2D _texture;
void main(){
    FragColor=texture(_texture,texPos);
}