set(EXAMPLES
    ${CMAKE_CURRENT_SOURCE_DIR}/trigger-note.cpp
)

get_filename_component(REPO_DIR ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)

foreach( FILE ${EXAMPLES} )
    get_filename_component(FILE_NAME ${FILE} NAME_WE)

    add_executable(${FILE_NAME} ${FILE})

    target_link_libraries(${FILE_NAME} PRIVATE bragi)

    target_include_directories(${FILE_NAME} PRIVATE ${REPO_DIR}/src)
endforeach()
