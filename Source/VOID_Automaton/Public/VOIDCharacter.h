
#pragma once

#include "InputActionValue.h"
#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Character.h"
#include "Interfaces/HasAttribute.h"
#include "Interfaces/HasGameplayTag.h"
#include "Interfaces/HasSaveGame.h"
#include "Interfaces/HasWeapon.h"
#include "Templates/SubclassOf.h"
#include "VOIDCharacter.generated.h"

class UPotionComponent;
class UMainGameInstance;
class UBuffComponent;
class UAttributeComponent;
class UPlayerInventoryComponent;
class UVOIDCharacterAnimInstance;
class UNiagaraSystem;
class UCapsuleComponent;
class USkeletalMeshComponent;
class USpringArmComponent;
class UCameraComponent;
class UCharacterMovementComponent;
class USoundBase;
class UParticleSystem;
class UAnimMontage;
class APickableItem;
class AWeapon;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHeadshot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBodyshot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDamageDealt, int, damageAmount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDamageTaken, int, damageAmount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerDash);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWeaponFired);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerDead);

UCLASS()
class VOID_AUTOMATON_API AVOIDCharacter : public ACharacter, public IHasGameplayTag, public IHasAttribute, public IHasWeapon
{
	GENERATED_BODY()

	// インプットマネージャーがこのクラスのプライベートメンバーにアクセスできるようにする
	friend class UPlayerInputManagerComponent;
	
public:

	AVOIDCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/*
		イベント
	*/
	FOnHeadshot OnHeadshot; // when player hit an enemy's head
	FOnBodyshot OnBodyshot; // when player hit an enemy's body
	FOnDamageDealt OnDamageDealt; // when player dealt damage
	FOnDamageTaken OnDamageTaken; // when player took damage
	FOnPlayerDash OnPlayerDash; // when player dashed
	FOnWeaponFired OnWeaponFired; // when player fired a bullet
	FOnPlayerDead OnPlayerDead; // when player died
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	/*
		インターフェース
	*/
	virtual FGameplayTagContainer GetGameplayTagContainer_Implementation() override;
	virtual UAttributeComponent* GetAttribute_Implementation() override;
	virtual AWeapon* GetCurrentWeapon_Implementation() override;

	// ** ゲームセーブ ** //
	UFUNCTION()
	FPlayerSaveStruct GetPlayerSaveData();
	UFUNCTION()
	void UsePlayerSaveData(FPlayerSaveStruct playerSaveData, bool bIsBase = true);
	
	/*
		一般
	*/
	void GetPickUpItem(APickableItem* pickedUpitem);
	UFUNCTION(BlueprintCallable)
	void PauseInput();
	UFUNCTION(BlueprintCallable)
	void ResumeInput();
	
	// ** 一般 | ムーブメント **
	UFUNCTION(BlueprintCallable)
	void EnterIFrame();
	UFUNCTION(BlueprintCallable)
	void ExitIFrame();
	
protected:

	virtual void BeginPlay() override;
	virtual void Initialize();

	UFUNCTION()
	void LoadCombatModeData();
	
	UFUNCTION()
	void BindQuestStat();
	UFUNCTION()
	void UnbindQuestStat();
	
	void Move(const FInputActionValue& Value);
	virtual void Jump() override;
	void Run();
	UFUNCTION()
	void CancelRun();
	void MouseLook(const FInputActionValue& Value);
	void ControllerLook(const FInputActionValue& Value);
	void Interact(const FInputActionValue& Value);
	void ESC(const FInputActionValue& Value);
	
	/*
		戦闘関数
	*/
	// ** 武器 **//
	void SpawnDefaultWeapon();
	
	// ** 射撃 ** //
	UFUNCTION()
	void StartFire();
	UFUNCTION()
	void FireShot();
	UFUNCTION()
	void StopFire();
	bool GetBeamEndLocation(const FVector& muzzleSocketLocation, FHitResult& outHitResult);

	// ** エイム ** //
	void Aim();
	void StopAim();

	// ** カメラ ** //
	void MoveCameraOnDead();
	void CameraInterpZoom(float deltaTime);
	void CameraInterpAction(float deltaTime);
	
	// ** クロスヘア ** //
	void CalculateCrosshairSpread(float deltaTime);

