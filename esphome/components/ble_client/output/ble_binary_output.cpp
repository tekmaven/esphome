#include "ble_binary_output.h"
#include "esphome/core/log.h"
#include "esphome/components/ble_client/ble_client.h"
#include "esphome/components/esp32_ble_tracker/esp32_ble_tracker.h"

namespace esphome {
namespace ble_client {

static const char *TAG = "ble_binary_output";


void BLEBinaryOutput::dump_config() {

  ESP_LOGCONFIG(TAG, "BLE Binary Output:");
  ESP_LOGCONFIG(TAG, "  MAC address        : %s", this->parent_->address_str().c_str());
  ESP_LOGCONFIG(TAG, "  Service UUID       : %s", this->service_uuid_.to_string().c_str());
  ESP_LOGCONFIG(TAG, "  Characteristic UUID: %s", this->char_uuid_.to_string().c_str());
  LOG_BINARY_OUTPUT(this);
}

void BLEBinaryOutput::write_state(bool state) {
  auto stateAsUint = (uint8_t)state;
  ESP_LOGW(TAG, "[%s] Write State: %d", this->char_uuid_.to_string().c_str(), stateAsUint);
  this->parent_->get_characteristic(this->service_uuid_, this->char_uuid_)->write_value(stateAsUint);
  ESP_LOGW(TAG, "[%s] SUCCESS! State: %d", this->char_uuid_.to_string().c_str(), stateAsUint);
}

}  // namespace ble_client
}  // namespace esphome
