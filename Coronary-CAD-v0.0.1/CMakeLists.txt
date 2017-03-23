CMAKE_MINIMUM_REQUIRED(VERSION 2.4)

IF(WIN32)
   SET(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install" CACHE PATH "Installation Directory")
   MESSAGE(STATUS "WIN32")
ELSE()
   SET(CMAKE_INSTALL_PREFIX "/usr/local" CACHE PATH "Installation Directory")
ENDIF()

PROJECT(Coronary-CAD-v0.0.1-test)

SET (EXECUTABLE_OUTPUT_PATH  ${PROJECT_BINARY_DIR}/bin CACHE INTERNAL
  "Where to put the executables")

SET (LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib CACHE INTERNAL
  "Where to put the libraries")
 
IF(WIN32)
   SET (VAC_DEBUG_PREFIX "d") 
ELSE()
   SET (VAC_DEBUG_PREFIX "") 
ENDIF()

# Setup the path to load our macros
SET (CMAKE_MODULE_PATH "${UCLTK_BINARY_DIR}/CMakeModules")

# Open the property setting
SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)
SET_PROPERTY(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "CMakeTargets")

SUBDIRS(
    Apps 
    vacClass
	ExtLibs
)

INCLUDE_DIRECTORIES (${PROJECT_BINARY_DIR})
INCLUDE_DIRECTORIES (${PROJECT_SOURCE_DIR}/vacClass/vacCmprTransformer
					 ${PROJECT_SOURCE_DIR}/vacClass/vac3Dto2DTransformer
					 ${PROJECT_SOURCE_DIR}/vacClass/vacMainInterface
					 ${PROJECT_SOURCE_DIR}/vacClass/vacPointReader
					 ${PROJECT_SOURCE_DIR}/vacClass/vacQtView
					 ${PROJECT_SOURCE_DIR}/vacClass/vacQtWidgets
					 ${PROJECT_SOURCE_DIR}/vacClass/vacVnandLsTransformer
												
)

INCLUDE_DIRECTORIES (${PROJECT_SOURCE_DIR}/ExtLibs/qxt_lib/include/widgets
                     ${PROJECT_SOURCE_DIR}/ExtLibs/qxt_lib/include/core

)
LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/ExtLibs/qxt_lib/lib)

ADD_DEFINITIONS(-DQXT_STATIC)
ADD_DEFINITIONS(-DQXT_DOXYGEN_RUN)

if(COMMAND cmake_policy)
   cmake_policy(SET CMP0003 NEW)
   cmake_policy(SET CMP0015 NEW)
endif(COMMAND cmake_policy)


#SET ITK
SET(ITK_FOUND FALSE)
FIND_PACKAGE(ITK )
IF(ITK_FOUND)
  MESSAGE(STATUS, "Found itk4 at ${ITK_USE_FILE}")
  INCLUDE(${ITK_USE_FILE})
ELSE(ITK_FOUND)
  MESSAGE(FATAL_ERROR
     "Cannot build BIGRCarotid without ITK.  Please set ITK_DIR.")
ENDIF(ITK_FOUND)

#SET VTK
FIND_PACKAGE(VTK)
IF(VTK_FOUND)
   INCLUDE(${USE_VTK_FILE})
else(VTK_FOUND)
   message(FATAL_ERROR 
	"Cannot build without VTK.Please set VTK_DIR")
ENDIF(VTK_FOUND)

# For OpenCV
FIND_PACKAGE(OpenCV REQUIRED)
INCLUDE_DIRECTORIES(${OpenCV_INCLUDE_DIRS})

#FOR qt
FIND_PACKAGE(Qt4 REQUIRED)
INCLUDE (${QT_USE_FILE})
ADD_DEFINITIONS (${QT_DEFINITIONS})

FIND_PACKAGE(Boost)
IF(Boost_FOUND)
  INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIRS})
  LINK_DIRECTORIES( ${Boost_LIBRARY_DIRS})
ELSE(Boost_FOUND)
  MESSAGE(FATAL_ERROR
          "Cannot build without boost. Please set BOOST_DIR.")
ENDIF(Boost_FOUND)