	// ** リロード ** //
	UFUNCTION()
	void BeginReload();
	UFUNCTION()
	void StopReload();
	
	// ** ダメージの誤差 ** //
	float GetDamageWithError(float damageVal);

	// ** アビリティ・ポーション ** //
	UFUNCTION()
	void UsePotion(const FInputActionValue& Value);
	
	// ** ムーブメント ** //
	UFUNCTION()
	void StartDash();
	UFUNCTION()
	void Dash(float deltaTime);
	UFUNCTION()
	void StopDash();

	// ** Animation ** //
	void SetDataBasedOnCombatMode();
	UFUNCTION()
	void PlayDrinkPotionMontage();
	
	// ** 死亡 ** //
	UFUNCTION()
	void Die();
	
	/*
		UI
	*/
	UFUNCTION()
	void ShowHurtScreen();
	
	// ** トレース ** //
	bool TraceUnderCrosshair(FHitResult& outHitResult, FVector& outHitLocation); // クロスヘアの下にトレースを行う
	void TraceForInteract();
	
protected:	

	/*
		コンポネント
	*/
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCapsuleComponent* VOIDCapsule;
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* VOIDMesh;
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCharacterMovementComponent* VOIDMovement;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USpringArmComponent* boomstick;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UCameraComponent* viewCamera;
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	APlayerController* VOIDController;
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	UPlayerInputManagerComponent* inputManagerComponent;
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	UAttributeComponent* attributes;
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	UBuffComponent* buffComponent;
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	UPotionComponent* potionComponent;
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	UPlayerInventoryComponent* inventoryComponent;
	
	/*
		関連のある変数
	*/
	// ** アニメーション ** //
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UVOIDCharacterAnimInstance* VOIDAnimInstance;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UMainGameInstance* gameInstance;
	
	// ** カメラ | ブームスティック ** //
	UPROPERTY()
	float springArmLength = 0.f;
	UPROPERTY()
	float springArmLengthOffset = 100.f; // 走っている時にカメラが遠ざかる演出

	// ** 一般 | アイテム | トレス ** //
	UPROPERTY(VisibleAnywhere, Category = "General | Item | Tracing")
	int overlappedItemCount = 0; // プレイヤーと重なっているアイテムの数
	class IHasInstructionWidget* traceHitInteractLastFrame; // 前のフレームでトレースされたアイテム

	// ** 一般 | アイテム | Interping ** //
	UPROPERTY(EditAnywhere, Category = "General | Item | Interping")
	float cameraInterpDistance = 250.f; // Interpolation forward vector offset for picking up item animation
	UPROPERTY(EditAnywhere, Category = "General | Item | Interping")
	float cameraInterpElevation = 65.f; // Interpolation up vector offset for picking up item animation

	// ** UI ** //
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> playerInfoWidgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UI")
	class UPlayerInfoUI* playerInfoWidget = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> playerHurtScreenWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> reloadProgressWidgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UI")
	class UReloadWidget* reloadProgressWidget = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> menuWidgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UI")
	UUserWidget* menuWidget = nullptr;

	/*
		タグ
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTagContainer actorTagsContainer;

	/*
		戦闘
	*/
	// ** 銃 ** //
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat | Gun")
	AWeapon* equippedWeapon = nullptr; // プレイヤーが持っている武器
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat | Gun", meta = (AllowPrivateAccess = "true"))
	TSoftClassPtr<AWeapon> defaultWeaponClassSoft = nullptr; // スポーンした時に持っている武器クラスのソフト参照
	
	UPROPERTY();
	FTimerHandle timerHandlerRefire;

	// ** クロスヘア ** //
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat | Crosshair")
	float crosshairSpreadMultiplier = 0.f; // クロスヘア拡散の定数
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat | Crosshair")
	float crosshairVelocityFactor = 0.f; // 移動している時のクロスヘアの拡散
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat | Crosshair")
	float crosshairInAirFactor = 0.f; // 空中状態のクロスヘアの拡散
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat | Crosshair")
	float crosshairAimFactor = 0.f; // エイム時のクロスヘアの拡散
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat | Crosshair")
	float crosshairFiringFactor = 0.f; // 撃っている時のクロスヘアの拡散
	
