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

#include "quad.h"

#include <d3d9.h>
#include <D3dx9tex.h>

#include <string>

Quad::Quad(){

}

Quad::Quad(LPDIRECT3DDEVICE9 pd3dDevice, D3DCOLOR newColour){

	/// 0            1
	///  .----------.
	///  |         /|
	///  |       /  |
	///  |     /    |
	///  |   /      |
	///  | /        |
	///  '----------'
	/// 2            3

	/// set our idle colour
	colour = newColour;

	/// create the vertex buffer
	HRESULT hr = pd3dDevice->CreateVertexBuffer(sizeof(vertices) * sizeof(SCREEN3DVERTEX),
		  							    0,
										D3DFVF_SCREEN3DVERTEX,
										D3DPOOL_DEFAULT,
										&vb,
										NULL );

	if(hr != D3D_OK)
		MessageBox(NULL, "Unable to Create Vertex Buffer", "ERROR", MB_OK);

	/// update all the relevant information of the quad
	D3DVECTOR zero = {0.0, 0.0, 0.0};
	setPosition(zero, 0, false);
	setPosition(zero, 1, false);
	setPosition(zero, 2, false);
	setPosition(zero, 3, false);
	setColour(newColour);

	/// update the verts
	this->updateVerts();

}

/// Destructorizers
Quad::~Quad(){

	vb->Release();

}

void Quad::setColour(D3DCOLOR newColour, bool update){

	vertices[0].c = newColour;
	vertices[1].c = newColour;
	vertices[2].c = newColour;
	vertices[3].c = newColour;

	if(update) this->updateVerts();

}


void Quad::setPosition(D3DVECTOR p, int index, bool update){

	vertices[index].x = p.x;
	vertices[index].y = p.y;
	vertices[index].z = p.z;

	if(update) this->updateVerts();

}

void Quad::updateVerts(){

	/// prepare to copy the vertices into the vertex buffer
	VOID* pVertices;

	/// lock the vertex buffer
	vb->Lock(0, sizeof(vertices), (void**)&pVertices, 0);

	/// copy the vertices into the buffer
	memcpy( pVertices, vertices, sizeof(vertices) );

	/// unlock the vertex buffer
	vb->Unlock();

}

void Quad::setRenderParamaters(LPDIRECT3DDEVICE9 pd3dDevice){

	pd3dDevice->SetStreamSource( 0, vb, 0, sizeof(SCREEN3DVERTEX) );
	pd3dDevice->SetFVF( D3DFVF_SCREEN3DVERTEX );

}

/// draw the thing
void Quad::render(LPDIRECT3DDEVICE9 pd3dDevice){

	pd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP,  0, 2 );

}
