#include <MSCE/MSCEWindow.h>
#include <MSCE/BuiltIns/transformComponent.hpp>
#include <MSCE/prototype.h>

#include <MSCE/Types/color.h>
#include <MSCE/event.h>
#include <MSCE/Events/graphicsEvents.h>
#include <MSCE/entity.h>
#include <MSCE/BuiltIns/Renderers/baseRendererComponent.hpp>
#include <iostream>
#include <memory>
using namespace std;
using namespace msce;

namespace
{
constexpr const uint32_t MAX_ENTITY_VERTEX_COUNT = MSCE_CIRCLE_VERTICES_COUNT;
}
std::shared_ptr<Shader> &
msce::MSCEWindow::get_shader_ref(const std::string &shader_id)
{
  return this->context_.shaders[shader_id];
}

GLFWwindow *msce::MSCEWindow::get_glfw_window() const noexcept
{
  return this->p_window_.get();
}

void msce::MSCEWindow::prepare_shaders()
{

  if (!PrototypeManager::instance)
    EventManager::instance->subscribe<PrototypeLoadingFinishedEvent>(
        [this](auto &ev) { this->prepare_shaders(); });

  else
  {
    int suc = 0, fail = 0;
    this->use_context();

    auto shader_prototypes =
        PrototypeManager::instance->enumerate_prototypes<ShaderPrototype>();
    if (shader_prototypes.size() <= 0)
    {
      logger_.log_warning("There are no {} ready at time of window creation.",
                          typeof(ShaderPrototype).get_name());
      return;
    }

    logger_.log_debug("Preparing shaders...");
    for (const auto *sp : shader_prototypes)
    {
      if (!sp) continue;

      try
      {
        auto shrd = std::make_shared<Shader>(*sp);

        if (shrd->is_ready()) suc++;
        else
          fail++;

        this->context_.shaders.insert_or_assign(sp->id, std::move(shrd));
        logger_.log_debug("Loaded '{}'", sp->id);
      }
      catch (std::exception &e)
      {
        logger_.log_error(
            "Failed to load '{}': {}: {}", sp->id,
            Platform::demangle(
                current_exception().__cxa_exception_type()->name()),
            e.what());
        fail++;
      }
    }
    logger_.log_debug("Finished compiling shaders. total: {} | successes: {} "
                      "| failures: {}",
                      suc + fail, suc, fail);
  }
}

void msce::MSCEWindow::reload_shaders()
{
  context_.shaders.clear();
  glClear(GL_COLOR_BUFFER_BIT);

  prepare_shaders();
}

void msce::MSCEWindow::use_context()
{
  glfwMakeContextCurrent(p_window_.get());
}

msce::MSCEWindow::MSCEWindow(vec2ui window_size, const char *title,
                             GLFWmonitor *glfw_monitor, GLFWwindow *glfw_share)
    : p_window_(glfwCreateWindow(window_size.x, window_size.y, title,
                                 glfw_monitor, glfw_share),
                &glfwDestroyWindow),
      size_(window_size), title_(title),
      logger_(std::format("Window('{}')", title))
{
  if (!p_window_)
  {
    logger_.log_error("Failed to create window '{}'.", title_);
    throw std::runtime_error("Failed to create glfw window.");
  }

  this->use_context();

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    logger_.log_error("Failed to load glad GL-loader for this window.");
    throw std::runtime_error("Failed to load GL-loader.");
  }

  logger_.log_info("GL version: {}",
                   reinterpret_cast<const char *>(glGetString(GL_VERSION)));

  glGenBuffers(1, &this->context_.vbo);
  glGenBuffers(1, &this->context_.ebo);
  glGenVertexArrays(1, &this->context_.vao);

  glViewport(0, 0, this->size().x, this->size().y);
  glClearColor(0.5, .5, .5, 1);
  prepare_shaders();
}

vec2ui msce::MSCEWindow::size() { return size_; }

bool msce::MSCEWindow::bind_image(const Image &image)
{
  if (image_bound(image)) return true;
  if (image.format() == ImageFormat::INVALID) return false;
  use_context();
  GLuint handle = 0;
  glGenTextures(1, &handle);
  if (handle == 0)
  {
    logger_.log_error("Failed to bind an image. GL error: {}", glGetError());
    return false;
  }
  glBindTexture(GL_TEXTURE_2D, handle);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  glTexImage2D(GL_TEXTURE_2D, 0, (GLenum)image.format(), image.resolution.x,
               image.resolution.y, 0, (GLenum)image.format(), GL_UNSIGNED_BYTE,
               image.pixel_data.get());

  this->context_.textures[&image] = handle;
  return true;
}

