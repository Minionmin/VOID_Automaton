
#include "VOIDCharacter.h"
#include "VOIDCharacterAnimInstance.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/PlayerInputManagerComponent.h"
#include "Components/AttributeComponent.h"
#include "Components/BuffComponent.h"
#include "Components/PotionComponent.h"
#include "Components/WidgetComponent.h"
#include "Components\PlayerInventoryComponent.h"
#include "HUDs/PlayerInfoUI.h"
#include "HUDs/ReloadWidget.h"
#include "Interfaces/HasInstructionWidget.h"
#include "Interfaces/Hitable.h"
#include "Interfaces/Interactable.h"
#include "Enemy/Enemy.h"
#include "Weapons/Weapon.h"
#include "NiagaraFunctionLibrary.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundBase.h"
#include "MyCustomUnrealLibrary.h"
#include "Managers/AudioManager.h"
#include "Saves/MainGameInstance.h"
#include "Saves/MainSaveGame.h"
#include "VOID_Automaton/DebugMacros.h"

AVOIDCharacter::AVOIDCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Player rotates along camera rotation
	bUseControllerRotationRoll = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;

	// Camera & Boomstick
	boomstick = CreateDefaultSubobject<USpringArmComponent>(TEXT("Boomstick"));
	boomstick->SetupAttachment(GetRootComponent());
	boomstick->bUsePawnControlRotation = true;
	boomstick->SocketOffset = FVector(0.f, 50.f, 80.f);

	viewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	viewCamera->SetupAttachment(boomstick);

	// Get variables ref
	VOIDCapsule = GetCapsuleComponent();
	VOIDMesh = GetMesh();
	VOIDMovement = GetCharacterMovement();
	springArmLength = boomstick->TargetArmLength;
	
	// Components
	attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("Player Attributes"));
	buffComponent = CreateDefaultSubobject<UBuffComponent>(TEXT("Buff Component"));
	inputManagerComponent = CreateDefaultSubobject<UPlayerInputManagerComponent>(TEXT("Input Manager Component"));
	potionComponent = CreateDefaultSubobject<UPotionComponent>(TEXT("Potion Component"));
	inventoryComponent = CreateDefaultSubobject<UPlayerInventoryComponent>(TEXT("Inventory Component"));
	
	// Configure character movement
	VOIDMovement->bOrientRotationToMovement = false; // character moves in the direction of input
	VOIDMovement->RotationRate = FRotator(0.f, 540.f, 0.f); // rotate at this rate
	VOIDMovement->MaxWalkSpeed = 500.f; // how fast character can walk
	VOIDMovement->JumpZVelocity = 500.f; // how high character can jump
	VOIDMovement->AirControl = 10.f; // how much character can strafe in the air
	VOIDMovement->AirControlBoostMultiplier = 5.f;
	VOIDMovement->AirControlBoostVelocityThreshold = 50.f;
	VOIDMovement->BrakingDecelerationWalking = 85.f;
	VOIDMovement->GroundFriction = 2.f; // make player sliding a bit after stop moving

	// Make sure player posses this pawn
	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

void AVOIDCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Initializing variables
	Initialize();
}

