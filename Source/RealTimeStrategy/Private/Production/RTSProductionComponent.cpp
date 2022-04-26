#include "Production/RTSProductionComponent.h"

#include "GameFramework/Actor.h"
#include "GameFramework/Controller.h"
#include "Engine/SCS_Node.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Sound/SoundCue.h"

#include "RTSGameMode.h"
#include "RTSLog.h"
#include "RTSPlayerController.h"
#include "RTSPlayerAdvantageComponent.h"
#include "Economy/RTSPlayerResourcesComponent.h"
#include "Libraries/RTSCollisionLibrary.h"
#include "Libraries/RTSGameplayLibrary.h"
#include "Libraries/RTSGameplayTagLibrary.h"
#include "Production/RTSProductionCostComponent.h"


URTSProductionComponent::URTSProductionComponent(const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);

	// Set reasonable default values.
	CapacityPerQueue = 5;
	QueueCount = 1;

	InitialGameplayTags.AddTag(URTSGameplayTagLibrary::Status_Permanent_CanProduce());
}

void URTSProductionComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(URTSProductionComponent, ProductionQueues);
	DOREPLIFETIME(URTSProductionComponent, MostRecentProduct);
	DOREPLIFETIME(URTSProductionComponent, RallyPoint);
}

void URTSProductionComponent::BeginPlay()
{
	Super::BeginPlay();

	// Setup queues.
	for (int32 QueueIndex = 0; QueueIndex < QueueCount; ++QueueIndex)
	{
		FRTSProductionQueue NewQueue;
		ProductionQueues.Add(NewQueue);
	}
}

void URTSProductionComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	UActorComponent::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Don't update production progress on clients - will be replicated from server.
	if (GetNetMode() == NM_Client)
	{
		return;
	}

	// Check for speed boosts.
	float SpeedBoostFactor = 1.0f;
	AActor* OwningActor = GetOwner();

	if (OwningActor)
	{
		const AActor* OwningPlayer = OwningActor->GetOwner();

		if (OwningPlayer)
		{
			const URTSPlayerAdvantageComponent* PlayerAdvantageComponent = OwningPlayer->FindComponentByClass<URTSPlayerAdvantageComponent>();

			if (PlayerAdvantageComponent)
			{
				SpeedBoostFactor = PlayerAdvantageComponent->GetSpeedBoostFactor();
			}
		}
	}

	// Process all queues.
	for (int32 QueueIndex = 0; QueueIndex < QueueCount; ++QueueIndex)
	{
		if (ProductionQueues[QueueIndex].Num() <= 0)
		{
			continue;
		}

		// Check production costs.
		const auto ProductClass = GetCurrentProduction(QueueIndex);
		const auto ProductionCostComponent = URTSGameplayLibrary::FindDefaultComponentByClass<URTSProductionCostComponent>(ProductClass);

		bool bProductionCostPaid = false;

		if (ProductionCostComponent && ProductionCostComponent->GetProductionCostType() == ERTSPaymentType::PAYMENT_PayOverTime)
		{
			const auto Owner = GetOwner()->GetOwner();

			if (!Owner)
			{
				UE_LOG(LogRTS, Error, TEXT("%s needs to pay for production, but has no owner."), *GetOwner()->GetName());
				continue;
			}

			const auto PlayerResourcesComponent = Owner->FindComponentByClass<URTSPlayerResourcesComponent>();

			if (!PlayerResourcesComponent)
			{
				UE_LOG(LogRTS, Error, TEXT("%s needs to pay for production, but has no PlayerResourcesComponent."), *Owner->GetName());
				continue;
			}

			bool bCanPayAllProductionCosts = true;

			for (const auto& Resource : ProductionCostComponent->GetResources())
			{
				const float ResourceAmount = Resource.Value * SpeedBoostFactor * DeltaTime / ProductionCostComponent->GetProductionTime();

				if (!PlayerResourcesComponent->CanPayResources(Resource.Key, ResourceAmount))
				{
					// Production stopped until resources become available again.
					bCanPayAllProductionCosts = false;
					break;
				}
			}

			if (bCanPayAllProductionCosts)
			{
				// Pay production costs.
				for (const auto& Resource : ProductionCostComponent->GetResources())
				{
					const float ResourceAmount = Resource.Value * SpeedBoostFactor * DeltaTime / ProductionCostComponent->GetProductionTime();
					PlayerResourcesComponent->PayResources(Resource.Key, ResourceAmount);
				}

				bProductionCostPaid = true;
			}
		}
		else
		{
			bProductionCostPaid = true;
		}

		if (!bProductionCostPaid)
		{
			continue;
		}

		// Update production progress.
		ProductionQueues[QueueIndex].RemainingProductionTime -= SpeedBoostFactor * DeltaTime;

		// Check if finished.
		if (ProductionQueues[QueueIndex].RemainingProductionTime <= 0)
		{
			FinishProduction(QueueIndex);
		}
		else
		{
			OnProductionProgressChanged.Broadcast(OwningActor, QueueIndex, GetProgressPercentage(QueueIndex));
		}
	}

	// Verify rally points.
	if (RallyPoint.bIsSet && IsValid(RallyPoint.TargetActor) && !URTSGameplayLibrary::IsVisibleForActor(GetOwner(), RallyPoint.TargetActor))
	{
		ClearRallyPoint();
	}
}

