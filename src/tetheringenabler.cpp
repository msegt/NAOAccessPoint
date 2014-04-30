// Copyright (C) 2014 Maria Jose Galvez Trigo

#include <sys/socket.h>
#include <sys/param.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

#include "tetheringenabler.h"

#include <alvalue/alvalue.h>
#include <alcommon/alproxy.h>
#include <alcommon/albroker.h>
#include <althread/alcriticalsection.h>

#include <qi/log.hpp>

TetheringEnabler::TetheringEnabler(boost::shared_ptr<AL::ALBroker> broker, const std::string& name):
  AL::ALModule(broker, name), memory(getParentBroker()), mutex(AL::ALMutex::createALMutex())
{
  setModuleDescription("This module turns NAO into a wifi or bluetooth access point.");

  functionName("FrontSensorTouched", getName(), "Called when the front sensor on NAO's head is touched. Turns NAO into a wifi access point.");
  BIND_METHOD(TetheringEnabler::FrontSensorTouched);

  functionName("RearSensorTouched", getName(), "Called when the rear sensor on NAO's head is touched. Turns NAO into a bluetooth access point.");
  BIND_METHOD(TetheringEnabler::RearSensorTouched);

}

TetheringEnabler::~TetheringEnabler()
{
  // We unsubscribe from the events that detect touches on NAO's
  // head
  memory.unsubscribeToEvent("FrontTactilTouched", "TetheringEnabler");
  memory.unsubscribeToEvent("RearTactilTouched", "TetheringEnabler");
}

void TetheringEnabler::init()
{
  try {
    // We give values to the variables that will be used as
    // parametres for enabling tethering. See details in
    // tetheringenabler.h
    char hostname[MAXHOSTNAMELEN];
    gethostname(hostname, MAXHOSTNAMELEN);
    access_point = hostname;
    passphrase = "nao12345";

    // Make NAO wait before start talking to avoid him to talk at the same
    // time that the startup sound bips
    sleep(2);

    // If tethering is not enabled NAO will give instructions on how to enable
    // it via wifi or bluetooth (if there is a bluetooth module installed.
    if (!connection.getTetheringEnable("wifi") && !connection.getTetheringEnable("bluetooth")) {

        technologies = connection.technologies();

        for (int i = 0; i < technologies.size(); i++) {
          technology.assign(technologies[i]);
          if (technology.compare("bluetooth") == 0) {
            // If there is a bluetooth module then the robot will allow you to enable
            // tethering via bluetooth and will say how
            tts.say("To activate tethering via bluetooth, touch the rear sensor on my head.");
            memory.subscribeToEvent("RearTactilTouched", "TetheringEnabler", "RearSensorTouched");
          }
        }

        tts.say("Hello, you will be able to connect to me without cables using tethering mode: ");
        tts.say("To activate tethering via wifi, touch the front sensor on my head.");

        // We subscribe to the events that detect touches on NAO's
        // head and call the corresponding method.
        memory.subscribeToEvent("FrontTactilTouched", "TetheringEnabler", "FrontSensorTouched");


      } else if (connection.getTetheringEnable("wifi")) {
        tts.say("Tethering via wifi is enabled, try to connect to access point.");
        tts.say(access_point);
        tts.say("With password.");
        tts.say(passphrase);
        memory.subscribeToEvent("FrontTactilTouched", "TetheringEnabler", "FrontSensorTouched");
      }

  }
  catch (const AL::ALError& e) {
    qiLogError("module.name") << e.what() << std::endl;
  }
}

