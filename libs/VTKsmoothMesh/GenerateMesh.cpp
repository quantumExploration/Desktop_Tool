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

struct Spin{
 double x;
 double y;
 double z;
 double r;
};
vector<Spin> qdot;
vector<Spin> contour;
vector<Spin> top;
vector<Spin> bottom;
Spin toplb;
Spin toprb;
Spin bottomlb;
Spin bottomrb;
float threshold1, threshold2;
float density=0.4;
float avgx=0;
float avgy=0;//hard
vtkSmartPointer<vtkPolyData> contourPolygon;
vtkSmartPointer<vtkPolyData> ctPolygon;
vtkSmartPointer<vtkPolyData> topPolygon;
vtkSmartPointer<vtkPolyData> tbPolygon;
vtkSmartPointer<vtkPolyData> bottomPolygon;

bool Sort(Spin A, Spin B){
 return(atan2(A.x-avgx, A.y-avgy) < atan2(B.x-avgx, B.y-avgy));
}
bool SortY(const Spin &a, const Spin &b)
{
 if (fabs(a.y - b.y)>0.0001)
 {
   return (a.y<b.y);
 }
 else if (fabs(a.x - b.x)>0.0001)
 {
   return (a.x<b.x);
 }

 return 0;
}

//read the txt file
void readFiles(char *ifname){
  vtkSmartPointer<vtkUnstructuredGridReader> reader =
     vtkSmartPointer<vtkUnstructuredGridReader>::New();
  reader->SetFileName(ifname);
  reader->Update();
  for(int i=0;i<reader->GetOutput()->GetNumberOfPoints();i++){
    double p[3];
    reader->GetOutput()->GetPoint(i,p);
    double den[3];
    reader->GetOutput()->GetPointData()->GetArray(0)->GetTuple(i,den);
    Spin tmp;
    tmp.x = p[0]; tmp.y = p[1]; tmp.z = p[2];
    tmp.r = den[0];
    qdot.push_back(tmp);
    if(p[2]>threshold1){
      contour.push_back(tmp);
    }
    else if(fabs(p[2]-threshold1)<1e-8){
      top.push_back(tmp);
    }
    else if(fabs(p[2]-threshold2)<1e-8){
      bottom.push_back(tmp);
    }
  }
}
//=========================
void smoothTop(){
  sort(top.begin(),top.end(),SortY);
  vector<Spin> tmptop;
  toplb.x = 9e+9; toplb.y=9e+9; toplb.z=9e+9;
  toprb.x = -9e+9; toprb.y=-9e+9; toprb.z=-9e+9;
  for(int i=0;i<top.size();i++){
    if(top[i].x < toplb.x)toplb.x = top[i].x;
    if(top[i].y < toplb.y)toplb.y = top[i].y;
    if(top[i].z < toplb.z)toplb.z = top[i].z;
    if(top[i].x > toprb.x)toprb.x = top[i].x;
    if(top[i].y > toprb.y)toprb.y = top[i].y;
    if(top[i].z > toprb.z)toprb.z = top[i].z;
  }
  if(top[0].x>toplb.x ){
    Spin tmp; tmp.x = toplb.x; tmp.y = toplb.y;tmp.z=toplb.z;
    tmptop.push_back(tmp);
  }
  for(int i=1;i<top.size();i++){
    if(fabs(top[i].y-top[i-1].y)>1e-8){
      Spin tmp;
      if(top[i-1].x < toprb.x){
        tmp.x = toprb.x;
        tmp.y = top[i-1].y;tmp.z=toplb.z;
        tmptop.push_back(tmp);
      }
      if(top[i].x > toplb.x){
        tmp.x = toplb.x;
        tmp.y = top[i].y;tmp.z=toplb.z;
        tmptop.push_back(tmp);
      }
    }
  }
  avgx = (toplb.x + toprb.x)/2.;
  avgy = (toplb.y + toprb.y)/2.;
  if(top[top.size()-1].x<toprb.x ){
    Spin tmp; tmp.x = toprb.x; tmp.y = toprb.y;tmp.z=toplb.z;
    tmptop.push_back(tmp);
  }
  top.insert(top.begin(), tmptop.begin(), tmptop.end());
  tmptop.clear();
}

