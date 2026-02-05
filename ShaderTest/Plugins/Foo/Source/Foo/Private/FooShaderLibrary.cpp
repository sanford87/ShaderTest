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

// --- New Function ---
void UFooShaderLibrary::DrawLensDistortion(
    const UObject* WorldContextObject,
    UTextureRenderTarget2D* OutputRenderTarget,
    float K1, float K2, float K3,
    float P1, float P2,
    float CenterX, float CenterY,
    float Fx, float Fy)
{
    if (!OutputRenderTarget) return;

    FTextureRenderTargetResource* OutResource = OutputRenderTarget->GameThread_GetRenderTargetResource();
    int32 Width = OutputRenderTarget->SizeX;
    int32 Height = OutputRenderTarget->SizeY;

    // Default center if 0
    if (CenterX == 0) CenterX = 0.5f;
    if (CenterY == 0) CenterY = 0.5f;
    if (Fx == 0) Fx = 1.0f;
    if (Fy == 0) Fy = 1.0f;

    ENQUEUE_RENDER_COMMAND(DrawLensDistortionCmd)(
        [OutResource, Width, Height, K1, K2, K3, P1, P2, CenterX, CenterY, Fx, Fy](FRHICommandListImmediate& RHICmdList)
        {
            check(IsInRenderingThread());

            FRHIRenderPassInfo RPInfo(OutResource->GetRenderTargetTexture(), ERenderTargetActions::Load_Store);
            RHICmdList.BeginRenderPass(RPInfo, TEXT("LensDistortionPass"));

            // 1. Viewport
            RHICmdList.SetViewport(0, 0, 0.0f, Width, Height, 1.0f);

            // 2. Shaders
            auto ShaderMap = GetGlobalShaderMap(GMaxRHIFeatureLevel);
            TShaderMapRef<FLensDistortionVS> VertexShader(ShaderMap);
            TShaderMapRef<FLensDistortionPS> PixelShader(ShaderMap);

            // 3. Pipeline
            FGraphicsPipelineStateInitializer GraphicsPSOInit;
            RHICmdList.ApplyCachedRenderTargets(GraphicsPSOInit);
            GraphicsPSOInit.BlendState = TStaticBlendState<>::GetRHI();
            GraphicsPSOInit.RasterizerState = TStaticRasterizerState<>::GetRHI();
            GraphicsPSOInit.DepthStencilState = TStaticDepthStencilState<false, CF_Always>::GetRHI();
            GraphicsPSOInit.PrimitiveType = PT_TriangleList;

            // Create Empty Vertex Declaration (VS uses SV_VertexID)
            FVertexDeclarationElementList Elements;
            FVertexDeclarationRHIRef EmptyVertexDeclaration = RHICreateVertexDeclaration(Elements);
            GraphicsPSOInit.BoundShaderState.VertexDeclarationRHI = EmptyVertexDeclaration;
            GraphicsPSOInit.BoundShaderState.VertexShaderRHI = VertexShader.GetVertexShader();
            GraphicsPSOInit.BoundShaderState.PixelShaderRHI = PixelShader.GetPixelShader();

            SetGraphicsPipelineState(RHICmdList, GraphicsPSOInit, 0);

            // 4. Calculate Parameters
            FLensDistortionParameters Params;
            Params.PixelUVSize = FVector2f(1.0f / Width, 1.0f / Height);
            Params.RadialDistortionCoefs = FVector3f(K1, K2, K3);
            Params.TangentialDistortionCoefs = FVector2f(P1, P2);
            Params.OutputMultiplyAndAdd = FVector2f(1.0f, 0.0f); // Scale 1, Bias 0

            // Construct Matrices (Simplified for tutorial sake)
            // Fx, Fy, Cx, Cy
            Params.DistortedCameraMatrix = FVector4f(Fx, Fy, CenterX, CenterY);
            Params.UndistortedCameraMatrix = FVector4f(Fx, Fy, CenterX, CenterY);

            // 5. Bind Params
            SetShaderParameters(RHICmdList, VertexShader, VertexShader.GetVertexShader(), Params);
            SetShaderParameters(RHICmdList, PixelShader, PixelShader.GetPixelShader(), Params);

            // 6. Draw Grid
            // The shader uses a grid of 32x16. 
            // Triangles = 32 * 16 * 2. Vertices = Triangles * 3.
            // 32 * 16 * 2 * 3 = 3072 vertices.
            RHICmdList.DrawPrimitive(0, 32 * 16 * 2, 1);

            RHICmdList.EndRenderPass();
        }
        );
}


