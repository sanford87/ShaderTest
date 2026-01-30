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
};
