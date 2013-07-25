#version 150

uniform sampler2D texture;

smooth in vec2 tex_coord_out;
in vec4 color_out;

out vec4 fragmentColor;

void main()
{
    float a = texture(texture, tex_coord_out.st).r;
    fragmentColor = vec4(color_out.rgb, color_out.a*a);
}