void smoothBottom(){
  sort(bottom.begin(),bottom.end(),SortY);
  vector<Spin> tmpbottom;
  bottomlb.x = 9e+9; bottomlb.y=9e+9; bottomlb.z=9e+9;
  bottomrb.x = -9e+9; bottomrb.y=-9e+9; bottomrb.z=-9e+9;
  for(int i=0;i<bottom.size();i++){
    if(bottom[i].x < bottomlb.x)bottomlb.x = bottom[i].x;
    if(bottom[i].y < bottomlb.y)bottomlb.y = bottom[i].y;
    if(bottom[i].z < bottomlb.z)bottomlb.z = bottom[i].z;
    if(bottom[i].x > bottomrb.x)bottomrb.x = bottom[i].x;
    if(bottom[i].y > bottomrb.y)bottomrb.y = bottom[i].y;
    if(bottom[i].z > bottomrb.z)bottomrb.z = bottom[i].z;
  }
  if(bottom[0].x>bottomlb.x ){
    Spin tmp; tmp.x = bottomlb.x; 
    tmp.y = bottomlb.y;tmp.z=bottomlb.z;
    tmpbottom.push_back(tmp);
  }
  for(int i=1;i<bottom.size();i++){
    if(fabs(bottom[i].y-bottom[i-1].y)>1e-8){
      Spin tmp;
      if(bottom[i-1].x < bottomrb.x){
        tmp.x = bottomrb.x;
        tmp.y = bottom[i-1].y;tmp.z=bottomlb.z;
        tmpbottom.push_back(tmp);
      }
      if(bottom[i].x > bottomlb.x){
        tmp.x = bottomlb.x;
        tmp.y = bottom[i].y;tmp.z=bottomlb.z;
        tmpbottom.push_back(tmp);
      }
    }
  }
  if(bottom[bottom.size()-1].x<bottomrb.x ){
    Spin tmp; tmp.x = bottomrb.x; 
    tmp.y = bottomrb.y;tmp.z=bottomlb.z;
    tmpbottom.push_back(tmp);
  }
  bottom.insert(bottom.begin(), tmpbottom.begin(), tmpbottom.end());
  tmpbottom.clear();
}

void meshConstruction(){
  clock_t t1,t2;
  t1=clock();

  vtkSmartPointer<vtkUnstructuredGrid> contourgrid = 
   vtkSmartPointer<vtkUnstructuredGrid>::New();
  vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
  vtkSmartPointer<vtkDoubleArray> weights = vtkSmartPointer<vtkDoubleArray>::New();
  weights->SetNumberOfValues(contour.size());
  for(int i=0;i<contour.size();i++){
    points->InsertNextPoint(contour[i].x, contour[i].y, contour[i].z);
    weights->SetValue(i, contour[i].r);
  }
  contourgrid->SetPoints(points);
  contourgrid->GetPointData()->SetScalars(weights);

  cout<<"It will take 10 minutes to generate an initial surface ..."<<endl;
  vtkSmartPointer<vtkDelaunay3D> del3d =
     vtkSmartPointer<vtkDelaunay3D>::New();
#if VTK_MAJOR_VERSION <= 5
  del3d->SetInput(contourgrid);
#else
  del3d->SetInputData(contourgrid);
#endif
  del3d->SetTolerance(0.001);
  del3d->SetAlpha(0);

  vtkSmartPointer<vtkContourFilter> iso =
     vtkSmartPointer<vtkContourFilter>::New();
  iso->SetInputConnection(del3d->GetOutputPort());
  iso->SetValue(0.,density);
  iso->Update();

  contourPolygon = iso->GetOutput();
/*
  vtkSmartPointer<vtkPolyDataWriter> writer =
  vtkSmartPointer<vtkPolyDataWriter>::New();
  writer->SetInputConnection(iso->GetOutputPort());
  writer->SetFileName("contour.vtk");
  writer->Update();
*/
  t2=clock();
  float diff = ((float)t2-(float)t1)/CLOCKS_PER_SEC;
  //cout<<"Time "<<diff<<endl;
}

