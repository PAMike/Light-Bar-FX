//-------------------------------------------------------------------------------------------------------------------
// Created By PLASTIC ANT SOFTWARE, LTD
// Copyright 2018. All Rights Reserved.
//-------------------------------------------------------------------------------------------------------------------

#pragma once

#include "Modules/ModuleManager.h"
#include "PropertyEditorModule.h"
#include "Editor.h"
#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "DetailCategoryBuilder.h"
#include "Layout/Margin.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Layout/SWrapBox.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Input/SButton.h"
#include "IDetailCustomization.h"

#include "PALightBarFXAnimationBank.h"

#define LOCTEXT_NAMESPACE "PALightBarFXEditorAnimationBankDetails"

//-------------------------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------------------------
class FPALightBarFXEditorAnimationBankDetails : public IDetailCustomization
{
	IDetailLayoutBuilder* ThisDetailLayout = nullptr;

public:

	static TSharedRef<IDetailCustomization> MakeInstance()
	{
		return MakeShareable(new FPALightBarFXEditorAnimationBankDetails);
	}

	void OnTypeChanged()
	{
		TArray<TWeakObjectPtr<UObject>> objects;
		ThisDetailLayout->GetObjectsBeingCustomized(objects);
		
		auto fxAsset = Cast<UPALightBarFXAnimationBank>(objects[0]);
		if (fxAsset)
		{
			if (fxAsset->FXListType == EPALightBarFXAnimationBankType::GameplayTag)
			{
				fxAsset->FXListName.Empty();
			}
			else if (fxAsset->FXListType == EPALightBarFXAnimationBankType::Name)
			{
				fxAsset->FXListGameplayTag.Empty();
			}
		}
	}

	EVisibility GetListVisibility(UProperty* InProperty)
	{
		TArray<TWeakObjectPtr<UObject>> objects;
		ThisDetailLayout->GetObjectsBeingCustomized(objects);

		auto fxAsset = Cast<UPALightBarFXAnimationBank>(objects[0]);
		if (fxAsset)
		{
			if (InProperty->GetFName() == GET_MEMBER_NAME_CHECKED(UPALightBarFXAnimationBank, FXListName))
			{
				return fxAsset->FXListType == EPALightBarFXAnimationBankType::Name ? EVisibility::Visible : EVisibility::Collapsed;
			}
			else if (InProperty->GetFName() == GET_MEMBER_NAME_CHECKED(UPALightBarFXAnimationBank, FXListGameplayTag))
			{
				return fxAsset->FXListType == EPALightBarFXAnimationBankType::GameplayTag ? EVisibility::Visible : EVisibility::Collapsed;
			}
		}

		return EVisibility::Visible;
	}

	void CustomizeDetails(IDetailLayoutBuilder& DetailLayout) override
	{
		TSharedPtr<IPropertyHandle> FXListNameProperty = DetailLayout.GetProperty(GET_MEMBER_NAME_CHECKED(UPALightBarFXAnimationBank, FXListName));
		TSharedPtr<IPropertyHandle> FXListGameplayTagProperty = DetailLayout.GetProperty(GET_MEMBER_NAME_CHECKED(UPALightBarFXAnimationBank, FXListGameplayTag));
		TSharedPtr<IPropertyHandle> FXListListTypeProperty = DetailLayout.GetProperty(GET_MEMBER_NAME_CHECKED(UPALightBarFXAnimationBank, FXListType));

		IDetailCategoryBuilder& FXListCategory = DetailLayout.EditCategory(NAME_None);

		ThisDetailLayout = &DetailLayout;

		if (FXListListTypeProperty.IsValid() && FXListNameProperty.IsValid() && FXListGameplayTagProperty.IsValid())
		{
			{
				FXListListTypeProperty->SetOnPropertyValueChanged(FSimpleDelegate::CreateSP(this, &FPALightBarFXEditorAnimationBankDetails::OnTypeChanged));
				FXListCategory.AddProperty(FXListListTypeProperty.ToSharedRef());
				OnTypeChanged();
			}
			{
				TAttribute<EVisibility> propertyVisibility = TAttribute<EVisibility>::Create(TAttribute<EVisibility>::FGetter::CreateSP(this, &FPALightBarFXEditorAnimationBankDetails::GetListVisibility, FXListNameProperty->GetProperty()));
				FXListCategory.AddProperty(FXListNameProperty).Visibility(propertyVisibility);
			}
			{
				TAttribute<EVisibility> propertyVisibility = TAttribute<EVisibility>::Create(TAttribute<EVisibility>::FGetter::CreateSP(this, &FPALightBarFXEditorAnimationBankDetails::GetListVisibility, FXListGameplayTagProperty->GetProperty()));
				FXListCategory.AddProperty(FXListGameplayTagProperty).Visibility(propertyVisibility);
			}
		}
	}
};

#undef LOCTEXT_NAMESPACE
