#pragma once

#include <cmath>
#include <cstdint>
#include <chrono>


//static const int MU = 1; 						  							   				/**< Subcarrier spacing configuration. Section 4.2 - 3GPP TS 38.211 V15.2.0 (2018-06) */
static const int MU = 0;


const unsigned int SYNC_THRESHOLD = 30;    													// threshold for frame sync, self-correlation peak to average value about 100, cross-correlation Peak-Avg value for neighbor cell about 20.
const unsigned int CORRELATION_THRESHOLD = 4;    											// threshold for SSS correlation, self-corelation value about 11, cross correlation peak-avg value 0 (no CP included).
static const unsigned int PSS_CASE_NUM = MU;												/**< TBD  case num  should be set in configuration file */
static const char SSSYMINDEX = 'B';															/**< TBD config file? Determines the symbol indexes for candidate SS/PBCH blocks. 4.1 Cell search - 3GPP TS 38.213 V15.1.0 (2018-03) */

static const int REAL = 0; 															   		/**< Helper variable used for indexing fftwf_complex variables */
static const int IMAG = 1;															      	/**< Helper variable used for indexing fftwf_complex variables */
static const unsigned int MAXFRAMENUMBER = 1023;  										 	/**< Maximum number of frame count possible*/
static const unsigned int MAXNUMBEROFSYMBOLS = 14;											/**< Maximum number of symbols per slot*/
static const double FREQ = 3000000000.00;													/**< TBD Carrier frequency, should be configured by config file.*/

static const unsigned int CP0 = 288 + 32 * pow(2, MU) ;					    				/**< 5.3 OFDM baseband signal generation - 3GPP TS 38.211 V15.0.0 (2017-12) */
static const unsigned int CP = 288;									    					/**< 5.3 OFDM baseband signal generation - 3GPP TS 38.211 V15.0.0 (2017-12) */
static const unsigned int REFERENCE_FFT = 2048;								   		 		/**< FFT size. 4.1 General - 3GPP TS 38.211 V15.0.0 (2017-12) */
static const unsigned int FFTSIZE_NR = 4096;	     										/**< 5.3 OFDM baseband signal generation - 3GPP TS 38.211 V15.0.0 (2017-12) */
static const unsigned int NUMRB = 84;		//24 --405	48 -- 420		           			/**< max value possible currently. Ideally should be 275. This limitation is due to FFT SIZE being 2048 and there is no info regarding whether it increases with RB size.*/
static const unsigned int NUMSCINRB = 12; 		   								  			/**< Number of subcarriers in a resource block	*/
static const unsigned int NUMSC = NUMSCINRB * NUMRB; 							 			/**< Total number of resource blocks	*/
static const unsigned int BETA_CSIRS = 1;            										/**< TBD need to check CSI setting     7.4.1.5.3 Mapping to physical resources - 3GPP TS 38.211 V15.2.0 (2018-06)*/
static const unsigned int NUMBEROFFRAMES = 1024;     						   				/**< Required for calculating the slots in which a CSI-RS resource configured as periodic or semi-persistent is transmitted. 7.4.1.5.3 Mapping to physical resources - 3GPP TS 38.211 V15.2.0 (2018-06)	*/
//static const unsigned int NID1 = 181;										  				/**< TBD Select from {0,1, ... , 335} 7.4.2.1 Physical-layer cell identities - 3GPP TS 38.211 V15.0.0 (2017-12) */
static const unsigned int NID1 = 37;
//static const unsigned int NID2 = 0;											 				/**< TBD Select from {0,1,2}  7.4.2.1 Physical-layer cell identities - 3GPP TS 38.211 V15.0.0 (2017-12) */
static const unsigned int NID2 = 1;
static const unsigned int NCELLID = (3*NID1)+NID2;											/**< TBD cell_ID should be set according to config or sync .Cell ID. 7.4.2.1 Physical-layer cell identities - 3GPP TS 38.211 V15.0.0 (2017-12) */
static const unsigned int N = 240;										   					/**< Number of Sub Carriers in an SS/PBCH block. 7.4.3.1 Time-frequency structure of an SS/PBCH block - 3GPP TS 38.211 V15.0.0 (2017-12) */
static const unsigned int L = 7 * pow(2, MU);						      					/**< Repetition factor for CP0. 5.3 OFDM baseband signal generation - 3GPP TS 38.211 V15.0.0 (2017-12) */
static const unsigned int FCARRIER = FREQ/1e9;														/**< TBD Carrier frequency, should be given by config file!!! Refer 4.1 Cell Search: 3GPP TS 38.213 V15.1.0 (2018-03)for possible values.*/
static const unsigned int NC = 1600; 														/**< Refer 5.2.1 Pseudo-random sequence generation - 3GPP TS 38.211 V15.2.0 (2018-06)*/
static const unsigned int SCRAMBLING_ID = 1; 						   						/**< 38.214: defines scrambling ID of CSI-RS with length of 10 bits. http://www.sharetechnote.com/html/5G/5G_PDSCH_DMRS.html */
static const unsigned int DOWNLINK_SYMBOLS = 0;
static const unsigned int UPLINK_SYMBOLS = 1;
static const unsigned int FLEXIBLE_SYMBOLS = 2;
static const unsigned int SLOTFORMAT = 4;
static const unsigned int PBCH_RB_NUM = 20;                                     			/**< Resource block number of PBCH > */
static const unsigned int PBCH_SYMBOL_NUM = 4;                                     			/**< Symbol number of PBCH > */
static const unsigned int HALF_SS_SC_NUM = 64;         										/**< half synchronization sequence subcarrier number,  spectrum shape _| 64 | 63 |_   > */
static const unsigned int MAX_SLOT_WITH_SS = 36;         									/**< Max index of slot that contains SS, with case D and E > */
static const unsigned int MAX_SS_NUM_PER_SLOT = 3;         									/**< Max SS symbol in a slot, with case E > */
static constexpr int SUBFRAMEPERFRAME = 10;
static constexpr int SYMBOLSPERSUBFRAME = 14*(1<< MU); 										/**< Number of symbols per subframe. */
static constexpr int SLOTSPERFRAME = 10*(1<< MU); 			   								/**< Number of symbols per subframe. */
static constexpr int SLOTSPERSUBFRAME = (1<< MU); 			   								/**< Number of symbols per subframe. */

