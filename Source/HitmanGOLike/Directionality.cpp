// Fill out your copyright notice in the Description page of Project Settings.


#include "Directionality.h"

Directionality::Directionality()
{
}

Directionality::~Directionality()
{
}

EGeneralDirectionEnum Directionality::GetOpposite(EGeneralDirectionEnum ogDirection)
{
	if (ogDirection == EGeneralDirectionEnum::UP) { return EGeneralDirectionEnum::DOWN; }
	if (ogDirection == EGeneralDirectionEnum::DOWN) { return EGeneralDirectionEnum::UP; }
	if (ogDirection == EGeneralDirectionEnum::LEFT) { return EGeneralDirectionEnum::RIGHT; }
	if (ogDirection == EGeneralDirectionEnum::RIGHT) { return EGeneralDirectionEnum::LEFT; }
	return EGeneralDirectionEnum::VALNUM;
}
