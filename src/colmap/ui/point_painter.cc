// Copyright (c), ETH Zurich and UNC Chapel Hill.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//
//     * Neither the name of ETH Zurich and UNC Chapel Hill nor the names of
//       its contributors may be used to endorse or promote products derived
//       from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#include "colmap/ui/point_painter.h"

#include <cstddef>

namespace colmap {

// PainterBase uploads vertices as 3 floats + 4 bytes with the color at byte
// offset 12; line and triangle painters additionally rely on Data being a
// contiguous array of vertices.
static_assert(sizeof(PointPainter::Data) ==
              3 * sizeof(float) + 4 * sizeof(uint8_t));
static_assert(offsetof(PointPainter::Data, r) == 3 * sizeof(float));

void PointPainter::Setup() {
  SetupShaders({{QOpenGLShader::Vertex, ":/shaders/points.v.glsl"},
                {QOpenGLShader::Fragment, ":/shaders/points.f.glsl"}});
}

void PointPainter::Upload(const std::vector<PointPainter::Data>& data) {
  UploadGeoms(data, "a_position", sizeof(PointPainter::Data));
}

void PointPainter::Render(const QMatrix4x4& pmv_matrix,
                          const float point_size) {
  if (!BeginRender()) {
    return;
  }

  shader_program_.setUniformValue("u_pmv_matrix", pmv_matrix);
  shader_program_.setUniformValue("u_point_size", point_size);

  GLFunctions()->glDrawArrays(GL_POINTS, 0, (GLsizei)num_geoms_);

  EndRender();
}

}  // namespace colmap
