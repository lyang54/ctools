/***************************************************************************
 *                        ctool - ctool base class                         *
 * ----------------------------------------------------------------------- *
 *  copyright (C) 2014-2015 by Juergen Knoedlseder                         *
 * ----------------------------------------------------------------------- *
 *                                                                         *
 *  This program is free software: you can redistribute it and/or modify   *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation, either version 3 of the License, or      *
 *  (at your option) any later version.                                    *
 *                                                                         *
 *  This program is distributed in the hope that it will be useful,        *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *  GNU General Public License for more details.                           *
 *                                                                         *
 *  You should have received a copy of the GNU General Public License      *
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.  *
 *                                                                         *
 ***************************************************************************/
/**
 * @file ctool.hpp
 * @brief ctool base class definition
 * @author Juergen Knoedlseder
 */

#ifndef CTOOL_HPP
#define CTOOL_HPP

/* __ Includes ___________________________________________________________ */
#include "GammaLib.hpp"
#include "GCTALib.hpp"

/* __Definitions _________________________________________________________ */


/***********************************************************************//**
 * @class ctool
 *
 * @brief ctool base class
 ***************************************************************************/
class ctool : public GApplication  {
public:
    // Constructors and destructors
    ctool(void);
    ctool(const std::string& name, const std::string& version);
    ctool(const std::string& name, const std::string& version,
          int argc, char* argv[]);
    ctool(const ctool& app);
    virtual ~ctool(void);

    // Operators
    ctool& operator=(const ctool& app);

    // Pure virtual methods
    virtual void clear(void) = 0;
    virtual void run(void) = 0;
    virtual void save(void) = 0;

    // Public methods
    virtual void execute(void);

#ifdef SWIG
public:
#else
protected:
#endif
    // Protected methods
    void                  init_members(void);
    void                  copy_members(const ctool& app);
    void                  free_members(void);
    const bool&           read_ahead(void) const;
    const GTimeReference& time_reference(void) const;
    GObservations         get_observations(const bool& get_response = true);

    // Protected methods that create objects from user parameters
    GEbounds        create_ebounds(void);
    GSkymap         create_map(const GObservations& obs);
    GCTAEventCube   create_cube(const GObservations& obs);
    GCTAObservation create_cta_obs(void);

    // Protected methods that check user parameters
    void            require_inobs(const std::string& method);

    // Protected support methods
    void            set_response(GObservations& obs);
    void            set_obs_response(GCTAObservation* obs);
    void            set_obs_bounds(GObservations& obs);
    GSkyDir         get_mean_pointing(const GObservations& obs);
    size_t          get_current_rss(void);

    // Protected members
    bool           m_read_ahead; //!< Read ahead parameters
    bool           m_use_xml;    //!< Use XML file instead of FITS file for observations
    GTimeReference m_cta_ref;    //!< CTA time reference
};


/***********************************************************************//**
 * @brief Return observation container
 *
 * @return Reference to observation container
 ***************************************************************************/
inline
const bool& ctool::read_ahead(void) const
{
    return (m_read_ahead);
}


/***********************************************************************//**
 * @brief Return time reference
 *
 * @return Reference to time reference
 ***************************************************************************/
inline
const GTimeReference& ctool::time_reference(void) const
{
    return (m_cta_ref);
}

#endif /* CTOOL_HPP */
