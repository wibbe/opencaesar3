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



#include "oc3_tilemap_area.hpp"

#include <algorithm>
#include <iostream>
#include "oc3_positioni.hpp"
#include "oc3_exception.hpp"

TilemapArea::TilemapArea()
{
  _tilemap = NULL;
  _map_size = 0;
  _view_width = 0;
  _view_height = 0;
  _center_i = 0;
  _center_j = 0;
  _center_x = 0;
  _center_z = 0;
}

TilemapArea::~TilemapArea()
{
}

void TilemapArea::init(Tilemap &tilemap)
{
  _tilemap = &tilemap;
  _map_size = tilemap.getSize();
}

void TilemapArea::setViewSize(const int width, const int height)
{
  if (_view_width != width || _view_height != height)
  {
    _coordinates.clear();
  }
  _view_width = (width + 59) / 60;
  _view_height = (height + 29) / 30;
  
  std::cout << "width and height " <<  _view_width << " " << _view_height << std::endl;
  
  _coordinates.reserve(width * height);
}

void TilemapArea::setCenterIJ(const TilePos& pos )
{
  _center_i = pos.getI();
  _center_j = pos.getJ();

  setCenterXZ( _center_i + _center_j, _map_size - 1 + _center_j - _center_i );
}

void TilemapArea::setCenterXZ(const int x, const int z)
{
  if (_center_x != x || _center_z != z)
  {
  _coordinates.clear();
  }
  
  _center_x = x;
  _center_z = z;
}

int TilemapArea::getCenterX() const  {   return _center_x;   }
int TilemapArea::getCenterZ() const  {   return _center_z;   }
int TilemapArea::getCenterI() const  {   return _center_i;   }
int TilemapArea::getCenterJ() const  {   return _center_j;   }


void TilemapArea::moveRight(const int amount)
{
  setCenterXZ(getCenterX() + amount, getCenterZ());
}

void TilemapArea::moveLeft(const int amount)
{
  setCenterXZ(getCenterX() - amount, getCenterZ());
}

void TilemapArea::moveUp(const int amount)
{
  setCenterXZ(getCenterX(), getCenterZ() + amount);
}

void TilemapArea::moveDown(const int amount)
{
  setCenterXZ(getCenterX(), getCenterZ() - amount);
}

const std::vector< TilePos >& TilemapArea::getTiles()
{
  if( _coordinates.empty() )
  {
    int zm = _map_size + 1;
    int cx = _center_x;
    int cz = _center_z;
    int sx = _view_width;  // size x
    int sz = _view_height; // size z

    for (int z = cz + sz; z>=cz - sz; --z)
    {
      // depth axis. from far to near.

      int xstart = cx - sx;
      if ((xstart + z) % 2 == 0)
      {
	++xstart;
      }

      for (int x = xstart; x<=cx + sx; x+=2)
      {
	// left-right axis
        int j = (x + z - zm)/2;
        int i = x - j;

        if (i >= 0 && j >= 0 && i < _map_size && j < _map_size)
        {
	  _coordinates.push_back( TilePos( i, j ));
        }
      }
    }
  }

  return _coordinates;
}
