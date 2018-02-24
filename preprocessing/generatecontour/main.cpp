
#include "svContour.h"

#include <stdio.h>      /* printf, fgets */
#include <stdlib.h>     /* atof */
#include <iostream>
#include <string.h>

using namespace std;

int main(int argc, char *argv[])
{
//#ifdef _VTK
	char *in_fname = new char[200];	
	char *out_fname = new char[200];
	char *in_fname2 = new char[200];
	char *input = new char[200];
	float ratio;
	int layer;

	float zmin;
	float zdistance;
	float value;

	  if(argc == 4)
	  {
			in_fname = strdup(argv[1]);
			out_fname = strdup(argv[2]);
		//	input = strdup(argv[3]);
			value = atof(argv[3]);
	     vector<float> gv;
	     svContour *qdotcontour = new svContour();


		  char vtkfile[250];
		  sprintf(vtkfile, "%s", in_fname);

			gv.push_back(value);

		for(int j=0;j<gv.size();j++)
		{
		  qdotcontour->Get2DContour(vtkfile, gv[j]);//
		}
	       qdotcontour->SavePointtoFile(out_fname);

	
	      delete qdotcontour;
	      qdotcontour=NULL;
	  }
          else if(argc == 5)//generate svl
          {
                in_fname = strdup(argv[1]);
                out_fname = strdup(argv[2]);
                   value = atof(argv[3]);
             vector<float> gv;
             svContour *qdotcontour = new svContour();


                  char vtkfile[250];
                  sprintf(vtkfile, "%s", in_fname);

                        gv.push_back(value);

                for(int j=0;j<gv.size();j++)
                {
                  qdotcontour->Get2DContourLine(vtkfile, gv[j]);//
                }
               qdotcontour->SavePointtoSVL(out_fname);

              delete qdotcontour;
              qdotcontour=NULL;

          }
	  else
	  {
			cerr << "Usage:  ./main [Input_File] [Output_File] [config file]" << endl;  
			//exit(0);
	  }

//	vector<int> gt;


	/*ifstream infile(input);

	int n;
	infile>>n;
	//local/global z0 z1 absolute/ratio value
	for(int i=0;i<n;i++)
	{
		int ar;
//		infile>>ar;

		float v;
		infile>>v;

//		gt.push_back(ar);
		gv.push_back(v);
	}

	infile.close();*/

	  //--------Generate Contours----------

		//ifstream infile(in_fname2);
		//infile>>layer;
		//dinfile.close();

              delete [] in_fname;
              delete [] out_fname;

//#endif
	  return 0;
}
