// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//code written by Joey Bertrand

//blueprint enum for enemy weapon equipment
UENUM(BlueprintType)
enum class EEquipstate: uint8
{
    ECS_Unequipped UMETA (DisplayName = "Unequipped"),
    ECS_EquippedKnife UMETA (DisplayName = "Equipped Knife"),
    ECS_EquippedAxe UMETA (DisplayName = " Equipped Axe")
};

//enum for enemy weapon mechanic options
UENUM(BlueprintType)
enum class EActionstate: uint8
{
    EAS_Unoccupied UMETA (DisplayName = "Unoccupied"),
    EAS_Attacking UMETA (DIsplayName = "Attack"),
    EAS_EquipMeleeWeapon UMETA (DisplayName = "Equip Melee Weapon")
};

//enum for death animations
UENUM(BlueprintType)
enum class EDeathPose: uint8
{
    EDP_Alive UMETA (DisplayName = "Alive"),
    EDP_Death1 UMETA (DisplayName = "Death1"),
    //EDP_Death2 UMETA (DisplayName = "Death2"),
    //EDP_Death3 UMETA (DIsplayName = "Death3")
    
};

//enum for 3 character stages
UENUM(BlueprintType)
enum class EEnemyState: uint8
{
    EES_Attacking UMETA (DisplayName = "Attacking")
};

