# - Try to find libRocket
# Once done this will define
#  LIBROCKET_FOUND - System has libRocket
#  LIBROCKET_INCLUDE_DIRS - The LibRocket include directories
#  LIBROCKET_LIBRARIES - The libraries needed to use LibRocket
#  LIBROCKET_DEFINITIONS - Compiler switches required for using LibRocket

find_path(LIBROCKET_INCLUDE_DIR Rocket/Core.h
		  HINTS
		  $ENV{ROCKET_DIR}
		  PATH_SUFFIXES include Rocket)

find_library(LIBROCKET_LIBRARY_CORE 
  NAMES RocketCore RocketCore_d RocketCoreOSX
  HINTS 
  $ENV{ROCKET_DIR}
  PATH_SUFFIXES lib)

find_library(LIBROCKET_LIBRARY_CONTROLS 
  NAMES RocketControls RocketControls_d RocketControlsOSX
  HINTS $ENV{ROCKET_DIR}
  PATH_SUFFIXES lib)

find_library(LIBROCKET_LIBRARY_DEBUGGER 
  NAMES RocketDebugger RocketDebugger_d RocketDebuggerOSX
  HINTS $ENV{ROCKET_DIR}
  PATH_SUFFIXES lib)

set(LIBROCKET_LIBRARIES ${LIBROCKET_LIBRARY_CORE} ${LIBROCKET_LIBRARY_CONTROLS} ${LIBROCKET_LIBRARY_DEBUGGER})
set(LIBROCKET_INCLUDE_DIRS ${LIBROCKET_INCLUDE_DIR} )

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set LIBROCKET_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(LibRocket DEFAULT_MSG
                                  LIBROCKET_LIBRARY_CORE LIBROCKET_LIBRARY_CONTROLS  LIBROCKET_LIBRARY_DEBUGGER LIBROCKET_INCLUDE_DIR)

mark_as_advanced(LIBROCKET_INCLUDE_DIR LIBROCKET_LIBRARY_CORE LIBROCKET_LIBRARY_CONTROLS LIBROCKET_LIBRARY_DEBUGGER )
