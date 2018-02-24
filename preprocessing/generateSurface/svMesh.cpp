#include "svMesh.h"

int main(int argc, char *argv[])
{
 
  vtkSmartPointer<vtkPolyData> input;
  if(argc > 1)
    {
    vtkSmartPointer<vtkXMLPolyDataReader> reader =
      vtkSmartPointer<vtkXMLPolyDataReader>::New();
    reader->SetFileName(argv[1]);
    reader->Update();
    input = reader->GetOutput();
    }
  else
    {
    vtkSmartPointer<vtkSphereSource> sphereSource =
      vtkSmartPointer<vtkSphereSource>::New();
    sphereSource->Update();
    input = sphereSource->GetOutput();
    }
 
  vtkSmartPointer<vtkPolyData> polydata = 
    vtkSmartPointer<vtkPolyData>::New();
  polydata->SetPoints(input->GetPoints());

  vtkSmartPointer<vtkSurfaceReconstructionFilter> surf = 
    vtkSmartPointer<vtkSurfaceReconstructionFilter>::New();
#if VTK_MAJOR_VERSION <= 5
  surf->SetInput(polydata);
#else
  surf->SetInputData(polydata);
#endif

  vtkSmartPointer<vtkContourFilter> cf = 
    vtkSmartPointer<vtkContourFilter>::New();
  cf->SetInputConnection(surf->GetOutputPort());
  cf->SetValue(0, 0.0);

  vtkSmartPointer<vtkReverseSense> reverse = 
    vtkSmartPointer<vtkReverseSense>::New();
  reverse->SetInputConnection(cf->GetOutputPort());
  reverse->ReverseCellsOn();
  reverse->ReverseNormalsOn();
 
  vtkSmartPointer<vtkPolyDataMapper> map = 
    vtkSmartPointer<vtkPolyDataMapper>::New();
  map->SetInputConnection(reverse->GetOutputPort());
  map->ScalarVisibilityOff();
 
  vtkSmartPointer<vtkActor> surfaceActor = 
    vtkSmartPointer<vtkActor>::New();
  surfaceActor->SetMapper(map);

}
