#version 330

in vec4 v_VCol;
in vec2 v_TexCoord;

out vec4 o_Color;

uniform sampler2D u_Texture;

void main()
{
	o_Color = texture(u_Texture, v_TexCoord);
}