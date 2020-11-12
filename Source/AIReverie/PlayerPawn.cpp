// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPawn.h"
#include "Kismet/GameplayStatics.h"
// Sets default values
APlayerPawn::APlayerPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

bool APlayerPawn::CanIMoveForward()
{
	FVector Start = GetActorLocation();
	// This should be 15 units directly ahead of me
	FVector End = Start + (GetActorForwardVector() * 15.0);		
	FHitResult OutHit;
	FCollisionQueryParams CollisionParams;
	// Had to do this to collide with rocks, because they have no simple collision
	CollisionParams.bTraceComplex = true;
	// We need a negation because we can move when we DO NOT collide
	return !(GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECollisionChannel::ECC_WorldStatic, CollisionParams));	
}

void APlayerPawn::RandomRotate()
{
	FRotator PawnRotation = GetActorRotation();
	float DegreesToRotate = FMath::RandRange(5.0f, 25.0f);
	// Need to randomly negate for both directions.  I would just do a range of -25 to 25, but the instructions say it can't be under 5
	if (FMath::RandBool())
	{
		DegreesToRotate *= -1;
	}
	PawnRotation.Yaw += DegreesToRotate;
	SetActorRotation(PawnRotation);
	// Pretty sure there's a more elegant way to link the controller's facing to the pawn, but this was a really simple fix so I did it
	GetController()->SetControlRotation(PawnRotation);
}

void APlayerPawn::SaveAllActorsInView(TArray<AActor*> ActorsFound, int WhichScreenshot)
{	

	FString SaveOutString;
	FString FileLocation;
	bool IsFirst = true;	
	// The Comparisons to build this list were simpler to deal with in Blueprints
	for (AActor* ThisActor : ActorsFound)
	{
		// Trying not to have a trailing comma
		if (!IsFirst)
		{
			SaveOutString.Append(", ");
		}		

		SaveOutString.Append(ThisActor->GetName());	
		IsFirst = false;
	}
	// Figuring out where this was stored took entirely too long
	FileLocation = GetDefault<UEngine>()->GameScreenshotSaveDirectory.Path;
	FileLocation.Append("/image_");
	FileLocation.AppendInt(WhichScreenshot);
	FileLocation.Append("_actors.txt");
	// Easiest way I found to do this - defaults to overwriting if the file doesn't exist, creating if it does
	FFileHelper::SaveStringToFile(SaveOutString, *FileLocation);	
}