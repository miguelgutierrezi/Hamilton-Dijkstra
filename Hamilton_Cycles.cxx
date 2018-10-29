#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <random>
#include <set>
#include <tuple>
#include <utility>
#include <vector>

// ------------------------------------------------------------------------
class Point
{
  friend std::ostream& operator<<( std::ostream& o, const Point& p )
  {
    o << "[" << p.X << ", " << p.Y << "]";
    return( o );
  }
public:
  Point( )
    : X( 0 ),
      Y( 0 )
  {
    if( Point::Device == NULL )
      Point::Device = new std::random_device( );
    if( Point::Generator == NULL )
      Point::Generator = new std::mt19937( ( *Point::Device )( ) );
    if( Point::Distribution == NULL )
      Point::Distribution = new std::uniform_real_distribution<>( -1000, 1000 );
  }

  virtual ~Point( )
  {
  }

  void RandomFill( )
  {
    this->X = ( *Point::Distribution )( *Point::Generator );
    this->Y = ( *Point::Distribution )( *Point::Generator );
  }

  bool operator<( const Point& b ) const
  {
    if( this->X == b.X )
      return( this->Y < b.Y );
    else
      return( this->X < b.X );
  }

  double DistanceTo( const Point& b ) const
  {
    double d = ( this->X - b.X ) * ( this->X - b.X );
    d       += ( this->Y - b.Y ) * ( this->Y - b.Y );
    return( std::sqrt( d ) );
  }

protected:
  double X;
  double Y;
  static std::random_device* Device;
  static std::mt19937* Generator;
  static std::uniform_real_distribution<>* Distribution;
};

std::random_device* Point::Device = NULL;
std::mt19937* Point::Generator = NULL;
std::uniform_real_distribution<>* Point::Distribution = NULL;

// ------------------------------------------------------------------------
typedef std::set< Point >                TPointSet;
typedef std::vector< Point >             TPoints;
typedef std::multimap< double, TPoints > TPointMap;
typedef std::pair< double, TPoints >     THamiltonLoop;
typedef std::vector< long >              TMST;

// ------------------------------------------------------------------------
THamiltonLoop Hamilton_BF( const TPoints& points );
THamiltonLoop Hamilton_Dijkstra( const TPoints& points );

// ------------------------------------------------------------------------
TMST Dijkstra( const TPoints& points, long seed );

// ------------------------------------------------------------------------
int main( int argc, char* argv[] )
{
  TPointSet point_set;
  while( point_set.size( ) < 1000 )
  {
    Point p;
    p.RandomFill( );
    point_set.insert( p );
  } // end while
  TPoints points( point_set.begin( ), point_set.end( ) );

  /* TODO
  for( const Point& p: points )
    std::cout << p << std::endl;

  THamiltonLoop hl_bf = Hamilton_BF( points );
  std::cout << "-------------------------------" << std::endl;
  std::cout << "Cost: " << hl_bf.first << std::endl;
  std::cout << "Loop:";
  for( const Point& p: hl_bf.second )
    std::cout << " " << p;
  std::cout << std::endl << "-------------------------------" << std::endl;
  */

  THamiltonLoop hl_dijk = Hamilton_Dijkstra( points );
  std::cout << "-------------------------------" << std::endl;
  std::cout << "Cost: " << hl_dijk.first << std::endl;
  std::cout << "Loop:";
  for( const Point& p: hl_dijk.second )
    std::cout << " " << p;
  std::cout << std::endl << "-------------------------------" << std::endl;


  return( 0 );
}

// ------------------------------------------------------------------------
THamiltonLoop Hamilton_BF( const TPoints& points )
{
  TPoints perm( points.begin( ), points.end( ) );
  TPointMap all_options;
  while( std::next_permutation( perm.begin( ), perm.end( ) ) )
  {
    double c = 0;
    for( unsigned int i = 0; i < perm.size( ); ++i )
      c += perm[ i ].DistanceTo( perm[ ( i + 1 ) % perm.size( ) ] );
    all_options.insert( THamiltonLoop( c, perm ) );
  } // end while
  return( *( all_options.begin( ) ) );
}

// ------------------------------------------------------------------------
THamiltonLoop Hamilton_Dijkstra( const TPoints& points )
{
  std::random_device dev;
  std::mt19937 gen( dev( ) );
  std::uniform_int_distribution<> dist( 0, points.size( ) - 1 );
  long seed = dist( gen );
  TMST mst = Dijkstra( points, seed );
  // PreOrder( mst );
  return( THamiltonLoop( 0, TPoints( ) ) );
}

// ------------------------------------------------------------------------
TMST Dijkstra( const TPoints& points, long seed )
{
  typedef std::tuple< long, long, double > _TNode;
  struct
  {
    bool operator()( const _TNode& a, const _TNode& b ) const
    {
      return( std::get< 2 >( b ) < std::get< 2 >( a ) );
    }
  } cmp;

  TMST mst( points.size( ), -1 );
  std::vector< bool > marks( points.size( ), false );
  _TNode s( seed, seed, 0 );
  std::vector< _TNode > q;
  q.push_back( s );
  while( q.size( ) > 0 )
  {
    std::pop_heap( q.begin( ), q.end( ), cmp );
    _TNode n = q.back( );
    q.pop_back( );
    long v = std::get< 0 >( n );
    long p = std::get< 1 >( n );
    double c = std::get< 2 >( n );
    if( !marks[ v ] )
    {
      marks[ v ] = true;
      mst[ v ] = p;
      for( long i = 0; i < points.size( ); ++i )
      {
        q.push_back( _TNode( i, v, c + points[ v ].DistanceTo( points[ i ] ) ) );
        std::push_heap( q.begin( ), q.end( ), cmp );
      } // end for
    } // end if
  } // end while
  return( mst );
}

// eof - Hamilton_Cycles.cxx