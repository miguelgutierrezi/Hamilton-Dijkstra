#ifndef __PUJ__Graph__h__
#define __PUJ__Graph__h__

#include <map>
#include <vector>
#include <fstream>
#include <algorithm>

namespace PUJ
{



  template< class _TVertex, class _TEdge >
  class Graph
  {
	  struct TDijkstraNode
	  {
		  unsigned long Vertex;
		  unsigned long Parent;
		  _TEdge Cost;
	  };
	  struct TDijkstraComparator
	  {
		  bool operator() (TDijkstraNode lhs, TDijkstraNode rhs) const
		  {
			  return lhs.Cost < rhs.Cost;//reisar const
		  }
	  };
  public:
    typedef std::vector< _TVertex > TVertices;
    typedef std::map< unsigned long, _TEdge > TRow;
    typedef std::map< unsigned long, TRow >   TMatrix;
	typedef std::vector<_TEdge> TMinimumSpanningTree;

  public:
    Graph( );
    virtual ~Graph( );
    void clearG( );
	std::vector<_TEdge> Dijkstra( unsigned long & start);

    long AddVertex( const _TVertex& v );
    long AddEdge( unsigned long a, unsigned long b, const _TEdge& cost );
    void Draw( const std::string& filename );
	void generarArchivo(std::ofstream& fs);
	_TEdge buscarDato(const _TEdge& dato,std::vector<_TEdge>* tree);
	_TEdge camino(const _TEdge& dato,std::vector<_TEdge>* tree);
	void red_social(unsigned long & v1,unsigned long & v2);
  protected:
    TVertices m_Vertices;
    TMatrix   m_Edges;
  };
}

#include "Graph.hxx"

#endif // __PUJ__Graph__h__
