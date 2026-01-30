// FooShaderLibrary.cpp
#include "FooShaderLibrary.h"
#include "FooShader.h" 
#include "PixelShaderUtils.h" 

void UFooShaderLibrary::DrawFooShader(const UObject* WorldContextObject, UTextureRenderTarget2D* OutputRenderTarget, FLinearColor Color)
{
    if (!OutputRenderTarget) return;

    FTextureRenderTargetResource* TextureRenderTargetResource = OutputRenderTarget->GameThread_GetRenderTargetResource();

    // 1. CACHE DATA ON GAME THREAD
    // We grab the integers here because we cannot safely access the UObject inside the Render Thread
    int32 Width = OutputRenderTarget->SizeX;
    int32 Height = OutputRenderTarget->SizeY;

    ENQUEUE_RENDER_COMMAND(DrawFooShaderCommand)(
        // 2. CAPTURE DATA BY VALUE
        // We add Width and Height to this list so the lambda knows about them
        [TextureRenderTargetResource, Color, Width, Height](FRHICommandListImmediate& RHICmdList)
        {
            // Check if we are on the render thread
            check(IsInRenderingThread());

            FRHIRenderPassInfo RPInfo(TextureRenderTargetResource->GetRenderTargetTexture(), ERenderTargetActions::Load_Store);
            RHICmdList.BeginRenderPass(RPInfo, TEXT("FooShaderPass"));

            auto ShaderMap = GetGlobalShaderMap(GMaxRHIFeatureLevel);
            TShaderMapRef<FFooShaderPS> PixelShader(ShaderMap);

            FFooShaderParameters Parameters;
            Parameters.MyColor = Color;

            // 3. USE CACHED DIMENSIONS
            FPixelShaderUtils::DrawFullscreenPixelShader(
                RHICmdList,
                ShaderMap,
                PixelShader,
                Parameters,
                FIntRect(0, 0, Width, Height) // <--- Uses the safe integer captures
            );

            RHICmdList.EndRenderPass();
        }
        );
}