// /////////////////////////////////////////////////////////////////
// @file identity.fp
// @author Richard S. Wright Jr. and PJ O Halloran.
// @date 22/09/2010
//
// Sets all fragments to be the same color.
//
// /////////////////////////////////////////////////////////////////

#version 130

out vec4 fragmentColorVec;

uniform vec4 colorVec;

void main(void)
{ 
   fragmentColorVec = colorVec;
}