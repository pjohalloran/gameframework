// /////////////////////////////////////////////////////////////////
// @file phong.fp
// @author Richard S. Wright Jr. and PJ O Halloran.
// @date 22/09/2010
//
// Applies the Phong shading model to each vertex.
//
// Per pixel illumiation using ambient, diffuse and specular lighting.
//
// The vetices normal vector is interpolated across the surface of the
// primitive as well as the light direction.
//
// The interpolated normal and light direction are used here for each
// fragment to calculate the color.
//
// /////////////////////////////////////////////////////////////////

#version 130

out vec4 fragmentColorVec;

uniform vec4    ambientColor;
uniform vec4    diffuseColor;   
uniform vec4    specularColor;

smooth in vec3 varyingNormalVec;
smooth in vec3 varyingLightDirVec;


void main(void)
{ 
    // Dot product gives us diffuse intensity
    float diff = max(0.0, dot(normalize(varyingNormalVec), normalize(varyingLightDirVec)));

    // Multiply intensity by diffuse color, force alpha to 1.0
    fragmentColorVec = diff * diffuseColor;

    // Add in ambient light
    fragmentColorVec += ambientColor;

    // Specular Light
    vec3 reflectionVec = normalize(reflect(-normalize(varyingLightDirVec), normalize(varyingNormalVec)));
    float spec = max(0.0, dot(normalize(varyingNormalVec), reflectionVec));
    if(diff != 0)
    {
        float fSpec = pow(spec, 128.0);
        fragmentColorVec.rgb += vec3(fSpec, fSpec, fSpec);
    }
}
