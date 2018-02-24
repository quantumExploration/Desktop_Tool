# Install script for directory: /home/davinci/Documents/henan/NIST/NIST_SPLIT/SPLIT_VIS2/preprocessing/generateVTK/Convex_hull_3/examples/Convex_hull_3/CGAL-4.7

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
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/doc/CGAL-4.7" TYPE FILE FILES
    "/home/davinci/Documents/henan/NIST/NIST_SPLIT/SPLIT_VIS2/preprocessing/generateVTK/Convex_hull_3/examples/Convex_hull_3/CGAL-4.7/AUTHORS"
    "/home/davinci/Documents/henan/NIST/NIST_SPLIT/SPLIT_VIS2/preprocessing/generateVTK/Convex_hull_3/examples/Convex_hull_3/CGAL-4.7/CHANGES"
    "/home/davinci/Documents/henan/NIST/NIST_SPLIT/SPLIT_VIS2/preprocessing/generateVTK/Convex_hull_3/examples/Convex_hull_3/CGAL-4.7/LICENSE"
    "/home/davinci/Documents/henan/NIST/NIST_SPLIT/SPLIT_VIS2/preprocessing/generateVTK/Convex_hull_3/examples/Convex_hull_3/CGAL-4.7/LICENSE.FREE_USE"
    "/home/davinci/Documents/henan/NIST/NIST_SPLIT/SPLIT_VIS2/preprocessing/generateVTK/Convex_hull_3/examples/Convex_hull_3/CGAL-4.7/LICENSE.GPL"
    "/home/davinci/Documents/henan/NIST/NIST_SPLIT/SPLIT_VIS2/preprocessing/generateVTK/Convex_hull_3/examples/Convex_hull_3/CGAL-4.7/LICENSE.LGPL"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "/home/davinci/Documents/henan/NIST/NIST_SPLIT/SPLIT_VIS2/preprocessing/generateVTK/Convex_hull_3/examples/Convex_hull_3/CGAL-4.7/include/CGAL" REGEX "/\\.svn$" EXCLUDE)
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "/home/davinci/Documents/henan/NIST/NIST_SPLIT/SPLIT_VIS2/preprocessing/generateVTK/Convex_hull_3/examples/Convex_hull_3/CGAL-4.7/build/include/CGAL" REGEX "/\\.svn$" EXCLUDE)
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE PROGRAM FILES
    "/home/davinci/Documents/henan/NIST/NIST_SPLIT/SPLIT_VIS2/preprocessing/generateVTK/Convex_hull_3/examples/Convex_hull_3/CGAL-4.7/scripts/cgal_create_cmake_script"
    "/home/davinci/Documents/henan/NIST/NIST_SPLIT/SPLIT_VIS2/preprocessing/generateVTK/Convex_hull_3/examples/Convex_hull_3/CGAL-4.7/scripts/cgal_make_macosx_app"
    "/home/davinci/Documents/henan/NIST/NIST_SPLIT/SPLIT_VIS2/preprocessing/generateVTK/Convex_hull_3/examples/Convex_hull_3/CGAL-4.7/scripts/cgal_create_CMakeLists"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/CGAL" TYPE DIRECTORY FILES "/home/davinci/Documents/henan/NIST/NIST_SPLIT/SPLIT_VIS2/preprocessing/generateVTK/Convex_hull_3/examples/Convex_hull_3/CGAL-4.7/cmake/modules/")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/CGAL" TYPE FILE FILES "/home/davinci/Documents/henan/NIST/NIST_SPLIT/SPLIT_VIS2/preprocessing/generateVTK/Convex_hull_3/examples/Convex_hull_3/CGAL-4.7/cmake/modules/UseCGAL.cmake")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  IF(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/CGAL/CGALExports.cmake")
    FILE(DIFFERENT EXPORT_FILE_CHANGED FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/CGAL/CGALExports.cmake"
         "/home/davinci/Documents/henan/NIST/NIST_SPLIT/SPLIT_VIS2/preprocessing/generateVTK/Convex_hull_3/examples/Convex_hull_3/CGAL-4.7/build/CMakeFiles/Export/lib/CGAL/CGALExports.cmake")
    IF(EXPORT_FILE_CHANGED)
      FILE(GLOB OLD_CONFIG_FILES "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/CGAL/CGALExports-*.cmake")
      IF(OLD_CONFIG_FILES)
        MESSAGE(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/CGAL/CGALExports.cmake\" will be replaced.  Removing files [${OLD_CONFIG_FILES}].")
        FILE(REMOVE ${OLD_CONFIG_FILES})
      ENDIF(OLD_CONFIG_FILES)
    ENDIF(EXPORT_FILE_CHANGED)
  ENDIF()
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/CGAL" TYPE FILE FILES "/home/davinci/Documents/henan/NIST/NIST_SPLIT/SPLIT_VIS2/preprocessing/generateVTK/Convex_hull_3/examples/Convex_hull_3/CGAL-4.7/build/CMakeFiles/Export/lib/CGAL/CGALExports.cmake")
  IF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/CGAL" TYPE FILE FILES "/home/davinci/Documents/henan/NIST/NIST_SPLIT/SPLIT_VIS2/preprocessing/generateVTK/Convex_hull_3/examples/Convex_hull_3/CGAL-4.7/build/CMakeFiles/Export/lib/CGAL/CGALExports-release.cmake")
  ENDIF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/CGAL" TYPE FILE FILES "/home/davinci/Documents/henan/NIST/NIST_SPLIT/SPLIT_VIS2/preprocessing/generateVTK/Convex_hull_3/examples/Convex_hull_3/CGAL-4.7/build/config/CGALConfig.cmake")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/man/man1" TYPE FILE FILES "/home/davinci/Documents/henan/NIST/NIST_SPLIT/SPLIT_VIS2/preprocessing/generateVTK/Convex_hull_3/examples/Convex_hull_3/CGAL-4.7/auxiliary/cgal_create_cmake_script.1")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  INCLUDE("/home/davinci/Documents/henan/NIST/NIST_SPLIT/SPLIT_VIS2/preprocessing/generateVTK/Convex_hull_3/examples/Convex_hull_3/CGAL-4.7/build/src/cmake_install.cmake")

ENDIF(NOT CMAKE_INSTALL_LOCAL_ONLY)

IF(CMAKE_INSTALL_COMPONENT)
  SET(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
ELSE(CMAKE_INSTALL_COMPONENT)
  SET(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
ENDIF(CMAKE_INSTALL_COMPONENT)

FILE(WRITE "/home/davinci/Documents/henan/NIST/NIST_SPLIT/SPLIT_VIS2/preprocessing/generateVTK/Convex_hull_3/examples/Convex_hull_3/CGAL-4.7/build/${CMAKE_INSTALL_MANIFEST}" "")
FOREACH(file ${CMAKE_INSTALL_MANIFEST_FILES})
  FILE(APPEND "/home/davinci/Documents/henan/NIST/NIST_SPLIT/SPLIT_VIS2/preprocessing/generateVTK/Convex_hull_3/examples/Convex_hull_3/CGAL-4.7/build/${CMAKE_INSTALL_MANIFEST}" "${file}\n")
ENDFOREACH(file)