bool URTSProductionComponent::CanAssignProduction(TSubclassOf<AActor> ProductClass) const
{
	return URTSGameplayLibrary::IsReadyToUse(GetOwner()) && FindQueueForProduct(ProductClass) >= 0;
}

int32 URTSProductionComponent::FindQueueForProduct(TSubclassOf<AActor> ProductClass) const
{
	// Find queue with least products that is not at capacity limit.
	int32 QueueWithLeastProducts = -1;
	int32 ProductsInShortestQueue = CapacityPerQueue;

	for (int32 QueueIndex = 0; QueueIndex < QueueCount; ++QueueIndex)
	{
		const FRTSProductionQueue& Queue = ProductionQueues[QueueIndex];

		if (Queue.Num() < ProductsInShortestQueue)
		{
			QueueWithLeastProducts = QueueIndex;
			ProductsInShortestQueue = Queue.Num();
		}
	}

	return QueueWithLeastProducts;
}

void URTSProductionComponent::GetProduction(TMap<TSubclassOf<AActor>, int32>& Production) const
{
	for (FRTSProductionQueue ProductionQueue : ProductionQueues)
	{
		const int32 Amount = ProductionQueue.Num();
		for (int32 Index = 0; Index < Amount; Index++)
		{
			int32& Count = Production.FindOrAdd(ProductionQueue[Index]);
			++Count;
		}
	}
}

TSubclassOf<AActor> URTSProductionComponent::GetCurrentProduction(int32 QueueIndex /*= 0*/) const
{
	if (QueueIndex < 0 || QueueIndex >= QueueCount)
	{
		return nullptr;
	}

	const FRTSProductionQueue& Queue = ProductionQueues[QueueIndex];

	return Queue.Num() > 0 ? Queue[0] : nullptr;
}

float URTSProductionComponent::GetProductionTime(int32 QueueIndex /*= 0*/) const
{
	const TSubclassOf<AActor> CurrentProduction = GetCurrentProduction(QueueIndex);

	if (!CurrentProduction)
	{
		return 0.0f;
	}

	return GetProductionTimeForProduct(CurrentProduction);
}

float URTSProductionComponent::GetProductionTimeForProduct(TSubclassOf<AActor> ProductClass) const
{
	const URTSProductionCostComponent* ProductionCostComponent =
		URTSGameplayLibrary::FindDefaultComponentByClass<URTSProductionCostComponent>(ProductClass);
	return ProductionCostComponent ? ProductionCostComponent->GetProductionTime() : 0.0f;
}

float URTSProductionComponent::GetProgressPercentage(int32 QueueIndex /*= 0*/) const
{
	const float TotalProductionTime = GetProductionTime(QueueIndex);

	if (TotalProductionTime <= 0.0f)
	{
		return 1.0f;
	}

	const float RemainingProductionTime = GetRemainingProductionTime(QueueIndex);

	if (RemainingProductionTime <= 0.0f)
	{
		return 1.0f;
	}

	return 1 - (RemainingProductionTime / TotalProductionTime);
}

float URTSProductionComponent::GetRemainingProductionTime(int32 QueueIndex /*= 0*/) const
{
	if (QueueIndex < 0 || QueueIndex >= QueueCount)
	{
		return 0.0f;
	}

	return ProductionQueues[QueueIndex].RemainingProductionTime;
}

