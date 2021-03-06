.. _sec_stacked:

Performing a stacked analysis
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

A stacked analysis is a binned analysis where all data from multiple
observations are stacked into a single counts cube.
The event stacking is done using the :ref:`ctbin` tool.
Instead of providing to :ref:`ctbin` an event list we
now specify the observation definition XML file ``obs.xml`` 
on input.
:ref:`ctbin` will then loop over all observations and collect all events
into a single counts cube:

.. code-block:: bash

  $ ctbin
  Event list or observation definition file [events.fits] obs.xml
  First coordinate of image center in degrees (RA or galactic l) (0-360) [83.63] 
  Second coordinate of image center in degrees (DEC or galactic b) (-90-90) [22.01] 
  Projection method (AIT|AZP|CAR|MER|MOL|STG|TAN) [CAR] 
  Coordinate system (CEL - celestial, GAL - galactic) (CEL|GAL) [CEL] 
  Image scale (in degrees/pixel) [0.02] 
  Size of the X axis in pixels [200] 
  Size of the Y axis in pixels [200] 
  Algorithm for defining energy bins (FILE|LIN|LOG) [LOG] 
  Start value for first energy bin in TeV [0.1] 
  Stop value for last energy bin in TeV [100.0] 
  Number of energy bins [20] 
  Output counts cube [cntcube.fits] 

We now have a stacked counts cube ``cntcube.fits`` on disk.
Before we can use that counts cube in a maximum likelihood
analysis, we have to compute the instrument response and the
background model that are needed to describe the stacked data.

For the former, we have to compute the total exposure for the stacked
cube (i.e. the sum of the effective areas for each observation multiplied
by the corresponding livetimes) and an effective point spread function
(i.e. the point spread function of the different observations weighted by
the corresponding exposures).
To get both informations we use the :ref:`ctexpcube` and 
:ref:`ctpsfcube` tools:

.. code-block:: bash

  $ ctexpcube
  Event list or observation definition file [NONE] obs.xml
  Calibration database [prod2] 
  Instrument response function [South_50h] 
  Counts cube for exposure cube definition [NONE] cntcube.fits
  Output exposure cube file [expcube.fits] 

.. code-block:: bash

  $ ctpsfcube
  Event list or observation definition file [NONE] obs.xml
  Calibration database [prod2] 
  Instrument response function [South_50h] 
  Counts cube for psf cube definition [NONE] 
  First coordinate of image center in degrees (RA or galactic l) (0-360) [83.63] 
  Second coordinate of image center in degrees (DEC or galactic b) (-90-90) [22.01] 
  Projection method (AIT|AZP|CAR|MER|MOL|STG|TAN) [CAR] 
  Coordinate system (CEL - celestial, GAL - galactic) (CEL|GAL) [CEL] 
  Image scale (in degrees/pixel) [1.0] 
  Size of the X axis in pixels [10] 
  Size of the Y axis in pixels [10] 
  Lower energy limit (TeV) [0.1] 
  Upper energy limit (TeV) [100.0] 
  Number of energy bins [20] 
  Output psf cube file [psfcube.fits] 

We provide the ``obs.xml`` file on input to inform both tools which
observations have been combined.
For :ref:`ctexpcube` we further provide the counts cube so that the
tool can copy the exposure cube definition (number of spatial pixels
and pixel size, number of energy bins) from the counts cube.
This minimises the number of further user parameters that need to be
provided and assures an exposure cube that is compatible with the counts
cube.
For :ref:`ctpsfcube` we do not use the counts cube for the PSF cube
definition as this would lead to a hugh file owing to the fine spatial
pixelisation of the counts cube.
Since the PSF evolves only slowly over the field of fiew, we provide a
rather coarse spatial binning of 1 degree covering a grid of 10 x 10 
degrees around the centre of the counts cube.
For the energy binning, we use the same logarithmic binning that has
also been used for the counts cube.

As final step of the analysis preparation, we need to generate a
background cube using the :ref:`ctbkgcube` tool:

.. code-block:: bash

  $ ctbkgcube
  Input event list or observation definition file [NONE] obs.xml
  Calibration database [prod2] 
  Instrument response function [South_50h] 
  Counts cube for background cube definition [NONE] cntcube.fits
  Input model XML file [NONE] $CTOOLS/share/models/crab.xml
  Output background cube file [bkgcube.fits] 
  Output model XML file [NONE] model.xml

