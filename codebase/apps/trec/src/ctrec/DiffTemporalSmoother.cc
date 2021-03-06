// *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=* 
// ** Copyright UCAR (c) 1990 - 2016                                         
// ** University Corporation for Atmospheric Research (UCAR)                 
// ** National Center for Atmospheric Research (NCAR)                        
// ** Boulder, Colorado, USA                                                 
// ** BSD licence applies - redistribution and use in source and binary      
// ** forms, with or without modification, are permitted provided that       
// ** the following conditions are met:                                      
// ** 1) If the software is modified to produce derivative works,            
// ** such modified software should be clearly marked, so as not             
// ** to confuse it with the version available from UCAR.                    
// ** 2) Redistributions of source code must retain the above copyright      
// ** notice, this list of conditions and the following disclaimer.          
// ** 3) Redistributions in binary form must reproduce the above copyright   
// ** notice, this list of conditions and the following disclaimer in the    
// ** documentation and/or other materials provided with the distribution.   
// ** 4) Neither the name of UCAR nor the names of its contributors,         
// ** if any, may be used to endorse or promote products derived from        
// ** this software without specific prior written permission.               
// ** DISCLAIMER: THIS SOFTWARE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS  
// ** OR IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED      
// ** WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.    
// *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=* 
/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/

// RCS info
//   $Author: dixon $
//   $Locker:  $
//   $Date: 2016/03/06 23:28:57 $
//   $Id: DiffTemporalSmoother.cc,v 1.6 2016/03/06 23:28:57 dixon Exp $
//   $Revision: 1.6 $
//   $State: Exp $
 
/**-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-**/
/*********************************************************************
 * DiffTemporalSmoother: Temporal smoother that limits vector changes
 *                       to a percentage of the difference between the
 *                       current U and V values and the previous U and
 *                       V values.
 *
 * RAP, NCAR, Boulder CO
 *
 * December 2002
 *
 * Nancy Rehak
 *
 *********************************************************************/

#include <cassert>

#include "DiffTemporalSmoother.hh"
using namespace std;



/**********************************************************************
 * Constructor
 */

DiffTemporalSmoother::DiffTemporalSmoother(const string &prev_url,
					   const string &prev_u_field_name,
					   const string &prev_v_field_name,
					   const double u_diff_percent,
					   const double v_diff_percent,
					   const bool debug_flag) :
  TemporalSmoother(prev_url, debug_flag),
  _prevUFieldName(prev_u_field_name),
  _prevVFieldName(prev_v_field_name),
  _uDiffPercent(u_diff_percent),
  _vDiffPercent(v_diff_percent)
{
  // Do nothing
}


/**********************************************************************
 * Destructor
 */

DiffTemporalSmoother::~DiffTemporalSmoother(void)
{
  // Do nothing
}


/**********************************************************************
 * smoothData() - Smooth the current vectors based on the previous vectors.
 *
 * Updates the current U and V grids with the smoothed values.
 */

void DiffTemporalSmoother::smoothData(const time_t prev_data_time,
				      const int searchMargin,
				      fl32 *current_u_grid,
				      fl32 *current_v_grid,
				      int grid_size, fl32 bad_data_value)
{
  static const string method_name = "DiffTemporalSmoother::smoothData()";
  
  // Read in the previous data fields

  MdvxField *prev_u_field;
  MdvxField *prev_v_field;
  
  if ((prev_u_field = _readField(_prevUFieldName, prev_data_time, searchMargin)) == 0)
  {
    cerr << "ERROR: " << method_name << endl;
    cerr << "Error readin in previous U field: " << _prevUFieldName << endl;
    
    return;
  }
  
  if ((prev_v_field = _readField(_prevVFieldName, prev_data_time, searchMargin)) == 0)
  {
    cerr << "ERROR: " << method_name << endl;
    cerr << "Error reading in previous V field: " << _prevVFieldName << endl;
    
    delete prev_u_field;
    
    return;
  }
  
  fl32 *previous_u_grid = (fl32 *)prev_u_field->getVol();
  fl32 *previous_v_grid = (fl32 *)prev_v_field->getVol();
  
  for (int i = 0; i < grid_size; ++i)
  {
    // Skip any bad or missing data values

    if (previous_u_grid[i] == bad_data_value ||
	previous_v_grid[i] == bad_data_value)
      continue;
      
    if (current_u_grid[i] == bad_data_value ||
	current_v_grid[i] == bad_data_value)
      continue;
      
    // Perform the temporal smoothing for the U value

    double u_change =
      (current_u_grid[i] - previous_u_grid[i]) * _uDiffPercent;
    current_u_grid[i] = previous_u_grid[i] + u_change;
    
    double v_change =
      (current_v_grid[i] - previous_v_grid[i]) * _vDiffPercent;
    current_v_grid[i] = previous_v_grid[i] + v_change;
    
  } /* endfor - i */
      
  // Reclaim memory

  delete prev_u_field;
  delete prev_v_field;
}


/**********************************************************************
 *              Private Member Functions                              *
 **********************************************************************/
