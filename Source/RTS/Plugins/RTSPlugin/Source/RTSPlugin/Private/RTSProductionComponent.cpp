#include "RTSPluginPrivatePCH.h"
#include "RTSProductionComponent.h"

#include "GameFramework/Actor.h"
#include "Engine/BlueprintGeneratedClass.h"
#include "Engine/SCS_Node.h"

#include "RTSGameMode.h"
#include "RTSProductionCostComponent.h"
#include "RTSUtilities.h"


URTSProductionComponent::URTSProductionComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicated(true);
}

void URTSProductionComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(URTSProductionComponent, ProductionQueues);
}

void URTSProductionComponent::BeginPlay()
{
	UActorComponent::BeginPlay();

	// Setup queues.
	for (int32 QueueIndex = 0; QueueIndex < QueueCount; ++QueueIndex)
	{
		FRTSProductionQueue NewQueue;
		ProductionQueues.Add(NewQueue);
	}
}

void URTSProductionComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	UActorComponent::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Process all queues.
	for (int32 QueueIndex = 0; QueueIndex < QueueCount; ++QueueIndex)
	{
		if (ProductionQueues[QueueIndex].Num() <= 0)
		{
			continue;
		}

		// Update production progress.
		ProductionQueues[QueueIndex].RemainingProductionTime -= DeltaTime;

		// Check if finished.
		if (ProductionQueues[QueueIndex].RemainingProductionTime <= 0)
		{
			FinishProduction(QueueIndex);
		}
	}
}

bool URTSProductionComponent::CanAssignProduction(TSubclassOf<AActor> ProductClass) const
{
	return FindQueueForProduct(ProductClass) >= 0;
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
	TSubclassOf<AActor> CurrentProduction = GetCurrentProduction(QueueIndex);

	if (!CurrentProduction)
	{
		return 0.0f;
	}

	return GetProductionTimeForProduct(CurrentProduction);
}

float URTSProductionComponent::GetProductionTimeForProduct(TSubclassOf<AActor> ProductClass) const
{
	URTSProductionCostComponent* ProductionCostComponent =
		URTSUtilities::FindDefaultComponentByClass<URTSProductionCostComponent>(ProductClass);
	return ProductionCostComponent ? ProductionCostComponent->ProductionTime : 0.0f;
}

float URTSProductionComponent::GetProgressPercentage(int32 QueueIndex /*= 0*/) const
{
	float TotalProductionTime = GetProductionTime(QueueIndex);

	if (TotalProductionTime <= 0.0f)
	{
		return 1.0f;
	}

	float RemainingProductionTime = GetRemainingProductionTime(QueueIndex);

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
	int32 QueueIndex = FindQueueForProduct(ProductClass);

	if (QueueIndex < 0)
	{
		return;
	}

	// Insert into queue.
	FRTSProductionQueue& Queue = ProductionQueues[QueueIndex];
	Queue.Add(ProductClass);

	UE_LOG(RTSLog, Log, TEXT("%s queued %s for production in queue %i."), *GetOwner()->GetName(), *ProductClass->GetName(), QueueIndex);

	// Notify listeners.
	OnProductQueued.Broadcast(ProductClass, QueueIndex);

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
	UWorld* World = GetWorld();

	if (!World)
	{
		return;
	}

	ARTSGameMode* GameMode = Cast<ARTSGameMode>(UGameplayStatics::GetGameMode(World));

	if (!GameMode)
	{
		return;
	}

	// Spawn product.
	TSubclassOf<AActor> ProductClass = Queue[0];

	AActor* Product = GameMode->SpawnActorForPlayer(
		ProductClass,
		Cast<ARTSPlayerController>(GetOwner()->GetOwner()),
		FTransform(FRotator::ZeroRotator, GetOwner()->GetActorLocation()));

	if (!Product)
	{
		return;
	}

	UE_LOG(RTSLog, Log, TEXT("%s finished producing %s in queue %i."), *GetOwner()->GetName(), *Product->GetName(), QueueIndex);

	// Notify listeners.
	OnProductionFinished.Broadcast(Product, QueueIndex);

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

	FRTSProductionQueue& Queue = ProductionQueues[QueueIndex];

	// Get product.
	if (ProductIndex < 0 || ProductIndex >= Queue.Num())
	{
		return;
	}

	TSubclassOf<AActor> ProductClass = Queue[ProductIndex];

	// Get elapsed production time.
	float TotalProductionTime = GetProductionTimeForProduct(ProductClass);
	float RemainingProductionTime = ProductIndex == 0 ? ProductionQueues[QueueIndex].RemainingProductionTime : TotalProductionTime;
	float ElapsedProductionTime = TotalProductionTime - RemainingProductionTime;

	UE_LOG(RTSLog, Log, TEXT("%s canceled producing product %i of type %s in queue %i after %f seconds."),
		*GetOwner()->GetName(),
		ProductIndex,
		*ProductClass->GetName(),
		QueueIndex,
		ElapsedProductionTime);

	// Notify listeners.
	OnProductionCanceled.Broadcast(ProductClass, QueueIndex, ElapsedProductionTime);

	// Remove product from queue.
	DequeueProduct(QueueIndex, ProductIndex);
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

	TSubclassOf<AActor> ProductClass = Queue[0];

	// Start production.
	float ProductionTime = GetProductionTimeForProduct(ProductClass);
	Queue.RemainingProductionTime = ProductionTime;

	UE_LOG(RTSLog, Log, TEXT("%s started producing %s in queue %i."), *GetOwner()->GetName(), *ProductClass->GetName(), QueueIndex);

	// Notify listeners.
	OnProductionStarted.Broadcast(ProductClass, QueueIndex, ProductionTime);
}
