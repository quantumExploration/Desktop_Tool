# Install script for directory: /home/davinci/Documents/henan/NIST/NIST_SPLIT/SPLIT_VIS2/preprocessing/generateVTK/Convex_hull_3/examples/Convex_hull_3/CGAL-4.7/src/CGAL_ImageIO

# Set the install prefix
IF(NOT DEFINED CMAKE_INSTALL_PREFIX)
  SET(CMAKE_INSTALL_PREFIX "/home/davinci/Documents/henan/NIST/NIST_SPLIT/SPLIT_VIS2/preprocessing/generateVTK/Convex_hull_3/examples/Convex_hull_3/CGAL-4.7/install")
ENDIF(NOT DEFINED CMAKE_INSTALL_PREFIX)
STRING(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
IF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  IF(BUILD_TYPE)
    STRING(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  ELSE(BUILD_TYPE)
    SET(CMAKE_INSTALL_CONFIG_NAME "Release")
  ENDIF(BUILD_TYPE)
  MESSAGE(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
ENDIF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)

# Set the component getting installed.
IF(NOT CMAKE_INSTALL_COMPONENT)
  IF(COMPONENT)
    MESSAGE(STATUS "Install component: \"${COMPONENT}\"")
    SET(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  ELSE(COMPONENT)
    SET(CMAKE_INSTALL_COMPONENT)
  ENDIF(COMPONENT)
ENDIF(NOT CMAKE_INSTALL_COMPONENT)

# Install shared libraries without execute permission?
IF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  SET(CMAKE_INSTALL_SO_NO_EXE "0")
ENDIF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FOREACH(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libCGAL_ImageIO.so.11.0.1"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libCGAL_ImageIO.so.11"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libCGAL_ImageIO.so"
      )
    IF(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      FILE(RPATH_CHECK
           FILE "${file}"
           RPATH "")
    ENDIF()
  ENDFOREACH()
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES
    "/home/davinci/Documents/henan/NIST/NIST_SPLIT/SPLIT_VIS2/preprocessing/generateVTK/Convex_hull_3/examples/Convex_hull_3/CGAL-4.7/build/lib/libCGAL_ImageIO.so.11.0.1"
    "/home/davinci/Documents/henan/NIST/NIST_SPLIT/SPLIT_VIS2/preprocessing/generateVTK/Convex_hull_3/examples/Convex_hull_3/CGAL-4.7/build/lib/libCGAL_ImageIO.so.11"
    "/home/davinci/Documents/henan/NIST/NIST_SPLIT/SPLIT_VIS2/preprocessing/generateVTK/Convex_hull_3/examples/Convex_hull_3/CGAL-4.7/build/lib/libCGAL_ImageIO.so"
    )
  FOREACH(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libCGAL_ImageIO.so.11.0.1"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libCGAL_ImageIO.so.11"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libCGAL_ImageIO.so"
      )
    IF(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      FILE(RPATH_REMOVE
           FILE "${file}")
      IF(CMAKE_INSTALL_DO_STRIP)
        EXECUTE_PROCESS(COMMAND "/usr/bin/strip" "${file}")
      ENDIF(CMAKE_INSTALL_DO_STRIP)
    ENDIF()
  ENDFOREACH()
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

