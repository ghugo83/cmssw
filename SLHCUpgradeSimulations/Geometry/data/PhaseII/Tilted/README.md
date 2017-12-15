Each row has 3 (or 4) entries:
sensor DetId  //  numCols   //  numRows   //   (DTC Id)


DTC Id (Outer Tracker only):
The 4th entry is an identifier of the DTC to which the module is connected.

Scheme:
* |dtcId| = phiSectorIdentifier * 12 + dtcSlot

* dtcId > 0 :                      The DTC is connected to the Tracker positive cabling side.
  dtcId < 0 :                      The DTC is connected to the Tracker negative cabling side.
  
* phiSectorIdentifier :            int
                                   0 <= phiSectorIdentifier <= 8
                                   There are 9 Phi Sectors in total, covering 40° each. 
                                   Phi angles are considered in CMS (XY) plane and counted from (X) axis.
                                   Example: phiSectorIdentifier = 0 corresponds to the (0° 40°) Phi Sector.
                                            phiSectorIdentifier = 1 corresponds to the (40° 80°) Phi Sector.
                                            phiSectorIdentifier = 2 corresponds to the (80° 120°) Phi Sector. 
                                            
* dtcSlot :                        int
                                   1 <= dtcSlot <= 12
                                   if (1 <= dtcSlot <= 2)  : the DTC is connected to PS modules. Module -> DTC optical up-links are at 10 GB/s.
                                   if (3 <= dtcSlot <= 6)  : the DTC is connected to PS modules. Module -> DTC optical up-links are at 5 GB/s.
                                   if (7 <= dtcSlot <= 12) : the DTC is connected to 2S modules.
                                          
Example: dtcId = -73
73 = 6 * 12 + 1.
This DTC is connected to modules on the negative cabling side.
phiSectorIdentifier = 6, ie the DTC covers the (240° 280°) Phi Sector.
dtcSlot = 1, ie the DTC is connected to PS modules, and optical up-links from modules are at 10 GB/s.

