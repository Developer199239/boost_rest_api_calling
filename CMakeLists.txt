cmake_minimum_required(VERSION 3.5)
project(restApi)

if(WIN32)
  if(CYGWIN)
    message("CYGWIN Not Supported for RTLite-cloud-websocket-server-async")
  else(CYGWIN)
    message("WIN32 for RTLite-cloud-websocket-server-async")
  endif(CYGWIN)
else(WIN32)
  if(UNIX)
    if(APPLE)
      message("APPLE for RTLite-cloud-websocket-server-async")
    else(APPLE)
      message("UNIX for RTLite-cloud-websocket-server-async")
    endif(APPLE)
  else(UNIX)
    message(SEND_ERROR "Unable to determine platform")
  endif(UNIX)
endif(WIN32)

get_cmake_property(CACHE_VARS CACHE_VARIABLES)
foreach(CACHE_VAR ${CACHE_VARS})
  if(${CACHE_VAR} STREQUAL CMAKE_BUILD_TYPE)
    message("FOUND CMAKE_BUILD_TYPE with value ${${CACHE_VAR}}")
    if(${${CACHE_VAR}} STREQUAL "Debug")
      message("FOUND CMAKE_BUILD_TYPE equals Debug, forcing this with set FORCE_DEBUG")
      set(FORCE_DEBUG "FORCE")
    endif()
    if(${${CACHE_VAR}} STREQUAL "DEBUG")
      message("FOUND CMAKE_BUILD_TYPE equals Debug, forcing this with set FORCE_DEBUG")
      set(FORCE_DEBUG "FORCE")
    endif()
    if(${${CACHE_VAR}} STREQUAL "MinSizeRel")
      message("FOUND CMAKE_BUILD_TYPE equals MinSizeRel, forcing this with set FORCE_DEBUG to MinSizeRel")
      set(FORCE_DEBUG "MinSizeRel")
    endif()
  endif()
  get_property(CACHE_VAR_HELPSTRING CACHE ${CACHE_VAR} PROPERTY HELPSTRING)
  if(CACHE_VAR_HELPSTRING STREQUAL "No help, variable specified on the command line.")
    get_property(CACHE_VAR_TYPE CACHE ${CACHE_VAR} PROPERTY TYPE)
    if(CACHE_VAR_TYPE STREQUAL "UNINITIALIZED")
      set(CACHE_VAR_TYPE)
    else()
      set(CACHE_VAR_TYPE :${CACHE_VAR_TYPE})
    endif()
    set(CMAKE_ARGS "${CMAKE_ARGS} -D${CACHE_VAR}${CACHE_VAR_TYPE}=\"${${CACHE_VAR}}\"")
  endif()
endforeach()
message("CMAKE_ARGS: ${CMAKE_ARGS}")

if(FORCE_DEBUG STREQUAL "FORCE")
  set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Default build type: Debug" FORCE)
  message("FOUND FORCE_DEBUG equals FORCE, forcing this with set(CMAKE_BUILD_TYPE \"Debug\" CACHE STRING \"Default build type: Debug\" FORCE)")
else()
  if(FORCE_DEBUG STREQUAL "MinSizeRel")
    set(CMAKE_BUILD_TYPE MinSizeRel)
    message("set(CMAKE_BUILD_TYPE MinSizeRel)")
  else()
    set(CMAKE_BUILD_TYPE Release)
    message("set(CMAKE_BUILD_TYPE Release)")
  endif()
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_definitions("-std=c++2a")
endif()

set(CMAKE_FIND_PACKAGE_PREFER_CONFIG TRUE)
if(FORCE_DEBUG STREQUAL "FORCE")
  set(Boost_DEBUG ON)
else()
  set(Boost_DEBUG OFF)
endif()

list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
find_package(Boost)
#find_package(Boost REQUIRED COMPONENTS system filesystem log thread)
include_directories(${Boost_INCLUDE_DIRS})

set(OPENSSL_USE_STATIC_LIBS ON)
find_package(OpenSSL REQUIRED)
message("OPENSSL_INCLUDE_DIRS = ${OPENSSL_INCLUDE_DIR})")
include_directories(${OPENSSL_INCLUDE_DIR})

message("CMAKE_INCLUDE_PATH = ${CMAKE_INCLUDE_PATH}")

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a")
endif()
message(${CMAKE_CXX_FLAGS})

if (WIN32)
  add_definitions(-D_WIN32_WINNT=0x0501)
endif(WIN32)

if (WIN32)
  #set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /ignore:4099")
endif(WIN32)

if(FORCE_DEBUG STREQUAL "FORCE")
  if(WIN32)
    SET(dir "${CMAKE_HOME_DIRECTORY}\\debug")
    SET(CMAKE_INSTALL_PREFIX ${dir} CACHE PATH "default install path" FORCE)
  else(WIN32)
    SET(dir "${CMAKE_HOME_DIRECTORY}/debug")
    SET(CMAKE_INSTALL_PREFIX ${dir} CACHE PATH "default install path" FORCE)
  endif(WIN32)
