//-------------------------------------------------------------------------------------------------------------------
// Created By PLASTIC ANT SOFTWARE, LTD
// Copyright 2018. All Rights Reserved.
//-------------------------------------------------------------------------------------------------------------------

#include "PALightBarFXDualshockModule.h"
#include "Modules/ModuleManager.h"
#include "Windows/WindowsApplication.h"
#include "Framework/Application/SlateApplication.h"
#include "Windows/Windows_Device_DualShock4.h"
#include <array>

struct FDLLPointers
{
public:

	typedef bool(*HidD_SetOutputReportType)(HANDLE device, PVOID buffer, uint32 buffer_len);
	typedef bool(*HidD_GetSerialNumberStringType)(HANDLE device, PVOID buffer, uint32 buffer_len);
	typedef bool(*HidD_GetManufacturerStringType)(HANDLE handle, PVOID buffer, uint32 buffer_len);
	typedef bool(*HidD_GetProductStringType)(HANDLE handle, PVOID buffer, uint32 buffer_len);
	typedef int32(*HidP_GetValueCapsType)(HIDP_REPORT_TYPE ReportType, PHIDP_VALUE_CAPS ValueCaps, uint16* ValueCapsLength, PHIDP_PREPARSED_DATA PreparsedData);
	typedef int32(*HidP_GetButtonCapsType)(HIDP_REPORT_TYPE ReportType, PHIDP_BUTTON_CAPS ButtonCaps, uint16* ButtonCapsLength, PHIDP_PREPARSED_DATA PreparsedData);
	typedef int32(*HidP_GetCapsType)(PHIDP_PREPARSED_DATA PreparsedData, PHIDP_CAPS Capabilities);
	typedef int32(*HidP_GetUsagesType)(HIDP_REPORT_TYPE ReportType, uint16 UsagePage, uint16 LinkCollection, uint16* UsageList, uint32* UsageLength, PHIDP_PREPARSED_DATA PreparsedData, PCHAR Report, uint32 ReportLength);
	typedef int32(*HidP_GetUsageValueType)(HIDP_REPORT_TYPE ReportType, uint16 UsagePage, uint16 LinkCollection, uint16 Usage, uint32* UsageValue, PHIDP_PREPARSED_DATA PreparsedData, PCHAR Report, uint32 ReportLength);

	FDLLPointers()
		: HIDDLLHandle(nullptr)
		, HidD_SetOutputReport(nullptr)
		, HidD_GetSerialNumberString(nullptr)
		, HidD_GetManufacturerString(nullptr)
		, HidD_GetProductString(nullptr)
		, HidP_GetButtonCaps(nullptr)
		, HidP_GetValueCaps(nullptr)
		, HidP_GetCaps(nullptr)
		, HidP_GetUsages(nullptr)
		, HidP_GetUsageValue(nullptr)
	{
	}

	virtual ~FDLLPointers()
	{
		if (HIDDLLHandle)
		{
			FPlatformProcess::FreeDllHandle(HIDDLLHandle);
			HIDDLLHandle = nullptr;
		}
	}

	bool InitFuncPointers()
	{
		HIDDLLHandle = FPlatformProcess::GetDllHandle(TEXT("HID.dll"));
		if (HIDDLLHandle)
		{
			HidD_SetOutputReport = (FDLLPointers::HidD_SetOutputReportType)FPlatformProcess::GetDllExport(HIDDLLHandle, TEXT("HidD_SetOutputReport"));
			check(HidD_SetOutputReport);

			HidP_GetCaps = (FDLLPointers::HidP_GetCapsType)FPlatformProcess::GetDllExport(HIDDLLHandle, TEXT("HidP_GetCaps"));
			check(HidP_GetCaps);
			HidD_GetSerialNumberString = (FDLLPointers::HidD_GetSerialNumberStringType)FPlatformProcess::GetDllExport(HIDDLLHandle, TEXT("HidD_GetSerialNumberString"));
			check(HidD_GetSerialNumberString);
			HidD_GetManufacturerString = (FDLLPointers::HidD_GetManufacturerStringType)FPlatformProcess::GetDllExport(HIDDLLHandle, TEXT("HidD_GetManufacturerString"));
			check(HidD_GetManufacturerString);
			HidD_GetProductString = (FDLLPointers::HidD_GetProductStringType)FPlatformProcess::GetDllExport(HIDDLLHandle, TEXT("HidD_GetProductString"));
			check(HidD_GetProductString);

			HidP_GetButtonCaps = (FDLLPointers::HidP_GetButtonCapsType)FPlatformProcess::GetDllExport(HIDDLLHandle, TEXT("HidP_GetButtonCaps"));
			check(HidP_GetButtonCaps);
			HidP_GetValueCaps = (FDLLPointers::HidP_GetValueCapsType)FPlatformProcess::GetDllExport(HIDDLLHandle, TEXT("HidP_GetValueCaps"));
			check(HidP_GetValueCaps);
			HidP_GetUsages = (FDLLPointers::HidP_GetUsagesType)FPlatformProcess::GetDllExport(HIDDLLHandle, TEXT("HidP_GetUsages"));
			check(HidP_GetUsages);
			HidP_GetUsageValue = (FDLLPointers::HidP_GetUsageValueType)FPlatformProcess::GetDllExport(HIDDLLHandle, TEXT("HidP_GetUsageValue"));
			check(HidP_GetUsageValue);
		}

		return (HIDDLLHandle != nullptr);
	}

