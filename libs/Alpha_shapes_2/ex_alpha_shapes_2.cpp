#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/algorithm.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Alpha_shape_2.h>

#include <iostream>
#include <fstream>
#include <vector>
#include <list>

using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;

typedef K::FT FT;
typedef K::Point_2  Point;
typedef K::Segment_2  Segment;

typedef CGAL::Alpha_shape_vertex_base_2<K> Vb;
typedef CGAL::Alpha_shape_face_base_2<K>  Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds> Triangulation_2;

typedef CGAL::Alpha_shape_2<Triangulation_2>  Alpha_shape_2;
typedef Alpha_shape_2::Alpha_shape_edges_iterator Alpha_shape_edges_iterator;

typedef Alpha_shape_2::Alpha_iterator               Alpha_iterator;


template <class OutputIterator>
void
alpha_edges( const Alpha_shape_2&  A,
	     OutputIterator out)
{
  //int count=0;
  for(Alpha_shape_edges_iterator it =  A.alpha_shape_edges_begin();
      it != A.alpha_shape_edges_end();
      ++it){
  //  count++;
  // if(A.classify(*it)!=2) continue;
     *out++ = A.segment(*it);
    cout<<A.classify(*it)<<endl;
  }
//cout<<count<<endl;
}
// Reads a list of points and returns a list of segments
// corresponding to the Alpha shape.
int main(int argc, char **argv)
{
  std::list<Point> points;
  std::ifstream is(strdup(argv[1]));
  int n;
  is >> n;
  int nComponent = n;
  for(;n>0;n--)
  {
    double pp[3];
    is>>pp[0]>>pp[1]>>pp[2];
    Point p (pp[0],pp[1]);
    //is>>p;
    points.push_back(p);
  }
  //nComponent=1;

  Alpha_shape_2 A(points.begin(), points.end(),
		  FT(10000),
	  Alpha_shape_2::REGULARIZED);//GENERAL);
  Alpha_iterator alpha_solid =  A.find_optimal_alpha(nComponent);
  A.set_alpha(*alpha_solid);

  std::vector<Segment> segments;
  alpha_edges(A, std::back_inserter(segments));

  std::cout << "Alpha Shape computed" << std::endl;
  std::cout << segments.size() << " alpha shape edges" << std::endl;
  std::cout << "Optimal alpha: " <<*alpha_solid<<" "<<A.number_of_solid_components(*alpha_solid)<< std::endl;

  std::size_t nbe = segments.size();
  cout<<nbe*2<<endl;
  for(std::size_t i=0;i<nbe;i++)
  {
    cout<<segments[i].vertex(0)<<endl;
    cout<<segments[i].vertex(1)<<endl;  

  }
  return 0;
}
