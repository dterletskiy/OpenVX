#pragma once

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <limits>

#include <VX/vx.h>

#include "ImageSize.hpp"



bool load_pgm(
      const char* filename, vx_image image, ImageSize& image_size
   );

bool save_pgm(
      const char* filename, vx_image image, const ImageSize& image_size
   );