bool URTSProductionComponent::IsProducing() const
{
	for (int32 QueueIndex = 0; QueueIndex < QueueCount; ++QueueIndex)
	{
		const FRTSProductionQueue& Queue = ProductionQueues[QueueIndex];

		if (Queue.Num() > 0)
		{
			return true;
		}
	}

	return false;
}

void URTSProductionComponent::StartProduction(TSubclassOf<AActor> ProductClass)
{
	// Check production state.
	if (!CanAssignProduction(ProductClass))
	{
		return;
	}

	const int32 QueueIndex = FindQueueForProduct(ProductClass);

	if (QueueIndex < 0)
	{
		return;
	}

	// Check requirements.
	TSubclassOf<AActor> MissingRequirement;

	if (URTSGameplayLibrary::GetMissingRequirementFor(this, GetOwner(), ProductClass, MissingRequirement))
	{
		UE_LOG(LogRTS, Error, TEXT("%s wants to produce %s, but is missing requirement %s."), *GetOwner()->GetName(), *ProductClass->GetName(), *MissingRequirement->GetName());
		return;
	}

	// Check production cost.
	const URTSProductionCostComponent* ProductionCostComponent =
		URTSGameplayLibrary::FindDefaultComponentByClass<URTSProductionCostComponent>(ProductClass);

	if (ProductionCostComponent && ProductionCostComponent->GetProductionCostType() == ERTSPaymentType::PAYMENT_PayImmediately)
	{
		const auto Owner = GetOwner()->GetOwner();

		if (!Owner)
		{
			UE_LOG(LogRTS, Error, TEXT("%s needs to pay for production, but has no owner."), *GetOwner()->GetName());
			return;
		}

		const auto PlayerResourcesComponent = Owner->FindComponentByClass<URTSPlayerResourcesComponent>();

		if (!PlayerResourcesComponent)
		{
			UE_LOG(LogRTS, Error, TEXT("%s needs to pay for production, but has no PlayerResourcesComponent."), *Owner->GetName());
			return;
		}

		if (!PlayerResourcesComponent->CanPayAllResources(ProductionCostComponent->GetResources()))
		{
			UE_LOG(LogRTS, Error, TEXT("%s needs to pay for producing %s, but does not have enough resources."),
			       *Owner->GetName(),
			       *ProductClass->GetName());
			return;
		}

		// Pay production costs.
		PlayerResourcesComponent->PayAllResources(ProductionCostComponent->GetResources());
	}

	// Insert into queue.
	FRTSProductionQueue& Queue = ProductionQueues[QueueIndex];
	Queue.Add(ProductClass);

	UE_LOG(LogRTS, Log, TEXT("%s queued %s for production in queue %i."), *GetOwner()->GetName(), *ProductClass->GetName(), QueueIndex);

	// Notify listeners.
	OnProductQueued.Broadcast(GetOwner(), ProductClass, QueueIndex);

	if (Queue.Num() == 1)
	{
		// Start production.
		StartProductionInQueue(QueueIndex);
	}
}

void URTSProductionComponent::FinishProduction(int32 QueueIndex /*= 0*/)
{
	if (QueueIndex < 0 || QueueIndex >= QueueCount)
	{
		return;
	}

	FRTSProductionQueue& Queue = ProductionQueues[QueueIndex];
	Queue.RemainingProductionTime = 0.0f;

	// Get game.
	ARTSGameMode* GameMode = Cast<ARTSGameMode>(UGameplayStatics::GetGameMode(this));

	if (!GameMode)
	{
		return;
	}

	const TSubclassOf<AActor> ProductClass = Queue[0];

	// Determine spawn location: Start at producing actor location.
	FVector SpawnLocation = GetOwner()->GetActorLocation();

	// Spawn next to production actor.
	float SpawnOffset = 0.0f;
	SpawnOffset += URTSCollisionLibrary::GetActorCollisionSize(GetOwner()) / 2;
	SpawnOffset += URTSCollisionLibrary::GetCollisionSize(ProductClass) / 2;
	SpawnOffset *= 1.05f;
	SpawnLocation.X -= SpawnOffset;

	// Calculate location on the ground.
	SpawnLocation = URTSCollisionLibrary::GetGroundLocation(this, SpawnLocation);

	// Prevent spawn collision or spawning at wrong side of the world.
	SpawnLocation.Z += URTSCollisionLibrary::GetCollisionHeight(ProductClass) * 1.1f;

	// Spawn product.
	AActor* Product = GameMode->SpawnActorForPlayer(
		ProductClass,
		Cast<AController>(GetOwner()->GetOwner()),
		FTransform(FRotator::ZeroRotator, SpawnLocation));

	if (!Product)
	{
		return;
	}

	UE_LOG(LogRTS, Log, TEXT("%s finished producing %s in queue %i."), *GetOwner()->GetName(), *Product->GetName(), QueueIndex);

	MostRecentProduct = Product;

	// Use rally point.
	IssueRallyPointDependentOrder(Product);

	// Notify listeners.
	NotifyOnProductionFinished(GetOwner(), Product, QueueIndex);

	// Remove product from queue.
	DequeueProduct(QueueIndex);
}

