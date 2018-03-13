#include <time.h>
#include <fstream>
#include <vector>
#include <string.h>
#include <cmath>
#include <algorithm>
#include <cstdlib>

#include <vtkDoubleArray.h>
#include <vtkWindowedSincPolyDataFilter.h>
#include <vtkTriangle.h>
#include <vtkPolyDataConnectivityFilter.h>
#include <vtkPolyData.h>
#include <vtkSTLReader.h>
#include <vtkPolyDataReader.h>
#include <vtkSmartPointer.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkCellArray.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkPolyDataWriter.h>
#include <vtkSmoothPolyDataFilter.h>
#include <vtkContourFilter.h>
#include <vtkPolyDataNormals.h>
#include <vtkUnstructuredGridReader.h>
#include <vtkPolygon.h>
#include <vtkDelaunay3D.h>
#include <vtkDelaunay2D.h>
#include <vtkDataSetSurfaceFilter.h>
#include <vtkUnstructuredGrid.h>
#include <vtkFillHolesFilter.h>
#include <vtkCleanPolyData.h>
#include <vtkAppendPolyData.h>
#include <vtkCellData.h>
#include <vtkCell.h>
#include <vtkPointData.h>
#include <vtkUnstructuredGrid.h>
#include <vtkUnstructuredGridWriter.h>
#include <vtkPoints.h>
using namespace std;


void Smooth(char * vtkfile, char *ofname){
  vtkSmartPointer<vtkPolyDataReader> vtkReader =
  vtkSmartPointer<vtkPolyDataReader>::New();
  vtkReader->SetFileName(vtkfile);
//  vtkReader->Update();

  vtkSmartPointer<vtkWindowedSincPolyDataFilter> smoothFilter =
  vtkSmartPointer<vtkWindowedSincPolyDataFilter>::New();
  smoothFilter->SetInputConnection(vtkReader->GetOutputPort());
  smoothFilter->SetNumberOfIterations(50);
  //smoothFilter->SetEdgeAngle(180);
  //smoothFilter->SetFeatureAngle(180);
  //smoothFilter->SetPassBand(0.01);
  //smoothFilter->NonManifoldSmoothingOn();
  //smoothFilter->SetRelaxationFactor(0.25);
  //smoothFilter->FeatureEdgeSmoothingOff();
  //smoothFilter->BoundarySmoothingOff();
  smoothFilter->NormalizeCoordinatesOn();
//  smoothFilter->Update();

  vtkSmartPointer<vtkPolyDataNormals> normalGenerator =
      vtkSmartPointer<vtkPolyDataNormals>::New();
  normalGenerator->SetInputConnection(smoothFilter->GetOutputPort());
  normalGenerator->ComputePointNormalsOff();
  //normalGenerator->ConsistencyOn();
  normalGenerator->SetFeatureAngle(30);
  normalGenerator->NonManifoldTraversalOff();
  normalGenerator->AutoOrientNormalsOn();
  normalGenerator->ComputeCellNormalsOn();
  normalGenerator->Update();

  vtkSmartPointer<vtkPolyData>  wholePolygon = 
      vtkSmartPointer<vtkPolyData>::New();
  wholePolygon = normalGenerator->GetOutput();
  double nn[3];

  vector<double> meshpos[3];
  vector<double> meshdir[3];

  vtkDataArray* normalsGeneric = wholePolygon->GetCellData()->GetNormals(); //works
  if(normalsGeneric){
      int size = normalsGeneric->GetNumberOfTuples();
      for(int i=0;i<size;i++){
         double testDouble[3];
         normalsGeneric->GetTuple(i, testDouble);
         meshdir[0].push_back(-testDouble[0]);
         meshdir[1].push_back(-testDouble[1]);
         meshdir[2].push_back(-testDouble[2]);
       }
  }

  for(vtkIdType i=0;i<wholePolygon->GetNumberOfCells();i++){
    vtkSmartPointer<vtkIdList> cellPointIds =
    vtkSmartPointer<vtkIdList>::New();
    wholePolygon->GetCellPoints(i,cellPointIds);
    for(int j=0;j<3;j++){
     double *p = new double[3];
     wholePolygon->GetPoint(cellPointIds->GetId(j), p);
     meshpos[0].push_back(p[0]);
     meshpos[1].push_back(p[1]);
     meshpos[2].push_back(p[2]);
    }
  }

  ofstream outfile(ofname);
  outfile<<meshpos[0].size()<<endl;
  for(int i=0;i<meshpos[0].size();i++){
   outfile<<meshpos[0][i]<<" "
          <<meshpos[1][i]<<" "
          <<meshpos[2][i]<<" "
          <<meshdir[0][i/3]<<" "
          <<meshdir[1][i/3]<<" "
          <<meshdir[2][i/3]<<endl;
  }
  outfile.close();

  vtkSmartPointer<vtkPolyDataWriter> writer =
  vtkSmartPointer<vtkPolyDataWriter>::New();
  writer->SetInputConnection(normalGenerator->GetOutputPort());//topgrid);//wholePolygon);
  writer->SetFileName("bottom.vtk");
  writer->Update();

}
int main(int argc, char **argv){

  Smooth(strdup(argv[1]), strdup(argv[2]));

  return 0;
}
