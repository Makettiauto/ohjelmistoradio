# SRC/CMakeLists.txt


set(SRC_FILES
    demoUE2024.cpp
    demoBS2024.cpp
    Logger.cpp
)

# Receiver
add_executable(rxTextMessage
    demoUE2024.cpp
    Logger.cpp
)

# Transmitter
add_executable(txTextMessage
    demoBS2024.cpp
    Logger.cpp
)

# Add include directories for both targets
target_include_directories(rxTextMessage PRIVATE
    ${CMAKE_SOURCE_DIR}/Facilities
    ${CMAKE_SOURCE_DIR}/SRC
)

target_include_directories(txTextMessage PRIVATE
    ${CMAKE_SOURCE_DIR}/Facilities
    ${CMAKE_SOURCE_DIR}/SRC
)

# add demo lbiragry
add_library(demo_lib demo.cpp)



# Link libraries
foreach(target rxTextMessage txTextMessage)
    target_link_libraries(${target}
        fmt
        fftw3f
        fftw3
        uhd
        boost_system
        boost_program_options
        pthread
        demo_lib
    )
endforeach()
