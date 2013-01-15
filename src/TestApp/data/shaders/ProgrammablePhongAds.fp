// /////////////////////////////////////////////////////////////////
// @file ProgrammableAds.fp
// @author PJ O Halloran.
// @date 08/12/2010
//
// FRAGMENT SHADER
//
// Implementation of the ADS lighting model commonly used in 3D scenes.
// This lighting model is the same as that used in the fixed function
// pipeline of OpenGL and others.
//
// - Features:
// - Phong shading.
// - Texture application [optional].
// - Dynamic number of lights (8 maximum - max may be increased by
//		changing the constant below MAX_NUMBER_LIGHTS upto maximum
//		allowable by your graphics hardware (8 is a good general number)).
// - Light attenuation.
// - Three different types of lights (Directional, Positional and
//		spotlights).
//
// References:
//  - This is mainly an implementation of the ADS lighting model as described
// in the OpenGL Programming Guide (Red book) 7th edition by David
// Shreiner et al.  See Chapter 5: Lighting: The Matematics of Lighting.
//
// I also referred to:
//  - OpenGL Superbible, 5th edition by Richard Wright et al.
// Chapter 6: Shaders: ADS lighting model.
//
// - Fundamentals of Computer Graphics 3rd Edition by Peter Shirley
// et al.  Chapter 10: Surface shading, Chapter 20: Light and
// Chapter 24: Global illumination.
//
// - Mathematics for Computer Graphics, 2nd edition by John Vince.
//
// /////////////////////////////////////////////////////////////////

// Force use of GLSL version 3.3
//#version 330
#version 150

// /////////////////////////////////////////////////////////////////
// Program constants.
const int MAX_NUMBER_LIGHTS = 8;							// Maximum number of lights allowed in the scene.
const int DIRECTIONAL_LIGHT_TYPE = 0;						// Directional light type (infinitly far away e.g. sun, illminates all vertices equally).
const int POSITIONAL_LIGHT_TYPE = 1;						// Positional light type (has a position in the scene, e.g. candle ,illminates all vertices depending on their position).
const int SPOTLIGHT_LIGHT_TYPE = 2;							// Spotlight (e.g. Flashlight, illminates vertices inside its cone of illumination)
const int DEFAULT_LIGHT_TYPE = DIRECTIONAL_LIGHT_TYPE;		// Default light type in case a missing or invalid value is found.
// /////////////////////////////////////////////////////////////////


// /////////////////////////////////////////////////////////////////
// Incoming Vertex Shader parameters.
smooth in vec3 vp_varyingNormalVec;							// The vertex normal smoothly interpolated across the surface of the triangle.
smooth in vec3 vp_varyingLightDirArr[MAX_NUMBER_LIGHTS];	// Array of direction vectors to the lights smoothly interpolated accross the triangle.
flat in vec3 vp_flatVertexPosition;							// Vertex position (camera space) sent to the fragment shader.

smooth in vec2 vp_texCoordsVec;								// Texture coordinates smoothly interpolated accross the triangle (TEXTURE mode).

smooth in vec4 vp_varyingVertexPosition;

// /////////////////////////////////////////////////////////////////


// /////////////////////////////////////////////////////////////////
// GLSL shader uniforms.

// /////////////////////////////////////////////////////////////////
// 1) High level switches
uniform bool u_applyTexture;								// Is there a texture being applied?
// /////////////////////////////////////////////////////////////////

// /////////////////////////////////////////////////////////////////
// 2) Light(s) information.
uniform int u_numberLights;									// Number of lights in the scene (should be <= MAX_NUMBER_LIGHTS constant!).
uniform int u_lightTypesArr[MAX_NUMBER_LIGHTS];				// Array of light types.
uniform vec3 u_lightPositionArr[MAX_NUMBER_LIGHTS];			// Array of light positions in the scene (Must be in camera space, else the results will be wrong).
uniform vec4 u_lightAmbientArr[MAX_NUMBER_LIGHTS];			// Array of light ambient properties.
uniform vec4 u_lightDiffuseArr[MAX_NUMBER_LIGHTS];			// Array of light diffuse properties.
uniform vec4 u_lightSpecularArr[MAX_NUMBER_LIGHTS];			// Array of light specular properties.
uniform float u_spotlightCutoffArr[MAX_NUMBER_LIGHTS];		// Spotlight cutoff factor for each light (== 180.0 if the light is not a spotlight).
uniform float u_spotlightExpArr[MAX_NUMBER_LIGHTS];			// Spotlight exponent factor for each light (N/A if light is not a spotlight).
uniform vec3 u_spotlightDirection[MAX_NUMBER_LIGHTS];		// Spotlight direction (N/A if light is not a spotlight).
uniform float u_cAttArr[MAX_NUMBER_LIGHTS];					// Constant Attenuation value array.
uniform float u_lAttArr[MAX_NUMBER_LIGHTS];					// Linear Attenuation value array.
uniform float u_qAttArr[MAX_NUMBER_LIGHTS];					// Quadratic Attenuation value array.
// /////////////////////////////////////////////////////////////////

