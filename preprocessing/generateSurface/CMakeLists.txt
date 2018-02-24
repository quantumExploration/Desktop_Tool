cmake_minimum_required(VERSION 2.8)
 
PROJECT(SurfaceFromUnorganizedPointsWithPostProc)
 
find_package(VTK REQUIRED)
include(${VTK_USE_FILE})
 
add_executable(SurfaceFromUnorganizedPointsWithPostProc MACOSX_BUNDLE main.cpp
vtkPowerCrustSurfaceReconstruction.cxx)
 
if(VTK_LIBRARIES)
  target_link_libraries(SurfaceFromUnorganizedPointsWithPostProc ${VTK_LIBRARIES})
else()
  target_link_libraries(SurfaceFromUnorganizedPointsWithPostProc vtkHybrid vtkWidgets)
endif()

