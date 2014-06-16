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


#include "stdafx.h"

#include "globals.h"

#include <math.h>
#include <vector>

#include "quad.h"
#include "SIRDClass.h"



SIRD* theSird = NULL;



// Note: The image that is being passed into this function MUST have
//		 been loaded with the Texture Flag set to 1 so it is pixel
//		 perfect, otherwise, the wrong size is reported!

void initSIRD(int outImg, int srcImg, int Camera, float maxDist){

	theSird = new SIRD(outImg, srcImg, Camera, maxDist);

}



void RenderSIRD( ){

	theSird->Render();

}


// Original Function by Cloggy - D3DFunc DLL
void GetCameraNormal ( int iCamera, D3DVECTOR* pOut ){

	sCamera* camData = (sCamera *) GetInternalData((DWORD)iCamera);
	if (!camData)
	{
		// Invalid camera
		*(DWORD*)g_pGlob->g_pErrorHandlerRef = (DWORD)7203;
		return;
	}

	pOut->x = camData->vecLook.x;
	pOut->y = camData->vecLook.y;
	pOut->z = camData->vecLook.z;
	D3DXVec3Normalize( (LPD3DXVECTOR3)pOut, (LPD3DXVECTOR3)pOut );

}


void UpdateSIRD( void ){

	theSird->Update();

}
