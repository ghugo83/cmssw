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


The work so far more or less creates, for the telescope, the equivalent of the full TrackerNumberingBuilder and TrackerGeometry packages.
