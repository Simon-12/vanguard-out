cmake_minimum_required(VERSION 3.16)

project(vanguard-gui VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Quick QuickControls2)

qt_standard_project_setup(REQUIRES 6.5)

# Resource file
set(RESOURCE_FILE res.qrc)
qt_add_resources(RESOURCE_FILES ${RESOURCE_FILE})

qt_add_executable(vanguard-gui
    main.cpp
    controller.h
    controller.cpp
    ${RESOURCE_FILES}
)

qt_add_qml_module(vanguard-gui
    URI qml.components
    VERSION 1.0
    QML_FILES
        Main.qml
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(vanguard-gui PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.vanguard-gui
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(vanguard-gui
    PRIVATE Qt6::Quick Qt6::QuickControls2
)

include(GNUInstallDirs)
install(TARGETS vanguard-gui
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Copy cli to gui path
add_custom_command(TARGET vanguard-gui POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy
                           "${CMAKE_BINARY_DIR}/vanguard-cli/vanguard-cli.exe"
                           "${CMAKE_BINARY_DIR}/vanguard-gui/vanguard-cli.exe"
                   COMMENT "Copying vanguard-cli to vanguard-gui path")
