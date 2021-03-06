.. _ctcubemask:

ctcubemask
==========

Filter a counts cube.


Synopsis
--------

This tool masks out specific regions from a counts cube by setting the
corresponding pixels to negative values.


General parameters
------------------

``inobs [file]``
    Input counts cube or observation definition XML file.

``regfile [file]``
    Exclusion region file in ds9 format.

``outcube [file]``
    Output counts cube or observation definition XML file.

``(prefix = filtered_) [string]``
    Prefix for output counts cube files in case that an observation
    definition XML file has been specified on input.

``(usepnt = no) [boolean]``
    Use CTA pointing direction instead of ra/dec parameters?
 	 	 
``ra [real]``
    Right Ascension of circular selection region centre (J2000, in degrees).
 	 	 
``dec [real]``
    Declination of circular selection region centre (J2000, in degrees).

``rad [real]``
    Radius of circular selection region (in degrees).

``emin [real]``
    Lower energy limit (in TeV).
 	 	 
``emax [real]``
    Upper energy limit (in TeV).
 	 	 

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

``(logfile = ctexpcube.log) [string]``
    Name of log file.


Related tools
-------------

None
