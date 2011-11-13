QT += opengl

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += main.cpp \
    mer.cpp

CONFIG += mobility
MOBILITY += feedback

# Please do not modify the following two lines. Required for deployment.
include(qmlapplicationviewer/qmlapplicationviewer.pri)
qtcAddDeployment()

OTHER_FILES += shaders/vertexshader.glsl \
    shaders/fragmentshader.glsl

HEADERS += \
    mer.h

RESOURCES += \
    res.qrc




