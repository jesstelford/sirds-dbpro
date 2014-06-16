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

#include "SIRDClass.h"


SIRD::SIRD(int outImg, int srcImg, int Camera, float maxDist){


	// Store the data
	zDepth = maxDist;

	// get a pointer to the D3DTexture9 of the images
	srcTxt = GetImageTexture(srcImg);
	outTxt = GetImageTexture(outImg);

	// Fill the Description structure of the Random Dot texture & the output Texture
	srcTxt->GetLevelDesc(0, &srcDesc);
	outTxt->GetLevelDesc(0, &outDesc);

	// Get the data we're after
	imgWidth = srcDesc.Width;
	imgHeight = srcDesc.Height;

	// For whatever reasons, if I don't do this again here, it all goes to the shit...
	// Time for some VooDoo!
	g_pD3DDevice = getD3DDevice();


	// Lock the Backbuffer to get the pointer to the pixel data
	//pZBuffer->LockRect(&zBuffLR, NULL, D3DLOCK_READONLY);

	// Normally, Locking the ZBuffer would be as simple as:
	//		pZBuffer->LockRect(&zBuffLR, NULL, D3DLOCK_READONLY)
	// BUT! Since when the ZBuffer was initialized by DBP, the D3DFMT_D16_LOCKABLE was NOT
	//  specified, then it cannot be locked, so we need to use the following Hack...

	// Get the camera data.
	camNumber = Camera;
	camData = (sCamera*)GetInternalData(camNumber);
	if(!camData) return;



	// Create the Psuedo Z Buffer
	g_pD3DDevice->CreateRenderTarget(	camData->viewPort3D.Width, camData->viewPort3D.Height,
										D3DFMT_A8R8G8B8,
										D3DMULTISAMPLE_NONE,
										0,
										TRUE,
										&psuedoZBuffer,
										NULL);

	// Create the vertex buffer, and colour it accordingly
	myQuad = new Quad(g_pD3DDevice, D3DCOLOR_XRGB(0,0,0));

	
	psuedoZBuffer->GetDesc(&zbDesc);

	// For each step, we need to change the colour of the plane and re-render
	//  This will give the colour values of the height-map
	distDiff = maxDist / imgWidth;


	// Set-up some in-memory area's to do the pixel manipulation
	//  This is faster than continually interfacing with the GPU
	tmpSrcPtr = (UINT32*)malloc(imgWidth * imgHeight * 4);

	
	// Lock the source Image to read pixels from it
	switch(srcTxt->LockRect(0, &srcLR, NULL, D3DLOCK_READONLY)){

		case D3D_OK:
			break;

		case D3DERR_INVALIDCALL:
			MessageBox(NULL, "Cannot Read from Random Dot Image", "", MB_OK);
			return;
			break;

	}

	// Get its pointer
	srcPtr = (UINT32*)srcLR.pBits;

	// Copy across the random dot image
	memcpy(tmpSrcPtr, srcPtr, imgWidth * imgHeight * 4);

	// Not needed anymore, so unlock it
	srcTxt->UnlockRect(0);

	// Create the surface to get the Rendered Z-Buffer into to SIRD it
	g_pD3DDevice->CreateOffscreenPlainSurface(	zbDesc.Width,
												zbDesc.Height,
												D3DFMT_A8R8G8B8,
												D3DPOOL_SYSTEMMEM,
												&ZBufferDataCopy,
												NULL
											);
}


SIRD::~SIRD(){

	delete myQuad;

	// Since we don't need the memory area's anymore, un-allocate them
	free(tmpSrcPtr);
}


