#pragma once

#include "GlobalShader.h"
#include "ShaderParameterStruct.h"
#include "DataDrivenShaderPlatformInfo.h"

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