#include "lp_filter.h"

void filter_init(LowPassFilter *filter, Uint32 alpha, signed long y0) {
    filter->alpha = alpha;
    filter->y_prev = y0;
}

signed long apply_filter(LowPassFilter *filter, signed long x) {
    signed long y = ((filter->alpha * x) + (1000 - filter->alpha) * filter->y_prev)/1000;
    filter->y_prev = y;
    return y;
}
