#include "MyColorShader.h"

// Link the C++ class to the USF file and Entry Point
// Arguments: ClassName, VirtualPath, EntryPointName, ShaderType
IMPLEMENT_GLOBAL_SHADER(FMyColorShaderPS, "/Plugin/foo/MyColorShader.usf", "MainPS", SF_Pixel);