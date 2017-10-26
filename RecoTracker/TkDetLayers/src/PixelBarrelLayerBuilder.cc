#include "PixelBarrelLayerBuilder.h"
#include "Phase2ITtiltedBarrelLayer.h"
#include "PixelRodBuilder.h"
#include "Phase2EndcapRingBuilder.h"

using namespace std;
using namespace edm;

PixelBarrelLayer* PixelBarrelLayerBuilder::build(const GeometricDet* aPixelBarrelLayer,
						 const TrackerGeometry* theGeomDetGeometry)
{
  // This builder is very similar to TOBLayer one. Most of the code should be put in a 
  // common place.


  LogTrace("TkDetLayers") << "PixelBarrelLayerBuilder::build";
  vector<const GeometricDet*>  theGeometricDets = aPixelBarrelLayer->components();
  LogDebug("TkDetLayers") << "PixelBarrelLayerBuilder with #Components: " << theGeometricDets.size() << std::endl;
  vector<const GeometricDet*>  theGeometricDetRods;
  vector<const GeometricDet*>  theGeometricDetRings;

  for(vector<const GeometricDet*>::const_iterator it = theGeometricDets.begin();
      it!=theGeometricDets.end(); it++){

    if( (*it)->type() == GeometricDet::ladder) {
      theGeometricDetRods.push_back(*it);
    } else if( (*it)->type() == GeometricDet::panel) {
      theGeometricDetRings.push_back(*it);
    } else {
      LogDebug("TkDetLayers") << "PixelBarrelLayerBuilder with no Rods and no Rings! ";
    }
  }

  LogDebug("TkDetLayers") << "PixelBarrelLayerBuilder with #Rods: " << theGeometricDetRods.size() << std::endl;

  PixelRodBuilder myPixelRodBuilder;

  vector<const PixelRod*> theInnerRods;
  vector<const PixelRod*> theOuterRods;

  // properly calculate the meanR value to separate rod in inner/outer.

  double meanR = 0;
  for (unsigned int index=0; index!=theGeometricDetRods.size(); index++)   meanR+=theGeometricDetRods[index]->positionBounds().perp();
  if (!theGeometricDetRods.empty())
    meanR/=(double) theGeometricDetRods.size();
  
  for(unsigned int index=0; index!=theGeometricDetRods.size(); index++){    
    if(theGeometricDetRods[index]->positionBounds().perp() < meanR)
      theInnerRods.push_back(myPixelRodBuilder.build(theGeometricDetRods[index],
						     theGeomDetGeometry)    );       

    if(theGeometricDetRods[index]->positionBounds().perp() > meanR)
      theOuterRods.push_back(myPixelRodBuilder.build(theGeometricDetRods[index],
						     theGeomDetGeometry)    );       

  }
  
  return new PixelBarrelLayer(theInnerRods,theOuterRods);

  LogDebug("TkDetLayers") << "PixelBarrelLayerBuilder with #Rings: " << theGeometricDetRings.size() << std::endl;

  Phase2EndcapRingBuilder myPhase2EndcapRingBuilder;

  vector<const Phase2EndcapRing*> theNegativeRings;
  vector<const Phase2EndcapRing*> thePositiveRings;

  // properly calculate the meanR value to separate rod in inner/outer.
  double centralZ = 0.0;

  for(vector<const GeometricDet*>::const_iterator it=theGeometricDetRings.begin();
      it!=theGeometricDetRings.end();it++){
    if((*it)->positionBounds().z() < centralZ)
      theNegativeRings.push_back(myPhase2EndcapRingBuilder.build( *it,theGeomDetGeometry,true ));
    if((*it)->positionBounds().z() > centralZ)
      thePositiveRings.push_back(myPhase2EndcapRingBuilder.build( *it,theGeomDetGeometry,true ));
  }

  return new Phase2ITtiltedBarrelLayer(theInnerRods,theOuterRods,theNegativeRings,thePositiveRings);

}