void URTSProductionComponent::CancelProduction(int32 QueueIndex /*= 0*/, int32 ProductIndex /*= 0*/)
{
	// Get queue.
	if (QueueIndex < 0 || QueueIndex >= QueueCount)
	{
		return;
	}

	const FRTSProductionQueue& Queue = ProductionQueues[QueueIndex];

	// Get product.
	if (ProductIndex < 0 || ProductIndex >= Queue.Num())
	{
		return;
	}

	const TSubclassOf<AActor> ProductClass = Queue[ProductIndex];

	// Get elapsed production time.
	const float TotalProductionTime = GetProductionTimeForProduct(ProductClass);
	const float RemainingProductionTime = ProductIndex == 0 ? ProductionQueues[QueueIndex].RemainingProductionTime : TotalProductionTime;
	const float ElapsedProductionTime = TotalProductionTime - RemainingProductionTime;

	UE_LOG(LogRTS, Log, TEXT("%s canceled producing product %i of class %s in queue %i after %f seconds."),
	       *GetOwner()->GetName(),
	       ProductIndex,
	       *ProductClass->GetName(),
	       QueueIndex,
	       ElapsedProductionTime);

	// Notify listeners.
	OnProductionCanceled.Broadcast(GetOwner(), ProductClass, QueueIndex, ElapsedProductionTime);

	// Remove product from queue.
	DequeueProduct(QueueIndex, ProductIndex);

	// Refund resources.
	const URTSProductionCostComponent* ProductionCostComponent =
		URTSGameplayLibrary::FindDefaultComponentByClass<URTSProductionCostComponent>(ProductClass);

	if (ProductionCostComponent)
	{
		const auto Owner = GetOwner()->GetOwner();

		if (!Owner)
		{
			return;
		}

		const auto PlayerResourcesComponent = Owner->FindComponentByClass<URTSPlayerResourcesComponent>();

		if (!PlayerResourcesComponent)
		{
			return;
		}

		float TimeRefundFactor = 0.0f;

		if (ProductionCostComponent->GetProductionCostType() == ERTSPaymentType::PAYMENT_PayImmediately)
		{
			TimeRefundFactor = 1.0f;
		}
		else if (ProductionCostComponent->GetProductionCostType() == ERTSPaymentType::PAYMENT_PayOverTime)
		{
			TimeRefundFactor = ElapsedProductionTime / TotalProductionTime;
		}

		const float ActualRefundFactor = ProductionCostComponent->GetRefundFactor() * TimeRefundFactor;

		// Refund production costs.
		for (const auto& Resource : ProductionCostComponent->GetResources())
		{
			TSubclassOf<URTSResourceType> ResourceType = Resource.Key;
			float ResourceAmount = Resource.Value * ActualRefundFactor;

			PlayerResourcesComponent->AddResources(ResourceType, ResourceAmount);

			UE_LOG(LogRTS, Log, TEXT("%f %s of production costs refunded."), ResourceAmount, *ResourceType->GetName());

			// Notify listeners.
			OnProductionCostRefunded.Broadcast(GetOwner(), ResourceType, ResourceAmount);
		}
	}
}

void URTSProductionComponent::SetRallyPointToActor(AActor* Target)
{
	RallyPoint.TargetActor = Target;
	RallyPoint.bIsSet = true;
	
	OnRallyPointSet.Broadcast(RallyPoint.TargetActor, RallyPoint.TargetLocation);
}

