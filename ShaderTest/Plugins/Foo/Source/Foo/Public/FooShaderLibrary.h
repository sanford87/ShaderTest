// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Engine/TextureRenderTarget2D.h"
#include "FooShaderLibrary.generated.h"

/**
 * 
 */
UCLASS()
class FOO_API UFooShaderLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable, Category = "Foo Shader", meta = (WorldContext = "WorldContextObject"))
    static void DrawFooShader(const UObject* WorldContextObject, UTextureRenderTarget2D* OutputRenderTarget, UTexture2D* InputTexture);

    // New Function for Visualizing Lens Distortion
    UFUNCTION(BlueprintCallable, Category = "Foo Shader", meta = (WorldContext = "WorldContextObject"))
    static void DrawLensDistortion(
        const UObject* WorldContextObject,
        UTextureRenderTarget2D* OutputRenderTarget,
        float K1, float K2, float K3,
        float P1, float P2,
        float CenterX = 0.5, float CenterY = 0.5,
        float Fx = 1.0, 
        float Fy = 1.0
    );
    // New Function for applying Lens Distortion
    UFUNCTION(BlueprintCallable, Category = "Foo Shader", meta = (WorldContext = "WorldContextObject"))
    static void ApplyDistortionToScene(
        const UObject* WorldContextObject,
        UTextureRenderTarget2D* OutputRenderTarget,
        UTextureRenderTarget2D* SceneTexture,
        UTextureRenderTarget2D* DistortionMap
    );

    UFUNCTION(BlueprintCallable, Category = "Foo Shader", meta = (WorldContext = "WorldContextObject"))
    static void ApplyBitShift(
        const UObject* WorldContextObject,
        UTextureRenderTarget2D* OutputRenderTarget,
        UTextureRenderTarget2D* InputTexture,
        int32 BitShift
    );

};
