// /////////////////////////////////////////////////////////////////
// @file GuiTextureColor.fp
// @author PJ O Halloran
// @date 25/10/2010
//
// Shader for drawing GUI widgets overlayed on my application
// using an orthographic projection matrix.
//
// Sets the fragment color to be the color of the current texture
// coordinate.
//
// /////////////////////////////////////////////////////////////////

//#version 330
#version 150

// Texture sampler
uniform sampler2D colorMap;

uniform float uiAlpha;

// Input texture coordinates.
smooth in vec2 varyTexCoords;

// Output fragment color
out vec4 fragmentColorVec;

void main(void)
{
	if(uiAlpha < 0.1f)
	{
		discard;
	}

	fragmentColorVec = texture(colorMap, varyTexCoords.st);
	fragmentColorVec.a = uiAlpha;
}