void AVOIDCharacter::Initialize()
{
	// GameInstanceを最初に取得
	gameInstance = Cast<UMainGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	
	// コントローラー・インプット初期化と処理
	VOIDController = UGameplayStatics::GetPlayerController(this, 0); // Player won't be possessing the controller in class constructor
	if(VOIDController)
	{
		// インプットのバインディングはPlayerInputManagerComponentが担当する
		inputManagerComponent->Initialize(this, GetComponentByClass<UInputComponent>());
	}
	
	// カメラ
	cameraDefaultFOV = viewCamera->FieldOfView;
	cameraCurrentFOV = cameraDefaultFOV;
	defaultMotionBlur = viewCamera->PostProcessSettings.MotionBlurAmount;
	viewCamera->PostProcessSettings.bOverride_MotionBlurAmount = true;
	viewCamera->PostProcessSettings.bOverride_MotionBlurMax = true;
	viewCamera->PostProcessSettings.bOverride_MotionBlurTargetFPS = true;

	// 武器（銃）
	SpawnDefaultWeapon();
	barrelSocket = VOIDMesh->GetSocketByName(BARREL_SOCKET);

	// ムーブメント
	walkSpeed = GetCharacterMovement()->MaxWalkSpeed; // 走る（Run関数）の計算に使う

	// UI
	// ステタスバーの初期化
	playerInfoWidget = CreateWidget<UPlayerInfoUI>(VOIDController, playerInfoWidgetClass);
	if(playerInfoWidget && buffComponent && potionComponent)
	{
		playerInfoWidget->AddToViewport();
		if(equippedWeapon)
		{
			playerInfoWidget->SetAmmoLeft(equippedWeapon->GetMaxAmmo());	
		}
		playerInfoWidget->UpdatePotionUIs(potionComponent->GetPotionTextures(), potionComponent->GetPotionCapacityArray());
		buffComponent->OnBuffListChanged.AddDynamic(playerInfoWidget, &UPlayerInfoUI::UpdateBuffImages);
		potionComponent->OnPotionUsed.AddDynamic(this, &AVOIDCharacter::PlayDrinkPotionMontage);
		potionComponent->OnPotionCapacityChanged.AddDynamic(playerInfoWidget, &UPlayerInfoUI::UpdatePotionCapacity);
	}
	
	// リロードプログレスバーの初期化
	reloadProgressWidget = CreateWidget<UReloadWidget>(VOIDController, reloadProgressWidgetClass);
	if(reloadProgressWidget)
	{
		reloadProgressWidget->AddToViewport();
	}

	// 処理とUIのバインディング
	if(attributes && equippedWeapon)
	{
		// ** ビジュアル | 一般ステタス **
		attributes->OnHealthValueChanged.AddDynamic(playerInfoWidget, &UPlayerInfoUI::SetHealthBarPercentage);
		attributes->OnStaminaValueChanged.AddDynamic(playerInfoWidget, &UPlayerInfoUI::SetStaminaBarPercentage);
		attributes->OnMaxHealthValueChanged.AddDynamic(playerInfoWidget, &UPlayerInfoUI::SetHealthBarSize);
		attributes->OnMaxStaminaValueChanged.AddDynamic(playerInfoWidget, &UPlayerInfoUI::SetStaminaBarSize);
		
		// ** ビジュアル | 戦闘 **
		equippedWeapon->OnAmmoValueChanged.AddDynamic(playerInfoWidget, &UPlayerInfoUI::SetAmmoLeft);
		equippedWeapon->OnReloadProgressStarted.AddDynamic(reloadProgressWidget, &UReloadWidget::Show);
		equippedWeapon->OnReloadProgressChanged.AddDynamic(reloadProgressWidget, &UReloadWidget::UpdateReloadProgress);
		equippedWeapon->OnReloadProgressFinished.AddDynamic(reloadProgressWidget, &UReloadWidget::Hide);
		equippedWeapon->OnReloadProgressFinished.AddDynamic(this, &AVOIDCharacter::StopReload);
		
		// ** 条件 **
		attributes->OnOutOfStamina.AddDynamic(this, &AVOIDCharacter::CancelRun); // スタミナが尽きたら走るのをやめる
	}
	
	IHasSaveGame* hasSaveGame = Cast<IHasSaveGame>(gameInstance);
	if(hasSaveGame)
	{
		const auto saveData = hasSaveGame->GetSaveGame_Implementation()->GetPlayerSaveData();
		// セーブデータをロード（メインメニューから「継続ボタンによる」移動時のみ）
		if(hasSaveGame->GetMainMenuContinue_Implementation())
		{
			// セーブデータの全部をロードする
			UsePlayerSaveData(saveData);
			// フラグをリセット
			hasSaveGame->SetMainMenuContinue_Implementation(false);
		}
		// レベル間のデータを初期化
		else if(hasSaveGame->GetDataNextLevel_Implementation())
		{
			// このレベルで戦闘モードに入るかどうか
			bIsInCombat = hasSaveGame->GetCombatModeNextLevel_Implementation();
			// 設定のデータだけをロードする
			UsePlayerSaveData(saveData, false);
		}
	}

	// プレイヤーのアニメーションセットを決める
	SetDataBasedOnCombatMode();

	// 戦闘中なら、QuestManagerに必要な関数をバインドする
	BindQuestStat();
}

void AVOIDCharacter::LoadCombatModeData()
{
	// 銃のインパクトパーティクルをロード
	if(auto loadedImpactParticle = impactParticleVFXSoft.LoadSynchronous())
	{
		impactParticleVFX = loadedImpactParticle;
	}
}

void AVOIDCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Handle smooth velocity during Dash
	Dash(DeltaTime);

	// Handle smooth camera transition when player is moving or dead
	CameraInterpAction(DeltaTime);
	
	// Handle interpolation for zoom when aiming
	CameraInterpZoom(DeltaTime);
	
	// Calculate how much crosshair should spread
	CalculateCrosshairSpread(DeltaTime);

	// Only trace every frame if we're overlapping item(s)
	TraceForInteract();
}

void AVOIDCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AVOIDCharacter::Move(const FInputActionValue& Value)
{
	if(bIsDashing || bIsDead) return;
	
	const FVector2D MovementVector = Value.Get<FVector2D>();

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation = FRotator(0.f, Rotation.Yaw, 0.f);
	
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(ForwardDirection, MovementVector.X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(RightDirection, MovementVector.Y);
}

void AVOIDCharacter::Jump()
{
	if(bIsDashing || bIsInteracting || bIsDead || !bIsInCombat) return;
	
	Super::Jump();
}

void AVOIDCharacter::Run()
{
	if(bIsDashing || bIsInteracting || bIsAiming || bIsFiring || bIsDead || !bIsInCombat || attributes->HasNoStamina()) return;

	bIsRunning = true;
	VOIDMovement->MaxWalkSpeed = walkSpeed * 1.5f;

	// Start consuming stamina every second we run
	attributes->StartConsumingStamina(
		attributes->GetRunningStaminaCost(),
		attributes->GetRunningStaminaConsumeRate()
		);
}

void AVOIDCharacter::CancelRun()
{
	bIsRunning = false;
	VOIDMovement->MaxWalkSpeed = walkSpeed;

	// Stop consuming stamina after run button is released
	attributes->StopConsumingStamina();
}

void AVOIDCharacter::StartDash()
{
	// This movement required player to tap "Shift (Run Button)" 2 times rapidly
	if(bIsDashing || bIsInteracting || bIsDead || !bIsInCombat || VOIDMovement->IsFalling() || attributes->HasNoStamina()) return;
	
	// Consume required amount of stamina
	attributes->ConsumeStamina(attributes->GetDashingStaminaCost());
	
	// Start smoothing velocity during animation
	bIsDashing = true; // Use curve in to smoothly move in Tick
	dashDir = VOIDMovement->GetLastUpdateVelocity().GetSafeNormal(); // Set direction before moving player
	GetWorldTimerManager().SetTimer(dashAnimationTimer, this, &AVOIDCharacter::StopDash, velocityCurveTime);

	// ** Animation **
	if(dashMontage)
	{
		PlayAnimMontage(dashMontage);	
	}

	// ** VFX **
	if(dashExplosiveVFX && dashTrailVFX)
	{
		// ** Dash ExplosiveVFX **
		// Prevent explosiveVFX from blocking player vision
		const FVector explosiveSpawnPoint = GetActorLocation() + FVector(0.f, 0.f, dashExplosiveZOffset);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), dashExplosiveVFX, explosiveSpawnPoint);

		// ** Dash TrailVFX **
		// Spawn VFX in the opposite direction of input direction
		const FQuat dashOppositeQuat = (dashDir * -1.f).ToOrientationQuat();
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), dashTrailVFX, GetActorLocation(), dashOppositeQuat.Rotator());
	}

	// ** Post Processing **
	viewCamera->PostProcessSettings.MotionBlurAmount = dashMotionBlur;

	// QuestManagerの戦闘Stats更新要
	OnPlayerDash.Broadcast();
}

void AVOIDCharacter::Dash(float deltaTime)
{
	if(!bIsDashing || bIsInteracting || bIsDead || !bIsInCombat) return;

	if(VOIDMovement && velocityMultiplierCurve)
	{
		// Elapsed time since we started dashAnimationTimer
		const float elapsedTime = GetWorldTimerManager().GetTimerElapsed(dashAnimationTimer);
		// Get curve value corresponding to elapsedTime
		const float velocityMultiplier = velocityMultiplierCurve->GetFloatValue(elapsedTime);
		// Move player in the direction with velocity determined by parameter multiplied with curve
		VOIDMovement->Velocity = dashDir * (dashVelocity * velocityMultiplier);
	}
}

void AVOIDCharacter::StopDash()
{
	bIsDashing = false;
	dashDir = FVector::Zero(); // Clear dashDir
	viewCamera->PostProcessSettings.MotionBlurAmount = defaultMotionBlur; // return motion blur to default
}

void AVOIDCharacter::EnterIFrame()
{
	// Will be called in animnotify
	bIsInIFrame = true;
}

void AVOIDCharacter::ExitIFrame()
{
	// Will be called in animnotify
	bIsInIFrame = false;
}

void AVOIDCharacter::MouseLook(const FInputActionValue& Value)
{
	if(bIsDead) return;
	
	const FVector2D LookVector = Value.Get<FVector2D>();

	// マウス感度のスケーリング
	float scaleFactor;
	if (bIsAiming)
	{
		scaleFactor = inputManagerComponent->GetMouseAimingTurnRate();
	}
	else
	{
		scaleFactor = inputManagerComponent->GetMouseHipTurnRate();
	}

	AddControllerPitchInput(LookVector.Y * inputManagerComponent->GetBaseTurnRate() * scaleFactor * GetWorld()->GetDeltaSeconds());
	AddControllerYawInput(LookVector.X * inputManagerComponent->GetBaseTurnRate() * scaleFactor * GetWorld()->GetDeltaSeconds());
}

void AVOIDCharacter::ControllerLook(const FInputActionValue& Value)
{
	if(bIsDead) return;
	
	const FVector2D LookVector = Value.Get<FVector2D>();

	// Sensitivity scaling for controller
	float scaleFactor;
	if (bIsAiming)
	{
		scaleFactor = inputManagerComponent->GetControllerAimingTurnRate();
	}
	else
	{
		scaleFactor = inputManagerComponent->GetControllerHipTurnRate();
	}

	AddControllerPitchInput(LookVector.Y * inputManagerComponent->GetBaseTurnRate() * scaleFactor * GetWorld()->GetDeltaSeconds());
	AddControllerYawInput(LookVector.X * inputManagerComponent->GetBaseTurnRate() * scaleFactor * GetWorld()->GetDeltaSeconds());
}

void AVOIDCharacter::Interact(const FInputActionValue& Value)
{
	if(bIsInteracting || bIsDead) return;
	
	// if traced item isn't null
	if(traceHitInteractLastFrame)
	{
		// And it's interactable, call it's function
		IInteractable* interactableItem = Cast<IInteractable>(traceHitInteractLastFrame);
		if(interactableItem)
		{
			interactableItem->Interact_Implementation(this);
		}
	}
}

void AVOIDCharacter::StartFire()
{
	// Can't fire if dead or no weapon equipped
	if(bIsDead || bIsInteracting || !bIsInCombat || !equippedWeapon) return;
	
	// Left-Mouse is pressed
	bIsFiring = true;
	FireShot();
	GetWorldTimerManager().SetTimer(timerHandlerRefire, this, &AVOIDCharacter::FireShot, equippedWeapon->GetTimeBetweenShot(), true);
}

