// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "TRTC_Unreal_Demo/TRTC_Unreal_DemoGameModeBase.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeTRTC_Unreal_DemoGameModeBase() {}
// Cross Module References
	TRTC_UNREAL_DEMO_API UClass* Z_Construct_UClass_ATRTC_Unreal_DemoGameModeBase_NoRegister();
	TRTC_UNREAL_DEMO_API UClass* Z_Construct_UClass_ATRTC_Unreal_DemoGameModeBase();
	ENGINE_API UClass* Z_Construct_UClass_AGameModeBase();
	UPackage* Z_Construct_UPackage__Script_TRTC_Unreal_Demo();
// End Cross Module References
	void ATRTC_Unreal_DemoGameModeBase::StaticRegisterNativesATRTC_Unreal_DemoGameModeBase()
	{
	}
	UClass* Z_Construct_UClass_ATRTC_Unreal_DemoGameModeBase_NoRegister()
	{
		return ATRTC_Unreal_DemoGameModeBase::StaticClass();
	}
	struct Z_Construct_UClass_ATRTC_Unreal_DemoGameModeBase_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UE4CodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_ATRTC_Unreal_DemoGameModeBase_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_AGameModeBase,
		(UObject* (*)())Z_Construct_UPackage__Script_TRTC_Unreal_Demo,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ATRTC_Unreal_DemoGameModeBase_Statics::Class_MetaDataParams[] = {
		{ "Comment", "/**\n * \n */" },
		{ "HideCategories", "Info Rendering MovementReplication Replication Actor Input Movement Collision Rendering Utilities|Transformation" },
		{ "IncludePath", "TRTC_Unreal_DemoGameModeBase.h" },
		{ "ModuleRelativePath", "TRTC_Unreal_DemoGameModeBase.h" },
		{ "ShowCategories", "Input|MouseInput Input|TouchInput" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_ATRTC_Unreal_DemoGameModeBase_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<ATRTC_Unreal_DemoGameModeBase>::IsAbstract,
	};
	const UE4CodeGen_Private::FClassParams Z_Construct_UClass_ATRTC_Unreal_DemoGameModeBase_Statics::ClassParams = {
		&ATRTC_Unreal_DemoGameModeBase::StaticClass,
		"Game",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		nullptr,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		0,
		0,
		0x009002ACu,
		METADATA_PARAMS(Z_Construct_UClass_ATRTC_Unreal_DemoGameModeBase_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_ATRTC_Unreal_DemoGameModeBase_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_ATRTC_Unreal_DemoGameModeBase()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_ATRTC_Unreal_DemoGameModeBase_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(ATRTC_Unreal_DemoGameModeBase, 1962867826);
	template<> TRTC_UNREAL_DEMO_API UClass* StaticClass<ATRTC_Unreal_DemoGameModeBase>()
	{
		return ATRTC_Unreal_DemoGameModeBase::StaticClass();
	}
	static FCompiledInDefer Z_CompiledInDefer_UClass_ATRTC_Unreal_DemoGameModeBase(Z_Construct_UClass_ATRTC_Unreal_DemoGameModeBase, &ATRTC_Unreal_DemoGameModeBase::StaticClass, TEXT("/Script/TRTC_Unreal_Demo"), TEXT("ATRTC_Unreal_DemoGameModeBase"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(ATRTC_Unreal_DemoGameModeBase);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
