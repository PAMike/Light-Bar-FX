//-------------------------------------------------------------------------------------------------------------------
// Created By PLASTIC ANT SOFTWARE, LTD
// Copyright © 2017. All Rights Reserved.
//-------------------------------------------------------------------------------------------------------------------

#pragma once
#ifndef PA_PLATFORM_WINDOWS_DEVICES_H
#define PA_PLATFORM_WINDOWS_DEVICES_H

#include "Windows/WindowsApplication.h"
#include "Devices.h"
#include "hidsdi.h"

#pragma comment(lib, "Setupapi.lib" )
#pragma comment(lib, "Hid.lib" )

namespace PA
{
	struct PALIGHTBARFXDUALSHOCK_API WindowsDevice : public Devices::Device
	{
		HANDLE DeviceHandle = nullptr;
		HANDLE HID = nullptr;

		~WindowsDevice()
		{
			if (HID)
			{
				CloseHandle(HID);
				HID = nullptr;
			}
		}
	};

} // PA

#endif
