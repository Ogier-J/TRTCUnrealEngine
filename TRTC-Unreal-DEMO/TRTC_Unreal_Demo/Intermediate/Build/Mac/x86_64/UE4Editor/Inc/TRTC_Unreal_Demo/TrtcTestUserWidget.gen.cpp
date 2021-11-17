// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "TRTC_Unreal_Demo/Private/TrtcTestUserWidget.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeTrtcTestUserWidget() {}
// Cross Module References
	TRTC_UNREAL_DEMO_API UClass* Z_Construct_UClass_UTrtcTestUserWidget_NoRegister();
	TRTC_UNREAL_DEMO_API UClass* Z_Construct_UClass_UTrtcTestUserWidget();
	UMG_API UClass* Z_Construct_UClass_UUserWidget();
	UPackage* Z_Construct_UPackage__Script_TRTC_Unreal_Demo();
// End Cross Module References
	void UTrtcTestUserWidget::StaticRegisterNativesUTrtcTestUserWidget()
	{
	}
	UClass* Z_Construct_UClass_UTrtcTestUserWidget_NoRegister()
	{
		return UTrtcTestUserWidget::StaticClass();
	}
	struct Z_Construct_UClass_UTrtcTestUserWidget_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UE4CodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UTrtcTestUserWidget_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UUserWidget,
		(UObject* (*)())Z_Construct_UPackage__Script_TRTC_Unreal_Demo,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTrtcTestUserWidget_Statics::Class_MetaDataParams[] = {
		{ "Comment", "/**\n * \n */" },
		{ "IncludePath", "TrtcTestUserWidget.h" },
		{ "ModuleRelativePath", "Private/TrtcTestUserWidget.h" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_UTrtcTestUserWidget_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UTrtcTestUserWidget>::IsAbstract,
	};
	const UE4CodeGen_Private::FClassParams Z_Construct_UClass_UTrtcTestUserWidget_Statics::ClassParams = {
		&UTrtcTestUserWidget::StaticClass,
		nullptr,
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		nullptr,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		0,
		0,
		0x00A010A0u,
		METADATA_PARAMS(Z_Construct_UClass_UTrtcTestUserWidget_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_UTrtcTestUserWidget_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UTrtcTestUserWidget()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_UTrtcTestUserWidget_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(UTrtcTestUserWidget, 2841699565);
	template<> TRTC_UNREAL_DEMO_API UClass* StaticClass<UTrtcTestUserWidget>()
	{
		return UTrtcTestUserWidget::StaticClass();
	}
	static FCompiledInDefer Z_CompiledInDefer_UClass_UTrtcTestUserWidget(Z_Construct_UClass_UTrtcTestUserWidget, &UTrtcTestUserWidget::StaticClass, TEXT("/Script/TRTC_Unreal_Demo"), TEXT("UTrtcTestUserWidget"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(UTrtcTestUserWidget);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
