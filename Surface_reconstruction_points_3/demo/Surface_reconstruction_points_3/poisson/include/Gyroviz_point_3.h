// Copyright (c) 2007  INRIA (France).
// All rights reserved.
//
// This file is part of CGAL (www.cgal.org); you may redistribute it under
// the terms of the Q Public License version 1.0.
// See the file LICENSE.QPL distributed with CGAL.
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
//
// Author(s)     : Laurent Saboret, Nader Salman

#ifndef GYROVIZ_POINT_3_H
#define GYROVIZ_POINT_3_H

#include <CGAL/Point_with_normal_3.h>
#include <CGAL/Orientable_normal_3.h>
#include <CGAL/Iterator_project.h>

#include <set>
#include <algorithm>
#include <cassert>


/// The Gyroviz_point_3 class represents a 3D point with:
/// - a position,
/// - a normal (oriented or not),
/// - a list of camera/2D point pairs used to reconstruct the point from images,
///
/// @heading Is Model for the Concepts: 
/// Model of the PointWithOrientableNormal_3 concept.
///
/// @heading Parameters:
/// @param Gt   Kernel's geometric traits.

template<class Gt>
class Gyroviz_point_3 
  : public CGAL::Point_with_normal_3<Gt, CGAL::Orientable_normal_3<Gt> >
{
// Private types
private:

    // Base class
    typedef CGAL::Point_with_normal_3<Gt, CGAL::Orientable_normal_3<Gt> > Base;

    // Auxiliary class to build a camera iterator
    template <class Node> // Node is Camera_point2_pair
    struct Project_camera {
      typedef Node                  argument_type;
      typedef typename Gt::Point_3  Point_3;
      typedef Point_3               result_type;
      Point_3&       operator()(Node& x)       const { return x.first; }
      const Point_3& operator()(const Node& x) const { return x.first; }
    };

// Public types
public:

    // Base class
    typedef Base Point_with_normal; ///< Model of the PointWithOrientableNormal_3 concept.

    // Repeat Point_with_normal_3 public types
    typedef Gt Geom_traits; ///< Kernel's geometric traits.
    typedef typename Geom_traits::FT FT;
    typedef typename Geom_traits::RT RT;
    typedef typename Geom_traits::Point_2 Point_2; ///< Kernel's Point_2 class.
    typedef typename Geom_traits::Point_3 Point_3; ///< Kernel's Point_3 class.
    typedef typename Point_with_normal::Normal Normal; ///< Model of OrientableNormal_3 concept.

    /// Camera/2D point pair. The 2D point is the 3D point (*this) projection's
    /// in the camera's image plane.
    typedef std::map <Point_3, Point_2> Camera_point2_map;
    typedef std::pair<Point_3, Point_2> Camera_point2_pair;

    /// Iterator over camera/2D point pairs.
    typedef typename Camera_point2_map::const_iterator 
                                        Camera_point2_pair_const_iterator;

    /// Iterator over cameras.
    typedef CGAL::Iterator_project<Camera_point2_pair_const_iterator, 
                                   Project_camera<std::pair<const Point_3,Point_2> > > // warning: const is required
                                        Camera_const_iterator;      

// Public methods
public:

    /// Point is (0,0,0) by default.
    /// Normal is (0,0,0) and is oriented by default.
    /// Camera list is empty by default.
    Gyroviz_point_3(const CGAL::Origin& o = CGAL::ORIGIN)
    : Base(o)
    {
    }
    Gyroviz_point_3(FT x, FT y, FT z,
                    const Normal& normal = CGAL::NULL_VECTOR)
    : Base(x,y,z,normal)
    {
    }
    Gyroviz_point_3(RT hx, RT hy, RT hz, RT hw,
                    const Normal& normal = CGAL::NULL_VECTOR)
    : Base(hx,hy,hz,hw,normal)
    {
    }
    Gyroviz_point_3(const Point_3& point,
                    const Normal& normal = CGAL::NULL_VECTOR)
    : Base(point, normal)
    {
    }
    template <class K, class N>
    Gyroviz_point_3(const Point_with_normal_3<K,N>& pwn)
    : Base(pwn)
    {
    }
    template < class InputIterator >
    Gyroviz_point_3(const Point_3& point,
                    InputIterator first_camera_point2_pair, 
                    InputIterator beyond_camera_point2_pair)
    : Base(point, CGAL::NULL_VECTOR)
    {
      camera_point2_map.insert(first_camera_point2_pair, beyond_camera_point2_pair); 
    }
    template < class InputIterator >
    Gyroviz_point_3(const Point_3& point,
                    const Normal& normal,
                    InputIterator first_camera_point2_pair, 
                    InputIterator beyond_camera_point2_pair)
    : Base(point, normal)
    {
      camera_point2_map.insert(first_camera_point2_pair, beyond_camera_point2_pair); 
    }

    /// Copy constructor
    Gyroviz_point_3(const Gyroviz_point_3& gpt)
    : Base(gpt)
    {
      camera_point2_map = gpt.camera_point2_map;
    }
    template<class K>
    Gyroviz_point_3(const Gyroviz_point_3<K>& gpt)
    : Base(gpt)
    {
      camera_point2_map.insert(gpt.camera_point2_pairs_begin(), gpt.camera_point2_pairs_end()); 
    }
    /// Operator =()
    Gyroviz_point_3& operator=(const Gyroviz_point_3& gpt)
    {
      Base::operator=(gpt);
      camera_point2_map = gpt.camera_point2_map;
      return *this;
    }

    /// Merge points, including lists of camera/2D point pairs.
    void merge(const Gyroviz_point_3& gpt)
    { 
      // we assume that both points 3D position is the same
      
      // merge camera/2D point maps
      camera_point2_map.insert(gpt.camera_point2_pairs_begin(), gpt.camera_point2_pairs_end()); 
    }

    // Inherited operators ==() and !=() are fine.

    /// Get camera/2D point pairs.
    const Camera_point2_map& camera_point2_pairs() const 
    { 
      return camera_point2_map; 
    }
    Camera_point2_pair_const_iterator camera_point2_pairs_begin() const 
    { 
      return camera_point2_map.begin(); 
    }
    Camera_point2_pair_const_iterator camera_point2_pairs_end  () const 
    { 
      return camera_point2_map.end(); 
    }

    /// Set camera/2D point pairs.
    template < class InputIterator >
    void set_camera_point2_pairs(InputIterator first_camera_point2_pair, 
                                 InputIterator beyond_camera_point2_pair)
    {
      camera_point2_map.clear();
      camera_point2_map.insert(first_camera_point2_pair, beyond_camera_point2_pair); 
    }

    /// Add camera/2D point pairs.
    void add_camera_point2_pair(Camera_point2_pair camera_and_point2)
    {
      camera_point2_map.insert(camera_and_point2); 
    }
    template < class InputIterator >
    void add_camera_point2_pairs(InputIterator first_camera_point2_pair, 
                                 InputIterator beyond_camera_point2_pair)
    {
      camera_point2_map.insert(first_camera_point2_pair, beyond_camera_point2_pair); 
    }

    /// Get cameras.
    Camera_const_iterator cameras_begin() const 
    { 
      return Camera_const_iterator(camera_point2_pairs_begin()); 
    }
    Camera_const_iterator cameras_end() const   
    { 
      return Camera_const_iterator(camera_point2_pairs_end()); 
    }
    
// Data
private:

    // List of cameras
    Camera_point2_map camera_point2_map;
};


namespace boost {

/// Helper type and constant to get a "vertex_cameras" property map.
enum vertex_cameras_t { vertex_cameras } ;
BOOST_INSTALL_PROPERTY(vertex, cameras);

} // namespace boost


#endif //GYROVIZ_POINT_3_H
