// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InputMappingContext.h"
#include "PlayerInputManagerComponent.generated.h"

class AVOIDCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VOID_AUTOMATON_API UPlayerInputManagerComponent : public UActorComponent
{
	
	GENERATED_BODY()

public:	

	UPlayerInputManagerComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void Initialize(AVOIDCharacter* ownerCharacter, UInputComponent* playerInputComponent);
	UFUNCTION()
	void SetupPlayerInputFunction(UInputComponent* playerInputComponent);
	
protected:

	virtual void BeginPlay() override;

public:

	// インプットの感度の範囲（設定UIとの連動用）
	// constexprは他のクラスによって変更されることがない定数を定義するためのキーワード
	static constexpr float BASE_TURN_RATE_MIN = 0.0f;
	static constexpr float BASE_TURN_RATE_MAX = 2.0f;
	
	static constexpr float CONTROLLER_HIP_TURN_RATE_MIN = 0.0f;
	static constexpr float CONTROLLER_HIP_TURN_RATE_MAX = 2.0f;
	static constexpr float CONTROLLER_AIM_TURN_RATE_MIN = 0.0f;
	static constexpr float CONTROLLER_AIM_TURN_RATE_MAX = 2.0f;
	
	static constexpr float MOUSE_HIP_TURN_RATE_MIN = 0.0f;
	static constexpr float MOUSE_HIP_TURN_RATE_MAX = 2.0f;
	static constexpr float MOUSE_AIM_TURN_RATE_MIN = 0.0f;
	static constexpr float MOUSE_AIM_TURN_RATE_MAX = 2.0f;
	
private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess = "true"))
	AVOIDCharacter* VOIDCharacter = nullptr;

	/*
		インプット
	*/
	UPROPERTY(EditDefaultsOnly, Category = Input)
	UInputMappingContext* VOIDContext = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = Input)
	UInputAction* moveAction = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = Input)
	UInputAction* runAction = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = Input)
	UInputAction* dashAction = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = Input)
	UInputAction* mouseLookAction = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = Input)
	UInputAction* controllerLookAction = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = Input)
	UInputAction* jumpAction = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = Input)
	UInputAction* interactAction = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = Input)
	UInputAction* fireAction = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = Input)
	UInputAction* aimAction = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = Input)
	UInputAction* reloadAction = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = Input)
	UInputAction* ESCAction = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = Input)
	UInputAction* useConsumableAction = nullptr;

	/*
		設定
	*/
	UPROPERTY(EditDefaultsOnly, Category = "Options | Gameplay")
	float baseTurnRate = 45.f; // ベース感度（マウスもコントローラーも）
	UPROPERTY(EditDefaultsOnly, Category = "Options | Gameplay", meta = (ClampMin = "0.0", ClampMax = "2.0", UIMin = "0.0", UIMax = "2.0"))
	float controllerHipTurnRate = 1.f; // 腰撃ち時のコントローラーの感度
	UPROPERTY(EditDefaultsOnly, Category = "Options | Gameplay", meta = (ClampMin = "0.0", ClampMax = "2.0", UIMin = "0.0", UIMax = "2.0"))
	float controllerAimingTurnRate = 1.f; // エイム時のコントローラーの感度
	UPROPERTY(EditDefaultsOnly, Category = "Options | Gameplay", meta = (ClampMin = "0.0", ClampMax = "2.0", UIMin = "0.0", UIMax = "2.0"))
	float mouseHipTurnRate = 1.f; // 腰撃ち時のマウスの感度
	UPROPERTY(EditDefaultsOnly, Category = "Options | Gameplay", meta = (ClampMin = "0.0", ClampMax = "2.0", UIMin = "0.0", UIMax = "2.0"))
	float mouseAimingTurnRate = 1.f; // エイム時のマウスの感度

public:
	
	// インプットの感度を取得
	UFUNCTION()
	float GetBaseTurnRate() const { return baseTurnRate; }
	UFUNCTION()
	float GetControllerHipTurnRate() const { return controllerHipTurnRate; }
	UFUNCTION()
	float GetControllerAimingTurnRate() const { return controllerAimingTurnRate; }
	UFUNCTION()
	float GetMouseHipTurnRate() const { return mouseHipTurnRate; }
	UFUNCTION()
	float GetMouseAimingTurnRate() const { return mouseAimingTurnRate; }

	// インプットの感度を設定
	UFUNCTION()
	void SetBaseTurnRate(float newRate);
	UFUNCTION()
	void SetControllerHipTurnRate(float newRate);
	UFUNCTION()
	void SetControllerAimingTurnRate(float newRate);
	UFUNCTION()
	void SetMouseHipTurnRate(float newRate);
	UFUNCTION()
	void SetMouseAimingTurnRate(float newRate);
};