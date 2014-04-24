NAOAccessPoint
==============

Copyright (C) 2014 Maria Jose Galvez Trigo GNU GPLv3


NAO Robot can become a bluetooth or wifi access point, so it is not necessary to connect to any external network in order to use the robot.

INFO:
By default, tethering mode can be activated via wifi from the configuration webpage of the robot, but it is necessary to be connected with your computer to the same network as the robot or connect the robot to your computer via ethernet. Furthermore, every time that the robot shuts down this option is disabled again.

WHAT DOES THIS MODULE DO:
Every time that NAO starts up, this code will run, allowing you to enable an access point via wifi or bluetooth without the need of connecting to any network.
NAO will tell you what to do.

If you touch the front sensor on NAO's head you will have a wifi access point with the name of your robot (nao by default) with password "nao12345"

If you touch the rear sensor on NAO's head you will have a bluetooth access point with the name of your robot (nao by default). Note that to enable tethering via bluetooth you should have previously installed a bluetooth receiver in your NAO (usually a USB one), otherwise you won't have the option to enable tethering via bluetooth.

This way you will be able to use your NAO without any external network connexion.

INSTALATION:
You will just need to connect to your NAO the first time to install the module on it. - Connect your robot to your computer and open Choregraphe.
- Connect to your robot from Choregraphe.
- Under the Connection menu open Advanced and File Transfer.
- Navigate to naoqi and there to preferences folder and upload there the file called "autoload.ini" (doesn't matter if there is already a file called "autoload.ini", you have to upload the one provided with this module anyway to make it work).
- Navigate to naoqi and there to lib folder and upload there the file called âlibtetheringenabler.soâ (if there is no lib folder under naoqi just create it).
- Once you have done it you are ready to go. If you reboot your robot he would tell you how to activate tethering.
I hope this module is useful.
