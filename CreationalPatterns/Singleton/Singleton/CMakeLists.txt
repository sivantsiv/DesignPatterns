# CMakeList.txt : CMake project for Singleton, include source and define
# project specific logic here.

# include Boost
#===========================================================================
set(Boost_INCLUDE_DIR "../../../boost")
include_directories(${Boost_INCLUDE_DIR})

# Add source to this project's executable.
add_executable (Singleton "Singleton.cpp" "Singleton.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Singleton PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
