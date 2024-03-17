#include "ed/code.hpp"

namespace Ed
{

ParseResult parse( const std::string& strInput, Code& code, std::ostream& errorStream )
{
    return parse_impl< CodeGrammar >( strInput, code, errorStream );
}

const unsigned int TOTAL_CODE_REF_DELIMITERS                         = 6u;
const char*        pszCodeRefDelimiters[ TOTAL_CODE_REF_DELIMITERS ] = { "(.", "{.", "[.", " .", "\t.", "\n." };

void extractCodeReferences( const std::string& strCode, CodeReferenceVector& references )
{
    std::ostringstream          errorStream; // dummy
    std::string::const_iterator i = strCode.begin(), iNext = strCode.begin(), iEnd = strCode.end();

    // if very first char is . then attempt parse
    if( !strCode.empty() && *strCode.begin() == '.' )
    {
        CodeReference codeRef;
        std::string   s( i, iEnd );
        ParseResult   result = parse( s, codeRef.ref, errorStream );
        if( result.first )
        {
            codeRef.szStart = 0u;
            codeRef.szEnd   = static_cast< std::size_t >( result.second.base() - s.begin() );
            references.push_back( codeRef );
            i = strCode.begin() + static_cast< int >( codeRef.szEnd );
            if( i == iEnd )
                return;
            iNext = i;
        }
    }

    for( ; i != iEnd; ++i )
    {
        ++iNext;
        if( iNext != iEnd )
        {
            for( auto& pszCodeRefDelimiter : pszCodeRefDelimiters )
            {
                if( ( pszCodeRefDelimiter[ 0u ] == *i ) && ( pszCodeRefDelimiter[ 1u ] == *iNext ) )
                {
                    // found candidate...
                    CodeReference codeRef;
                    std::string   s( iNext, iEnd );
                    ParseResult   result = parse( s, codeRef.ref, errorStream );
                    if( result.first )
                    {
                        codeRef.szStart = static_cast< std::size_t >( iNext - strCode.begin() );
                        codeRef.szEnd   = static_cast< std::size_t >( ( iNext - strCode.begin() )
                                                                    + ( result.second.base() - s.begin() ) );
                        references.push_back( codeRef );
                        i = strCode.begin() + static_cast< int >( codeRef.szEnd );
                        if( i == iEnd )
                            return;
                        iNext = i + 1;
                    }
                }
            }
        }
    }
}

bool operator==( const Code& left, const Code& right )
{
    return left.code == right.code;
}

} // namespace Ed