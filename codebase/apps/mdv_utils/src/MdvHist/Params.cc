// *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
// ** Copyright UCAR (c) 1992 - 2016
// ** University Corporation for Atmospheric Research(UCAR)
// ** National Center for Atmospheric Research(NCAR)
// ** Boulder, Colorado, USA
// *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
////////////////////////////////////////////
// Params.cc
//
// TDRP C++ code file for class 'Params'.
//
// Code for program MdvHist
//
// This file has been automatically
// generated by TDRP, do not modify.
//
/////////////////////////////////////////////

/**
 *
 * @file Params.cc
 *
 * @class Params
 *
 * This class is automatically generated by the Table
 * Driven Runtime Parameters (TDRP) system
 *
 * @note Source is automatically generated from
 *       paramdef file at compile time, do not modify
 *       since modifications will be overwritten.
 *
 *
 * @author Automatically generated
 *
 */
using namespace std;

#include "Params.hh"
#include <cstring>

  ////////////////////////////////////////////
  // Default constructor
  //

  Params::Params()

  {

    // zero out table

    memset(_table, 0, sizeof(_table));

    // zero out members

    memset(&_start_, 0, &_end_ - &_start_);

    // class name

    _className = "Params";

    // initialize table

    _init();

    // set members

    tdrpTable2User(_table, &_start_);

    _exitDeferred = false;

  }

  ////////////////////////////////////////////
  // Copy constructor
  //

  Params::Params(const Params& source)

  {

    // sync the source object

    source.sync();

    // zero out table

    memset(_table, 0, sizeof(_table));

    // zero out members

    memset(&_start_, 0, &_end_ - &_start_);

    // class name

    _className = "Params";

    // copy table

    tdrpCopyTable((TDRPtable *) source._table, _table);

    // set members

    tdrpTable2User(_table, &_start_);

    _exitDeferred = false;

  }

  ////////////////////////////////////////////
  // Destructor
  //

  Params::~Params()

  {

    // free up

    freeAll();

  }

  ////////////////////////////////////////////
  // Assignment
  //

  void Params::operator=(const Params& other)

  {

    // sync the other object

    other.sync();

    // free up any existing memory

    freeAll();

    // zero out table

    memset(_table, 0, sizeof(_table));

    // zero out members

    memset(&_start_, 0, &_end_ - &_start_);

    // copy table

    tdrpCopyTable((TDRPtable *) other._table, _table);

    // set members

    tdrpTable2User(_table, &_start_);

    _exitDeferred = other._exitDeferred;

  }

  ////////////////////////////////////////////
  // loadFromArgs()
  //
  // Loads up TDRP using the command line args.
  //
  // Check usage() for command line actions associated with
  // this function.
  //
  //   argc, argv: command line args
  //
  //   char **override_list: A null-terminated list of overrides
  //     to the parameter file.
  //     An override string has exactly the format of an entry
  //     in the parameter file itself.
  //
  //   char **params_path_p:
  //     If this is non-NULL, it is set to point to the path
  //     of the params file used.
  //
  //   bool defer_exit: normally, if the command args contain a 
  //      print or check request, this function will call exit().
  //      If defer_exit is set, such an exit is deferred and the
  //      private member _exitDeferred is set.
  //      Use exidDeferred() to test this flag.
  //
  //  Returns 0 on success, -1 on failure.
  //

  int Params::loadFromArgs(int argc, char **argv,
                           char **override_list,
                           char **params_path_p,
                           bool defer_exit)
  {
    int exit_deferred;
    if (_tdrpLoadFromArgs(argc, argv,
                          _table, &_start_,
                          override_list, params_path_p,
                          _className,
                          defer_exit, &exit_deferred)) {
      return (-1);
    } else {
      if (exit_deferred) {
        _exitDeferred = true;
      }
      return (0);
    }
  }

  ////////////////////////////////////////////
  // loadApplyArgs()
  //
  // Loads up TDRP using the params path passed in, and applies
  // the command line args for printing and checking.
  //
  // Check usage() for command line actions associated with
  // this function.
  //
  //   const char *param_file_path: the parameter file to be read in
  //
  //   argc, argv: command line args
  //
  //   char **override_list: A null-terminated list of overrides
  //     to the parameter file.
  //     An override string has exactly the format of an entry
  //     in the parameter file itself.
  //
  //   bool defer_exit: normally, if the command args contain a 
  //      print or check request, this function will call exit().
  //      If defer_exit is set, such an exit is deferred and the
  //      private member _exitDeferred is set.
  //      Use exidDeferred() to test this flag.
  //
  //  Returns 0 on success, -1 on failure.
  //

  int Params::loadApplyArgs(const char *params_path,
                            int argc, char **argv,
                            char **override_list,
                            bool defer_exit)
  {
    int exit_deferred;
    if (tdrpLoadApplyArgs(params_path, argc, argv,
                          _table, &_start_,
                          override_list,
                          _className,
                          defer_exit, &exit_deferred)) {
      return (-1);
    } else {
      if (exit_deferred) {
        _exitDeferred = true;
      }
      return (0);
    }
  }

  ////////////////////////////////////////////
  // isArgValid()
  // 
  // Check if a command line arg is a valid TDRP arg.
  //

  bool Params::isArgValid(const char *arg)
  {
    return (tdrpIsArgValid(arg));
  }

  ////////////////////////////////////////////
  // load()
  //
  // Loads up TDRP for a given class.
  //
  // This version of load gives the programmer the option to load
  // up more than one class for a single application. It is a
  // lower-level routine than loadFromArgs, and hence more
  // flexible, but the programmer must do more work.
  //
  //   const char *param_file_path: the parameter file to be read in.
  //
  //   char **override_list: A null-terminated list of overrides
  //     to the parameter file.
  //     An override string has exactly the format of an entry
  //     in the parameter file itself.
  //
  //   expand_env: flag to control environment variable
  //               expansion during tokenization.
  //               If TRUE, environment expansion is set on.
  //               If FALSE, environment expansion is set off.
  //
  //  Returns 0 on success, -1 on failure.
  //

  int Params::load(const char *param_file_path,
                   char **override_list,
                   int expand_env, int debug)
  {
    if (tdrpLoad(param_file_path,
                 _table, &_start_,
                 override_list,
                 expand_env, debug)) {
      return (-1);
    } else {
      return (0);
    }
  }

  ////////////////////////////////////////////
  // loadFromBuf()
  //
  // Loads up TDRP for a given class.
  //
  // This version of load gives the programmer the option to
  // load up more than one module for a single application,
  // using buffers which have been read from a specified source.
  //
  //   const char *param_source_str: a string which describes the
  //     source of the parameter information. It is used for
  //     error reporting only.
  //
  //   char **override_list: A null-terminated list of overrides
  //     to the parameter file.
  //     An override string has exactly the format of an entry
  //     in the parameter file itself.
  //
  //   const char *inbuf: the input buffer
  //
  //   int inlen: length of the input buffer
  //
  //   int start_line_num: the line number in the source which
  //     corresponds to the start of the buffer.
  //
  //   expand_env: flag to control environment variable
  //               expansion during tokenization.
  //               If TRUE, environment expansion is set on.
  //               If FALSE, environment expansion is set off.
  //
  //  Returns 0 on success, -1 on failure.
  //

  int Params::loadFromBuf(const char *param_source_str,
                          char **override_list,
                          const char *inbuf, int inlen,
                          int start_line_num,
                          int expand_env, int debug)
  {
    if (tdrpLoadFromBuf(param_source_str,
                        _table, &_start_,
                        override_list,
                        inbuf, inlen, start_line_num,
                        expand_env, debug)) {
      return (-1);
    } else {
      return (0);
    }
  }

  ////////////////////////////////////////////
  // loadDefaults()
  //
  // Loads up default params for a given class.
  //
  // See load() for more detailed info.
  //
  //  Returns 0 on success, -1 on failure.
  //

  int Params::loadDefaults(int expand_env)
  {
    if (tdrpLoad(NULL,
                 _table, &_start_,
                 NULL, expand_env, FALSE)) {
      return (-1);
    } else {
      return (0);
    }
  }

  ////////////////////////////////////////////
  // sync()
  //
  // Syncs the user struct data back into the parameter table,
  // in preparation for printing.
  //
  // This function alters the table in a consistent manner.
  // Therefore it can be regarded as const.
  //

  void Params::sync(void) const
  {
    tdrpUser2Table(_table, (char *) &_start_);
  }

  ////////////////////////////////////////////
  // print()
  // 
  // Print params file
  //
  // The modes supported are:
  //
  //   PRINT_SHORT:   main comments only, no help or descriptions
  //                  structs and arrays on a single line
  //   PRINT_NORM:    short + descriptions and help
  //   PRINT_LONG:    norm  + arrays and structs expanded
  //   PRINT_VERBOSE: long  + private params included
  //

  void Params::print(FILE *out, tdrp_print_mode_t mode)
  {
    tdrpPrint(out, _table, _className, mode);
  }

  ////////////////////////////////////////////
  // checkAllSet()
  //
  // Return TRUE if all set, FALSE if not.
  //
  // If out is non-NULL, prints out warning messages for those
  // parameters which are not set.
  //

  int Params::checkAllSet(FILE *out)
  {
    return (tdrpCheckAllSet(out, _table, &_start_));
  }

  //////////////////////////////////////////////////////////////
  // checkIsSet()
  //
  // Return TRUE if parameter is set, FALSE if not.
  //
  //

  int Params::checkIsSet(const char *paramName)
  {
    return (tdrpCheckIsSet(paramName, _table, &_start_));
  }

  ////////////////////////////////////////////
  // freeAll()
  //
  // Frees up all TDRP dynamic memory.
  //

  void Params::freeAll(void)
  {
    tdrpFreeAll(_table, &_start_);
  }

  ////////////////////////////////////////////
  // usage()
  //
  // Prints out usage message for TDRP args as passed
  // in to loadFromArgs().
  //

  void Params::usage(ostream &out)
  {
    out << "TDRP args: [options as below]\n"
        << "   [ -params/--params path ] specify params file path\n"
        << "   [ -check_params/--check_params] check which params are not set\n"
        << "   [ -print_params/--print_params [mode]] print parameters\n"
        << "     using following modes, default mode is 'norm'\n"
        << "       short:   main comments only, no help or descr\n"
        << "                structs and arrays on a single line\n"
        << "       norm:    short + descriptions and help\n"
        << "       long:    norm  + arrays and structs expanded\n"
        << "       verbose: long  + private params included\n"
        << "       short_expand:   short with env vars expanded\n"
        << "       norm_expand:    norm with env vars expanded\n"
        << "       long_expand:    long with env vars expanded\n"
        << "       verbose_expand: verbose with env vars expanded\n"
        << "   [ -tdrp_debug] debugging prints for tdrp\n"
        << "   [ -tdrp_usage] print this usage\n";
  }

  ////////////////////////////////////////////
  // arrayRealloc()
  //
  // Realloc 1D array.
  //
  // If size is increased, the values from the last array 
  // entry is copied into the new space.
  //
  // Returns 0 on success, -1 on error.
  //

  int Params::arrayRealloc(const char *param_name, int new_array_n)
  {
    if (tdrpArrayRealloc(_table, &_start_,
                         param_name, new_array_n)) {
      return (-1);
    } else {
      return (0);
    }
  }

  ////////////////////////////////////////////
  // array2DRealloc()
  //
  // Realloc 2D array.
  //
  // If size is increased, the values from the last array 
  // entry is copied into the new space.
  //
  // Returns 0 on success, -1 on error.
  //

  int Params::array2DRealloc(const char *param_name,
                             int new_array_n1,
                             int new_array_n2)
  {
    if (tdrpArray2DRealloc(_table, &_start_, param_name,
                           new_array_n1, new_array_n2)) {
      return (-1);
    } else {
      return (0);
    }
  }

  ////////////////////////////////////////////
  // _init()
  //
  // Class table initialization function.
  //
  //

  void Params::_init()

  {

    TDRPtable *tt = _table;

    // Parameter 'Comment 0'
    
    memset(tt, 0, sizeof(TDRPtable));
    tt->ptype = COMMENT_TYPE;
    tt->param_name = tdrpStrDup("Comment 0");
    tt->comment_hdr = tdrpStrDup("MdvHist does simple histograms on MDV formatted gridded\nfield data.\n\nThe histogram is defined by the numBins, binStart and binWidth\nparameters. Only those points that lie within the lat/lon\ndomain defined by the latMin, latMax, lonMin and lonMax parameters\nare considered in the histogram. There is no vertical differentiation\n(it would be fairly easy to add).\n\nThe output is printed to standard out. Debugging messages are printed\nto standard error. The output is ASCII columbs of numbers with the\nfollowing meanings :\nThe year, month, day, hour, minute and second of the MDV data, and\nThe number of non-missing points found in the domain,\nThe number of non-missing points that did not lie in any of the\n      histogram bins,\nThe counts from the histogram bins themselves. The lower limit on\nthe histogram is inclusive, the upper is exclusive.\n\nThe -yesterday option can be used to process yesterday's data,\nuseful for cron-driven post processing.\n\nIf file output is selected and archive data are run, an ASCII\nfile is output that has cumulative histogram data for the\nentire data set over the specified archive time.\n\nNiles Oien, August 2003");
    tt->comment_text = tdrpStrDup("");
    tt++;
    
    // Parameter 'TriggerUrl'
    // ctype is 'char*'
    
    memset(tt, 0, sizeof(TDRPtable));
    tt->ptype = STRING_TYPE;
    tt->param_name = tdrpStrDup("TriggerUrl");
    tt->descr = tdrpStrDup("URL to trigger off, typically of the form\nmdvp:://host.domain:[port]:dir/dir");
    tt->help = tdrpStrDup("Program runs on data from here.");
    tt->val_offset = (char *) &TriggerUrl - &_start_;
    tt->single_val.s = tdrpStrDup("mdvp:://localhost::mdv/input");
    tt++;
    
    // Parameter 'Instance'
    // ctype is 'char*'
    
    memset(tt, 0, sizeof(TDRPtable));
    tt->ptype = STRING_TYPE;
    tt->param_name = tdrpStrDup("Instance");
    tt->descr = tdrpStrDup("Instance to check in to Procmap with");
    tt->help = tdrpStrDup("Set to something unique.");
    tt->val_offset = (char *) &Instance - &_start_;
    tt->single_val.s = tdrpStrDup("primary");
    tt++;
    
    // Parameter 'Mode'
    // ctype is '_mode'
    
    memset(tt, 0, sizeof(TDRPtable));
    tt->ptype = ENUM_TYPE;
    tt->param_name = tdrpStrDup("Mode");
    tt->descr = tdrpStrDup("Operation mode");
    tt->help = tdrpStrDup("Archive mode automatically invoked by -interval option.");
    tt->val_offset = (char *) &Mode - &_start_;
    tt->enum_def.name = tdrpStrDup("mode");
    tt->enum_def.nfields = 2;
    tt->enum_def.fields = (enum_field_t *)
        tdrpMalloc(tt->enum_def.nfields * sizeof(enum_field_t));
      tt->enum_def.fields[0].name = tdrpStrDup("ARCHIVE");
      tt->enum_def.fields[0].val = ARCHIVE;
      tt->enum_def.fields[1].name = tdrpStrDup("REALTIME");
      tt->enum_def.fields[1].val = REALTIME;
    tt->single_val.e = REALTIME;
    tt++;
    
    // Parameter 'MaxRealtimeValidAge'
    // ctype is 'int'
    
    memset(tt, 0, sizeof(TDRPtable));
    tt->ptype = INT_TYPE;
    tt->param_name = tdrpStrDup("MaxRealtimeValidAge");
    tt->descr = tdrpStrDup("Maximum valid age for files");
    tt->help = tdrpStrDup("REALTIME mode only.");
    tt->val_offset = (char *) &MaxRealtimeValidAge - &_start_;
    tt->single_val.i = 1800;
    tt++;
    
    // Parameter 'Debug'
    // ctype is 'tdrp_bool_t'
    
    memset(tt, 0, sizeof(TDRPtable));
    tt->ptype = BOOL_TYPE;
    tt->param_name = tdrpStrDup("Debug");
    tt->descr = tdrpStrDup("Set to print debugging messages.");
    tt->help = tdrpStrDup("Defaults to FALSE");
    tt->val_offset = (char *) &Debug - &_start_;
    tt->single_val.b = pFALSE;
    tt++;
    
    // Parameter 'FieldName'
    // ctype is 'char*'
    
    memset(tt, 0, sizeof(TDRPtable));
    tt->ptype = STRING_TYPE;
    tt->param_name = tdrpStrDup("FieldName");
    tt->descr = tdrpStrDup("Input field name");
    tt->help = tdrpStrDup("Histograms will be generated for these data.");
    tt->val_offset = (char *) &FieldName - &_start_;
    tt->single_val.s = tdrpStrDup("DBZ");
    tt++;
    
    // Parameter 'latMin'
    // ctype is 'double'
    
    memset(tt, 0, sizeof(TDRPtable));
    tt->ptype = DOUBLE_TYPE;
    tt->param_name = tdrpStrDup("latMin");
    tt->descr = tdrpStrDup("Minimum latitude of points to consider.");
    tt->help = tdrpStrDup("latMin, latMax, lonMin and lonMax define the domain.");
    tt->val_offset = (char *) &latMin - &_start_;
    tt->single_val.d = -90;
    tt++;
    
    // Parameter 'latMax'
    // ctype is 'double'
    
    memset(tt, 0, sizeof(TDRPtable));
    tt->ptype = DOUBLE_TYPE;
    tt->param_name = tdrpStrDup("latMax");
    tt->descr = tdrpStrDup("Maximum latitude of points to consider.");
    tt->help = tdrpStrDup("latMin, latMax, lonMin and lonMax define the domain.");
    tt->val_offset = (char *) &latMax - &_start_;
    tt->single_val.d = 90;
    tt++;
    
    // Parameter 'lonMin'
    // ctype is 'double'
    
    memset(tt, 0, sizeof(TDRPtable));
    tt->ptype = DOUBLE_TYPE;
    tt->param_name = tdrpStrDup("lonMin");
    tt->descr = tdrpStrDup("Minimum longitude of points to consider.");
    tt->help = tdrpStrDup("latMin, latMax, lonMin and lonMax define the domain.");
    tt->val_offset = (char *) &lonMin - &_start_;
    tt->single_val.d = -180;
    tt++;
    
    // Parameter 'lonMax'
    // ctype is 'double'
    
    memset(tt, 0, sizeof(TDRPtable));
    tt->ptype = DOUBLE_TYPE;
    tt->param_name = tdrpStrDup("lonMax");
    tt->descr = tdrpStrDup("Maximum longitude of points to consider.");
    tt->help = tdrpStrDup("latMin, latMax, lonMin and lonMax define the domain.");
    tt->val_offset = (char *) &lonMax - &_start_;
    tt->single_val.d = 180;
    tt++;
    
    // Parameter 'applyVerticalLimits'
    // ctype is 'tdrp_bool_t'
    
    memset(tt, 0, sizeof(TDRPtable));
    tt->ptype = BOOL_TYPE;
    tt->param_name = tdrpStrDup("applyVerticalLimits");
    tt->descr = tdrpStrDup("Flag indicating whether to apply vertical limits to the input.");
    tt->help = tdrpStrDup("If set to TRUE, only the vertical levels between minVerticalLevel and maxVerticalLevel will be processed.");
    tt->val_offset = (char *) &applyVerticalLimits - &_start_;
    tt->single_val.b = pFALSE;
    tt++;
    
    // Parameter 'minVerticalLevel'
    // ctype is 'long'
    
    memset(tt, 0, sizeof(TDRPtable));
    tt->ptype = LONG_TYPE;
    tt->param_name = tdrpStrDup("minVerticalLevel");
    tt->descr = tdrpStrDup("Minimum vertical level number to process");
    tt->help = tdrpStrDup("Only used if applyVerticalLimits is set to TRUE.");
    tt->val_offset = (char *) &minVerticalLevel - &_start_;
    tt->single_val.l = 0;
    tt++;
    
    // Parameter 'maxVerticalLevel'
    // ctype is 'long'
    
    memset(tt, 0, sizeof(TDRPtable));
    tt->ptype = LONG_TYPE;
    tt->param_name = tdrpStrDup("maxVerticalLevel");
    tt->descr = tdrpStrDup("Maximum vertical level number to process");
    tt->help = tdrpStrDup("Only used if applyVerticalLimits is set to TRUE.");
    tt->val_offset = (char *) &maxVerticalLevel - &_start_;
    tt->single_val.l = 0;
    tt++;
    
    // Parameter 'binStart'
    // ctype is 'double'
    
    memset(tt, 0, sizeof(TDRPtable));
    tt->ptype = DOUBLE_TYPE;
    tt->param_name = tdrpStrDup("binStart");
    tt->descr = tdrpStrDup("Lower limit of first bin.");
    tt->help = tdrpStrDup("binStart, binWidth and numBins define the histograms.");
    tt->val_offset = (char *) &binStart - &_start_;
    tt->single_val.d = 0;
    tt++;
    
    // Parameter 'binWidth'
    // ctype is 'double'
    
    memset(tt, 0, sizeof(TDRPtable));
    tt->ptype = DOUBLE_TYPE;
    tt->param_name = tdrpStrDup("binWidth");
    tt->descr = tdrpStrDup("Bin width");
    tt->help = tdrpStrDup("binStart, binWidth and numBins define the histograms.");
    tt->val_offset = (char *) &binWidth - &_start_;
    tt->single_val.d = 1;
    tt++;
    
    // Parameter 'numBins'
    // ctype is 'int'
    
    memset(tt, 0, sizeof(TDRPtable));
    tt->ptype = INT_TYPE;
    tt->param_name = tdrpStrDup("numBins");
    tt->descr = tdrpStrDup("Number of bins.");
    tt->help = tdrpStrDup("binStart, binWidth and numBins define the histograms.");
    tt->val_offset = (char *) &numBins - &_start_;
    tt->single_val.i = 30;
    tt++;
    
    // Parameter 'fileOut'
    // ctype is 'tdrp_bool_t'
    
    memset(tt, 0, sizeof(TDRPtable));
    tt->ptype = BOOL_TYPE;
    tt->param_name = tdrpStrDup("fileOut");
    tt->descr = tdrpStrDup("Set to output to files instead of to stdout.");
    tt->help = tdrpStrDup("Defaults to FALSE, but can be useful.");
    tt->val_offset = (char *) &fileOut - &_start_;
    tt->single_val.b = pFALSE;
    tt++;
    
    // Parameter 'outDir'
    // ctype is 'char*'
    
    memset(tt, 0, sizeof(TDRPtable));
    tt->ptype = STRING_TYPE;
    tt->param_name = tdrpStrDup("outDir");
    tt->descr = tdrpStrDup("Output directory.");
    tt->help = tdrpStrDup("Relevant if fileOutput is TRUE. Output files are written here.");
    tt->val_offset = (char *) &outDir - &_start_;
    tt->single_val.s = tdrpStrDup("./MdvHistOut");
    tt++;
    
    // Parameter 'outputDateSub'
    // ctype is 'tdrp_bool_t'
    
    memset(tt, 0, sizeof(TDRPtable));
    tt->ptype = BOOL_TYPE;
    tt->param_name = tdrpStrDup("outputDateSub");
    tt->descr = tdrpStrDup("Option to output files to YYYYMMDD directory under the outDir directory.");
    tt->help = tdrpStrDup("Relevant if fileOutput is TRUE. Not used for Total files");
    tt->val_offset = (char *) &outputDateSub - &_start_;
    tt->single_val.b = pFALSE;
    tt++;
    
    // Parameter 'outputIndividualFiles'
    // ctype is 'tdrp_bool_t'
    
    memset(tt, 0, sizeof(TDRPtable));
    tt->ptype = BOOL_TYPE;
    tt->param_name = tdrpStrDup("outputIndividualFiles");
    tt->descr = tdrpStrDup("Output data for individual files.");
    tt->help = tdrpStrDup("");
    tt->val_offset = (char *) &outputIndividualFiles - &_start_;
    tt->single_val.b = pTRUE;
    tt++;
    
    // Parameter 'individualOutputType'
    // ctype is '_ind_output_type_t'
    
    memset(tt, 0, sizeof(TDRPtable));
    tt->ptype = ENUM_TYPE;
    tt->param_name = tdrpStrDup("individualOutputType");
    tt->descr = tdrpStrDup("Output type for the individual output files.");
    tt->help = tdrpStrDup("Used only if outputIndividualFiles is set to true.");
    tt->val_offset = (char *) &individualOutputType - &_start_;
    tt->enum_def.name = tdrpStrDup("ind_output_type_t");
    tt->enum_def.nfields = 2;
    tt->enum_def.fields = (enum_field_t *)
        tdrpMalloc(tt->enum_def.nfields * sizeof(enum_field_t));
      tt->enum_def.fields[0].name = tdrpStrDup("OUTPUT_PLAIN");
      tt->enum_def.fields[0].val = OUTPUT_PLAIN;
      tt->enum_def.fields[1].name = tdrpStrDup("OUTPUT_FORMATTED");
      tt->enum_def.fields[1].val = OUTPUT_FORMATTED;
    tt->single_val.e = OUTPUT_PLAIN;
    tt++;
    
    // Parameter 'outputTotalFile'
    // ctype is 'tdrp_bool_t'
    
    memset(tt, 0, sizeof(TDRPtable));
    tt->ptype = BOOL_TYPE;
    tt->param_name = tdrpStrDup("outputTotalFile");
    tt->descr = tdrpStrDup("Output total data in a file.");
    tt->help = tdrpStrDup("");
    tt->val_offset = (char *) &outputTotalFile - &_start_;
    tt->single_val.b = pFALSE;
    tt++;
    
    // Parameter 'forecastMode'
    // ctype is 'tdrp_bool_t'
    
    memset(tt, 0, sizeof(TDRPtable));
    tt->ptype = BOOL_TYPE;
    tt->param_name = tdrpStrDup("forecastMode");
    tt->descr = tdrpStrDup("Operate in forecast mode (forecast MDV files for\ninput, lead time in output filenames).");
    tt->help = tdrpStrDup("");
    tt->val_offset = (char *) &forecastMode - &_start_;
    tt->single_val.b = pFALSE;
    tt++;
    
    // trailing entry has param_name set to NULL
    
    tt->param_name = NULL;
    
    return;
  
  }
