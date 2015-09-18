/*
 * (C) Copyright 1996-2015 ECMWF.
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 * In applying this licence, ECMWF does not waive the privileges and immunities
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

#include <sstream>
#include <iostream>

#include "eckit/exception/Exceptions.h"
#include "eckit/config/Resource.h"
#include "eckit/runtime/Tool.h"
#include "eckit/memory/SharedPtr.h"
#include "atlas/atlas.h"
#include "atlas/Mesh.h"
#include "atlas/Nodes.h"
#include "atlas/grids/grids.h"
#include "atlas/functionspace/NodesFunctionSpace.h"
#include "atlas/actions/GenerateMesh.h"
#include "atlas/actions/WriteLoadBalanceReport.h"

//------------------------------------------------------------------------------------------------------

using namespace eckit;
using namespace atlas;
using namespace atlas::actions;
using namespace atlas::grids;
using namespace atlas::functionspace;

//------------------------------------------------------------------------------------------------------

class AtlasLoadbalance : public eckit::Tool {

  virtual void run();

public:

  AtlasLoadbalance(int argc,char **argv): eckit::Tool(argc,argv)
  {
    bool help = Resource< bool >("--help",false);

    do_run = true;

    std::string help_str =
        "NAME\n"
        "       atlas-loadbalance - <TODO>\n"
        "\n"
        "SYNOPSIS\n"
        "       atlas-loadbalance GRID [OPTION]... [--help] \n"
        "\n"
        "DESCRIPTION\n"
        "\n"
        "       GRID: unique identifier for grid \n"
        "           Example values: rgg.N80, rgg.TL159, gg.N40, ll.128x64\n"
        "\n"
        "       --halo       Output file for mesh\n"
        "\n"
        "AUTHOR\n"
        "       Written by Willem Deconinck.\n"
        "\n"
        "ECMWF                        September 2015"
        ;
    if( help )
    {
      Log::info() << help_str << std::endl;
      do_run = false;
    }

    if( argc == 1 )
    {
      Log::info() << "usage: atlas-loadbalance GRID [OPTION]... [--help]" << std::endl;
      do_run = false;
    }

    atlas_init(argc,argv);

    key = "";
    for( int i=0; i<argc; ++i )
    {
      if( i==1 && argv[i][0] != '-' )
      {
        key = std::string(argv[i]);
      }
    }

    halo       = Resource< int > ( "--halo", 1 );
  }

private:

  bool do_run;
  std::string key;
  int halo;
  std::string identifier;
};

//------------------------------------------------------------------------------------------------------

void AtlasLoadbalance::run()
{
  if( !do_run ) return;
  grids::load();

  ReducedGrid::Ptr grid;
  try{ grid = ReducedGrid::Ptr( ReducedGrid::create(key) ); }
  catch( eckit::BadParameter& err ){}

  if( !grid ) return;
  SharedPtr<Mesh> mesh( generate_mesh(*grid) );
  SharedPtr<NodesFunctionSpace> nodes( new NodesFunctionSpace("nodes",*mesh,Halo(halo)) );
  
  std::stringstream s;
  write_load_balance_report(*mesh,s);
  
  if( eckit::mpi::rank() == 0 )
  {
    std::cout << s.str() << std::endl;
  }
  atlas_finalize();
}

//------------------------------------------------------------------------------------------------------

int main( int argc, char **argv )
{
  AtlasLoadbalance tool(argc,argv);
  tool.start();
  return 0;
}