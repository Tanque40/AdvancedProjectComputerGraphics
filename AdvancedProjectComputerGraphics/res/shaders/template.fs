#version 410 core
in vec3 ourColor;
in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D ourTexture;

void main(){
	vec4 texColor = texture(ourTexture, TexCoord);
	FragColor = texColor;
}