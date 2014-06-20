/*
 * (C) Copyright 1996-2014 ECMWF.
 * 
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0. 
 * In applying this licence, ECMWF does not waive the privileges and immunities 
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */



#ifndef BuildEdges_hpp
#define BuildEdges_hpp
#include <string>
namespace atlas {
class Mesh;

namespace actions {

void build_edges( Mesh& mesh );
void build_pole_edges( Mesh& mesh );

// ------------------------------------------------------------------
// C wrapper interfaces to C++ routines
extern "C" 
{
  void atlas__build_edges (Mesh* mesh);
  void atlas__build_pole_edges (Mesh* mesh);
}
// ------------------------------------------------------------------

} // namespace actions
} // namespace atlas

#endif // BuildEdges_hpp