# CMakeList.txt : CMake project for VulkanEngine, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("VulkanEngine")

# Set path to vcpkg
include(C:/Users/unfea/Documents/vcpkg/vcpkg/scripts/buildsystems/vcpkg.cmake)

# Set path to Vulkan SDK
set(VULKAN_SDK_PATH "C:/VulkanSDK/1.3.268.0")

# Include Vulkan headers
include_directories(${VULKAN_SDK_PATH}/Include)

# Add source to this project's executable.
add_executable (VulkanEngine "VulkanEngine.cpp" "VulkanEngine.h")

# Link Vulkan library
target_link_libraries(VulkanEngine ${VULKAN_SDK_PATH}/Lib/vulkan-1.lib)

# Find GLFW3 package
find_package(glfw3 CONFIG REQUIRED)

# Link GLFW3 library
target_link_libraries(VulkanEngine glfw)

# Find GLM package (if installed using vcpkg)
find_package(glm CONFIG REQUIRED)

# Link GLM library
target_link_libraries(VulkanEngine glm::glm)
