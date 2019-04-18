//-------------------------------------------------------------------------------------------------------------------
// Created By PLASTIC ANT SOFTWARE, LTD
// Copyright © 2017. All Rights Reserved.
//-------------------------------------------------------------------------------------------------------------------

#include "Windows/WindowsApplication.h"
#include "Windows_Device_DualShock4.h"

namespace PA
{
	std::vector<Windows_Device_Dualshock4::Dualshock4Ptr> Windows_Device_Dualshock4::ConnectedControllers;

	//-------------------------------------------------------------------------------------------------------------------
	//
	//-------------------------------------------------------------------------------------------------------------------
	void Windows_Device_Dualshock4::SetLightBarColour(const uint32_t controllerIndex, const uint8_t red, const uint8_t green, const uint8_t blue)
	{
		auto controller = GetByIndex(controllerIndex);
		if (controller)
		{
			controller->SetLightBarColour(red, green, blue);
		}
	}

	//-------------------------------------------------------------------------------------------------------------------
	//
	//-------------------------------------------------------------------------------------------------------------------
	void Windows_Device_Dualshock4::SetLightBarColour(const uint8_t red, const uint8_t green, const uint8_t blue)
	{
		DWORD writeCount = 0;

		Buffer[0] = 0x05;
		Buffer[DataOffset + 1] = 0xAA;
		Buffer[DataOffset + 6] = red;
		Buffer[DataOffset + 7] = green;
		Buffer[DataOffset + 8] = blue;

		CurrentState.LightColour = { red, green, blue };

		WriteFile(HID, Buffer.data(), PacketSize, &writeCount, nullptr);
	}

	//-------------------------------------------------------------------------------------------------------------------
	//
	//-------------------------------------------------------------------------------------------------------------------
	Windows_Device_Dualshock4* Windows_Device_Dualshock4::GetByDevice(const HANDLE device)
	{
		auto found = std::find_if(Windows_Device_Dualshock4::ConnectedControllers.begin(), Windows_Device_Dualshock4::ConnectedControllers.end(), [&device](const Windows_Device_Dualshock4::Dualshock4Ptr& pad)
		{
			return pad->DeviceHandle == device;
		});

		return (found != Windows_Device_Dualshock4::ConnectedControllers.end()) ? found->get() : nullptr;
	}

	//-------------------------------------------------------------------------------------------------------------------
	//
	//-------------------------------------------------------------------------------------------------------------------
	Windows_Device_Dualshock4* Windows_Device_Dualshock4::GetByIndex(const uint32_t index) 
	{ 
		return ConnectedControllers.size() > index ? ConnectedControllers[index].get() : nullptr;
	}

	//-------------------------------------------------------------------------------------------------------------------
	//
	//-------------------------------------------------------------------------------------------------------------------
	template <typename T>
	static float CalcStickValue(const T rawValue)
	{
		static const float t = 1.0f / 255.0f;
		const float retValue = float(((rawValue << 1) - 255) * t);
		return std::fabsf(retValue) < Windows_Device_Dualshock4::State::kDeadZone ? 0.0f : retValue;
	}	
		
