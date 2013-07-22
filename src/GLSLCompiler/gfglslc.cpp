// /////////////////////////////////////////////////////////////////
// @file gfglslc.cpp
// @author PJ O Halloran
// @date 23/09/2010
//
// Small application for checking if GLSL vertex, fragment and
// geometry shaders compile and link successfully and the resulting
// GLSL program is okay.
//
// /////////////////////////////////////////////////////////////////

// External Headers
#include <iostream>
#include <string>
#include <cstring>

#include <boost/filesystem.hpp>

#include <GL/glew.h>
#include <GL/glfw.h>

// Project Headers
#include "GLSLShader.h"

// /////////////////////////////////////////////////////////////////
// Print out usage information.
//
// @param programNameStr The name of the executable.
//
// /////////////////////////////////////////////////////////////////
void PrintUsage(const char *programNameStr)
{
    if(!programNameStr) {
        std::cerr << "Error: Program name not supplied to PrintUsage()." << std::endl;
        return;
    }

    std::cout << programNameStr << " [-h] [--help] VSFile FSFile [GSFile]" << std::endl;
    std::cout << "\tVSFile = The path of the vertex shader." << std::endl;
    std::cout << "\tFSFile = The path of the fragment shader." << std::endl;
    std::cout << "\tGSFile = The path of the geometry shader (optional)." << std::endl;
    std::cout << "\tPlease note the files must be specified in the order defined above." << std::endl;
}

// /////////////////////////////////////////////////////////////////
// Main entry point.
//
//
// /////////////////////////////////////////////////////////////////
int main(int argc, char *argv[])
{
    const char *vsFileStr = 0;                  // Vertex shader file path.
    const char *fsFileStr = 0;                  // Fragment shader file path.
    const char *gsFileStr = 0;                  // Geometry shader file path.

    // Check if the user asked to see the help.
    for(int i = 0; i < argc; ++i) {
        if(strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            PrintUsage(argv[0]);
            return (0);
        }
    }

    // Set file strings.
    if(argc == 3) {
        vsFileStr = argv[1];
        fsFileStr = argv[2];
    } else if(argc == 4) {
        vsFileStr = argv[1];
        fsFileStr = argv[2];
        gsFileStr = argv[3];
    } else {
        std::cerr << "Incorrect arguments supplied.\n" << std::endl;
        PrintUsage(argv[0]);
        return (-1);
    }

    //vsFileStr = "C:/Documents and Settings/HP_Administrator/My Documents/Visual Studio 2008/Projects/gameframework/trunk/src/graphics/shaders/phong.vp";
    //fsFileStr = "C:/Documents and Settings/HP_Administrator/My Documents/Visual Studio 2008/Projects/gameframework/trunk/src/graphics/shaders/phong.fp";

    glfwInit();
    glfwOpenWindow(640, 480, 8, 8, 8, 8, 24, 0, GLFW_WINDOW);

    GLenum res = glewInit();
    if(GLEW_OK != res) {
        // Problem: glewInit failed, something is seriously wrong.
        std::cerr << "Failed to initialize the GLEW library: " << std::string(reinterpret_cast<const char *>(glewGetErrorString(res))) << std::endl;
        glfwTerminate();
        return (-1);
    }

    GameHalloran::GLSLShader shaderProg;
    GameHalloran::VSAttributeNameList nameList;
    std::string errorMsg;
    bool buildResult, activateResult;

    if(gsFileStr) {
        buildResult = shaderProg.Build(boost::filesystem::path(vsFileStr), boost::filesystem::path(gsFileStr), boost::filesystem::path(fsFileStr), nameList, errorMsg);
    } else {
        buildResult = shaderProg.Build(boost::filesystem::path(vsFileStr), boost::filesystem::path(fsFileStr), nameList, errorMsg);
    }
    activateResult = shaderProg.Activate();

    if(!buildResult) {
        std::cout << "Error: " << errorMsg << std::endl;
    } else if(!activateResult) {
        std::cout << "Error: Failed to activate the shader." << std::endl;
    } else {
        std::cout << "Success!" << std::endl;
    }

    glfwTerminate();
    return (0);
}
