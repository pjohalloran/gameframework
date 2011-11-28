// /////////////////////////////////////////////////////////////////
// @file goraud.fp
// @author Richard S. Wright Jr. and PJ O Halloran.
// @date 22/09/2010
//
// Applies the Goraud shading model to each fragment based on input
// from the vertex shader.
//
// The vertex color is interpolated across the surface of the 
// primitive.
//
// /////////////////////////////////////////////////////////////////

#version 130

out vec4 fragmentColorVec;
smooth in vec4 varyingColorVec;

void main(void)
{ 
   fragmentColorVec = varyingColorVec;
}
