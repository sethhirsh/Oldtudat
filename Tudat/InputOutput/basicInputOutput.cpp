#include <vector>
#include "Tudat/InputOutput/basicInputOutput.h"

//! Tudat library namespace.
/*!
 * The Tudat Library namespace.
 */
namespace tudat
{

//! The input/output namespace.
/*!
 * The input/output namespace.
 */
namespace input_output
{

//! Get root-path for Tudat Package library.
std::string getPackageRootPath( )
{
#ifdef TUDAT_CUSTOM_ROOT_PATH
    return std::string( TUDAT_CUSTOM_ROOT_PATH );
#else
    // Declare file path string assigned to filePath.
    std::string filePath_( __FILE__ );

    // Strip filename from temporary string and return root-path string.
    return filePath_.substr( 0, filePath_.length( ) -
                                std::string( "InputOutput/basicInputOutput.cpp" ).length( ) );
#endif
}

//! Lists all files in directory.
std::vector< boost::filesystem3::path > listAllFilesInDirectory(
    const boost::filesystem3::path& directory, bool isRecurseIntoSubdirectories )
{
    // Declare local variables.
    std::vector < boost::filesystem3::path > listOfFileNamesWithPath_;

    if ( boost::filesystem3::exists( directory ) )
    {
        boost::filesystem3::directory_iterator iteratorPastEndOfDirectory_;

        for ( boost::filesystem3::directory_iterator directoryIterator_( directory );
              directoryIterator_ != iteratorPastEndOfDirectory_ ; ++directoryIterator_ )
        {
            if ( boost::filesystem3::is_directory( *directoryIterator_ ) )
            {
                if ( isRecurseIntoSubdirectories )
                {
                    listAllFilesInDirectory( *directoryIterator_ );
                }
            }

            else
            {
                listOfFileNamesWithPath_.push_back( directoryIterator_->path( ).filename( ) );
            }
        }
    }

    // Return container of filenames.
    return listOfFileNamesWithPath_;
}


} // namespace input_output

} // namespace tudat
