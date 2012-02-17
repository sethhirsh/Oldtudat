# \file MakeDistribution.cmake
 #    This file defines the install target to create a distribution of Tudat
 #
 #    Copyright (c) 2012 Delft University of Technology.
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
 #      120217    B. Tong Minh      File created
 #/

if(NOT TUDAT_DISTRIBUTION_PATH)
    set(TUDAT_DISTRIBUTION_PATH "${CODEROOT}/dist/packages")
endif(NOT TUDAT_DISTRIBUTION_PATH)


# Install the Tudat files
install(DIRECTORY "${SRCROOT}/"
        DESTINATION "${TUDAT_DISTRIBUTION_PATH}/Tudat"
        FILES_MATCHING
            PATTERN "*.*"
            PATTERN "CMakeLists.txt.user" EXCLUDE
)

# Installation of the compiled libraries is defined in the setup_tudat_library_target() macro
