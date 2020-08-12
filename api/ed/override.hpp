#ifndef OVERRIDEANDEXTEND_28_12_2013
#define OVERRIDEANDEXTEND_28_12_2013

#include <boost/optional.hpp>

#include <list>

namespace Ed
{

class Declarator;
class Shorthand;
class TagList;
class Statement;
class EdNode;

void overrideAndExtend( const Declarator& origin, Declarator& overrider );
void overrideAndExtend( const boost::optional< Shorthand >& origin, boost::optional< Shorthand >& overrider );
void overrideAndExtend( const boost::optional< TagList >& origin, boost::optional< TagList >& overrider );
void overrideAndExtend( const Statement& origin, Statement& overrider );
void overrideAndExtend( const EdNode& origin, EdNode& overrider );

}


#endif //OVERRIDEANDEXTEND_28_12_2013