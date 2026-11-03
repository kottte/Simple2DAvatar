cmake_minimum_required(VERSION 4.4)

project(Simple2DAvatar)

include(FetchContent)

# Указываем CMake собрать wxWidgets статически
set(wxBUILD_SHARED OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    wxWidgets
    GIT_REPOSITORY https://github.com/wxWidgets/wxWidgets.git
    GIT_TAG        v3.3.4   # или ваша версия
    GIT_SHALLOW    TRUE
)
set(wxBUILD_SHARED OFF CACHE BOOL "" FORCE)
set(wxBUILD_SAMPLES OFF CACHE BOOL "" FORCE)
set(wxBUILD_TESTS   OFF CACHE BOOL "" FORCE)

set(wxUSE_WEBVIEW      OFF CACHE BOOL "" FORCE)
set(wxUSE_STC          OFF CACHE BOOL "" FORCE)
set(wxUSE_RIBBON       OFF CACHE BOOL "" FORCE)
set(wxUSE_PROPGRID     OFF CACHE BOOL "" FORCE)
set(wxUSE_RICHTEXT     OFF CACHE BOOL "" FORCE)
set(wxUSE_MEDIACTRL    OFF CACHE BOOL "" FORCE)
set(wxUSE_WXHTML_HELP  OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(wxWidgets)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<CONFIG>")

#find_package(wxWidgets REQUIRED COMPONENTS core base)
#include(${wxWidgets_USE_FILE})

add_executable(${PROJECT_NAME} src/main.cpp src/MainFrame.cpp
    src/AudioCapture.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE src ${CMAKE_SOURCE_DIR}/external/miniaudio)
target_link_libraries(${PROJECT_NAME} PRIVATE wx::core wx::base)
#target_link_libraries(${PROJECT_NAME} ${wxWidgets_LIBRARIES})

if(APPLE)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_INFO_PLIST "${CMAKE_SOURCE_DIR}/Info.plist.in"
        MACOSX_BUNDLE_BUNDLE_NAME           "${PROJECT_NAME}"
        MACOSX_BUNDLE_GUI_IDENTIFIER        "com.example.${PROJECT_NAME}"
        MACOSX_BUNDLE_BUNDLE_VERSION        "1.0"
        MACOSX_BUNDLE_SHORT_VERSION_STRING  "1.0"
    )
    target_link_options(${PROJECT_NAME} PRIVATE "$<$<CONFIG:Release>:-Wl,-dead_strip>")
    set(_assets_target "$<TARGET_BUNDLE_DIR:${PROJECT_NAME}>/..")
elseif(WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE TRUE)
    set(_assets_target "$<TARGET_FILE_DIR:${PROJECT_NAME}>")

elseif(UNIX)
    # Linux: miniaudio нужны ALSA и PulseAudio
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(PULSE REQUIRED libpulse)
    pkg_check_modules(ALSA  REQUIRED alsa)
    target_link_libraries(${PROJECT_NAME} PRIVATE
        ${PULSE_LIBRARIES}
        ${ALSA_LIBRARIES}
    )
    target_include_directories(${PROJECT_NAME} PRIVATE
        ${PULSE_INCLUDE_DIRS}
        ${ALSA_INCLUDE_DIRS}
    )
    target_link_options(${PROJECT_NAME} PRIVATE "$<$<CONFIG:Release>:-Wl,--gc-sections>")
    set(_assets_target "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
endif()


add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        ${_assets_target}
    COMMENT "Copy assets next to the app bundle / executable"
)

if(APPLE)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_STRIP} -u -r $<TARGET_FILE:${PROJECT_NAME}>
        COMMENT "Stripping binary for macOS"
    )
endif()