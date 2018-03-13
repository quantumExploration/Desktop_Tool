#include "svUtil.h"
#include "readfiles.h"

namespace __svl_lib{

void loadOBJ(char *path, svMeshData *data, int layer)
{
  FILE * file = fopen(path, "r");
  if( file == NULL ){
    printf("Impossible to open the file !\n");
    exit(0);return ;
  }
  vector<svVector3> temp_vertices;
  vector<int> temp_indices[3];
  while( 1 ){
    char lineHeader[128];
    // read the first word of the line
    int res = fscanf(file, "%s", lineHeader);
    if (res == EOF)
        break; // EOF = End Of File. Quit the loop.
    // else : parse lineHeader
    if ( strcmp( lineHeader, "v" ) == 0 ){
      svVector3 vertex;
      fscanf(file, "%f %f %f\n", &vertex[0], &vertex[1], &vertex[2] );
      temp_vertices.push_back(vertex);
    }
    else if ( strcmp( lineHeader, "f" ) == 0 ){
      unsigned int vertexIndex[3];
      int matches = fscanf(file, "%d/%d/%d\n", &vertexIndex[0], &vertexIndex[1], &vertexIndex[2] );
      if (matches != 3){
        printf("File can't be read by our simple parser : ( Try exporting with other options\n");
        exit(1);return ;
      }
      temp_indices[0].push_back(vertexIndex[0]);
      temp_indices[1].push_back(vertexIndex[1]);
      temp_indices[2].push_back(vertexIndex[2]);
    }
  }
  for(int i=0;i<temp_indices[0].size();i++)
  {
      meshFacet facet;
      facet.pos[0] = temp_vertices[temp_indices[0][i]];
      facet.pos[1] = temp_vertices[temp_indices[1][i]];
      facet.pos[2] = temp_vertices[temp_indices[2][i]];
      facet.norm = cross(normalize(facet.pos[1]-facet.pos[0]),
                         normalize(facet.pos[2]-facet.pos[0]));
      data->facets[layer].push_back(facet);
  }
}

}
