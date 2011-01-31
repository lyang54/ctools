/***************************************************************************
 *                ctobssim - CTA observation simulator tool                *
 * ----------------------------------------------------------------------- *
 *  copyright (C) 2011 by Jurgen Knodlseder                                *
 * ----------------------------------------------------------------------- *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
/**
 * @file ctobssim.cpp
 * @brief CTA observation simulator tool implementation
 * @author J. Knodlseder
 */

/* __ Includes ___________________________________________________________ */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <stdio.h>
#include "ctobssim.hpp"
#include "GTools.hpp"

/* __ Debug definitions __________________________________________________ */

/* __ Coding definitions _________________________________________________ */


/*==========================================================================
 =                                                                         =
 =                        Constructors/destructors                         =
 =                                                                         =
 ==========================================================================*/

/***********************************************************************//**
 * @brief Void constructor
 ***************************************************************************/
ctobssim::ctobssim(void) : GApplication(CTOBSSIM_NAME, CTOBSSIM_VERSION)
{
    // Initialise members
    init_members();

    // Write header into logger
    log_header();

    // Return
    return;
}


/***********************************************************************//**
 * @brief Command line constructor
 *
 * @param[in] argc Number of arguments in command line.
 * @param[in] argv Array of command line arguments.
 ***************************************************************************/
ctobssim::ctobssim(int argc, char *argv[]) : 
                    GApplication(CTOBSSIM_NAME, CTOBSSIM_VERSION, argc, argv)
{
    // Initialise members
    init_members();

    // Write header into logger
    log_header();

    // Return
    return;
}


/***********************************************************************//**
 * @brief Destructor
 ***************************************************************************/
ctobssim::~ctobssim(void)
{
    // Free members
    free_members();

    // Return
    return;
}


/*==========================================================================
 =                                                                         =
 =                               Operators                                 =
 =                                                                         =
 ==========================================================================*/

/*==========================================================================
 =                                                                         =
 =                            Public methods                               =
 =                                                                         =
 ==========================================================================*/

/***********************************************************************//**
 * @brief Simulate event data
 ***************************************************************************/
void ctobssim::run(void)
{
    // Get parameters
    get_parameters();

    // Write parameters into logger
    if (logTerse()) {
        log_parameters();
        log << std::endl;
        log << m_pnt << std::endl;
        log << m_obs << std::endl;
        log << m_models << std::endl;
    }

    // Write header
    if (logTerse()) {
        log << std::endl;
        log.header1("Simulate photons");
    }

    // Simulate photons
    GPhotons photons = simulate_photons();

    // Write header
    if (logTerse()) {
        log << std::endl;
        log.header1("Simulate events");
    }

    // Simulate events
    GCTAEventList* events = simulate_events(photons);

    // Write header
    if (logTerse()) {
        log << std::endl;
        log.header1("Save events in FITS file");
    }

    // Save events
    save_events(events);

    // Release events
    delete events;

    // Write separator into logger
    if (logTerse())
        log << std::endl;

    // Return
    return;
}


/***********************************************************************//**
 * @brief Get application parameters
 *
 * Get all task parameters from parameter file or (if required) by querying
 * the user.
 ***************************************************************************/
void ctobssim::get_parameters(void)
{
    // Get parameters
    m_infile  = par("infile")->value();
    m_outfile = par("outfile")->value();
    m_caldb   = par("caldb")->value();
    m_irf     = par("irf")->value();
    m_seed    = par("seed")->integer();
    m_ra      = par("ra")->real();
    m_dec     = par("dec")->real();
    m_rad     = par("rad")->real();
    m_tmin    = par("tmin")->real();
    m_tmax    = par("tmax")->real();
    m_emin    = par("emin")->real();
    m_emax    = par("emax")->real();

    // Initialise random number generator
    m_ran.seed(m_seed);

    // Set CTA pointing
    GSkyDir dir;
    dir.radec_deg(m_ra, m_dec);
    m_pnt.dir(dir);

    // Set GTI
    GGti  gti;
    GTime tstart;
    GTime tstop;
    tstart.met(m_tmin);
    tstop.met(m_tmax);
    gti.append(tstart, tstop);

    // Set energy boundaries
    GEbounds ebds;
    GEnergy  emin;
    GEnergy  emax;
    emin.TeV(m_emin);
    emax.TeV(m_emax);
    ebds.append(emin, emax);

    // Set observation
    m_obs.ebounds(ebds);
    m_obs.gti(gti);
    m_obs.response(m_irf, m_caldb);
    m_obs.pointing(m_pnt);

    // Load models
    m_models.load(m_infile);

    // Return
    return;
}


/***********************************************************************//**
 * @brief Simulate photons
 ***************************************************************************/
GPhotons ctobssim::simulate_photons(void)
{
    // Allocate MC parameters
    GSkyDir dir;
    GEnergy emin;
    GEnergy emax;
    GTime   tmin;
    GTime   tmax;

    // Define MC parameters
    dir.radec_deg(m_ra, m_dec);
    emin.TeV(m_emin);
    emax.TeV(m_emax);
    tmin.met(m_tmin);
    tmax.met(m_tmax);

    // Allocate photons
    GPhotons photons;

    // Simulate photons for all sky models
    for (int i = 0; i < m_models.size(); ++i) {

        // Get sky model (NULL if not a sky model)
        GModelSky* model = dynamic_cast<GModelSky*>(m_models(i));

        // If we have a sky model then simulate photons
        if (model != NULL) {

            // Get photons
            GPhotons p = model->mc(m_area, dir, m_rad, emin, emax, tmin, tmax, m_ran);

            // Reserve new space for photons
            photons.reserve(photons.size() + p.size());
        
            // Add photons to list
            for (int k = 0; k < p.size(); ++k) {
                p[k].mcid(i);
                photons.push_back(p[k]);
            }

        } // endif: model was a sky model

    } // endfor: looped over models

    // Dump simulation results
    if (logNormal()) {
        log << parformat("Simulated photons") << str(photons.size()) << std::endl;
        log << parformat("Simulation area");
        log << str(m_area) << " cm2" << std::endl;
        log << parformat("Simulation cone");
        log << "RA=" << dir.ra_deg() << " deg";
        log << ", Dec=" << dir.dec_deg() << "deg";
        log << ", r=" << m_rad << " deg" << std::endl;
        log << parformat("Time interval");
        log << tmin << " - " << tmax << std::endl;
        log << parformat("Energy range");
        log << emin << " - " << emax << std::endl;
        log << parformat("Random number seed") << str(m_seed) << std::endl;
    }
    
    // Return photons
    return photons;
}