void AVOIDCharacter::FireShot()
{
	// Don't fire if reloading or cannot input
	if (bIsReloading || bIsInteracting || bIsDead || !bIsInCombat || !equippedWeapon) return;
	
	// Reload if there is no ammo and the player is not already reloading
	if (equippedWeapon->AmmoIsEmpty() && !bIsReloading)
	{
		BeginReload();
		return;
	}

	// Broadcast the event when the weapon is fired
	OnWeaponFired.Broadcast();
	
	// Ammo--
	equippedWeapon->SetAmmo(equippedWeapon->GetCurrentAmmo() - 1);

	// Recoil
	AddControllerYawInput(0.05f);
	AddControllerPitchInput(-0.1f);
	
	// ** Play SFX **
	if (fireSFX)
	{
		if(IManageSingleton* singletonManager = Cast<IManageSingleton>(GetWorld()->GetGameInstance()))
		{
			singletonManager->GetAudioManager_Implementation()->PlaySFX(fireSFX, this);
		}
	}

	// ** Play VFX & LineTrace **
	if (barrelSocket && VOIDMesh)
	{
		const FTransform socketTransform = barrelSocket->GetSocketTransform(VOIDMesh);
		if (muzzleFlashVFX)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), muzzleFlashVFX, socketTransform);
		}

		// Trace here
		FHitResult beamHitResult;
		bool bBeamSuccess = GetBeamEndLocation(socketTransform.GetLocation(), beamHitResult);
		
		// Success in getting beam location from crosshair?
		if (bBeamSuccess)
		{
			// ** Spawn smoke beam VFX whether the bullet hit or not **
			if (beamVFX)
			{
				UParticleSystemComponent* beam = UGameplayStatics::SpawnEmitterAtLocation(
					GetWorld(),
					beamVFX,
					socketTransform
				);

				// Set beam's target (one of Cascade property)
				if (beam)
				{
					beam->SetVectorParameter(FName("Target"), beamHitResult.Location);
				}
			}

			// ** Apply damage + Spawn impact on beam hit **
			if(beamHitResult.bBlockingHit && beamHitResult.GetActor() != nullptr)
			{
				// Does hit Actor implement hitableInterface?
				AActor* hitActor = beamHitResult.GetActor();
				if(IHitable* hitableActor = Cast<IHitable>(hitActor))
				{
					// Damage if hit actor is "Enemy" type
					if(AEnemy* enemy = Cast<AEnemy>(hitActor))
					{
						// ** Deal Damage to Enemy ** //
						int32 dmg = 0;
						bool isHeadShotDmg = false;
						if(beamHitResult.BoneName.ToString() == enemy->GetHeadBone())
						{
							// Deal headshot damage
							isHeadShotDmg = true;
							dmg = GetDamageWithError(equippedWeapon->GetHeadshotDamage());
							// QuestManagerの戦闘Stats更新要
							OnHeadshot.Broadcast();
						}
						else
						{
							// Deal body damage
							dmg = GetDamageWithError(equippedWeapon->GetBodyshotDamage());
							// QuestManagerの戦闘Stats更新要
							OnBodyshot.Broadcast();
						}
						
						// Show dmg dealt (Visual Feedback)
						enemy->ShowDmgIndicator(dmg, beamHitResult.Location, isHeadShotDmg);
						// QuestManagerの戦闘Stats更新要
						OnDamageDealt.Broadcast(dmg);
						UGameplayStatics::ApplyDamage(hitActor, dmg, GetController(), this, UDamageType::StaticClass());
					}

					// Other result
					hitableActor->GetHit_Implementation(beamHitResult);
				}
				else
				{
					if(impactParticleVFX)
					{
						UGameplayStatics::SpawnEmitterAtLocation(
							GetWorld(),
							impactParticleVFX,
							beamHitResult.Location
						);
					}
				}
			}
			// ** End Spawning impact on beam hit **
		}
	}

	// Animation
	if (VOIDAnimInstance && hipFireMontage)
	{
		VOIDAnimInstance->Montage_Play(hipFireMontage);
		VOIDAnimInstance->Montage_JumpToSection(SECTION_START_FIRE);
	}
}

void AVOIDCharacter::StopFire()
{
	bIsFiring = false;
	GetWorldTimerManager().ClearTimer(timerHandlerRefire);
}

float AVOIDCharacter::GetDamageWithError(float damageVal)
{
	// Dealing damage with random error value
	const float errorVal = FMath::RandRange(-2,2);
	return damageVal + errorVal;
}

void AVOIDCharacter::UsePotion(const FInputActionValue& Value)
{
	if(bIsDead || bIsInteracting || bIsRunning || bIsDashing || bIsFiring || bIsReloading || bIsAiming || !bIsInCombat) return;
	
	const auto inputVector = Value.Get<FVector2D>();

	if(inputVector.X > 0)
	{
		// 上のポーションを使う
		potionComponent->UsePotionSlot(0, this);
	}
	else if(inputVector.X < 0)
	{
		// 右のポーションを使う
		potionComponent->UsePotionSlot(1, this);
	}
	else if(inputVector.Y > 0)
	{
		// 下のポーションを使う
		potionComponent->UsePotionSlot(2, this);
	}
	else if(inputVector.Y < 0)
	{
		// 左のポーションを使う
		potionComponent->UsePotionSlot(3, this);
	}
}