void msce::MSCEWindow::unbind_image(const Image &image)
{

  if (!image_bound(image)) return;
  use_context();

  GLint curently_bount_handle = 0;
  glGetIntegerv(GL_TEXTURE_BINDING_2D, &curently_bount_handle);
  if (curently_bount_handle == this->context_.textures.at(&image))
    glBindTexture(GL_TEXTURE_2D, 0);

  glDeleteTextures(1, &this->context_.textures.at(&image));
  this->context_.textures.erase(&image);
}

bool msce::MSCEWindow::image_bound(const Image &image) const
{
  return this->context_.textures.contains(&image);
}

GLuint msce::MSCEWindow::get_texture(const Image &image) const
{
  return this->context_.textures.at(&image);
}

bool msce::MSCEWindow::try_get_texture(const Image &image,
                                       GLuint &out_handle) const noexcept
{
  if (image_bound(image))
  {
    out_handle = this->context_.textures.at(&image);
    return true;
  }
  else
    return false;
}

void msce::MSCEWindow::size(vec2ui s) { size_ = s; }

bool msce::MSCEWindow::should_close()
{
  return glfwWindowShouldClose(this->p_window_.get());
}

void msce::MSCEWindow::render()
{
  this->use_context();

  glfwPollEvents();
  RenderStartEvent ev(*this);
  EventManager::instance->fire(ev);

  auto render_ent = [this](EntityHandle &ent)
  {
    static float verts[MAX_ENTITY_VERTEX_COUNT * 5];

    if (!ent) return;

    auto transform = ent->get_component<TransformComponent>();
    auto renderer = ent->get_component_based_on<BaseRendererComponent>();

    if (!renderer) return;
    if (!transform) return;
    if (renderer->shader_id().empty()) return;

    if (!context_.shaders.contains(renderer->shader_id())) return;

    auto &shader = *context_.shaders.at(renderer->shader_id()).get();
    if (!shader.is_ready()) return;
    auto prog = shader.get_handle();

    const IMesh &mesh = transform->shape->get_mesh_ref();
    auto verts_count = std::min(mesh.vertex_count(), MAX_ENTITY_VERTEX_COUNT);
    for (uint32_t i = 0; i < verts_count; i++)
    {
      verts[i * 5] = mesh.get_vertecies()[i].x;
      verts[i * 5 + 1] = mesh.get_vertecies()[i].y;
      verts[i * 5 + 2] = 0.0f;
      verts[i * 5 + 3] = mesh.get_uv()[i].x;
      verts[i * 5 + 4] = mesh.get_uv()[i].y;
    }

    glBufferData(GL_ARRAY_BUFFER, verts_count * 5 * sizeof(float), verts,
                 GL_STREAM_DRAW);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.index_count() * sizeof(uint32_t),
                 mesh.get_indecies(), GL_STREAM_DRAW);

    glBindVertexArray(this->context_.vao);

    glUseProgram(prog);

    if (shader.unitorm_locations.contains(SHADER_TRANSFORM_POS_UNIFORM_NAME))
      glUniform2f(
          shader.unitorm_locations.at(SHADER_TRANSFORM_POS_UNIFORM_NAME),
          transform->position.x, transform->position.y);

    if (shader.unitorm_locations.contains(SHADER_TRANSFORM_ROT_UNIFORM_NAME))
      glUniform2f(
          shader.unitorm_locations.at(SHADER_TRANSFORM_ROT_UNIFORM_NAME),
          transform->rotation.x, transform->rotation.y);

    if (shader.unitorm_locations.contains(SHADER_TRANSFORM_SCL_UNIFORM_NAME))
      glUniform2f(
          shader.unitorm_locations.at(SHADER_TRANSFORM_SCL_UNIFORM_NAME),
          transform->scale.x, transform->scale.y);

    if (shader.unitorm_locations.contains(SHADER_TRANSFORM_SIZ_UNIFORM_NAME))
      glUniform2f(
          shader.unitorm_locations.at(SHADER_TRANSFORM_SIZ_UNIFORM_NAME),
          transform->shape->get_w(), transform->shape->get_h());

    if (shader.unitorm_locations.contains(SHADER_WIN_RESOLUTION_UNIFORM_NAME))
      glUniform2f(
          shader.unitorm_locations.at(SHADER_WIN_RESOLUTION_UNIFORM_NAME),
          this->size().x, this->size().y);

    msce::ObjectBeingRenderedEvent ev(*this, *transform.get(), *ent.get());
    EventManager::instance->fire(ev);

    glDrawElements(GL_TRIANGLES, mesh.index_count(), GL_UNSIGNED_INT, 0);
  };

  glClear(GL_COLOR_BUFFER_BIT);

  glBindVertexArray(this->context_.vao);
  glBindBuffer(GL_ARRAY_BUFFER, this->context_.vbo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->context_.ebo);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  EntityManager::instance->do_for_each_entity(render_ent);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void msce::MSCEWindow::draw()
{
  this->use_context();
  glfwSwapBuffers(this->p_window_.get());
}
