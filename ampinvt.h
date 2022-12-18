// *****************************************************************
// *          ESPHome Custom Component Modbus sniffer for          *
// *              Ampinvt MPPT Solar Controller                    *
// *                  Original code credits:                       *
// *                https://github.com/htvekov/                    *
// *               https://github.com/assembly12/                  *
// *****************************************************************

#include "esphome.h"

class ampinvtsensor : public PollingComponent, public Sensor, public UARTDevice {
  public:
    ampinvtsensor(UARTComponent *parent) : PollingComponent(600), UARTDevice(parent) {}

  //37 bytes total - 25 bytes used, 12 bytes unused
  // Bit sensors follow (bytes 3, 4, 5)
  Sensor *ampinvt_op_status              = new Sensor(); // 1 bit ~ byte 3 (0=Normal, 1=Abnormal - Battery Automatic Recognition Error)
  Sensor *ampinvt_battery_status         = new Sensor(); // 1 bit ~ byte 3 (0=Normal, 1=Over Discharge Protection)
  Sensor *ampinvt_fan_status             = new Sensor(); // 1 bit ~ byte 3 (0=Normal, 1=Fan Failure)
  Sensor *ampinvt_overheat_status        = new Sensor(); // 1 bit ~ byte 3 (0=Normal, 1=Over Temperature Protection)
  Sensor *ampinvt_dcoutput_status        = new Sensor(); // 1 bit ~ byte 3 (0=Normal, 1=DC Output SHort / Over Current Protection)
  Sensor *ampinvt_inttemp1_status        = new Sensor(); // 1 bit ~ byte 3 (0=Close, 1=Fault)
  Sensor *ampinvt_inttemp2_status        = new Sensor(); // 1 bit ~ byte 3 (0=Close, 1=Fault)
  Sensor *ampinvt_exttemp_status         = new Sensor(); // 1 bit ~ byte 3 (0=Close, 1=Fault)
  Sensor *ampinvt_chg_status             = new Sensor(); // 1 bit ~ byte 4 (0=Not Charging, 1=Charging)
  Sensor *ampinvt_equalchg_status        = new Sensor(); // 1 bit ~ byte 4 (1=True)
  Sensor *ampinvt_track_status           = new Sensor(); // 1 bit ~ byte 4 (1=True)
  Sensor *ampinvt_floatchg_status        = new Sensor(); // 1 bit ~ byte 4 (1=True)
  Sensor *ampinvt_chgcurrentlimit_status = new Sensor(); // 1 bit ~ byte 4 (1=True)
  Sensor *ampinvt_chgderating_status     = new Sensor(); // 1 bit ~ byte 4 (1=True)
  Sensor *ampinvt_remoteprohibchg_status = new Sensor(); // 1 bit ~ byte 4 (1=True)
  Sensor *ampinvt_pvovervolt_status      = new Sensor(); // 1 bit ~ byte 4 (1=True)
  Sensor *ampinvt_chgoutputrelay_status  = new Sensor(); // 1 bit ~ byte 5 (0=Close, 1=Open)
  Sensor *ampinvt_loadoutput_status      = new Sensor(); // 1 bit ~ byte 5 (0=Close, 1=Open)
  Sensor *ampinvt_fanrelay_status        = new Sensor(); // 1 bit ~ byte 5 (0=Close, 1=Open)
  Sensor *ampinvt_overchgprotect_status  = new Sensor(); // 1 bit ~ byte 5 (0=Normal, 1=Overcharge Protection)
  Sensor *ampinvt_overvoltprotect_status = new Sensor(); // 1 bit ~ byte 5 (0=Normal, 1=Overvoltage Protection)
  // Byte sensors follow
  Sensor *ampinvt_pv_voltage             = new Sensor(); // 2 byte
  Sensor *ampinvt_battery_voltage        = new Sensor(); // 2 byte
  Sensor *ampinvt_charge_current         = new Sensor(); // 2 byte
  Sensor *ampinvt_mppt_temperature       = new Sensor(); // 2 byte
  Sensor *ampinvt_battery_temperature    = new Sensor(); // 2 byte
  Sensor *ampinvt_today_yield            = new Sensor(); // 4 byte
  Sensor *ampinvt_generation_total       = new Sensor(); // 4 byte
  
