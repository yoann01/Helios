#ifndef SHADING_H
#define SHADING_H
#include <OSL/oslexec.h>
#include <OSL/oslcomp.h>
#include <QString>
#include <fstream>
#include <iostream>
//------------------------------------------------------------------------------------------------------------------------------------
class Shading{
public:
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief Ctor
    //------------------------------------------------------------------------------------------------------------------------------------
    Shading();
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief Dtor
    //------------------------------------------------------------------------------------------------------------------------------------
    ~Shading();
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief Initialise variables in the shading system
    //------------------------------------------------------------------------------------------------------------------------------------
    void initialise();
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief Register closures implemented in the Helios renderer
    //------------------------------------------------------------------------------------------------------------------------------------
    void registerClosures(OSL::ShadingSystem *_shadingSytem);
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief Compiles osl code to oso and dumps to file
    //------------------------------------------------------------------------------------------------------------------------------------
    void compileOSL(QString _shaderName);
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief Compiles osl code to oso and dumps to buffer
    //------------------------------------------------------------------------------------------------------------------------------------
    void compileOSLtoBuffer(QString _shaderName);
    //------------------------------------------------------------------------------------------------------------------------------------
private:
    //------------------------------------------------------------------------------------------------------------------------------------
    OSL::ShadingSystem *m_shadingSystem;
    //------------------------------------------------------------------------------------------------------------------------------------
};
//------------------------------------------------------------------------------------------------------------------------------------
#endif

/// Read the entire contents of the named file and place it in str,
/// returning true on success, false on failure.
inline bool read_text_file (QString filename, std::string &str)
{
    std::ifstream in (filename.toStdString().c_str(), std::ios::in | std::ios::binary);
    if (in) {
        std::ostringstream contents;
        contents << in.rdbuf();
        in.close ();
        str = contents.str();
        return true;
    }
    return false;
}