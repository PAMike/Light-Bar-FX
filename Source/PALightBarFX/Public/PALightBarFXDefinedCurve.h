//-------------------------------------------------------------------------------------------------------------------
// Created By PLASTIC ANT SOFTWARE, LTD
// Copyright 2018. All Rights Reserved.
//-------------------------------------------------------------------------------------------------------------------

#pragma once

#include <cmath>
#include "CoreMinimal.h"
#include "Runtime/Engine/Classes/Curves/CurveFloat.h"
#include "PALightBarFXDefinedCurve.generated.h"

//-------------------------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------------------------
UENUM()
enum class EPALightBarFXDefinedCurveType : int8
{
	Linear,
	Logistic,
	Normal,
	Sine,
	Random,
	PassThrough
};

//-------------------------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------------------------
UCLASS(EditInlineNew)
class PALIGHTBARFX_API UPALightBarFXDefinedCurve : public UObject
{
	GENERATED_BODY()

	static float Sanitize(const float x)
	{
		return FMath::Clamp(x, 0.0f, 1.0f);
	}

#if WITH_EDITORONLY_DATA
	void UpdatePreviewCurve()
	{
		Curve.EditorCurveData.Reset();

		switch (CurveType)
		{
		default:
			for (int32 i = 0; i < CurvePreviewSteps; ++i)
			{
				const float t = float(i) / float(CurvePreviewSteps);
				Curve.EditorCurveData.AddKey(t, ComputeValue(t));
			}
			break;

		case EPALightBarFXDefinedCurveType::PassThrough:
			for (int32 i = 0; i < CurvePreviewSteps; ++i)
			{
				const float t = float(i) / float(CurvePreviewSteps);
				Curve.EditorCurveData.AddKey(t, 0.0f);
			}
			break;
		}
	}

	void PostEditChangeProperty(FPropertyChangedEvent& event) override
	{
		Super::PostEditChangeProperty(event);
		UpdatePreviewCurve();
	}

	bool CanEditChange(const UProperty* InProperty) const override
	{
		Super::CanEditChange(InProperty);
		if (InProperty)
		{
			const FName propertyName = InProperty->GetFName();

			if (propertyName == GET_MEMBER_NAME_CHECKED(UPALightBarFXDefinedCurve, CurveType) ||
				propertyName == GET_MEMBER_NAME_CHECKED(UPALightBarFXDefinedCurve, Slope) ||
				propertyName == GET_MEMBER_NAME_CHECKED(UPALightBarFXDefinedCurve, Exponent) ||
				propertyName == GET_MEMBER_NAME_CHECKED(UPALightBarFXDefinedCurve, XY) ||
				propertyName == GET_MEMBER_NAME_CHECKED(UPALightBarFXDefinedCurve, InverseResult) ||
				propertyName == GET_MEMBER_NAME_CHECKED(UPALightBarFXDefinedCurve, OutputScale) ||
				propertyName == GET_MEMBER_NAME_CHECKED(UPALightBarFXDefinedCurve, CurvePreviewSteps))
			{
				return !Curve.ExternalCurve;
			}
		}
		return true;
	}
#endif

	UPROPERTY(EditAnywhere, Category = "Light Bar FX|Defined Curve", meta = (AllowPrivateAccess = "true"))
	EPALightBarFXDefinedCurveType CurveType = EPALightBarFXDefinedCurveType::Linear;

	UPROPERTY(EditAnywhere, Category = "Light Bar FX|Defined Curve", meta = (AllowPrivateAccess = "true"))
	float Slope = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Light Bar FX|Defined Curve", meta = (AllowPrivateAccess = "true"))
	float Exponent = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Light Bar FX|Defined Curve", meta = (AllowPrivateAccess = "true"))
	FVector2D XY = FVector2D::ZeroVector;

	UPROPERTY(EditAnywhere, Category = "Light Bar FX|Defined Curve", meta = (AllowPrivateAccess = "true"))
	bool InverseResult = false;

	UPROPERTY(EditAnywhere, Category = "Light Bar FX|Defined Curve", meta = (AllowPrivateAccess = "true"))
	float OutputScale = 1.0f;

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, Category = "Light Bar FX|Defined Curve", meta = (AllowPrivateAccess = "true"))
	int32 CurvePreviewSteps = 32;
#endif

	UPROPERTY(EditAnywhere, Category = "Light Bar FX|Defined Curve", meta = (AllowPrivateAccess = "true"))
	FRuntimeFloatCurve Curve;

public:

	UPALightBarFXDefinedCurve()
	{
#if WITH_EDITORONLY_DATA
		UpdatePreviewCurve();
#endif
	}

	float ComputeValue(const float x) const
	{
		float cost;

		if (Curve.ExternalCurve)
		{
			cost = Curve.ExternalCurve->GetFloatValue(x);
		}
		else
		{
			switch (CurveType)
			{
			default:
				cost = Sanitize((Slope * (x - XY.X)) + XY.Y);
				break;

			case EPALightBarFXDefinedCurveType::PassThrough:
				cost = x;
				break;

			case EPALightBarFXDefinedCurveType::Random:
				cost = FMath::RandRange(0.0f, 1.0f);
				break;

			case EPALightBarFXDefinedCurveType::Logistic:
				cost = Sanitize((Slope / (1.0f + std::expf(-10.0f * Exponent * (x - 0.5f - XY.X)))) + XY.Y);
				break;

			case EPALightBarFXDefinedCurveType::Normal:
				cost = Sanitize(Slope * std::expf(-30.0f * Exponent * (x - XY.X - 0.5f) * (x - XY.X - 0.5f)) + XY.Y);
				break;

			case EPALightBarFXDefinedCurveType::Sine:
				cost = Sanitize(0.5f * Slope * std::sinf(2.0f * 3.1415926535f * (x - XY.X)) + 0.5f + XY.Y);
				break;
			}
		}

		return OutputScale * (InverseResult ? 1.0f - cost : cost);
	}
};

