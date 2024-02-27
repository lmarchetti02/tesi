# SETUP
#-----------------------------------------------------------------------
cmake_minimum_required(VERSION 3.26 FATAL_ERROR)
set(CMAKE_CXX_STANDARD 20)
project(Rivelatore)

# delete root files
set(results_path ${PROJECT_SOURCE_DIR}/results/)
file(GLOB results "${results_path}/*.root")

if(NOT results)
    message("ROOT directory is empty")
else()
  file(REMOVE_RECURSE ${results})
  message("ROOT files deleted")
endif()

# look for GEANT4
find_package(Geant4 REQUIRED ui_all vis_all)

# include necessary files and directories
include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/include/utils)

# compile every source and header file in project directory
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cpp)
file(GLOB utils_cpp ${PROJECT_SOURCE_DIR}/src/utils/*.cpp)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)
file(GLOB utils_hh ${PROJECT_SOURCE_DIR}/include/utils/*.hh)

# copy macro files to build directory
set(SCRIPTS vis.mac run.mac compile.sh)

foreach(_script ${SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()

# create executable and link it to Geant4 libraries
add_executable(rivelatore main.cpp ${sources} ${headers} ${utils_cpp} ${utils_hh})
target_link_libraries(rivelatore ${Geant4_LIBRARIES})

# install executable
install(TARGETS rivelatore DESTINATION bin)
#-----------------------------------------------------------------------