void AVOIDCharacter::UpdateOverlappedInteractCount(int amount)
{
	if(overlappedItemCount + amount <= 0)
	{
		overlappedItemCount = 0;
		bShouldTraceForItem = false;
	}
	else
	{
		overlappedItemCount += amount;
		bShouldTraceForItem = true;
	}
}

void AVOIDCharacter::ShowHurtScreen()
{
	// Create hurt screen and add it to viewport when player is taking damage
	if(playerHurtScreenWidgetClass)
	{
		// This widget will get removed automatically after finished playing animation
		UUserWidget* hurtScreen = CreateWidget<UUserWidget>(VOIDController, playerHurtScreenWidgetClass);
		hurtScreen->AddToViewport();
	}
}

float AVOIDCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	// Take no damage if in IFrame or dead
	if(bIsInIFrame || bIsDead)
	{
		DamageAmount = 0.f;
		return DamageAmount;
	}
	
	if(attributes)
	{
		// Clamp between 0 and maxHealth
		attributes->ReceiveDamage(DamageAmount);

		// QuestManagerの戦闘Stats更新要
		OnDamageTaken.Broadcast(DamageAmount);
		
		// Visual feedback on taking damage
		ShowHurtScreen();

		// Die if health is 0
		if(attributes->HasNoHealth())
		{
			Die();

			// QuestManagerにGameoverUIを表示させる
			OnPlayerDead.Broadcast();
		}
	}
	
	return DamageAmount;
}

void AVOIDCharacter::Die()
{
	bIsDead = true;

	// Stop firing, aiming, running, and reloading
	StopFire();
	StopAim();
	CancelRun();
	StopReload();

	// Adjust camera position
	MoveCameraOnDead();
	
	if(deadMontage)
	{
		PlayAnimMontage(deadMontage);
	}
}

void AVOIDCharacter::GetPickUpItem(APickableItem* pickedUpitem)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(3, 2.f, FColor::Green, TEXT("Picked up item!"));
	}
}

void AVOIDCharacter::TraceForInteract()
{
	if(bShouldTraceForItem)
	{
		FHitResult interactTraceResult;
		FVector hitLocation;
		TraceUnderCrosshair(interactTraceResult, hitLocation);
		if(interactTraceResult.bBlockingHit)
		{
			AActor* hitActor = interactTraceResult.GetActor();
			if(hitActor)
			{
				// Check if actor implements IHasInstructionWidget
				IHasInstructionWidget* widgetActor = Cast<IHasInstructionWidget>(hitActor);
				if(widgetActor && widgetActor->GetInstructionWidget_Implementation())
				{
					widgetActor->GetInstructionWidget_Implementation()->SetVisibility(true);
				}
				
				// We are hitting a difference interactable from last frame
				// or interactable is null
				if(traceHitInteractLastFrame && traceHitInteractLastFrame != widgetActor)
				{
					traceHitInteractLastFrame->GetInstructionWidget_Implementation()->SetVisibility(false);
				}
				
				// Store a reference to hitItemLastFrame (can be null)
				traceHitInteractLastFrame = widgetActor;
			}
		}
	}
	else if(traceHitInteractLastFrame)
	{
		// No longer overlapping any items
		// Item last frame should not show widget
		traceHitInteractLastFrame->GetInstructionWidget_Implementation()->SetVisibility(false);
	}
}

bool AVOIDCharacter::TraceUnderCrosshair(FHitResult& outHitResult, FVector& outHitLocation)
{
	// Get current viewport size (for getting crosshair location in world space later)
	FVector2D viewportSize;
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(viewportSize);
	}

	// Get crosshair location on screen
	FVector2D crosshairLocation{ viewportSize.X / 2.f, viewportSize.Y / 2.f };
	FVector crosshairWorldPosition;
	FVector crosshairWorldDirection;

	// Get world transform from screen
	bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(VOIDController, crosshairLocation, crosshairWorldPosition, crosshairWorldDirection);

	if(bScreenToWorld)
	{
		// Trace outward from crosshair's world location (1st trace)
		const FVector screenTraceStart = crosshairWorldPosition;
		const FVector screenTraceEnd = crosshairWorldPosition + crosshairWorldDirection * 4000.f;
		outHitLocation = screenTraceEnd; // in case nothing hit, we output the end point of our trace

		GetWorld()->LineTraceSingleByChannel(
			outHitResult,
			screenTraceStart,
			screenTraceEnd,
			ECollisionChannel::ECC_Visibility
		);

		// If crosshair hits
		if (outHitResult.bBlockingHit)
		{
			outHitLocation = outHitResult.Location; // Update hit location if trace hit an object
		}
		return true;
	}
	// Failed to get crosshair world location
	return false;
}

