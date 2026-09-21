#pragma once

#include <MSCE/Types/vector.h>
#include <GLFW/glfw3.h>
#include <MSCE/logger.h>
#include <cstdint>

#include <stb/stb_image.h>

namespace msce
{
enum class ImageFormat : GLenum
{
  INVALID = 0,
  RED_ONLY = GL_RED,
  RGB = GL_RGB,
  RGBA = GL_RGBA
};

/// @brief Data structure for holding image information. Please initialize with
/// @ref msce::ImageResource
struct Image
{
  std::unique_ptr<uint8_t[], decltype(&stbi_image_free)> pixel_data =
      std::unique_ptr<uint8_t[], decltype(&stbi_image_free)>(nullptr,
                                                             stbi_image_free);
  vec2ui resolution;
  uint8_t channels;
  ImageFormat format() const;
  Image(const std::string &source);

private:
  inline static Logger logger = Logger("Image");
};

} // namespace msce