	void* HIDDLLHandle;

	HidD_SetOutputReportType HidD_SetOutputReport;
	HidD_GetSerialNumberStringType HidD_GetSerialNumberString;
	HidD_GetManufacturerStringType HidD_GetManufacturerString;
	HidD_GetProductStringType HidD_GetProductString;
	HidP_GetButtonCapsType HidP_GetButtonCaps;
	HidP_GetValueCapsType HidP_GetValueCaps;
	HidP_GetCapsType HidP_GetCaps;
	HidP_GetUsagesType HidP_GetUsages;
	HidP_GetUsageValueType HidP_GetUsageValue;
};

//-------------------------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------------------------
class PALIGHTBARFXDUALSHOCK_API FPALightBarFXDualshockDevice : public IInputDevice, IWindowsMessageHandler
{
	template <typename T>
	inline void PressedAndReleased(const T& buttonFlags, const typename T::flag_type flag, const int32_t controllerId, const FName& name) const
	{
		if (buttonFlags.GetPressedFlags() & flag)
		{
			MessageHandler->OnControllerButtonPressed(name, controllerId, false);
			UE_LOG(LogTemp, Warning, L"PRESSED: %s", *name.ToString());
		}
		else if (buttonFlags.GetReleasedFlags() & flag)
		{
			MessageHandler->OnControllerButtonReleased(name, controllerId, false);
			UE_LOG(LogTemp, Warning, L"RELEASED: %s", *name.ToString());
		}
	}

	bool EventsSent = false;

	TSharedRef<FGenericApplicationMessageHandler> MessageHandler;
	FDLLPointers DLLPointers;

public:

	FPALightBarFXDualshockDevice(const TSharedRef< FGenericApplicationMessageHandler >& InMessageHandler) : MessageHandler(InMessageHandler)
	{
		HWND hWnd = nullptr;

		DLLPointers.InitFuncPointers();

		FWindowsApplication* WindowsApplication = (FWindowsApplication*)FSlateApplication::Get().GetPlatformApplication().Get();
		check(WindowsApplication);
		WindowsApplication->AddMessageHandler(*this);

		const TSharedPtr<SWindow> ParentWindow = FSlateApplication::Get().GetActiveTopLevelWindow();
		if (ParentWindow.IsValid() && (ParentWindow->GetNativeWindow().IsValid()))
		{
			hWnd = (HWND)ParentWindow->GetNativeWindow()->GetOSWindowHandle();
		}

		RAWINPUTDEVICE RawInputDevice;
		RawInputDevice.usUsagePage = 0x01;
		RawInputDevice.usUsage = 0x05;
		RawInputDevice.dwFlags = RIDEV_DEVNOTIFY;
		RawInputDevice.hwndTarget = hWnd;
		RegisterRawInputDevices(&RawInputDevice, 1, sizeof(RAWINPUTDEVICE));
	}

	void SetMessageHandler(const TSharedRef< FGenericApplicationMessageHandler >& InMessageHandler) override { MessageHandler = InMessageHandler; }
	
	void Tick(float DeltaTime) override;

	void SendControllerEvents() override;
	void SetChannelValue(int32 ControllerId, FForceFeedbackChannelType ChannelType, float Value) override {}
	void SetChannelValues(int32 ControllerId, const FForceFeedbackValues& Values) override {}
	bool Exec(UWorld* InWorld, const TCHAR* Cmd, FOutputDevice& Ar) override { return false; }

	bool ProcessMessage(const HWND hwnd, const uint32 Msg, const WPARAM wParam, const LPARAM lParam, int32& OutResult) override;

