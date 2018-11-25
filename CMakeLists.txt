cmake_minimum_required(VERSION 2.4)

set (CMAKE_CXX_FLAGS "-Wno-deprecated -Dcimg_display=0")

# OpenCV Config
find_package(OpenCV REQUIRED)
message("-- OPENCV include:   " ${OpenCV_INCLUDE_DIRS})
message("-- OPENCV libs dir:  " ${OpenCV_LIB_DIR})
message("-- OPENCV libs:   " ${OpenCV_LIBS} )

find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

INCLUDE_DIRECTORIES(${OpenCV_INCLUDE_DIRS})
LINK_DIRECTORIES(${OpenCV_LIB_DIR})

ADD_EXECUTABLE(
  blindconsistency
  regularization.cpp 
  ap.cpp 
  patchmatch/allegro_emu.cpp 
  patchmatch/knn.cpp 
  patchmatch/nn.cpp 
  patchmatch/patch.cpp 
  patchmatch/simnn.cpp 
  patchmatch/vecnn.cpp
)

TARGET_LINK_LIBRARIES(
  blindconsistency
  ${OPENCV_LIBRARIES}
  ${OpenCV_LIBS}
)
