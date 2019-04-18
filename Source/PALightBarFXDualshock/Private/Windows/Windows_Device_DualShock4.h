//-------------------------------------------------------------------------------------------------------------------
// Created By PLASTIC ANT SOFTWARE, LTD
// Copyright © 2017. All Rights Reserved.
//-------------------------------------------------------------------------------------------------------------------

#pragma once
#ifndef PA_PLATFORM_WINDOWS_DEVICE_DUALSHOCK4_H
#define PA_PLATFORM_WINDOWS_DEVICE_DUALSHOCK4_H

#include <array>
#include <vector>
#include <iso646.h>
#include <Dbt.h>
#include "Windows_Devices.h"

namespace PA
{
	//-------------------------------------------------------------------------------------------------------------------
	//
	//-------------------------------------------------------------------------------------------------------------------
	class PALIGHTBARFXDUALSHOCK_API Windows_Device_Dualshock4 : public WindowsDevice
	{
	public:

		struct State
		{
			template <typename T>
			class ButtonFlags
			{
				T Flags, ReleasedFlags, PressedFlags;

			public:

				using flag_type = T;

				T GetFlags() const { return Flags; }
				T GetPressedFlags() const { return PressedFlags; }
				T GetReleasedFlags() const { return ReleasedFlags; }

				void SetFlags(const T value)
				{
					const T previous = Flags;
					Flags = value;
					PressedFlags = (value & ~previous);
					ReleasedFlags = (~value & previous);
				}
			};

			static constexpr float kDeadZone = 0.2f;

			static const uint8_t kDPad_Up = 1 << 0;
			static const uint8_t kDPad_Right = 1 << 1;
			static const uint8_t kDPad_Down = 1 << 2;
			static const uint8_t kDPad_Left = 1 << 3;

			static const uint16_t kButton_Left = 0x0010;
			static const uint16_t kButton_Bottom = 0x0020;
			static const uint16_t kButton_Right = 0x0040;
			static const uint16_t kButton_Top = 0x0080;
			static const uint16_t kButton_LeftShoulder = 0x100;
			static const uint16_t kButton_RightShoulder = 0x200;
			static const uint16_t kButton_Share = 0x1000;
			static const uint16_t kButton_Option = 0x2000;
			static const uint16_t kButton_LeftStick = 0x4000;
			static const uint16_t kButton_RightStick = 0x8000;

			// Light
			std::array<uint8_t, 3> LightColour;

			// Digital buttons
			ButtonFlags<uint8_t> DPadFlags;
			ButtonFlags<uint16_t> FaceButtonFlags;

			// Triggers
			float LeftTrigger, RightTrigger;

			// Analog sticks
			float StickLeftX, StickLeftY;
			float StickRightX, StickRightY;

			// Gyroscope
			int16_t GyroPitch;
			int16_t GyroYaw;
			int16_t GyroRoll;

			// Accelerometer
			int16_t AccelerometerPitch;
			int16_t AccelerometerYaw;
			int16_t AccelerometerRoll;

			// Touchpad
			uint16_t TouchX[2];
			uint16_t TouchY[2];
			uint16_t TouchTrackId[2];
			uint8_t	 TouchCount;
		};

	private:

		std::array<uint8_t, 128> Buffer;
		State CurrentState = {};

	public:

		using Dualshock4Ptr = std::unique_ptr<Windows_Device_Dualshock4>;
		using Ids = Devices::Device::Ids<1356, 1476, 2508>;

		const bool Wireless;
		const uint32_t DataOffset;
		const uint32_t PacketSize;

		Windows_Device_Dualshock4(bool wireless=false) : Wireless(wireless), DataOffset(wireless ? 2 : 0), PacketSize(wireless ? 78 : 64) {}

		void ProcessRawData(HRAWINPUT* inputData);

		void SetLightBarColour(const uint8_t red, const uint8_t green, const uint8_t blue);
		static void SetLightBarColour(const uint32_t controllerIndex, const uint8_t red, const uint8_t green, const uint8_t blue);

		const State& GetCurrentState() const { return CurrentState; }

		static Windows_Device_Dualshock4* GetByDevice(const HANDLE device);
		static Windows_Device_Dualshock4* GetByIndex(const uint32_t index);

		static std::vector<Dualshock4Ptr> ConnectedControllers;
	};

	namespace Devices
	{
		template <>
		std::error_code PollConnection<Windows_Device_Dualshock4>();

		template <>
		std::error_code ProcessRawInput<Windows_Device_Dualshock4>(HRAWINPUT* inputData);

		template <>
		bool MakeConnection<Windows_Device_Dualshock4>(const HANDLE deviceHandle, const RID_DEVICE_INFO& deviceInfo);

		template <>
		bool IsDevice<Windows_Device_Dualshock4>(const uint64_t vendorId, const uint64_t productId);
	}

} // PA

#endif