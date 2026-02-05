// FooShader.cpp
#include "FooShader.h"

// This links the C++ class to the USF file
// Args: Class, VirtualPath, EntryFunction, ShaderType
//IMPLEMENT_GLOBAL_SHADER(FFooShaderPS, "/Plugin/Foo/BasicColor.usf", "MainPS", SF_Pixel);
//IMPLEMENT_GLOBAL_SHADER(FFooShaderPS, "/Plugin/Foo/Shaders/Private/BasicColor.usf", "MainPS", SF_Pixel);
//IMPLEMENT_GLOBAL_SHADER(FFooShaderPS, "E:/GitLocal/ShaderTest/ShaderTest/Plugins/Foo/Shaders/Private/BasicColor.usf", "MainPS", SF_Pixel);// should be absolute from the virtual root directory "/".
//IMPLEMENT_GLOBAL_SHADER(FFooShaderPS, "/Plugin/Foo/Private/BasicColor.usf", "MainPS", SF_Pixel);



IMPLEMENT_GLOBAL_SHADER(FFooShaderVS, "/Plugin/Foo/Private/BasicColor.usf", "MainVS", SF_Vertex);// Link VS to "MainVS"
IMPLEMENT_GLOBAL_SHADER(FFooShaderPS, "/Plugin/Foo/Private/BasicColor.usf", "MainPS", SF_Pixel);// Link PS to "MainPS"

// Link New Lens Distortion Shader, "LensDistortionVS" and "LensDistortionPS" MAPPED inside "LensDistortion.usf"
IMPLEMENT_GLOBAL_SHADER(FLensDistortionVS, "/Plugin/foo/Private/LensDistortion.usf", "LensDistortionVS", SF_Vertex);
IMPLEMENT_GLOBAL_SHADER(FLensDistortionPS, "/Plugin/foo/Private/LensDistortion.usf", "LensDistortionPS", SF_Pixel);

// Link Apply Distortion Shader
IMPLEMENT_GLOBAL_SHADER(FDistortSceneVS, "/Plugin/foo/Private/DistortScene.usf", "DistortVS", SF_Vertex);
IMPLEMENT_GLOBAL_SHADER(FDistortScenePS, "/Plugin/foo/Private/DistortScene.usf", "DistortPS", SF_Pixel);

// ... (Previous code) ...

// Link Bit Shift Shader
IMPLEMENT_GLOBAL_SHADER(FBitShiftVS, "/Plugin/foo/Private/BitShift.usf", "BitShiftVS", SF_Vertex);
IMPLEMENT_GLOBAL_SHADER(FBitShiftPS, "/Plugin/foo/Private/BitShift.usf", "BitShiftPS", SF_Pixel);