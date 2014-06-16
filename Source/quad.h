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



/// Quads for the screen


#ifndef __quad
#define __quad

#include "stdafx.h"

#include "globals.h"

#include <d3d9.h>




class Quad
{

	private:
		typedef struct SCREEN3DVERTEX
		{
			FLOAT x, y, z;	/// x, y, z = world coords
			D3DCOLOR c;		/// D3DCOLOR Dword
		}SCREEN3DVERTEX;

		/// x/z = world-coords
		/// vertex diffuse colour
		const static int D3DFVF_SCREEN3DVERTEX = (D3DFVF_XYZ | D3DFVF_DIFFUSE);

		D3DCOLOR colour;			/// Quad colour
		SCREEN3DVERTEX vertices[4];	/// The verticies themselves
		LPDIRECT3DVERTEXBUFFER9 vb;	/// Vertex Buffer
		void updateVerts();

	public:
		/// Constructorizors
		Quad();
		Quad(LPDIRECT3DDEVICE9 pd3dDevice, D3DCOLOR newColour = D3DCOLOR_XRGB(255,255,255));

		/// Destructorizers
		~Quad();

		/// states of the buttons
		void setColour(	D3DCOLOR newColour = D3DCOLOR_XRGB(255,255,255), bool update = true );
		void setPosition(D3DVECTOR p, int index, bool update);

		void setRenderParamaters(LPDIRECT3DDEVICE9 pd3dDevice);

		/// draw the thing
		void render(LPDIRECT3DDEVICE9 pd3dDevice);	/// D3D Device to render to

};

#endif
