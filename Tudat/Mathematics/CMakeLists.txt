# \file CMakeLists.txt
 #    This is the CMake file for the Mathematics sub-directory.
 #
 #    Path              : /Mathematics/
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
 #    Last modified     : 27 October, 2011
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
 #      111027    K. Kumar          Removed RandomNumberGenerators as sub-directory.
 #/

# Add sub-directories.
add_subdirectory("${SRCROOT}${MATHEMATICSDIR}/GeometricShapes")
add_subdirectory("${SRCROOT}${MATHEMATICSDIR}/Interpolators")
add_subdirectory("${SRCROOT}${MATHEMATICSDIR}/NumericalIntegrators")
add_subdirectory("${SRCROOT}${MATHEMATICSDIR}/RootFindingMethods")
add_subdirectory("${SRCROOT}${MATHEMATICSDIR}/Statistics")

# Get target properties for static libraries.
get_target_property(GEOMETRICSHAPESSOURCES tudat_geometric_shapes SOURCES)
get_target_property(INTERPOLATORSSOURCES tudat_interpolators SOURCES)
get_target_property(LINEARALGEBRASOURCES tudat_linear_algebra SOURCES)
get_target_property(NUMERICALINTEGRATORSSOURCES tudat_numerical_integrators SOURCES)
get_target_property(ROOTFINDINGMETHODSSOURCES tudat_root_finding_methods SOURCES)
get_target_property(STATISTICSSOURCES tudat_statistics SOURCES)



# End of file.