	/*
		カメラ
	*/
	// ** カメラ | エイム ** //
	float cameraDefaultFOV = 0.f; // 元のFOV（BeginPlayでセット）
	float cameraZoomedFOV = 45.f; // プレイヤーがエイムしている時のFOV
	float cameraCurrentFOV = 0.f; // 後でInterpするために使う（BeginPlayでセット）
	UPROPERTY(EditDefaultsOnly, Category = "Camera | Aiming")
	float zoomInterpSpeed = 15.f; // 走る時 + エイムする時のカメラのInterpスピード

	// ** カメラ | 死亡 ** //
	UPROPERTY(EditAnywhere, Category = "Camera | Dead")
	float cameraDeadBackOffset = 200.f; // 死亡時のカメラの後ろのオフセット
	UPROPERTY(EditAnywhere, Category = "Camera | Dead")
	float cameraDeadZOffset = 200.f; // 死亡時のカメラのZオフセット
	UPROPERTY(EditAnywhere, Category = "Camera | Dead")
	float cameraDeadInterpSpeed = 2.f; // 死亡時のカメラののInterpスピード
	
	/*
		ムーブメント
	*/
	UPROPERTY(EditDefaultsOnly, Category = "Movement | Parameters")
	float nonCombatWalkSpeed = 400.f;
	UPROPERTY(EditDefaultsOnly, Category = "Movement | Parameters")
	float walkSpeed = 500.f;

	// ** ムーブメント | ダッシュ ** //
	UPROPERTY(VisibleAnywhere, Category = "Movement | Dash")
	FVector dashDir = FVector::Zero();
	UPROPERTY(EditAnywhere, Category = "Movement | Dash")
	float dashVelocity = 2500.f; // プレイヤーのダッシュ速度
	UPROPERTY(EditDefaultsOnly, Category = "Movement | Dash")
	UCurveFloat* velocityMultiplierCurve; // ダッシュアニメーション中のプレイヤーの速度をコントロールするためのカーブ
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement | Dash")
	float velocityCurveTime = 0.5f; // カーブの総計時間
	FTimerHandle dashAnimationTimer; // ダッシュアニメーションが始まってからどれだけの時間が経過したかを測定するため（カーブに使う）
	

	// ** ムーブメント | ダッシュ | 演出効果 ** //
	UPROPERTY(EditAnywhere, Category = "Movement | Dash | Post Processing")
	float cameraDashZoomedOutFOV = 120.f; // ダッシュ中のFOVの補正値（元のFOVは90）
	UPROPERTY(EditAnywhere, Category = "Movement | Dash | Post Processing")
	float extraFOVWhileDashAndAim = 20.f; // ダッシュ中にエイムしているときのFOVの補正値
	UPROPERTY(VisibleAnywhere, Category = "Movement | Dash | Post Processing")
	float defaultMotionBlur = 0.f; // ダッシュ前の元のモーションブラー値
	UPROPERTY(EditAnywhere, Category = "Movement | Dash | Post Processing")
	float dashMotionBlur = 2.0f; // ダッシュ中のモーションブラー値
	
	
	/*
		ステート
	*/
	UPROPERTY(VisibleAnywhere, Category = "States | General")
	bool bCanInput = true; // false if in animation or certain condition
	UPROPERTY(VisibleAnywhere, Category = "States | General")
	bool bIsRunning = false;
	UPROPERTY(VisibleAnywhere, Category = "States | General")
	bool bIsInteracting = false;
	UPROPERTY(VisibleAnywhere, Category = "States | General")
	bool bShouldTraceForItem = false; // 毎フレームアイテムをトレースするかどうか（プレイヤーがアイテムと重なっているかどうか）
	UPROPERTY(VisibleAnywhere, Category = "States | General")
	bool bIsInCombat = false; // プレイヤーが戦闘中かどうか
	UPROPERTY(VisibleAnywhere, Category = "States | Combat")
	bool bIsAiming = false;
	UPROPERTY(VisibleAnywhere, Category = "States | Combat")
	bool bIsFiring = false; // 左クリックを押しているかどうか
	UPROPERTY(VisibleAnywhere, Category = "States | Combat")
	bool bIsReloading = false;
	UPROPERTY(VisibleAnywhere, Category = "States | Combat")
	bool bIsDashing = false;
	UPROPERTY(VisibleAnywhere, Category = "States | Combat")
	bool bIsInIFrame = false; // ダッシュ中は無敵状態になる
	UPROPERTY(VisibleAnywhere, Category = "States | Combat")
	bool bIsDead = false;

