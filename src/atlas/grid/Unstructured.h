/*
 * (C) Copyright 1996-2017 ECMWF.
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 * In applying this licence, ECMWF does not waive the privileges and immunities
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

/// @author Willem Deconinck
/// @author Tiago Quintino
/// @author Pedro Maciel
/// @date January 2015


#ifndef atlas_grid_Unstructured_H
#define atlas_grid_Unstructured_H

#include <cstddef>
#include <vector>
#include "eckit/memory/ScopedPtr.h"
#include "atlas/grid/Grid.h"


namespace atlas {
namespace grid {


class Unstructured : public Grid {

  public: // methods

    static std::string grid_type_str();

    static std::string className();

    std::string gridType() const;

    /// Constructor taking a list of parameters
    Unstructured(const eckit::Parametrisation&);

    /// Constructor taking a list of points
    Unstructured(std::vector< Point >* pts);

    /// Constructor taking a mesh
    Unstructured(const mesh::Mesh& m);

    virtual ~Unstructured();

    virtual size_t npts() const;

    virtual void lonlat(std::vector< Point >&) const;

    virtual eckit::Properties spec() const;

    virtual const Domain& domain() const {
        return domain_;
    }

  private: // methods

    virtual void print(std::ostream&) const;

    /// Human readable name
    virtual std::string shortName() const;

    /// Hash of the lonlat array + BoundingBox
    virtual void hash(eckit::MD5&) const;

  protected:

    /// Storage of coordinate points
    eckit::ScopedPtr< std::vector< Point > > points_;

    /// Cache for the shortName
    mutable std::string shortName_;

    /// Cache for the spec since may be quite heavy to compute
    mutable eckit::ScopedPtr<eckit::Properties> cached_spec_;

  protected:  // members

    /// Area represented by the grid
    Domain domain_;

};


}  // namespace grid
}  // namespace atlas


#endif
