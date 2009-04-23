#include <cstring>
#include <iostream>
#include <mpi.h>
#include "image.h"

bool Image::Save(const std::string &filename) const {
  int len = filename.length();
  if (!(len > 4 && filename.substr(len-4) == std::string(".ppm"))) {
    std::cerr << "ERROR: This is not a PPM filename: " << filename << std::endl;
    return false;
  }
  FILE *file = fopen(filename.c_str(), "wb");
  if (file == NULL) {
    std::cerr << "Unable to open " << filename << " for writing\n";
    return false;
  }

  // misc header information
  fprintf (file, "P6\n");
  fprintf (file, "%d %d\n", width,height);
  fprintf (file, "255\n");

  // the data
  // flip y so that (0,0) is bottom left corner
  for (int y = height-1; y >= 0; y--) {
    for (int x=0; x<width; x++) {
      Color v = GetPixel(x,y);
      fputc ((unsigned char)(v.r),file);
      fputc ((unsigned char)(v.g),file);
      fputc ((unsigned char)(v.b),file);
    }
  }
  fclose(file);
  return true;
}

bool Image::Load(const std::string &filename) {
  int len = filename.length();
  if (!(len > 4 && filename.substr(len-4) == std::string(".ppm"))) {
    std::cerr << "ERROR: This is not a PPM filename: " << filename << std::endl;
    return false;
  }
  MPI::File fh = MPI::File::Open(MPI::COMM_WORLD, filename.c_str(),
		  MPI::MODE_RDONLY, MPI::INFO_NULL);

  // misc header information
  char tmp[2<<20];
  fh.Read(tmp, 2<<20, MPI::CHAR);
  char *newline1 = strstr(tmp, "\n");
  char *newline2 = strstr(newline1 + 1, "\n");
  char *newline3 = strstr(newline2 + 1, "\n");
  assert (strstr(tmp,"P6"));
  sscanf(newline1+1,"%d %d",&width,&height);
  assert (strstr(newline2+1,"255"));

  // the data
  delete [] data;
  data = new Color[height*width];
  newline3 += 1;
  // flip y so that (0,0) is bottom left corner
  for (int y = height-1; y >= 0; y--) {
    for (int x = 0; x < width; x++) {
      Color c;
      c.r = *newline3++;
      c.g = *newline3++;
      c.b = *newline3++;
      SetPixel(x,y,c);
    }
  }
  fh.Close();
  return true;
}

unsigned char* Image::getGLPixelData() {
  delete [] gl_data;
  gl_data = new unsigned char[width*height*3];
  for (int x=0; x<width; x++) {
    for (int y=0; y<height; y++) { 
      Color v = GetPixel(x,y);
      gl_data[y*width*3+x*3+0] = v.r;
      gl_data[y*width*3+x*3+1] = v.g;
      gl_data[y*width*3+x*3+2] = v.b;
    }
  }
  return gl_data;
}

// ====================================================================
// ====================================================================

