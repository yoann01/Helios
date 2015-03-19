TARGET=Helios.out
OBJECTS_DIR=obj
QMAKE_CXXFLAGS = -Wno-deprecated-register -Wno-c++11-compat-deprecated-writable-strings -Wno-unneeded-internal-declaration
# as I want to support 4.8 and 5 this will set a flag for some of the mac stuff
# mainly in the types.h file for the setMacVisual which is native in Qt5
isEqual(QT_MAJOR_VERSION, 5) {
        cache()
        DEFINES +=QT5BUILD
}
UI_HEADERS_DIR=ui
MOC_DIR=moc

CONFIG-=app_bundle
CONFIG += c++11
QT+=gui opengl core
SOURCES += \
    src/main.cpp \
    src/mainwindow.cpp \
    src/Camera.cpp \
    src/ShaderUtils.cpp \
    src/TextureUtils.cpp \
    src/ShaderProgram.cpp \
    src/Texture.cpp \
    src/OpenGLWidget.cpp \
    src/Shader.cpp \
    src/pathtracerscene.cpp \
    src/pinholecamera.cpp \
    optixSrc/*.cu \
    src/HDRLoader.cpp \
    src/optixmodel.cpp \
    src/TextureLoader.cpp \
    src/MeshDockWidget.cpp \
    src/GenSetDockWidget.cpp \
    src/OsoReader.cpp \
    src/OslReader.cpp \
    src/AbstractMaterialWidget.cpp \
    src/MeshWidget.cpp \
    src/MaterialLibrary.cpp \
    src/qneblock.cpp \
    src/qneconnection.cpp \
    src/qneport.cpp \
    src/qnodeseditor.cpp \
    src/oso.y \
    src/osolexer.l \
    src/OSLBlock.cpp

SOURCES -= optixSrc/*.cu \
           src/oso.y \
           src/osolexer.l

HEADERS += \
    include/mainwindow.h \
    include/Camera.h \
    include/ShaderUtils.h \
    include/TextureUtils.h \
    include/ShaderProgram.h \
    include/Texture.h \
    include/OpenGLWidget.h \
    include/Shader.h \
    include/ui_mainwindow.h \
    include/helpers.h \
    include/random.h \
    include/path_tracer.h \
    include/pathtracerscene.h \
    include/pinholecamera.h \
    include/HDRLoader.h \
    include/optixmodel.h \
    include/TextureLoader.h \
    include/ShaderGlobals.h \
    include/MeshDockWidget.h \
    include/GenSetDockWidget.h \
    include/OsoReader.h \
    include/BRDFUtils.h \
    include/OslReader.h \
    include/AbstractMaterialWidget.h \
    include/MeshWidget.h \
    include/MaterialLibrary.h \
    include/qneblock.h \
    include/qneconnection.h \
    include/qneport.h \
    include/qnodeseditor.h \
    include/OSLBlock.h

INCLUDEPATH +=./include /opt/local/include /usr/local/include ./include/OSL /usr/local/oiio/src/include
macx:LIBS += -ll
linux:LIBS += -lfl
LIBS += -L/opt/local/lib -lIL -L/usr/local/lib -lOpenImageIO -lassimp -lboost_system -L./osl/lib -loslcomp
DESTDIR=./

CONFIG += console
CONFIG -= app_bundle

# use this to suppress some warning from boost
QMAKE_CXXFLAGS_WARN_ON += "-Wno-unused-parameter"
QMAKE_CXXFLAGS+= -msse -msse2 -msse3
macx:QMAKE_CXXFLAGS+= -arch x86_64
macx:INCLUDEPATH+=/usr/local/include/
# define the _DEBUG flag for the graphics lib

unix:LIBS += -L/usr/local/lib

# now if we are under unix and not on a Mac (i.e. linux) define GLEW
linux-*{
                linux-*:QMAKE_CXXFLAGS +=  -march=native
                linux-*:DEFINES+=GL42
                DEFINES += LINUX
                LIBS += -lGLEW
}
DEPENDPATH+=include
# if we are on a mac define DARWIN
macx:DEFINES += DARWIN


FORMS += \
    ui/mainwindow.ui

OTHER_FILES += \
    shaders/PhongFrag.glsl \
    shaders/PhongVert.glsl \
    shaders/pathTraceVert.vert \
    shaders/pathTraceFrag.frag \
    textures/CedarCity.hdr \
    textures/map.png \
    shaders/OSL/metal.osl \
    shaders/OSL/checkerboard.osl \
    shaders/OSL/envmap.osl \
    shaders/OSL/matte.osl \
    shaders/OSL/ward.osl \
    shaders/OSO/emitter.oso \
    shaders/OSO/glass.oso \
    shaders/OSO/image.oso \
    shaders/OSO/matte.oso \
    shaders/OSO/metal.oso \
    shaders/OSO/ubersurface.oso \
    shaders/OSL/checkerboard.oso \
    shaders/OSL/emitter.osl \
    shaders/OSL/glass.osl \
    shaders/OSL/image.osl \
    shaders/OSL/ubersurface.osl \
    shaders/OSL/ifTest.osl \
    shaders/OSO/ifTest.oso \
    styleSheet/darkOrange

#Sources we want compiled with bison
BISONSOURCES = src/oso.y
#Sources we want compiles with flex
FLEXSOURCES = src/osolexer.l

#Optix Stuff
CUDA_SOURCES += optixSrc/*.cu

# Path to cuda SDK install
macx:CUDA_DIR = /Developer/NVIDIA/CUDA-6.5
linux:CUDA_DIR = /usr/local/cuda-6.5
# Path to cuda toolkit install
macx:CUDA_SDK = /Developer/NVIDIA/CUDA-6.5/samples
linux:CUDA_SDK = /usr/local/cuda-6.5/samples

# include paths
macx:INCLUDEPATH += /Developer/OptiX/SDK/sutil
macx:INCLUDEPATH += /Developer/OptiX/SDK
linux:INCLUDEPATH += /usr/local/OptiX/SDK/sutil
linux:INCLUDEPATH += /usr/local/OptiX/SDK
INCLUDEPATH += $$CUDA_DIR/include
INCLUDEPATH += $$CUDA_DIR/common/inc/
INCLUDEPATH += $$CUDA_DIR/../shared/inc/
macx:INCLUDEPATH += /Developer/OptiX/include
linux:INCLUDEPATH += /usr/local/OptiX/include
# lib dirs
#QMAKE_LIBDIR += $$CUDA_DIR/lib64
macx:QMAKE_LIBDIR += $$CUDA_DIR/lib
linux:QMAKE_LIBDIR += $$CUDA_DIR/lib64
QMAKE_LIBDIR += $$CUDA_SDK/common/lib
macx:QMAKE_LIBDIR += /Developer/OptiX/lib64
linux:QMAKE_LIBDIR += /usr/local/OptiX/lib64
LIBS += -lcudart  -loptix -loptixu

##############################################################
################# bison compiler set up ######################
##############################################################
#files we want bison to compile
bison.input = BISONSOURCES
#what we want our output files to be named
bison.output = src/y.tab.cpp
#commands for our compiler
bison.commands = yacc -d -o src/y.tab.cpp ${QMAKE_FILE_IN}
#Declare that we want the output to be added to our projects
#sources
bison.variable_out = SOURCES
#this flag means that this compiler will be run before our main
#c++ compiler
bison.CONFIG += target_predeps
#tells the make file what to remove on clean
bison.clean = src/y.tab.cpp
#name of the extra compiler used in the make file
bison.name = bison
#finally add this compiler to our qmake project
QMAKE_EXTRA_COMPILERS += bison

##############################################################
################# flex compiler set up #######################
##############################################################
#files we want flex to compile
flex.input = FLEXSOURCES
#what we want our output files to be named
flex.output = src/lex.yy.cc
#commands for our compiler
flex.commands = lex -o src/lex.yy.cc ${QMAKE_FILE_IN}
#Declare that we want the outpur to be added to our projects
#sources
flex.variable_out = SOURCES
#this flag means that this compiler will be run before our main
#c++ compiler
flex.CONFIG += target_predeps
#tells the make file what to remove on clean
flex.clean = src/lex.yy.cc
#name of the extra compiler used in the make file
flex.name = flex
#finally add this compiler to our qmake project
QMAKE_EXTRA_COMPILERS += flex


##############################################################
################# nvcc compiler set up #######################
##############################################################


# nvcc flags (ptxas option verbose is always useful)
# add the PTX flags to compile optix files
NVCCFLAGS = --compiler-options -fno-strict-aliasing -use_fast_math --ptxas-options=-v -ptx

#set our ptx directory
PTX_DIR = ptx

# join the includes in a line
CUDA_INC = $$join(INCLUDEPATH,' -I','-I',' ')

# Prepare the extra compiler configuration (taken from the nvidia forum - i'm not an expert in this part)
optix.input = CUDA_SOURCES

#cuda.output = ${OBJECTS_DIR}${QMAKE_FILE_BASE}_cuda.o
optix.output = $$PTX_DIR/${QMAKE_FILE_BASE}.cu.ptx

#tell qt what the files to remove on clean
optix.clean = $$PTX_DIR/*.ptx

# Tweak arch according to your hw's compute capability
#for optix you can only have one architechture when using the PTX flags when using the -ptx flag you dont want to have the -c flag for compiling
macx:optix.commands = $$CUDA_DIR/bin/nvcc -m64 -gencode arch=compute_30,code=sm_30 $$NVCCFLAGS $$CUDA_INC $$LIBS  ${QMAKE_FILE_NAME} -o ${QMAKE_FILE_OUT}
linux:optix.commands = $$CUDA_DIR/bin/nvcc -m64 -gencode arch=compute_50,code=sm_50 $$NVCCFLAGS $$CUDA_INC $$LIBS  ${QMAKE_FILE_NAME} -o ${QMAKE_FILE_OUT}

#use this line for debug code
#optix.commands = $$CUDA_DIR/bin/nvcc -m64 -g -G -gencode arch=compute_52,code=sm_52 $$NVCCFLAGS $$CUDA_INC $$LIBS  ${QMAKE_FILE_NAME} -o ${QMAKE_FILE_OUT}
#Declare that we wnat to do this before compiling the C++ code
optix.CONFIG = target_predeps
#now declare that we dont want to link these files with gcc
optix.CONFIG += no_link
optix.dependency_type = TYPE_C
# Tell Qt that we want add our optix compiler
QMAKE_EXTRA_UNIX_COMPILERS += optix





