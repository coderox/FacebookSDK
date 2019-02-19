project(FacebookSDK CXX)

include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}/"
    "${CMAKE_CURRENT_SOURCE_DIR}/Generated"
)

execute_process(COMMAND "codegen.cmd"
                WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/"
                RESULT_VARIABLE codegen_result
)

##########
# FacebookSDK
file(GLOB FACEBOOKSDK_COMMON_SOURCES 
    "*.h" 
    "*.cpp"
    "*.idl"
    "*.xaml"
    "Themes/*.xaml"
)

file(GLOB FACEBOOKSDK_CONTROLS_SOURCES
    "Controls/*.idl"
    "Controls/*.xaml"
    "Controls/*.xaml.cpp"
    "Controls/*.xaml.h"
)

file(GLOB FACEBOOKSDK_THEME_SOURCES
    "Themes/*.xaml"
)

file(GLOB FACEBOOKSDK_GENERATED_SOURCES 
    "Generated/*.idl"
    "Generated/*.h" 
    "Generated/*.cpp"
)

file(GLOB FACEBOOKSDK_CONTENT_SOURCES 
    "Images/*.png" 
)
set_property(SOURCE ${FACEBOOKSDK_CONTENT_SOURCES} PROPERTY VS_DEPLOYMENT_CONTENT 1)

add_library(FacebookSDK SHARED 
            ${FACEBOOKSDK_COMMON_SOURCES} 
            ${FACEBOOKSDK_CONTROL_SOURCES} 
            ${FACEBOOKSDK_GENERATED_SOURCES} 
            ${FACEBOOKSDK_THEME_SOURCES}
            ${FACEBOOKSDK_APPEVENTS_SOURCES}
            ${FACEBOOKSDK_CONTENT_SOURCES}
)

source_group("Controls" FILES ${FACEBOOKSDK_CONTROL_SOURCES})
source_group("Themes" FILES ${FACEBOOKSDK_THEME_SOURCES})
source_group("Generated" FILES ${FACEBOOKSDK_GENERATED_SOURCES})
source_group("Content" FILES ${FACEBOOKSDK_CONTENT_SOURCES})

set_property(TARGET FacebookSDK PROPERTY VS_WINRT_COMPONENT TRUE)
set_property(TARGET FacebookSDK PROPERTY VS_GLOBAL_ROOTNAMESPACE FacebookSDK)

set_target_properties(FacebookSDK PROPERTIES COMPILE_FLAGS "/Yupch.h")
set_source_files_properties(pch.cpp PROPERTIES COMPILE_FLAGS "/Ycpch.h")
