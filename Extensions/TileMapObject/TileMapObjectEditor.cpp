/**

Game Develop - Tiled Sprite Extension
Copyright (c) 2012 Victor Levasseur (victorlevasseur01@orange.fr)
Copyright (c) 2014 Florian Rival (Florian.Rival@gmail.com)

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
    claim that you wrote the original software. If you use this software
    in a product, an acknowledgment in the product documentation would be
    appreciated but is not required.

    2. Altered source versions must be plainly marked as such, and must not be
    misrepresented as being the original software.

    3. This notice may not be removed or altered from any source
    distribution.

*/

#if defined(GD_IDE_ONLY) && !defined(GD_NO_WX_GUI)

#include <wx/dcclient.h>
#include <wx/dcmemory.h>
#include <wx/dcbuffer.h>

#include "GDCore/Tools/Log.h"
#include "GDCore/Tools/Localization.h"
#include "GDCore/IDE/Dialogs/MainFrameWrapper.h"
#include "GDCore/IDE/SkinHelper.h"
#include "GDCpp/Project.h"
#include "GDCpp/CommonTools.h"
#include "GDCore/IDE/Dialogs/ResourcesEditor.h"
#include "GDCore/IDE/CommonBitmapManager.h"
#include "TileMapObjectEditor.h"
#include "TileMapObject.h"

#include <iostream>

TileMapObjectEditor::TileMapObjectEditor( wxWindow* parent, gd::Project & game_, TileMapObject & object_, gd::MainFrameWrapper & mainFrameWrapper_ ) :
TileMapObjectEditorBase(parent),
tileSetBitmap(NULL),
game(game_),
mainFrameWrapper(mainFrameWrapper_),
object(object_)
{
	
}

TileMapObjectEditor::~TileMapObjectEditor()
{
	if(tileSetBitmap)
		delete tileSetBitmap;
}

void TileMapObjectEditor::OnTileSetPanelErase(wxEraseEvent& event)
{

}

void TileMapObjectEditor::OnTileSetPanelPaint(wxPaintEvent& event)
{
	m_tileSetPanel->SetBackgroundStyle( wxBG_STYLE_PAINT );
    wxBufferedPaintDC dc( m_tileSetPanel );

    if(tileSetBitmap != NULL)
    {
    	dc.DrawBitmap(*tileSetBitmap, 0, 0);
    }
}

void TileMapObjectEditor::OnUpdateClicked(wxCommandEvent& event)
{
	std::string imageName = ToString(m_imageNameTextCtrl->GetValue());
	SetTileSet(imageName);

	m_tileSetPanel->SetTileCount(m_tileColumnsSpin->GetValue(), m_tileRowsSpin->GetValue());
	m_tileSetPanel->SetTileSize(wxSize(m_tileWidthSpin->GetValue(), m_tileHeightSpin->GetValue()));
	m_tileSetPanel->SetTileMargins(wxSize(m_horizontalMarginsSpin->GetValue(), m_verticalMarginsSpin->GetValue()));

	m_tileMapPanel->SetTileSetCount(m_tileColumnsSpin->GetValue(), m_tileRowsSpin->GetValue());
	m_tileMapPanel->SetTileSetSize(wxSize(m_tileWidthSpin->GetValue(), m_tileHeightSpin->GetValue()));
	m_tileMapPanel->SetTileSetMargins(wxSize(m_horizontalMarginsSpin->GetValue(), m_verticalMarginsSpin->GetValue()));

	m_tileSetPanel->Update();
	m_tileMapPanel->Update();
}

void TileMapObjectEditor::OnMapUpdateButtonClicked(wxCommandEvent& event)
{
	m_tileMapPanel->SetMapSize(m_mapWidthSpin->GetValue(), m_mapHeightSpin->GetValue());
}

void TileMapObjectEditor::SetTileSet(const std::string &tileSetName)
{
	if(tileSetBitmap != NULL)
	{
		delete tileSetBitmap;
		tileSetBitmap = NULL;
	}

	if(game.GetResourcesManager().HasResource(tileSetName))
	{
		tileSetBitmap = new wxBitmap(GetwxBitmapFromImageResource(game.GetResourcesManager().GetResource(tileSetName)));
		m_tileSetPanel->SetTileSet(tileSetBitmap);
		m_tileMapPanel->SetTileSet(tileSetBitmap);
	}
	else
	{
		tileSetBitmap = NULL;
	}
}

wxBitmap TileMapObjectEditor::GetwxBitmapFromImageResource(gd::Resource & resource)
{
    try
    {
        gd::ImageResource & image = dynamic_cast<gd::ImageResource&>(resource);

        if ( wxFileExists(image.GetAbsoluteFile(game)) )
        {
            wxBitmap bmp( image.GetAbsoluteFile(game), wxBITMAP_TYPE_ANY);
            return bmp;
        }
    }
    catch(...)
    {
        //Resource is probably not an image.
    }

    return gd::CommonBitmapManager::Get()->error48;
}

#endif

