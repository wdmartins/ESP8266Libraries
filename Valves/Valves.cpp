#include <Arduino.h>
#include <Ticker.h>
#include <Valves.h>

void skipCheckTime(SolenoidValve *sv) {
  sv->_skipSyncCheck = !sv->_skipSyncCheck;
}

void SolenoidValve::run() {
    if (_flowMeter != nullptr && _skipSyncCheck == false) {
        // Serial.print("valve run: _isOpen"); Serial.print(_isOpen); Serial.print("isFlowing: "); Serial.println(_flowMeter->isFlowing());
        if (_isOpen != _flowMeter->isFlowing()) {
            Serial.print("Valve out of sync. Activate Valve");
            _activateSolenoid();
            _skipSyncCheck = true;
            _ticker.once(SKIP_SYNC_INTERVAL_SECONDS, skipCheckTime, this);
        }
    }
}