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



#ifndef __SIRDClass
#define __SIRDClass


#include "stdafx.h"

#include "globals.h"

#include <math.h>
#include <vector>

#include "quad.h"


class SIRD
{

	private:

		LPDIRECT3DSURFACE9 psuedoZBuffer;
		LPDIRECT3DSURFACE9 ZBufferDataCopy;

		LPDIRECT3DTEXTURE9 srcTxt;
		LPDIRECT3DTEXTURE9 outTxt;

		D3DSURFACE_DESC srcDesc;
		D3DSURFACE_DESC outDesc;
		D3DSURFACE_DESC zbDesc;
		
		D3DLOCKED_RECT zBuffLR;
		D3DLOCKED_RECT srcLR;
		D3DLOCKED_RECT outLR;

		UINT32* outPtr;
		UINT32* zbPtr;
		UINT32* srcPtr;

		D3DVECTOR rightVect;
		D3DVECTOR upVect;
		D3DXVECTOR3 camNormal;
		D3DVECTOR camPos;

		D3DXVECTOR3 newPos;
		D3DXVECTOR3 newWidth;
		D3DXVECTOR3 newHeight;

		LPDIRECT3DSURFACE9 oldTarget;		// So we can restore it after we're done

		Quad* myQuad;

		int imgWidth;
		int imgHeight;

		UINT32* srcData;
		UINT32* rndData;
		UINT32* dstData;

		float zDepth;
		float distDiff;
		float widthOn2;
		float depth;
		float FOVon2;

		UINT32 colour;

		sCamera* camData;
		int camNumber;

		UINT32* tmpSrcPtr;


	public:
		SIRD(int outImg, int srcImg, int Camera, float maxDist);

		~SIRD();

		void Update();

		void Render();

};


#endif