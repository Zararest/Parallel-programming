cmake_minimum_required(VERSION 3.13.4)
project(e-calc)

find_package(MPI REQUIRED)
find_package(Boost REQUIRED)

include_directories(SYSTEM ${MPI_INCLUDE_PATH})

add_executable(${PROJECT_NAME} e-task.cpp)
target_link_libraries(${PROJECT_NAME} Boost::headers)
target_link_libraries(${PROJECT_NAME} ${MPI_C_LIBRARIES})

add_compile_options(-lgmp)
add_compile_options(-lmpi_cxx)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../)

find_library(GMP_LIBRARY NAMES gmp)
target_link_libraries(${PROJECT_NAME} gmp)
target_link_libraries(${PROJECT_NAME} mpi_cxx)