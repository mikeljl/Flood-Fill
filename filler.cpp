/*
*  File:        filler.cpp
*  Description: Implementation of functions in the filler namespace.
*
*/

/*
*  Performs a flood fill using breadth first search.
*
*  PARAM:  config - FillerConfig struct to setup the fill
*  RETURN: animation object illustrating progression of flood fill algorithm
*/
animation filler::FillBFS(FillerConfig& config) {
  // complete your implementation below
  // You should replace the following line with a
  // correct call to fill.
  return Fill<Queue>(config);
  
}

/*
*  Performs a flood fill using depth first search.
*
*  PARAM:  config - FillerConfig struct to setup the fill
*  RETURN: animation object illustrating progression of flood fill algorithm
*/
animation filler::FillDFS(FillerConfig& config) {
  // complete your implementation below
  // You should replace the following line with a
  // correct call to fill.
  return Fill<Stack>(config);
  
}

/*
*  Run a flood fill on an image starting at the seed point
*
*  PARAM:  config - FillerConfig struct with data for flood fill of image
*  RETURN: animation object illustrating progression of flood fill algorithm
*/
template <template <class T> class OrderingStructure> animation filler::Fill(FillerConfig& config)
{
  /*
  * You need to implement this function!
  *
  * This is the basic description of a flood-fill algorithm: Every fill
  * algorithm requires an ordering structure, which is passed to this
  * function via its template parameter. For a breadth-first-search
  * fill, that structure is a Queue, for a depth-first-search, that
  * structure is a Stack. To begin the algorithm, you simply place the
  * given point in the ordering structure, marking it processed
  * (the way you mark it is a design decision you'll make yourself).
  * We have a choice to either change the color, if appropriate, when we
  * add the point to the OS, or when we take it off. In our test cases,
  * we have assumed that you will change the color when a point is added
  * to the structure.
  * Until the structure is empty, you do the following:
  *
  * 1.     Remove a point from the ordering structure, and then...
  *
  *        1.    add its unprocessed neighbors (up/down/left/right) whose color values are
  *              within (or equal to) tolerance distance from the seed point,
  *              to the ordering structure, and
  *              mark them as processed.
  *        2.    if it is an appropriate frame, send the current PNG to the
  *              animation (as described below).
  *
  * 2.     When implementing your breadth-first-search and
  *        depth-first-search fills, you will need to explore neighboring
  *        pixels (up/down/left/right) in some order.
  *
  *        While the order in which you examine neighbors does not matter
  *        for a proper fill, you must use the same order as we do for
  *        your animations to come out like ours! The order you should put
  *        neighboring pixels **ONTO** the queue or stack is based on the
  *        following priority condition:
  *        ** MINIMUM COLOR DISTANCE FROM THE CURRENT PIXEL **
  *        Ties are broken first by minimum y-coordinate, then by minimum x-coordinate.
  *        The HSLAPixel.dist() function will be useful, and you should
  *        take advantage of the functionality in your PriorityNeighbours class.
  * 
  *        If you process the neighbours in a different order, your fill may
  *        still work correctly, but your animations will be different
  *        from the grading scripts!
  *
  * 3.     For every k pixels filled, **starting at the kth pixel**, you
  *        must add a frame to the animation, where k = frameFreq.
  *
  *        For example, if frameFreq is 4, then after the 4th pixel has
  *        been filled you should add a frame to the animation, then again
  *        after the 8th pixel, etc.  You must only add frames for the
  *        number of pixels that have been filled, not the number that
  *        have been checked. So if frameFreq is set to 1, a pixel should
  *        be filled every frame.
  *
  * 4.     Finally, as you leave the function, send one last frame to the
  *        animation. This frame will be the final result of the fill, and
  *        it will be the one we test against.
  *
  */

  int framecount = 0; // increment after processing one pixel; used for producing animation frames (step 3 above)
  animation anim;
  OrderingStructure<PixelPoint> os;
  config.neighbourorder = PriorityNeighbours();

  // complete your implementation below
  // HINT: you will likely want to declare some kind of structure to track
  //       which pixels have already been visited

  vector<vector<bool>> tracker;
  
  for (unsigned int x = 0; x < config.img.width(); x++) {
    vector<bool> col;
    for (unsigned int y= 0; y < config.img.height(); y++) {
      col.push_back(false);
    }
      tracker.push_back(col);
  }


  os.Add(config.seedpoint);

  HSLAPixel newPixel = config.picker->operator()(config.seedpoint);
  *(config.img.getPixel(config.seedpoint.x, config.seedpoint.y)) = newPixel;
  tracker[config.seedpoint.x][config.seedpoint.y] = true;
  framecount++;
  
  if (framecount == config.frameFreq) {
    anim.addFrame(config.img);
    framecount = 0;
    }
  
  while(!os.IsEmpty()) {

    PixelPoint removed = os.Remove();
    config.neighbourorder.SetReferenceColor(removed.color);


    if (removed.y+1 < config.img.height() && removed.x < config.img.width() && removed.x>=0 && removed.y>=0) {
    PixelPoint up(removed.x, removed.y+1, *(config.img.getPixel(removed.x, removed.y+1)));
    config.neighbourorder.Insert(up);
    }

    if (removed.y !=0 && removed.x < config.img.width() && removed.x>=0 && removed.y<config.img.height()) {
    PixelPoint down(removed.x, removed.y-1, *(config.img.getPixel(removed.x, removed.y-1)));
    config.neighbourorder.Insert(down);
    }


    if (removed.x !=0 && removed.y < config.img.height() && removed.x-1 < config.img.width() && removed.y>=0) {
    PixelPoint left(removed.x-1, removed.y, *(config.img.getPixel(removed.x-1, removed.y)));
    config.neighbourorder.Insert(left);
    }

    if (removed.x+1 < config.img.width() && removed.y < config.img.height() && removed.x+1>=0 && removed.y>=0) {
    PixelPoint right(removed.x+1, removed.y, *(config.img.getPixel(removed.x+1, removed.y)));
    config.neighbourorder.Insert(right);
    }


    while(!config.neighbourorder.IsEmpty()) {
      PixelPoint tpp = config.neighbourorder.Remove();
      if (tpp.x>=0 && tpp.y>=0 && tpp.x<config.img.width() && tpp.y < config.img.height() && config.seedpoint.color.dist(tpp.color) <= config.tolerance && !tracker[tpp.x][tpp.y]) {
        os.Add(tpp);
        tracker[tpp.x][tpp.y] = true;
        framecount++;
        HSLAPixel pickedPixel = config.picker->operator()(tpp);
        *(config.img.getPixel(tpp.x, tpp.y)) = pickedPixel;

        if (framecount == config.frameFreq) {
          anim.addFrame(config.img);
          framecount = 0;
          }
      }
    }
  }
   anim.addFrame(config.img);
  

  return anim;
  }
