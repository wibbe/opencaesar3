// This file is part of openCaesar3.
//
// openCaesar3 is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// openCaesar3 is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with openCaesar3.  If not, see <http://www.gnu.org/licenses/>.
//
// Copyright 2012-2013 Gregoire Athanase, gathanase@gmail.com


#include "oc3_screen_menu.hpp"

#include "oc3_loadmapwindow.hpp"
#include "oc3_gfx_engine.hpp"
#include "oc3_exception.hpp"
#include "oc3_pic_loader.hpp"

#include "oc3_startmenu.hpp"
#include "oc3_guienv.hpp"
#include "oc3_pushbutton.hpp"

class ScreenMenu::Impl
{
public:
  Picture* bgPicture;
  StartMenu* menu;         // menu to display
  GfxEngine* engine;
  GuiEnv* gui;
  int result;
  bool isStopped;
  std::string fileMap;

    void resolveNewGame() { result=startNewGame; isStopped=true; }
    void resolveLoadGame( std::string fileName ) 
    {
      result=loadSavedGame; 
      fileMap = fileName;
      isStopped=true; 
    }
    
    void resolveQuitGame() { result=closeApplication; isStopped=true; }
    
    void resolveSelectFile( std::string fileName )
    {
      result = loadMap;
      fileMap = fileName;
      isStopped = true;
    }

  void resolveShowLoadMapWnd();
    void resolveShowLoadGameWnd();
};

void ScreenMenu::Impl::resolveShowLoadGameWnd()
{
  Size rootSize = gui->getRootWidget()->getSize();
  LoadMapWindow* wnd = new LoadMapWindow( gui->getRootWidget(), 
                                          Rect( 0.25f * rootSize.getWidth(), 0.25f * rootSize.getHeight(), 
                                          0.75f * rootSize.getWidth(), 0.75f * rootSize.getHeight() ), 
                                          "./", ".oc3save",
                                          -1 );

  CONNECT( wnd, onSelectFile(), this, Impl::resolveSelectFile );
  wnd->setTitle( "##Load save##" );
}

void ScreenMenu::Impl::resolveShowLoadMapWnd()
{
  Size rootSize = gui->getRootWidget()->getSize();
  LoadMapWindow* wnd = new LoadMapWindow( gui->getRootWidget(), 
                                          Rect( 0.25f * rootSize.getWidth(), 0.25f * rootSize.getHeight(), 
                                                0.75f * rootSize.getWidth(), 0.75f * rootSize.getHeight() ), 
                                                "./resources/maps/", ".map",
                                                -1 );

  CONNECT( wnd, onSelectFile(), this, Impl::resolveSelectFile );
  wnd->setTitle( "##Load map##" );
}

ScreenMenu::ScreenMenu() : _d( new Impl )
{
  _d->bgPicture = NULL;
  _d->isStopped = false;
}

ScreenMenu::~ScreenMenu() {}

void ScreenMenu::draw()
{
  _d->gui->beforeDraw();

  _d->engine->drawPicture(*_d->bgPicture, 0, 0);
  _d->gui->draw();
}

void ScreenMenu::handleEvent( NEvent& event )
{
  _d->gui->handleEvent( event );
}

void ScreenMenu::initialize( GfxEngine& engine, GuiEnv& gui )
{
  _d->bgPicture = &Picture::load("title", 1);

  // center the bgPicture on the screen
  _d->bgPicture->set_offset( (engine.getScreenWidth() - _d->bgPicture->get_width()) / 2,
                             -( engine.getScreenHeight() - _d->bgPicture->get_height() ) / 2 );

  _d->gui = &gui;
  _d->gui->clear();
  
  _d->engine = &engine;
  _d->menu = new StartMenu( gui.getRootWidget() );

  PushButton* btn = _d->menu->addButton( "New game", -1 );
  CONNECT( btn, onClicked(), _d.data(), Impl::resolveNewGame );

  btn = _d->menu->addButton( "Load game", -1 );
  CONNECT( btn, onClicked(), _d.data(), Impl::resolveShowLoadGameWnd );

  btn = _d->menu->addButton( "##Load map##", -1 );
  CONNECT( btn, onClicked(), _d.data(), Impl::resolveShowLoadMapWnd );

  btn = _d->menu->addButton( "Quit", -1 );
  CONNECT( btn, onClicked(), _d.data(), Impl::resolveQuitGame );
}

int ScreenMenu::getResult() const
{
  return _d->result;
}

bool ScreenMenu::isStopped() const
{
  return _d->isStopped;
}

const std::string& ScreenMenu::getMapName() const
{
  return _d->fileMap;
}