void URTSProductionComponent::SetRallyPointToLocation(const FVector& TargetLocation)
{
	RallyPoint.TargetActor = nullptr;
	RallyPoint.TargetLocation = TargetLocation;
	RallyPoint.bIsSet = true;

	OnRallyPointSet.Broadcast(RallyPoint.TargetActor, RallyPoint.TargetLocation);
}

void URTSProductionComponent::ClearRallyPoint()
{
	RallyPoint.TargetActor = nullptr;
	RallyPoint.bIsSet = false;

	OnClearRallyPoint.Broadcast();
}

TArray<TSubclassOf<AActor>> URTSProductionComponent::GetAvailableProducts() const
{
	return AvailableProducts;
}

int32 URTSProductionComponent::GetQueueCount() const
{
	return QueueCount;
}

int32 URTSProductionComponent::GetCapacityPerQueue() const
{
	return CapacityPerQueue;
}

FRTSProductionRallyPoint URTSProductionComponent::GetRallyPoint() const
{
	return RallyPoint;
}

void URTSProductionComponent::NotifyOnProductionFinished(AActor* Actor, AActor* Product, int32 QueueIndex)
{
	// Notify listeners.
	OnProductionFinished.Broadcast(Actor, Product, QueueIndex);

	// Play sound.
	if (IsValid(Product) && URTSGameplayLibrary::IsOwnedByLocalPlayer(Actor))
	{
		const URTSProductionCostComponent* ProductionCostComponent = Product->FindComponentByClass<URTSProductionCostComponent>();

		if (IsValid(ProductionCostComponent) && IsValid(ProductionCostComponent->GetFinishedSound()))
		{
			UGameplayStatics::PlaySound2D(this, ProductionCostComponent->GetFinishedSound());
		}
	}
}

void URTSProductionComponent::DequeueProduct(int32 QueueIndex /*= 0*/, int32 ProductIndex /*= 0*/)
{
	// Get queue.
	if (QueueIndex < 0 || QueueIndex >= QueueCount)
	{
		return;
	}

	FRTSProductionQueue& Queue = ProductionQueues[QueueIndex];

	if (ProductIndex < 0 || ProductIndex >= Queue.Num())
	{
		return;
	}

	Queue.RemoveAt(ProductIndex);

	// Check if need to start next production.
	if (ProductIndex == 0 && Queue.Num() > 0)
	{
		StartProductionInQueue(QueueIndex);
	}
}

void URTSProductionComponent::StartProductionInQueue(int32 QueueIndex /*= 0*/)
{
	// Get queue.
	if (QueueIndex < 0 || QueueIndex >= QueueCount)
	{
		return;
	}

	FRTSProductionQueue& Queue = ProductionQueues[QueueIndex];

	// Get product.
	if (Queue.Num() <= 0)
	{
		return;
	}

	const TSubclassOf<AActor> ProductClass = Queue[0];

	// Start production.
	const float ProductionTime = GetProductionTimeForProduct(ProductClass);
	Queue.RemainingProductionTime = ProductionTime;

	UE_LOG(LogRTS, Log, TEXT("%s started producing %s in queue %i."), *GetOwner()->GetName(), *ProductClass->GetName(), QueueIndex);

	// Notify listeners.
	OnProductionStarted.Broadcast(GetOwner(), ProductClass, QueueIndex, ProductionTime);
}

void URTSProductionComponent::IssueRallyPointDependentOrder(AActor* ProducedActor)
{
	if (!RallyPoint.bIsSet)
	{
		return;
	}

	ARTSPlayerController* PlayerController = Cast<ARTSPlayerController>(ProducedActor->GetOwner());

	if (!IsValid(PlayerController))
	{
		return;
	}

	PlayerController->IssueDefaultOrderToActor(ProducedActor, RallyPoint.TargetActor, RallyPoint.TargetLocation);
}

void URTSProductionComponent::ReceivedProductionQueues()
{
	for (int32 QueueIndex = 0; QueueIndex < QueueCount; ++QueueIndex)
	{
		OnProductionProgressChanged.Broadcast(GetOwner(), QueueIndex, GetProgressPercentage(QueueIndex));
	}
}

void URTSProductionComponent::ReceivedMostRecentProduct()
{
	NotifyOnProductionFinished(GetOwner(), MostRecentProduct, -1);
}