	void Windows_Device_Dualshock4::ProcessRawData(HRAWINPUT* inputData)
	{
		static float r = 1.0f / 255.0f;

		UINT headerSize = sizeof(RAWINPUTHEADER);
		RAWINPUTHEADER rawHeader;
		GetRawInputData(*inputData, RID_HEADER, &rawHeader, &headerSize, sizeof(RAWINPUTHEADER));

		Buffer.fill(0);

		UINT bufferSize = Buffer.size();
		auto readSize = GetRawInputData((HRAWINPUT)*inputData, RID_INPUT, Buffer.data(), &bufferSize, sizeof(RAWINPUTHEADER));
		
		ensure(readSize < bufferSize);
		
		if (readSize < bufferSize)
		{
			RAWINPUT* rawInput = (RAWINPUT*)Buffer.data();
			uint8_t* rawInputData = (uint8_t*)rawInput->data.hid.bRawData;

			if (rawHeader.dwType == RIM_TYPEHID)
			{
				Windows_Device_Dualshock4* foundDualshock = Windows_Device_Dualshock4::GetByDevice(rawHeader.hDevice);
				if (foundDualshock)
				{
					Windows_Device_Dualshock4::State& currentState = foundDualshock->CurrentState;
					{
						const uint32_t dataOffset = foundDualshock->DataOffset;

						// Sticks:
						currentState.StickLeftX = CalcStickValue(rawInputData[dataOffset + 1]);
						currentState.StickLeftY = CalcStickValue(rawInputData[dataOffset + 2]);
						currentState.StickRightX = CalcStickValue(rawInputData[dataOffset + 3]);
						currentState.StickRightY = CalcStickValue(rawInputData[dataOffset + 4]);

						// Buttons and dpad:
						const uint16_t DPadAndFaceButtons = *(uint16_t*)(&rawInputData[dataOffset + 5]);
						{
							static const uint8_t dpadRemap[] =
							{
								Windows_Device_Dualshock4::State::kDPad_Up,
								Windows_Device_Dualshock4::State::kDPad_Up bitor Windows_Device_Dualshock4::State::kDPad_Right,
								Windows_Device_Dualshock4::State::kDPad_Right,
								Windows_Device_Dualshock4::State::kDPad_Down bitor Windows_Device_Dualshock4::State::kDPad_Right,
								Windows_Device_Dualshock4::State::kDPad_Down,
								Windows_Device_Dualshock4::State::kDPad_Down bitor Windows_Device_Dualshock4::State::kDPad_Left,
								Windows_Device_Dualshock4::State::kDPad_Left,
								Windows_Device_Dualshock4::State::kDPad_Up bitor Windows_Device_Dualshock4::State::kDPad_Left,
								0,
							};

							const uint8_t currentDPadFlags = dpadRemap[DPadAndFaceButtons bitand 0x000f];
							const uint16_t currentButtonFlags = DPadAndFaceButtons bitand 0xfff0;

							currentState.DPadFlags.SetFlags(currentDPadFlags);
							currentState.FaceButtonFlags.SetFlags(currentButtonFlags);
						}

						// Triggers:
						currentState.LeftTrigger = float(rawInputData[dataOffset + 8]) * r;
						currentState.RightTrigger = float(rawInputData[dataOffset + 9]) * r;

						// Gyro:
						currentState.GyroPitch = *(int16_t*)(&rawInputData[dataOffset + 23]);
						currentState.GyroYaw = *(int16_t*)(&rawInputData[dataOffset + 21]);
						currentState.GyroRoll = *(int16_t*)(&rawInputData[dataOffset + 19]);

						// Accelerometer:
						currentState.AccelerometerPitch = *(int16_t*)(&rawInputData[dataOffset + 13]);
						currentState.AccelerometerYaw = *(int16_t*)(&rawInputData[dataOffset + 15]);
						currentState.AccelerometerRoll = *(int16_t*)(&rawInputData[dataOffset + 17]);

						// Touchpad:
						currentState.TouchX[0] = rawInputData[dataOffset + 36] bitor ((rawInputData[dataOffset + 37] bitand 0x0f) << 8);
						currentState.TouchY[0] = (rawInputData[dataOffset + 38]) << 4 bitor (rawInputData[dataOffset + 37] bitand 0x0f);
						currentState.TouchX[1] = rawInputData[dataOffset + 40] bitor ((rawInputData[dataOffset + 41] bitand 0x0f) << 8);
						currentState.TouchY[1] = (rawInputData[dataOffset + 42]) << 4 bitor (rawInputData[dataOffset + 41] bitand 0x0f);
						currentState.TouchTrackId[0] = rawInputData[dataOffset + 35] bitand 0x7F;
						currentState.TouchTrackId[1] = rawInputData[dataOffset + 39] bitand 0x7F;
						currentState.TouchCount = 0;
						currentState.TouchCount += (rawInputData[dataOffset + 35] >> 7) == 0 ? 1 : 0;
						currentState.TouchCount += (rawInputData[dataOffset + 39] >> 7) == 0 ? 1 : 0;
					}
				}
			}
		}
	}

