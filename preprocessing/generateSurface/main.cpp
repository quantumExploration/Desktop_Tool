#include <vtkVersion.h>
#include <vtkSurfaceReconstructionFilter.h>
#include <vtkProgrammableSource.h>
#include <vtkContourFilter.h>
#include <vtkReverseSense.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkPolyData.h>
#include <vtkCamera.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkMath.h>
#include <vtkSmartPointer.h>
#include <cmath>
#include <vtkPolyDataNormals.h> 
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkSmoothPolyDataFilter.h> 
#include <vtkPoints.h>
#include <vtkPointData.h>
#include <vtkPolygon.h>
#include <vtkCellArray.h>
#include <vtkDoubleArray.h>
#include <vtkCellData.h>
#include "vtkPowerCrustSurfaceReconstruction.h"

#include <cstdlib>
#include <string.h>

using namespace std;

static void readPoints(char *file, vtkPoints* points);
double tminx, tminy, tminz, minz, tmaxx, tmaxy, tmaxz;
double threshold = 0;

vtkSmartPointer<vtkPolyData> transform_back(vtkSmartPointer<vtkPoints> pt, vtkSmartPointer<vtkPolyData> pd)
{  double pt_bounds[6];  // (xmin,xmax, ymin,ymax, zmin,zmax)
  pt->GetBounds(pt_bounds);
  double pd_bounds[6];  // (xmin,xmax, ymin,ymax, zmin,zmax)
  pd->GetBounds(pd_bounds);
  double scale = (pt_bounds[1] - pt_bounds[0])/(pd_bounds[1] - pd_bounds[0]);
  vtkSmartPointer<vtkTransform> transp = vtkSmartPointer<vtkTransform>::New();
  transp->Translate(pt_bounds[0], pt_bounds[2], pt_bounds[4]);
  transp->Scale(scale, scale, scale);
  transp->Translate(- pd_bounds[0], - pd_bounds[2], - pd_bounds[4]);
 
  vtkSmartPointer<vtkTransformPolyDataFilter> tpd = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
#if VTK_MAJOR_VERSION <= 5
  tpd->SetInput(pd);
#else
  tpd->SetInputData(pd);
#endif
  tpd->SetTransform(transp);
  tpd->Update();
 
 
  return tpd->GetOutput();
}

