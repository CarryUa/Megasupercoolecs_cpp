#include "image.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#undef STB_IMAGE_IMPLEMENTATION

msce::ImageFormat msce::Image::format() const
{

  switch (this->channels)
  {
  case 1:
    return ImageFormat::RED_ONLY;
  case 3:
    return ImageFormat::RGB;
  case 4:
    return ImageFormat::RGBA;
  default:
    return ImageFormat::INVALID;
  }
}

msce::Image::Image(const std::string &source)
    : pixel_data(nullptr, stbi_image_free)
{
  int channels, w, h;

  uint8_t *pixels = stbi_load(source.c_str(), &w, &h, &channels, 0);
  if (!pixels)
  {
    logger.log_error("Failed to load image at '{}'. Bad file!", source);
    this->channels = 0;
    this->resolution = vec2ui(0, 0);
    this->pixel_data = nullptr;
  }
  else
  {
    this->channels = channels;
    this->resolution = vec2ui(w, h);
    this->pixel_data.reset(pixels);
  }
}
