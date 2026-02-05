#pragma once

#include "GlobalShader.h"
#include "ShaderParameterStruct.h"
#include "DataDrivenShaderPlatformInfo.h"


// =========================================================================
// 1. BASIC FOO SHADER EXAMPLE
// =========================================================================

// 1. Pixel Shader Parameters (Texture & Sampler)
BEGIN_SHADER_PARAMETER_STRUCT(FFooShaderParameters, )
    SHADER_PARAMETER_TEXTURE(Texture2D, InputTexture)
    SHADER_PARAMETER_SAMPLER(SamplerState, InputSampler)
END_SHADER_PARAMETER_STRUCT()

// 2. Vertex Shader Parameters (EMPTY - FIX)
// We explicitly define this struct so we have a type to give to the VS
BEGIN_SHADER_PARAMETER_STRUCT(FFooVertexShaderParameters, )
END_SHADER_PARAMETER_STRUCT()

// 3. Vertex Shader Class
class FFooShaderVS : public FGlobalShader
{
public:
    DECLARE_GLOBAL_SHADER(FFooShaderVS);

    // Bind the empty struct we defined above
    using FParameters = FFooVertexShaderParameters;

    SHADER_USE_PARAMETER_STRUCT(FFooShaderVS, FGlobalShader);

    static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Parameters)
    {
        return IsFeatureLevelSupported(Parameters.Platform, ERHIFeatureLevel::SM5);
    }
};

// 4. Pixel Shader Class
class FFooShaderPS : public FGlobalShader
{
public:
    DECLARE_GLOBAL_SHADER(FFooShaderPS);
    using FParameters = FFooShaderParameters;
    SHADER_USE_PARAMETER_STRUCT(FFooShaderPS, FGlobalShader);

    static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Parameters)
    {
        return IsFeatureLevelSupported(Parameters.Platform, ERHIFeatureLevel::SM5);
    }
};


// =========================================================================
// 2. VISUALIZE LENS DISTORTION SHADER
// =========================================================================

// Define the complex parameters required by the tutorial algorithm
BEGIN_SHADER_PARAMETER_STRUCT(FLensDistortionParameters, )
    SHADER_PARAMETER(FVector2f, PixelUVSize)
    SHADER_PARAMETER(FVector3f, RadialDistortionCoefs)
    SHADER_PARAMETER(FVector2f, TangentialDistortionCoefs)
    SHADER_PARAMETER(FVector4f, UndistortedCameraMatrix)
    SHADER_PARAMETER(FVector4f, DistortedCameraMatrix)
    SHADER_PARAMETER(FVector2f, OutputMultiplyAndAdd)
END_SHADER_PARAMETER_STRUCT()

// VS Class
class FLensDistortionVS : public FGlobalShader
{
public:
    DECLARE_GLOBAL_SHADER(FLensDistortionVS);
    using FParameters = FLensDistortionParameters; // VS needs params to undistort grid
    SHADER_USE_PARAMETER_STRUCT(FLensDistortionVS, FGlobalShader);

    static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Parameters) {
        return IsFeatureLevelSupported(Parameters.Platform, ERHIFeatureLevel::SM5);
    }
};

// PS Class
class FLensDistortionPS : public FGlobalShader
{
public:
    DECLARE_GLOBAL_SHADER(FLensDistortionPS);
    using FParameters = FLensDistortionParameters; // PS needs params for calculating color
    SHADER_USE_PARAMETER_STRUCT(FLensDistortionPS, FGlobalShader);

    static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Parameters) {
        return IsFeatureLevelSupported(Parameters.Platform, ERHIFeatureLevel::SM5);
    }
};

// =========================================================================
// 3. APPLY DISTORTION SHADER
// =========================================================================


// Pixel Shader Parameters
BEGIN_SHADER_PARAMETER_STRUCT(FDistortSceneParameters, )
    SHADER_PARAMETER_TEXTURE(Texture2D, SceneTexture)
    SHADER_PARAMETER_TEXTURE(Texture2D, DistortionTexture)
    SHADER_PARAMETER_SAMPLER(SamplerState, InputSampler)
END_SHADER_PARAMETER_STRUCT()

// 1. Define an empty parameter struct explicitly
BEGIN_SHADER_PARAMETER_STRUCT(FEmptyVSParams, )
END_SHADER_PARAMETER_STRUCT()

// Vertex Shader Class
class FDistortSceneVS : public FGlobalShader
{
public:
    DECLARE_GLOBAL_SHADER(FDistortSceneVS);
    using FParameters = FEmptyVSParams; // Use explicit empty struct
    SHADER_USE_PARAMETER_STRUCT(FDistortSceneVS, FGlobalShader);

    static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Parameters) {
        return IsFeatureLevelSupported(Parameters.Platform, ERHIFeatureLevel::SM5);
    }
};

// Pixel Shader Class
class FDistortScenePS : public FGlobalShader
{
public:
    DECLARE_GLOBAL_SHADER(FDistortScenePS);
    using FParameters = FDistortSceneParameters;
    SHADER_USE_PARAMETER_STRUCT(FDistortScenePS, FGlobalShader);

    static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Parameters) {
        return IsFeatureLevelSupported(Parameters.Platform, ERHIFeatureLevel::SM5);
    }
};

// =========================================================================
// 4. BIT SHIFT SHADER
// =========================================================================

BEGIN_SHADER_PARAMETER_STRUCT(FBitShiftParameters, )
    SHADER_PARAMETER_TEXTURE(Texture2D, InputTexture)
    SHADER_PARAMETER_SAMPLER(SamplerState, InputSampler)
    SHADER_PARAMETER(int, ShiftAmount) // The integer argument
END_SHADER_PARAMETER_STRUCT()

class FBitShiftVS : public FGlobalShader
{
public:
    DECLARE_GLOBAL_SHADER(FBitShiftVS);
    using FParameters = FEmptyVSParams; // Reuse our empty struct
    SHADER_USE_PARAMETER_STRUCT(FBitShiftVS, FGlobalShader);

    static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Parameters) {
        return IsFeatureLevelSupported(Parameters.Platform, ERHIFeatureLevel::SM5);
    }
};

class FBitShiftPS : public FGlobalShader
{
public:
    DECLARE_GLOBAL_SHADER(FBitShiftPS);
    using FParameters = FBitShiftParameters;
    SHADER_USE_PARAMETER_STRUCT(FBitShiftPS, FGlobalShader);

    static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Parameters) {
        return IsFeatureLevelSupported(Parameters.Platform, ERHIFeatureLevel::SM5);
    }
};