void SIRD::Update(){

	// Get the camera data.
	// We need to get the Right and Up vectors of the camera so that our
	//  planes will be properly aligned with the view-port.
	rightVect = camData->vecRight;
	upVect = camData->vecUp;
	D3DXVec3Normalize( (LPD3DXVECTOR3)&rightVect, (LPD3DXVECTOR3)&rightVect );
	D3DXVec3Normalize( (LPD3DXVECTOR3)&upVect, (LPD3DXVECTOR3)&upVect );

	// Store some other relevant Camera-based data
	GetCameraNormal ( camNumber, &camNormal );
	FOVon2 = camData->fFOV;
	camPos = camData->vecPosition;


	// Set the renderer to only paste the pixel if it is BEHIND the current depth at that pixel
	//  In this mannor, we can layer from furthest to closest to the camera and get a rought
	//  estimate which represents the height-map (ZBuffer).
	g_pD3DDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_GREATEREQUAL);
	g_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);


	// Set the new Render Target to our Psuedo-ZBuffer
	g_pD3DDevice->GetRenderTarget(0, &oldTarget);
	g_pD3DDevice->SetRenderTarget(0, psuedoZBuffer);

	// clean out the ZBuffer so it's all "Black"
	g_pD3DDevice->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0,0,0), 1.0f, 0 );

	g_pD3DDevice->BeginScene();

	myQuad->setRenderParamaters(g_pD3DDevice);

	// Now, this is the actual 'Hack' part...
	//  What we need to do is render a whole bunch of planes.
	//  Each plane is rendered and its Z-Position relative to anything
	//  else on the screen is recorded into the psuedo-ZBuffer
	// Then, from here, we can pick the pixels which represent depths.
	for(int i = 0; i < imgWidth; i++){

		// How far 'into' the screen to place the plane
		depth = (zDepth - (i * distDiff));

		// The Render width of the view frustrum - Needed for correctly sizing the
		//  planes that we're using as the psuedo-Zbuffer
		widthOn2 = depth * tan(FOVon2);

		// The different colours determine 'Depth'
		myQuad->setColour(D3DCOLOR_XRGB(0, 0, i), false);

		// Calculate positions and width/height
		newPos = (D3DXVECTOR3)camPos + ((D3DXVECTOR3)camNormal * depth);
		newWidth = (D3DXVECTOR3)rightVect * widthOn2;
		newHeight = (D3DXVECTOR3)upVect * widthOn2;

		// Properly position the plane
		myQuad->setPosition(newPos - newWidth + newHeight, 0, false);
		myQuad->setPosition(newPos + newWidth + newHeight, 1, false);
		myQuad->setPosition(newPos - newWidth - newHeight, 2, false);
		myQuad->setPosition(newPos + newWidth - newHeight, 3, true);

		// and render
		myQuad->render(g_pD3DDevice);

	}

	g_pD3DDevice->EndScene();

	// Restore the proper render target
	g_pD3DDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	g_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	g_pD3DDevice->SetRenderTarget(0, oldTarget);

}


void SIRD::Render(){

	// Get the data from the Psuedo Z-Buffer into a System Memory surface (quick copy)
	g_pD3DDevice->GetRenderTargetData(psuedoZBuffer, ZBufferDataCopy);

		// Lock the ZBuffer for reading from
	switch(ZBufferDataCopy->LockRect(&zBuffLR, NULL, D3DLOCK_READONLY)){

		case D3D_OK:
			break;

		case D3DERR_INVALIDCALL:
			MessageBox(NULL, "Could not read the SIRDS ZBuffer", "SIRDS - Error", MB_OK);
			break;

	}

	// Get a pointer to the ZBuffer
	zbPtr = (UINT32*)zBuffLR.pBits;




	// Lock the Output Image to write pixels from it
	switch(outTxt->LockRect(0, &outLR, NULL, 0)){

		case D3D_OK:
			break;

		case D3DERR_INVALIDCALL:
			MessageBox(NULL, "Cannot write to output Image", "", MB_OK);
			return;
			break;

	}

	// Get a pointer to the Output Image's pixels
	outPtr = (UINT32*)outLR.pBits;



	// Loop through and write the pixels to the BackBuffer
	for(int y = 0; y < g_pGlob->iScreenHeight; y++){

		srcData = (zbPtr + (y * (zBuffLR.Pitch / 4)));
		rndData = (tmpSrcPtr + ((y % imgHeight) * (srcLR.Pitch / 4)));
		dstData = (outPtr + (y * g_pGlob->iScreenWidth));

		for (int x = zbDesc.Width - 1; x != zbDesc.Width - 1 - imgWidth; x--){
			colour = rndData[ ( x - ARGBB32(srcData[x]) ) % imgWidth ];
            dstData[x] = colour;
		}

		for (int x = zbDesc.Width - 1 - imgWidth, int x2 = zbDesc.Width - 1; x >= 0; x--, x2--){
			colour = dstData[x2 - ARGBB32(srcData[x])];
            dstData[x] = colour;
		}

	}

	// We no longer need the PsuedoZBuffer
	// So, Unlock the Image
	ZBufferDataCopy->UnlockRect();

	// Unlock the output Image
	outTxt->UnlockRect(0);


}
