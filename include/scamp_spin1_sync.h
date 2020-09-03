//! Clock drift fixed-point number definition in terms of the number of
//! fractional bits and the mask to get the fractional bits
#define DRIFT_FP_BITS 17U
#define DRIFT_INT_MASK (((1U << (32 - DRIFT_FP_BITS)) - 1) << DRIFT_FP_BITS)
#define DRIFT_FRAC_MASK ((1U << DRIFT_FP_BITS) - 1)
#define DRIFT_ONE (1U << DRIFT_FP_BITS)
#define TIME_BETWEEN_SYNC_US 2000000U // !< The time between sync signals in us