void sphereSmooth(){
/*  vtkSmartPointer<vtkPolyDataReader> vtkReader =
   vtkSmartPointer<vtkPolyDataReader>::New();
  vtkReader->SetFileName("contour.vtk");
  vtkReader->Update();
*/
  vtkSmartPointer<vtkWindowedSincPolyDataFilter> smoothFilter =
  vtkSmartPointer<vtkWindowedSincPolyDataFilter>::New();
#if VTK_MAJOR_VERSION <= 5
  smoothFilter->SetInput(contourPolygon);
#else
  smoothFilter->SetInputData(contourPolygon);//SetInputConnection(vtkReader->GetOutputPort());
#endif
  smoothFilter->SetNumberOfIterations(75);
  smoothFilter->SetEdgeAngle(180);
  smoothFilter->SetFeatureAngle(180);
  smoothFilter->SetPassBand(0.01);
  smoothFilter->NonManifoldSmoothingOn();
  //smoothFilter->SetRelaxationFactor(0.25);
  smoothFilter->FeatureEdgeSmoothingOff();
  smoothFilter->BoundarySmoothingOn();
  smoothFilter->NormalizeCoordinatesOn();
  smoothFilter->Update();
/*
  vtkSmartPointer<vtkPolyDataWriter> writer =
  vtkSmartPointer<vtkPolyDataWriter>::New();
  writer->SetInputConnection(smoothFilter->GetOutputPort());//wholePolygon);
  writer->SetFileName("contoursmooth.vtk");
  writer->Update();
*/
  contourPolygon = smoothFilter->GetOutput();
}

void topProcess(){
/*  vtkSmartPointer<vtkPolyDataReader> vtkReader =
   vtkSmartPointer<vtkPolyDataReader>::New();
  vtkReader->SetFileName("contoursmooth.vtk");
  vtkReader->Update();
*/
  vtkSmartPointer<vtkPolyData> data = vtkSmartPointer<vtkPolyData>::New();
  double minz=9e+9;
  data = contourPolygon;//vtkReader->GetOutput();
  for(int i=0;i<data->GetNumberOfPoints();i++){
    double p[3];
    data->GetPoint(i,p);
    if(p[2] < minz){minz = p[2];} 
  }

  int count = 0;
  vtkSmartPointer<vtkPolyData> apolydata = 
   vtkSmartPointer<vtkPolyData>::New();
  vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
  vtkSmartPointer<vtkDoubleArray> weights = vtkSmartPointer<vtkDoubleArray>::New();
  vtkSmartPointer<vtkCellArray> aCellArray =
   vtkSmartPointer<vtkCellArray>::New();
  vtkSmartPointer<vtkPolygon> apolygon = 
   vtkSmartPointer<vtkPolygon>::New();

  vector<Spin> tmpp;
  for(int i=0;i<data->GetNumberOfPoints();i++){
    double p[3];
    data->GetPoint(i,p);
    if(fabs(p[2] - minz)<0.001){
      Spin pp; pp.x = p[0];pp.y=p[1];pp.z=threshold1;
      tmpp.push_back(pp);
    }
  }
  sort(tmpp.begin(), tmpp.end(), Sort);
  for(int i=0;i<tmpp.size();i++){
     points->InsertNextPoint(tmpp[i].x, tmpp[i].y, tmpp[i].z);
      count++;
  }
  tmpp.clear();
  /*
  for(int i=0;i<data->GetNumberOfPoints();i++){
    double p[3];
    data->GetPoint(i,p);
    if(fabs(p[2] - minz)<0.001){
      points->InsertNextPoint(p[0],p[1],threshold1);
      cout<<p[0]<<" "<<p[1]<<" "<<p[2]<<endl;
      count++;
    }       
  }*/
  weights->SetNumberOfValues(top.size() + count);
  for(int i=0;i<count;i++){
    weights->SetValue(i, 0.4);
    apolygon->GetPointIds()->InsertNextId(i);
  }
  aCellArray->InsertNextCell(apolygon);
  for(int i=0;i<top.size();i++){
    points->InsertNextPoint(top[i].x, top[i].y, top[i].z);
    weights->SetValue(i+count, 1);
  }
  apolydata->SetPoints(points);
  apolydata->GetPointData()->SetScalars(weights);

  vtkSmartPointer<vtkPolyData> polys =
                vtkSmartPointer<vtkPolyData>::New();
  polys->SetPoints(apolydata->GetPoints());
  polys->SetPolys(aCellArray);
 
  vtkSmartPointer<vtkDelaunay2D> triangulator =
                vtkSmartPointer<vtkDelaunay2D>::New();
  triangulator->SetOffset(2.5);
#if VTK_MAJOR_VERSION <= 5
        triangulator->SetInput(apolydata);//topgrid);
        triangulator->SetSource(polys);
#else
        triangulator->SetInputData(apolydata);//topgrid);
        triangulator->SetSourceData(polys);
#endif
        triangulator->Update();

  topPolygon = triangulator->GetOutput();
/*
  vtkSmartPointer<vtkPolyDataWriter> writer =
  vtkSmartPointer<vtkPolyDataWriter>::New();
  writer->SetInputConnection(triangulator->GetOutputPort());//topgrid);//wholePolygon);
  writer->SetFileName("top.vtk");
  writer->Update();
*/
}

