
/*
 * se_cpy_field
 * se_dir
 * se_establish_field
 * se_funfold
 * se_hard_zap
 * se_once_only
 * se_readin_cmds
 * se_threshold_field
 *  
 *  
 *  
 */

/*
"    forced-unfolding in <field> around <real>",
"!  Replace angle brackets and argument types with appropriate arguments.",
" ",
"!  Forces all data points to fall within plus or minus the Nyquist",
"!  Velocity of the <real> value.",
*/


/* c------------------------------------------------------------------------ */

/* #forced-unfolding# */

int se_funfold(const float *data, float *newData, size_t nGates,
	       float nyquist_velocity, float dds_radd_eff_unamb_vel,
	       float azimuth_angle_degrees, float elevation_angle_degrees,
	       float ew_wind, float ns_wind, float ud_wind,
	       int max_pos_folds, int max_neg_folds,
	       size_t ngates_averaged,
	       float center,
	       float bad_data_value, size_t dgi_clip_gate, 
	       bool *boundary_mask)
{
    int below;
    char *name;
    float ctr, nyqv, rcp_nyqi;

    size_t nc;
    int nd, nchar, bad, thr_bad, fn, fgg, abs_diff;
    int gg, ii, jj, kk, nn, scaled_ctr, mark, scaled_nyqv, scaled_nyqi, idiff;
    short *anchor;
    const float *ss, *zz;
    float *tt;
    short *thr=NULL;
    bool *bnd;

    nyqv = seds_nyquist_velocity ? seds_nyquist_velocity
	  : dds_radd_eff_unamb_vel;
    if(nyqv <=0)
      throw "ERROR, nyquist velocity must be greater than zero";

    ctr = center; // (cmdq++)->uc_v.us_v_float;

    nc = dgi_clip_gate;
    bnd = boundary_mask;

    ss = data; // (short *)dds->qdat_ptrs[fn];
    zz = ss +nc;
    bad = bad_data_value;

    // convert nyquist velocity to integer
    scaled_nyqv = DD_SCALE(nyqv);
    scaled_nyqi = 2*scaled_nyqv;
    rcp_nyqi = 1./(float)scaled_nyqi;

    // memcopy data into newData
    memcpy(newData, data, nGates*sizeof(float));
    tt = newData;

    //
    // loop through the data
    //

    for(; ss < zz; ss++,bnd++,tt++) {
	if(!(*bnd) || *ss == bad)
	      continue;

	if(abs(idiff = ctr - (*ss)) > scaled_nyqv) {
	    nn = idiff*rcp_nyqi + (idiff < 0 ? -.5 : .5);
	    *tt += nn*scaled_nyqi;
	    //	    *ss += nn*scaled_nyqi;
	}
    }
}  
