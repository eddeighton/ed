

#include "common/assert_verify.hpp"
#include "common/string.hpp"
#include "common/file.hpp"

#include "ed/ed.hpp"

#include <boost/program_options.hpp>

#include <iostream>
#include <regex>

int main(int argc, char* argv[])
{
    std::vector< std::string > inputFiles, edgeTypes;
    std::string strTargetDirectory, strGraph, strStates, strFilter;
    bool bExpandShorthand = false, bRemoveTypes = false;
    {
        namespace po = boost::program_options;
        po::variables_map vm;
        try
        {
            po::options_description desc("Allowed options");
            desc.add_options()
                ("help", "produce help message")
                ("file", po::value< std::vector< std::string > >( &inputFiles ), "Input .ed File")
                ("graph", po::value<std::string>( &strGraph ), "GraphVis Dot File Path")
                ("edges", po::value< std::vector< std::string > >( &edgeTypes ), "Edge Types" )
                ("states", po::value<std::string>( &strStates ), "GraphVis Dot File Path for states")
                ("filter", po::value< std::string>( &strFilter ), "Regular expression filter" )
                ("expand", po::value< bool >( &bExpandShorthand ), "Expand shorthand" )
                ("prune", po::value< bool >( &bRemoveTypes ), "Prune Types" )
            ;

            po::positional_options_description p;
            p.add( "file", -1 );

            po::store( po::command_line_parser( argc, argv).
                        options( desc ).
                        positional( p ).run(),
                        vm );
            po::notify(vm);

            if (vm.count("help"))
            {
                std::cout << desc << "\n";
                return 1;
            }
        }
        catch( std::exception& )
        {
            std::cout << "Invalid input. Type '--help' for options" << "\n";
            return 1;
        }
    }

    if( !inputFiles.empty() )
    {
        Ed::BasicFileSystem filesystem;
        
        for( std::vector< std::string >::const_iterator
            j = inputFiles.begin(), jEnd = inputFiles.end();
            j!=jEnd; ++j )
        {
            Ed::File::Ptr pFile;
            try
            {
                pFile.reset( new Ed::File( filesystem, *j ) );

                if( bRemoveTypes )
                    pFile->removeTypes();

                if( bExpandShorthand )
                    pFile->expandShorthand();
            }
            catch( std::runtime_error& e )
            {
                std::cout << "Could not load file: " << *j << " exception: " << e.what();
                continue;
            }
            if( pFile )
            {
                try
                {
                    Ed::Node nResult;
                    if( !strFilter.empty() )
                    {
                        std::regex reg( strFilter );
                        pFile->toNode( nResult, reg );
                    }
                    else
                    {
                        pFile->toNode( nResult );
                    }
                    std::cout << nResult << "\n";
                }
                catch( std::runtime_error& e )
                {
                    std::cout << "Error: " << e.what() << "\n";
                }
            }
        }
    }
    else
    {
        std::cout << "No input ed files specified.  Type '--help' for options" << "\n";
    }

    return 0;
}