static constexpr unsigned int SUBCARRIER_SPACING[] = {15000,30000,60000,120000,240000};

static const unsigned int SYMBOLS_PER_TTI = 14*(1<< MU);
static const unsigned int UL_DL_SWITCHPOINT = 0; 											// If you change this, also change the value of UL_DL_SWITCH in RAN/include/Constants.hpp
static const unsigned int DL_UL_SWITCHPOINT = 1; 											//If you change this, also change the value of DL_UL_SWITCH in RAN/include/Constants.hpp
static const unsigned int GUARD_SAMPLES = 8;
static const unsigned int GUARD_PERIOD_COUNT = 2;
static const unsigned int LATENCY_PROBE_SYMBOL_LOCATION = 5;
static const unsigned int LATENCY_PROBE_REPLY_SYMBOL_LOCATION = 11;
static const unsigned int USEFUL_SUBCARRIERS = NUMRB * NUMSCINRB;

//int SEQUENCELENGTH = 0;
static constexpr unsigned int SAMPLES_PER_SYMBOL0 = FFTSIZE_NR + CP0;							// samples per symbol for 0 and 7*2^mu
const unsigned int CYCLIC_PREFIX = CP;
const unsigned int SPECIAL_CYCLIC_PREFIX = CP0;
const unsigned int SYMBOLS_PER_SLOT =14;
constexpr unsigned int SAMPLES_PER_SYMBOL = FFTSIZE_NR + CYCLIC_PREFIX; 						//  samples per symbol other than 0 and 7*2^mu
constexpr unsigned int SAMPLES_PER_TTI = SYMBOLS_PER_TTI * (CYCLIC_PREFIX + CP0 + CP0* int(pow(2, -MU)) + FFTSIZE_NR) + GUARD_PERIOD_COUNT * GUARD_SAMPLES;
constexpr unsigned int SAMPLES_PER_TTI_TWO_CP0 = (SYMBOLS_PER_TTI-2) * (CYCLIC_PREFIX + FFTSIZE_NR) + 2*(CP0 + FFTSIZE_NR) ; 	/**< The TTI is slot here in NR, can be with two CP0 and 12 CP, only for MU=0 */
constexpr unsigned int SAMPLES_PER_TTI_ONE_CP0 = (SYMBOLS_PER_TTI-1) * (CYCLIC_PREFIX + FFTSIZE_NR) + (CP0 + FFTSIZE_NR) ; 	/**< The TTI is slot here in NR, can be with one CP0 and 13 CP, for MU>0 */
constexpr unsigned int SAMPLES_PER_TTI_NO_CP0 = SYMBOLS_PER_TTI * (CYCLIC_PREFIX + FFTSIZE_NR) ; 	/**< The TTI is slot here in NR, can be totally only CP, for MU>0*/
constexpr unsigned int SAMPLES_PER_TTI_MAX = SAMPLES_PER_TTI_TWO_CP0;
static constexpr unsigned int SAMPLE_PER_SLOT0 =  SYMBOLS_PER_SLOT * SAMPLES_PER_SYMBOL;								// sample per slot without symbol 0 and 7*2^mu
static constexpr unsigned int SAMPLE_PER_SLOT1 =  SYMBOLS_PER_SLOT * SAMPLES_PER_SYMBOL + 2*(CP0-CP) ;					// sample per slot when Case A
static constexpr unsigned int SAMPLE_PER_SLOT2 =  SYMBOLS_PER_SLOT * SAMPLES_PER_SYMBOL + (CP0-CP);						// sample per slot when Case B
static constexpr unsigned int SAMPLE_PER_SLOT3 =  SYMBOLS_PER_SLOT * SAMPLES_PER_SYMBOL + (CP0-CP);						// sample per slot when Case C
static constexpr unsigned int SAMPLE_PER_SLOT4 =  SYMBOLS_PER_SLOT * SAMPLES_PER_SYMBOL + (CP0-CP)/4;					// sample per slot when Case D (averaged value)
static constexpr unsigned int SAMPLE_PER_SLOT5 =  SYMBOLS_PER_SLOT * SAMPLES_PER_SYMBOL + (CP0-CP)/8;					// sample per slot when Case E (averaged value)
static constexpr unsigned int SAMPLE_PER_FRAME = SYMBOLS_PER_SLOT * SAMPLES_PER_SYMBOL * (1<< MU) * 10 + 20*(CP0 - CP);					// Each frame have 20 long cp

