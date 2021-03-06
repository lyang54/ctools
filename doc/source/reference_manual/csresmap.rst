.. _csresmap:

csresmap
========

Generates residual counts map.


Synopsis
--------

This scripts generates a residual counts map.


General parameters
------------------

``inobs [file]``
    Input event list, counts cube or observation definition XML file.

``inmodel [file]``
    Model XML file.

``outmap [file]``
    Output residual counts map file.

``caldb [string]``
    Calibration database.

``irf [string]``
    Instrument response function.

``(ebinalg = LOG) <FILE|LIN|LOG> [string]``
    Algorithm for defining energy bins.
 	 	 
``(emin = 0.1) [real]``
    Lower energy value for first energy bin (in TeV).
 	 	 
``(emax = 100.0) [real]``
    Upper energy value for last energy bin (in TeV).
 	 	 
``(enumbins = 20) [integer]``
    Number of energy bins.
 	 	 
``(ebinfile = NONE) [file]``
    Name of the file containing the energy bin definition.
 	 	 
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

``(algorithm = SUBDIV) <SUB|SUBDIV|SUBDIVSQRT> [string]``
    Algorithm used to generate the residual map.
 	 	 

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


Related tools
-------------

None
