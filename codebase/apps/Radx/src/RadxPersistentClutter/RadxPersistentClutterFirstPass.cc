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
/**
 * @file RadxPersistentClutterFirstPass.cc
 */

#include "RadxPersistentClutterFirstPass.hh"
#include "FrequencyCount.hh"
#include "Volume.hh"
#include <Radx/RadxRay.hh>
#include <toolsa/LogStream.hh>
#include <toolsa/Path.hh>
#include <toolsa/DateTime.hh>
#include <algorithm>

//------------------------------------------------------------------
// from the paper, mu(k) = sum of i*p[i] up to k
static double _mu(const vector<double> &p, const int k)
{
  double ret = 0.0;
  for (int i=0; i<=k; ++i)
  {
    ret += static_cast<double>(i+1)*p[i];
  }
  return ret;
}

//------------------------------------------------------------------
// from the paper, omega(k) = sum of p[i] up to k
static double _omega(const vector<double> &p, const int k)
{
  double ret = 0.0;
  for (int i=0; i<=k; ++i)
  {
    ret += p[i];
  }
  return ret;
}

//------------------------------------------------------------------
RadxPersistentClutterFirstPass::
RadxPersistentClutterFirstPass(const Parms &parms) :
  RadxPersistentClutter(parms, FIRST_PASS), _nvolume(0), _total_pixels(0),
  _kstar(0), _converged(false)
{
}

//------------------------------------------------------------------
RadxPersistentClutterFirstPass::~RadxPersistentClutterFirstPass(void)
{
  // create output ascii before calling it a day
  if (!_ascii_output.empty())
  {
    FILE *fp = fopen(_ascii_fname.c_str(), "w");
    if (fp == NULL)
    {
      // try to make the path
      Path path(_ascii_fname);
      path.makeDirRecurse();
      fp = fopen(_ascii_fname.c_str(), "w");
      if (fp == NULL)
      {
	LOG(ERROR) << "Cannot open file " << _ascii_fname;
      }
      else
      {
	for (size_t i=0; i<_ascii_output.size(); ++i)
	{
	  fprintf(fp, "%s\n", _ascii_output[i].c_str());
	}
	fclose(fp);
      }
    }
  }

  if (!_freq_output.empty())
  {
    FILE *fp = fopen(_freq_fname.c_str(), "w");
    if (fp == NULL)
    {
      // try to make the path
      Path path(_freq_fname);
      path.makeDirRecurse();
      fp = fopen(_freq_fname.c_str(), "w");
      if (fp == NULL)
      {
	LOG(ERROR) << "Cannot open file " << _freq_fname;
      }
      else
      {
	for (size_t i=0; i<_freq_output.size(); ++i)
	{
	  fprintf(fp, "%s\n", _freq_output[i].c_str());
	}
	fclose(fp);
      }
    }
  }
}

//------------------------------------------------------------------
void RadxPersistentClutterFirstPass::initFirstTime(const Volume *vol)
{
  // build path
  _ascii_fname = _parms.output_ascii_path;
  _ascii_fname += "/scan";
  DateTime dt(vol->getTime());
  char buf[1000];
  sprintf(buf, "%04d-%02d-%02d", dt.getYear(), dt.getMonth(), dt.getDay());
  _ascii_fname += buf;
  _ascii_fname += ".out";

  _ascii_output.clear();

  // build other path
  _freq_fname = _parms.output_ascii_path;
  _freq_fname += "/frequency";
  _freq_fname += buf;
  _freq_fname += ".out";

  _freq_output.clear();

  // set first time member value
  _first_t = vol->getTime();
}

//------------------------------------------------------------------
void RadxPersistentClutterFirstPass::finishLastTimeGood(Volume *vol)
{
  LOG(DEBUG) << "Finished, with convergence";

  // set last time member value
  _final_t = vol->getTime();
}

//------------------------------------------------------------------
void RadxPersistentClutterFirstPass::finishBad(void)
{
  LOG(WARNING) << "Never converged";
}

//------------------------------------------------------------------
bool RadxPersistentClutterFirstPass::processFinishVolume(Volume *vol)
{
  // Up the volume count
  ++_nvolume;

  // Compute k* value (see paper part II.)
  _kstar = _computeHistoCutoff();

  // output some ASCII stuff that can be graphed later
  _converged = _outputForGraphics(vol->getTime());

  if (_parms.diagnostic_output)
  {
    // prepare this volume for output, and return true indicating write 
    _processForOutput(vol);
    return true;
  }
  else
  {
    return false;
  }
}

//------------------------------------------------------------------
bool RadxPersistentClutterFirstPass::preProcessRay(const RadxRay &ray)
{
  // first time through, pull out values and initialize state
  double az = ray.getAzimuthDeg();
  double elev = ray.getElevationDeg();
  double x0 = ray.getStartRangeKm();
  double dx = ray.getGateSpacingKm();
  int nx = ray.getNGates();

  if (_processFirstRay(ray, az, elev, x0, dx, nx))
  {
    // add these pixels to the state count total
    _total_pixels += nx;
    return true;
  }
  else
  {
    return false;
  }
}

//------------------------------------------------------------------
bool RadxPersistentClutterFirstPass::processRay(const RayxData &r,
						RayClutterInfo *h) const
{
  // Update the input RayClutterInfo object using the RayxData
  return h->update(r, _parms.threshold);
}

//------------------------------------------------------------------
bool RadxPersistentClutterFirstPass::setRayForOutput(const RayClutterInfo *h,
						     const RayxData &r,
						     RadxRay &ray)

