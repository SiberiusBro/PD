#include <windows.h>
#include <setupapi.h>
#include <initguid.h>
#include <devguid.h>
#include <cfgmgr32.h>
#include <iostream>

#pragma comment(lib, "setupapi.lib")

int main() {
    // Ia USB
    HDEVINFO deviceInfo = SetupDiGetClassDevs(
        &GUID_DEVCLASS_USB, // clasa USB
        NULL,
        NULL,
        DIGCF_PRESENT | DIGCF_PROFILE
    );

    if (deviceInfo == INVALID_HANDLE_VALUE) {
        std::cerr << "Eroare la obtinerea dispozitivelor USB.\n";
        return 1;
    }

    SP_DEVINFO_DATA devInfoData;
    devInfoData.cbSize = sizeof(SP_DEVINFO_DATA);

    int deviceIndex = 0;

    while (SetupDiEnumDeviceInfo(deviceInfo, deviceIndex, &devInfoData)) {
        DWORD dataType, dataSize = 0;
        TCHAR buffer[1024];

        if (SetupDiGetDeviceRegistryProperty(
            deviceInfo,
            &devInfoData,
            SPDRP_DEVICEDESC,
            &dataType,
            (PBYTE)buffer,
            sizeof(buffer),
            &dataSize
        )) {
            std::wcout << L"Dispozitiv USB [" << deviceIndex << L"]: " << buffer << std::endl;
        }

        deviceIndex++;
    }

    SetupDiDestroyDeviceInfoList(deviceInfo);

    return 0;
}
