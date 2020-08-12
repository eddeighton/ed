#ifndef PARSER_25_12_2013
#define PARSER_25_12_2013

#include "skip.hpp"

#include "common/assert_verify.hpp"

#include <boost/spirit/home/support/iterators/line_pos_iterator.hpp>
#include <boost/spirit/include/support_line_pos_iterator.hpp>

#include <string>
#include <sstream>

namespace Ed
{

typedef std::string::const_iterator UnderlyingIterType;
typedef boost::spirit::line_pos_iterator< UnderlyingIterType > IteratorType;
typedef std::pair< bool, IteratorType > ParseResult;

template <typename Iterator>
struct error_handler
{
    template <typename, typename, typename, typename>
    struct result { typedef void type; };

    std::ostream& errorStream;
    error_handler( std::ostream& _errorStream )
        : errorStream( _errorStream )
    {
    }

    void operator()( Iterator first, Iterator last, Iterator err_pos, boost::spirit::info const& what ) const
    {
        Iterator eol = err_pos;
        //int line = calculateLineNumber( first, err_pos );
        int line = boost::spirit::get_line( err_pos );
        if (line != -1)
            errorStream << '(' << line << ')';
        else
            errorStream << "( unknown )";
        errorStream << " : Error! Expected " << what << std::endl;
    }
};

template< template< typename IterType > class GrammarType, typename ResultType >
ParseResult parse_impl( const std::string& strInput, ResultType& result, std::ostream& errorStream )
{
    GrammarType< IteratorType > grammar;
    {
        using namespace boost::phoenix;
        function< error_handler< IteratorType > > const error_handler( errorStream );
        boost::spirit::qi::on_error< boost::spirit::qi::fail >(
            grammar.m_main_rule,
            error_handler(
                boost::spirit::qi::_1,
                boost::spirit::qi::_2,
                boost::spirit::qi::_3,
                boost::spirit::qi::_4 ) );
    }

    ParseResult resultPair( true, IteratorType( strInput.begin() ) );

    resultPair.first = boost::spirit::qi::phrase_parse(
        resultPair.second,
        IteratorType( strInput.end() ),
        grammar,
        SkipGrammar< IteratorType >(),
        result );

    return resultPair;
}

template< class GrammarType, class IteratorType, typename ResultType >
ParseResult parse_impl( const std::string& strInput, ResultType& result, std::ostream& errorStream )
{
    GrammarType grammar;
    {
        using namespace boost::phoenix;
        function< error_handler< IteratorType > > const error_handler( errorStream );
        boost::spirit::qi::on_error< boost::spirit::qi::fail >(
            grammar.m_main_rule,
            error_handler(
                boost::spirit::qi::_1,
                boost::spirit::qi::_2,
                boost::spirit::qi::_3,
                boost::spirit::qi::_4 ) );
    }

    ParseResult resultPair( true, IteratorType( strInput.begin() ) );

    resultPair.first = boost::spirit::qi::phrase_parse(
        resultPair.second,
        IteratorType( strInput.end() ),
        grammar,
        SkipGrammar< IteratorType >(),
        result );

    return resultPair;
}

template< class T >
ParseResult parse( const std::string& strInput, T& resultValue )
{
    std::ostringstream osError;
    return parse( strInput, resultValue, osError );
}

template< class T >
T parse( const std::string& strInput, std::ostream& osError )
{
	T resultType;
	const ParseResult result = parse( strInput, resultType, osError );
	VERIFY_RTE_MSG( result.first && result.second.base() == strInput.end(), "Failed to parse string: " << strInput );
	return resultType;
}

template< class T >
T parse( const std::string& strInput )
{
    std::ostringstream osError;
    T resultType;
    const ParseResult result = parse( strInput, resultType, osError );
    VERIFY_RTE_MSG( result.first && result.second.base() == strInput.end(), "Failed to parse string: " << strInput << " : " << osError.str() );
    return resultType;
}

}

#endif