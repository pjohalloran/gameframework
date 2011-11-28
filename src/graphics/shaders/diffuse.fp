// /////////////////////////////////////////////////////////////////
// @file diffuse.fp
// @author Richard S. Wright Jr. and PJ O Halloran.
// @date 21/09/2010
//
// Difuse light fragment shader.  Applies the color from the vertex
// shader stage smoothly interpolated depending on the fragment we
// are rendering.
//
// /////////////////////////////////////////////////////////////////

#version 130

out vec4 fragmentColorVec;
smooth in vec4 varyingColorVec;

void main(void)
{ 
   fragmentColorVec = varyingColorVec;
}
