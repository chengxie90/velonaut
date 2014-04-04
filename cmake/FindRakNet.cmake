SET(RakNet_LIBRARY_Name RakNetLibStatic)
SET(RakNet_LIBRARY_Name_Debug RakNetLibStatic_d)

FIND_LIBRARY (RakNet_LIBRARY_RELEASE NAMES ${RakNet_LIBRARY_Name}
    PATHS
    ENV LD_LIBRARY_PATH
    ENV LIBRARY_PATH
    /usr/lib64
    /usr/lib
    /usr/local/lib64
    /usr/local/lib
    /opt/local/lib
	$ENV{RAKNET_ROOT}/lib
    $ENV{RAKNET_ROOT}/lib/Release
    )
	
FIND_LIBRARY (RakNet_LIBRARY_DEBUG NAMES  ${RakNet_LIBRARY_Name_Debug}
    PATHS
    ENV LD_LIBRARY_PATH
    ENV LIBRARY_PATH
    /usr/lib64
    /usr/lib
    /usr/local/lib64
    /usr/local/lib
    /opt/local/lib
	$ENV{RAKNET_ROOT}/lib
    $ENV{RAKNET_ROOT}/lib/Debug
    )	
	
	

FIND_PATH (RakNet_INCLUDES RakPeer.h
    ENV CPATH
    /usr/include
    /usr/local/include
    /opt/local/include
	$ENV{RAKNET_ROOT}/include
    )

IF(RakNet_INCLUDES AND RakNet_LIBRARY_RELEASE)
    SET(RakNet_FOUND TRUE)
ENDIF(RakNet_INCLUDES AND RakNet_LIBRARY_RELEASE)

IF(RakNet_FOUND)
  SET(RakNet_INCLUDE_DIRS ${RakNet_INCLUDES})
  
  
   IF (CMAKE_CONFIGURATION_TYPES OR CMAKE_BUILD_TYPE)
        SET(RakNet_LIBRARIES optimized ${RakNet_LIBRARY_RELEASE} debug ${RakNet_LIBRARY_DEBUG})
      ELSE()
        # if there are no configuration types and CMAKE_BUILD_TYPE has no value
        # then just use the release libraries
        SET(RakNet_LIBRARIES ${RakNet_LIBRARY_RELEASE} )
      ENDIF()
  IF(NOT RakNet_FIND_QUIETLY)
    MESSAGE(STATUS "Found RakNet: ${RakNet_LIBRARY}")
  ENDIF(NOT RakNet_FIND_QUIETLY)
ELSE(RakNet_FOUND)
  IF(RakNet_FIND_REQUIRED)
    MESSAGE(FATAL_ERROR "Could not find RakNet")
  ENDIF(RakNet_FIND_REQUIRED)
ENDIF(RakNet_FOUND)

