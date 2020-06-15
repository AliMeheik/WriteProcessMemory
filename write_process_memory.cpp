#include <iostream>
#include <windows.h>
#include <string>
#include <TlHelp32.h>

using std::wcin;
using std::cout;
using std::cin;
using std::wstring;
using std::endl;



DWORD Get_processID( wstring process_name ) {

    PROCESSENTRY32 process_buffer;
    DWORD processId = 0;
	HANDLE process_snapshot = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, NULL );

	if( process_snapshot != INVALID_HANDLE_VALUE ) {
		process_buffer.dwSize = sizeof( PROCESSENTRY32 );
		if( Process32First( process_snapshot, &process_buffer ) ) {
			do {
				if( !_wcsicmp( process_name.c_str(), process_buffer.szExeFile ) ) {
					processId = process_buffer.th32ProcessID;
                    break;
				}
			} while( Process32Next( process_snapshot, &process_buffer ) );
		}
        CloseHandle( process_snapshot );
	}
    return processId;
}



template<typename t_WriteType>
const BOOL Write_process_memory( const t_WriteType* new_value, LPVOID address, const HANDLE* process_h ) {
    return WriteProcessMemory( *process_h, address, new_value, sizeof( t_WriteType ), nullptr );
}

int main() {

    cout << "Enter process name: ";
    wstring process_name;
    getline( wcin, process_name );

    DWORD processID;

    processID = Get_processID( process_name );

    if( processID ) {

        HANDLE tProcess_h = OpenProcess( PROCESS_VM_WRITE | PROCESS_VM_OPERATION, FALSE, processID );
        uintptr_t address;
        cout << "Enter integer field address: ";
        cin >> std::hex >> address;
        cout << "Enter new value for the field: ";
        int newValue;
        cin >> std::dec >> newValue;
        if( Write_process_memory<int>( &newValue, ( LPVOID )address, &tProcess_h ) ) {
            cout << "Success" << endl;
        } else {
            cout << "Write failed" << endl;
        }
    }

    cin.get();
    return 0;
}
