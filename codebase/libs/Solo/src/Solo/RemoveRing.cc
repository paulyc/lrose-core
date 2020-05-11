
/* #remove-ring# */

/*
"    remove-ring in <field> from <real> to <real> km.",
" ",
"!  Replace angle brackets and argument types with appropriate arguments.",
"!  Inserts a bad data flag between the two indicated ranges.",
" ",
"!  e.g.",
"    remove-ring in VQ from 75.15 to 75.8 km.",
*/

void se_ring_zap(float from_km, float to_km, 
		 const float *data, float *newData, size_t nGates,
		 float bad, size_t dgi_clip_gate, bool *boundary_mask)
{
  // routine to remove a ring of data; e.g. a test pulse
  //
    float rr, r1, r2=1.e22;
    int ii, mark, gg, g1, g2;
    size_t nc;
    int nd, fn, a_speckle;
    const float *ss, *zz;
    float *tt;
    bool *bnd;

    r1 = KM_TO_M(from_km); // get it to meters 
    if(cmdq->uc_ctype == UTT_VALUE)
	  r2 = KM_TO_M(to_km);

    nc = dgi_clip_gate;
    if (dgi_clip_gate > nGates)
      nc = nGates;
    bnd = boundary_mask;
    //ss = data;
    //zz = ss +nc;

    // memcopy data into newData                          
    memcpy(newData, data, nGates*sizeof(float));

    tt = newData;

    // TODO: map the from_km and to_km to a cell index in the ray
    // Q:Does this need to happend in the calling function? and send
    // the gate/cell index?
    g1 = dd_cell_num(dgi->dds, 0, r1);
    g2 = dd_cell_num(dgi->dds, 0, r2) +1;

    //ss += g1;
    bnd += g1;
    tt += g1;

    //
    // loop through the data
    //

    for(; g1++ < g2; bnd++, tt++) {  // ss++ need ss ??
	if(!(*bnd))
	      continue;	
	*tt = bad; // *ss = bad;
    }
}
/* c------------------------------------------------------------------------ */

