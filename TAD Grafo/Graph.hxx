#ifndef __PUJ__Graph__hxx__
#define __PUJ__Graph__hxx__
#include"Punto.h"
#include "Graph.h"


template< class _TVertex, class _TEdge >
PUJ::Graph< _TVertex, _TEdge >::
Graph( )
{
}

template< class _TVertex, class _TEdge >
PUJ::Graph< _TVertex, _TEdge >::
 ~Graph( )
{
}

template< class _TVertex, class _TEdge >
long PUJ::Graph< _TVertex, _TEdge >::
AddVertex( const _TVertex& v )
{
  this->m_Vertices.push_back( v );
  return( this->m_Vertices.size( ) - 1 );
}

template< class _TVertex, class _TEdge >
long PUJ::Graph< _TVertex, _TEdge >::
AddEdge( unsigned long a, unsigned long b, const _TEdge& cost )
{
  this->m_Edges[ a ][ b ] = cost;
  return 0;
}

template< class _TVertex, class _TEdge >
void PUJ::Graph< _TVertex, _TEdge >::
Draw( const std::string& filename )
{
  typename TMatrix::iterator mIt = this->m_Edges.begin( );
  for( ; mIt != this->m_Edges.end( ); ++mIt )
  {
    typename TRow::iterator rIt = mIt->second.begin( );
    for( ; rIt != mIt->second.end( ); ++rIt )
      std::cout
        << "(" << this->m_Vertices[mIt->first].getPx()<<","<<this->m_Vertices[mIt->first].getPy()
        << "->" << this->m_Vertices[rIt->first].getPx()<<","<<this->m_Vertices[rIt->first].getPy()
        << ") = " << rIt->second << std::endl;

  } // rof
}
template<class _TVertex, class _TEdge>
 void PUJ::Graph<_TVertex, _TEdge>::generarArchivo(std::ofstream& fs)
{
	  unsigned long a = 0;
	 std::vector<_TEdge> orden= Dijkstra(a);

	 fs << this->m_Vertices.size() << std::endl;
	 for (int i = 0; i < this->m_Vertices.size(); i++)
	 {
		 Punto p;
		 p.setPx(this->m_Vertices[orden[i] ].getPx() );
		 p.setPy(this->m_Vertices[orden[i] ].getPy() );
		 fs << p.getPx()<<" "<<p.getPy()<< std::endl;
	 }

}
/// limpia el grafo
template< class _TVertex, class _TEdge >
void PUJ::Graph< _TVertex, _TEdge >::clearG( )
{
    this-> m_Vertices.clear( );
    this-> m_Edges.clear( );
}


template <class _TVertex, class _TEdge>
std::vector<_TEdge> PUJ::Graph<_TVertex,_TEdge>::
Dijkstra(unsigned long& start)
{
	/* crea un vector de marcas iniciandolas en false
	  su funcion es saber por cuales nodos se ha pasado
	*/
	std::vector< bool > marks(this->m_Vertices.size(), false);
	/**/
	std::vector< TDijkstraNode > q;
	/*
	vector
	*/
	std::vector<_TEdge> tree(this->m_Vertices.size(), -1);

	TDijkstraNode n;
	n.Vertex = start;
	n.Parent = start;
	n.Cost = 0;
	q.push_back(n);

	while (q.size() > 0)
	{
		// ordena menor a mayor
		std::pop_heap(q.begin(), q.end(), TDijkstraComparator());
		// se saca el menor
		n = q.back();
		// quita la raiz
		q.pop_back();

		if (marks[n.Vertex])
			continue;
		marks[n.Vertex] = true;
		tree[n.Vertex] = n.Parent;

		typename TMatrix::const_iterator mIt = this->m_Edges.find(n.Vertex);
		for (
			typename TRow::const_iterator rIt = mIt->second.begin();
			rIt != mIt->second.end();
			++rIt
			)
		{
			TDijkstraNode m;
			m.Parent = n.Vertex;
			m.Vertex = rIt->first;
			m.Cost = n.Cost + rIt->second;
			q.push_back(m);
			std::push_heap(q.begin(), q.end(), TDijkstraComparator());
		}//fin for agregar conexiones a q
	}//fin while de q
	std::vector<_TEdge> aux;
	long bb;
  aux.push_back(tree[0]);
  for (int i = 0; i < tree.size(); i++)
  {
	  bb=buscarDato(aux.back(),&tree);
	  aux.push_back(bb);
  }
  return aux;
}


template <class _TVertex, class _TEdge>
_TEdge PUJ::Graph< _TVertex, _TEdge >::
buscarDato(const _TEdge& dato, std::vector<_TEdge> *tree)
{
	for (int i = 1; i < tree->size(); i++)
		if ((*tree)[i] == dato)
			return i;
}

template <class _TVertex, class _TEdge>
void PUJ::Graph< _TVertex, _TEdge >::
red_social(unsigned long& v1,unsigned long& v2)
{
    std::vector<_TEdge> orden= Dijkstra(v1);
    long ret = v2 , aux;
    std::vector<_TEdge> vec;
    for(int i=0 ; i < orden.size();i++)
    {

        aux=camino(ret , &orden);
        if(ret==aux)
            continue;
        vec.push_back( aux );
        ret = aux;
    }

    for(int j =0;j<vec.size();j++)
    {
        std::cout<<this->m_Vertices[orden[j] ].getPx()<<",";
        std::cout<<this->m_Vertices[orden[j] ].getPy()<<'\n';
    }


}

template <class _TVertex, class _TEdge>
_TEdge PUJ::Graph< _TVertex, _TEdge >::
camino(const _TEdge& dato, std::vector<_TEdge> *tree)
{
	for (int i = 1; i < tree->size(); i++)
		if ( i == dato )
			return (*tree)[i];
}


#endif // __PUJ__Graph__hxx__

