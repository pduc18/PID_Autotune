#ifndef __PID_AUTOTUNE_H
#define __PID_AUTOTUNE_H

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    float outputStep;
    float noiseBand;
    uint32_t lookbackTime;
    float controlOutput;
    float minOutput;
    float maxOutput;

    // Internal
    float lastInput;
    uint32_t peakCount;
    float peaks[10];
    uint32_t peakTimes[10];
    uint32_t lastTime;
    uint8_t isMax, isMin;
    float absMax, absMin;
    bool running;
    float Ku, Pu;
} PIDAutotuner;

void PIDAutotuner_Init(PIDAutotuner* tuner, float outputStep, float noiseBand, uint32_t lookbackTime, float minOutput, float maxOutput);
bool PIDAutotuner_Runtime(PIDAutotuner* tuner, float input, uint32_t now, float* output);
void PIDAutotuner_ComputeParameters(PIDAutotuner* tuner, float* Kp, float* Ki, float* Kd);

#endif
