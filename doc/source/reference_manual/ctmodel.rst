.. _ctmodel:

ctmodel
=======

Computes model counts cube.


Synopsis
--------

This tool computes a model counts cube based on an XML model.
The counts cube is a 3-dimensional data cube spanned by Right Ascension or
Galactic longitude, Declination or Galactic latitude, and energy.


General parameters
------------------

``incube [file]``
    Input counts cube. If a filename is given the counts cube will be
    used to define the dimensions of the model cube. The content of
    the counts cube is not relevant.

``inmodel [string]``
    Source model XML file.

``outcube [file]``
    Output model counts cube.
 	 	 
``(edisp = no) [boolean]``
    Apply energy dispersion to response computation.

``inobs [file]``
    Input event list, counts cube or observation definition XML file.
    This information is needed to determine the observation information
    (number of observations, duration, liftime, pointing, eventually
    observation specific instrument response functions, etc.).

``expcube [file]``
    Exposure cube file (only needed for stacked analysis).

``psfcube [file]``
    PSF cube file (only needed for stacked analysis).

``bkgcube [file]``
    Background cube file (only needed for stacked analysis).

``caldb [string]``
    Calibration database.
 	 	 
``irf [string]``
    Instrument response function.
 	 	 
``ra [real]``
    Right Ascension of pointing (J2000, in degrees).
 	 	 
``dec [real]``
    Declination of pointing (J2000, in degrees).

``rad [real]``
    Radius of field of view (in degrees).
 	 	 
``tmin [real]``
    CTA mission elapsed start time (in seconds).
 	 	 
``tmax [real]``
    CTA mission elapsed stop time (in seconds).
 	 	 
``(deadc = 0.95) [real]``
    Average deadtime correction factor.

``(ebinalg = LOG) <FILE|LIN|LOG> [string]``
    Algorithm for defining energy bins.
 	 	 
``emin [real]``
    Lower energy value for first energy bin (in TeV).
 	 	 
``emax [real]``
    Upper energy value for last energy bin (in TeV).
 	 	 
``enumbins [integer]``
    Number of energy bins.
 	 	 
``(ebinfile = NONE) [file]``
    Name of the file containing the energy bin definition.

``(usepnt = no) [boolean]``
    Use pointing instead of xref/yref parameters?
 	 	 
``nxpix [integer]``
    Size of the Right Ascension / Galactic longitude axis (in pixels).
 	 	 
``nypix [integer]``
    Size of the Declination / Galactic latitude axis (in pixels).
 	 	 
``binsz [real]``
    Pixel size (in degrees/pixel).
 	 	 
``coordsys <CEL|GAL> [string]``
    Coordinate system (CEL - celestial, GAL - galactic).
 	 	 
``xref [real]``
    Right Ascension / Galactic longitude of image centre (J2000, in degrees).
 	 	 
``yref [real]``
    Declination / Galactic latitude of image centre (J2000, in degrees).
 	 	 
``proj <AIT|AZP|CAR|MER|MOL|STG|TAN> [string]``
    Projection method.


Standard parameters
-------------------

``(chatter = 2) [integer]``
    Verbosity of the executable:
     chatter = 0: no information will be logged
     
     chatter = 1: only errors will be logged
     
     chatter = 2: errors and actions will be logged
     
     chatter = 3: report about the task execution
     
     chatter = 4: detailed report about the task execution
 	 	 
``(clobber = yes) [boolean]``
    Specifies whether an existing output counts cube should be overwritten.
 	 	 
``(debug = no) [boolean]``
    Enables debug mode. In debug mode the executable will dump any log file output to the console.
 	 	 
``(mode = ql) [string]``
    Mode of automatic parameters (default is "ql", i.e. "query and learn").

``(logfile = ctmodel.log) [string]``
    Name of log file.


Related tools
-------------

None
