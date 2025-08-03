file(GLOB SOURCES_LIST
    ${CMAKE_CURRENT_SOURCE_DIR}/views/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/views/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/comm/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/comm/*.cpp
)

target_sources(${PROJECT_NAME}
    PRIVATE
    ${SOURCES_LIST}
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/views
    ${CMAKE_CURRENT_SOURCE_DIR}/comm
)