void bottomProcess(){
  int count = 0;
  vtkSmartPointer<vtkPolyData> apolydata = 
   vtkSmartPointer<vtkPolyData>::New();
  vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
  vtkSmartPointer<vtkDoubleArray> weights = vtkSmartPointer<vtkDoubleArray>::New();

  weights->SetNumberOfValues(bottom.size());
  for(int i=0;i<bottom.size();i++){
    points->InsertNextPoint(bottom[i].x, bottom[i].y, bottom[i].z);
    weights->SetValue(i, 1);
  }
  apolydata->SetPoints(points);
  apolydata->GetPointData()->SetScalars(weights);
 
  vtkSmartPointer<vtkDelaunay2D> triangulator =
                vtkSmartPointer<vtkDelaunay2D>::New();
  triangulator->SetOffset(2.5);
#if VTK_MAJOR_VERSION <= 5
        triangulator->SetInput(apolydata);//topgrid);
#else
        triangulator->SetInputData(apolydata);//topgrid);
#endif
        triangulator->Update();
 
  bottomPolygon = triangulator->GetOutput();
/*
  vtkSmartPointer<vtkPolyDataWriter> writer =
  vtkSmartPointer<vtkPolyDataWriter>::New();
  writer->SetInputConnection(triangulator->GetOutputPort());//topgrid);//wholePolygon);
  writer->SetFileName("bottom.vtk");
  writer->Update();
*/
}

