nclude <vtkVersion.h>
#include <vtkSmartPointer.h>
 
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
#include <vtkSphereSource.h>
#include <vtkXMLPolyDataReader.h>i


#include <fstream>
#include <algorithm>
#include <cmath>
#include <vector>

using namespace std;


class svMesh{
public:
 svMesh();
 ~svMesh();

 void ComputeMesh(char *ifname, char *ofname); 
};
