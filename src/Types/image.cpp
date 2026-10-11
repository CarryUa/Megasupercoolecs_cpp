#include <MSCE/Types/image.h>

#include <MSCE/event.h>
#include <MSCE/Events/graphicsEvents.h>

#include <filesystem>

extern "C"
{
#include <libpng/png.h>
#include <jpeglib.h>
}

namespace
{
static constexpr const uint8_t MAX_IMAGE_MAGIC_BYTES_LEN = 8;

static void jpeg_dont_exit(j_common_ptr cinfo) {}
struct jpeg_ignore_error_mgr
{
  struct jpeg_error_mgr pub;
  jmp_buf setjmp_buffer;
};

} // namespace

msce::ImageChannels msce::Image::channels() const
{
  switch (this->channel_count)
  {
  case 1:
    return ImageChannels::RED_ONLY;
  case 3:
    return ImageChannels::RGB;
  case 4:
    return ImageChannels::RGBA;
  default:
    return ImageChannels::INVALID;
  }
}

msce::Image::Image(const std::string &source) { load(source); }

msce::Image::~Image()
{
  ImageNeedsUnbindingEvent ev(*this);
  EventManager::instance->fire(ev);
}

void msce::Image::load(const std::string &source)
{
  if (!std::filesystem::exists(source))
  {
    logger.log_error("Tried to load image at '{}', but specified file does not "
                     "exist. Check spelling :)",
                     source);

    return;
  }

  if (pixel_data_ != nullptr)
  {
    pixel_data_.reset(nullptr);
    resolution = vec2ui(0, 0);
    channel_count = 0;
    format = ImageFormat::UNSUPPORTED;
  };

  this->pixel_data_ = Codec::load_image(source, this->resolution,
                                        this->channel_count, this->format);
  if (!pixel_data_)
  {
    logger.log_error("Failed to load image at '{}'.", source);
    this->pixel_data_ = nullptr;
  }

  ImageNeedsBindingEvent ev(*this);
  EventManager::instance->fire(ev);
  pixel_data_.reset();
}
#pragma region Codec

std::unique_ptr<uint8_t[]> msce::Image::Codec::load_png(const uint8_t *source,
                                                        size_t source_size,
                                                        vec2ui &o_res,
                                                        uint8_t &o_chann_count,
                                                        ImageFormat &o_format)
{
  o_format = ImageFormat::PNG;

  png_image img;
  std::memset(&img, 0, sizeof(img));
  img.version = PNG_IMAGE_VERSION;

  if (!png_image_begin_read_from_memory(&img, source, source_size))
    return nullptr;

  img.format = PNG_FORMAT_RGBA;
  o_res = vec2ui(img.width, img.height);
  o_chann_count = 4;
  auto pixels = std::make_unique<uint8_t[]>(PNG_IMAGE_SIZE(img));

  if (!png_image_finish_read(&img, nullptr, pixels.get(), 0, nullptr))
  {
    png_image_free(&img);
    return nullptr;
  }
  return pixels;
}

std::unique_ptr<uint8_t[]> msce::Image::Codec::load_jpg(const uint8_t *source,
                                                        size_t source_size,
                                                        vec2ui &o_res,
                                                        uint8_t &o_chann_count,
                                                        ImageFormat &o_format)
{
  if (!source) return nullptr;
  o_format = ImageFormat::JPEG;
  struct jpeg_decompress_struct jpeg;
  jpeg_ignore_error_mgr err;

  jpeg.err = jpeg_std_error(&err.pub);
  err.pub.error_exit = jpeg_dont_exit;
  err.pub.output_message = jpeg_dont_exit;

  jpeg_create_decompress(&jpeg);

  jpeg_mem_src(&jpeg, source, (unsigned long)source_size);
  jpeg_read_header(&jpeg, true);

  jpeg.out_color_space = JCS_RGB;

  jpeg_start_decompress(&jpeg);
  o_chann_count = jpeg.output_components;
  o_res = vec2ui(jpeg.output_width, jpeg.output_height);

  size_t row_stride = (size_t)o_res.x * o_chann_count;
  auto pixels = std::make_unique<uint8_t[]>(row_stride * o_res.y);
  if (!pixels)
  {
    jpeg_destroy_decompress(&jpeg);
    return nullptr;
  }

  while (jpeg.output_scanline < o_res.y)
  {
    uint8_t *row = pixels.get() + jpeg.output_scanline * row_stride;

    jpeg_read_scanlines(&jpeg, &row, 1);
  }

  jpeg_finish_decompress(&jpeg);
  jpeg_destroy_decompress(&jpeg);
  return pixels;
}

std::unique_ptr<uint8_t[]>
msce::Image::Codec::load_image(const std::string &path, vec2ui &o_res,
                               uint8_t &o_chann_count, ImageFormat &o_format)
{
  o_res = vec2ui(0, 0);
  o_chann_count = 0;
  o_format = ImageFormat::UNSUPPORTED;

  std::ifstream file(path, std::ios::in | std::ios::binary | std::ios::ate);
  if (!file) return nullptr;
  size_t size = file.tellg();
  std::unique_ptr<uint8_t[]> file_content(new uint8_t[size]);
  file.seekg(0);
  file.read(reinterpret_cast<char *>(file_content.get()), size);
  if (!file) return nullptr;

  if (png_sig_cmp(&file_content[0], 0, 8) == 0)
    return Codec::load_png(file_content.get(), size, o_res, o_chann_count,
                           o_format);

  if (size >= 3 && file_content[0] == 0xff && file_content[1] == 0xd8 &&
      file_content[2] == 0xff)
    return Codec::load_jpg(file_content.get(), size, o_res, o_chann_count,
                           o_format);

  logger.log_error("Unsuported image format for file: '{}'", path);
  return nullptr;
}
