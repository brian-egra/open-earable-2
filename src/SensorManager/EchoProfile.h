#ifndef ECHO_PROFILE_H
#define ECHO_PROFILE_H

#include "EdgeMLSensor.h"

#include "openearable_common.h"

/**
 * Active acoustic sensing sensor (EarIO-style): plays an inaudible FMCW chirp
 * (18-21 kHz) on the speaker and streams matched-filter echo profiles of the
 * inner microphone over BLE. One profile = ECHO_PROFILE_BINS log-compressed
 * magnitude bins covering the first bins of acoustic delay.
 */
class EchoProfile : public EdgeMlSensor {
public:
    static EchoProfile sensor;

    bool init(struct k_msgq * queue) override;
    void start(int sample_rate_idx) override;
    void stop() override;

    const static SampleRateSetting<3> sample_rates;
private:
    bool _active = false;
};

#endif
