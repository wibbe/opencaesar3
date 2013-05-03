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



#include "oc3_training_building.hpp"

#include <iostream>

#include "oc3_scenario.hpp"
#include "oc3_walker.hpp"
#include "oc3_exception.hpp"
#include "oc3_gui_info_box.hpp"
#include "oc3_gettext.hpp"

namespace {
static const char* rcEntertaimentGroup    = "entertainment";
}


TrainingBuilding::TrainingBuilding( const BuildingType type, const Size& size )
  : WorkingBuilding( type, size )
{
   setMaxWorkers(5);
   setWorkers(0);
   _trainingTimer = 0;
   _trainingDelay = 80;
}

void TrainingBuilding::timeStep(const unsigned long time)
{
   Building::timeStep(time);

   if (_trainingTimer == 0)
   {
      deliverTrainee();
      _trainingTimer = _trainingDelay;
   }
   else if (_trainingTimer > 0)
   {
      _trainingTimer -= 1;
   }

   _animation.update( time );
   Picture *pic = _animation.getCurrentPicture();
   if (pic != NULL)
   {
      int level = _fgPictures.size()-1;
      _fgPictures[level] = _animation.getCurrentPicture();
   }
}

// void TrainingBuilding::deliverTrainee()
// {
//    // make a service walker and send him to his wandering
//    ServiceWalker *walker = new ServiceWalker(_service);
//    walker->setServiceBuilding(*this);
//    walker->start();
// }

void TrainingBuilding::serialize(OutputSerialStream &stream)
{
   WorkingBuilding::serialize(stream);
   stream.write_int(_trainingTimer, 2, 0, 1000);
   stream.write_int(_trainingDelay, 2, 0, 1000);
}

void TrainingBuilding::unserialize(InputSerialStream &stream)
{
   WorkingBuilding::unserialize(stream);
   _trainingTimer = stream.read_int(2, 0, 1000);
   _trainingDelay = stream.read_int(2, 0, 1000);
}


//GuiInfoBox* TrainingBuilding::makeInfoBox()
//{
//   GuiInfoService* box = new GuiInfoService(*this);
//   return box;
//}


BuildingActor::BuildingActor() : TrainingBuilding( B_ACTOR, Size(3) )
{
  setPicture( Picture::load(rcEntertaimentGroup, 81));

   _animation.load( rcEntertaimentGroup, 82, 9);
   _animation.setOffset( Point( 68, -6 ) );
   _fgPictures.resize(1);
}

void BuildingActor::deliverTrainee()
{
   // std::cout << "Deliver trainee!" << std::endl;
   TraineeWalker *trainee = new TraineeWalker(WTT_ACTOR);
   trainee->setOriginBuilding(*this);
   trainee->start();
}

BuildingGladiator::BuildingGladiator() : TrainingBuilding( B_GLADIATOR, Size(3))
{
  setPicture( Picture::load(rcEntertaimentGroup, 51));

   _animation.load(rcEntertaimentGroup, 52, 10);
   _animation.setOffset( Point( 62, 24 ) );
   _fgPictures.resize(1);
}

void BuildingGladiator::deliverTrainee()
{
   // std::cout << "Deliver trainee!" << std::endl;
   TraineeWalker *trainee = new TraineeWalker(WTT_GLADIATOR);
   trainee->setOriginBuilding(*this);
   trainee->start();
}


BuildingLion::BuildingLion() : TrainingBuilding( B_LION, Size(3) )
{
  setPicture( Picture::load(rcEntertaimentGroup, 62));

   _animation.load( rcEntertaimentGroup, 63, 18);
   _animation.setOffset( Point( 78, 21) );
   _fgPictures.resize(1);
}

void BuildingLion::deliverTrainee()
{
   // std::cout << "Deliver trainee!" << std::endl;
   TraineeWalker *trainee = new TraineeWalker(WTT_TAMER);
   trainee->setOriginBuilding(*this);
   trainee->start();
}


BuildingChariot::BuildingChariot() : TrainingBuilding( B_CHARIOT, Size(3) )
{
  setPicture( Picture::load(rcEntertaimentGroup, 91));

   _animation.load(rcEntertaimentGroup, 92, 10);
   _animation.setOffset( Point( 54, 23 ));
   _fgPictures.resize(1);
}

void BuildingChariot::deliverTrainee()
{
   // std::cout << "Deliver trainee!" << std::endl;
   TraineeWalker *trainee = new TraineeWalker(WTT_CHARIOT);
   trainee->setOriginBuilding(*this);
   trainee->start();
}


