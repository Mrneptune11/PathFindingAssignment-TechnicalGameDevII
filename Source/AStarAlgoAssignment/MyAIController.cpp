#include "MyAIController.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"  // Add this include to resolve incomplete type error

AMyAIController::AMyAIController()
{
	PrimaryActorTick.bCanEverTick = true; //need to be able to update every frame
	PathUpdateInterval = 1.0f; //interval
	TimeSinceLastPathUpdate = 0.0f; //time since last update
}

//once spawned in
void AMyAIController::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("This is running"));
	Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0); // Get the player, need to know player location
}

//every frame processs
void AMyAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	//track time since last path update and then call find path again once enough time has passed
	TimeSinceLastPathUpdate += DeltaSeconds;	
	if (TimeSinceLastPathUpdate >= PathUpdateInterval)
	{
		FindPath();
		TimeSinceLastPathUpdate = 0.0f; //reset timer
	}
}

//logic for the pawns to follow the player around
void AMyAIController::FindPath()
{
	if (!Player) //wait for player to exist
		return;

	//applying a* to the worlds nav mesh
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
	if (NavSystem) //nav system is not null
	{
		FVector StartLocation = GetPawn()->GetActorLocation(); //draw lines to the player
		FVector EndLocation = Player->GetActorLocation();

		//find the path using the nav mesh 
		UNavigationPath* NavPath = NavSystem->FindPathToLocationSynchronously(GetWorld(), StartLocation, EndLocation);

		//follow the nodes along the path to the player
		if (NavPath && NavPath->IsValid() && NavPath->PathPoints.Num() > 1)
		{
			// helpful to visualize the pawns direction to the player along the nodes
			for (int i = 0; i < NavPath->PathPoints.Num() - 1; ++i)
			{
				DrawDebugLine(GetWorld(), NavPath->PathPoints[i], NavPath->PathPoints[i + 1], FColor::Green, false, 1.0f, 0, 5.0f);
			}

			// Move to next point (or full path using MoveToLocation)
			MoveToLocation(NavPath->PathPoints.Last()); // Or loop through them manually
		}
	}
}
