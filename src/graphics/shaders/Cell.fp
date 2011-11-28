// /////////////////////////////////////////////////////////////////
// @file Cell.fp
// @author Richard S. Wright Jr. and PJ O Halloran.
// @date 22/09/2010
//
// Renders a primitive using a cell or toon shading style using a 1D
// texture as a lookup table to fill a primitive with color.
//
// /////////////////////////////////////////////////////////////////

#version 130

uniform sampler1D colorTable;
out vec4 fragmentColor;

smooth in float textureCoordinate;


void main(void)
{ 
   fragmentColor = texture(colorTable, textureCoordinate);
}