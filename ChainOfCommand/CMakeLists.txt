# CMakeList.txt : CMake project for ChainOfCommand, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# include Boost
#===========================================================================
set(Boost_INCLUDE_DIR "C:/myprojects/boost_1_81_0")
include_directories(${Boost_INCLUDE_DIR})

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("ChainOfCommand")

# Add source to this project's executable.
add_executable (ChainOfCommand "ChainOfCommand.cpp" "ChainOfCommand.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ChainOfCommand PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
