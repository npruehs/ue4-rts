#include "RTSPluginPCH.h"
#include "RTSProductionComponent.h"

#include "GameFramework/Actor.h"
#include "GameFramework/Controller.h"
#include "Engine/BlueprintGeneratedClass.h"
#include "Engine/SCS_Node.h"

#include "RTSGameMode.h"
#include "RTSProductionCostComponent.h"
#include "RTSPlayerController.h"
#include "RTSPlayerResourcesComponent.h"
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

		// Check production costs.
		auto ProductClass = GetCurrentProduction(QueueIndex);
		auto ProductionCostComponent = URTSUtilities::FindDefaultComponentByClass<URTSProductionCostComponent>(ProductClass);

		bool bProductionCostPaid = false;

		if (ProductionCostComponent && ProductionCostComponent->ProductionCostType == ERTSProductionCostType::COST_PayOverTime)
		{
			auto Owner = GetOwner()->GetOwner();

			if (!Owner)
			{
				UE_LOG(LogRTS, Error, TEXT("%s needs to pay for production, but has no owner."), *GetOwner()->GetName());
				continue;
			}

            auto PlayerResourcesComponent = Owner->FindComponentByClass<URTSPlayerResourcesComponent>();

            if (!PlayerResourcesComponent)
            {
                UE_LOG(LogRTS, Error, TEXT("%s needs to pay for production, but has no PlayerResourcesComponent."), *Owner->GetName());
                continue;
            }

			bool bCanPayAllProductionCosts = true;

			for (auto& Resource : ProductionCostComponent->Resources)
			{
				float ResourceAmount = Resource.Value * DeltaTime / ProductionCostComponent->ProductionTime;

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
				for (auto& Resource : ProductionCostComponent->Resources)
				{
					float ResourceAmount = Resource.Value * DeltaTime / ProductionCostComponent->ProductionTime;
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
	return URTSUtilities::IsReadyToUse(GetOwner()) && FindQueueForProduct(ProductClass) >= 0;
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
	// Check production state.
	if (!CanAssignProduction(ProductClass))
	{
		return;
	}

	int32 QueueIndex = FindQueueForProduct(ProductClass);

	if (QueueIndex < 0)
	{
		return;
	}

	// Check production cost.
	URTSProductionCostComponent* ProductionCostComponent =
		URTSUtilities::FindDefaultComponentByClass<URTSProductionCostComponent>(ProductClass);

	if (ProductionCostComponent && ProductionCostComponent->ProductionCostType == ERTSProductionCostType::COST_PayImmediately)
	{
		auto Owner = GetOwner()->GetOwner();

		if (!Owner)
		{
			UE_LOG(LogRTS, Error, TEXT("%s needs to pay for production, but has no owner."), *GetOwner()->GetName());
			return;
		}

        auto PlayerResourcesComponent = Owner->FindComponentByClass<URTSPlayerResourcesComponent>();

        if (!PlayerResourcesComponent)
        {
            UE_LOG(LogRTS, Error, TEXT("%s needs to pay for production, but has no PlayerResourcesComponent."), *Owner->GetName());
            return;
        }

		if (!PlayerResourcesComponent->CanPayAllResources(ProductionCostComponent->Resources))
		{
			UE_LOG(LogRTS, Error, TEXT("%s needs to pay for producing %s, but does not have enough resources."),
				*Owner->GetName(),
				*ProductClass->GetName());
			return;
		}

		// Pay production costs.
        PlayerResourcesComponent->PayAllResources(ProductionCostComponent->Resources);
	}
	
	// Insert into queue.
	FRTSProductionQueue& Queue = ProductionQueues[QueueIndex];
	Queue.Add(ProductClass);

	UE_LOG(LogRTS, Log, TEXT("%s queued %s for production in queue %i."), *GetOwner()->GetName(), *ProductClass->GetName(), QueueIndex);

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
		Cast<AController>(GetOwner()->GetOwner()),
		FTransform(FRotator::ZeroRotator, GetOwner()->GetActorLocation()));

	if (!Product)
	{
		return;
	}

	UE_LOG(LogRTS, Log, TEXT("%s finished producing %s in queue %i."), *GetOwner()->GetName(), *Product->GetName(), QueueIndex);

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

	UE_LOG(LogRTS, Log, TEXT("%s canceled producing product %i of class %s in queue %i after %f seconds."),
		*GetOwner()->GetName(),
		ProductIndex,
		*ProductClass->GetName(),
		QueueIndex,
		ElapsedProductionTime);

	// Notify listeners.
	OnProductionCanceled.Broadcast(ProductClass, QueueIndex, ElapsedProductionTime);

	// Remove product from queue.
	DequeueProduct(QueueIndex, ProductIndex);

	// Refund resources.
	URTSProductionCostComponent* ProductionCostComponent =
		URTSUtilities::FindDefaultComponentByClass<URTSProductionCostComponent>(ProductClass);

	if (ProductionCostComponent)
	{
		auto Owner = GetOwner()->GetOwner();

		if (!Owner)
		{
			return;
		}

        auto PlayerResourcesComponent = Owner->FindComponentByClass<URTSPlayerResourcesComponent>();

        if (!PlayerResourcesComponent)
        {
            return;
        }

		float TimeRefundFactor = 0.0f;

		if (ProductionCostComponent->ProductionCostType == ERTSProductionCostType::COST_PayImmediately)
		{
			TimeRefundFactor = 1.0f;
		}
		else if (ProductionCostComponent->ProductionCostType == ERTSProductionCostType::COST_PayOverTime)
		{
			TimeRefundFactor = ElapsedProductionTime / TotalProductionTime;
		}

		float ActualRefundFactor = ProductionCostComponent->RefundFactor * TimeRefundFactor;

		// Refund production costs.
		for (auto& Resource : ProductionCostComponent->Resources)
		{
			TSubclassOf<URTSResourceType> ResourceType = Resource.Key;
			float ResourceAmount = Resource.Value * ActualRefundFactor;

            PlayerResourcesComponent->AddResources(ResourceType, ResourceAmount);

			UE_LOG(LogRTS, Log, TEXT("%f %s of production costs refunded."), ResourceAmount, *ResourceType->GetName());

			// Notify listeners.
			OnProductionCostRefunded.Broadcast(ResourceType, ResourceAmount);
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

	TSubclassOf<AActor> ProductClass = Queue[0];

	// Start production.
	float ProductionTime = GetProductionTimeForProduct(ProductClass);
	Queue.RemainingProductionTime = ProductionTime;

	UE_LOG(LogRTS, Log, TEXT("%s started producing %s in queue %i."), *GetOwner()->GetName(), *ProductClass->GetName(), QueueIndex);

	// Notify listeners.
	OnProductionStarted.Broadcast(ProductClass, QueueIndex, ProductionTime);
}