	virtual ~FPALightBarFXDualshockDevice() {}
};

//-------------------------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------------------------
void FPALightBarFXDualshockDevice::Tick(float DeltaTime)
{
	PA::Devices::PollConnection<PA::Windows_Device_Dualshock4>();
}

//-------------------------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------------------------
void FPALightBarFXDualshockDevice::SendControllerEvents()
{
	EventsSent = true;

	int32_t controllerId = 0;
	for (auto& dualshockPtr : PA::Windows_Device_Dualshock4::ConnectedControllers)
	{
		const PA::Windows_Device_Dualshock4* pad = dualshockPtr.get();
		const PA::Windows_Device_Dualshock4::State& currentState = pad->GetCurrentState();
		const auto& faceButtonFlags = currentState.FaceButtonFlags;
		const auto& DpadFlags = currentState.DPadFlags;

		PressedAndReleased(faceButtonFlags, PA::Windows_Device_Dualshock4::State::kButton_Bottom, controllerId, FGamepadKeyNames::FaceButtonBottom);
		PressedAndReleased(faceButtonFlags, PA::Windows_Device_Dualshock4::State::kButton_Top, controllerId, FGamepadKeyNames::FaceButtonTop);
		PressedAndReleased(faceButtonFlags, PA::Windows_Device_Dualshock4::State::kButton_Left, controllerId, FGamepadKeyNames::FaceButtonLeft);
		PressedAndReleased(faceButtonFlags, PA::Windows_Device_Dualshock4::State::kButton_Right, controllerId, FGamepadKeyNames::FaceButtonRight);
		PressedAndReleased(faceButtonFlags, PA::Windows_Device_Dualshock4::State::kButton_LeftShoulder, controllerId, FGamepadKeyNames::LeftShoulder);
		PressedAndReleased(faceButtonFlags, PA::Windows_Device_Dualshock4::State::kButton_RightShoulder, controllerId, FGamepadKeyNames::RightShoulder);

		PressedAndReleased(DpadFlags, PA::Windows_Device_Dualshock4::State::kDPad_Down, controllerId, FGamepadKeyNames::DPadDown);
		PressedAndReleased(DpadFlags, PA::Windows_Device_Dualshock4::State::kDPad_Up, controllerId, FGamepadKeyNames::DPadUp);
		PressedAndReleased(DpadFlags, PA::Windows_Device_Dualshock4::State::kDPad_Left, controllerId, FGamepadKeyNames::DPadLeft);
		PressedAndReleased(DpadFlags, PA::Windows_Device_Dualshock4::State::kDPad_Right, controllerId, FGamepadKeyNames::DPadRight);

		MessageHandler->OnControllerAnalog(FGamepadKeyNames::LeftAnalogX, controllerId, currentState.StickLeftX);
		MessageHandler->OnControllerAnalog(FGamepadKeyNames::LeftAnalogY, controllerId, -currentState.StickLeftY);

		MessageHandler->OnControllerAnalog(FGamepadKeyNames::RightAnalogX, controllerId, currentState.StickRightX);
		MessageHandler->OnControllerAnalog(FGamepadKeyNames::RightAnalogY, controllerId, -currentState.StickRightY);

		++controllerId;
	}
}

//-------------------------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------------------------
bool FPALightBarFXDualshockDevice::ProcessMessage(const HWND hwnd, const uint32 Msg, const WPARAM wParam, const LPARAM lParam, int32& OutResult)
{
	if (Msg == WM_INPUT)
	{
		if (EventsSent == true)
		{
			EventsSent = false;
			PA::Devices::ProcessRawInput<PA::Windows_Device_Dualshock4>((HRAWINPUT*)&lParam);
			return true;
		}
	}
	
	return false;
}

//-------------------------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------------------------
TSharedPtr<IInputDevice> FPALightBarFXDualshockModule::CreateInputDevice(const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler)
{
	DualShockInputDevice = MakeShareable(new FPALightBarFXDualshockDevice(InMessageHandler));
	return DualShockInputDevice;
}

//-------------------------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------------------------
void FPALightBarFXDualshockModule::StartupModule()
{
	IInputDeviceModule::StartupModule();
}

//-------------------------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------------------------
void FPALightBarFXDualshockModule::SetLightBarColour(const int32 controllerId, const FColor& colour)
{
	PA::Windows_Device_Dualshock4::SetLightBarColour(controllerId, colour.R, colour.G, colour.B);
}

IMPLEMENT_MODULE(FPALightBarFXDualshockModule, PALightBarFXDualshock)
