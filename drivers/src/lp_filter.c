#include "lp_filter.h"

void filter_init(LowPassFilter *filter, int alpha, int y0) {
    filter->alpha = alpha;
    filter->y_prev = y0;
}

int apply_filter(LowPassFilter *filter, int x) {
    int y = filter->alpha * x + (1 - filter->alpha) * filter->y_prev;
    filter->y_prev = y;
    return y;
}