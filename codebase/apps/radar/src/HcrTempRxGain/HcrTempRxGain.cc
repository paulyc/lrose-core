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
//////////////////////////////////////////////////////////////////////////
// HcrTempRxGain.cc
//
// Mike Dixon, EOL, NCAR, P.O.Box 3000, Boulder, CO, 80307-3000, USA
//
// August 2018
//
///////////////////////////////////////////////////////////////
//
// HcrTempRxGain reads in HCR monitoring data from SPDB,
// computes the gain correction from the temperatures,
// and writes out the results to SPDB. The HCR monitoring
// data is generated by TsStatusMonitor, which reads in time
// series and writes out the status XML to SPDB.
//
////////////////////////////////////////////////////////////////

#include "HcrTempRxGain.hh"
#include <toolsa/pmu.h>
#include <toolsa/uusleep.h>
#include <toolsa/TaXml.hh>
#include <Spdb/DsSpdb.hh>
using namespace std;

// Constructor

HcrTempRxGain::HcrTempRxGain(int argc, char **argv)
  
{

  OK = TRUE;
  
  // set programe name
  
  _progName = "HcrTempRxGain";
  
  // parse command line args
  
  if (_args.parse(argc, argv, _progName)) {
    cerr << "ERROR: " << _progName << endl;
    cerr << "Problem with command line args." << endl;
    OK = FALSE;
    return;
  }
  
  // get TDRP params
  
  _paramsPath = (char *) "unknown";
  if (_params.loadFromArgs(argc, argv,
			   _args.override.list, &_paramsPath)) {
    cerr << "ERROR: " << _progName << endl;
    cerr << "Problem with TDRP parameters." << endl;
    OK = FALSE;
    return;
  }
  
  // init process mapper registration

  if (_params.mode == Params::REALTIME) {
    PMU_auto_init((char *) _progName.c_str(),
                  _params.instance,
                  PROCMAP_REGISTER_INTERVAL);
  }

}

// destructor

HcrTempRxGain::~HcrTempRxGain()

{

  // unregister process

  PMU_auto_unregister();

}

//////////////////////////////////////////////////
// Run

int HcrTempRxGain::Run()
{

  if (_params.mode == Params::REALTIME) {
    return _runRealtime();
  } else {
    return _runArchive();
  }

}

//////////////////////////////////////////////////
// Run in realtime mode

int HcrTempRxGain::_runRealtime()
{

  int iret = 0;
  time_t prevTime = 0;

  while (true) {

    PMU_auto_register("reading realtime data");

    time_t now = time(NULL);
    int secsSincePrev = now - prevTime;
    if (secsSincePrev >= _params.realtime_wait_interval_secs) {
      if (_processRealtime(now)) {
        iret = -1;
      }
      prevTime = now;
    }
    umsleep(100);

  } // while

  return iret;

}

//////////////////////////////////////////////////
// Run in archive mode

int HcrTempRxGain::_runArchive()
{

  int iret = 0;

  _archiveStartTime = DateTime::parseDateTime(_params.archive_start_time);
  if (_archiveStartTime == DateTime::NEVER) {
    cerr << "ERROR - HcrTempRxGain::_runArchive()" << endl;
    cerr << "  Bad start time: " << _params.archive_start_time << endl;
    return -1;
  }

  _archiveEndTime = DateTime::parseDateTime(_params.archive_end_time);
  if (_archiveEndTime == DateTime::NEVER) {
    cerr << "ERROR - HcrTempRxGain::_runArchive()" << endl;
    cerr << "  Bad end time: " << _params.archive_end_time << endl;
    return -1;
  }

  time_t archiveTime = _archiveStartTime;
  while (archiveTime <= _archiveEndTime) {
    if (_processArchive(archiveTime)) {
      iret = -1;
    }
    archiveTime += _params.archive_processing_interval_secs;
  }

  return iret;

}

//////////////////////////////////////////////////
// Process realtime mode

int HcrTempRxGain::_processRealtime(time_t now)
{

  // get retrieval limits relative to now

  _retrieveStartTime = (now -
                        _params.v_lna_temperature_time_lag_secs -
                        _params.temperature_smoothing_interval_secs);

  _retrieveEndTime = (now + 
                      _params.v_lna_temperature_time_lag_secs +
                      _params.temperature_smoothing_interval_secs);

  // retrieve data

  if (_retrieveFromSpdb()) {
    cerr << "ERROR - HcrTempRxGain::_processRealtime()" << endl;
    return -1;
  }

  // process for this time

  if (_processTime(now)) {
    cerr << "ERROR - HcrTempRxGain::_processRealtime()" << endl;
    return -1;
  }

  return 0;

}

