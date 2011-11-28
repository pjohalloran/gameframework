// /////////////////////////////////////////////////////////////////
// @file GoraudTexture.fp
// @author Richard S. Wright Jr. and PJ O Halloran.
// @date 22/09/2010
//
// Applies the Goraud shading model to each fragment based on input
// from the vertex shader.
//
// The vertex color is interpolated across the surface of the 
// primitive.
//
// A texture is also applied to the fragment.
//
// /////////////////////////////////////////////////////////////////

#version 130

out vec4 fragmentColorVec;

uniform sampler2D colorMap;
uniform vec4    specularColor;

// Color to fragment program
smooth in vec4 varyingColorVec;

// Texture coordinate to fragment program.
smooth in vec2 varyingTexCoordVec;

// Light direction to fragment program.
flat in vec3 flatLightDirVec;

// Vertex normal to fragment program.
flat in vec3 flatEyeNormalVec;

flat in float flatDiff;

void main(void)
{
	// Apply the interpolated color (plus the ambient and diffuse componenents) to the fragment.
	fragmentColorVec = varyingColorVec;
	
	// Apply the texture color to the fragement.
	fragmentColorVec *= texture(colorMap, varyingTexCoordVec);

    // Apply Specular Light for the fragment.
    vec3 reflectionVec = normalize(reflect(-flatLightDirVec, flatEyeNormalVec));
    float spec = max(0.0, dot(flatEyeNormalVec, reflectionVec));
    if(flatDiff != 0)
    {
        float fSpec = pow(spec, 128.0);
        fragmentColorVec.rgb += vec3(fSpec, fSpec, fSpec);
	}
}
