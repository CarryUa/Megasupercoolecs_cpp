#pragma once

#include <MSCE/Types/vector.h>
#include <GLFW/glfw3.h>
#include <MSCE/logger.h>
#include <cstdint>
#include <cstdio>

namespace msce
{
/**
 * @brief Image format from which the image was last loaded.
 */
enum class ImageFormat : uint8_t
{
  UNSUPPORTED = 0,
  PNG = 1,
  JPEG = 2,
};

/**
 * @brief Wrapper around GL channel types(rgb, rgba, red_only, etc).
 */
enum class ImageChannels : GLenum
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
private:
  /**
   * @brief Utility class used to determain @ref msce::ImageFormat, and fully
   * load @ref msce::Image.
   */
  class Codec
  {
  private:
    static std::unique_ptr<uint8_t[]>
    load_png(const uint8_t *source, size_t source_size, vec2ui &o_res,
             uint8_t &o_chann_count, ImageFormat &o_format);

    static std::unique_ptr<uint8_t[]>
    load_jpg(const uint8_t *source, size_t source_size, vec2ui &o_res,
             uint8_t &o_chann_count, ImageFormat &o_format);

  public:
    static std::unique_ptr<uint8_t[]> load_image(const std::string &path,
                                                 vec2ui &o_res,
                                                 uint8_t &o_chann_count,
                                                 ImageFormat &o_format);
  };

  inline static Logger logger = Logger("Image");

public:
  /**
   * @brief Tightly packed pixel color values.
   */
  std::unique_ptr<uint8_t[]> pixel_data;
  vec2ui resolution;
  uint8_t channel_count;
  ImageFormat format;
  /**
   * @brief Getter for this image @ref msce::ImageChannels.
   */
  ImageChannels channels() const;
  Image(const std::string &source);

  /**
   * @brief Loads or reloads an image to one at specified source.
   * @note This method will free both previous pixel_data in RAM and on GPU and
   * replace them with new image's data.
   * @note If file does not exist, no changes will be applied.
   */
  void load(const std::string &source);
};

} // namespace msce
