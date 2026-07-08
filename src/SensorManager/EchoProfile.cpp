#include "EchoProfile.h"

#include "audio_datapath.h"
#include "streamctrl.h"

#include "SensorManager.h"

#include <zephyr/kernel.h>

#ifdef __cplusplus
extern "C" {
#endif

#include <data_fifo.h>

#ifdef __cplusplus
}
#endif

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(echo_profile, CONFIG_LOG_DEFAULT_LEVEL);

extern struct data_fifo fifo_rx;

EchoProfile EchoProfile::sensor;

/* reg_vals = number of 10 ms chirp frames averaged per emitted profile */
const SampleRateSetting<2> EchoProfile::sample_rates = {
    { 2, 1 },

    { 50.0, 100.0 },

    { 50.0, 100.0 }
};

bool EchoProfile::init(struct k_msgq * queue) {
    _active = true;

    sensor_queue = queue;

    set_sensor_queue(queue);

    return true;
}

void EchoProfile::start(int sample_rate_idx) {
    if (!_active) return;

    LOG_INF("Starting ultrasound echo profile at %f Hz",
            sample_rates.sample_rates[sample_rate_idx]);

    audio_datapath_aquire(&fifo_rx);

    chirp_set(true);
    echo_stream_set(true, sample_rates.reg_vals[sample_rate_idx]);

    _running = true;
}

void EchoProfile::stop() {
    if (!_active) return;
    _active = false;

    if (!_running) return;

    echo_stream_set(false, 1);
    chirp_set(false);

    audio_datapath_release();

    _running = false;
}
