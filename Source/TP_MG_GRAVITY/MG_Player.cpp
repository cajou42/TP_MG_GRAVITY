﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "MG_Player.h"


// Sets default values
AMG_Player::AMG_Player()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AMG_Player::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMG_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AMG_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

