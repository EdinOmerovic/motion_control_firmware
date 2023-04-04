#ifndef LOW_PASS_FILTER_H_
#define LOW_PASS_FILTER_H_

typedef struct {
    int alpha;
    int y_prev;
} LowPassFilter;

void filter_init(LowPassFilter *filter, int alpha, int y0);
int apply_filter(LowPassFilter *filter, int x);

#endif /* LOW_PASS_FILTER_H_ */