static const unsigned int MAX_ANTENNA_PORTS = 4;
static const unsigned int PSS_SYMBOL_LOCATION = 10;
static const unsigned int PILOT_SYMBOL_LOCATION = 13;
static const unsigned int UL_BEACON_SYMBOL_LOCATION = 2;
static const unsigned int UL_REFERENCE_SYMBOL_LOCATION = 0;
static const unsigned int ANGLE_SPAN = 360;
static const unsigned int FIDX = 20;
static const unsigned int BITS_PER_SYMBOL_BPSK = 1;
static const unsigned int BITS_PER_SYMBOL_QPSK = 2;
static const unsigned int BITS_PER_SYMBOL_QAM16 = 4;
static const unsigned int BITS_PER_SYMBOL_QAM64 = 6;
static const unsigned int PSS_SEQUENCE_INDEX = 0;
static const unsigned int REFERENCE_SEQUENCE_INDEX = 1;
static const unsigned int BEACON_SEQUENCE_INDEX = 2;
static const unsigned int EKF_STATE_VECTOR_SIZE = 6;
static const unsigned int EKF_STATE_VECTOR_SIZE_FOR_gNB = 9;
static const unsigned int NUMBER_OF_TRP_IN_gNB_EKF = 2;
static const unsigned int SIZE_OF_COV_OF_AoAs = 2 * NUMBER_OF_TRP_IN_gNB_EKF;
static const unsigned int NMODES = 25;
static const unsigned int NUMTAU = 53;
static const unsigned int NUMEL  = 90;
static const unsigned int NUMAZ  = 180;
static const unsigned int PSS_CORRELATION_LENGTH = 10;	// In TTIs
static const double TIME_IN_RADIAN_FOR_EKF = 1 / (M_PI * SUBCARRIER_SPACING[4]);
static const unsigned long long C = 299792458;			// @todo This should be speed-of-light in air; not vacuum ?
static constexpr float ONE_OVER_SQRT10 = 1/sqrt(10);
static constexpr float ONE_OVER_SQRT42 = 1/sqrt(42);
static const double LAMBDA = C / FREQ;
static const double ANTENNA_SPACING = LAMBDA / 2;
static const float BPSK[2][2] = {{0.70710710, 0.70710710}, {-0.70710710, -0.70710710}};
static const float QPSK[4][2] = {{0.70710710, 0.70710710}, {0.70710710, -0.70710710}, {-0.70710710, 0.70710710}, {-0.70710710, -0.70710710}};
static const float QAM16[16][2] = {{0.31622810, 0.31622810}, {0.31622810, 0.94868310}, {0.94868310, 0.31622810}, {0.94868310, 0.94868310}, {0.31622810, -0.31622810}, {0.31622810, -0.94868310}, {0.94868310, -0.31622810}, {0.94868310, -0.94868310}, {-0.31622810, 0.31622810}, {-0.31622810, 0.94868310}, {-0.94868310, 0.31622810}, {-0.94868310, 0.94868310}, {-0.31622810, -0.31622810}, {-0.31622810, -0.94868310}, {-0.94868310, -0.31622810}, {-0.94868310, -0.94868310}};
static const float QAM64[64][2] = { {0.46291010, 0.46291010}, {0.46291010, 0.15430310}, {0.15430310, 0.46291010}, {0.15430310, 0.15430310}, {0.46291010, 0.77151710}, {0.46291010, 1.08012310}, {0.15430310, 0.77151710}, {0.15430310, 1.08012310}, {0.77151710, 0.46291010}, {0.77151710, 0.15430310}, {1.08012310, 0.46291010}, {1.08012310, 0.15430310}, {0.77151710, 0.77151710}, {0.77151710, 1.08012310}, {1.08012310, 0.77151710}, {1.08012310, 1.08012310}, {0.46291010, -0.46291010}, {0.46291010, -0.15430310}, {0.15430310, -0.46291010}, {0.15430310, -0.15430310}, {0.46291010, -0.77151710}, {0.46291010, -1.08012310}, {0.15430310, -0.77151710}, {0.15430310, -1.08012310}, {0.77151710, -0.46291010}, {0.77151710, -0.15430310}, {1.08012310, -0.46291010}, {1.08012310, -0.15430310}, {0.77151710, -0.77151710}, {0.77151710, -1.08012310}, {1.08012310, -0.77151710}, {1.08012310, -1.08012310}, {-0.46291010, 0.46291010}, {-0.46291010, 0.15430310}, {-0.15430310, 0.46291010}, {-0.15430310, 0.15430310}, {-0.46291010, 0.77151710}, {-0.46291010, 1.08012310}, {-0.15430310, 0.77151710}, {-0.15430310, 1.08012310}, {-0.77151710, 0.46291010}, {-0.77151710, 0.15430310}, {-1.08012310, 0.46291010}, {-1.08012310, 0.15430310}, {-0.77151710, 0.77151710}, {-0.77151710, 1.08012310}, {-1.08012310, 0.77151710}, {-1.08012310, 1.08012310}, {-0.46291010, -0.46291010}, {-0.46291010, -0.15430310}, {-0.15430310, -0.46291010}, {-0.15430310, -0.15430310}, {-0.46291010, -0.77151710}, {-0.46291010, -1.08012310}, {-0.15430310, -0.77151710}, {-0.15430310, -1.08012310}, {-0.77151710, -0.46291010}, {-0.77151710, -0.15430310}, {-1.08012310, -0.46291010}, {-1.08012310, -0.15430310}, {-0.77151710, -0.77151710}, {-0.77151710, -1.08012310}, {-1.08012310, -0.77151710}, {-1.08012310, -1.08012310}};