else()
  if(FORCE_DEBUG STREQUAL "MinSizeRel")
    if(WIN32)
      SET(dir "${CMAKE_HOME_DIRECTORY}\\MinSizeRel")
      SET(CMAKE_INSTALL_PREFIX ${dir} CACHE PATH "default install path" FORCE)
    else(WIN32)
      SET(dir "${CMAKE_HOME_DIRECTORY}/MinSizeRel")
      SET(CMAKE_INSTALL_PREFIX ${dir} CACHE PATH "default install path" FORCE)
    endif(WIN32)
  else()
    if(WIN32)
      SET(dir "${CMAKE_HOME_DIRECTORY}\\release")
      SET(CMAKE_INSTALL_PREFIX ${dir} CACHE PATH "default install path" FORCE)
    else(WIN32)
      SET(dir "${CMAKE_HOME_DIRECTORY}/release")
      SET(CMAKE_INSTALL_PREFIX ${dir} CACHE PATH "default install path" FORCE)
    endif(WIN32)
  endif()
endif()

# set(InternalClientSrcs
#     ../Common/InternalClient.cpp)
# add_library(InternalClient STATIC ${InternalClientSrcs})
# target_link_libraries(InternalClient PUBLIC ${CONAN_LIBS})

# set(InternalServerSrcs
#     ../Common/InternalServer.cpp)
# add_library(InternalServer STATIC ${InternalServerSrcs})
# target_link_libraries(InternalServer PUBLIC ${CONAN_LIBS})

# set(ToCloudTransmitBufferListenerSrcs
#     ../Common/ToCloudTransmitBufferListener.cpp)
# add_library(ToCloudTransmitBufferListener STATIC ${ToCloudTransmitBufferListenerSrcs})
# target_link_libraries(ToCloudTransmitBufferListener PUBLIC ${CONAN_LIBS})

# set(ToCloudTransmitBufferListenerManagerSrcs
#     ../Common/ToCloudTransmitBufferListenerManager.cpp)
# add_library(ToCloudTransmitBufferListenerManager STATIC ${ToCloudTransmitBufferListenerManagerSrcs})
# target_link_libraries(ToCloudTransmitBufferListenerManager PUBLIC ${CONAN_LIBS})

# set(ToInternalTransmitBufferListenerSrcs
#     ../Common/ToInternalTransmitBufferListener.cpp)
# add_library(ToInternalTransmitBufferListener STATIC ${ToInternalTransmitBufferListenerSrcs})
# target_link_libraries(ToInternalTransmitBufferListener PUBLIC ${CONAN_LIBS})

# set(ToInternalTransmitBufferListenerManagerSrcs
#     ../Common/ToInternalTransmitBufferListenerManager.cpp)
# add_library(ToInternalTransmitBufferListenerManager STATIC ${ToInternalTransmitBufferListenerManagerSrcs})
# target_link_libraries(ToInternalTransmitBufferListenerManager PUBLIC ${CONAN_LIBS})

# set(LoggingHandlerSrcs
#     ../Common/LoggingHandler.cpp)
# add_library(LoggingHandler STATIC ${LoggingHandlerSrcs})
# target_link_libraries(LoggingHandler PUBLIC ${CONAN_LIBS})

# set(LoggingHandlerManagerSrcs
#     ../Common/LoggingHandlerManager.cpp)
# add_library(LoggingHandlerManager STATIC ${LoggingHandlerManagerSrcs})
# target_link_libraries(LoggingHandlerManager PUBLIC ${CONAN_LIBS})

# set(TraceUserBaseClassIdentifierLoggingNameGeneratorSrcs
#     ../Common/TraceUserBaseClassIdentifierLoggingNameGenerator.cpp)
# add_library(TraceUserBaseClassIdentifierLoggingNameGenerator STATIC ${TraceUserBaseClassIdentifierLoggingNameGeneratorSrcs})
# target_link_libraries(TraceUserBaseClassIdentifierLoggingNameGenerator PUBLIC ${CONAN_LIBS})


add_executable(restApi restApi.cpp)
target_link_libraries(restApi PUBLIC ${CONAN_LIBS}
  ${OPENSSL_LIBRARIES}
#   InternalClient InternalServer
#   LoggingHandler LoggingHandlerManager
#   ToCloudTransmitBufferListener ToCloudTransmitBufferListenerManager
#   ToInternalTransmitBufferListener ToInternalTransmitBufferListenerManager
#   TraceUserBaseClassIdentifierLoggingNameGenerator
  )
install(TARGETS restApi DESTINATION bin)

get_cmake_property(CACHE_VARS CACHE_VARIABLES)
foreach(CACHE_VAR ${CACHE_VARS})
  message("CACHE_VAR => ${CACHE_VAR} ${${CACHE_VAR}}")
endforeach()
