project(winsdkfb CXX)

include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}/"
    "${CMAKE_CURRENT_SOURCE_DIR}/Generated"
)

execute_process(COMMAND "codegen.cmd"
                WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/"
                RESULT_VARIABLE codegen_result
)

##########
# winsdkfb
file(GLOB winsdkfb_COMMON_SOURCES 
    "*.h" 
    "*.cpp"
    "*.idl"
    "*.xaml"
    "Themes/*.xaml"
)

file(GLOB winsdkfb_CONTROLS_SOURCES
    "Controls/*.idl"
    "Controls/*.xaml"
    "Controls/*.xaml.cpp"
    "Controls/*.xaml.h"
)

file(GLOB winsdkfb_THEME_SOURCES
    "Themes/*.xaml"
)

file(GLOB winsdkfb_GENERATED_SOURCES 
    "Generated/*.idl"
    "Generated/*.h" 
    "Generated/*.cpp"
)

file(GLOB winsdkfb_CONTENT_SOURCES 
    "Images/*.png" 
)
set_property(SOURCE ${winsdkfb_CONTENT_SOURCES} PROPERTY VS_DEPLOYMENT_CONTENT 1)

add_library(winsdkfb SHARED 
            ${winsdkfb_COMMON_SOURCES} 
            ${winsdkfb_CONTROL_SOURCES} 
            ${winsdkfb_GENERATED_SOURCES} 
            ${winsdkfb_THEME_SOURCES}
            ${winsdkfb_APPEVENTS_SOURCES}
            ${winsdkfb_CONTENT_SOURCES}
)

source_group("Controls" FILES ${winsdkfb_CONTROL_SOURCES})
source_group("Themes" FILES ${winsdkfb_THEME_SOURCES})
source_group("Generated" FILES ${winsdkfb_GENERATED_SOURCES})
source_group("Content" FILES ${winsdkfb_CONTENT_SOURCES})

set_property(TARGET winsdkfb PROPERTY VS_WINRT_COMPONENT TRUE)
set_property(TARGET winsdkfb PROPERTY VS_GLOBAL_ROOTNAMESPACE winsdkfb)

set_target_properties(winsdkfb PROPERTIES COMPILE_FLAGS "/Yupch.h")
set_source_files_properties(pch.cpp PROPERTIES COMPILE_FLAGS "/Ycpch.h")