int main(int argc, char **argv){
  threshold = atof(argv[3]);
  // Read some points
  vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();

  char *infile = strdup(argv[1]); 
  char *ofname = strdup(argv[2]);

  readPoints(infile, points);
   
  vtkSmartPointer<vtkPolyData> polydata = vtkSmartPointer<vtkPolyData>::New();
  polydata->SetPoints(points);
 
  // Construct the surface and create isosurface.	


  vtkSmartPointer<vtkSurfaceReconstructionFilter> surf = 
  vtkSmartPointer<vtkSurfaceReconstructionFilter>::New();
    //surf->SetSampleSpacing(1);
  #if VTK_MAJOR_VERSION <= 5
   surf->SetInput(polydata);
  #else
   surf->SetInputData(polydata);
  #endif
  //cout<<surf->GetSampleSpacing()<<endl;

  vtkSmartPointer<vtkContourFilter> contourFilter = vtkSmartPointer<vtkContourFilter>::New();
  contourFilter->SetInputConnection(surf->GetOutputPort());
  contourFilter->SetValue(0, 0.0);
 
  // Sometimes the contouring algorithm can create a volume whose gradient
  // vector and ordering of polygon (using the right hand rule) are
  // inconsistent. vtkReverseSense cures this problem.
  vtkSmartPointer<vtkReverseSense> reverse = vtkSmartPointer<vtkReverseSense>::New();
  reverse->SetInputConnection(contourFilter->GetOutputPort());
  reverse->ReverseCellsOn();
  reverse->ReverseNormalsOn();
  reverse->Update();

  vtkSmartPointer<vtkPolyData> newSurf = transform_back(points, reverse->GetOutput());

  vtkSmartPointer<vtkSmoothPolyDataFilter> smoothFilter =
  vtkSmartPointer<vtkSmoothPolyDataFilter>::New();
  smoothFilter->SetInput(newSurf);//->GetOutput());
  smoothFilter->SetNumberOfIterations(30);
  smoothFilter->SetRelaxationFactor(0.05);
  smoothFilter->FeatureEdgeSmoothingOff();
  smoothFilter->BoundarySmoothingOn();
  smoothFilter->Update();
 
  vtkSmartPointer<vtkPolyDataNormals> normalGenerator = vtkSmartPointer<vtkPolyDataNormals>::New();
  normalGenerator->SetInput(newSurf);
  //normalGenerator->SetInputConnection(smoothFilter->GetOutputPort());
  normalGenerator->ComputePointNormalsOn();
  normalGenerator->ComputeCellNormalsOn();
  normalGenerator->Update();
 
  vtkSmartPointer<vtkPolyDataMapper> map = 
    vtkSmartPointer<vtkPolyDataMapper>::New();
  map->SetInputConnection(normalGenerator->GetOutputPort());
  map->ScalarVisibilityOff();

  vtkPolyData *pd = normalGenerator->GetOutput();
  vtkDataArray* normalsGeneric = pd->GetCellData()->GetNormals(); //works
  vector<double> dir[3];
  vector<double> pos[3];

  if(normalsGeneric)
  {
    //std::cout << "There are " << normalsGeneric->GetNumberOfTuples()
    //         << " normals in normalsGeneric" << std::endl;
 
      int size = normalsGeneric->GetNumberOfTuples();
 //     cout<<size<<endl;
      for(int i=0;i<size;i++){
         double testDouble[3];
         normalsGeneric->GetTuple(i, testDouble);
 
         dir[0].push_back(testDouble[0]);
         dir[1].push_back(testDouble[1]);
         dir[2].push_back(testDouble[2]);
 //        std::cout <<  testDouble[0] << " "
   //           << testDouble[1] << " " << testDouble[2] << std::endl;
      }
  }//cout<<pd->GetNumberOfCells()<<endl;
  double m[6]; m[0]=9e+9; m[1]=9e+9; m[2] =9e+9;
               m[3]=-9e+9;m[4]=-9e+9;m[5]=-9e+9;
  double bl[3], br[3];
  bl[0]=9e+9;bl[1]=9e+9;bl[2] = threshold;
  br[0]=-9e+9;br[1]=-9e+9;br[2]=-threshold;
  for(vtkIdType i=0;i<pd->GetNumberOfCells();i++){
    double *p = new double[3];
    vtkCell *cell = pd->GetCell(i);
    for(int j=0;j<3;j++){
     cell->GetPoints()->GetPoint(j,p);
     pos[0].push_back(p[0]);
     pos[1].push_back(p[1]);
     pos[2].push_back(p[2]);
     if(p[0]<m[0])m[0] = p[0];
     if(p[1]<m[1])m[1] = p[1];
     if(p[2]<m[2])m[2] = p[2];
     if(p[0]>m[3])m[3] = p[0];
     if(p[1]>m[4])m[4] = p[1];
     if(p[2]>m[5])m[5] = p[2];
    }
    // cout<<p[0]<<" "<<p[1]<<" "<<p[2]<<endl;
  }//cerr<<ofname<<endl;
  ofstream outfile(ofname);
//  outfile<<(pd->GetNumberOfCells()+2*4+2)*3<<endl;
  //cout<<m[0]<<" "<<tminx<<endl;
  //cout<<m[1]<<" "<<tminy<<endl;
  //cout<<pos[0].size()<<" "<<dir[0].size()<<" "<<pd->GetNumberOfCells()<<endl;
  outfile<<pd->GetNumberOfCells()*3<<endl;
  double scalex = (tmaxx-tminx)/(m[3]-m[0]);
  double scaley = (tmaxy-tminy)/(m[4]-m[1]);
  double scalez = (tmaxz-tminz)/(m[5]-m[2]);
//cerr<<tmaxz<<" "<<tminz<<" "<<m[5]<<" "<<m[2]<<" "<<scalez<<endl;
  for(int i=0;i<pd->GetNumberOfCells()*3;i++){
    //pos[0][i] = scalex*(pos[0][i] - m[0]) + tminx;
    //pos[1][i] = scaley*(pos[1][i] - m[1]) + tminy;
    //pos[2][i] = scalez*(pos[2][i] - m[2]) + tminz;
    outfile<<pos[0][i]<<" "<<pos[1][i]<<" "<<pos[2][i]<<" ";
    outfile<<dir[0][i/3]<<" "<<dir[1][i/3]<<" "<<dir[2][i/3]<<endl;
     if(fabs(pos[2][i]-threshold)<1e-8){
       if(pos[0][i]>br[0])br[0] = pos[0][i];
       if(pos[1][i]>br[1])br[1] = pos[1][i];
       if(pos[0][i]<bl[0])bl[0] = pos[0][i];
       if(pos[1][i]<bl[1])bl[1] = pos[1][i];
     }
  }
//bottom
//
/*
  outfile<<bl[0]<<" "<<bl[1]<<" "<<minz<<" ";
  outfile<<0<<" "<<0<<" "<<-1<<endl;
  outfile<<br[0]<<" "<<bl[1]<<" "<<minz<<" ";
  outfile<<0<<" "<<0<<" "<<-1<<endl;
  outfile<<br[0]<<" "<<br[1]<<" "<<minz<<" ";
  outfile<<0<<" "<<0<<" "<<-1<<endl;
  outfile<<bl[0]<<" "<<bl[1]<<" "<<minz<<" ";
  outfile<<0<<" "<<0<<" "<<-1<<endl;
  outfile<<br[0]<<" "<<br[1]<<" "<<minz<<" ";
  outfile<<0<<" "<<0<<" "<<-1<<endl;
  outfile<<bl[0]<<" "<<br[1]<<" "<<minz<<" ";
  outfile<<0<<" "<<0<<" "<<-1<<endl;
//side
  outfile<<bl[0]<<" "<<bl[1]<<" "<<minz<<" ";
  outfile<<0<<" "<<-1<<" "<<0<<endl;
  outfile<<br[0]<<" "<<bl[1]<<" "<<minz<<" ";
  outfile<<0<<" "<<-1<<" "<<0<<endl;
  outfile<<br[0]<<" "<<bl[1]<<" "<<bl[2]<<" ";
  outfile<<0<<" "<<-1<<" "<<0<<endl;
  outfile<<bl[0]<<" "<<bl[1]<<" "<<minz<<" ";
  outfile<<0<<" "<<-1<<" "<<0<<endl;
  outfile<<br[0]<<" "<<bl[1]<<" "<<bl[2]<<" ";
  outfile<<0<<" "<<-1<<" "<<0<<endl;
  outfile<<bl[0]<<" "<<bl[1]<<" "<<bl[2]<<" ";
  outfile<<0<<" "<<-1<<" "<<0<<endl;
//side
  outfile<<bl[0]<<" "<<br[1]<<" "<<minz<<" ";
  outfile<<0<<" "<<1<<" "<<0<<endl;
  outfile<<br[0]<<" "<<br[1]<<" "<<minz<<" ";
  outfile<<0<<" "<<1<<" "<<0<<endl;
  outfile<<br[0]<<" "<<br[1]<<" "<<bl[2]<<" ";
  outfile<<0<<" "<<1<<" "<<0<<endl;
  outfile<<bl[0]<<" "<<br[1]<<" "<<minz<<" ";
  outfile<<0<<" "<<1<<" "<<0<<endl;
  outfile<<br[0]<<" "<<br[1]<<" "<<bl[2]<<" ";
  outfile<<0<<" "<<1<<" "<<0<<endl;
  outfile<<bl[0]<<" "<<br[1]<<" "<<bl[2]<<" ";
  outfile<<0<<" "<<1<<" "<<0<<endl;
//side
  outfile<<br[0]<<" "<<bl[1]<<" "<<minz<<" ";
  outfile<<1<<" "<<0<<" "<<0<<endl;
  outfile<<br[0]<<" "<<br[1]<<" "<<minz<<" ";
  outfile<<1<<" "<<0<<" "<<0<<endl;
  outfile<<br[0]<<" "<<br[1]<<" "<<bl[2]<<" ";
  outfile<<1<<" "<<0<<" "<<0<<endl;
  outfile<<br[0]<<" "<<bl[1]<<" "<<minz<<" ";
  outfile<<1<<" "<<0<<" "<<0<<endl;
  outfile<<br[0]<<" "<<br[1]<<" "<<bl[2]<<" ";
  outfile<<1<<" "<<0<<" "<<0<<endl;
  outfile<<br[0]<<" "<<bl[1]<<" "<<bl[2]<<" ";
  outfile<<1<<" "<<0<<" "<<0<<endl;
//side
  outfile<<bl[0]<<" "<<bl[1]<<" "<<minz<<" ";
  outfile<<-1<<" "<<0<<" "<<0<<endl;
  outfile<<bl[0]<<" "<<br[1]<<" "<<minz<<" ";
  outfile<<-1<<" "<<0<<" "<<0<<endl;
  outfile<<bl[0]<<" "<<br[1]<<" "<<bl[2]<<" ";
  outfile<<-1<<" "<<0<<" "<<0<<endl;
  outfile<<bl[0]<<" "<<bl[1]<<" "<<minz<<" ";
  outfile<<-1<<" "<<0<<" "<<0<<endl;
  outfile<<bl[0]<<" "<<br[1]<<" "<<bl[2]<<" ";
  outfile<<-1<<" "<<0<<" "<<0<<endl;
  outfile<<bl[0]<<" "<<bl[1]<<" "<<bl[2]<<" ";
  outfile<<-1<<" "<<0<<" "<<0<<endl;
*/
  outfile.close();

//=====================render====================== 
  vtkSmartPointer<vtkActor> surfaceActor = 
    vtkSmartPointer<vtkActor>::New();
  surfaceActor->SetMapper(map);
 
  // Create the RenderWindow, Renderer and both Actors
  vtkSmartPointer<vtkRenderer> ren = 
    vtkSmartPointer<vtkRenderer>::New();
  vtkSmartPointer<vtkRenderWindow> renWin = 
    vtkSmartPointer<vtkRenderWindow>::New();
  renWin->AddRenderer(ren);
  vtkSmartPointer<vtkRenderWindowInteractor> iren = 
    vtkSmartPointer<vtkRenderWindowInteractor>::New();
  iren->SetRenderWindow(renWin);
 
  // Add the actors to the renderer, set the background and size
  ren->AddActor(surfaceActor);
  ren->SetBackground(.2, .3, .4);
 
  renWin->Render();
  iren->Start();
 
  return EXIT_SUCCESS;
}
 
 
void readPoints(char *file, vtkPoints* points)
{
  tminx = 9e+9;
  tminy = 9e+9;
  tminz = 9e+9;
  tmaxx = -9e+9;
  tmaxy = -9e+9;
  tmaxz = -9e+9;
  minz = 9e+9;
  ifstream infile(file);
  int n;
  infile>>n;
//cout<<n<<endl;
  for(int i=0;i<n;i++){
    float x, y, z;
    infile>>x>>y>>z;
   // if(fabs(z+1)<1e-8)z==-20;
   // if(fabs(z)<1e-8)z=-10;
  //  cout<<x<<endl;
   //if(z<0.25) 
    if(z>threshold || fabs(z-threshold)<1e-8){
      points->InsertNextPoint(x,y,z);
      if(x<tminx)tminx=x;
      if(y<tminy)tminy=y;
      if(z<tminz)tminz=z;
      if(x>tmaxx)tmaxx=x;
      if(y>tmaxy)tmaxy=y;
      if(z>tmaxz)tmaxz=z;
    }
    if(z<minz) minz = z;
}  
  infile.close();
/*  float x, y, z;
  // generate random points on unit sphere
  for(int i=0; i<500; i++) 
    {
      double u = vtkMath::Random(0.0,1.0);
      double v = vtkMath::Random(0.0,1.0);
      double phi = 2.0*3.14159265*u;
      double theta = acos(2.0*v-1.0);
 
      x = std::cos(phi)*std::sin(theta);
      y = std::sin(phi)*std::sin(theta);
      z = std::cos(theta);
      points->InsertNextPoint(x, y, z);
    }
 */
  return;
}
