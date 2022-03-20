/*
*  File: negativeColorPicker.cpp
*  Implements the negative color picker for CPSC 221 PA2.
*
*/

#include "negativeColorPicker.h"

NegativeColorPicker::NegativeColorPicker(PNG& inputimg)
{
  // complete your implementation below
  img = inputimg;
  
}

HSLAPixel NegativeColorPicker::operator()(PixelPoint p)
{
  // complete your implementation below
  HSLAPixel * result = img.getPixel(p.x, p.y);
  result->h = result->h + 180;
  while (result->h >= 360) {
    result->h = result->h - 360;
  }
  result->l = 1 - result->l;

  return *result;

  
}
