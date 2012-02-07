# \file CMakeLists.txt
 #    This is the CMake file for the Ephemeris sub-directory.
 #
 #    Path              : /Astrodynamics/Ephemeris/
 #    Version           : 4
 #    Check status      : Checked
 #
 #    Author/Checker    : S.M. Persson
 #    Affiliation       : McGill University, Department of Mechanical Engineering
 #    E-mail address    : mikael.s.persson@gmail.com
 #
 #    Author/Checker    : K. Kumar
 #    Affiliation       : Delft University of Technology
 #    E-mail address    : K.Kumar@tudelft.nl
 #
 #    Author            : B. Tong Minh
 #    Affiliation       : Delft University of Technology
 #    E-mail address    : b.tongminh@student.tudelft.nl
 #
 #    Date created      : 20 August, 2011
 #    Last modified     : 23 November, 2011
 #
 #    References
 #
 #    Notes
 #
 #    Copyright (c) 2010-2011 Delft University of Technology.
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
 #      111123    B. Tong Minh      Added unit test for State.
 #      120204    B. Tong Minh      Copied from States 
 #/

# Add source files.
set(EPHEMERIS_SOURCES
  "${SRCROOT}${EPHEMERISDIR}/approximatePlanetPositions.cpp"
  "${SRCROOT}${EPHEMERISDIR}/approximatePlanetPositionsBase.cpp"
  "${SRCROOT}${EPHEMERISDIR}/approximatePlanetPositionsCircularCoplanar.cpp"
)

# Add header files.
set(EPHEMERIS_HEADERS 
  "${SRCROOT}${EPHEMERISDIR}/approximatePlanetPositions.h"
  "${SRCROOT}${EPHEMERISDIR}/approximatePlanetPositionsBase.h"
  "${SRCROOT}${EPHEMERISDIR}/approximatePlanetPositionsCircularCoplanar.h"
  "${SRCROOT}${EPHEMERISDIR}/approximatePlanetPositionsDataContainer.h"
  "${SRCROOT}${EPHEMERISDIR}/ephemeris.h"
)

# Add static libraries.
add_library(tudat_ephemeris STATIC ${EPHEMERIS_SOURCES} ${EPHEMERIS_HEADERS})
setup_tudat_library_target(tudat_ephemeris "${SRCROOT}${EPHEMERISDIR}")
setup_headers("${EPHEMERIS_HEADERS}" "${EPHEMERISDIR}")

# Add unit tests.
add_executable(test_ApproximatePlanetPositions "${SRCROOT}${EPHEMERISDIR}/UnitTests/unitTestApproximatePlanetPositions.cpp")
setup_custom_test_program(test_ApproximatePlanetPositions "${SRCROOT}${EPHEMERISDIR}")
target_link_libraries(test_ApproximatePlanetPositions tudat_ephemeris tudat_basic_astrodynamics tudat_states tudat_bodies tudat_gravitation tudat_root_finding_methods ${TUDAT_CORE_LIBRARIES})


# End of file.
