#include "lp_filter.h"

void filter_init(LowPassFilter *filter, Uint32 alpha, signed long y0) {
    filter->alpha = alpha;
    filter->y_prev = y0;
}

signed long apply_filter(LowPassFilter *filter, signed long x) {
    // definitivno ovo pravi problem
    signed long y = (((signed long)filter->alpha * x)/1000) + (((1000 - (signed long)filter->alpha) * filter->y_prev)/1000);
    filter->y_prev = y;
    return y;
}