void mergeMeshes(){
 /* vtkSmartPointer<vtkPolyDataReader> vtkReader =
   vtkSmartPointer<vtkPolyDataReader>::New();
  vtkReader->SetFileName("contoursmooth.vtk");
  vtkReader->Update();
*/
  vtkSmartPointer<vtkPolyData> data = vtkSmartPointer<vtkPolyData>::New();
  double minz=9e+9;
  data = contourPolygon;//vtkReader->GetOutput();
  for(int i=0;i<data->GetNumberOfPoints();i++){
    double p[3];
    data->GetPoint(i,p);
    if(p[2] < minz){minz = p[2];}
  }

  int count = 0;
  vtkSmartPointer<vtkPolyData> apolydata =
   vtkSmartPointer<vtkPolyData>::New();
  vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
  vtkSmartPointer<vtkDoubleArray> weights = vtkSmartPointer<vtkDoubleArray>::New();
  vtkSmartPointer<vtkCellArray> triangles = vtkSmartPointer<vtkCellArray>::New();

  vector<Spin> tmpp;
  for(int i=0;i<data->GetNumberOfPoints();i++){
    double p[3];
    data->GetPoint(i,p);
    if(fabs(p[2] - minz)<0.001){
      Spin pp; pp.x = p[0];pp.y=p[1];pp.z=threshold1;
      tmpp.push_back(pp);
    }
  }
  sort(tmpp.begin(), tmpp.end(),  Sort);
  for(int i=0;i<tmpp.size();i++){
    points->InsertNextPoint(tmpp[i].x, tmpp[i].y, tmpp[i].z);
    points->InsertNextPoint(tmpp[i].x, tmpp[i].y, minz);
    vtkSmartPointer<vtkTriangle> triangle = vtkSmartPointer<vtkTriangle>::New();
    if(i<tmpp.size()-1){
       triangle->GetPointIds()->SetId(0,i*2+1);
       triangle->GetPointIds()->SetId(1,i*2);
       triangle->GetPointIds()->SetId(2,i*2+2);
    }
    else{
       triangle->GetPointIds()->SetId(0,i*2+1);
       triangle->GetPointIds()->SetId(1,i*2);
       triangle->GetPointIds()->SetId(2,0);
    }
    triangles->InsertNextCell(triangle);
    if(i<tmpp.size()-1){
       triangle->GetPointIds()->SetId(0,i*2+1);
       triangle->GetPointIds()->SetId(1,i*2+2);
       triangle->GetPointIds()->SetId(2,i*2+3);
    }
    else{
       triangle->GetPointIds()->SetId(0,i*2+1);
       triangle->GetPointIds()->SetId(1,1);
       triangle->GetPointIds()->SetId(2,0);
    }
    triangles->InsertNextCell(triangle);
  }
//  triangles->InsertNextCell(triangle);
  apolydata->SetPoints(points);
  apolydata->SetPolys(triangles);

  ctPolygon = apolydata;
/*
  vtkSmartPointer<vtkPolyDataWriter> writer =
  vtkSmartPointer<vtkPolyDataWriter>::New();
  writer->SetInput(apolydata);//triangulator->GetOutputPort());//topgrid);//wholePolygon);
  writer->SetFileName("ctconnection.vtk");
  writer->Update();
*/
}