void UFooShaderLibrary::ApplyDistortionToScene(
    const UObject* WorldContextObject,
    UTextureRenderTarget2D* OutputRenderTarget,
    UTexture2D* SceneTexture,
    UTextureRenderTarget2D* DistortionMap)
{
    if (!OutputRenderTarget || !SceneTexture || !DistortionMap) return;

    FTextureRenderTargetResource* OutResource = OutputRenderTarget->GameThread_GetRenderTargetResource();
    FTextureResource* SceneResource = SceneTexture->GetResource();
    FTextureRenderTargetResource* DistMapResource = DistortionMap->GameThread_GetRenderTargetResource();

    int32 Width = OutputRenderTarget->SizeX;
    int32 Height = OutputRenderTarget->SizeY;

    ENQUEUE_RENDER_COMMAND(ApplyDistortionCmd)(
        [OutResource, SceneResource, DistMapResource, Width, Height](FRHICommandListImmediate& RHICmdList)
        {
            check(IsInRenderingThread());

            FRHIRenderPassInfo RPInfo(OutResource->GetRenderTargetTexture(), ERenderTargetActions::Load_Store);
            RHICmdList.BeginRenderPass(RPInfo, TEXT("ApplyDistortionPass"));

            RHICmdList.SetViewport(0, 0, 0.0f, Width, Height, 1.0f);

            auto ShaderMap = GetGlobalShaderMap(GMaxRHIFeatureLevel);
            TShaderMapRef<FDistortSceneVS> VertexShader(ShaderMap);
            TShaderMapRef<FDistortScenePS> PixelShader(ShaderMap);

            FGraphicsPipelineStateInitializer GraphicsPSOInit;
            RHICmdList.ApplyCachedRenderTargets(GraphicsPSOInit);
            GraphicsPSOInit.BlendState = TStaticBlendState<>::GetRHI();
            GraphicsPSOInit.RasterizerState = TStaticRasterizerState<>::GetRHI();
            GraphicsPSOInit.DepthStencilState = TStaticDepthStencilState<false, CF_Always>::GetRHI();
            GraphicsPSOInit.PrimitiveType = PT_TriangleList;

            // Empty VS Declaration
            FVertexDeclarationElementList Elements;
            FVertexDeclarationRHIRef EmptyVertexDeclaration = RHICreateVertexDeclaration(Elements);
            GraphicsPSOInit.BoundShaderState.VertexDeclarationRHI = EmptyVertexDeclaration;
            GraphicsPSOInit.BoundShaderState.VertexShaderRHI = VertexShader.GetVertexShader();
            GraphicsPSOInit.BoundShaderState.PixelShaderRHI = PixelShader.GetPixelShader();

            SetGraphicsPipelineState(RHICmdList, GraphicsPSOInit, 0);

            // Bind Parameters
            FDistortSceneParameters Params;
            Params.SceneTexture = SceneResource->TextureRHI;
            Params.DistortionTexture = DistMapResource->TextureRHI; // The output from previous step
            // Bilinear sampling is important here for smooth warping
            Params.InputSampler = TStaticSamplerState<SF_Bilinear, AM_Clamp, AM_Clamp, AM_Clamp>::GetRHI();

            SetShaderParameters(RHICmdList, PixelShader, PixelShader.GetPixelShader(), Params);

            // Set Empty VS Params amd use the empty struct we defined in the header
            FEmptyVSParams VSParams;
            SetShaderParameters(RHICmdList, VertexShader, VertexShader.GetVertexShader(), VSParams);

            // Draw Full Screen Triangle
            RHICmdList.DrawPrimitive(0, 1, 1);

            RHICmdList.EndRenderPass();
        }
        );
}