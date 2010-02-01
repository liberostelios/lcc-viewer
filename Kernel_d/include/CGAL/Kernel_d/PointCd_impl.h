// Copyright (c) 2001  Utrecht University (The Netherlands),
// ETH Zurich (Switzerland), Freie Universitaet Berlin (Germany),
// INRIA Sophia-Antipolis (France), Martin-Luther-University Halle-Wittenberg
// (Germany), Max-Planck-Institute Saarbruecken (Germany), RISC Linz (Austria),
// and Tel-Aviv University (Israel).  All rights reserved.
//
// This file is part of CGAL (www.cgal.org); you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License as
// published by the Free Software Foundation; version 2.1 of the License.
// See the file LICENSE.LGPL distributed with CGAL.
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
// Author(s)     : Michael Seel <seel@mpi-sb.mpg.de>

#ifndef CGAL_POINTCDXXX_C
#define CGAL_POINTCDXXX_C
CGAL_BEGIN_NAMESPACE
#define PointCd PointCd2

template <class FT, class LA>
PointCd<FT,LA> PointCd<FT,LA>::
transform(const Aff_transformationCd<FT,LA>& t) const
{ typename LA::Vector res = t(vector_rep());
  return PointCd<FT,LA>(dimension(),res.begin(),res.end()); }

template <class FT, class LA>
VectorCd<FT,LA> PointCd<FT,LA>::operator-(const Origin&) const 
{ return VectorCd<FT,LA>(Base(*this)); }

template <class FT, class LA>
PointCd<FT,LA> PointCd<FT,LA>::operator+(const VectorCd<FT,LA> &v) const
{ PointCd<FT,LA> res(dimension()); 
  res.ptr()->cartesian_add(ptr(), v.ptr());
  return res; 
}

template <class FT, class LA>
PointCd<FT,LA> PointCd<FT,LA>::operator-(const VectorCd<FT,LA> &v) const
{ PointCd<FT,LA> res(dimension()); 
  res.ptr()->cartesian_sub(ptr(), v.ptr()); 
  return res; 
}

template <class FT, class LA>
PointCd<FT,LA>& PointCd<FT,LA>::operator+= (const VectorCd<FT,LA>& v)
{ int d = dimension(); 
  PointCd<FT,LA> old(*this); 
  *this = PointCd<FT,LA>(d); 
  ptr()->cartesian_add(old.ptr(), v.ptr()); 
  return *this; 
}

template <class FT, class LA>
PointCd<FT,LA>& PointCd<FT,LA>::operator-= (const VectorCd<FT,LA>& v)
{ int d = dimension(); 
  PointCd<FT,LA> old(*this); 
  *this = PointCd<FT,LA>(d); 
  ptr()->cartesian_sub(old.ptr(), v.ptr()); 
  return *this; 
}

template <class FT, class LA>
std::istream& operator>>(std::istream& I, PointCd<FT,LA>& p)
{ p.copy_on_write(); p.ptr()->read(I);
  return I; 
}

template <class FT, class LA>
std::ostream& operator<<(std::ostream& O, const PointCd<FT,LA>& p)
{ p.ptr()->print(O,"PointCd"); return O; } 

#undef PointCd
CGAL_END_NAMESPACE
#endif // CGAL_POINTCDXXX_C
