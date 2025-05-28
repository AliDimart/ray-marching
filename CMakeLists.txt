cmake_minimum_required(VERSION 4.0.1)
project(RayMarching LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Явно указываем все исходные файлы
add_executable(RayMarching
    src/main.cpp
    src/vec3.cpp 
)

# Пути к заголовкам
target_include_directories(RayMarching PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Для математических функций
target_link_libraries(RayMarching PRIVATE ${CMAKE_DL_LIBS})
if(UNIX)
    target_link_libraries(RayMarching PRIVATE m)
endif()