/// <param name="muzzleSocketLocation">: location for tracing from gun socket </param>
/// <param name="outHitResult">: hitResult occured in the trace function </param>
/// <returns> Did the function success in converting crosshair location to world position </returns>
bool AVOIDCharacter::GetBeamEndLocation(const FVector& muzzleSocketLocation, FHitResult& outHitResult)
{
	// Trace from crosshair
	// Check for crosshair trace hit
	FHitResult crosshairHitResult;
	FVector outBeamLocation;
	bool bCrosshairHit = TraceUnderCrosshair(crosshairHitResult, outBeamLocation);

	// If successfully get crosshair world location
	if(bCrosshairHit)
	{
		// Trace from gun barrel
		const FVector weaponTraceStart = muzzleSocketLocation;
		const FVector startToEnd = outBeamLocation - muzzleSocketLocation;
		const FVector weaponTraceEnd = muzzleSocketLocation + startToEnd * 1.25f; // make line trace from gun barrel a little bit longer

		GetWorld()->LineTraceSingleByChannel(
			outHitResult,
			weaponTraceStart,
			weaponTraceEnd,
			ECollisionChannel::ECC_Visibility
		);

		// Is there any object between barrel and beamEndPoint?
		if (!outHitResult.bBlockingHit)
		{
			// If there is no object in between, adjust final location to the end of beam
			outHitResult.Location = outBeamLocation;
		}
		
		// Successfully get beam world location from screen
		return true;
	}

	// Return false if failed to get beam world location from screen
	return false;
}

void AVOIDCharacter::Aim()
{
	// Can't aim if reloading or dead
	if(bIsReloading || bIsInteracting || bIsDead || !bIsInCombat) return;
	
	// Cancel running on aim
	if(bIsRunning)
	{
		CancelRun();
	}
	
	//LOG("Aiming!");
	bIsAiming = true;
}

void AVOIDCharacter::StopAim()
{
	//LOG("Stop aiming!");
	bIsAiming = false;
}

void AVOIDCharacter::BeginReload()
{
	// Can't reload if ammo is full, dead or not in combat
	if(equippedWeapon->AmmoIsFull() || bIsDead || !bIsInCombat) return;

	StopAim(); // Cancel aiming
	bIsReloading = true;
	equippedWeapon->StartReloading();

	// Play montage & SFX
	if(reloadMontage)
	{
		PlayAnimMontage(reloadMontage,
			equippedWeapon->GetBaseTimeToReload() / equippedWeapon->GetTimeToReload());
	}
}

void AVOIDCharacter::StopReload()
{
	bIsReloading = false;
}

void AVOIDCharacter::MoveCameraOnDead()
{
	// Reset boomstick socket offset
	boomstick->SocketOffset = FVector(0.f, 0.f, 0.f);

	// Since player dead animation makes player bounce back, we need the camera to the back of the player
	// Get player back vector
	const FVector playerBackVector = -GetActorForwardVector() * cameraDeadBackOffset;
	viewCamera->SetWorldLocation(GetActorLocation() + playerBackVector + FVector(0.f, 0.f, cameraDeadZOffset));
}

void AVOIDCharacter::CameraInterpAction(float deltaTime)
{
	// If player is dead
	if(bIsDead)
	{
		// Face camera to the player
		const FRotator newRotation = UActorUtilities::GetRotatorTowardActor(
			viewCamera->GetComponentLocation(),
			GetActorLocation(),
			true);
		const FRotator lookAtRotation = FMath::RInterpTo(viewCamera->GetComponentRotation(), newRotation, deltaTime, cameraDeadInterpSpeed);
		viewCamera->SetWorldRotation(lookAtRotation);
	}

	// Smooth camera transition when player is moving
	if(VOIDAnimInstance && VOIDAnimInstance->GetGroundSpeed())
	{
		
	}
}

void AVOIDCharacter::CameraInterpZoom(float deltaTime)
{
	const float newTargetArmLength = springArmLength + springArmLengthOffset;
	
	// ** Running ** //
	if (bIsRunning)
	{
		boomstick->TargetArmLength = FMath::FInterpTo(boomstick->TargetArmLength, newTargetArmLength, deltaTime, zoomInterpSpeed * 0.5f);
	}
	else
	{
		boomstick->TargetArmLength = FMath::FInterpTo(boomstick->TargetArmLength, springArmLength, deltaTime, zoomInterpSpeed * 0.5f);
	}

	// ** Aiming + Dashing ** //
	if (!bIsDashing && bIsAiming) // just aiming
	{
		// Interpolate to zoomed FOV
		cameraCurrentFOV = FMath::FInterpTo(cameraCurrentFOV, cameraZoomedFOV, deltaTime, zoomInterpSpeed);
	}
	else if(bIsDashing && !bIsAiming) // just dashing
	{
		// Interpolate to zoomed out FOV
		cameraCurrentFOV = FMath::FInterpTo(cameraCurrentFOV, cameraDashZoomedOutFOV, deltaTime, zoomInterpSpeed);
	}
	else if(bIsDashing && bIsAiming) // dashing while aiming
	{
		// Interpolate to zoomed FOV with some extra FOX
		cameraCurrentFOV = FMath::FInterpTo(cameraCurrentFOV, cameraZoomedFOV + extraFOVWhileDashAndAim, deltaTime, zoomInterpSpeed);
	}
	else 
	{
		// Interpolate to default FOV
		cameraCurrentFOV = FMath::FInterpTo(cameraCurrentFOV, cameraDefaultFOV, deltaTime, zoomInterpSpeed); 
	}
	
	// Update camera FOV per frame
	viewCamera->SetFieldOfView(cameraCurrentFOV);
}

