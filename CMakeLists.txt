cmake_minimum_required(VERSION 2.6)

PROJECT(Arb2PWL)

FIND_PACKAGE(ITK REQUIRED ITKCommon ITKIOPNG ITKImageIntensity
ITKMathematicalMorphology ITKBinaryMathematicalMorphology ITKRegionGrowing
ITKImageFusion ITKColormap
)
INCLUDE(${ITK_USE_FILE})

FIND_PACKAGE(Qt4 REQUIRED)
INCLUDE(${QT_USE_FILE})

QT4_WRAP_UI(UISrcs form.ui)
QT4_WRAP_CPP(MOCSrcs form.h  ClickableScene.h CustomGraphicsView.h)

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

# The hxx files are only added so that they are included in the QtCreator/VS project.
add_library(AutoTrace AutoTrace.cpp MooreTracing.cpp
AutoTrace.hxx
Helpers.hxx)

ADD_EXECUTABLE(Arb2PWL main.cpp ClickableScene.cpp form.cpp ${MOCSrcs} ${UISrcs})
TARGET_LINK_LIBRARIES(Arb2PWL ${QT_LIBRARIES} ${ITK_LIBRARIES} AutoTrace)

ADD_EXECUTABLE(TestAutoTrace Tests/TestAutoTrace.cpp)
TARGET_LINK_LIBRARIES(TestAutoTrace ${QT_LIBRARIES} ${ITK_LIBRARIES} AutoTrace)

IF(WIN32)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mwindows")
ENDIF()
