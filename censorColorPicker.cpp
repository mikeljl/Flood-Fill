/*
*  File: censorColorPicker.cpp
*  Implements the censor color picker for CPSC 221 PA2.
*
*/

#include "censorColorPicker.h"

#include <math.h> // gives access to sqrt function

/*
*  Useful function for computing the Euclidean distance between two PixelPoints
*/
double PointDistance(PixelPoint a, PixelPoint b) {
  unsigned int diff_x, diff_y;
  if (a.x > b.x)
    diff_x = a.x - b.x;
  else
    diff_x = b.x - a.x;
  if (a.y > b.y)
    diff_y = a.y - b.y;
  else
    diff_y = b.y - a.y;
  return sqrt(diff_x * diff_x + diff_y * diff_y);
}



CensorColorPicker::CensorColorPicker(unsigned int b_width, PixelPoint ctr, unsigned int rad, PNG& inputimage)
{

  blockwidth = b_width;
  center = ctr;
  radius = rad;
  img = inputimage;

  // set up blockified image

  double total_h = 0;
  double total_s = 0;
  double total_l = 0;
  double total_a = 0;

  int xcoor = 0;
  int ycoor = 0;

  double numPixel = 0;

  PNG bpng(img.width(), img.height());



  int xcount = bpng.width() / blockwidth;
  int ycount = bpng.height() / blockwidth;


  if (bpng.width() % blockwidth != 0 && bpng.width() > blockwidth) {
    xcount++;
  }

  if (bpng.height() % blockwidth != 0 && bpng.height() > blockwidth) {
    ycount++;
  }

  for (int x = 0; x < xcount; x++) {
    for (int y = 0; y < ycount; y++) {


      int corner_x = x*blockwidth;
      int corner_y = y*blockwidth;
      
      if (corner_x < img.width() && corner_y < img.height()) {

      int cur_x = corner_x;
      int cur_y = corner_y;

      for (int i = 0; i < (int)blockwidth && corner_x + i < bpng.width(); i++) {
        for (int j = 0; j< (int)blockwidth && corner_y + j < bpng.height(); j++) {

          cur_x = corner_x + i;
          cur_y = corner_y + j;


          HSLAPixel * temp1 = img.getPixel(cur_x,cur_y);

          total_h += temp1->h;
          total_s += temp1->s;
          total_l += temp1->l;
          total_a += temp1->a;

          numPixel++;
          
        }
      }

      double avgh = total_h/ numPixel;
      double avgs = total_s/ numPixel;
      double avgl = total_l/ numPixel;
      double avga = total_a/ numPixel;

      numPixel = 0;
      total_h = 0;
      total_s = 0;
      total_l = 0;
      total_a = 0;

      cur_x = corner_x;
      cur_y = corner_y;

      for (int i = 0; i < (int)blockwidth && corner_x + i < bpng.width(); i++) {
        for (int j = 0; j< (int)blockwidth && corner_y + j < bpng.height(); j++) {
          cur_x = corner_x + i;
          cur_y = corner_y + j;


          HSLAPixel * temp = bpng.getPixel(cur_x,cur_y);

          temp->h = avgh;
          temp->s = avgs;
          temp->l = avgl;
          temp->a = avga;

          
        }
      }
    }
  
  }
}
blockyimg = bpng;
}

 




HSLAPixel CensorColorPicker::operator()(PixelPoint p)
{
  // complete your implementation below

  if (PointDistance(p, center) <= radius) {
    return *(blockyimg.getPixel(p.x, p.y));
  } else {
    return *(img.getPixel(p.x, p.y));
  }
}

