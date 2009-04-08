// Copyright (c) 2007-09  INRIA Sophia-Antipolis (France).
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
// Author(s) : Pierre Alliez and Laurent Saboret

#ifndef CGAL_READ_XYZ_POINT_SET_H
#define CGAL_READ_XYZ_POINT_SET_H

#include <CGAL/Point_with_normal_3.h>
#include <CGAL/value_type_traits.h>
#include <CGAL/point_set_processing_assertions.h>

#include <deque>
#include <iostream>
#include <sstream>
#include <string>

CGAL_BEGIN_NAMESPACE


/// Read points (positions + normals, if available) from a .xyz file (ASCII).
///
/// @commentheading Template Parameters:
/// @param OutputIterator value_type must be a model of the PointWithNormal_3 concept.
///
/// @return true on success.
template <typename OutputIterator>
bool read_xyz_point_set(std::istream& stream, ///< input stream.
                        OutputIterator output) ///< output iterator.
{
  // value_type_traits is a workaround as back_insert_iterator's value_type is void
  typedef typename value_type_traits<OutputIterator>::type Point_with_normal;

  typedef typename Point_with_normal::Geom_traits Geom_traits;
  typedef typename Geom_traits::Point_3 Point;
  typedef typename Geom_traits::Vector_3 Vector;

  if(!stream)
  {
    std::cerr << "Error: cannot open file" << std::endl;
    return false;
  }

  // scan points
  long pointsCount; // number of points in file
  int lineNumber = 0;
  std::string line;
  while(getline(stream,line))
  {
    lineNumber++;

    // Read position + normal...
    double x,y,z;
    double nx,ny,nz;
    if (std::istringstream(line) >> x >> y >> z >> nx >> ny >> nz)
    {
      Point point(x,y,z);
      Vector normal(nx,ny,nz);
      *output = Point_with_normal(point,normal);
      output++;
    }
    // ...or read only position...
    else if (std::istringstream(line) >> x >> y >> z)
    {
      Point point(x,y,z);
      *output = point;
      output++;
    }
    // ...or skip number of points (optional)
    else if (lineNumber == 1 && std::istringstream(line) >> pointsCount)
    {
      continue;
    }
    else
    {
      std::cerr << "Error line " << lineNumber << " of file" << std::endl;
      return false;
    }
  }

  return true;
}

/// Read points (positions only) from a .xyz file (ASCII).
/// Normals are ignored.
///
/// @commentheading Template Parameters:
/// @param OutputIterator value_type must be Point_3.
///
/// @return true on success.
template <typename OutputIterator>
bool read_xyz_point_set(std::istream& stream, ///< input stream.
                        OutputIterator output, ///< output iterator.
                        bool read_normals) ///<  must be false.
{
  CGAL_point_set_processing_assertion(read_normals == false);

  typedef typename value_type_traits<OutputIterator>::type Value_type;
  typedef typename Kernel_traits<Value_type>::Kernel Kernel;
  typedef typename CGAL::Point_with_normal_3<Kernel> Point_with_normal;

  // Read file in temporary Point_with_normal_3 container
  std::deque<Point_with_normal> pwns;
  if (read_xyz_point_set(stream, std::back_inserter(pwns)))
  {
    // copy to Point_3 container, removing normals
    std::copy(pwns.begin(), pwns.end(), output);

    return true;
  }
  else
    return false;
}


CGAL_END_NAMESPACE

#endif // CGAL_READ_XYZ_POINT_SET_H