/***********************************************************************//**
 * @brief Simulate events
 *
 * @param[in] photons Photon list.
 *
 * Simulate events from a photon list using a given response function and
 * a given pointing.
 ***************************************************************************/
GCTAEventList* ctobssim::simulate_events(const GPhotons& photons)
{
    // Allocate CTA event list
    GCTAEventList* events = new GCTAEventList;

    // Reserves space for events
    events->reserve(photons.size());

    // Simulate events from photons
    for (int i = 0; i < photons.size(); ++i) {
        GCTAEventAtom* event = m_obs.response()->mc(m_area, photons[i], m_pnt, m_ran);
        if (event != NULL)
            events->append(*event);
        delete event;
    }

    // Dump simulation results
    if (logNormal()) {
        log << parformat("MC source events");
        log << str(events->size()) << std::endl;
    }

    // Simulate background events
    for (int i = 0; i < m_models.size(); ++i) {

        // Get model (NULL if not a radial acceptance model)
        GCTAModelRadialAcceptance* model = 
                 dynamic_cast<GCTAModelRadialAcceptance*>(m_models(i));

        // If we have a radial acceptance model then simulate events
        if (model != NULL) {

            // Get simulated event list
            GCTAEventList* list = model->mc(m_obs, m_ran);

            // Reserves space for events
            events->reserve(list->size()+events->size());

            // Append events
            for (int k = 0; k < list->size(); k++)
                events->append(*(list->pointer(k)));

            // Dump simulation results
            if (logNormal()) {
                log << parformat("MC background events");
                log << str(list->size()) << std::endl;
            }

            // Free event list
            delete list;

        } // endif: model was valid

    } // endfor: looped over all models

    // Return event list
    return events;
}


/***********************************************************************//**
 * @brief Save events
 *
 * @todo Set header keywords
 ***************************************************************************/
void ctobssim::save_events(const GCTAEventList* events)
{
    // Allocate events file
    GFits file;

    // Write events
    events->write(&file);

    // Set keywords in EVENTS header
    set_events_keywords(&file);

    // Allocate Gti
    GGti gti;

    // Setup single GTI covering the selected time range
    GTime tstart;
    GTime tstop;
    tstart.met(m_tmin);
    tstop.met(m_tmax);
    gti.add(tstart, tstop);

    // Write GTI
    gti.write(&file);

    // Dump FITS file
    if (logExplicit()) {
        log << file << std::endl;
    }

    // Save events file. We need to use to saveto() method to make sure
    // that any existing file is indeed overwritten
    file.saveto(m_outfile, clobber());

    // Close FITS file
    file.close();

    // Return
    return;
}


/***********************************************************************//**
 * @brief Set keywords of EVENTS header
 *
 * @param[in] file FITS file.
 ***************************************************************************/
void ctobssim::set_events_keywords(GFits* file)
{
    // Get EVENTS HDU
    GFitsHDU* hdu = file->hdu("EVENTS");

    // Set observation time information
    hdu->card("TSTART",   m_tmin,        "[s] Mission time of start of observation");
    hdu->card("TSTOP",    m_tmax,        "[s] Mission time of end of observation");
    hdu->card("TELAPSE",  m_tmax-m_tmin, "[s] Mission elapsed time");
    hdu->card("ONTIME",   m_tmax-m_tmin, "[s] Total good time including deadtime");
    hdu->card("LIVETIME", m_tmax-m_tmin, "[s] Total livetime");

    // Set target information
    //hdu->card("RA_OBJ",  m_ra,  "[deg] Target Right Ascension");
    //hdu->card("DEC_OBJ", m_dec, "[deg] Target Declination");

    // Set pointing information
    hdu->card("RA_PNT",  m_ra,  "[deg] Pointing Right Ascension");
    hdu->card("DEC_PNT", m_dec, "[deg] Pointing Declination");

    // Return
    return;
}


/*==========================================================================
 =                                                                         =
 =                             Private methods                             =
 =                                                                         =
 ==========================================================================*/

/***********************************************************************//**
 * @brief Initialise class members
 ***************************************************************************/
void ctobssim::init_members(void)
{
    // Initialise members
    m_infile.clear();
    m_outfile.clear();
    m_caldb.clear();
    m_irf.clear();
    m_obs.clear();
    m_pnt.clear();
    m_seed =   1;
    m_ra   = 0.0;
    m_dec  = 0.0;
    m_rad  = 0.0;
    m_tmin = 0.0;
    m_tmax = 0.0;
    m_emin = 0.0;
    m_emax = 0.0;

    // Set fixed parameters
    m_area = 3200000.0 * 1.0e4; //!< 3200000.0 m^2

    // Set logger properties
    log.date(true);

    // Return
    return;
}


/***********************************************************************//**
 * @brief Delete class members
 ***************************************************************************/
void ctobssim::free_members(void)
{
    // Return
    return;
}