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
/////////////////////////////////////////////////////////////
// AparTsSim.hh
//
// Mike Dixon, EOL, NCAR
// P.O.Box 3000, Boulder, CO, 80307-3000, USA
//
// Aug 2019
//
///////////////////////////////////////////////////////////////
//
// AparTsSim reads IWRF data from specified files, converts
// the data to APAR TS format, and writes the
// converted files to a specified location
//
////////////////////////////////////////////////////////////////

#ifndef AparTsSim_H
#define AparTsSim_H

#include <string>
#include <vector>
#include <cstdio>

#include "Args.hh"
#include "Params.hh"
#include <radar/iwrf_data.h>
#include <radar/apar_ts_data.h>

using namespace std;

////////////////////////
// This class

class AparTsSim {
  
public:

  // constructor

  AparTsSim(int argc, char **argv);

  // destructor
  
  ~AparTsSim();

  // run 

  int Run();

  // data members

  bool isOK;

  // condition angle from 0 to 360

  static double conditionAngle360(double angle);

  // condition angle from -180 to 180

  static double conditionAngle180(double angle);
  
  // copy metadat from IWRF to APAR
  
  static void copyIwrf2Apar(const iwrf_packet_info_t &iwrf,
                            apar_ts_packet_info_t &apar);
  
  static void copyIwrf2Apar(const iwrf_radar_info_t &iwrf,
                            apar_ts_radar_info_t &apar);
  
  static void copyIwrf2Apar(const iwrf_scan_segment_t &iwrf,
                            apar_ts_scan_segment_t &apar);
  
  static void copyIwrf2Apar(const iwrf_ts_processing_t &iwrf,
                            apar_ts_processing_t &apar);
  
  static void copyIwrf2Apar(const iwrf_calibration_t &iwrf,
                            apar_ts_calibration_t &apar);
  
  static void copyIwrf2Apar(const iwrf_pulse_header_t &iwrf,
                            apar_ts_pulse_header_t &apar);
  
  static void copyIwrf2Apar(const iwrf_event_notice_t &iwrf,
                            apar_ts_event_notice_t &apar);

protected:
  
private:
  
  string _progName;
  char *_paramsPath;
  Args _args;
  Params _params;

  // functions

  int _runWriteToFile();
  int _runWriteToUdp();
  int _runReadFromUdp();

};

#endif
