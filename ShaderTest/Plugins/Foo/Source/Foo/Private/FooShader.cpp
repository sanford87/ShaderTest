// FooShader.cpp
#include "FooShader.h"

// This links the C++ class to the USF file
// Args: Class, VirtualPath, EntryFunction, ShaderType
//IMPLEMENT_GLOBAL_SHADER(FFooShaderPS, "/Plugin/Foo/BasicColor.usf", "MainPS", SF_Pixel);
//IMPLEMENT_GLOBAL_SHADER(FFooShaderPS, "/Plugin/Foo/Shaders/Private/BasicColor.usf", "MainPS", SF_Pixel);
//IMPLEMENT_GLOBAL_SHADER(FFooShaderPS, "E:/GitLocal/ShaderTest/ShaderTest/Plugins/Foo/Shaders/Private/BasicColor.usf", "MainPS", SF_Pixel);// should be absolute from the virtual root directory "/".
//IMPLEMENT_GLOBAL_SHADER(FFooShaderPS, "/Plugin/Foo/Private/BasicColor.usf", "MainPS", SF_Pixel);


// Link VS to "MainVS"
IMPLEMENT_GLOBAL_SHADER(FFooShaderVS, "/Plugin/Foo/Private/BasicColor.usf", "MainVS", SF_Vertex);
// Link PS to "MainPS"
IMPLEMENT_GLOBAL_SHADER(FFooShaderPS, "/Plugin/Foo/Private/BasicColor.usf", "MainPS", SF_Pixel);

// --- Link New Lens Distortion Shader ---
// Note: We map to "LensDistortionVS" and "LensDistortionPS" inside "LensDistortion.usf"
IMPLEMENT_GLOBAL_SHADER(FLensDistortionVS, "/Plugin/foo/Private/LensDistortion.usf", "LensDistortionVS", SF_Vertex);
IMPLEMENT_GLOBAL_SHADER(FLensDistortionPS, "/Plugin/foo/Private/LensDistortion.usf", "LensDistortionPS", SF_Pixel);