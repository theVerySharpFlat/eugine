#ifndef EUGINE_PLATFORM_OPENGL_VERTEXBUFFER_H
#define EUGINE_PLATFORM_OPENGL_VERTEXBUFFER_H
#include <eugine/rendering/VertexBuffer.h>
#include <eugine/rendering/VertexBufferLayout.h>
//#include <eugine/platform/OpenGL/wrapper/VertexBufferLayout.h>

namespace eg {
namespace GLWrapper {

class EG_API VertexBuffer : public ::eg::rendering::VertexBuffer {
public:
  VertexBuffer(const void *data, u32 size,
               rendering::VertexBufferLayout layout);
  virtual ~VertexBuffer();

  void bind() const override;
  void unBind() const override;

  void setData(void *data, u32 size) override;

  void setLayout(const rendering::VertexBufferLayout &layout) override {
    m_layout = layout;
  }
  const rendering::VertexBufferLayout& getLayout() const override { return m_layout; }

private:
  unsigned int m_ID = 0;
  rendering::VertexBufferLayout m_layout;
};

} // namespace GLWrapper
} // namespace eg

#endif
