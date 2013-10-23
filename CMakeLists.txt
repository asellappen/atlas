
cmake_minimum_required( VERSION 2.8.4 FATAL_ERROR )

project( mpdata_shallow_water Fortran )

message( STATUS "--------------------------------------------------------------------" )
message( STATUS "MPDATA Shallow Water configuration" )


get_filename_component (Fortran_COMPILER_NAME ${CMAKE_Fortran_COMPILER} NAME)
if( NOT Fortran_COMPILER_NAME MATCHES "mpi.*" AND NOT Fortran_COMPILER_NAME MATCHES "ftn" )
  message( FATAL_ERROR "MPI Compiler needed.\n"
    "Please export FC to a MPI compiler such as mpif90 or ftn on Cray, or rerun "
    "cmake . -DCMAKE_Fortran_COMPILER=mpif90" )
endif( )

# make sure that the default build type is a RELEASE
if( NOT CMAKE_BUILD_TYPE )
  set( CMAKE_BUILD_TYPE Release CACHE STRING
      "Choose the type of build, options are: None Debug BitReproducible Release." FORCE )
endif( NOT CMAKE_BUILD_TYPE )
string( TOUPPER ${CMAKE_BUILD_TYPE} CMAKE_BUILD_TYPE )

# FFLAGS depend on the compiler
if( CMAKE_Fortran_COMPILER_ID MATCHES "GNU" )
  set( CMAKE_Fortran_FLAGS_RELEASE "-O3 -fstack-arrays -funroll-all-loops" )
  set( CMAKE_Fortran_FLAGS_DEBUG   "-O0 -g -fbounds-check" )
  set( CMAKE_Fortran_FLAGS_BIT "-O2 -fstack-arrays -funroll-all-loops" )
  # -fstack-arrays     : Allocate automatic arrays on the stack
  # -funroll-all-loops : Unroll all loops
  # -fbounds-check     : Bounds checking
  
elseif( CMAKE_Fortran_COMPILER_ID MATCHES "Intel" )
  set( CMAKE_Fortran_FLAGS_RELEASE "-O3" )
  set( CMAKE_Fortran_FLAGS_DEBUG   "-O0 -g" )
  set( CMAKE_Fortran_FLAGS_BIT "-O2" )

elseif( CMAKE_Fortran_COMPILER_ID MATCHES "Cray" )
  set( CMAKE_Fortran_FLAGS_RELEASE "-O3 -hfp3 -hmpi1 -hscalar3 -hvector3" )
  set( CMAKE_Fortran_FLAGS_DEBUG   "-O0 -Gfast -Ktrap=fp -R b" )
  set( CMAKE_Fortran_FLAGS_BIT "-O2 -hflex_mp=conservative -hadd_paren -hfp1 -hmpi1" )
  add_definitions( -emf ) 
  # -hfp3     : Special optimisation for floating points
  # -Ktrap=fp : Abort on NaN
  # -R b      : Bounds checking
  # -hflex_mp=conservative -hfp1 : Obtain bit-reproducible results
  # -hflex_mp=intolerant -hfp2   : Obtain bit-reproducible results (also)
  # -hadd_paren : encourage left to right fp evaluation
  # -hscalarN , -hvectorN : optimisation for scalar and vectorisation

endif( )

if( GRIB_API_DIR )
  message( STATUS "Grib enabled : ${GRIB_API_DIR}" )
  add_definitions( -DHAVE_GRIB )
  include_directories(${GRIB_API_DIR}/include)
  link_directories(${GRIB_API_DIR}/lib)
  list( APPEND TPL grib_api_f90 grib_api )
else( )
  message( STATUS "Grib disabled : To enable, run \"cmake . -DGRIB_API_DIR=path/to/grib\"" )
endif( )

# Datastructure library
add_library( datastructure
  src/common/parallel_module.F90
  src/common/common_module.F90
  src/mesh/elements_module.F90
  src/mesh/lagrangep0_module.F90
  src/mesh/lagrangep1_module.F90
  src/mesh/grid_module.F90
  src/mesh/split_globe_module.F90
  src/mesh/datastruct_module.F90
  src/io/joanna_module.F90
  src/io/gmsh_module.F90
  src/io/grib_module.F90
)
target_link_libraries( datastructure ${TPL} )


# Shallow water executable
file( COPY data DESTINATION ${CMAKE_CURRENT_BINARY_DIR} )
add_executable( shallow_water src/shallow_water_module.F90 src/shallow_water.F90 )
target_link_libraries( shallow_water datastructure ${TPL} )

# Test for parallelisation
add_executable( test_sync src/test_sync.F90 )
target_link_libraries( test_sync datastructure )

message( STATUS "--------------------------------------------------------------------" )
message( STATUS "Build   : ${CMAKE_BUILD_TYPE}" )
message( STATUS "Fortran : ${Fortran_COMPILER_NAME} -- ${CMAKE_Fortran_COMPILER_ID} ${CMAKE_Fortran_COMPILER_VERSION}" )
message( STATUS "Flags   : ${CMAKE_Fortran_FLAGS_${CMAKE_BUILD_TYPE}}" )
message( STATUS "--------------------------------------------------------------------" )
