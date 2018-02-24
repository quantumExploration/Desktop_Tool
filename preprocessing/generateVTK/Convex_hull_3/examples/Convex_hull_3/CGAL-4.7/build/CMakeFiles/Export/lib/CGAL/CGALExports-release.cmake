#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "CGAL::CGAL" for configuration "Release"
set_property(TARGET CGAL::CGAL APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(CGAL::CGAL PROPERTIES
  IMPORTED_LINK_INTERFACE_LIBRARIES_RELEASE "/usr/lib64/libmpfr.so;/usr/lib64/libgmp.so;/usr/lib64/libboost_thread-mt.so;/usr/lib64/libboost_system-mt.so"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libCGAL.so.11.0.1"
  IMPORTED_SONAME_RELEASE "libCGAL.so.11"
  )

list(APPEND _IMPORT_CHECK_TARGETS CGAL::CGAL )
list(APPEND _IMPORT_CHECK_FILES_FOR_CGAL::CGAL "${_IMPORT_PREFIX}/lib/libCGAL.so.11.0.1" )

# Import target "CGAL::CGAL_ImageIO" for configuration "Release"
set_property(TARGET CGAL::CGAL_ImageIO APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(CGAL::CGAL_ImageIO PROPERTIES
  IMPORTED_LINK_INTERFACE_LIBRARIES_RELEASE "/usr/lib64/libmpfr.so;/usr/lib64/libgmp.so;CGAL::CGAL;/usr/lib64/libboost_thread-mt.so;/usr/lib64/libboost_system-mt.so;/usr/lib64/libGLU.so;/usr/lib64/libGL.so;/usr/lib64/libSM.so;/usr/lib64/libICE.so;/usr/lib64/libX11.so;/usr/lib64/libXext.so;/usr/lib64/libz.so"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libCGAL_ImageIO.so.11.0.1"
  IMPORTED_SONAME_RELEASE "libCGAL_ImageIO.so.11"
  )

list(APPEND _IMPORT_CHECK_TARGETS CGAL::CGAL_ImageIO )
list(APPEND _IMPORT_CHECK_FILES_FOR_CGAL::CGAL_ImageIO "${_IMPORT_PREFIX}/lib/libCGAL_ImageIO.so.11.0.1" )

# Import target "CGAL::CGAL_Core" for configuration "Release"
set_property(TARGET CGAL::CGAL_Core APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(CGAL::CGAL_Core PROPERTIES
  IMPORTED_LINK_INTERFACE_LIBRARIES_RELEASE "/usr/lib64/libmpfr.so;/usr/lib64/libgmp.so;CGAL::CGAL;/usr/lib64/libboost_thread-mt.so;/usr/lib64/libboost_system-mt.so"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libCGAL_Core.so.11.0.1"
  IMPORTED_SONAME_RELEASE "libCGAL_Core.so.11"
  )

list(APPEND _IMPORT_CHECK_TARGETS CGAL::CGAL_Core )
list(APPEND _IMPORT_CHECK_FILES_FOR_CGAL::CGAL_Core "${_IMPORT_PREFIX}/lib/libCGAL_Core.so.11.0.1" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
