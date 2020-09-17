// Fill out your copyright notice in the Description page of Project Settings.

#include "Puzzle_Manager.h"

Puzzle_Manager::Puzzle_Manager()
{
}

Puzzle_Manager::~Puzzle_Manager()
{
}


void Puzzle_Manager::Room01_UnlockDoor()
{
	Room01_UnlockDoor_Door->ChangeLockState(false);
}