cmake_minimum_required(VERSION 3.16)

project(simpleBlender VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(OpenGL REQUIRED)
# 设置 GLEW 的路径
set(GLEW_ROOT "C:/Qt/6.5.3/glew-2.2.0") # 替换为你的 GLEW 安装路径
find_package(GLEW REQUIRED)
# 添加头文件路径
include_directories(${GLEW_ROOT}/include)

# 添加库文件路径
link_directories(${GLEW_ROOT}/lib)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets opengl OpenGLWidgets Gui)
# 查找OpenCV
set(OPENCV_DIR "C:/Qt/6.5.3/opencv/build")
find_package(OpenCV REQUIRED)
if(NOT OpenCV_FOUND)
    message(FATAL_ERROR "OpenCV not found!")
endif()
# 包含OpenCV头文件路径
include_directories(${OPENCV_DIR}/install/include)

set(PROJECT_SOURCES
        ./sources/main.cpp
        ./sources/mainwindow.cpp
        ./headers/mainwindow.h
        ./sources/mainwindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(simpleBlender
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        #object
        ./headers/objects/cube.h
        ./sources/objects/cube.cpp
        ./headers/objects/object.h
        ./sources/objects/object.cpp
        ./headers/objects/uvsphere.h
        ./sources/objects/uvsphere.cpp
        ./headers/objects/torus.h
        ./sources/objects/torus.cpp
        ./headers/objects/texturemanager.h
        ./sources/objects/texturemanager.cpp
        ./headers/objects/texture.h
        ./sources/objects/texture.cpp

        #lights
        ./headers/lights/light.h
        ./sources/lights/light.cpp
        ./headers/lights/pointlight.h
        ./sources/lights/pointlight.cpp
        ./headers/lights/directionallight.h
        ./sources/lights/directionallight.cpp
        ./headers/lights/spotlight.h
        ./sources/lights/spotlight.cpp
        ./headers/lights/converttoshaderlight.h
        ./sources/lights/converttoshaderlight.cpp

        #cameras
        ./headers/cameras/camera.h
        ./sources/cameras/camera.cpp

        #resources
        ./resources/resources.qrc
        #others

        ./headers/glwidget.h
        ./sources/glwidget.cpp

        ./headers/toolfunc.h
        ./sources/toolfunc.cpp

        ./headers/renderimagewidget.h
        ./sources/renderimagewidget.cpp
        ./sources/renderImageWidget.ui

        ./headers/singleton.h
        ./sources/singleton.cpp
        ./headers/logger.h
        ./sources/logger.cpp
        headers/customUI/mystandarditemmodel.h
        sources/customUI/mystandarditemmodel.cpp
        headers/uitransform.h
        sources/uitransform.cpp
        sources/uitransform.ui
        headers/uicamera.h
        sources/uicamera.cpp
        sources/uicamera.ui
        headers/customUI/myLineEdit.h
        sources/customUI/myLineEdit.cpp
        headers/uimaterial.h
        sources/uimaterial.cpp
        sources/uimaterial.ui
        headers/uilight.h
        sources/uilight.cpp
        sources/uilight.ui



    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET blender APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(simpleBlender SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(simpleBlender
            ${PROJECT_SOURCES}
        )
    endif()
endif()
#这里添加头文件夹的路径，在程序中就不用加文件夹的前缀了
target_include_directories(simpleBlender PRIVATE
    ${PROJECT_SOURCE_DIR}/headers
)
target_include_directories(simpleBlender PRIVATE
    ${PROJECT_SOURCE_DIR}/headers/objects/
)
target_include_directories(simpleBlender PRIVATE
    ${PROJECT_SOURCE_DIR}/headers/lights/
)
target_include_directories(simpleBlender PRIVATE
    ${PROJECT_SOURCE_DIR}/glm/
)
target_include_directories(simpleBlender PRIVATE
    ${PROJECT_SOURCE_DIR}/headers/cameras/
)
target_include_directories(simpleBlender PRIVATE
    ${PROJECT_SOURCE_DIR}/headers/customUI/
)

target_link_libraries(simpleBlender PRIVATE ${OPENGL_LIBRARIES}
    glew32
    )#glfw3
target_link_libraries(simpleBlender PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt6::OpenGL Qt6::OpenGLWidgets Qt6::Gui)
target_link_libraries(simpleBlender PRIVATE ${OpenCV_LIBS})

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.simpleBlender)
endif()
set_target_properties(simpleBlender PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS simpleBlender
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(simpleBlender)
endif()
