#pragma once
#include "GlobalShader.h"
#include "ShaderParameterStruct.h"

// Define parameters to pass to the shader
BEGIN_SHADER_PARAMETER_STRUCT(FMyShaderParameters,) 
    SHADER_PARAMETER(FLinearColor, MyColor) // Matches 'float4 MyColor' in USF
END_SHADER_PARAMETER_STRUCT()

class FMyColorShaderPS : public FGlobalShader
{
public:
    // Declare the shader type for UE to recognize
    DECLARE_GLOBAL_SHADER(FMyColorShaderPS);
    SHADER_USE_PARAMETER_STRUCT(FMyColorShaderPS, FGlobalShader);

    // Required serialization constructor
    static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Parameters)
    {
        return IsFeatureLevelSupported(Parameters.Platform, ERHIFeatureLevel::SM5);
    }
};