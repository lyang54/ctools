.. _ctbin:

ctbin
=====

Generate a counts cube for binned maximum likelihood analysis.


Synopsis
--------

This tool computes the counts cube for use in a binned maximum likelihood
analysis.
The counts cubes are 3-dimensional data cubes spanned by Right Ascension or
Galactic longitude, Declination or Galactic latitude, and energy.
The energy binning may be either linear, logarithmic, or custom defined.
If an observation definition XML file containing several observations is 
provided on input, the tool will stack all data into a single counts cube.


General parameters
------------------

``inobs [file]``
    Input event list or observation definition XML file.

``outcube [file]``
    Output counts cube file.
 	 	 
``ebinalg <FILE|LIN|LOG> [string]``
    Algorithm for defining energy bins.
 	 	 
``emin [real]``
    Lower energy value for first energy bin (in TeV).
 	 	 
``emax [real]``
    Upper energy value for last energy bin (in TeV).
 	 	 
``enumbins [integer]``
    Number of energy bins.
 	 	 
``ebinfile [file]``
    Name of the file containing the energy bin definition.
 	 	 
``(usepnt = no) [boolean]``
    Use CTA pointing direction for cube centre instead of xref/yref parameters?
 	 	 
``nxpix [integer]``
    Number of cube bins in Right Ascension or Galactic longitude.
 	 	 
``nypix [integer]``
    Number of cube bins in Declination or Galactic latitude.
 	 	 
``binsz [real]``
    Cube bin size (in degrees/pixel).
 	 	  	 	 
``coordsys <CEL|GAL> [string]``
    Coordinate system (CEL - celestial, GAL - galactic).
 	 	 
``xref [real]``
    Right Ascension / Galactic longitude of cube centre (J2000, in degrees).
 	 	 
``yref [real]``
    Declination / Galactic latitude of cube centre (J2000, in degrees).
 	 	 
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

``(logfile = ctbin.log) [string]``
    Name of log file.


Related tools
-------------

None