//////////////////////////////////////////////////
// Process archive mode

int HcrTempRxGain::_processArchive(time_t archiveTime)
{
  
  // get retrieval limits relative to archive time of interest

  _retrieveStartTime = 
    archiveTime -
    _params.v_lna_temperature_time_lag_secs -
    _params.temperature_smoothing_interval_secs;

  _retrieveEndTime = 
    archiveTime +
    _params.archive_processing_interval_secs +
    _params.v_lna_temperature_time_lag_secs +
    _params.temperature_smoothing_interval_secs;
  
  // retrieve data

  if (_retrieveFromSpdb()) {
    cerr << "ERROR - HcrTempRxGain::_processArchive()" << endl;
    return -1;
  }

  // loop through the processing interval, one second at a time
  
  int iret = 0;
  for (time_t procTime = archiveTime; 
       procTime < archiveTime + _params.archive_processing_interval_secs;
       procTime++) {
  
    // process this second
    
    if (_processTime(procTime)) {
      cerr << "ERROR - HcrTempRxGain::_processRealtime()" << endl;
      iret = -1;
    }

  } // procTime

  return iret;

}

//////////////////////////////////////////////////
// Retrieve temp data from SPDB
// from _retrieveStartTime to _retrieveEndTime

int HcrTempRxGain::_retrieveFromSpdb()
  
{

  int iret = 0;

  // read XML status from spdb

  DsSpdb spdb;

  if (_params.debug) {
    cerr << "Retrieving XML status from spdb, url: " << _params.input_spdb_url << endl;
    cerr << "  _retrieveStartTime: " << DateTime::strm(_retrieveStartTime) << endl;
    cerr << "  _retrieveEndTime: " << DateTime::strm(_retrieveEndTime) << endl;
  }
  
  if (spdb.getInterval(_params.input_spdb_url,
                       _retrieveStartTime, _retrieveEndTime)) {
    if (_params.debug >= Params::DEBUG_VERBOSE) {
      cerr << "WARNING - HcrTempRxGain::_retrieveFromSpdb" << endl;
      cerr << "  Cannot read data from URL: " << _params.input_spdb_url << endl;
      cerr << "  startTime: " << DateTime::strm(_retrieveStartTime) << endl;
      cerr << "  endTime: " << DateTime::strm(_retrieveEndTime) << endl;
      cerr << spdb.getErrStr() << endl;
    }
    return -1;
  }

  // initialize vector for samples

  _samplesV.clear();
  for (time_t time = _retrieveStartTime; time <= _retrieveEndTime; time++) {
    TimeSample sample(_params);
    sample.setTime(time);
    _samplesV.push_back(sample);
  }
  
  // got chunks
  
  const vector<Spdb::chunk_t> &chunks = spdb.getChunks();
  if (chunks.size() < 1) {
    if (_params.debug >= Params::DEBUG_VERBOSE) {
      cerr << "WARNING - RadxPartRain::_retrieveSiteTempFromSpdb" << endl;
      cerr << "  No temp data from URL: " << _params.input_spdb_url << endl;
      cerr << "  startTime: " << DateTime::strm(_retrieveStartTime) << endl;
      cerr << "  endTime: " << DateTime::strm(_retrieveEndTime) << endl;
    }
    return -1;
  }

  // loop through chunks
  
  for (size_t ichunk = 0; ichunk < chunks.size(); ichunk++) {

    // create string from chunk, ensure null termination
    const Spdb::chunk_t &chunk = chunks[ichunk];
    string xml((char *) chunk.data, chunk.len - 1);
    if (_params.debug >= Params::DEBUG_EXTRA) {
      cerr << "================ XML STATUS =================" << endl;
      cerr << "Valid time: " << DateTime::strm(chunk.valid_time) << endl;
      cerr << xml << endl;
      cerr << "=============================================" << endl;
    }

    // create sample

    if (chunk.valid_time >= _retrieveStartTime &&
        chunk.valid_time <= _retrieveEndTime) {

      size_t isample = chunk.valid_time - _retrieveStartTime;
      TimeSample &sample = _samplesV[isample]; 

      // get receiver status block
      
      string rxStatus;
      if (TaXml::readString(xml, _params.receiver_status_tag, rxStatus)) {
        cerr << "ERROR - RadxPartRain::_retrieveSiteTempFromSpdb" << endl;
        cerr << "  No receiver status tag found: "
             << _params.receiver_status_tag << endl;
        cerr << "  xml: " << xml << endl;
        iret = -1;
      }
      
      // read in temps
      
      double lnaTemp;
      if (TaXml::readDouble(rxStatus, _params.v_lna_temperature_tag, lnaTemp)) {
        cerr << "ERROR - RadxPartRain::_retrieveSiteTempFromSpdb" << endl;
        cerr << "  No LNA temp tag found: "
             << _params.v_lna_temperature_tag << endl;
        cerr << "  rxStatus: " << rxStatus << endl;
        iret = -1;
      }
      
      sample.addLnaTempObs(lnaTemp);
      
      for (int ii = 0; ii < _params.pod_temperature_tags_n; ii++) {
        string podTempTag = _params._pod_temperature_tags[ii];
       double podTemp;
        if (TaXml::readDouble(rxStatus, podTempTag, podTemp)) {
          cerr << "ERROR - RadxPartRain::_retrieveSiteTempFromSpdb" << endl;
          cerr << "  No POD temp tag found: "
               << podTempTag << endl;
          cerr << "  rxStatus: " << rxStatus << endl;
          iret = -1;
        }
        sample.addRxTempObs(podTemp);
      } // ii

    } // if (chunk.valid_time >= _retrieveStartTime ...
    
  } // ichunk

  // compute mean temps for each time
  
  for (size_t isample = 0; isample < _samplesV.size(); isample++) {
    TimeSample &sample = _samplesV[isample]; 
    sample.computeMeanObs();
  }

  return iret;

}

