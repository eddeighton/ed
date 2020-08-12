#ifndef ARGUMENT_30_03_2014
#define ARGUMENT_30_03_2014

#include "fileRef.hpp"
#include "ref.hpp"
#include "code.hpp"
#include "expression.hpp"
#include "skip.hpp"
#include "parser.hpp"

#include "common/variant_utils.hpp"

#include <boost/variant.hpp>
#include <boost/spirit/include/qi_eoi.hpp>

#include <string>
#include <sstream>

namespace Ed
{

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
//Arguments
typedef boost::variant< FileRef, Ref, Code, Expression, double, bool > ArgumentVariant;

static const char SH_OPEN = '(';
static const char SH_CLOSE = ')';
static const char SH_DELIMITER = ',';

template< typename Iterator >
class ArgumentGrammar : public boost::spirit::qi::grammar< Iterator, SkipGrammar< Iterator >, ArgumentVariant() >
{
public:
    ArgumentGrammar() : ArgumentGrammar::base_type( m_main_rule, "argument" )
    {
        using namespace boost::spirit;
        using namespace boost::spirit::qi;
        using boost::spirit::qi::eoi;
        using namespace boost::phoenix;

        m_main_rule =    (
                                            (m_grammar_fileref       ) |
                                            (m_grammar_code          ) |
                                            (m_grammar_ref            >> &( lit( SH_DELIMITER ) | lit( SH_CLOSE ) | eoi ) ) |
                                            (boost::spirit::double_   >> &( lit( SH_DELIMITER ) | lit( SH_CLOSE ) | eoi ) ) |
                                            (boost::spirit::bool_     >> &( lit( SH_DELIMITER ) | lit( SH_CLOSE ) | eoi ) ) |
                                            (m_grammar_expression    )
                                    )[ _val = qi::_1 ];
    }

    FileRefGrammar< Iterator >       m_grammar_fileref;
    RefGrammar< Iterator >           m_grammar_ref;
    CodeGrammar< Iterator >          m_grammar_code;
    ExpressionGrammar< Iterator >    m_grammar_expression;

    boost::spirit::qi::rule< Iterator, SkipGrammar< Iterator >, ArgumentVariant() >     m_main_rule;
};

ParseResult parse( const std::string& strInput, ArgumentVariant& argument, std::ostream& errorStream );

void propagate( Ref& ref );
void propagate( ArgumentVariant& argument );

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//meta functions

//anything else convertible to a double
template< bool, bool >
struct eds_onlyfirst : std::false_type {};

template<>
struct eds_onlyfirst< true, false > : std::true_type {};

template< bool, bool >
struct ed_neither : std::false_type {};

template<>
struct ed_neither< false, false > : std::true_type {};


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//ed_cast
//anything arithmetic other than bool converts to double
template< class T >
inline typename std::enable_if<
        eds_onlyfirst<  std::is_arithmetic< T >::value,
                        std::is_same< T, bool >::value >::value,
        ArgumentVariant >::type
    ed_cast( const T& from )
{
    return ArgumentVariant( static_cast< double >( from ) );
}

//bool
template< class T >
inline typename std::enable_if<
        std::is_same< T, bool >::value,
        ArgumentVariant >::type
    ed_cast( const T& from )
{
    return ArgumentVariant( from );
}

//anything that parses to an argument
template< class T >
inline typename std::enable_if<
        eds_onlyfirst<  std::is_convertible< T, std::string >::value,
                                std::is_same< T, Expression >::value >::value,
        ArgumentVariant >::type
    ed_cast( const T& data )
{
    std::string str( data );
	std::ostringstream osError;
	boost::optional< ArgumentVariant > result = Ed::parse< ArgumentVariant >( str, osError );
    VERIFY_RTE_MSG( result, "Failed to parse argument: " << str << " : " << osError.str() );
    return result.get();
}

//anything else convertible
template< class T >
inline typename std::enable_if<
        ed_neither<     std::is_arithmetic< T >::value,
                        eds_onlyfirst<  std::is_convertible< T, std::string >::value,
                                                std::is_same< T, Expression >::value >::value >::value,
        ArgumentVariant >::type
    ed_cast( const T& from )
{
    return ArgumentVariant( from );
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//ed_convert

//anything arithmetic other than bool converts from double
template< class T >
inline typename std::enable_if<
        eds_onlyfirst<  std::is_arithmetic< T >::value,
                        std::is_same< T, bool >::value >::value,
        T >::type
    ed_convert( const ArgumentVariant& from, T& value )
{
    boost::optional< const double& > opt =
        boost::apply_visitor( boost::TypeAccessor< const double >(), from );
    VERIFY_RTE( opt );
    return value = static_cast< T >( opt.get() );
}

//bool
template< class T >
inline typename std::enable_if<
        std::is_same< T, bool >::value,
        T >::type
    ed_convert( const ArgumentVariant& from, T& value )
{
    boost::optional< const bool& > opt =
        boost::apply_visitor( boost::TypeAccessor< const bool >(), from );
    VERIFY_RTE( opt );
    return value = opt.get();
}

//anything that parses to an argument
template< class T >
inline typename std::enable_if<
        eds_onlyfirst<  std::is_convertible< T, std::string >::value,
                                std::is_same< T, Expression >::value >::value,
        T >::type
    ed_convert( const ArgumentVariant& from, T& value )
{
    std::ostringstream os;
    os << from;
    return value = os.str();
}

//anything else convertible
template< class T >
inline typename std::enable_if<
        ed_neither<     std::is_arithmetic< T >::value,
                        eds_onlyfirst<  std::is_convertible< T, std::string >::value,
                                                std::is_same< T, Expression >::value >::value >::value,
        T >::type
    ed_convert( const ArgumentVariant& from, T& value )
{
    boost::optional< const T& > opt =
        boost::apply_visitor( boost::TypeAccessor< const T >(), from );
    VERIFY_RTE( opt );
    return value = opt.get();
}

}

#endif //ARGUMENT_30_03_2014