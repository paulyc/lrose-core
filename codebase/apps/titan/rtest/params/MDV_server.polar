/****************************************************
 * TDRP params for MDV_server
 ****************************************************/

/*
 * Debug option.
 * If set, debug messages will be printed appropriately.
 *
 * Type: enum
 * Default: DEBUG_OFF 
 * Legal values: DEBUG_OFF, DEBUG_NORM, DEBUG_VERBOSE.
 */
debug = DEBUG_OFF;

/*
 * Malloc debug level.
 * 0 - none, 1 - corruption checking, 2 - records all malloc blocks
 *   and checks, 3 - printout of all mallocs etc.
 *
 * Type: long
 * Default: 0 
 * Min value: 0 
 * Max value: 3 
 */
malloc_debug_level = 0;

/*
 * Server subtype.
 * Used for registration with servmap.
 *
 * Type: string
 * Default: "MDV" 
 */
subtype = "MDV";

/*
 * Process and server instance.
 * Used for registration with procmap and servmap.
 *
 * Type: string
 * Default: "Test" 
 */
instance = "polar";

/*
 * Server info.
 * Used for registration with servmap.
 *
 * Type: string
 * Default: "Test" 
 */
info = "Test";

/*
 * Server port number.
 * TCP/IP port for this server.
 *
 * Type: long
 * Default: 43000 
 * Min value: 10000 
 */
port = 43910;

/*
 * Data directories.
 * List of directories holding the storm data.
 *
 * Type: string
 * Array elem size: 4 bytes
 * Array has no max number of elements
 */
data_dirs = {"$(RTEST_HOME)/polar_data"};

/*
 * Suffix on data files.
 *
 * Type: string
 * Default: "mdv" 
 */
data_file_suffix = "mdv";

/*
 * Real-time flag.
 * If set, indicates real-time data is available, shmem is created.
 *
 * Type: boolean
 * Default: FALSE 
 */
realtime_avail = FALSE;

/*
 * Option to use real-time file.
 * If set, servers uses a single file for GET_NEW requests.
 *
 * Type: boolean
 * Default: FALSE 
 */
use_realtime_file = TRUE;

/*
 * Path to realtime file.
 * See use_realtime_file.
 *
 * Type: string
 */
realtime_file_path = "$(TITAN_HOME)/cdata/latest.mdv";

/*
 * Option to compress data for transfer.
 * If set, data is run-length encoded for transfer over slow links.
 *
 * Type: boolean
 * Default: FALSE 
 */
compress_for_transfer = FALSE;

/*
 * Time offset (secs).
 * Search for data which is offset by this amount from the requested
 *   time.
 *
 * Type: long
 * Default: 0 
 */
time_offset = 0;

/*
 * Grid projection.
 * Local area flat grid, or lat-lon grid.
 *
 * Type: enum
 * Default: PROJ_FLAT 
 * Legal values: PROJ_FLAT, PROJ_LATLON.
 */
projection = PROJ_FLAT;