// /////////////////////////////////////////////////////////////////
// 3) Material information
uniform vec4 u_materialE;									// Materials emmissive value.
uniform vec4 u_materialA;									// Materials ambient value.
uniform vec4 u_materialD;									// Materials diffuse value.
uniform vec4 u_materialS;									// Materials specular value.
uniform float u_materialExp;								// Materials exponent/shininess.
// /////////////////////////////////////////////////////////////////

// /////////////////////////////////////////////////////////////////
// 4) Global illumination
uniform vec4 u_globalAmbient;								// Global ambient illumination in the scene.
// /////////////////////////////////////////////////////////////////

// /////////////////////////////////////////////////////////////////
// 5) Texture map access.
uniform sampler2D u_texture2dMap;							// The texture map sampler.
// /////////////////////////////////////////////////////////////////

uniform vec4 u_cameraPos;

//struct Fog
//{
//    bool on;
//    float minDistance;
//    float maxDistance;
//    vec3 color;
//};
//uniform Fog u_fog;

uniform int u_fogType;
uniform float u_fogMin;
uniform float u_fogMax;
uniform float u_fogDensity;
uniform vec3 u_fogColor;

const int FOG_OFF = 0;
const int FOG_LINEAR = 1;
const int FOG_EXPONENTIAL = 2;
const int FOG_EXPONENTIAL_BY_TWO = 3;

// /////////////////////////////////////////////////////////////////


// /////////////////////////////////////////////////////////////////
// Fragment Shader output parameters.
out vec4 fp_colorVec;
// /////////////////////////////////////////////////////////////////


// /////////////////////////////////////////////////////////////////
// Check if two floating point numbers are equal taking floating
// point error into account.  
//
// @param a Value a.
// @param b Value b.
// @param epsilon Negligible difference to ignore.
//
// @return bool True if both values are within the epsilon apart.
//
// /////////////////////////////////////////////////////////////////
bool FloatCmp(float a, float b, float epsilon)
{
	return(abs(a - b) <= epsilon);
}

// /////////////////////////////////////////////////////////////////
// Get the scaled global ambient light factor.
// A term from the ADS vertex color formula defined on pg. 244 in
// Chapter 5: Lighting of the OpenGL Programming Guide, 7th edition.
//
// @return vec3 Global ambient light color vector (rgb).
//
// /////////////////////////////////////////////////////////////////
vec3 GetScaledGlobalAmbientLight()
{
	return (u_globalAmbient.rgb * u_materialA.rgb);
}

// /////////////////////////////////////////////////////////////////
// Get the attenuation factor.
// A term from the ADS vertex color formula defined on pg. 244 in
// Chapter 5: Lighting of the OpenGL Programming Guide, 7th edition.
//
// @param lightType The type of light.
// @param lightPos Position of the light.
// @param cameraVertexPos Position of the vertex in camera space.
// @param cAtt Constant attenuation value for this light.
// @param lAtt Linear attenuation value for this light.
// @param qAtt Quadratic attenuation value for this light.
//
// @return Attenuation Factor (0.0 <= af <= 1.0).
//
// /////////////////////////////////////////////////////////////////
float GetAttenuationFactor(int lightType, vec3 lightPos, vec3 cameraVertexPos, float cAtt, float lAtt, float qAtt)
{
	// Directional light is infinitely far away so we disable attenuation for it.
	if(lightType == DIRECTIONAL_LIGHT_TYPE)
	{
		return (1.0);
	}
	
	float d = distance(cameraVertexPos, lightPos);

	float denom = (cAtt + (lAtt * d) + (qAtt * d * d));

	if(FloatCmp(denom, 0.0, 0.000001))
	{
		// Avoid divide by zero if the light source has C,L and Q attenuation factors set as zero
		//  (we will ignore attenuation in this case).
		return (1.0);
	}
	
	return (1.0 / denom);
}

