#include "FooShaderLibrary.h"
#include "FooShader.h"
#include "TextureResource.h"
#include "PipelineStateCache.h" 

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

            // 1. Begin Render Pass
            FRHIRenderPassInfo RPInfo(OutResource->GetRenderTargetTexture(), ERenderTargetActions::Load_Store);
            RHICmdList.BeginRenderPass(RPInfo, TEXT("FooShaderPass"));

            // 2. Set Viewport
            RHICmdList.SetViewport(0, 0, 0.0f, Width, Height, 1.0f);

            // 3. Get Shaders
            auto ShaderMap = GetGlobalShaderMap(GMaxRHIFeatureLevel);
            TShaderMapRef<FFooShaderVS> VertexShader(ShaderMap);
            TShaderMapRef<FFooShaderPS> PixelShader(ShaderMap);

            // 4. Create Pipeline State (PSO)
            FGraphicsPipelineStateInitializer GraphicsPSOInit;
            RHICmdList.ApplyCachedRenderTargets(GraphicsPSOInit);
            GraphicsPSOInit.BlendState = TStaticBlendState<>::GetRHI();
            GraphicsPSOInit.RasterizerState = TStaticRasterizerState<>::GetRHI();
            GraphicsPSOInit.DepthStencilState = TStaticDepthStencilState<false, CF_Always>::GetRHI();
            GraphicsPSOInit.PrimitiveType = PT_TriangleList;

            // --- FIX: MANUALLY CREATE EMPTY VERTEX DECLARATION ---
            // This replaces GEmptyVertexDeclaration.VertexDeclarationRHI
            FVertexDeclarationElementList Elements;
            FVertexDeclarationRHIRef EmptyVertexDeclaration = RHICreateVertexDeclaration(Elements);

            GraphicsPSOInit.BoundShaderState.VertexDeclarationRHI = EmptyVertexDeclaration;
            // -----------------------------------------------------

            GraphicsPSOInit.BoundShaderState.VertexShaderRHI = VertexShader.GetVertexShader();
            GraphicsPSOInit.BoundShaderState.PixelShaderRHI = PixelShader.GetPixelShader();

            SetGraphicsPipelineState(RHICmdList, GraphicsPSOInit, 0);

            // 5. Set Pixel Shader Parameters
            FFooShaderParameters Parameters;
            Parameters.InputTexture = InResource->TextureRHI;
            Parameters.InputSampler = TStaticSamplerState<SF_Point, AM_Clamp, AM_Clamp, AM_Clamp>::GetRHI();
            SetShaderParameters(RHICmdList, PixelShader, PixelShader.GetPixelShader(), Parameters);

            // 6. Set Vertex Shader Parameters (Empty)
            FFooVertexShaderParameters VertexParameters;
            SetShaderParameters(RHICmdList, VertexShader, VertexShader.GetVertexShader(), VertexParameters);

            // 7. Draw
            RHICmdList.DrawPrimitive(0, 1, 1);

            RHICmdList.EndRenderPass();
        }
        );
}