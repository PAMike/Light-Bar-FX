//-------------------------------------------------------------------------------------------------------------------
// Created By PLASTIC ANT SOFTWARE, LTD
// Copyright 2018. All Rights Reserved.
//-------------------------------------------------------------------------------------------------------------------

#include "PALightBarFXAnimationBase.h"
#include "PALightBarFXAnimationBank.h"

//-------------------------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------------------------
/*bool UPALightBarFXAnimationBase::UpdateTime(UPALightBarFXComponent* component, UPALightBarFXAnimationBank* context, const float deltaTime)
{
	CurrentTime += deltaTime;

	if (CurrentTime > DurationInSeconds)
	{
		if (LoopCounter != -1)
		{
			--CurrentLoopCounter;

			if (CurrentLoopCounter <= 0)
			{
				return false;
			}

			Restart(component, context);
		}
		else
		{
			Restart(component, context);
		}
	}

	return true;
}*/
