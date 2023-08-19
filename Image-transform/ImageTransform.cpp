#include <iostream>
#include <cmath>
#include <cstdlib>

#include "uiuc/PNG.h"
#include "uiuc/HSLAPixel.h"
#include "ImageTransform.h"

/* ******************
(Begin multi-line comment...)

Write your name and email address in the comment space here:

Name:
Email:

(...end multi-line comment.)
******************** */

using uiuc::PNG;
using uiuc::HSLAPixel;

// Helper function header inserted here
bool is_orange(double h);
double luminance_decrease(double lu, int distance);

/**
 * Returns an image that has been transformed to grayscale.
 *
 * The saturation of every pixel is set to 0, removing any color.
 *
 * @return The grayscale image.
 */
PNG grayscale(PNG image) {
  /// This function is already written for you so you can see how to
  /// interact with our PNG class.
  for (unsigned x = 0; x < image.width(); x++) {
    for (unsigned y = 0; y < image.height(); y++) {
      HSLAPixel & pixel = image.getPixel(x, y);

      // `pixel` is a reference to the memory stored inside of the PNG `image`,
      // which means you're changing the image directly. No need to `set`
      // the pixel since you're directly changing the memory of the image.
      pixel.s = 0;
    }
  }

  return image;
}



/**
 * Returns an image with a spotlight centered at (`centerX`, `centerY`).
 *
 * A spotlight adjusts the luminance of a pixel based on the distance the pixel
 * is away from the center by decreasing the luminance by 0.5% per 1 pixel euclidean
 * distance away from the center.
 *
 * For example, a pixel 3 pixels above and 4 pixels to the right of the center
 * is a total of `sqrt((3 * 3) + (4 * 4)) = sqrt(25) = 5` pixels away and
 * its luminance is decreased by 2.5% (0.975x its original value).  At a
 * distance over 160 pixels away, the luminance will always decreased by 80%.
 * 
 * The modified PNG is then returned.
 *
 * @param image A PNG object which holds the image data to be modified.
 * @param centerX The center x coordinate of the crosshair which is to be drawn.
 * @param centerY The center y coordinate of the crosshair which is to be drawn.
 *
 * @return The image with a spotlight.
 */
PNG createSpotlight(PNG image, int centerX, int centerY) {
  // iterate over the whole image and get the distance and calculate h decreased
  for (unsigned int x = 0; x < image.width(); x++) {
    for (unsigned int y = 0; y < image.height(); y++) {
      HSLAPixel & pixel = image.getPixel(x, y);
      int x_d = x - centerX;
      int y_d = y - centerY;
      int sum = x_d * x_d + y_d * y_d;
      int distance = static_cast<int>(std::ceil(sqrt(sum)));
      pixel.l = luminance_decrease(pixel.l, distance);
    }
  }
  return image;
  
}
 
double luminance_decrease(double lu, int distance) {
  double decreaed_lu;
  if (distance < 0) {
    cout << "Wront distance." << endl;
  } else if (distance < 160) {
    decreaed_lu = distance * 0.005;
  } else {
    decreaed_lu = 0.8;
  }
  lu *= (1 - decreaed_lu);
  return lu;
}

/**
 * Returns a image transformed to Illini colors.
 *
 * The hue of every pixel is set to the a hue value of either orange or
 * blue, based on if the pixel's hue value is closer to orange than blue.
 *
 * @param image A PNG object which holds the image data to be modified.
 *
 * @return The illinify'd image.
**/
PNG illinify(PNG image) {
  for (unsigned int i = 0; i < image.width(); i++) {
    for (unsigned int j = 0; j < image.height(); j++) {
        HSLAPixel & pixel = image.getPixel(i, j);
        bool set_to_orange = is_orange(pixel.h);

        pixel.h = set_to_orange ? 11 : 216;
    }
  }

  return image;
}
 
bool is_orange(double h) {
  double half_f;
  double half_s;
  double dist;
  if ((h >= 0) and (h <= 11)) {
    return true;
  } else if (h < 216) {
    half_f = (216 - 11) / 2;
    // cout << "Half_first is: " << half_f << endl;
    dist = h - 11;
    return dist < half_f;
  } else {
    half_s = (11 + 360 - 216) / 2;
    dist = h - 216;
    return dist > half_s;
  }
}

/**
* Returns an immge that has been watermarked by another image.
*
* The luminance of every pixel of the second image is checked, if that
* pixel's luminance is 1 (100%), then the pixel at the same location on
* the first image has its luminance increased by 0.2.
*
* @param firstImage  The first of the two PNGs, which is the base image.
* @param secondImage The second of the two PNGs, which acts as the stencil.
*
* @return The watermarked image.
*/
PNG watermark(PNG firstImage, PNG secondImage) {
  for (unsigned int x = 0; x < secondImage.width(); x++) {
    for (unsigned int y = 0; y < secondImage.height(); y++) {
      HSLAPixel & second_pixel = secondImage.getPixel(x, y);
      HSLAPixel & first_pixel = firstImage.getPixel(x, y);
      if (second_pixel.l >= 1) {
        double change_lu = first_pixel.l + 0.2;
        first_pixel.l = (change_lu >= 0) ? change_lu : 0.0;
        // cout << "first pixel luminance after changed " << first_pixel.l <<endl;
      }
      
    }
  }

  return firstImage;
}
