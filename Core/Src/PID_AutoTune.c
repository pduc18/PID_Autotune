#include "PID_AutoTune.h"
#include <math.h>

void PIDAutotuner_Init(PIDAutotuner* tuner, float outputStep, float noiseBand, uint32_t lookbackTime, float minOutput, float maxOutput) {
    tuner->outputStep = outputStep;
    tuner->noiseBand = noiseBand;
    tuner->lookbackTime = lookbackTime;
    tuner->controlOutput = 0;
    tuner->minOutput = minOutput;
    tuner->maxOutput = maxOutput;
    tuner->peakCount = 0;
    tuner->isMax = 0;
    tuner->isMin = 0;
    tuner->absMax = 0;
    tuner->absMin = 0;
    tuner->running = true;
    tuner->lastTime = 0;
}

bool PIDAutotuner_Runtime(PIDAutotuner* tuner, float input, uint32_t now, float* output) {
    if (!tuner->running) return false;
    if ((now - tuner->lastTime) < tuner->lookbackTime) return false;

    tuner->lastTime = now;

    if (input > tuner->absMax) tuner->absMax = input;
    if (input < tuner->absMin) tuner->absMin = input;

    if (input > tuner->lastInput + tuner->noiseBand) {
        if (tuner->isMin) {
            tuner->isMin = 0;
            tuner->peaks[tuner->peakCount] = input;
            tuner->peakTimes[tuner->peakCount++] = now;
        }
        tuner->isMax = 1;
        *output = tuner->controlOutput + tuner->outputStep;
    } else if (input < tuner->lastInput - tuner->noiseBand) {
        if (tuner->isMax) {
            tuner->isMax = 0;
            tuner->peaks[tuner->peakCount] = input;
            tuner->peakTimes[tuner->peakCount++] = now;
        }
        tuner->isMin = 1;
        *output = tuner->controlOutput - tuner->outputStep;
    }

    tuner->lastInput = input;

    if (tuner->peakCount >= 8) {
        tuner->running = false;
        return true;
    }
    return false;
}

void PIDAutotuner_ComputeParameters(PIDAutotuner* tuner, float* Kp, float* Ki, float* Kd) {
    float avgAmp = 0;
    for (uint8_t i = 1; i < tuner->peakCount; i++) {
        avgAmp += fabsf(tuner->peaks[i] - tuner->peaks[i - 1]);
    }
    avgAmp /= (tuner->peakCount - 1);
    float Pu = (tuner->peakTimes[tuner->peakCount - 1] - tuner->peakTimes[tuner->peakCount - 5]) / 4.0f;

    tuner->Ku = (4.0f * tuner->outputStep) / (3.14159f * avgAmp);
    tuner->Pu = Pu;

    *Kp = 0.6f * tuner->Ku;
    *Ki = 1.2f * tuner->Ku / Pu;
    *Kd = 0.075f * tuner->Ku * Pu;
}
