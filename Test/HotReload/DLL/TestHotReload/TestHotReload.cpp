// TestHotReload.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//
#define _CRT_SECURE_NO_WARNINGS
#include "../DLL/MyTestDLL.h"
#include <iostream>
#include <cassert>
#include <filesystem>
#include <Windows.h>

void GetExeRelativePath(char* Dest, int DestSize, const char* Filename)
{
	int ExeLength = (int)GetModuleFileNameA(0, Dest, DestSize);
	int FilenameLen = (int)strlen(Filename);

	char* BaseName = Dest;
	for (char* Scan = Dest; *Scan; ++Scan)
	{
		if (*Scan == '\\')
		{
			BaseName = Scan + 1;
		}
	}

	strncpy(BaseName, Filename, DestSize - (BaseName - Dest));
}

IDLL* ReloadAPI(HMODULE* DllHandleOut)
{
	assert(DllHandleOut != nullptr);

	if (*DllHandleOut != 0)
		FreeLibrary(*DllHandleOut);

	char path[MAX_PATH];
	GetCurrentDirectoryA(MAX_PATH, path);
	std::string str("\"C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\Common7\\IDE\\devenv.com\" ");
	str += path;
	str += "\\..\\DLL\\DLL.vcxproj /Build Debug";
	std::cout << str << std::endl;
	system(str.c_str());

	// Copy source dll to target
	char DllPath[MAX_PATH];
	char TmpDllPath[MAX_PATH];
	GetExeRelativePath(DllPath, MAX_PATH, "DLL.dll");
	GetExeRelativePath(TmpDllPath, MAX_PATH, "DLL_tmp.dll");
	CopyFileA(DllPath, TmpDllPath, FALSE);

	// Load tmp dll
	HMODULE DllHandle = LoadLibraryA(TmpDllPath);
	assert(DllHandle);
	IDLL* (*idll)(void) = (IDLL*(*)(void))GetProcAddress(DllHandle, "CreateDLL");
	assert(idll);

	*DllHandleOut = DllHandle;

	return idll();
}

int main()
{
	HMODULE DllHandle = 0;
	IDLL* dll = ReloadAPI(&DllHandle);

	// Main loop
	bool Run = true;
	while (Run)
	{
		dll->DoStuff();
		printf("Press Enter to continue (r for reload, q for exit)...\n");
		switch (getchar())
		{
		case 'r':
		{
			printf("Reloading...\n");
			dll = ReloadAPI(&DllHandle);
		}
		break;
		case 'q':
		{
			Run = false;
		}
		break;
		default:;
		}
	}
	FreeLibrary(DllHandle);

	return 0;
}

// Exécuter le programme : Ctrl+F5 ou menu Déboguer > Exécuter sans débogage
// Déboguer le programme : F5 ou menu Déboguer > Démarrer le débogage

// Astuces pour bien démarrer : 
//   1. Utilisez la fenêtre Explorateur de solutions pour ajouter des fichiers et les gérer.
//   2. Utilisez la fenêtre Team Explorer pour vous connecter au contrôle de code source.
//   3. Utilisez la fenêtre Sortie pour voir la sortie de la génération et d'autres messages.
//   4. Utilisez la fenêtre Liste d'erreurs pour voir les erreurs.
//   5. Accédez à Projet > Ajouter un nouvel élément pour créer des fichiers de code, ou à Projet > Ajouter un élément existant pour ajouter des fichiers de code existants au projet.
//   6. Pour rouvrir ce projet plus tard, accédez à Fichier > Ouvrir > Projet et sélectionnez le fichier .sln.