void AVOIDCharacter::CalculateCrosshairSpread(float deltaTime)
{
	// Calculate crosshair velocity factor
	const FVector2D walkSpeedRange{ 0.f, VOIDMovement->GetMaxSpeed() };
	const FVector2D velocityMultiplierRange{ 0.f, 1.f };
	FVector velocity = VOIDMovement->Velocity;
	velocity.Z = 0.f;

	crosshairVelocityFactor = FMath::GetMappedRangeValueClamped(walkSpeedRange, velocityMultiplierRange, velocity.Size());

	// Calculate crosshair in air factor
	if (VOIDMovement->IsFalling())
	{
		// Spread the crosshairs slowly while in air
		crosshairInAirFactor = FMath::FInterpTo(crosshairInAirFactor, 2.25f, deltaTime, 2.25f);
	}
	else // Character is on the ground
	{
		// Shrink the crosshair rapidly while on the ground
		crosshairInAirFactor = FMath::FInterpTo(crosshairInAirFactor, 0.f, deltaTime, 30.f);
	}

	// Calculate crosshair aim factor
	if (bIsAiming)
	{
		// Shrink crosshairs a small amount very quickly
		crosshairAimFactor = FMath::FInterpTo(crosshairAimFactor, 0.5f, deltaTime, 30.f);
	}
	else
	{
		// Spread crosshairs back to normal very quickly
		crosshairAimFactor = FMath::FInterpTo(crosshairAimFactor, 0.f, deltaTime, 30.f);
	}

	// Calculate crosshair firing factor
	if (bIsFiring)
	{
		// Limit max error
		const float newFiringError = FMath::Clamp(crosshairFiringFactor + 0.01f, 0.f, 0.6f);
		// Spread crosshairs a small amount very quickly
		crosshairFiringFactor = FMath::FInterpTo(crosshairFiringFactor, newFiringError, deltaTime, 60.f);
	}
	else
	{
		// Spread crosshairs back to normal very quickly
		crosshairFiringFactor = FMath::FInterpTo(crosshairAimFactor, 0.f, deltaTime, 15.f);
	}

	crosshairSpreadMultiplier = 0.5f + crosshairVelocityFactor 
		+ crosshairInAirFactor 
		+ crosshairFiringFactor
		- crosshairAimFactor;
}

FVector AVOIDCharacter::GetCameraInterpLocation()
{
	// ** For item animation **
	const FVector cameraWorldLocation = viewCamera->GetComponentLocation();
	const FVector cameraForwardVector = viewCamera->GetForwardVector();

	// Desired location = cameraWorldLocation + forwardVector * A + upVector * B

	return cameraWorldLocation + cameraForwardVector * cameraInterpDistance
	+ FVector(0.f, 0.f, cameraInterpElevation);
}

void AVOIDCharacter::ESC(const FInputActionValue& Value)
{
	if(menuWidgetClass && menuWidget == nullptr)
	{
		menuWidget = CreateWidget<UUserWidget>(VOIDController, menuWidgetClass);
		menuWidget->AddToViewport();
		UUIUtilities::SetFocusUIAndInGame(this);
	}
	else if(menuWidget)
	{
		menuWidget->RemoveFromParent();
		menuWidget = nullptr;
		UUIUtilities::SetFocusInGame(this);
	}
}

void AVOIDCharacter::PauseInput()
{
	bCanInput = false;
}

void AVOIDCharacter::ResumeInput()
{
	bCanInput = true;
}

void AVOIDCharacter::SpawnDefaultWeapon()
{
	// If player already has a weapon, don't spawn another
	if(equippedWeapon) return;
	
	// 持っている銃をロード
	if(auto defaultWeaponClass = defaultWeaponClassSoft.LoadSynchronous())
	{
		AWeapon* defaultWeapon = GetWorld()->SpawnActor<AWeapon>(defaultWeaponClass);
		if(!defaultWeapon) return;

		const USkeletalMeshSocket* handSocket = VOIDMesh->GetSocketByName(RIGHT_HAND_SOCKET);
		if(handSocket)
		{
			handSocket->AttachActor(defaultWeapon, VOIDMesh);
		}

		equippedWeapon = defaultWeapon;
	}
}

FGameplayTagContainer AVOIDCharacter::GetGameplayTagContainer_Implementation()
{
	return actorTagsContainer;
}

UAttributeComponent* AVOIDCharacter::GetAttribute_Implementation()
{
	return attributes;
}

AWeapon* AVOIDCharacter::GetCurrentWeapon_Implementation()
{
	return equippedWeapon;
}

void AVOIDCharacter::SetDataBasedOnCombatMode()
{
	if(!combatAnimInstanceClass || !nonCombatAnimInstanceClass) return;
	
	// プレイヤーのアニメーションセットを決める
	if (bIsInCombat)
	{
		// 戦闘モード
		VOIDMesh->SetAnimInstanceClass(combatAnimInstanceClass);
		LoadCombatModeData();
		// ムーブメント
		walkSpeed = GetCharacterMovement()->MaxWalkSpeed;
	}
	else
	{
		// 非戦闘モード
		VOIDMesh->SetAnimInstanceClass(nonCombatAnimInstanceClass);
		// ムーブメント
		GetCharacterMovement()->MaxWalkSpeed = nonCombatWalkSpeed;
		// 武器を隠す
		if(equippedWeapon)
		{
			equippedWeapon->HideWeapon();
		}
	}
}

