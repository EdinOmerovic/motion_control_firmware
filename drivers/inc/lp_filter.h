#ifndef LOW_PASS_FILTER_H_
#define LOW_PASS_FILTER_H_

typedef struct {
    signed long alpha;
    signed long y_prev;
} LowPassFilter;

void filter_init(LowPassFilter *filter, int alpha, int y0);
signed long apply_filter(LowPassFilter *filter, signed long x);

#endif /* LOW_PASS_FILTER_H_ */