void TetheringEnabler::FrontSensorTouched()
{
  // Use a mutex to make it all thread safe
  AL::ALCriticalSection section(mutex);

  try {
    // We unsubscribe from the events that detect touches on NAO's head to
    // avoid it from reacting to several touches meanwhile it is already
    // reacting to one.
    // TODO change for a call to the destructor and try

    memory.unsubscribeToEvent("FrontTactilTouched", "TetheringEnabler");

    technologies = connection.technologies();

    for (int i = 0; i < technologies.size(); i++) {
      technology.assign(technologies[i]);
      if (technology.compare("bluetooth") == 0) {
          // If there is a bluetooth module then the robot will allow you to enable
          // tethering via bluetooth and will say how
          memory.unsubscribeToEvent("RearTactilTouched", "TetheringEnabler");
      }
    }

    // When the front sensor is touched the technology chosen will be wifi.
    technology = "wifi";

    // If tethering via bluetooth is enabled it will be disabled.
    if (connection.getTetheringEnable("bluetooth") == true) {
      tts.say("Tethering was already enabled via bluetooth. Disabling tethering via bluetooth.");
      connection.disableTethering("bluetooth");
      tts.say("tethering via bluetooth has been disabled.");
    }else if (connection.getTetheringEnable("wifi") == true) {
      tts.say("Tethering via wifi was already enabled, try to connect to access point.");
      tts.say(access_point);
      tts.say("With password.");
      tts.say(passphrase);
    }

    // if tethering was disabled then we enable tethering via wifi (value
    // defined for the variable technology at the beginning of the method.
    if (connection.getTetheringEnable(technology) == false) {
      connection.enableTethering(technology, access_point, passphrase);
      tts.say("Tethering via wifi has been enabled, try to connect to access point.");
      tts.say(access_point);
      tts.say("With password.");
      tts.say(passphrase);

    }
    memory.subscribeToEvent("FrontTactilTouched", "TetheringEnabler", "FrontSensorTouched");

    for (int i = 0; i < technologies.size(); i++) {
      technology.assign(technologies[i]);
      if (technology.compare("bluetooth") == 0) {
          // If there is a bluetooth module then the robot will allow you to enable
          // tethering via bluetooth and will say how
          memory.subscribeToEvent("RearTactilTouched", "TetheringEnabler", "RearSensorTouched");
      }
    }
  }
  catch (const AL::ALError& e) {
    tts.say("I could not enable tethering via wifi due to an error.");
    qiLogError("module.name") << e.what() << std::endl;
  }
}

void TetheringEnabler::RearSensorTouched()
{
  // Use a mutex to make it all thread safe
  AL::ALCriticalSection section(mutex);

  try {
    // We unsubscribe from the events that detect touches on NAO's head to
    // avoid it from reacting to several touches meanwhile it is already
    // reacting to one.
    memory.unsubscribeToEvent("FrontTactilTouched", "TetheringEnabler");
    technologies = connection.technologies();

    for (int i = 0; i < technologies.size(); i++) {
      technology.assign(technologies[i]);
      if (technology.compare("bluetooth") == 0) {
          // If there is a bluetooth module then the robot will allow you to enable
          // tethering via bluetooth and will say how
          memory.unsubscribeToEvent("RearTactilTouched", "TetheringEnabler");
      }
    }

    // When the rear sensor is touched the technology chosen will be
    // bluetooth.
    technology = "bluetooth";

    // If tethering via wifi is enabled it will be disabled.
    if (connection.getTetheringEnable("wifi") == true) {
      tts.say("Tethering was already enabled via wifi. Disabling tethering via wifi.");
      connection.disableTethering("wifi");
      tts.say("tethering via wifi has been disabled.");
    }else if (connection.getTetheringEnable("bluetooth") == true) {
      tts.say("Tethering via bluetooth was already enabled.");
    }

    // if tethering was disabled then we enable tethering via bluetooth
    if (connection.getTetheringEnable(technology) == false) {
      connection.enableTethering(technology);
      tts.say("Tethering via bluetooth has been enabled.");
    }

    memory.subscribeToEvent("FrontTactilTouched", "TetheringEnabler", "FrontSensorTouched");
    for (int i = 0; i < technologies.size(); i++) {
      technology.assign(technologies[i]);
      if (technology.compare("bluetooth") == 0) {
          // If there is a bluetooth module then the robot will allow you to enable
          // tethering via bluetooth and will say how
          memory.subscribeToEvent("RearTactilTouched", "TetheringEnabler", "RearSensorTouched");
      }
    }
  }
  catch (const AL::ALError& e) {
    tts.say("I could not enable tethering via bluetooth. Are you sure that I have a bluetooth module installed?");
    qiLogDebug("module.name") << e.what() << std::endl;
  }
}
