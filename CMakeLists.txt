#
# Building C++ library for bragi-midi
#
cmake_minimum_required(VERSION 3.10)

project(bragi)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if ( CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT )
    set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_SOURCE_DIR}/install" CACHE PATH "Default install location" FORCE)
endif()

# # Misc. troubleshooting
# set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
# set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
# set(CMAKE_CXX_VISIBILITY_PRESET hidden)
# set(CMAKE_VISIBILITY_INLINES_HIDDEN YES)
# if ( BUILD_SHARED_LIBS )
#     if ( WIN32 )
#         set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
#     endif()
# endif()


### I. Setup bragi library
set(BRAGI_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/bragi/midi/v1/message.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/bragi/midi/v1/output.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/bragi/midi/v1/note.cpp
)

if ( BUILD_SHARED_LIBS )
    add_library(bragi SHARED ${BRAGI_SRC})
else()
    add_library(bragi STATIC ${BRAGI_SRC})
endif()

set_property(TARGET bragi PROPERTY VERSION ${PROJECT_VERSION})
set_property(TARGET bragi PROPERTY SOVERSION ${PROJECT_VERSION_MAJOR})



### II. Set bragi include directories
target_include_directories(bragi
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/
)

target_link_libraries(bragi PRIVATE winmm)



### II. Compile examples
add_subdirectory(examples)



### III. Install rules
install(TARGETS bragi
    EXPORT bragiTargets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)

install(DIRECTORY src/bragi
    DESTINATION include
    FILES_MATCHING
        PATTERN "*.hpp"
        PATTERN "*.hh"

)

install(EXPORT bragiTargets
    FILE bragiTargets.cmake
    NAMESPACE bragi::
    DESTINATION lib/cmake/bragi
)
