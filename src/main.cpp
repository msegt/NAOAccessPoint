// Program that allows the user to enable tethering via wifi or bluetooth (if the
// NAO Robot has a bluetooth module installed) when it is switched on.

// Copyright (C) 2014 Maria Jose Galvez Trigo GNU GPLV3

#include <signal.h>
#include <boost/shared_ptr.hpp>
#include <alcommon/albroker.h>
#include <alcommon/almodule.h>
#include <alcommon/albrokermanager.h>
#include <alcommon/altoolsmain.h>

#include "tetheringenabler.h"

#ifdef TETHERINGENABLER_IS_REMOTE
# define ALCALL
#else
# ifdef _WIN32
#   define ALCALL __declspec(dllexport)
# else
#   define ALCALL
# endif
#endif

extern "C"
{
  ALCALL int _createModule(boost::shared_ptr<AL::ALBroker> pBroker)
  {
    AL::ALBrokerManager::setInstance(pBroker->fBrokerManager.lock());
    AL::ALBrokerManager::getInstance()->addBroker(pBroker);
      AL::ALModule::createModule<TetheringEnabler>( pBroker, "TetheringEnabler");

    return 0;
  }

  ALCALL int _closeModule()
  {
    return 0;
  }
}

#ifdef TETHERINGENABLER_IS_REMOTE
  int main(int argc, char *argv[])
  {
    TMainType sig;
    sig = &_createModule;

    ALTools::mainFunction("tetheringenabler", argc, argv, sig);
  }
#endif
