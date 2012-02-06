# \file CMakeLists.txt
 #    This is the CMake file for the BasicAstrodynamics package.
 #
 #    Path              : /Astrodynamics/BasicAstrodynamics/
 #    Version           : 2
 #    Check status      : Checked
 #
 #    Author/Checker    : S.M. Persson
 #    Affiliation       : McGill University, Department of Mechanical Engineering
 #    E-mail address    : mikael.s.persson@gmail.com
 #
 #    Author            : K. Kumar
 #    Affiliation       : Delft University of Technology
 #    E-mail address    : K.Kumar@tudelft.nl
 #
 #    Date created      : 20 August, 2011
 #    Last modified     : 3 February, 2012
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
 #	120203    K. Kumar	    Adapted for new BasicAstrodynamics package.
 #/

# Set the source files.
set(BASICASTRODYNAMICS_SOURCES
  "${SRCROOT}${BASICASTRODYNAMICSDIR}/keplerPropagator.cpp"
  "${SRCROOT}${BASICASTRODYNAMICSDIR}/convertMeanAnomalyToEccentricAnomaly.cpp"
  "${SRCROOT}${BASICASTRODYNAMICSDIR}/convertMeanAnomalyToHyperbolicEccentricAnomaly.cpp"
  "${SRCROOT}${BASICASTRODYNAMICSDIR}/seriesPropagator.cpp"
  "${SRCROOT}${BASICASTRODYNAMICSDIR}/momentDueToForceModel.cpp"
  "${SRCROOT}${BASICASTRODYNAMICSDIR}/generalizedForceModel.cpp"
)

# Set the header files.
set(BASICASTRODYNAMICS_HEADERS 
  "${SRCROOT}${BASICASTRODYNAMICSDIR}/keplerPropagator.h"
  "${SRCROOT}${BASICASTRODYNAMICSDIR}/convertMeanAnomalyBase.h"
  "${SRCROOT}${BASICASTRODYNAMICSDIR}/convertMeanAnomalyToEccentricAnomaly.h"
  "${SRCROOT}${BASICASTRODYNAMICSDIR}/convertMeanAnomalyToHyperbolicEccentricAnomaly.h"
  "${SRCROOT}${BASICASTRODYNAMICSDIR}/propagator.h"
  "${SRCROOT}${BASICASTRODYNAMICSDIR}/propagatorDataContainer.h"
  "${SRCROOT}${BASICASTRODYNAMICSDIR}/seriesPropagator.h"
  "${SRCROOT}${BASICASTRODYNAMICSDIR}/momentModel.h"
  "${SRCROOT}${BASICASTRODYNAMICSDIR}/momentDueToForceModel.h"
  "${SRCROOT}${BASICASTRODYNAMICSDIR}/forceModel.h"
  "${SRCROOT}${BASICASTRODYNAMICSDIR}/generalizedForceModel.h"
)

# Add static libraries.
add_library(tudat_basic_astrodynamics STATIC ${BASICASTRODYNAMICS_SOURCES} ${BASICASTRODYNAMICS_HEADERS})
setup_tudat_library_target(tudat_basic_astrodynamics "${SRCROOT}${BASICASTRODYNAMICSDIR}")
setup_headers("${BASICASTRODYNAMICS_HEADERS}" "${BASICASTRODYNAMICSDIR}")

# Add unit tests.
add_executable(test_OrbitalElementConversions "${SRCROOT}${BASICASTRODYNAMICSDIR}/unitTestOrbitalElementConversions.cpp")
setup_custom_test_program(test_OrbitalElementConversions "${SRCROOT}${BASICASTRODYNAMICSDIR}")
target_link_libraries(test_OrbitalElementConversions tudat_input tudat_output tudat_mathematics tudat_astrodynamics tudat_basics ${TUDAT_CORE_LIBRARIES} ${Boost_LIBRARIES})

add_executable(test_MeanToEccenticAnomalyConversion "${SRCROOT}${BASICASTRODYNAMICSDIR}/unitTestConvertMeanAnomalyToEccentricAnomaly.cpp")
setup_custom_test_program(test_MeanToEccenticAnomalyConversion "${SRCROOT}${BASICASTRODYNAMICSDIR}")
target_link_libraries(test_MeanToEccenticAnomalyConversion tudat_astrodynamics tudat_input tudat_output tudat_mathematics tudat_basics ${Boost_LIBRARIES})

add_executable(test_KeplerPropagator "${SRCROOT}${BASICASTRODYNAMICSDIR}/unitTestKeplerPropagator.cpp")
setup_custom_test_program(test_KeplerPropagator "${SRCROOT}${BASICASTRODYNAMICSDIR}")
target_link_libraries(test_KeplerPropagator tudat_astrodynamics tudat_input tudat_output tudat_mathematics tudat_basics ${TUDAT_CORE_LIBRARIES} ${Boost_LIBRARIES})

# End of file.
