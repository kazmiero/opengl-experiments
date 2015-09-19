#include <array>
#include <vector>
#include <GL/glew.h>

class Renderer
{
  public:
    Renderer();
    void init();
    void render();

  private:
    GLuint vbo_;
    std::vector<std::array<float, 3>> vertices_;
};
