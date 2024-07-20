cmake_minimum_required(VERSION 3.16)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDebug$<$<CONFIG:Debug>:Debug>")
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Release>:Release>")

project(raytracer)

add_subdirectory(thirdparty/glm)


file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")


file(GLOB_RECURSE MODULE_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/sources/*")

# here add subdirectory created in your module
add_subdirectory(sources/color)
add_subdirectory(sources/tobmp)
add_subdirectory(sources/ray)
add_subdirectory(sources/hitrec)
add_subdirectory(sources/hittable)
add_subdirectory(sources/hittablelist)
add_subdirectory(sources/camera)
add_subdirectory(sources/renderer)
add_subdirectory(sources/utils)
add_subdirectory(sources/material)
add_subdirectory(sources/scattering_record)

add_executable("${CMAKE_PROJECT_NAME}")
add_compile_definitions(SOURCE_ROOT="${CMAKE_CURRENT_SOURCE_DIR}")

set_property(TARGET "${CMAKE_PROJECT_NAME}" PROPERTY CXX_STANDARD 20)

target_sources("${CMAKE_PROJECT_NAME}" PRIVATE ${MY_SOURCES} )


if(MSVC) # Microsoft Visual Studio
	target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC _CRT_SECURE_NO_WARNINGS)
	set_property(TARGET "${CMAKE_PROJECT_NAME}" PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreadedDebug<$<CONFIG:Debug>:Debug>")
	set_property(TARGET "${CMAKE_PROJECT_NAME}" PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Release>:Release>")

endif()

target_include_directories("${CMAKE_PROJECT_NAME}" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

target_link_libraries("${CMAKE_PROJECT_NAME}" PRIVATE Color Tobmp Ray HitRecord Hittable HittableList Camera Renderer Utils Material ScatteringRecord)

target_link_libraries("${CMAKE_PROJECT_NAME}" PRIVATE glm)