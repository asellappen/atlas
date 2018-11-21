/*
 * (C) Copyright 2013 ECMWF.
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 * In applying this licence, ECMWF does not waive the privileges and immunities
 * granted to it by virtue of its status as an intergovernmental organisation
 * nor does it submit to any jurisdiction.
 */

#include "FunctionSpaceImpl.h"
#include "atlas/field/Field.h"
#include "atlas/option/Options.h"
#include "atlas/runtime/ErrorHandling.h"
#include "atlas/util/Metadata.h"

namespace atlas {
namespace functionspace {

// ------------------------------------------------------------------

FunctionSpaceImpl::FunctionSpaceImpl() : metadata_( new util::Metadata() ) {}

FunctionSpaceImpl::~FunctionSpaceImpl() {
    delete metadata_;
}

atlas::Field FunctionSpaceImpl::createField( const atlas::Field& field ) const {
    return createField( field, util::NoConfig() );
}

void FunctionSpaceImpl::haloExchange( FieldSet&, bool ) const {
    NOTIMP;
}

void FunctionSpaceImpl::haloExchange( Field&, bool ) const {
    NOTIMP;
}

Field NoFunctionSpace::createField( const eckit::Configuration& ) const {
    NOTIMP;
}
Field NoFunctionSpace::createField( const Field&, const eckit::Configuration& ) const {
    NOTIMP;
}

template <typename DATATYPE>
Field FunctionSpaceImpl::createField( const eckit::Configuration& options ) const {
    return createField( option::datatypeT<DATATYPE>() | options );
}

template <typename DATATYPE>
Field FunctionSpaceImpl::createField() const {
    return createField( option::datatypeT<DATATYPE>() );
}

template Field FunctionSpaceImpl::createField<double>() const;
template Field FunctionSpaceImpl::createField<float>() const;
template Field FunctionSpaceImpl::createField<int>() const;
template Field FunctionSpaceImpl::createField<long>() const;


template Field FunctionSpaceImpl::createField<double>( const eckit::Configuration& ) const;
template Field FunctionSpaceImpl::createField<float>( const eckit::Configuration& ) const;
template Field FunctionSpaceImpl::createField<int>( const eckit::Configuration& ) const;
template Field FunctionSpaceImpl::createField<long>( const eckit::Configuration& ) const;


// ------------------------------------------------------------------

}  // namespace functionspace

// ------------------------------------------------------------------

}  // namespace atlas