// /////////////////////////////////////////////////////////////////
// Get the spotlight effect.
// A term from the ADS vertex color formula defined on pg. 244 in
// Chapter 5: Lighting of the OpenGL Programming Guide, 7th edition.
//
// @param lightType Type of the light.
// @param dirFromLight Unit Direction vector from the light to the
//						vertex.
// @param spotlightDir Direction the spotlight is aiming at.
// @param spotlightCutoff 1/2 the cones angle of illumination.
// @param spotlightExp The intensity of the spotlight.
//
// @return The spotlight effect (0.0 <= se <= 1.0).
//
// /////////////////////////////////////////////////////////////////
float GetSpotlightEffect(int lightType, vec3 dirFromLight, vec3 spotlightDir, float spotlightCutoff, float spotlightExp)
{
	// Check if the light is not a spot light or if its cone is surrounding the light source.
	if(((lightType == DIRECTIONAL_LIGHT_TYPE) || (lightType == POSITIONAL_LIGHT_TYPE)) || (FloatCmp(spotlightCutoff, 180.0, 0.000001)))
	{
		return (1.0);
	}
	
	float dotFromSpot = max(dot(dirFromLight, spotlightDir), 0.0);
	float sptCutOffCos = cos(radians(spotlightCutoff*2.0));
	
	// Check if the vertex is outside the cone of illumination.
	if(dotFromSpot < sptCutOffCos)
	{
		return (0.0);
	}
	
	return (pow(dotFromSpot, spotlightExp));
}

// /////////////////////////////////////////////////////////////////
// Get the ambient term.
// A term from the ADS vertex color formula defined on pg. 244 in
// Chapter 5: Lighting of the OpenGL Programming Guide, 7th edition.
//
// @param lightAmbientColor The color of ambient light from a light
//							source.
//
// @return vec3 The combined material and lights ambient term.
//
// /////////////////////////////////////////////////////////////////
vec3 GetAmbientTerm(vec4 lightAmbientColor)
{
	return(lightAmbientColor.rgb * u_materialA.rgb);
}

// /////////////////////////////////////////////////////////////////
// Get the diffuse term.
// A term from the ADS vertex color formula defined on pg. 244 in
// Chapter 5: Lighting of the OpenGL Programming Guide, 7th edition.
//
// @param lightDiffuseColor The color of diffuse light from a light
//							source.
// @param dirToLight Unit vector that points to the light from the
//						vertex.
// @param vertexCameraNormal Vertex normal in camera space.
//
// @return vec3 The combined material and lights diffuse term scaled by
//			the vertices position relative to the light.
//
// /////////////////////////////////////////////////////////////////
vec3 GetDiffuseTerm(vec4 lightDiffuseColor, vec3 dirToLight, vec3 vertexCameraNormal)
{
    return (vec3(0.0));
	//float dotProduct = min(dot(vertexCameraNormal, dirToLight), 0.0);
    float dotProduct = dot(vertexCameraNormal, dirToLight);
	return (dotProduct * lightDiffuseColor.rgb * u_materialD.rgb);
}

// /////////////////////////////////////////////////////////////////
// Get the specular term.
// A term from the ADS vertex color formula defined on pg. 244 in
// Chapter 5: Lighting of the OpenGL Programming Guide, 7th edition.
//
// @param lightSpecularColor The color of specular light from a light
//							source.
// @param dirFromLight Unit vector that points from the light to the
//						vertex.
// @param vertexCameraNormal Vertex normal in camera space.
//
// @return vec3 The combined material and lights specular term scaled by the
//			vertices position relative to the light.
//
// /////////////////////////////////////////////////////////////////
vec3 GetSpecularTermGlBlue(vec4 lightSpecularColor, vec3 dirFromLight, vec3 vertexCameraNormal)
{
	vec3 reflectedLightVec = reflect(dirFromLight, vertexCameraNormal);
	float dotProduct = max(dot(vertexCameraNormal, reflectedLightVec), 0.0);
//    if(dotProduct == 0.0)
//    {
//        return (vec3(0.0, 0.0, 0.0));
//    }
	if(FloatCmp(dotProduct, 0.0, 0.000001))
	{
		return (vec3(0.0));
	}
	float spec = pow(dotProduct, u_materialExp);
	return (spec * lightSpecularColor.rgb * u_materialS.rgb);
}

// /////////////////////////////////////////////////////////////////
// Get the specular term.
// A term from the ADS vertex color formula defined on pg. 244 in
// Chapter 5: Lighting of the OpenGL Programming Guide, 7th edition.
//
// @param lightSpecularColor The color of specular light from a light
//							source.
// @param dirToLight Unit vector that points to the light from the
//						vertex.
// @param cameraPosition The position of the camera in eye space.
// @param vertexCameraPos The position of the vertex in camera
//							space.
//
// @return vec3 The combined material and lights specular term.
//
// /////////////////////////////////////////////////////////////////
vec3 GetSpecularTermGlRed(vec4 lightSpecularColor, vec3 dirToLight, vec3 cameraPosition, vec3 vertexCameraPos)
{
	if(FloatCmp(dot(dirToLight, vertexCameraPos), 0.0, 0.000001))
	{
		return (vec3(0.0));
	}

	vec3 s = normalize(dirToLight + normalize(cameraPosition - vertexCameraPos));
	float dotProduct = max(dot(s, vertexCameraPos), 0.0);
//	if(FloatCmp(dotProduct, 0.0, 0.000001))
//	{
//		return (vec3(0.0));
//	}
	
	float spec = pow(dotProduct, u_materialExp);
	
	return (spec * lightSpecularColor.rgb * u_materialS.rgb);
}

