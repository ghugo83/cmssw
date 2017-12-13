Each row has 3 (or 4) entries:
sensor DetId  //  numCols   //  numRows   //   (DTC Id)


DTC identification (Outer Tracker only):
The 4th entry is the DTC to which the module is connected.

Scheme: Each DTC is identified by a string similar to 'neg_a_PS_b'.
* "neg" :                   This is added when the DTC is connected to the Tracker negative cabling side.
* int a (0 <= a <= 8) :     Phi sector number.
                            There are 9 Phi-sectors in total, of 40° each. 
                            Phi sector 0 is (0° 40°). Phi sector 1 is (40° 80°). Phi sector 2 is (80° 120°)...
                            Angles are considered in CMS (XY) plane and counted from (X) axis.
* "2S" or "PS" :            module type.
* int b (1 <= b <= 6]) :    DTC slot.

Example:
neg_1_2S_2 is the DTC connected to modules on the negative cabling side. 
The modules are 2S and located in the (40° 80°) Phi sector. 
DTC slot is 2.

