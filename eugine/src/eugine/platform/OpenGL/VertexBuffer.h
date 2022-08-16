#ifndef EUGINE_PLATFORM_OPENGL_VERTEXBUFFER_H
#define EUGINE_PLATFORM_OPENGL_VERTEXBUFFER_H
#include "eugine/rendering/VertexBuffer.h"
#include "eugine/rendering/VertexBufferLayout.h"
#include "VertexArray.h"
//#include <eugine/platform/OpenGL/wrapper/VertexBufferLayout.h>

namespace eg::rendering::GLWrapper {

class EG_API VertexBuffer : public ::eg::rendering::VertexBuffer {
public:
  VertexBuffer(const void *data, u32 size, rendering::VertexBuffer::UsageHints usageHint,
               rendering::VertexBufferLayout layout);
  virtual ~VertexBuffer();

  void bind();
  void unBind();

  void setData(void *data, u32 size) override;

  void setLayout(const rendering::VertexBufferLayout &layout) override {
    m_layout = layout;
  }
  const rendering::VertexBufferLayout& getLayout() const override { return m_layout; }

private:
    friend class VertexArray;
  unsigned int m_ID = 0;
  rendering::VertexBufferLayout m_layout;

  VertexArray m_vertexArray{};
};

} // namespace eg::rendering::OpenGLWrapper

#endif
