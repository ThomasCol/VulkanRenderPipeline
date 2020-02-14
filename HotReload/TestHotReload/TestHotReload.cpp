// TestHotReload.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//
#define _CRT_SECURE_NO_WARNINGS
#include "../DLL/MyTestDLL.h"
#include <iostream>
#include <cassert>
#include <filesystem>
#include <Windows.h>
#include "LivePP/API/LPP_API.h"

int main()
{
	HMODULE DllHandle = LoadLibraryA("DLL.dll");
	HMODULE livePP = LoadLibraryA("LivePP/x64/LPP_x64.dll");
	lpp::lppStartup(livePP);

	lpp::lppRegisterProcessGroup(livePP, "AGroupName");
	lpp::lppEnableAllModulesSync(livePP, L"DLL.dll");

	IDLL* dll = nullptr;
	if (DllHandle != 0)
	{
		IDLL* (*idll)(void) = (IDLL * (*)(void))GetProcAddress(DllHandle, "CreateDLL");
		dll = idll();
	}

	// Main loop
	bool Run = true;
	while (Run)
	{
		lpp::lppSyncPoint(livePP);
		if (GetKeyState(VK_ESCAPE) & 0x8000)
			Run = false;
		if (dll)
			dll->DoStuff();
	}
	lpp::lppDisableAllModulesSync(livePP, L"DLL.dll");
	lpp::lppShutdown(livePP);
	FreeLibrary(livePP);
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