/*******************
 * @Note UL-DL split
 *  0 -  4 UL  5
 *  5 - 22 DL 18
 * 23 - 41 UL 19
 ******************/

//static_assert(UL_DL_SWITCHPOINT < DL_UL_SWITCHPOINT && DL_UL_SWITCHPOINT < SYMBOLS_PER_TTI, "The following must hold : UL_DL_SWITCHPOINT < DL_UL_SWITCHPOINT AND DL_UL_SWITCHPOINT < SYMBOLS_PER_TTI");

// Derived parameters
static constexpr double SAMPLE_DURATION = 1 / (SUBCARRIER_SPACING[MU] * FFTSIZE_NR);
//static constexpr unsigned int SAMPLES_PER_TTI = SYMBOLS_PER_TTI * (CYCLIC_PREFIX + FFTSIZE_NR) + GUARD_PERIOD_COUNT * GUARD_SAMPLES;
static constexpr double SAMPLING_RATE = (100000000.0/6.0);
//static constexpr unsigned int SAMPLES_PER_SYMBOL = FFTSIZE_NR + CYCLIC_PREFIX;
static constexpr std::chrono::microseconds TTI_DURATION{(unsigned long long)(1 / SAMPLING_RATE * SAMPLES_PER_TTI * 1e6)};

