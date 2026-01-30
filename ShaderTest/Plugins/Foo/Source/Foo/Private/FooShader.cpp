// FooShader.cpp
#include "FooShader.h"

// This links the C++ class to the USF file
// Args: Class, VirtualPath, EntryFunction, ShaderType
//IMPLEMENT_GLOBAL_SHADER(FFooShaderPS, "/Plugin/Foo/MyShader.usf", "MainPS", SF_Pixel);
//IMPLEMENT_GLOBAL_SHADER(FFooShaderPS, "/Plugin/Foo/Shaders/Private/MyShader.usf", "MainPS", SF_Pixel);
//IMPLEMENT_GLOBAL_SHADER(FFooShaderPS, "E:/GitLocal/ShaderTest/ShaderTest/Plugins/Foo/Shaders/Private/MyShader.usf", "MainPS", SF_Pixel);// should be absolute from the virtual root directory "/".
IMPLEMENT_GLOBAL_SHADER(FFooShaderPS, "/Plugin/Foo/Private/MyShader.usf", "MainPS", SF_Pixel);