//////////////////////////////////////////////////
// Process the data for the specified time

int HcrTempRxGain::_processTime(time_t procTime)

{

  if (_params.debug >= Params::DEBUG_VERBOSE) {
    cerr << "==>> Processing retrieved data for time: " 
         << DateTime::strm(procTime) << endl;
  }

  // get time indices for sample vector

  int procIndex = procTime - _retrieveStartTime;
  TimeSample &procSampleV = _samplesV[procIndex]; 

  // get start and end of smoothing period for pod temps
  
  int smoothingIntervalHalf = _params.temperature_smoothing_interval_secs / 2;

  int smoothingStartIndex = procIndex - smoothingIntervalHalf;
  if (smoothingStartIndex < 0) {
    smoothingStartIndex = 0;
  }

  int smoothingEndIndex = procIndex + smoothingIntervalHalf;
  if (smoothingEndIndex > (int) _samplesV.size() - 1) {
    smoothingEndIndex = _samplesV.size() - 1;
  }

  // get start and end of lagged smoothing period for lna temps

  int lnaLagSecs = _params.v_lna_temperature_time_lag_secs;

  int laggedStartIndex = procIndex - smoothingIntervalHalf - lnaLagSecs;
  if (laggedStartIndex < 0) {
    laggedStartIndex = 0;
  }

  int laggedEndIndex = procIndex + smoothingIntervalHalf - lnaLagSecs;
  if (laggedEndIndex > (int) _samplesV.size() - 1) {
    laggedEndIndex = _samplesV.size() - 1;
  }

  // compute smoothed pod temp

  {
    double sum = 0.0;
    double nn = 0.0;
    for (int isample = smoothingStartIndex; isample <= smoothingEndIndex; isample++) {
      TimeSample &sample = _samplesV[isample];
      nn += sample.getRxTempN();
      sum += sample.getRxTempSum();
    }
    if (nn > 0) {
      double smoothedTemp = sum / nn;
      procSampleV.setRxSmoothedN(nn);
      procSampleV.setRxTempSmoothed(smoothedTemp);
    }
  }

  // compute smoothed lna temp, taking lag into account

  {
    double sum = 0.0;
    double nn = 0.0;
    for (int isample = laggedStartIndex; isample <= laggedEndIndex; isample++) {
      TimeSample &sample = _samplesV[isample];
      nn += sample.getLnaTempN();
      sum += sample.getLnaTempSum();
    }
    if (nn > 0) {
      double smoothedTemp = sum / nn;
      procSampleV.setLnaSmoothedN(nn);
      procSampleV.setLnaTempSmoothed(smoothedTemp);
    }
  }

  // compute delta gain

  procSampleV.computeDeltaGain(_params.v_lna_reference_temperature_c,
                              _params.v_lna_gain_change_per_c,
                              _params.pod_reference_temperature_c,
                              _params.v_rx_gain_change_per_c);
  
  // debug print
  
  if (_params.debug >= Params::DEBUG_VERBOSE) {
    
    cerr << "==>> temps for time    : "
         << DateTime::strm(procSampleV.getTime()) << endl ;
    cerr << "     N obs LNA         : " << procSampleV.getLnaTempN() << endl;
    cerr << "     N obs POD         : " << procSampleV.getRxTempN() << endl;
    cerr << "     mean temp LNA     : " << procSampleV.getLnaTempMean() << endl;
    cerr << "     mean temp POD     : " << procSampleV.getRxTempMean() << endl;
    cerr << "     smoothed N LNA    : " << procSampleV.getLnaSmoothedN() << endl;
    cerr << "     smoothed N POD    : " << procSampleV.getRxSmoothedN() << endl;
    cerr << "     smoothed temp LNA : " << procSampleV.getLnaTempSmoothed() << endl;
    cerr << "     smoothed temp POD : " << procSampleV.getRxTempSmoothed() << endl;
    cerr << "     delta gain LNA    : " << procSampleV.getLnaDeltaGain() << endl;
    cerr << "     delta gain RX     : " << procSampleV.getRxDeltaGain() << endl;
    cerr << "     delta gain SUM    : " << procSampleV.getSumDeltaGain() << endl;

  }

  // write the gain results to SPBD

  _writeToSpdb(procTime, procSampleV);

  return 0;

}