{
  // make a copy of the data to store binary values
  RayxData clutter(r);

  // make a copy of that data to store frequency data, and initialize
  RayxData rfreq(r);
  RadxApp::modifyRayForOutput(rfreq, "NormFreqCount", "none", -99.99);
  rfreq.setAllToValue(0.0);

  // set values in the cluter to 1 or 0
  if (h->equalOrExceed(clutter, _kstar))
  {
    // set the frequency values
    if (h->loadNormalizedFrequency(rfreq, _nvolume))
    {
      // prepare for output (put stuff into ray)
      RadxApp::modifyRayForOutput(clutter, _parms.output_field);
      vector<RayxData> vr;
      vr.push_back(r);
      vr.push_back(rfreq);
      RadxApp::updateRay(vr, ray);
      return true;
    }
  }
  return false;
}

//------------------------------------------------------------------
RayClutterInfo *
RadxPersistentClutterFirstPass::matchingClutterInfo(const double az,
						    const double elev)
{
  return matchInfo(_store, az, elev);
}

//------------------------------------------------------------------
const RayClutterInfo * RadxPersistentClutterFirstPass::
matchingClutterInfoConst(const double az, const double elev) const
{
  return matchInfoConst(_store, az, elev);
}

//------------------------------------------------------------------
bool RadxPersistentClutterFirstPass::isDone(void) const
{
  return _converged;
}

//------------------------------------------------------------------
bool RadxPersistentClutterFirstPass::_processFirstRay(const RadxRay &ray,
						      const double az,
						      const double elev,
						      const double x0,
						      const double dx,
						      const int nx)
{
  if (_rayMap.add(ray))
  {
    RadxAzElev ae = _rayMap.match(az, elev);
    if (_rayMap.isMulti(ae))
    {
      LOG(WARNING) << "Multiple az,elev in volume " << ae.sprint();
    }
    else
    {
      // start up a new RayHisto for this az, elev_match pair
      RayClutterInfo h(ae.getAz(), ae.getElev(), x0, dx, nx);
      _store[ae] = h;
    }
    return true;
  }
  else
  {
    LOG(ERROR) << "Elevation not configured within tolerance " << elev;
    return false;

  }
}

//------------------------------------------------------------------
int RadxPersistentClutterFirstPass::_computeHistoCutoff(void) const
{

  // compute p[i] for all i (# of points with exactly i scans indicating
  // clutter)
  vector<double> p;
  for (int i=0; i<=_nvolume; ++i)
  {
    double nn = _countOfScans(i);
    p.push_back(nn/_total_pixels);
  }

  // do the maximization as in the paper
  int T = static_cast<int>(p.size()-1);

  // mu at T
  double muT = _mu(p, T);

  int maxk = -1;
  double maxphisq = 0;

  for (int i=0; i<T; ++i)
  {
    double mu = _mu(p, i);
    double omega = _omega(p, i);
    double phisq = (muT*omega - mu)*(muT*omega - mu)/(omega*(1.0-omega));
    if (i == 0)
    {
      maxk = 0;
      maxphisq = phisq;
    }
    else
    {
      if (phisq > maxphisq)
      {
	maxphisq = phisq;
	maxk = i;
      }
    }
  }
  return maxk;
}

//------------------------------------------------------------------
bool
RadxPersistentClutterFirstPass::_outputForGraphics(const time_t &t)

{
  // create a FrequencyCount object
  FrequencyCount F(40, _nvolume);

  // Given _kstar, set the clutter yes/no state, and detect changed points
  int nchange = _updateClutterState(_kstar, F);

  // use time to build time value for output
  DateTime dt(t);
  int h = dt.getHour();
  int m = dt.getMin();
  double dm = static_cast<double>(m)/60.0;
  double mm = static_cast<double>(h) + dm;

  // percentage values for kstar and nchange = threshold and change%
  double thr = static_cast<double>(_kstar)/_nvolume;
  double ch = static_cast<double>(nchange)/_total_pixels;
  
  // write out time,threshold,change%
  char buf[1000];
  sprintf(buf, "%8.4lf %10.5lf %10.5lf", mm, thr, ch);
  _ascii_output.push_back(buf);
  // fprintf(_ascii_output, "%8.4lf %10.5lf %10.5lf\n", mm, thr, ch);

  // Write a line to the _freq_output file
  F.appendString(_freq_output);

  // store this times threshold and change percent values
  _threshold.push_back(thr);
  _change.push_back(ch);

  // see if things are stable, based on params, return true if so
  return _checkConvergence();
}

//------------------------------------------------------------------
bool RadxPersistentClutterFirstPass::_checkConvergence(void)
{
  // need at least a minimum number of volumes
  int n = static_cast<int>(_threshold.size());
  if (n < _parms.minimum_stable_volumes)
  {
    return false;
  }

  // get the last threshold
  double thresh = _threshold[n-1];
  double tave = 0.0;

  // see if at least the minimum number thresholds are all close to the last one
  // and that there is never a large change from one volume to the next
  for (int i = 0; i<_parms.minimum_stable_volumes; ++i)
  {
    int j = n - 1 - i;
    double thr = _threshold[j];
    double ch = _change[j];

    if (ch > _parms.maximum_percent_change)
    {
      return false;
    }

    if (fabs(thr - thresh) > _parms.threshold_tolerance)
    {
      return false;
    }
    tave += thr;
  }

  // it shows stability, we have converged.
  return true;
}
