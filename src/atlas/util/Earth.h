/*
 * (C) Copyright 1996-2017 ECMWF.
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 * In applying this licence, ECMWF does not waive the privileges and immunities
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

#pragma once

#include "atlas/util/Constants.h"

namespace atlas {
class PointLonLat;
class PointXYZ;
}

//------------------------------------------------------------------------------------------------------

namespace atlas {
namespace util {

//------------------------------------------------------------------------------------------------------

struct Earth
{
    static constexpr double radiusInMeters() { return 6371229.; }
    static constexpr double radiusInKm()     { return radiusInMeters() / 1.0e3; }

    static constexpr double areaInSqMeters() { return 4. * M_PI * radiusInMeters() * radiusInMeters(); }
    static constexpr double areaInSqKm()     { return 4. * M_PI * radiusInKm()     * radiusInKm();     }

    // Great-circle central angle between two points, in radians
    static double centralAngle(const PointLonLat&, const PointLonLat&);
    static double centralAngle(const PointXYZ&, const PointXYZ&);

    // Great-circle distance between two points
    static double distanceInMeters(const PointLonLat&, const PointLonLat&);
    static double distanceInMeters(const PointXYZ&, const PointXYZ&);
};

//------------------------------------------------------------------------------------------------------

} // namespace util
} // namespace atlas
