#pragma once

//
struct extended_timer {
  uint64_t last_ms = 0;

  uint64_t millis() {
    unsigned long now = millis();
    const unsigned long small_timer_bytes = sizeof(decltype(millis()));
    // compiler should optimize this away, this is constant
    if (small_timer_bytes == sizeof(decltype(last_ms))) {
      return last_ms = now;
    } else {
      // we match how the original timer would overflow
      uint64_t since_ms = (uint64_t)now - (last_ms & (~uint64_t(0) >> (64u - small_timer_bytes*8)));
      // then accumulate that difference into our "timer"
      last_ms += since_ms;
      return last_ms;
    }
  }

  uint64_t last_millis() {
    return last_ms;
  }
};

// The "drift" timer allows you to modify the timer results by decoupling the acculation
// result from the original timer if you absolutely need it. This means you could in theory
// "correct" this timer slowly by adjusting the last_ms member.
struct extended_drift_timer {
  uint64_t last_ms = 0;
  decltype(millis()) prev_ms = 0;

  uint64_t millis() {
    unsigned long now = millis();
    const unsigned long small_timer_bytes = sizeof(decltype(millis()));
    // compiler should optimize this away, this is constant
    if (small_timer_bytes == sizeof(decltype(last_ms))) {
      prev_ms = now;
      return last_ms = now;
    } else {
      // we match how the original timer would overflow
      uint64_t since_ms = (uint64_t)now - prev_ms;
      prev_ms = now;
      // then accumulate that difference into our "timer"
      last_ms += since_ms;
      return last_ms;
    }
  }

  uint64_t last_millis() {
    return last_ms;
  }
};
