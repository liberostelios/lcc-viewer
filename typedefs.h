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
// SPDX-License-Identifier: LGPL-3.0+
//
// Author(s)     : Guillaume Damiand <guillaume.damiand@liris.cnrs.fr>
//
#ifndef TYPEDEFS_H
#define TYPEDEFS_H

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

#include <CGAL/Triangulation_2_projection_traits_3.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>
#include <CGAL/Constrained_Delaunay_triangulation_2.h>
#include <CGAL/Constrained_triangulation_plus_2.h>

#include <CGAL/Linear_cell_complex.h>
#include <CGAL/Linear_cell_complex_constructors.h>
#include <CGAL/Linear_cell_complex_operations.h>
#include <CGAL/Combinatorial_map_save_load.h>

#include <CGAL/IO/Color.h>
#include <CGAL/Timer.h>
#include <CGAL/Random.h>

#include <cstdio>
#include <cstring>
#include <iostream>
#include <fstream>
#include <vector>
#include <list>

#include "thirdparty/json.hpp"

// Global random
extern CGAL::Random myrandom;

// Use to define properties on volumes.
#define LCC_DEMO_VISIBLE 1 // if not visible => hidden
#define LCC_DEMO_FILLED  2 // if not filled, wireframe
#define LCC_DEMO_SELECTED 4 // if selected

struct CachePoint
{
  double x, y, z;
};

struct CacheTriangle{
  CachePoint points[3];
  CachePoint normals[3];
};

class Face_cache
{
public:
  Face_cache() {}

  bool has_triangle_cache()
  {
    return !m_triangles.empty();
  }

  void clear_triangle_cache()
  {
    m_triangles.clear();
  }

  std::vector<CacheTriangle>* triangle_cache()
  {
    return &m_triangles;
  }

  void set_guid(std::string guid)
  {
    m_guid = guid;
  }

  std::string get_guid()
  {
    return m_guid;
  }

  void set_semantic_surface(std::string semantic_surface)
  {
    m_semantic_surface = semantic_surface;
  }

  std::string get_semantic_surface()
  {
    return m_semantic_surface;
  }

private:
  std::vector<CacheTriangle> m_triangles;
  std::string m_guid;
  std::string m_semantic_surface;
};

class Volume_info
{
  friend void CGAL::read_cmap_attribute_node<Volume_info>
  (const boost::property_tree::ptree::value_type &v,Volume_info &val);

  friend void CGAL::write_cmap_attribute_node<Volume_info>(boost::property_tree::ptree & node,
                                                           const Volume_info& arg);
public:
  Volume_info() : m_color(CGAL::Color(myrandom.get_int(0,256),
                                      myrandom.get_int(0,256),
                                      myrandom.get_int(0,256))),
    m_status( LCC_DEMO_VISIBLE | LCC_DEMO_FILLED ),
    m_guid("nothing")
  {}

  CGAL::Color& color()
  { return m_color; }
  const CGAL::Color& color() const
  { return m_color; }

  std::string color_name() const
  {
    std::ostringstream ss;
    ss<<std::setfill('0');
    ss<<"#"<<std::hex<<std::setw(2)<<(int)m_color.red()
     <<std::setw(2)<<(int)m_color.green()<<std::setw(2)<<(int)m_color.blue();
    return ss.str();
  }

  bool is_visible() const
  { return (m_status & LCC_DEMO_VISIBLE)!=0; }
  bool is_filled() const
  { return (m_status & LCC_DEMO_FILLED)!=0; }
  bool is_filled_and_visible() const
  { return is_filled() && is_visible(); }
  bool is_selected() const
  { return (m_status & LCC_DEMO_SELECTED)!=0; }

  void set_visible(bool val=true)
  {
    if ( is_visible()==val ) return;
    if ( val ) m_status = m_status | LCC_DEMO_VISIBLE;
    else       m_status = m_status ^ LCC_DEMO_VISIBLE;
  }
  void set_filled(bool val=true)
  {
    if ( is_filled()==val ) return;
    if ( val ) m_status = m_status | LCC_DEMO_FILLED;
    else       m_status = m_status ^ LCC_DEMO_FILLED;
  }
  void set_selected(bool val=true)
  {
    if ( is_selected()==val ) return;
    if ( val ) m_status = m_status | LCC_DEMO_SELECTED;
    else       m_status = m_status ^ LCC_DEMO_SELECTED;
  }

  void set_guid(std::string guid)
  {
    m_guid = guid;
  }

  std::string get_guid()
  {
    return m_guid;
  }

  void negate_visible()
  { set_visible(!is_visible()); }
  void negate_filled()
  { set_filled(!is_filled()); }
  void negate_selected()
  { set_selected(!is_selected()); }

private:
  CGAL::Color m_color;
  char        m_status;
  std::string m_guid;
};

