cmake_minimum_required(VERSION 2.8)
 
PROJECT(GenerateMesh)
 
find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

add_executable(GenerateMesh MACOSX_BUNDLE GenerateMesh.cpp)
 
if(VTK_LIBRARIES)
  target_link_libraries(GenerateMesh ${VTK_LIBRARIES} )
else()
  target_link_libraries(GenerateMesh vtkHybrid vtkWidgets )
endif()
