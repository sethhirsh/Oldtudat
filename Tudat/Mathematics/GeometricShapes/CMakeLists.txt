 #    Copyright (c) 2010-2012 Delft University of Technology.
 #
 #    This software is protected by national and international copyright.
 #    Any unauthorized use, reproduction or modification is unlawful and
 #    will be prosecuted. Commercial and non-private application of the
 #    software in any form is strictly prohibited unless otherwise granted
 #    by the authors.
 #
 #    The code is provided without any warranty; without even the implied
 #    warranty of merchantibility or fitness for a particular purpose.
 #
 #    Changelog
 #      YYMMDD    Author            Comment
 #      110820    S.M. Persson      File created.
 #      111025    K. Kumar          Adapted file to work with Revision 194.
 #      111026    K. Kumar          Adapted file so all headers show in project tree in Qt Creator.
 #

# Add source files.
set(GEOMETRICSHAPES_SOURCES
  "${SRCROOT}${MATHEMATICSDIR}/GeometricShapes/capsule.cpp"
  "${SRCROOT}${MATHEMATICSDIR}/GeometricShapes/compositeSurfaceGeometry.cpp"
  "${SRCROOT}${MATHEMATICSDIR}/GeometricShapes/conicalFrustum.cpp"
  "${SRCROOT}${MATHEMATICSDIR}/GeometricShapes/lawgsPartGeometry.cpp"
  "${SRCROOT}${MATHEMATICSDIR}/GeometricShapes/quadrilateralMeshedSurfaceGeometry.cpp"
  "${SRCROOT}${MATHEMATICSDIR}/GeometricShapes/singleSurfaceGeometry.cpp"
  "${SRCROOT}${MATHEMATICSDIR}/GeometricShapes/sphereSegment.cpp"
  "${SRCROOT}${MATHEMATICSDIR}/GeometricShapes/torus.cpp"
  "${SRCROOT}${MATHEMATICSDIR}/GeometricShapes/geometricShapesToFile.cpp"
)

# Add header files.
set(GEOMETRICSHAPES_HEADERS 
  "${SRCROOT}${MATHEMATICSDIR}/GeometricShapes/capsule.h"
  "${SRCROOT}${MATHEMATICSDIR}/GeometricShapes/compositeSurfaceGeometry.h"
  "${SRCROOT}${MATHEMATICSDIR}/GeometricShapes/conicalFrustum.h"
  "${SRCROOT}${MATHEMATICSDIR}/GeometricShapes/lawgsPartGeometry.h"
  "${SRCROOT}${MATHEMATICSDIR}/GeometricShapes/quadrilateralMeshedSurfaceGeometry.h"
  "${SRCROOT}${MATHEMATICSDIR}/GeometricShapes/singleSurfaceGeometry.h"
  "${SRCROOT}${MATHEMATICSDIR}/GeometricShapes/sphereSegment.h"
  "${SRCROOT}${MATHEMATICSDIR}/GeometricShapes/surfaceGeometry.h"
  "${SRCROOT}${MATHEMATICSDIR}/GeometricShapes/torus.h"
  "${SRCROOT}${MATHEMATICSDIR}/GeometricShapes/geometricShapesToFile.h"
)

# Add static libraries.
add_library(tudat_geometric_shapes STATIC ${GEOMETRICSHAPES_SOURCES})
setup_tudat_library_target(tudat_geometric_shapes "${SRCROOT}${MATHEMATICSDIR}/GeometricShapes")
setup_headers("${GEOMETRICSHAPES_HEADERS}" "${MATHEMATICSDIR}/GeometricShapes")

# Add unit tests.
add_executable(test_LawgsSurfaceGeometry "${SRCROOT}${MATHEMATICSDIR}/GeometricShapes/unitTestLawgsSurfaceGeometry.cpp")
setup_custom_test_program(test_LawgsSurfaceGeometry "${SRCROOT}${MATHEMATICSDIR}/GeometricShapes")
target_link_libraries(test_LawgsSurfaceGeometry tudat_geometric_shapes tudat_basic_mathematics ${TUDAT_CORE_LIBRARIES})