#ifndef TkDetLayers_Phase2ITtiltedBarrelLayer_h
#define TkDetLayers_Phase2ITtiltedBarrelLayer_h


#include "TrackingTools/DetLayers/interface/RodBarrelLayer.h"
#include "PixelBarrelLayer.h"
#include "PixelRod.h"
#include "Phase2EndcapRing.h"
#include "SubLayerCrossings.h"


/** A concrete implementation for Phase2ITtiltedBarrel layer 
 *  built out of BarrelPixelRod
 */

#pragma GCC visibility push(hidden)
class Phase2ITtiltedBarrelLayer final : public PixelBarrelLayer {
 public:

  Phase2ITtiltedBarrelLayer(std::vector<const PixelRod*>& innerRods,
                            std::vector<const PixelRod*>& outerRods,
                            std::vector<const Phase2EndcapRing*>& negRings,
                            std::vector<const Phase2EndcapRing*>& posRings);
  
  ~Phase2ITtiltedBarrelLayer() override;
  
  void groupedCompatibleDetsV( const TrajectoryStateOnSurface& tsos,
			       const Propagator& prop,
			       const MeasurementEstimator& est,
			       std::vector<DetGroup> & result) const final;
    
 private:
  std::vector<const GeometricSearchDet*> theNegativeRingsComps;
  std::vector<const GeometricSearchDet*> thePositiveRingsComps;

  ReferenceCountingPointer<BoundCylinder>  theCylinder;
  
};


#pragma GCC visibility pop
#endif 
