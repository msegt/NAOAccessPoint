// Copyright (C) 2014 Maria Jose Galvez Trigo GNU GPLV3

#ifndef TETHERINGENABLER_H
#define TETHERINGENABLER_H

#include <boost/shared_ptr.hpp>
#include <alcommon/almodule.h>
#include <string>

#include <alproxies/almemoryproxy.h>
#include <alproxies/altexttospeechproxy.h>
#include <alproxies/alconnectionmanagerproxy.h>

#include <althread/almutex.h>

namespace AL
{
  class ALBroker;
}

class TetheringEnabler : public AL::ALModule
{
  public:

    TetheringEnabler(boost::shared_ptr<AL::ALBroker> broker, const std::string& name);

    virtual ~TetheringEnabler();

    virtual void init();

    // Methods called when one sensor on NAO's head, the front or
    // the rear, is touched
    void FrontSensorTouched();
    void RearSensorTouched();

    // We create the variables that will be used as parametres for
    // enabling tethering. Passphrase will only be used if tethering
    // technology is wifi.
    // technology can be either wifi or bluetooth (if the robot has a
    // bluetooth module installed. If it does not have a bluetooth
    // module installed it won't be able to enable tethering via
    // bluetooth and will ask the user to check if it has a bluetooth
    // module installed.
    std::string technology;
    std::string access_point;
    std::string passphrase;

    // Vector that will contain the network technologies available for the robot
    std::vector<std::string> technologies;
    // Iterator to go through the vector of technologies
    std::vector<std::string>::iterator it;

   private:
    // Memory proxy for event subscription and data access.
    AL::ALMemoryProxy memory;
    // TTS proxy to let NAO talk.
    AL::ALTextToSpeechProxy tts;
    // Connection manager proxy to manage the tethering state.
    AL::ALConnectionManagerProxy connection;

    // Mutex to make the functions thread-safe.
    boost::shared_ptr<AL::ALMutex> mutex;
};

#endif // TETHERINGENABLER_H
