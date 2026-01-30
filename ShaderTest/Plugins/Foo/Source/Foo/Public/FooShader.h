// FooShader.h
#pragma once

#include "GlobalShader.h"
#include "ShaderParameterStruct.h"
#include "DataDrivenShaderPlatformInfo.h" // <--- FIX 2: Needed for IsFeatureLevelSupported

// Define the parameters we want to send to the USF file
BEGIN_SHADER_PARAMETER_STRUCT(FFooShaderParameters, )
    SHADER_PARAMETER_TEXTURE(Texture2D, InputTexture) // Matches 'InputTexture' in USF
    SHADER_PARAMETER_SAMPLER(SamplerState, InputSampler) // Matches 'InputSampler' in USF
END_SHADER_PARAMETER_STRUCT()

class FFooShaderPS : public FGlobalShader
{
public:
    // Declare this class as a global shader
    DECLARE_GLOBAL_SHADER(FFooShaderPS);

    // <--- FIX 1: This line was missing! 
    // It tells the engine that "FParameters" for this class refers to "FFooShaderParameters" defined above.
    using FParameters = FFooShaderParameters;

    SHADER_USE_PARAMETER_STRUCT(FFooShaderPS, FGlobalShader);

    static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Parameters)
    {
        return IsFeatureLevelSupported(Parameters.Platform, ERHIFeatureLevel::SM5);
    }
};