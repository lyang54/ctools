#! /usr/bin/env python
# ==========================================================================
# This scripts performs tests processing pipelines.
#
# Copyright (C) 2014 Juergen Knoedlseder
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
# ==========================================================================
import gammalib
import ctools


# ============================ #
# Test class for ctobssim tool #
# ============================ #
class Test(gammalib.GPythonTestSuite):
    """
    Test class for pipelines.
    """
    # Constructor
    def __init__(self):
        """
        Constructor.
        """
        # Call base class constructor
        gammalib.GPythonTestSuite.__init__(self)

        # Return
        return

    # Set test functions
    def set(self):
        """
        Set all test functions.
        """
        # Set test name
        self.name("pipelines")

        # Append tests
        self.append(self.test_unbinned_fits, "Test unbinned pipeline with FITS file saving")
        self.append(self.test_unbinned_mem, "Test unbinned in-memory pipeline")

        # Return
        return

    # Test unbinned pipeline with FITS file saving
    def test_unbinned_fits(self):
        """
        Test unbinned pipeline with FITS file saving.
        """
        # Set script parameters
        model_name           = "data/crab.xml"
        events_name          = "events.fits"
        selected_events_name = "selected_events.fits"
        result_name          = "results.xml"
        caldb                = "irf"
        irf                  = "cta_dummy_irf"
        ra                   =   83.63
        dec                  =   22.01
        rad_sim              =   10.0
        tstart               =    0.0
        tstop                = 1800.0
        emin                 =    0.1
        emax                 =  100.0
        rad_select           =    3.0
    
        # Simulate events
        sim = ctools.ctobssim()
        sim["inmodel"]   = model_name
        sim["outevents"] = events_name
        sim["caldb"]     = caldb
        sim["irf"]       = irf
        sim["ra"]        = ra
        sim["dec"]       = dec
        sim["rad"]       = rad_sim
        sim["tmin"]      = tstart
        sim["tmax"]      = tstop
        sim["emin"]      = emin
        sim["emax"]      = emax
        self.test_try("Execute ctobssim")
        try:
            sim.execute()
            self.test_try_success()
        except:
            self.test_try_failure("Exception occured in ctobssim.")
    
        # Select events
        select = ctools.ctselect()
        select["inobs"]  = events_name
        select["outobs"] = selected_events_name
        select["ra"]     = ra
        select["dec"]    = dec
        select["rad"]    = rad_select
        select["tmin"]   = tstart
        select["tmax"]   = tstop
        select["emin"]   = emin
        select["emax"]   = emax
        self.test_try("Execute ctselect")
        try:
            select.execute()
            self.test_try_success()
        except:
            self.test_try_failure("Exception occured in ctselect.")
    
        # Perform maximum likelihood fitting
        like = ctools.ctlike()
        like["inobs"]    = selected_events_name
        like["inmodel"]  = model_name
        like["outmodel"] = result_name
        like["caldb"]    = caldb
        like["irf"]      = irf
        self.test_try("Execute ctlike")
        try:
            like.execute()
            self.test_try_success()
        except:
            self.test_try_failure("Exception occured in ctlike.")


    # Test unbinned in-memory pipeline
    def test_unbinned_mem(self):
        """
        Test unbinned in-memory pipeline.
        """
        # Set script parameters
        model_name           = "data/crab.xml"
        caldb                = "irf"
        irf                  = "cta_dummy_irf"
        ra                   =   83.63
        dec                  =   22.01
        rad_sim              =   10.0
        tstart               =    0.0
        tstop                = 1800.0
        emin                 =    0.1
        emax                 =  100.0
        rad_select           =    3.0
    
        # Simulate events
        sim = ctools.ctobssim()
        sim["inmodel"] = model_name
        sim["caldb"]   = caldb
        sim["irf"]     = irf
        sim["ra"]      = ra
        sim["dec"]     = dec
        sim["rad"]     = rad_sim
        sim["tmin"]    = tstart
        sim["tmax"]    = tstop
        sim["emin"]    = emin
        sim["emax"]    = emax
        self.test_try("Run ctobssim")
        try:
            sim.run()
            self.test_try_success()
        except:
            self.test_try_failure("Exception occured in ctobssim.")
    
        # Select events
        select = ctools.ctselect(sim.obs())
        select["ra"]   = ra
        select["dec"]  = dec
        select["rad"]  = rad_select
        select["tmin"] = tstart
        select["tmax"] = tstop
        select["emin"] = emin
        select["emax"] = emax
        self.test_try("Run ctselect")
        try:
            select.run()
            self.test_try_success()
        except:
            self.test_try_failure("Exception occured in ctselect.")
    
        # Perform maximum likelihood fitting
        like = ctools.ctlike(select.obs())
        self.test_try("Run ctlike")
        try:
            like.run()
            self.test_try_success()
        except:
            self.test_try_failure("Exception occured in ctlike.")
