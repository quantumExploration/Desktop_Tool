#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/convex_hull_2.h>

#include <vector>
#include <fstream>
#include <string.h>
#include <cstdlib>

using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 Point_2;
typedef std::vector<Point_2> Points;

int main(int argc, char **argv)
{
  Points points, result;

  ifstream infile(strdup(argv[1]));
  int n;
  infile>>n;
  for(int i=0;i<n;i++){
    float p[3];
    infile>>p[0]>>p[1]>>p[2];
    Point_2 pp(p[0], p[1]);
    points.push_back(pp);
  }
  
  infile.close();

  //cout<<points.size()<<endl;

  CGAL::convex_hull_2( points.begin(), points.end(), std::back_inserter(result) );
  //std::cout << result.size() << " points on the convex hull" << std::endl;

  ofstream outfile(strdup(argv[2]));
  outfile<<result.size()<<endl;
  for(int i=0;i<result.size();i++){
    outfile<<result[i]<<" "<<argv[3]<<endl;
  }
  outfile.close();
  return 0;
}
