cmake_minimum_required(VERSION 3.10)
project(AIChatApp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 REQUIRED COMPONENTS Widgets)

set(CMAKE_AUTOMOC ON)  # Ensures Qt's Meta-Object Compiler (moc) runs

# Copy run_model.py from the source directory to the build directory
file(COPY ${CMAKE_SOURCE_DIR}/run_model.py DESTINATION ${CMAKE_BINARY_DIR})

# Copy spinner.gif from the source directory to the build directory
file(COPY ${CMAKE_SOURCE_DIR}/spinner.gif DESTINATION ${CMAKE_BINARY_DIR})


add_executable(AIChat main.cpp AIChatApp.cpp)
target_link_libraries(AIChat Qt5::Widgets)