void mergePlane(){
  int segment = (toprb.x-toplb.x);
  float degree = (bottomrb.x - bottomlb.x)/(float)segment;
  int count = 0;
  vtkSmartPointer<vtkPolyData> apolydata =
   vtkSmartPointer<vtkPolyData>::New();
  vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
  vtkSmartPointer<vtkDoubleArray> weights = vtkSmartPointer<vtkDoubleArray>::New();
  vtkSmartPointer<vtkCellArray> triangles = vtkSmartPointer<vtkCellArray>::New();

  for(int i =0;i<=segment;i++){
    points->InsertNextPoint(toplb.x+i, toplb.y, toplb.z);
    points->InsertNextPoint(bottomlb.x+(float)i*(float)degree, 
                            bottomlb.y, bottomlb.z);
    if(i<segment){
    vtkSmartPointer<vtkTriangle> triangle = vtkSmartPointer<vtkTriangle>::New();
    triangle->GetPointIds()->SetId(0,i*2);
    triangle->GetPointIds()->SetId(1,i*2+1);
    triangle->GetPointIds()->SetId(2,i*2+3);
    triangles->InsertNextCell(triangle);
    triangle->GetPointIds()->SetId(0,i*2);
    triangle->GetPointIds()->SetId(1,i*2+3);
    triangle->GetPointIds()->SetId(2,i*2+2);
    triangles->InsertNextCell(triangle);
    }
  }
  count = points->GetNumberOfPoints();
  for(int i =0;i<=segment;i++){
    points->InsertNextPoint(toplb.x+i, toprb.y, toplb.z);
    points->InsertNextPoint(bottomlb.x+(float)i*(float)degree,
                            bottomrb.y, bottomlb.z);
    if(i<segment){
    vtkSmartPointer<vtkTriangle> triangle = vtkSmartPointer<vtkTriangle>::New();
    triangle->GetPointIds()->SetId(0,i*2+count);
    triangle->GetPointIds()->SetId(1,i*2+1+count);
    triangle->GetPointIds()->SetId(2,i*2+3+count);
    triangles->InsertNextCell(triangle);
    triangle->GetPointIds()->SetId(0,i*2+count);
    triangle->GetPointIds()->SetId(1,i*2+3+count);
    triangle->GetPointIds()->SetId(2,i*2+2+count);
    triangles->InsertNextCell(triangle);
    }
  }
  count = points->GetNumberOfPoints();

  segment = (toprb.y-toplb.y);
  degree = (bottomrb.y - bottomlb.y)/(float)segment;
  for(int i =0;i<=segment;i++){
    points->InsertNextPoint(toplb.x, toplb.y+i, toplb.z);
    points->InsertNextPoint(bottomlb.x,
                            bottomlb.y+(float)i*(float)degree, bottomlb.z);
    if(i<segment){
    vtkSmartPointer<vtkTriangle> triangle = vtkSmartPointer<vtkTriangle>::New();
    triangle->GetPointIds()->SetId(0,i*2+count);
    triangle->GetPointIds()->SetId(1,i*2+1+count);
    triangle->GetPointIds()->SetId(2,i*2+3+count);
    triangles->InsertNextCell(triangle);
    triangle->GetPointIds()->SetId(0,i*2+count);
    triangle->GetPointIds()->SetId(1,i*2+3+count);
    triangle->GetPointIds()->SetId(2,i*2+2+count);
    triangles->InsertNextCell(triangle);
    }
  }
  count = points->GetNumberOfPoints();
  for(int i =0;i<=segment;i++){
    points->InsertNextPoint(toprb.x, toplb.y+i, toplb.z);
    points->InsertNextPoint(bottomrb.x,
                            bottomlb.y+(float)i*(float)degree, bottomlb.z);
    vtkSmartPointer<vtkTriangle> triangle = vtkSmartPointer<vtkTriangle>::New();
    if(i<segment){
    triangle->GetPointIds()->SetId(0,i*2+count);
    triangle->GetPointIds()->SetId(1,i*2+1+count);
    triangle->GetPointIds()->SetId(2,i*2+3+count);
    triangles->InsertNextCell(triangle);
    triangle->GetPointIds()->SetId(0,i*2+count);
    triangle->GetPointIds()->SetId(1,i*2+3+count);
    triangle->GetPointIds()->SetId(2,i*2+2+count);
    triangles->InsertNextCell(triangle);
    }
  }
  apolydata->SetPoints(points);
  apolydata->SetPolys(triangles);

  tbPolygon = apolydata;
/*
  vtkSmartPointer<vtkPolyDataWriter> writer =
  vtkSmartPointer<vtkPolyDataWriter>::New();
  writer->SetInput(apolydata);//triangulator->GetOutputPort());//topgrid);//wholePolygon);
  writer->SetFileName("planeconnection.vtk");
  writer->Update();
*/
}

