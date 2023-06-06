#ifndef SKIP_25_12_2013
#define SKIP_25_12_2013

#include <boost/spirit/include/qi.hpp>

#include <boost/phoenix/core.hpp>
#include <boost/phoenix/stl.hpp>

namespace Ed
{

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
//Skip grammar for handling c++ style comments and white space
template< typename Iterator >
class SkipGrammar : public boost::spirit::qi::grammar< Iterator >
{
public:
    SkipGrammar() : SkipGrammar::base_type( start )
    {
        using namespace boost::spirit;
        using namespace boost::spirit::qi;
        using namespace boost::phoenix;
        start =   boost::spirit::ascii::space
                        |   "//" >> *( char_ - '\n') >> '\n'     // C++ comment
                        |   "/*" >> *( char_ - "*/") >> "*/";     // C comment
    }

private:
    boost::spirit::qi::rule< Iterator > start;
};

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
//ALL grammar to help testing skip grammar
template< typename Iterator >
class AllGrammar : public boost::spirit::qi::grammar< Iterator, SkipGrammar< Iterator >, std::string() >
{
public:
    AllGrammar() : AllGrammar::base_type( m_main_rule, "all" )
    {
        using namespace boost::spirit;
        using namespace boost::spirit::qi;
        using namespace boost::phoenix;
        m_main_rule = *(char_[ push_back( _val, qi::_1 ) ]); 
    }

    boost::spirit::qi::rule< Iterator, SkipGrammar< Iterator >, std::string() > m_main_rule;
};

void strip( const std::string& strInput, std::string& strOutput );

}

#endif //SKIP_25_12_2013