	/*
		AnimInstance
	*/
	UPROPERTY(EditDefaultsOnly, Category = "AnimInstance")
	TSubclassOf<UVOIDCharacterAnimInstance> combatAnimInstanceClass;
	UPROPERTY(EditDefaultsOnly, Category = "AnimInstance")
    TSubclassOf<UVOIDCharacterAnimInstance> nonCombatAnimInstanceClass;
	
	/*
		AnimMontage
	*/
	UPROPERTY(EditDefaultsOnly, Category = "Montages | Combat")
	UAnimMontage* hipFireMontage; // firing weapon
	UPROPERTY(EditDefaultsOnly, Category = "Montages | Combat")
	UAnimMontage* reloadMontage; // reload weapon
	UPROPERTY(EditDefaultsOnly, Category = "Montages | Combat")
	UAnimMontage* drinkPotionMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Montages | Combat")
	UAnimMontage* dashMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Montages | Combat")
	UAnimMontage* deadMontage;

	/*
		SFXs
	*/
	UPROPERTY(EditDefaultsOnly, Category = "SFXs | Combat")
	USoundBase* fireSFX;

	/*
		VFXs
	*/
	UPROPERTY(EditDefaultsOnly, Category = "VFXs | Combat")
	UParticleSystem* muzzleFlashVFX = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = "VFXs | Combat")
	TSoftObjectPtr<UParticleSystem> impactParticleVFXSoft;
	UPROPERTY(EditAnywhere, Category = "VFXs | Combat")
	UParticleSystem* impactParticleVFX = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = "VFXs | Combat")
	UParticleSystem* beamVFX = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = "VFXs | Movement")
	UNiagaraSystem* dashExplosiveVFX = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = "VFXs | Movement")
	UNiagaraSystem* dashTrailVFX = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = "VFXs | Movement | Options")
	float dashExplosiveZOffset = -100.f; // オフセットを使って爆発VFXがプレイヤーの視界を遮らないようにする

	/*
		キーワード (タイプミスを防ぐために)
	*/
	// ** Socket ** //
	const FName BARREL_SOCKET = TEXT("barrelSocket");
	const FName RIGHT_HAND_SOCKET = TEXT("rightHandSocket");

	// ** AnimMontage Section ** //
	const FName SECTION_START_FIRE = TEXT("StartFire");

	/*
		Sockets
	*/
	UPROPERTY()
	const USkeletalMeshSocket* barrelSocket; // this character's barrel socket
	
public:

	// ** ゲッター ** //
	FORCEINLINE float GetWalkSpeed() const { return walkSpeed; }
	FORCEINLINE int GetOverlappedItemCount() const { return overlappedItemCount; }
	FORCEINLINE UCameraComponent* GetCamera() const { return viewCamera; }
	FORCEINLINE UAttributeComponent* GetAttributes() const { return attributes; }
	FORCEINLINE UBuffComponent* GetBuffComponent() const { return buffComponent; }
	FORCEINLINE UPlayerInputManagerComponent* GetInputManagerComponent() const { return inputManagerComponent; }
	
	// ** ステート ** //
	FORCEINLINE bool IsAiming() const { return bIsAiming; }
	FORCEINLINE bool IsInIFrame() const { return bIsInIFrame; }
	FORCEINLINE bool IsDead() const { return bIsDead; }
	UFUNCTION(BlueprintCallable)
	bool IsInCombat() const { return bIsInCombat; }
	UFUNCTION(BlueprintCallable)
	void SetIsInteracting(bool bInteracting) { bIsInteracting = bInteracting; }
	
	// ** 他 ** //
	UFUNCTION(BlueprintCallable)
	float GetCrosshairSpreadMultiplier() const { return crosshairSpreadMultiplier; }
	UFUNCTION()
	FVector GetCameraInterpLocation();
	UFUNCTION()
	void UpdateOverlappedInteractCount(int amount);
};