namespace CGAL
{

template<>
inline void read_cmap_attribute_node<Volume_info>
(const boost::property_tree::ptree::value_type &v,Volume_info &val)
{
  try
  {
    val.m_status = v.second.get<int>("status");
  }
  catch(const std::exception &  )
  {}

  try
  {
    char r = v.second.get<int>("color-r");
    char g = v.second.get<int>("color-g");
    char b = v.second.get<int>("color-b");
    val.m_color = CGAL::Color(r,g,b);
  }
  catch(const std::exception &  )
  {}

  try
  {
    val.m_guid = v.second.get<std::string>("guid");
  }
  catch(const std::exception &  )
  {}
}

// Definition of function allowing to save custon information.
template<>
inline void write_cmap_attribute_node<Volume_info>(boost::property_tree::ptree & node,
                                                   const Volume_info& arg)
{
  boost::property_tree::ptree & nValue = node.add("v","");
  nValue.add("status",(int)arg.m_status);
  nValue.add("color-r",(int)arg.m_color.r());
  nValue.add("color-g",(int)arg.m_color.g());
  nValue.add("color-b",(int)arg.m_color.b());
  nValue.add("guid",arg.m_guid);
}

}

class Myitems
{
public:
  template < class Refs >
  struct Dart_wrapper
  {
    typedef CGAL::Cell_attribute_with_point< Refs > Vertex_attrib;
    typedef CGAL::Cell_attribute< Refs, Face_cache> Face_attrib;
    typedef CGAL::Cell_attribute< Refs, Volume_info> Volume_attrib;

    typedef CGAL::cpp11::tuple<Vertex_attrib,void,Face_attrib,
                               Volume_attrib> Attributes;
  };
};

typedef CGAL::Linear_cell_complex_traits
<3,CGAL::Exact_predicates_inexact_constructions_kernel> Mytraits;

typedef CGAL::Linear_cell_complex_for_combinatorial_map<3,3,Mytraits,Myitems> LCC;
typedef LCC::Dart_handle      Dart_handle;
typedef LCC::Vertex_attribute Vertex;

typedef LCC::Point    Point_3;
typedef LCC::Vector   Vector_3;

typedef CGAL::Timer Timer;

struct Vertex_info
{
  Dart_handle dh;
  Vector_3 v;
};

struct Face_info {
  bool exist_edge[3];
  bool is_external;
  bool is_process;
};

typedef CGAL::Triangulation_2_projection_traits_3<CGAL::Exact_predicates_inexact_constructions_kernel> P_traits;
typedef CGAL::Triangulation_vertex_base_with_info_2<Vertex_info, P_traits> Vb;

typedef CGAL::Triangulation_face_base_with_info_2<Face_info,P_traits> Fb1;

typedef CGAL::Constrained_triangulation_face_base_2<P_traits, Fb1>    Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>                   TDS;
// typedef CGAL::No_intersection_tag                                     Itag;
 typedef CGAL::Exact_predicates_tag Itag;
typedef CGAL::Constrained_Delaunay_triangulation_2<P_traits, TDS,
                                                   Itag>              CDT;

class FaceFormatter {
public:
  virtual void getColor(Face_cache*, double&, double&, double&){};
};

class SurfaceFaceFormatter : public FaceFormatter {
public:
  void getColor(Face_cache* face_info, double& r, double& g, double& b) override
  {
    r *= 255.0;
    g *= 255.0;
    b *= 255.0;

    if (face_info->get_semantic_surface() == "WallSurface")
    {
      r = 255;
      g = 255;
      b = 255;
    }
    else if (face_info->get_semantic_surface() == "RoofSurface")
    {
      r = 255;
      g = 0;
      b = 0;
    }
    else if (face_info->get_semantic_surface() == "GroundSurface")
    {
      r = 0;
      g = 0;
      b = 0;
    }
    else if (face_info->get_semantic_surface() == "Window")
    {
      r = 0;
      g = 0;
      b = 255;
    }
    else if (face_info->get_semantic_surface() == "Door")
    {
      r = 0;
      g = 255;
      b = 0;
    }
    else if (face_info->get_semantic_surface() == "WaterSurface")
    {
      r = 0;
      g = 0;
      b = 255;
    }
    else if (face_info->get_semantic_surface() == "TrafficArea")
    {
      r = 0;
      g = 128;
      b = 128;
    }

    r /= 255.0;
    g /= 255.0;
    b /= 255.0;
  }

  ~SurfaceFaceFormatter()
  {}
};

struct Color
{
  Color() :
    r(myrandom.get_int(0,256)),
    g(myrandom.get_int(0,256)),
    b(myrandom.get_int(0,256))
  {}

  double r, g, b;
};

class ObjectFaceFormatter : public FaceFormatter {
public:
  std::map<std::string, Color*> colormap;

  ObjectFaceFormatter(LCC* lcc)
  {
    for (auto it=lcc->one_dart_per_cell<2>().begin();
         it!=lcc->one_dart_per_cell<2>().end(); ++it)
    {
      Color* new_color = new Color();
      colormap[lcc->info<2>(it).get_guid()] = new_color;
    }
  }

  void getColor(Face_cache* face_info, double& r, double& g, double& b) override
  {
    Color* mycolor = colormap[face_info->get_guid()];
    r = mycolor->r;
    g = mycolor->g;
    b = mycolor->b;

    r /= 255.0;
    g /= 255.0;
    b /= 255.0;
  }

  ~ObjectFaceFormatter()
  {
    for(auto color = colormap.begin(); color != colormap.end(); color++)
    {
      delete color->second;
    }
  }
};

struct Scene {
  LCC* lcc;
};

#endif
