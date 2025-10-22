cmake_minimum_required(VERSION 3.22)

# ---------- (optional) force Clang: do this BEFORE project() ----------
if(NOT DEFINED CMAKE_C_COMPILER AND NOT DEFINED CMAKE_CXX_COMPILER)
  set(CMAKE_C_COMPILER clang)
  set(CMAKE_CXX_COMPILER clang++)
endif()

# ---------- single place to rename your target ----------
set(TARGET_NAME breaking_lcg)

project(${TARGET_NAME} LANGUAGES CXX)

# Prefer mold (faster), fall back to lld
find_program(MOLD mold)
if(MOLD)
  add_link_options(-fuse-ld=mold)
else()
  add_link_options(-fuse-ld=lld)
endif()

# ---------- compiler debug options ----------
if(CMAKE_BUILD_TYPE MATCHES "Debug")
  # -Og: keep debug-friendly optimizations
  # -g: generate debug symbols
  # -fno-omit-frame-pointer: better stack traces in LLDB
  add_compile_options(-Og -g -fno-omit-frame-pointer)
endif()

# ===== Global defaults =====
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
endif()
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "RelWithDebInfo" "MinSizeRel")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ===== Feature toggles =====
option(ENABLE_EIGEN           "Use Eigen3"                            OFF)
option(ENABLE_XTENSOR         "Use xtensor"                           OFF)
option(ENABLE_XTENSOR_BLAS    "Use xtensor-blas (requires BLAS)"      OFF)
option(ENABLE_BLAS_LAPACK     "Link BLAS/LAPACK"                      OFF)
option(ENABLE_BLAZE           "Use Blaze (header-only)"               OFF)
option(ENABLE_ARRAYFIRE       "Use ArrayFire (CPU/unified)"           OFF)
option(ENABLE_DLIB            "Use dlib via pkg-config"               OFF)
option(ENABLE_OPENMP          "Enable OpenMP for C++"                 OFF)
option(ENABLE_FMT             "Use fmt formatting library"            OFF)

# ===== Target =====
add_executable(${TARGET_NAME}
  src/main.cpp
  src/second.cpp
)

target_include_directories(${TARGET_NAME} PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_compile_features(${TARGET_NAME} PRIVATE cxx_std_23)

# ===== Warnings =====
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
  target_compile_options(${TARGET_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()

# ===== Dependencies =====

# fmt
if(ENABLE_FMT)
  find_package(fmt REQUIRED)
  target_link_libraries(${TARGET_NAME} PRIVATE fmt::fmt)
endif()

# Eigen
if(ENABLE_EIGEN)
  find_package(Eigen3 3.4 REQUIRED NO_MODULE)
  target_link_libraries(${TARGET_NAME} PRIVATE Eigen3::Eigen)
endif()

# BLAS / LAPACK
if(ENABLE_BLAS_LAPACK)
  find_package(BLAS REQUIRED)
  find_package(LAPACK REQUIRED)
  target_link_libraries(${TARGET_NAME} PRIVATE BLAS::BLAS LAPACK::LAPACK)
endif()

# xtensor + xtensor-blas
if(ENABLE_XTENSOR)
  find_package(xtensor REQUIRED)
  target_link_libraries(${TARGET_NAME} PRIVATE xtensor)
endif()

if(ENABLE_XTENSOR_BLAS)
  find_package(xtensor-blas REQUIRED)
  target_link_libraries(${TARGET_NAME} PRIVATE xtensor-blas)
  if(ENABLE_BLAS_LAPACK)
    target_compile_definitions(${TARGET_NAME} PRIVATE HAVE_CBLAS=1)
  endif()
endif()

# Blaze (header-only)
if(ENABLE_BLAZE)
  include(FetchContent)
  FetchContent_Declare(
    blaze
    GIT_REPOSITORY https://bitbucket.org/blaze-lib/blaze.git
    GIT_TAG        v3.8.2
  )
  FetchContent_MakeAvailable(blaze)
  add_library(Blaze::Blaze INTERFACE IMPORTED)
  set_target_properties(Blaze::Blaze PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${blaze_SOURCE_DIR}"
  )
  target_link_libraries(${TARGET_NAME} PRIVATE Blaze::Blaze)
  if(ENABLE_BLAS_LAPACK)
    target_compile_definitions(${TARGET_NAME} PRIVATE BLAZE_BLAS_MODE=1)
  endif()
endif()

# ArrayFire
if(ENABLE_ARRAYFIRE)
  find_package(ArrayFire REQUIRED)
  if(TARGET ArrayFire::afcpu)
    set(AF_TARGET ArrayFire::afcpu)
  elseif(TARGET afcpu)
    set(AF_TARGET afcpu)
  elseif(TARGET ArrayFire::af)
    set(AF_TARGET ArrayFire::af)
  elseif(TARGET af)
    set(AF_TARGET af)
  elseif(DEFINED ArrayFire_CPU_LIBRARIES)
    add_library(AF_CPU_LEGACY INTERFACE)
    target_link_libraries(AF_CPU_LEGACY INTERFACE ${ArrayFire_CPU_LIBRARIES})
    target_include_directories(AF_CPU_LEGACY INTERFACE ${ArrayFire_INCLUDE_DIRS})
    set(AF_TARGET AF_CPU_LEGACY)
  else()
    message(FATAL_ERROR "ArrayFire found but no usable target detected.")
  endif()
  target_link_libraries(${TARGET_NAME} PRIVATE ${AF_TARGET})
endif()

# dlib
if(ENABLE_DLIB)
  find_package(PkgConfig REQUIRED)
  pkg_check_modules(dlib REQUIRED dlib-1)
  target_link_libraries(${TARGET_NAME} PRIVATE ${dlib_LIBRARIES})
  target_include_directories(${TARGET_NAME} PRIVATE ${dlib_INCLUDE_DIRS})
endif()

# OpenMP
if(ENABLE_OPENMP)
  find_package(OpenMP REQUIRED)
  target_link_libraries(${TARGET_NAME} PRIVATE OpenMP::OpenMP_CXX)
endif()

# ===== Usage hint =====
#
# cmake --preset ram-debug  -DENABLE_ARRAYFIRE=OFF
# cmake --preset ram-release -DENABLE_DLIB=ON
#
# export BUILD_FOLDER=build
#
# For Debug
# cmake -S . -B "$BUILD_FOLDER/Debug" -G Ninja -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++
# cmake --build "$BUILD_FOLDER/Debug"
#
# For Release
# cmake -S . -B "${BUILD_FOLDER}/Release" -G Ninja -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_BUILD_TYPE=Release
# cmake --build "${BUILD_FOLDER}/Release"
#
# Or use Presets (defined in CMakePresets.json):
#
# Debug
# cmake --preset ram-debug
# cmake --build --preset build-ram-debug
#
# Release
# cmake --preset ram-release
# cmake --build --preset build-ram-release