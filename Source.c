#include <Windows.h>
#include <stdio.h>


int main(void) {
	HKEY hKeyServices = NULL;
	LONG lRegResult = 0;
	DWORD dwIndex = 0;
	WCHAR szSubKeyName[256];
	DWORD dwSubKeyNameSize;

	lRegResult = RegOpenKeyExW(
		HKEY_LOCAL_MACHINE, L"SYSTEM\\CurrentControlSet\\Services", 0, KEY_READ, &hKeyServices
	);

	if (lRegResult != ERROR_SUCCESS) {
		wprintf(L"[Eroare] Nu s-a putut deschide cheia Services! Cod eroare: %ld\n", lRegResult);
		return 1;
	}

	while (1) {
		dwSubKeyNameSize = 256;

		lRegResult = RegEnumKeyExW(
			hKeyServices,
			dwIndex,
			szSubKeyName,
			&dwSubKeyNameSize,
			NULL,
			NULL,
			NULL,
			NULL
			);

		if (lRegResult == ERROR_NO_MORE_ITEMS) {
			break;
		}
		else if (lRegResult != ERROR_SUCCESS) {
			wprintf(L"[Eroare] RegEnumKeyExW a esuat la indexul %lu (cod: %ld)\n", dwIndex, lRegResult);
			break;
		}

		{
			HKEY hKeyCurrentService = NULL;
			lRegResult = RegOpenKeyExW(
				hKeyServices,
				szSubKeyName,
				0,
				KEY_READ,
				&hKeyCurrentService
			);
			if (lRegResult == ERROR_SUCCESS) {
				WCHAR szValue[1024];
				DWORD dwValueSize = sizeof(szValue);
				DWORD dwType = 0;
				ZeroMemory(szValue, sizeof(szValue));

				lRegResult = RegQueryValueExW(
					hKeyCurrentService,
					L"ImagePath",
					NULL,
					&dwType,
					(LPBYTE)szValue,
					&dwValueSize
				);

				if (lRegResult == ERROR_SUCCESS && (dwType == REG_SZ || dwType == REG_EXPAND_SZ)) {
					wprintf(L"Subcheie: %ls\n", szSubKeyName);
					wprintf(L"ImagePath: %ls\n\n", szValue);
				}
				RegCloseKey(hKeyCurrentService);
			}
			else {
				wprintf(L"[Eroare] Nu s-a putut deschide subcheia %ls. Cod eroare: %ld\n", szSubKeyName, lRegResult);
			}
		}
		dwIndex++;
	}
	RegCloseKey(hKeyServices);

	return 0;
}