// /////////////////////////////////////////////////////////////////
// Main entry point of vertex shader.
//
// @param N/A
//
// @return void.
//
// /////////////////////////////////////////////////////////////////
void main(void) 
{
	// Calculate the number of lights defined (ensure we only process the first MAX_NUMBER_LIGHTS in the arrays).
    int numberLights = u_numberLights;
    if(numberLights > MAX_NUMBER_LIGHTS)
    {
		numberLights = MAX_NUMBER_LIGHTS;
    }

	vec4 primaryColor;							// Color of fragment (Material emission, Global Ambient Light + Diffuse 
												//  and Ambient components of each light in the scene as well as the texture color.)
	vec3 secondaryColor = vec3(0.0);						// Specular highlight from each light in the scene.
	float attTermArr[MAX_NUMBER_LIGHTS];		// Attenuation factor array (calculated once during primary loop and used again in secondary loop).
	float spotTermArr[MAX_NUMBER_LIGHTS];		// Spotlight factor array (calculated once during primary loop and used again in secondary loop).

	// Apply first two light independant terms.
    primaryColor = vec4(u_materialE.xyz + GetScaledGlobalAmbientLight(), 1.0);

    // For each light in the scene, Apply the ambient and diffuse terms scaled by the attenuation and spotlight factors.
    for(int i = 0; i < numberLights; ++i)
    {
		attTermArr[i] = GetAttenuationFactor(u_lightTypesArr[i], u_lightPositionArr[i], vp_flatVertexPosition, u_cAttArr[i], u_lAttArr[i], u_qAttArr[i]);
		spotTermArr[i] = GetSpotlightEffect(u_lightTypesArr[i], normalize(-vp_varyingLightDirArr[i]), u_spotlightDirection[i], u_spotlightCutoffArr[i], u_spotlightExpArr[i]);
		vec3 ambientTerm = GetAmbientTerm(u_lightAmbientArr[i]);
		vec3 diffuseTerm = GetDiffuseTerm(u_lightDiffuseArr[i], normalize(vp_varyingLightDirArr[i]), normalize(vp_varyingNormalVec));
		
		primaryColor.xyz += (attTermArr[i] * spotTermArr[i]) * vec3(ambientTerm + diffuseTerm);
	}

	// If we are applying a texture.
	if(u_applyTexture)
	{
		primaryColor *= texture(u_texture2dMap, vp_texCoordsVec);
	}


	// For each light in the scene, Apply the specular terms scaled by the attenuation and spotlight factors.
    for(int j = 0; j < numberLights; ++j)
    {
		vec3 specularTerm = GetSpecularTermGlBlue(u_lightSpecularArr[j], normalize(-vp_varyingLightDirArr[j]), normalize(vp_varyingNormalVec));
        //vec3 specularTerm = GetSpecularTermGlRed(u_lightSpecularArr[j], normalize(vp_varyingLightDirArr[j]), vec3(u_cameraPos.xyz), vp_flatVertexPosition);
		secondaryColor += (attTermArr[j] * spotTermArr[j]) * specularTerm;
	}

	// Final fragment color is the primary color (which is combined with the tex color) + the secondary color.
	vec3 adsColor = primaryColor.xyz + secondaryColor.xyz;

    
    if(u_fogType != FOG_OFF)
    {
        float dist = abs(vp_varyingVertexPosition.z);
        //float dist = length(vp_varyingVertexPosition.xyz);
        
        float fogFactor = 0.0f;
        if(u_fogType == FOG_LINEAR)
        {
            fogFactor = (u_fogMax - dist) / (u_fogMax - u_fogMin);
        }
        else if(u_fogType == FOG_EXPONENTIAL)
        {
            fogFactor = exp(-u_fogDensity * dist);
        }
        else if(u_fogType == FOG_EXPONENTIAL_BY_TWO)
        {
            fogFactor = exp(-pow(u_fogDensity * dist, 2));
        }
        
        fogFactor = clamp(fogFactor, 0.0, 1.0);
        adsColor = mix(u_fogColor, adsColor, fogFactor);
    }
    
    fp_colorVec = vec4(adsColor, u_materialD.a);
        
    
    //if(!gl_FrontFacing)
    //{
    //    fp_colorVec *= vec4(0.25, 0.25, 0.25, 1.0);
    //}
}

