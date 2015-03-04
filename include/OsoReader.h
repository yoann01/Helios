#ifndef OSOREADER_H_
#define OSOREADER_H_
/// @brief Class to interpret the oso code and turn it into a device function
/// @author Toby Gilbert
/// @date 04/03/15
#include <string>
#include <FlexLexer.h>
//class osoFlexLexer;
extern int yyparse ();
//----------------------------------------------------------------------------------------------------------------------------------------
class OsoReader{
public:
    //----------------------------------------------------------------------------------------------------------------------------------------
    /// @brief Constructor
    //----------------------------------------------------------------------------------------------------------------------------------------
    OsoReader();
    //----------------------------------------------------------------------------------------------------------------------------------------
    /// @brief Destructor
    //----------------------------------------------------------------------------------------------------------------------------------------
    ~OsoReader();
    //----------------------------------------------------------------------------------------------------------------------------------------
    /// @brief Set the oso version
    //----------------------------------------------------------------------------------------------------------------------------------------
    void version(float _major, int _minor);
    //----------------------------------------------------------------------------------------------------------------------------------------
    /// @brief Create the shader
    //----------------------------------------------------------------------------------------------------------------------------------------
    void shader(std::string _shaderType, std::string _shaderName);
    //----------------------------------------------------------------------------------------------------------------------------------------
    bool parseFile(const std::string &_filename);
private:
    //----------------------------------------------------------------------------------------------------------------------------------------
    /// @brief Oso major version
    //----------------------------------------------------------------------------------------------------------------------------------------
    float m_osoMajor;
    //----------------------------------------------------------------------------------------------------------------------------------------
    /// @brief Oso minor version
    //----------------------------------------------------------------------------------------------------------------------------------------
    int m_osoMinor;
    //----------------------------------------------------------------------------------------------------------------------------------------
    /// @brief The type of shader
    //----------------------------------------------------------------------------------------------------------------------------------------
    std::string m_shaderType;
    //----------------------------------------------------------------------------------------------------------------------------------------
    /// @brief The shader name
    //----------------------------------------------------------------------------------------------------------------------------------------
    std::string m_shaderName;
    //----------------------------------------------------------------------------------------------------------------------------------------
};
//----------------------------------------------------------------------------------------------------------------------------------------
#endif