//////////////////////////////////////////////////
// Write gain results to SPDB

int HcrTempRxGain::_writeToSpdb(time_t procTime,
                                const TimeSample &procSampleV)

{

  // create XML string with results

  string xml;

  xml += TaXml::writeStartTag("HcrTempGainCorrection", 0);
  
  xml += TaXml::writeString("time", 1, DateTime::strm(procTime));
  
  xml += TaXml::writeDouble("v_lna_ref_temp_c", 1,
                            _params.v_lna_reference_temperature_c);
  xml += TaXml::writeDouble("pod_ref_temp_c", 1,
                            _params.pod_reference_temperature_c);
  
  xml += TaXml::writeDouble("v_lna_gain_change_per_c", 1,
                            _params.v_lna_gain_change_per_c);
  xml += TaXml::writeDouble("rx_gain_change_per_c", 1,
                            _params.v_rx_gain_change_per_c);
  
  xml += TaXml::writeDouble("v_lna_temp_time_lag_secs", 1,
                            _params.v_lna_temperature_time_lag_secs);
  
  xml += TaXml::writeDouble("temp_smoothing_secs", 1,
                            _params.temperature_smoothing_interval_secs);
  
  xml += TaXml::writeDouble("v_lna_temp", 1, procSampleV.getLnaTempMean());
  xml += TaXml::writeDouble("pod_temp", 1, procSampleV.getRxTempMean());
  
  xml += TaXml::writeDouble("v_lna_smoothed_temp", 1, 
                            procSampleV.getLnaTempSmoothed());
  xml += TaXml::writeDouble("pod_smoothed_temp", 1, 
                            procSampleV.getRxTempSmoothed());
  
  xml += TaXml::writeDouble("v_lna_smoothed_n", 1, 
                            procSampleV.getLnaSmoothedN());
  xml += TaXml::writeDouble("pod_smoothed_n", 1, 
                            procSampleV.getRxSmoothedN());
  
  xml += TaXml::writeDouble("v_lna_delta_gain", 1, 
                            procSampleV.getLnaDeltaGain());
  xml += TaXml::writeDouble("rx_delta_gain", 1, 
                            procSampleV.getRxDeltaGain());
  xml += TaXml::writeDouble("v_delta_gain", 1, 
                            procSampleV.getSumDeltaGain());
  
  xml += TaXml::writeEndTag("HcrTempGainCorrection", 0);
  
  if (_params.debug >= Params::DEBUG_VERBOSE) {
    cerr << "Writing gain results to SPDB, url: "
         << _params.output_spdb_url << endl;
  }
  
  DsSpdb spdb;
  time_t validTime = procTime;
  spdb.addPutChunk(0, validTime, validTime, xml.size() + 1, xml.c_str());
  if (spdb.put(_params.output_spdb_url,
               SPDB_XML_ID, SPDB_XML_LABEL)) {
    cerr << "ERROR - HcrTempRxGain::_writeToSpdb" << endl;
    cerr << spdb.getErrStr() << endl;
    return -1;
  }
  
  if (_params.debug) {
    cerr << "Wrote gain results to spdb, url: " 
         << _params.output_spdb_url << endl;
  }
  if (_params.debug >= Params::DEBUG_VERBOSE) {
    cerr << "=====================================" << endl;
    cerr << xml;
    cerr << "=====================================" << endl;
  }

  return 0;

}


