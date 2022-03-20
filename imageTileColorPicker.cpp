/*
*  File: imageTileColorPicker.cpp
*  Implements the image tile color picker for CPSC 221 PA2.
*
*/

#include "imageTileColorPicker.h"

ImageTileColorPicker::ImageTileColorPicker(PNG& otherimage) {
  // complete your implementation below
  img_other = otherimage;
  
}

HSLAPixel ImageTileColorPicker::operator()(PixelPoint p) {
  // complete your implementation below
  
  unsigned int x = img_other.width();
  unsigned int y = img_other.height();

  unsigned int rx = p.x % x;
  unsigned int ry = p.y % y;

  HSLAPixel * result = img_other.getPixel(rx, ry);

  return *result;
}