vector<double> meshdir[3];
vector<double> meshpos[3];
void mergeWhole(){
/*  vtkSmartPointer<vtkPolyDataReader> read1 =
  vtkSmartPointer<vtkPolyDataReader>::New();
  read1->SetFileName("contoursmooth.vtk");
  read1->Update();

  vtkSmartPointer<vtkPolyDataReader> read2 =
  vtkSmartPointer<vtkPolyDataReader>::New();
  read2->SetFileName("ctconnection.vtk");
  read2->Update();

  vtkSmartPointer<vtkPolyDataReader> read3 =
  vtkSmartPointer<vtkPolyDataReader>::New();
  read3->SetFileName("top.vtk");
  read3->Update();

  vtkSmartPointer<vtkPolyDataReader> read4 =
  vtkSmartPointer<vtkPolyDataReader>::New();
  read4->SetFileName("planeconnection.vtk");
  read4->Update();

  vtkSmartPointer<vtkPolyDataReader> read5 =
  vtkSmartPointer<vtkPolyDataReader>::New();
  read5->SetFileName("bottom.vtk");
  read5->Update();
*/
  vtkSmartPointer<vtkAppendPolyData> appendFilter =
    vtkSmartPointer<vtkAppendPolyData>::New();
#if VTK_MAJOR_VERSION <= 5
  appendFilter->AddInput(contourPolygon);//read1->GetOutputPort());
  appendFilter->AddInput(ctPolygon);//Connection(read2->GetOutputPort());
  appendFilter->AddInput(topPolygon);//Connection(read3->GetOutputPort());
  appendFilter->AddInput(tbPolygon);//Connection(read4->GetOutputPort());
  appendFilter->AddInput(bottomPolygon);//Connection(read5->GetOutputPort());
#else
  appendFilter->AddInputData(contourPolygon);//read1->GetOutputPort());
  appendFilter->AddInputData(ctPolygon);//Connection(read2->GetOutputPort());
  appendFilter->AddInputData(topPolygon);//Connection(read3->GetOutputPort());
  appendFilter->AddInputData(tbPolygon);//Connection(read4->GetOutputPort());
  appendFilter->AddInputData(bottomPolygon);//Connection(read5->GetOutputPort());
#endif
/*
  appendFilter->AddInputConnection(read1->GetOutputPort());
  appendFilter->AddInputConnection(read2->GetOutputPort());
  appendFilter->AddInputConnection(read3->GetOutputPort());
  appendFilter->AddInputConnection(read4->GetOutputPort());
  appendFilter->AddInputConnection(read5->GetOutputPort());
*/
  appendFilter->Update();

  vtkSmartPointer<vtkCleanPolyData> cleanFilter =
    vtkSmartPointer<vtkCleanPolyData>::New();
  cleanFilter->SetInputConnection(appendFilter->GetOutputPort());
  cleanFilter->Update();

  vtkSmartPointer<vtkPolyDataNormals> normalGenerator =
      vtkSmartPointer<vtkPolyDataNormals>::New();
  normalGenerator->SetInputConnection(cleanFilter->GetOutputPort());
  normalGenerator->ComputePointNormalsOff();
  normalGenerator->ComputeCellNormalsOn();
  normalGenerator->Update();

  vtkSmartPointer<vtkPolyData>  wholePolygon = 
      vtkSmartPointer<vtkPolyData>::New();
  wholePolygon = normalGenerator->GetOutput();
  double nn[3];
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
}
void saveFiles(char *ofname){
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
}
int main(int argc, char **argv){
  contourPolygon = vtkSmartPointer<vtkPolyData>::New();
  ctPolygon = vtkSmartPointer<vtkPolyData>::New();
  topPolygon = vtkSmartPointer<vtkPolyData>::New();
  tbPolygon = vtkSmartPointer<vtkPolyData>::New();
  bottomPolygon = vtkSmartPointer<vtkPolyData>::New();

  threshold1=atof(argv[1]);
  threshold2=atof(argv[2]);

  readFiles(strdup(argv[3]));
  smoothTop(); 
  smoothBottom();
  meshConstruction();
  sphereSmooth();
  topProcess();
  bottomProcess();
  mergeMeshes();
  mergePlane();
  mergeWhole();
  saveFiles(argv[4]);

  return 0;
}
