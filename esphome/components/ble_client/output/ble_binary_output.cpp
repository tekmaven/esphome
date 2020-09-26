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

void BLEBinaryOutput::gattc_event_handler(esp_gattc_cb_event_t event, esp_gatt_if_t gattc_if,
                                          esp_ble_gattc_cb_param_t *param) {
  switch (event) {
    case ESP_GATTC_OPEN_EVT:
      ESP_LOGW(TAG, "[%s] Connected successfully!", this->char_uuid_.to_string().c_str());
      this->node_state_ = espbt::ClientState::Established;
      break;
    case ESP_GATTC_DISCONNECT_EVT:
      this->node_state_ = espbt::ClientState::Idle;
      ESP_LOGW(TAG, "[%s] Disconnected", this->char_uuid_.to_string().c_str());
      break;
    default:
      break;
  }
}


void BLEBinaryOutput::write_state(bool state) {
  auto stateAsUint = (uint8_t)state;
  if (this->parent_->state_ != espbt::ClientState::Established) {
    ESP_LOGW(TAG, "[%s] Not connected to BLE client.  State update can not be written.  Requested write: %d", this->char_uuid_.to_string().c_str(), stateAsUint);
    return;
  }
  ESP_LOGW(TAG, "[%s] Write State: %d", this->char_uuid_.to_string().c_str(), stateAsUint);
  this->parent_->get_characteristic(this->service_uuid_, this->char_uuid_)->write_value(stateAsUint);
  ESP_LOGW(TAG, "[%s] SUCCESS! State: %d", this->char_uuid_.to_string().c_str(), stateAsUint);
}

}  // namespace ble_client
}  // namespace esphome
