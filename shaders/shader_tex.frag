#version 430 core

uniform vec3 objectColor;
uniform vec3 lightDir;
uniform sampler2D uni; //+

in vec3 interpNormal;
in vec2 Coor;

void main()
{
	vec3 normal = normalize(interpNormal);
	float diffuse = max(dot(normal, -lightDir), 0.0)*2;
	
	vec4 textureColor = texture2D(uni, Coor); 
	gl_FragColor = vec4(textureColor.rgb * diffuse, 1.0);
}
