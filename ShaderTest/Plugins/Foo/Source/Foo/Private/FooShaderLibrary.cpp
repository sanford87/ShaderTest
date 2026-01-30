#include "FooShaderLibrary.h"
#include "FooShader.h"
#include "PixelShaderUtils.h"
#include "TextureResource.h"
#include "CommonRenderResources.h"
#include "ScreenRendering.h" // <--- CRITICAL FIX: Defines FScreenVS and DrawRectangle
#include "PipelineStateCache.h" // Required for PSOs

void UFooShaderLibrary::DrawFooShader(const UObject* WorldContextObject, UTextureRenderTarget2D* OutputRenderTarget, UTexture2D* InputTexture)
{
    if (!OutputRenderTarget || !InputTexture) return;

    FTextureRenderTargetResource* OutResource = OutputRenderTarget->GameThread_GetRenderTargetResource();
    FTextureResource* InResource = InputTexture->GetResource();

    int32 Width = OutputRenderTarget->SizeX;
    int32 Height = OutputRenderTarget->SizeY;

    ENQUEUE_RENDER_COMMAND(DrawFooShaderCommand)(
        [OutResource, InResource, Width, Height](FRHICommandListImmediate& RHICmdList)
        {
            check(IsInRenderingThread());

            FRHIRenderPassInfo RPInfo(OutResource->GetRenderTargetTexture(), ERenderTargetActions::Load_Store);
            RHICmdList.BeginRenderPass(RPInfo, TEXT("FooShaderPass"));

            // 1. Get Shaders
            auto ShaderMap = GetGlobalShaderMap(GMaxRHIFeatureLevel);
            TShaderMapRef<FScreenVS> VertexShader(ShaderMap);
            TShaderMapRef<FFooShaderPS> PixelShader(ShaderMap);

            // 2. Set Pipeline State (PSO)
            FGraphicsPipelineStateInitializer GraphicsPSOInit;
            RHICmdList.ApplyCachedRenderTargets(GraphicsPSOInit);
            GraphicsPSOInit.BlendState = TStaticBlendState<>::GetRHI();
            GraphicsPSOInit.RasterizerState = TStaticRasterizerState<>::GetRHI();
            GraphicsPSOInit.DepthStencilState = TStaticDepthStencilState<false, CF_Always>::GetRHI();

            GraphicsPSOInit.BoundShaderState.VertexDeclarationRHI = GFilterVertexDeclaration.VertexDeclarationRHI;
            GraphicsPSOInit.BoundShaderState.VertexShaderRHI = VertexShader.GetVertexShader();
            GraphicsPSOInit.BoundShaderState.PixelShaderRHI = PixelShader.GetPixelShader();
            GraphicsPSOInit.PrimitiveType = PT_TriangleList;

            // FIX: Added '0' for StencilRef (Required in UE 5.3)
            SetGraphicsPipelineState(RHICmdList, GraphicsPSOInit, 0);

            // 3. Set Parameters
            FFooShaderParameters Parameters;
            Parameters.InputTexture = InResource->TextureRHI;
            Parameters.InputSampler = TStaticSamplerState<SF_Point, AM_Clamp, AM_Clamp, AM_Clamp>::GetRHI();

            SetShaderParameters(RHICmdList, PixelShader, PixelShader.GetPixelShader(), Parameters);

            // 4. Draw Rectangle
            // FIX: Explicitly cast VertexShader to the base shader reference type to satisfy the template
            DrawRectangle(
                RHICmdList,
                0, 0, Width, Height, // Dest Rect
                0, 0, 1, 1,          // Source UVs
                FIntPoint(Width, Height),
                FIntPoint(1, 1),
                VertexShader, // The Vertex Shader to use
                EDRF_Default
            );

            RHICmdList.EndRenderPass();
        }
        );
}