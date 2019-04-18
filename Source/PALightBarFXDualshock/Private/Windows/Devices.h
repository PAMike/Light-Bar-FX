//-------------------------------------------------------------------------------------------------------------------
// Created By PLASTIC ANT SOFTWARE, LTD
// Copyright © 2017. All Rights Reserved.
//-------------------------------------------------------------------------------------------------------------------

#pragma once
#ifndef PA_DEVICES_H
#define PA_DEVICES_H

#include <array>

namespace PA
{
	namespace Devices
	{
		struct PALIGHTBARFXDUALSHOCK_API Device
		{
			template <uint64_t VendorId, uint64_t... ProductIds>
			struct PALIGHTBARFXDUALSHOCK_API Ids
			{
				const uint64_t kVendorId = VendorId;
				const std::array<uint64_t, sizeof...(ProductIds)> kProductIds = { ProductIds... };
			};
		};

		template <typename T, typename U>
		std::error_code ProcessRawInput(U inputData);

		template <typename T>
		std::error_code PollConnection();

		template <typename T, typename K, typename U>
		bool MakeConnection(const K deviceHandle, const U& deviceInfo);

		template <typename T>
		bool IsDevice(const uint64_t vendorId, const uint64_t productId);
	}
}

#if defined(PLATFORM_WINDOWS)
#include "Windows_Devices.h"
#endif

#endif

