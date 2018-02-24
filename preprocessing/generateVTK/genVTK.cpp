#include "genVTKfromOrg.h"
#include <iostream>
#include <stdio.h>      /* printf, fgets */
#include <stdlib.h>     /* atof */
#include <string.h>
#include <sys/stat.h>

using namespace std;

#define STR_SIZE  1024 

int main(int argc, char *argv[])
{
    //char *in_dir = new char[200];
    //char *in_fname = new char[200];
    //char *out_dir = new char[200];
    int scalar=0;
    
/*
    char *raw_fname = new char[STR_SIZE];
    char *sort_fname = new char[STR_SIZE];
    char *vtk_fname = new char[STR_SIZE];
    char *format_fnam = new char[STR_SIZE];
*/
    char *raw_fname = strdup(argv[1]);
    char *store_dir = strdup(argv[2]);
    char *sort_fname = strdup(argv[3]);
    char *twod_fname = strdup(argv[4]);
    char *format_fname = strdup(argv[5]);
    char *density_fname = strdup(argv[6]);
    char *src_dir = strdup(argv[7]);
    char *convex_fname = strdup(argv[8]);
    char *input_fname = strdup(argv[9]);
    int format = atoi(argv[10]);
   
    //cerr<<format<<endl;
 
    mkdir(store_dir, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);  

    float pos_x, pos_y, pos_z, dir_x, dir_y, dir_z, distance;

    ifstream infile(input_fname);
    infile>>pos_x>>pos_y>>pos_z>>dir_x>>dir_y>>dir_z>>distance;
//cerr<<distance<<endl;
    infile.close();
//cerr<<density_fname<<endl;
    //scalar = atoi(argv[5]); 
    
   // sprintf(raw_fname, "%s/%s", in_dir, in_fname);
  //  sprintf(sort_fname, "%s/%s_sorted", out_dir, in_fname);
   // sprintf(vtk_fname, "%s/%s", out_dir, in_fname);
   // sprintf(format_fname, "%s/%s_format", out_dir, in_fname);
   
cerr<<"...Data Processing ..."<<endl; 
    genVTKfromOrg *qdotData = new genVTKfromOrg();

    qdotData->SetFormat(format);    
    qdotData->ReadRawData(raw_fname, scalar);cerr<<"read"<<endl;
    qdotData->SplitData(pos_x, pos_y, pos_z, dir_x, dir_y, dir_z, distance);
    qdotData->Compute2DData();
    qdotData->SaveRegiontoFile(store_dir);cerr<<"region"<<endl;
    qdotData->ComputeMeshPoints(src_dir, store_dir);
    
    qdotData->SaveDataByLayer(store_dir, sort_fname, twod_fname);
//    qdotData->ComputeMesh(src_dir, store_dir, convex_fname);
    qdotData->SavetoVTK(store_dir);cerr<<"save to "<< store_dir << endl;
    qdotData->SaveLayerFormat(store_dir);//cerr<<"layer"<<endl;
    qdotData->SaveFormat(store_dir,format_fname);//cerr<<"save to "<< format_fname << endl;
    qdotData->Save2DDensity(store_dir);//cerr<<"density"<<endl;
    qdotData->SaveDensityBylayer(store_dir, density_fname);

cerr<<"....Data processing done..."<<endl;
    //delete [] in_dir;
    //delete [] in_fname;
    //delete [] out_dir;
    delete [] raw_fname;
    delete [] store_dir; 
    delete [] sort_fname;
    delete [] twod_fname;
    delete [] format_fname;
    delete [] density_fname;
    delete [] src_dir; 
    delete [] convex_fname;    
//    delete qdotData;
 //   qdotData = NULL;
}
