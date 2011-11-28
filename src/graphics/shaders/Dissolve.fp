// /////////////////////////////////////////////////////////////////
// @file Dissolve.fp
// @author Richard S. Wright Jr. and PJ O Halloran.
// @date 22/09/2010
//
// Applies the Phong shading model to each vertex and also shows
// use of the discard command in the fragment shader to make a 
// primitive appear to dissolve over time.
//
// /////////////////////////////////////////////////////////////////

#version 130

out vec4 fragmentColorVec;

uniform vec4       ambientColor;
uniform vec4       diffuseColor;   
uniform vec4       specularColor;
uniform sampler2D  cloudTexture;
uniform float      dissolveFactor;

smooth in vec3 varyingNormalVec;
smooth in vec3 varyingLightDir;
smooth in vec2 varyingTexCoordVec;

void main(void)
{ 
    vec4 vCloudSample = texture(cloudTexture, varyingTexCoordVec);

    if(vCloudSample.r < dissolveFactor)
        discard;
   

    // Dot product gives us diffuse intensity
    float diff = max(0.0, dot(normalize(varyingNormalVec), normalize(varyingLightDir)));

    // Multiply intensity by diffuse color, force alpha to 1.0
    fragmentColorVec = diff * diffuseColor;

    // Add in ambient light
    fragmentColorVec += ambientColor;


    // Specular Light
    vec3 reflectionVec = normalize(reflect(-normalize(varyingLightDir), normalize(varyingNormalVec)));
    float spec = max(0.0, dot(normalize(varyingNormalVec), reflectionVec));
    if(diff != 0)
    {
        float fSpec = pow(spec, 128.0);
        fragmentColorVec.rgb += vec3(fSpec, fSpec, fSpec);
    }
}
