
#include <Solo/GeneralDefinitions.hh>

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
"    threshold <field> on <field> <below>|<above> <real>",
"!  Replace angle brackets and argument types with appropriate arguments.",
"  ",
"!  Two example commands are:",
"    threshold VT on NCP below 0.333",
"    threshold VT on SW above 5.0",
*/

/* #threshold# */

// parameters:
// first_good_gate 
//
void se_threshold_field(Where where, float scaled_thr, int first_good_gate,
			const float *data1, const float *data2, size_t nGates,
			float *newData,
			float bad, size_t dgi_clip_gate,
			bool *boundary_mask, bool *bad_flag_mask) 
{
  //int below;
    char *thr_name, *dst_name, *where;
    float what, what2;

    int nc, nd, nchar, bad, thr_bad, fn, fgg;
    int gg, ii, jj, kk, nn, scaled_thr1, mark, fthr, scaled_thr2;
    short *anchor, *ss, *zz, *thr=NULL;
    bool *bnd;


    dst_name = (cmdq++)->uc_text;
    thr_name = (cmdq++)->uc_text;
    where =    (cmdq++)->uc_text;
    what = (cmdq++)->uc_v.us_v_float;
    if(cmdq->uc_ctype == UTT_VALUE)
	  what2 = (cmdq)->uc_v.us_v_float;
    //below = strstr(where, "below") ? YES : NO;
    nd = strlen(dst_name);
    nchar = strlen(thr_name);

    nc = dgi_clip_gate;
    fgg = first_good_gate;
    bnd = boundary_mask;
    /*
     * find the thr field
     */
    if((fthr = dd_find_field(dgi, thr_name)) < 0) {	
	/* thr field not found
	 */
	g_string_sprintfa
	  (gs_complaints, "Threshold field: %s not found\n", thr_name);
	seds->punt = YES;
	return(-1);
    }
# ifdef NEW_ALLOC_SCHEME
    thr = (short *)dds->qdat_ptrs[fthr];
# else
    thr = (short *)((char *)dds->rdat[fthr] + sizeof(struct paramdata_d));
# endif
    thr_bad = dds->parm[fthr]->bad_data;
    /*
     * find the field to be thresholded
     */
    if((fn = dd_find_field(dgi, dst_name)) < 0) {	
	/* field not found
	 */
	g_string_sprintfa
	  (gs_complaints, "Field to be thresholded: %s not found\n", dst_name);
	seds->punt = YES;
	return(-1);
    }
    strncpy(dgi->dds->parm[fn]->threshold_field, "          ", 8);
    strncpy(dgi->dds->parm[fn]->threshold_field, thr_name, nchar);
# ifdef NEW_ALLOC_SCHEME
    ss = (short *)dds->qdat_ptrs[fn];
# else
    ss = (short *)((char *)dds->rdat[fn] + sizeof(struct paramdata_d));
# endif
    zz = ss +nc;
    dgi->dds->parm[fn]->threshold_value = what;
    scaled_thr1 = DD_SCALE(what, dds->parm[fthr]->parameter_scale
			  , dds->parm[fthr]->parameter_bias);
    if(cmdq->uc_ctype == UTT_VALUE)
	  scaled_thr2 = DD_SCALE(what2, dds->parm[fthr]->parameter_scale
				 , dds->parm[fthr]->parameter_bias);
    //bad = dds->parm[fn]->bad_data;

    //
    // loop through the data
    //
    for(gg=0;  gg < fgg && ss < zz;  *ss = bad, gg++,ss++,thr++,bnd++);

    switch(where) {
    case BELOW:
      //    if(strncmp(where, "below", 3) == 0) {
	for(; ss < zz; ss++,thr++,bnd++) {
	    if(!(*bnd) || *ss == bad)
		  continue;
	    if(*thr == thr_bad || *thr < scaled_thr1) {
		*ss = bad;
	    }
	}
	//}
	break;
    case ABOVE:
      //    else if(strncmp(where, "above", 3) == 0) {
	for(; ss < zz; ss++,thr++,bnd++) {
	    if(!(*bnd) || *ss == bad)
		  continue;
	    if(*thr == thr_bad || *thr > scaled_thr1) {
		*ss = bad;
	    }
	}
	//}
	break;
    default:
      // between 
	if(cmdq->uc_ctype == UTT_VALUE) {
	    for(; ss < zz; ss++,thr++,bnd++) {
		if(!(*bnd) || *ss == bad)
		      continue;
		if(*thr == thr_bad ||
		   (*thr >= scaled_thr1 && *thr <= scaled_thr2)) {
		    *ss = bad;
		}
	    }
	}
    }

}  
