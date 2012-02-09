# \file CMakeLists.txt
 #    This is the CMake file for the Astrodynamics sub-directory.
 #
 #    Path              : /Astrodynamics/
 #    Version           : 4
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
 #    Last modified     : 2 February, 2012
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
 #      120202    K. Kumar          Removed astrodynamicsFunctions.h/.cpp and unit tests.
 #/

# Define the main sub-directories.
set(AERODYNAMICSDIR "${ASTRODYNAMICSDIR}/Aerodynamics")
set(GRAVITATIONDIR "${ASTRODYNAMICSDIR}/Gravitation")
set(BASICASTRODYNAMICSDIR "${ASTRODYNAMICSDIR}/BasicAstrodynamics")
set(BODIESDIR "${ASTRODYNAMICSDIR}/Bodies")
set(MISSIONSEGMENTSDIR "${ASTRODYNAMICSDIR}/MissionSegments")
set(REFERENCEFRAMESDIR "${ASTRODYNAMICSDIR}/ReferenceFrames")
set(STATESDIR "${ASTRODYNAMICSDIR}/States")

# Add subdirectories.
add_subdirectory("${SRCROOT}${BASICASTRODYNAMICSDIR}")
add_subdirectory("${SRCROOT}${BODIESDIR}")
add_subdirectory("${SRCROOT}${MISSIONSEGMENTSDIR}")
add_subdirectory("${SRCROOT}${REFERENCEFRAMESDIR}")
add_subdirectory("${SRCROOT}${STATESDIR}")
add_subdirectory("${SRCROOT}${AERODYNAMICSDIR}")
add_subdirectory("${SRCROOT}${GRAVITATIONDIR}")

# Get target properties for static libraries.
get_target_property(BASICASTRODYNAMICSSOURCES tudat_basic_astrodynamics SOURCES)
get_target_property(BODIESSOURCES tudat_bodies SOURCES)
get_target_property(AERODYNAMICSSOURCES tudat_aerodynamics SOURCES)
get_target_property(GRAVITATIONSOURCES tudat_gravitation SOURCES)
get_target_property(MISSIONSEGMENTSSOURCES tudat_mission_segments SOURCES)
get_target_property(REFERENCEFRAMESSOURCES tudat_reference_frames SOURCES)
get_target_property(STATESSOURCES tudat_states SOURCES)

# End of file.
