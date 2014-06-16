/*******************************************************************************

Copyright (C) 2006  Jess Telford

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

*******************************************************************************/


// SIRDs.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"

#include "globals.h"

#include "quad.h"

// our Global values
LPDIRECT3DDEVICE9 g_pD3DDevice;
GlobStruct* g_pGlob;
HMODULE g_SetupDebug;

IpV GetBackBufferPtr;
VV LockBackBuffer;
VV UnlockBackBuffer;
BI ImageExist;
OI GetImageTexture;
OV getD3DDevice;
DD GetInternalData;


// Main Entry Point for the DLL
BOOL APIENTRY DllMain(HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved){ return TRUE; }


TPCCommand void Constructor ( void ){
	// Create memory here
	g_pGlob = NULL;
	g_pD3DDevice = NULL;
	g_SetupDebug = NULL;
	GetBackBufferPtr = NULL;
	LockBackBuffer = NULL;
	UnlockBackBuffer = NULL;
	ImageExist = NULL;
	GetImageTexture = NULL;
	getD3DDevice = NULL;
	GetInternalData = NULL;

}

TPCCommand void Destructor ( void ){
	// Free memory here
	// Need to de-initialize all the items that were set up in the
	//  SIRDFuncs Init function.

	// Unload DLL
	FreeLibrary(g_SetupDebug);

	// unload the surfaces
//	psuedoZBuffer->Release();

//	delete theSird;

}



// Tells the DBP compiler to always include the Setup DLL. (Required for the plugin to function)
// NOTE: Not added to export table, undecorated form required.
TPCCommand char const * GetDependencyID(int iID){

	if (iID = 1)
		return "DBProSetupDebug.dll";
	else
		return NULL;
}


// Tell the DBP compiler how many DLLs we require.
// NOTE: Not added to export table, undecorated form required.
TPCCommand int GetNumDependencies( void ) { return 1; }


// NOTE: Not added to export table, undecorated form required.
TPCCommand void ReceiveCoreDataPtr( void *globPtr ){

	// Get the pointer to the global structure
	g_pGlob = (GlobStruct *) globPtr;


	/* --- Code Structure thanks to Cloggy's D3DFunc DLL --- */
	// Grab path to setup DLL
	char cPath[_MAX_PATH];
	strcpy(cPath, g_pGlob->pEXEUnpackDirectory);
	strcat(cPath, "\\DBProSetupDebug.dll");

	// Load in DLL
	g_SetupDebug = LoadLibrary(cPath);

	// Get device pointer
	getD3DDevice = (OV) GetProcAddress(g_SetupDebug, "?GetDirect3DDevice@@YAPAUIDirect3DDevice9@@XZ");
	g_pD3DDevice = getD3DDevice();
	/* --- End Cloggy's D3DFunc DLL Code --- */

	// Get the DBPro function pointers
	GetBackBufferPtr	=	(IpV) GetProcAddress(g_SetupDebug, "?GetBackbufferPtr@@YAKXZ");
	LockBackBuffer		=	(VV) GetProcAddress(g_SetupDebug, "?LockBackbuffer@@YAXXZ");
	UnlockBackBuffer	=	(VV) GetProcAddress(g_SetupDebug, "?UnlockBackbuffer@@YAXXZ");

	if(g_pGlob->g_Imagemade){
		ImageExist		=	(BI) GetProcAddress(g_pGlob->g_Image, "?GetExist@@YA_NH@Z");
		GetImageTexture	=	(OI) GetProcAddress(g_pGlob->g_Image, "?GetPointer@@YAPAUIDirect3DTexture9@@H@Z");
	}

	if (g_pGlob->g_Camera3Dmade){
		GetInternalData	=	(DD) GetProcAddress(g_pGlob->g_Camera3D, "?GetInternalData@@YAPAXH@Z");
	}

}
