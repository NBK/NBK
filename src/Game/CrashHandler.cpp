#include "CrashHandler.h"

namespace utils
{
	namespace CrashHandler
	{
		void CreateMiniDump( EXCEPTION_POINTERS* pep )
		{
			// Open the file
			HANDLE hFile = CreateFile("MiniDump.dmp", GENERIC_READ | GENERIC_WRITE,
				0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );

			if (hFile != NULL && hFile != INVALID_HANDLE_VALUE)
			{
				// Create the minidump
				MINIDUMP_EXCEPTION_INFORMATION mdei;

				mdei.ThreadId           = GetCurrentThreadId();
				mdei.ExceptionPointers  = pep;
				mdei.ClientPointers     = false;

				MINIDUMP_CALLBACK_INFORMATION mci;

				mci.CallbackRoutine     = (MINIDUMP_CALLBACK_ROUTINE)MyMiniDumpCallback;
				mci.CallbackParam       = 0;

				MINIDUMP_TYPE mdt       = (MINIDUMP_TYPE)(MiniDumpWithIndirectlyReferencedMemory | MiniDumpScanMemory);

				BOOL rv = MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hFile, mdt, (pep != 0) ? &mdei : 0, 0, &mci );

				if( !rv )
					printf("MiniDumpWriteDump failed. Error: %u \n", GetLastError());
				else
					printf("Minidump created.\n");

				// Close the file
				CloseHandle( hFile );

			}
			else
			{
				printf("CreateFile failed. Error: %u \n", GetLastError());
			}

		}

		bool CALLBACK MyMiniDumpCallback(PVOID pParam, const PMINIDUMP_CALLBACK_INPUT pInput, PMINIDUMP_CALLBACK_OUTPUT pOutput)
		{
			bool bRet = false;

			// Check parameters
			if( pInput == 0 )
				return false;

			if( pOutput == 0 )
				return false;

			// Process the callbacks
			switch( pInput->CallbackType )
			{
				case IncludeModuleCallback:
					// Include the module into the dump
					bRet = true;
					break;
				case IncludeThreadCallback:
					// Include the thread into the dump
					bRet = true;
					break;
				case ModuleCallback:
					// Does the module have ModuleReferencedByMemory flag set ?
					if( !(pOutput->ModuleWriteFlags & ModuleReferencedByMemory) )
					{
						// No, it does not - exclude it
						wprintf( L"Excluding module: %s \n", pInput->Module.FullPath );
						pOutput->ModuleWriteFlags &= (~ModuleWriteModule);
					}
					bRet = true;
					break;
				case ThreadCallback:
					// Include all thread information into the minidump
					bRet = true;
					break;
				case ThreadExCallback:
					// Include this information
					bRet = true;
					break;
				case MemoryCallback:
					// We do not include any information here -> return false
					bRet = false;
					break;
				case CancelCallback:
					break;
			}
			return bRet;
		}
	}
}