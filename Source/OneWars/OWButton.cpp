// Fill out your copyright notice in the Description page of Project Settings.


#include "OWButton.h"


UOWButton::UOWButton()
{
	mPressedButtonText = TEXT("Just Pressed This Button");
}

const FString& UOWButton::PressedButton()
{
	return mPressedButtonText;
}
