// Copyright (c) 2011 CNRS and LIRIS' Establishments (France).
// All rights reserved.
//
// This file is part of CGAL (www.cgal.org); you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License as
// published by the Free Software Foundation; either version 3 of the License,
// or (at your option) any later version.
//
// Licensees holding a valid commercial license may use this file in
// accordance with the commercial license agreement provided with the software.
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//
// $URL$
// $Id$
//
// Author(s)     : Guillaume Damiand <guillaume.damiand@liris.cnrs.fr>
//                 Kumar Snehasish <kumar.snehasish@gmail.com>
//
#ifndef VIEWER_H
#define VIEWER_H

#include "typedefs.h"
#include <QGLViewer/qglviewer.h>
#include <QKeyEvent>

#include <QOpenGLFunctions_2_1>
#include <QOpenGLVertexArrayObject>
#include <QGLBuffer>
#include <QOpenGLShaderProgram>
class Viewer : public QGLViewer, QOpenGLFunctions_2_1
{
  Q_OBJECT

  CGAL::Timer timer;
  Scene* scene;
  bool wireframe;
  bool flatShading;
  bool edges;
  bool vertices;
  CGAL::Bbox_3 bb;

  GLuint m_dlFaces;
  GLuint m_dlFacesFlat;
  GLuint m_dlEdges;
  GLuint m_dlVertices;
  bool m_displayListCreated;

  typedef LCC::Dart_handle Dart_handle;
  typedef LCC::Dart_const_handle Dart_const_handle;


public:
  Viewer(QWidget* parent);
  ~Viewer()
  {
      buffers[0].destroy();
      buffers[1].destroy();
      buffers[2].destroy();
      buffers[3].destroy();
      buffers[4].destroy();
      buffers[5].destroy();
      buffers[6].destroy();
      buffers[7].destroy();
      vao[0].destroy();
      vao[1].destroy();
      vao[2].destroy();
      vao[3].destroy();

  }
  void setScene(Scene* scene_)
  {
    scene = scene_;
  }

public:
  void draw();

  virtual void init();

  void keyPressEvent(QKeyEvent *e);

  virtual QString helpString() const;

public Q_SLOTS:

  void sceneChanged();


private:

  bool are_buffers_initialized;
  //Shaders elements

  int vertexLocation[3];
  int normalsLocation;
  int mvpLocation[2];
  int mvLocation;
  int colorLocation;
  int colorsLocation;
  int lightLocation[5];


  std::vector<float> pos_points;
  std::vector<float> pos_lines;
  std::vector<float> pos_facets;
  std::vector<float> smooth_normals;
  std::vector<float> flat_normals;
  std::vector <float> colors;

  QGLBuffer buffers[10];
  QOpenGLVertexArrayObject vao[10];
  QOpenGLShaderProgram rendering_program;
  QOpenGLShaderProgram rendering_program_p_l;

  void initialize_buffers();
  void compute_elements();
  void attrib_buffers(QGLViewer*);
  void compile_shaders();
  void triangulate_facet();
  bool is_Triangulated();
};
#endif
