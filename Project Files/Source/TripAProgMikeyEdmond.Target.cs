// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class TripAProgMikeyEdmondTarget : TargetRules
{
	public TripAProgMikeyEdmondTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;

		ExtraModuleNames.AddRange( new string[] { "TripAProgMikeyEdmond" } );
	}
}