The usage of :ref:`ctbkgcube` is very similar to that of :ref:`ctexpcube`,
yet it takes the model XML file as an additional input parameter.
We here use the usual ``$CTOOLS/share/models/crab.xml`` model
file that is shipped with the ctools.
:ref:`ctbkgcube` provides on output the background cube file
``bkgcube.fits`` and the model XML file ``model.xml`` that can
be used for further analysis.
Having a look at ``model.xml`` illustrates how the background
modelling works:

.. code-block:: xml

  <?xml version="1.0" encoding="UTF-8" standalone="no"?>
  <source_library title="source library">
    <source name="Crab" type="PointSource" tscalc="0">
      <spectrum type="PowerLaw">
        <parameter name="Prefactor" value="5.7" error="0" scale="1e-16" min="1e-07" max="1000" free="1" />
        <parameter name="Index" value="2.48" error="0" scale="-1" min="0" max="5" free="1" />
        <parameter name="Scale" value="0.3" scale="1e+06" min="0.01" max="1000" free="0" />
      </spectrum>
      <spatialModel type="SkyDirFunction">
        <parameter name="RA" value="83.6331" scale="1" min="-360" max="360" free="0" />
        <parameter name="DEC" value="22.0145" scale="1" min="-90" max="90" free="0" />
      </spatialModel>
    </source>
    <source name="BackgroundModel" type="CTACubeBackground" instrument="CTA,HESS,MAGIC,VERITAS">
      <spectrum type="PowerLaw">
        <parameter name="Prefactor" value="1" error="0" scale="1" min="0" free="1" />
        <parameter name="Index" value="0" error="0" scale="1" min="-10" max="10" free="1" />
        <parameter name="Scale" value="1" scale="1e+06" free="0" />
      </spectrum>
    </source>
  </source_library>

The Crab source component is the same that is also present in
``$CTOOLS/share/models/crab.xml`` and is not modified.
The background component, however, has been replaced by a model of
type ``CTACubeBackground``.
This model is a 3-dimensional data cube that describes the expected 
background rate as function of spatial position and energy.
The data cube is multiplied by a power law spectrum that allows to adjust
the normalization and slope of the background spectrum in the fit.
This power law could be replaced by any spectral model that is found
as an appropriate multiplicator to the background cube.

.. note::

   There is no constraint on providing the same spatial binning or
   the same energy binning for an exposure cube, a PSF cube,
   a background cube and a counts cube.
   ctools interpolates internally the exposure cube, PSF cube and
   background cube values, hence any arbitrary appropriate binning may
   be used.
   Using the same binning for the exposure cube, the background cube and
   the counts cube is only a convenience.

Now we have all files at hand to launch a stacked maximum likelihood
analysis using the :ref:`ctlike` tool:

.. code-block:: bash

  $ ctlike
  Event list, counts cube or observation definition file [obs.xml] cntcube.fits
  Exposure cube file (only needed for stacked analysis) [NONE] expcube.fits
  PSF cube file (only needed for stacked analysis) [NONE] psfcube.fits
  Background cube file (only needed for stacked analysis) [NONE] bkgcube.fits
  Source model [$CTOOLS/share/models/crab.xml] model.xml
  Source model output file [crab_results.xml] 

:ref:`ctlike` recognises that a counts cube should be analysed and queries
for the exposure cube, the PSF cube, and the background cube file names.
We specified the names of the files produced by the :ref:`ctexpcube`,
the :ref:`ctpsfcube` and the :ref:`ctbkgcube` tools.
Furthermore we provided as model the ``model.xml`` file that has been
generated by the :ref:`ctbkgcube` tool.

The log file of the :ref:`ctlike` run is shown below.
Note that the spectral model that is multiplied with the background
cube has a Prefactor of 1.09 +/- 0.02 and an Index of 0.03 +/- 0.01,
indicating a very small correction to the actual spectrum of the background
cube.
Real life situations may of course require larger correction factors.

