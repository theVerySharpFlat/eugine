#ifndef EUGINE_PLATFORM_OPENGL_VERTEXBUFFER_H
#define EUGINE_PLATFORM_OPENGL_VERTEXBUFFER_H

namespace eg {
namespace GLWrapper {

class EG_API VertexBuffer {
public:
  VertexBuffer(const void *data, u32 size);
  virtual ~VertexBuffer();

  void bind() const;
  void unBind() const;

private:
  unsigned int m_ID;
};

} // namespace glwrapper
} // namespace eg

#endif