FPlayerSaveStruct AVOIDCharacter::GetPlayerSaveData()
{
	FPlayerSaveStruct playerSaveData = FPlayerSaveStruct();

	// プレイヤーの各データをセーブデータに格納
	playerSaveData.playerLocation = GetActorLocation();
	playerSaveData.controllerRotation = VOIDController->GetControlRotation();
	
	// インプットの設定
	playerSaveData.mouseHipTurnRate = inputManagerComponent->GetMouseHipTurnRate();
	playerSaveData.mouseAimTurnRate = inputManagerComponent->GetMouseAimingTurnRate();
	playerSaveData.controllerHipTurnRate = inputManagerComponent->GetControllerHipTurnRate();
	playerSaveData.controllerAimTurnRate = inputManagerComponent->GetControllerAimingTurnRate();

	// 音量の設定
	if(IManageSingleton* singletonManager = Cast<IManageSingleton>(GetWorld()->GetGameInstance()))
	{
		if(const auto audioManager = singletonManager->GetAudioManager_Implementation())
		{
			playerSaveData.masterVolume = audioManager->GetMasterVolume();
			playerSaveData.bgmVolume = audioManager->GetBGMVolume();
			playerSaveData.sfxVolume = audioManager->GetSFXVolume();	
		}
	}
	
	// バフ
	buffComponent->GetActiveBuffIDs(playerSaveData.buffSaveDataIDs);
	
	// インベントリーのアイテム
	auto& itemInventory = inventoryComponent->GetItemInventory();
	for(const auto& item : itemInventory)
	{
		playerSaveData.playerInventory.Add(item);
	}
	
	return playerSaveData;
}

void AVOIDCharacter::UsePlayerSaveData(FPlayerSaveStruct playerSaveData, bool bFullData)
{
	if(bFullData)
	{
		// プレイヤーの位置情報（基地にいる時のみ）
		if(playerSaveData.playerLocation != FVector::Zero())
		{
			SetActorLocation(playerSaveData.playerLocation);
		}
		VOIDController->SetControlRotation(playerSaveData.controllerRotation);
		
		// インベントリーのアイテム
		// セーブファイルからデータをロードする前に既存のインベントリーデータをクリア
		inventoryComponent->GetItemInventory().Empty();
		for(const auto& item: playerSaveData.playerInventory)
		{
			inventoryComponent->AddItem(item.Key, item.Value);
		}
	}

	// インプットの設定
	inputManagerComponent->SetMouseHipTurnRate(playerSaveData.mouseHipTurnRate);
	inputManagerComponent->SetMouseAimingTurnRate(playerSaveData.mouseAimTurnRate);
	inputManagerComponent->SetControllerHipTurnRate(playerSaveData.controllerHipTurnRate);
	inputManagerComponent->SetControllerAimingTurnRate(playerSaveData.controllerAimTurnRate);


	// 音量の設定
	if(IManageSingleton* singletonManager = Cast<IManageSingleton>(GetWorld()->GetGameInstance()))
	{
		if(const auto audioManager = singletonManager->GetAudioManager_Implementation())
		{
			audioManager->SetMasterVolume(playerSaveData.masterVolume);
			audioManager->SetBGMVolume(playerSaveData.bgmVolume);
			audioManager->SetSFXVolume(playerSaveData.sfxVolume);	
		}
	}
	
	// バフ
	buffComponent->ReloadBuffList();
	
}

void AVOIDCharacter::PlayDrinkPotionMontage()
{
	if(drinkPotionMontage)
	{
		PlayAnimMontage(drinkPotionMontage);
	}
}

void AVOIDCharacter::BindQuestStat()
{
	if(bIsInCombat)
	{
		if(auto questManager = UMasterUtilities::GetQuestManager(this))
		{
			OnHeadshot.AddDynamic(questManager, &UQuestManager::AddHeadshotCount);
			OnBodyshot.AddDynamic(questManager, &UQuestManager::AddBodyshotCount);
			OnDamageDealt.AddDynamic(questManager, &UQuestManager::AddDamageDealtTotal);
			OnDamageTaken.AddDynamic(questManager, &UQuestManager::AddDamageTakenTotal);
			OnPlayerDash.AddDynamic(questManager, &UQuestManager::AddDashCount);
			OnWeaponFired.AddDynamic(questManager, &UQuestManager::AddShotCount);
			OnPlayerDead.AddDynamic(questManager, &UQuestManager::ShowGameOverWidget);

			questManager->OnQuestCompleted.AddDynamic(this, &AVOIDCharacter::StopFire);
			questManager->OnQuestCompleted.AddDynamic(this, &AVOIDCharacter::UnbindQuestStat);
		}
	}
}

void AVOIDCharacter::UnbindQuestStat()
{
	if(auto questManager = UMasterUtilities::GetQuestManager(this))
	{
		OnHeadshot.RemoveDynamic(questManager, &UQuestManager::AddHeadshotCount);
		OnBodyshot.RemoveDynamic(questManager, &UQuestManager::AddBodyshotCount);
		OnDamageDealt.RemoveDynamic(questManager, &UQuestManager::AddDamageDealtTotal);
		OnDamageTaken.RemoveDynamic(questManager, &UQuestManager::AddDamageTakenTotal);
		OnPlayerDash.RemoveDynamic(questManager, &UQuestManager::AddDashCount);
		OnWeaponFired.RemoveDynamic(questManager, &UQuestManager::AddShotCount);
		OnPlayerDead.RemoveDynamic(questManager, &UQuestManager::ShowGameOverWidget);
	}
}