const uint32_t CRC_TABLE_16[256] =
{
		0x0000,0x2110,0x4220,0x6330,0x8440,0xa550,0xc660,0xe770,
		0x0881,0x2991,0x4aa1,0x6bb1,0x8cc1,0xadd1,0xcee1,0xeff1,
		0x3112,0x1002,0x7332,0x5222,0xb552,0x9442,0xf772,0xd662,
		0x3993,0x1883,0x7bb3,0x5aa3,0xbdd3,0x9cc3,0xfff3,0xdee3,
		0x6224,0x4334,0x2004,0x0114,0xe664,0xc774,0xa444,0x8554,
		0x6aa5,0x4bb5,0x2885,0x0995,0xeee5,0xcff5,0xacc5,0x8dd5,
		0x5336,0x7226,0x1116,0x3006,0xd776,0xf666,0x9556,0xb446,
		0x5bb7,0x7aa7,0x1997,0x3887,0xdff7,0xfee7,0x9dd7,0xbcc7,
		0xc448,0xe558,0x8668,0xa778,0x4008,0x6118,0x0228,0x2338,
		0xccc9,0xedd9,0x8ee9,0xaff9,0x4889,0x6999,0xaa9,0x2bb9,
		0xf55a,0xd44a,0xb77a,0x966a,0x711a,0x500a,0x333a,0x122a,
		0xfddb,0xdccb,0xbffb,0x9eeb,0x799b,0x588b,0x3bbb,0x1aab,
		0xa66c,0x877c,0xe44c,0xc55c,0x222c,0x033c,0x600c,0x411c,
		0xaeed,0x8ffd,0xeccd,0xcddd,0x2aad,0x0bbd,0x688d,0x499d,
		0x977e,0xb66e,0xd55e,0xf44e,0x133e,0x322e,0x511e,0x700e,
		0x9fff,0xbeef,0xdddf,0xfccf,0x1bbf,0x3aaf,0x599f,0x788f,
		0x8891,0xa981,0xcab1,0xeba1,0x0cd1,0x2dc1,0x4ef1,0x6fe1,
		0x8010,0xa100,0xc230,0xe320,0x0450,0x2540,0x4670,0x6760,
		0xb983,0x9893,0xfba3,0xdab3,0x3dc3,0x1cd3,0x7fe3,0x5ef3,
		0xb102,0x9012,0xf322,0xd232,0x3542,0x1452,0x7762,0x5672,
		0xeab5,0xcba5,0xa895,0x8985,0x6ef5,0x4fe5,0x2cd5,0x0dc5,
		0xe234,0xc324,0xa014,0x8104,0x6674,0x4764,0x2454,0x0544,
		0xdba7,0xfab7,0x9987,0xb897,0x5fe7,0x7ef7,0x1dc7,0x3cd7,
		0xd326,0xf236,0x9106,0xb016,0x5766,0x7676,0x1546,0x3456,
		0x4cd9,0x6dc9,0x0ef9,0x2fe9,0xc899,0xe989,0x8ab9,0xaba9,
		0x4458,0x6548,0x0678,0x2768,0xc018,0xe108,0x8238,0xa328,
		0x7dcb,0x5cdb,0x3feb,0x1efb,0xf98b,0xd89b,0xbbab,0x9abb,
		0x754a,0x545a,0x376a,0x167a,0xf10a,0xd01a,0xb32a,0x923a,
		0x2efd,0x0fed,0x6cdd,0x4dcd,0xaabd,0x8bad,0xe89d,0xc98d,
		0x267c,0x076c,0x645c,0x454c,0xa23c,0x832c,0xe01c,0xc10c,
		0x1fef,0x3eff,0x5dcf,0x7cdf,0x9baf,0xbabf,0xd98f,0xf89f,
		0x176e,0x367e,0x554e,0x745e,0x932e,0xb23e,0xd10e,0xf01e
};


const uint32_t SS_POSITION_TABLE_A[8] = {2,8,16,22,30,36,44,50};
const uint32_t SS_POSITION_TABLE_B[8] = {4,8,16,20,32,36,44,48};
const uint32_t SS_POSITION_TABLE_D[64] =
{
	4,8,16,20,
	32,36,44,48,
	60,64,72,76,
	88,92,100,104,
	144,148,156,160,
	172,176,184,188,
	200,204,212,216,
	228,232,240,244,
	284,288,296,300,
	312,316,324,328,
	340,344,352,356,
	368,372,380,384,
	424,428,436,440,
	452,456,464,468,
	480,484,492,496,
	508,512,520,524
};

const uint32_t SS_POSITION_TABLE_E[64] =
{
	8,12,16,20,
	32,36,40,44,
	64,68,72,76,
	88,92,96,100,
	120,124,128,132,
	144,148,152,156,
	176,180,184,188,
	200,204,208,212,
	288,292,296,300,
	312,316,320,324,
	344,348,352,356,
	368,372,376,380,
	400,404,408,412,
	424,428,432,436,
	456,460,464,468,
	480,484,488,492
};
