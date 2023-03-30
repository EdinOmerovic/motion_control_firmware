
set(SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(LIBS ${CMAKE_CURRENT_SOURCE_DIR}/inc)


add_library(driver ${SOURCES}/control.c
    ${SOURCES}/encoder.c
    ${SOURCES}/motor.c
    ${SOURCES}/trajectory.c
    ${SOURCES}/lp_filter.c

)

target_include_directories(driver PUBLIC ${LIBS})


target_link_libraries(${TARGET_NAME} driver)