	//-------------------------------------------------------------------------------------------------------------------
	//
	//-------------------------------------------------------------------------------------------------------------------
	namespace Devices
	{
		//-------------------------------------------------------------------------------------------------------------------
		//
		//-------------------------------------------------------------------------------------------------------------------
		template <>
		std::error_code PollConnection<Windows_Device_Dualshock4>()
		{
			std::error_code retErr = std::make_error_code(std::errc::not_supported);
			
			std::vector<RAWINPUTDEVICELIST> deviceList;
			UINT deviceCount = 0;
			UINT byteSize = sizeof(RID_DEVICE_INFO);

			GetRawInputDeviceList(nullptr, &deviceCount, sizeof(RAWINPUTDEVICELIST));
			if (deviceCount == 0)
			{
				retErr = std::make_error_code(std::errc::no_such_device);
			}
			else
			{
				deviceList.resize(deviceCount);

				GetRawInputDeviceList(deviceList.data(), &deviceCount, sizeof(RAWINPUTDEVICELIST));

				Windows_Device_Dualshock4::ConnectedControllers.erase
				(
					std::remove_if(Windows_Device_Dualshock4::ConnectedControllers.begin(), Windows_Device_Dualshock4::ConnectedControllers.end(), [&deviceList](const Windows_Device_Dualshock4::Dualshock4Ptr& ptr)
					{
						for (const RAWINPUTDEVICELIST& device : deviceList)
						{
							if (ptr->DeviceHandle == device.hDevice)
							{
								return false;
							}
						}
						return true;
					}),
					Windows_Device_Dualshock4::ConnectedControllers.end()
				);

				for (const RAWINPUTDEVICELIST& device : deviceList)
				{
					RID_DEVICE_INFO deviceInfo = {};

					if (GetRawInputDeviceInfo(device.hDevice, RIDI_DEVICEINFO, &deviceInfo, &byteSize) != uint32_t(-1))
					{
						if (Devices::IsDevice<Windows_Device_Dualshock4>(uint64_t(deviceInfo.hid.dwVendorId), uint64_t(deviceInfo.hid.dwProductId)))
						{
							// TODO: Could do with some better return codes for this.
							if (Devices::MakeConnection<Windows_Device_Dualshock4>(device.hDevice, deviceInfo) == true)
							{
								retErr = {};
							}
						}
					}
				}
			}

			return retErr;
		}

		//-------------------------------------------------------------------------------------------------------------------
		//
		//-------------------------------------------------------------------------------------------------------------------
		template <>
		std::error_code ProcessRawInput<Windows_Device_Dualshock4>(HRAWINPUT* inputData)
		{	
			UINT headerSize = sizeof(RAWINPUTHEADER);
			RAWINPUTHEADER rawHeader;
			GetRawInputData(*inputData, RID_HEADER, &rawHeader, &headerSize, sizeof(RAWINPUTHEADER));

			Windows_Device_Dualshock4* foundDualshock = Windows_Device_Dualshock4::GetByDevice(rawHeader.hDevice);
			if (foundDualshock)
			{
				foundDualshock->ProcessRawData(inputData);
				return {};
			}

			return std::make_error_code(std::errc::no_such_device_or_address);
		}

		//-------------------------------------------------------------------------------------------------------------------
		//
		//-------------------------------------------------------------------------------------------------------------------
		template <>
		bool MakeConnection<Windows_Device_Dualshock4>(const HANDLE deviceHandle, const RID_DEVICE_INFO& deviceInfo)
		{
			if(Windows_Device_Dualshock4::GetByDevice(deviceHandle) != nullptr)
			{
				return true;
			}

			std::array<TCHAR, MAX_PATH> deviceName;
			UINT deviceNameLength = UINT(deviceName.size());
			GetRawInputDeviceInfo(deviceHandle, RIDI_DEVICENAME, deviceName.data(), &deviceNameLength);

			const bool wireless = (deviceName[8] == '{') ? true : false;

			std::unique_ptr<Windows_Device_Dualshock4> newDualshock4 = std::make_unique<Windows_Device_Dualshock4>(wireless);

			newDualshock4->HID = CreateFile(deviceName.data(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
			if (newDualshock4->HID)
			{
				newDualshock4->DeviceHandle = deviceHandle;
				Windows_Device_Dualshock4::ConnectedControllers.push_back(std::move(newDualshock4));
				return true;
			}

			return false;
		}

		//-------------------------------------------------------------------------------------------------------------------
		//
		//-------------------------------------------------------------------------------------------------------------------
		template <>
		bool IsDevice<Windows_Device_Dualshock4>(const uint64_t vendorId, const uint64_t productId)
		{
			const Windows_Device_Dualshock4::Ids ids;

			if (vendorId == ids.kVendorId)
			{
				if (std::find(ids.kProductIds.cbegin(), ids.kProductIds.cend(), uint64_t(productId)) != ids.kProductIds.cend())
				{
					return true;
				}
			}

			return false;
		}
	}
}