.. code-block:: xml

  2015-05-22T21:10:09: +=================================+
  2015-05-22T21:10:09: | Maximum likelihood optimisation |
  2015-05-22T21:10:09: +=================================+
  2015-05-22T21:10:10:  >Iteration   0: -logL=60663.798, Lambda=1.0e-03
  2015-05-22T21:10:11:  >Iteration   1: -logL=60642.413, Lambda=1.0e-03, delta=21.385, max(|grad|)=120.031102 [Index:7]
  2015-05-22T21:10:13:  >Iteration   2: -logL=60642.233, Lambda=1.0e-04, delta=0.180, max(|grad|)=1.139187 [Index:7]
  2015-05-22T21:10:14:  >Iteration   3: -logL=60642.233, Lambda=1.0e-05, delta=0.000, max(|grad|)=-0.001247 [Index:7]
  2015-05-22T21:10:16: 
  2015-05-22T21:10:16: +=========================================+
  2015-05-22T21:10:16: | Maximum likelihood optimization results |
  2015-05-22T21:10:16: +=========================================+
  2015-05-22T21:10:16: === GOptimizerLM ===
  2015-05-22T21:10:16:  Optimized function value ..: 60642.233 
  2015-05-22T21:10:16:  Absolute precision ........: 0.005
  2015-05-22T21:10:16:  Acceptable value decrease .: 2
  2015-05-22T21:10:16:  Optimization status .......: converged
  2015-05-22T21:10:16:  Number of parameters ......: 10
  2015-05-22T21:10:16:  Number of free parameters .: 4
  2015-05-22T21:10:16:  Number of iterations ......: 3
  2015-05-22T21:10:16:  Lambda ....................: 1e-06
  2015-05-22T21:10:16:  Maximum log likelihood ....: -60642.233
  2015-05-22T21:10:16:  Observed events  (Nobs) ...: 25963.000
  2015-05-22T21:10:16:  Predicted events (Npred) ..: 25963.000 (Nobs - Npred = 1.5749e-05)
  2015-05-22T21:10:16: === GModels ===
  2015-05-22T21:10:16:  Number of models ..........: 2
  2015-05-22T21:10:16:  Number of parameters ......: 10
  2015-05-22T21:10:16: === GModelSky ===
  2015-05-22T21:10:16:  Name ......................: Crab
  2015-05-22T21:10:16:  Instruments ...............: all
  2015-05-22T21:10:16:  Instrument scale factors ..: unity
  2015-05-22T21:10:16:  Observation identifiers ...: all
  2015-05-22T21:10:16:  Model type ................: PointSource
  2015-05-22T21:10:16:  Model components ..........: "SkyDirFunction" * "PowerLaw" * "Constant"
  2015-05-22T21:10:16:  Number of parameters ......: 6
  2015-05-22T21:10:16:  Number of spatial par's ...: 2
  2015-05-22T21:10:16:   RA .......................: 83.6331 [-360,360] deg (fixed,scale=1)
  2015-05-22T21:10:16:   DEC ......................: 22.0145 [-90,90] deg (fixed,scale=1)
  2015-05-22T21:10:16:  Number of spectral par's ..: 3
  2015-05-22T21:10:16:   Prefactor ................: 5.8499e-16 +/- 8.02289e-18 [1e-23,1e-13] ph/cm2/s/MeV (free,scale=1e-16,gradient)
  2015-05-22T21:10:16:   Index ....................: -2.49909 +/- 0.0118976 [-0,-5]  (free,scale=-1,gradient)
  2015-05-22T21:10:16:   PivotEnergy ..............: 300000 [10000,1e+09] MeV (fixed,scale=1e+06,gradient)
  2015-05-22T21:10:16:  Number of temporal par's ..: 1
  2015-05-22T21:10:16:   Normalization ............: 1 (relative value) (fixed,scale=1,gradient)
  2015-05-22T21:10:16: === GCTAModelCubeBackground ===
  2015-05-22T21:10:16:  Name ......................: BackgroundModel
  2015-05-22T21:10:16:  Instruments ...............: CTA, HESS, MAGIC, VERITAS
  2015-05-22T21:10:16:  Instrument scale factors ..: unity
  2015-05-22T21:10:16:  Observation identifiers ...: all
  2015-05-22T21:10:16:  Model type ................: "PowerLaw" * "Constant"
  2015-05-22T21:10:16:  Number of parameters ......: 4
  2015-05-22T21:10:16:  Number of spectral par's ..: 3
  2015-05-22T21:10:16:   Prefactor ................: 1.08781 +/- 0.0212678 [0,infty[ ph/cm2/s/MeV (free,scale=1,gradient)
  2015-05-22T21:10:16:   Index ....................: 0.0269681 +/- 0.0111532 [-10,10]  (free,scale=1,gradient)
  2015-05-22T21:10:16:   PivotEnergy ..............: 1e+06 MeV (fixed,scale=1e+06,gradient)
  2015-05-22T21:10:16:  Number of temporal par's ..: 1
  2015-05-22T21:10:16:   Normalization ............: 1 (relative value) (fixed,scale=1,gradient)
