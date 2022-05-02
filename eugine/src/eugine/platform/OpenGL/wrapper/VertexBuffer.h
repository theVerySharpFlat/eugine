#ifndef EUGINE_PLATFORM_OPENGL_VERTEXBUFFER_H
#define EUGINE_PLATFORM_OPENGL_VERTEXBUFFER_H
#include <eugine/rendering/VertexBuffer.h>
#include <eugine/platform/OpenGL/wrapper/VertexBufferLayout.h>

namespace eg {
namespace GLWrapper {

class EG_API VertexBuffer : public ::eg::VertexBuffer{
public:
  VertexBuffer(const void *data, u32 size, VertexBufferLayout  layout);
  virtual ~VertexBuffer();

  void bind() const override;
  void unBind() const override;

  void setData(void* data, u32 size) override;

  void setLayout(const VertexBufferLayout& layout) {
      m_layout = layout;
  }
  VertexBufferLayout getLayout() { return m_layout; }

private:
  unsigned int m_ID = 0;
  VertexBufferLayout m_layout;
};

} // namespace glwrapper
} // namespace eg

#endif
