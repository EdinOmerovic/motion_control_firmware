#ifndef LOW_PASS_FILTER_H_
#define LOW_PASS_FILTER_H_

typedef struct {
    Uint32 alpha;
    signed long y_prev;
} LowPassFilter;

void filter_init(LowPassFilter *filter, Uint32 alpha, signed long y0);
signed long apply_filter(LowPassFilter *filter, signed long x);

#endif /* LOW_PASS_FILTER_H_ */
