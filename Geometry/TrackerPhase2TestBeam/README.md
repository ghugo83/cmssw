### Pixel Telescope DetId scheme   

<table>
  <tr>
    <td> <b> Name </b> </td>
    <td> <b> Start bit </b> </td>
    <td> <b> Size <br> (number of bits) </b> </td>
    <td colspan="3"> <b> Value </b> </td>
  </tr>

  <tr>
    <td> Telescope </td>
    <td> 28 </td>
    <td> 4 </td>
    <td colspan="3"> always 8 </td>
  </tr>

  <tr>
    <td> Category </td>
    <td> 25 </td>
    <td> 3 </td>
    <td> DUT(s) Container: <br> 2 </td>
    <td> Telescope Arm, <br> (-Z) end: <br> 1 </td>
    <td> Telescope Arm, <br> (+Z) end: <br> 3 </td>
  </tr>

  <tr>
    <td> DUT Holder or Telescope Plane </td>
    <td> 4 </td>
    <td> 21 </td>
    <td> from 1 to the number of DUT Holders <br> (sorted in |Z|) </td>
    <td colspan="2"> Planes within a Telescope Arm: <br> from 1 to 4 <br> (sorted in |Z|) </td>
  </tr>

  <tr>
    <td> Module </td>
    <td> 2 </td>
    <td> 2 </td>
    <td> DUT: <br> always 1 </td>
    <td colspan="2"> Modules within a Telescope Plane: <br> from 1 to 2 <br> (sorted in Radius) </td>
  </tr>

  <tr>
    <td> Sensor </td>
    <td> 0 </td>
    <td> 2 </td>
    <td colspan="1"> <li> Sensor on (-Z) end: 1 </li> <li> Sensor on (+Z) end: 2 </li> </td>
    <td colspan="2"> Phase 1 Pixel sensor: <br> always 0 </td>
  </tr>
</table>


---         


### Parallel Tracker <-> Pixel Telescope

The work so far more or less creates, for the telescope, the equivalent of the full TrackerNumberingBuilder and TrackerGeometry packages.

<table>
  <tr>
    <td> <b> Tracker Geometry </b> </td>
    <td> <b> PixelTelescope geometry </b> </td>
    <td> <b> Notes </b> </td>
  </tr>

  <tr>
    <td> Geometry.CMSCommonData.cmsExtendedGeometry2023D21XML_cfi </td>
    <td> Geometry.TrackerPhase2TestBeam.Phase2TestBeamGeometryXML_cfi    </td>
    <td> DD Geometry. </td>
  </tr>

  <tr>
    <td> Geometry.TrackerNumberingBuilder.trackerNumberingGeometry_cfi  </td>
    <td> Geometry.TrackerPhase2TestBeam.telescopeGeometryNumbering_cfi    </td>
    <td> Geometry is ordered and DetIds are assigned. </td>
  </tr>

  <tr>
    <td> Geometry.TrackerGeometryBuilder.trackerParameters_cfi       </td>
    <td> Geometry.TrackerPhase2TestBeam.telescopeParameters_cfi      </td>
    <td> Read parameters from DD. </td>
  </tr>


  <tr>
    <td> Geometry.TrackerNumberingBuilder.trackerTopology_cfi   </td>
    <td>  Geometry.TrackerPhase2TestBeam.telescopeTopology_cfi         </td>
    <td>  Allow to get the layer, or plane, or whether a sensor is inner or outer, etc.. from a given DetId. </td>
  </tr>

  <tr>
    <td> Geometry.TrackerGeometryBuilder.trackerGeometry_cfi     </td>
    <td>  Geometry.TrackerPhase2TestBeam.telescopeGeometry_cfi          </td>
    <td>  Full geometry, as used by the Digitizer. </td>
  </tr>

  <tr>
    <td> SLHCUpgradeSimulations.Geometry.fakeConditions_phase2TkTiltedBase_cff   </td>
    <td>   ?            </td>
    <td> Not tuned yet. </td>
  </tr>

</table>

