#include <Arduino.h>
#include <Ticker.h>
#include <Valves.h>

void skipCheckTime(SolenoidValve *sv) {
//   Serial.println("skipCheckTime");
  sv->_skipSyncCheck = !(sv->_skipSyncCheck);
}

void SolenoidValve::openValve() {
    if (_isOpen) {
        Serial.println("Valve is already open");
        return;
    }
    _skipSyncCheck = true;
    _ticker.once(SKIP_SYNC_INTERVAL_SECONDS, skipCheckTime, this);
    _activateSolenoid();
    _isOpen = true;
}
void SolenoidValve::closeValve() {
    if (!_isOpen) {
        Serial.println("Valve is already close");
        return;
    }
    _skipSyncCheck = true;
    _ticker.once(SKIP_SYNC_INTERVAL_SECONDS, skipCheckTime, this);
    _activateSolenoid();
    _isOpen = false;
}

void SolenoidValve::run() {
    if (_flowMeter != nullptr && _skipSyncCheck == false) {
        // Serial.print("valve run: _isOpen"); Serial.print(_isOpen); Serial.print("isFlowing: "); Serial.println(_flowMeter->isFlowing());
        if (_isOpen != _flowMeter->isFlowing()) {
            Serial.println("Valve out of sync. Activate Valve");
            _activateSolenoid();
            // Serial.println("Valve Activated");
            _skipSyncCheck = true;
            _ticker.once(SKIP_SYNC_INTERVAL_SECONDS, skipCheckTime, this);
        }
    }
}