  void setup() override {
  }

  std::vector<int> bytes;

  void update() {
    while(available() > 0) {
      bytes.push_back(read());      
      if(bytes.size() < 37){
        continue;  
      }
    
      else {
      }
	    if(bytes.size() == 37) {

        uint8_t op_status_byte = (uint8_t)(bytes[3]);
        #define BIT_OPERATING_STATUS       0x1  // 00000001
        #define BIT_BATTERY_STATUS         0x2  // 00000010
        #define BIT_FAN_STATUS             0x4  // 00000100
        #define BIT_TEMPERATURE_STATUS     0x8  // 00001000
        #define BIT_DCOUTPUT_STATUS        0x10 // 00010000
        #define BIT_INTTEMP1_STATUS        0x20 // 00100000
        #define BIT_INTTEMP2_STATUS        0x40 // 01000000
        #define BIT_EXTTEMP_STATUS         0x80 // 10000000
        id(ampinvt_op_status).publish_state((bool)(op_status_byte & BIT_OPERATING_STATUS));
        id(ampinvt_battery_status).publish_state((bool)(op_status_byte & BIT_BATTERY_STATUS));
        id(ampinvt_fan_status).publish_state((bool)(op_status_byte & BIT_FAN_STATUS));
        id(ampinvt_overheat_status).publish_state((bool)(op_status_byte & BIT_TEMPERATURE_STATUS));
        id(ampinvt_dcoutput_status).publish_state((bool)(op_status_byte & BIT_DCOUTPUT_STATUS));
        id(ampinvt_inttemp1_status).publish_state((bool)(op_status_byte & BIT_INTTEMP1_STATUS));
        id(ampinvt_inttemp2_status).publish_state((bool)(op_status_byte & BIT_INTTEMP2_STATUS));
        id(ampinvt_exttemp_status).publish_state((bool)(op_status_byte & BIT_EXTTEMP_STATUS));     

        uint8_t chg_status_byte = (uint8_t)(bytes[4]);
        #define BIT_CHARGING_STATUS        0x1  // 00000001
        #define BIT_EQUALCHG_STATUS        0x2  // 00000010
        #define BIT_TRACK_STATUS           0x4  // 00000100
        #define BIT_FLOATCHG_STATUS        0x8  // 00001000
        #define BIT_CHGCURRENTLIMIT_STATUS 0x10 // 00010000
        #define BIT_CHGDERATING_STATUS     0x20 // 00100000
        #define BIT_REMOTEPROHIBCHG_STATUS 0x40 // 01000000
        #define BIT_PVOVERVOLT_STATUS      0x80 // 10000000
        id(ampinvt_chg_status).publish_state((bool)(chg_status_byte & BIT_CHARGING_STATUS));
        id(ampinvt_equalchg_status).publish_state((bool)(chg_status_byte & BIT_EQUALCHG_STATUS));
        id(ampinvt_track_status).publish_state((bool)(chg_status_byte & BIT_TRACK_STATUS));
        id(ampinvt_floatchg_status).publish_state((bool)(chg_status_byte & BIT_FLOATCHG_STATUS));
        id(ampinvt_chgcurrentlimit_status).publish_state((bool)(chg_status_byte & BIT_CHGCURRENTLIMIT_STATUS));
        id(ampinvt_chgderating_status).publish_state((bool)(chg_status_byte & BIT_CHGDERATING_STATUS));
        id(ampinvt_remoteprohibchg_status).publish_state((bool)(chg_status_byte & BIT_REMOTEPROHIBCHG_STATUS));
        id(ampinvt_pvovervolt_status).publish_state((bool)(chg_status_byte & BIT_PVOVERVOLT_STATUS));   

        uint8_t ctl_status_byte = (uint8_t)(bytes[5]);
        #define BIT_CHGOUTRLY_STATUS       0x1  // 00000001
        #define BIT_LOADOUTPUT_STATUS      0x2  // 00000010
        #define BIT_FANRLY_STATUS          0x4  // 00000100
        #define BIT_SPARE1_STATUS          0x8  // 00001000
        #define BIT_OVERCHGPROTECT_STATUS  0x10 // 00010000
        #define BIT_OVERVOLTPROTECT_STATUS 0x20 // 00100000
        #define BIT_SPARE2_STATUS          0x40 // 01000000
        #define BIT_SPARE3_STATUS          0x80 // 10000000
        id(ampinvt_chgoutputrelay_status).publish_state((bool)(ctl_status_byte & BIT_CHGOUTRLY_STATUS));
        id(ampinvt_loadoutput_status).publish_state((bool)(ctl_status_byte & BIT_LOADOUTPUT_STATUS));
        id(ampinvt_fanrelay_status).publish_state((bool)(ctl_status_byte & BIT_FANRLY_STATUS));
        id(ampinvt_overchgprotect_status).publish_state((bool)(ctl_status_byte & BIT_OVERCHGPROTECT_STATUS));
        id(ampinvt_overvoltprotect_status).publish_state((bool)(ctl_status_byte & BIT_OVERVOLTPROTECT_STATUS));  
        
        TwoByte pv_voltage_value;
        pv_voltage_value.Byte[1] = bytes[6]; // PV Voltage high byte
        pv_voltage_value.Byte[0] = bytes[7]; // PV Voltage low byte
        id(ampinvt_pv_voltage).publish_state(pv_voltage_value.UInt16);
        
        TwoByte battery_voltage_value;
        battery_voltage_value.Byte[1] = bytes[8]; // Battery Voltage high byte
        battery_voltage_value.Byte[0] = bytes[9]; // Battery Voltage low byte
        id(ampinvt_battery_voltage).publish_state(battery_voltage_value.UInt16);
        
        TwoByte charge_current_value;
        charge_current_value.Byte[1] = bytes[10]; // Charge Current high byte
        charge_current_value.Byte[0] = bytes[11]; // Charge Current low byte
        id(ampinvt_charge_current).publish_state(charge_current_value.UInt16);

        // Necessary to accomodate negative celcius temps in unheated installations        
        int16_t mppt_temperature_value;
          if (bytes[12] >= 0x10) {
            mppt_temperature_value = int(
              (signed char)(bytes[13] * -1));
          } else {
            mppt_temperature_value = int(
              (signed char)(bytes[13]));
          }
        id(ampinvt_mppt_temperature).publish_state(mppt_temperature_value);
        
        // Necessary to accomodate negative celcius temps in unheated installations
        int16_t battery_temperature_value;
          if (bytes[16] >= 0x10) {
            battery_temperature_value = int(
              (signed char)(bytes[17] * -1));
          } else {
            battery_temperature_value = int(
              (signed char)(bytes[17]));
          }
        id(ampinvt_battery_temperature).publish_state(battery_temperature_value);

        uint32_t today_yield_value = int(
            (unsigned char)(bytes[20]) << 24 |
            (unsigned char)(bytes[21]) << 16 |
            (unsigned char)(bytes[22]) << 8 |
            (unsigned char)(bytes[23]));
        id(ampinvt_today_yield).publish_state(today_yield_value);

        uint32_t generation_total_value = int(
            (unsigned char)(bytes[24]) << 24 |
            (unsigned char)(bytes[25]) << 16 |
            (unsigned char)(bytes[26]) << 8 |
            (unsigned char)(bytes[27]));
        id(ampinvt_generation_total).publish_state(generation_total_value);

        bytes.clear();
      }
      else {
      }    
    }    
  }

  typedef union
  {
    unsigned char Byte[2];
    int16_t Int16;
    uint16_t UInt16;
    unsigned char UChar;
    char Char;
  }TwoByte;};