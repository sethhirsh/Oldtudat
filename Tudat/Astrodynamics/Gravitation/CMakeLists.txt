# \file CMakeLists.txt
 #    This is the CMake file for the MissionSegments sub-directory.
 #
 #    Path              : /Astrodynamics/MissionSegments/
 #    Version           : 3
 #    Check status      : Checked
 #
 #    Author            : S.M. Persson
 #    Affiliation       : McGill University, Department of Mechanical Engineering
 #    E-mail address    : mikael.s.persson@gmail.com
 #
 #    Author            : K. Kumar
 #    Affiliation       : Delft University of Technology
 #    E-mail address    : K.Kumar@tudelft.nl
 #
 #    Author            : S.M. Persson
 #    Affiliation       : McGill University, Department of Mechanical Engineering
 #    E-mail address    : mikael.s.persson@gmail.com
 #
 #    Date created      : 20 August, 2011
 #    Last modified     : 26 October, 2011
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
 #/

# Set the source files.
set(GRAVITATION_SOURCES
  "${SRCROOT}${GRAVITATIONDIR}/centralGravityField.cpp"
  "${SRCROOT}${GRAVITATIONDIR}/gravitationalForceModel.cpp"
  "${SRCROOT}${GRAVITATIONDIR}/sphericalHarmonicsGravityField.cpp"

)

# Set the header files.
set(GRAVITATION_HEADERS 
  "${SRCROOT}${GRAVITATIONDIR}/gravityFieldModel.h"
  "${SRCROOT}${GRAVITATIONDIR}/centralGravityField.h"
  "${SRCROOT}${GRAVITATIONDIR}/gravitationalForceModel.h"
  "${SRCROOT}${GRAVITATIONDIR}/sphericalHarmonicsGravityField.h"


)

# Add static libraries.
add_library(tudat_gravitation STATIC ${GRAVITATION_SOURCES} ${GRAVITATION_HEADERS})
setup_tudat_library_target(tudat_gravitation "${SRCROOT}{GRAVITATIONDIR}")
setup_headers("${GRAVITATION_HEADERS}" "${GRAVITATIONDIR}")

add_executable(test_SphericalHarmonicsGravityField "${SRCROOT}${GRAVITATIONDIR}/UnitTests/unitTestSphericalHarmonicsGravityField.cpp")
setup_custom_test_program(test_SphericalHarmonicsGravityField "${SRCROOT}${GRAVITATIONDIR}")
target_link_libraries(test_SphericalHarmonicsGravityField tudat_input tudat_output tudat_mathematics tudat_astrodynamics tudat_basics tudat_gravitation tudat_bodies ${Boost_LIBRARIES})



# End of file.
