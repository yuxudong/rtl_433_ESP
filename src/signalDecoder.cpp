/*
  rtl_433_ESP - 433.92 MHz protocols library for ESP32

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 3 of the License, or (at your option) any later version.
  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with library. If not, see <http://www.gnu.org/licenses/>


  Project Structure

  rtl_433_ESP - Main Class
  decoder.cpp - Wrapper and interface for the rtl_433 classes
  receiver.cpp - Wrapper and interface for RadioLib
  rtl_433 - subset of rtl_433 package

*/

#include "signalDecoder.h"

/*----------------------------- rtl_433_ESP Internals -----------------------------*/

#ifndef rtl_433_Decoder_Stack
#  if defined(RTL_ANALYZER) || defined(RTL_ANALYZE)
#    define rtl_433_Decoder_Stack 60000
#  elif defined(RTL_VERBOSE) || defined(RTL_DEBUG)
#    define rtl_433_Decoder_Stack 30000
#  else
#    if OOK_MODULATION
#      define rtl_433_Decoder_Stack 11500
#    else
#      define rtl_433_Decoder_Stack 20000
#    endif
#  endif
#endif

#define rtl_433_Decoder_Priority 2
#define rtl_433_Decoder_Core     1

/*----------------------------- rtl_433_ESP Internals -----------------------------*/

int rtlVerbose = 0;

r_cfg_t g_cfg; // Global config object

TaskHandle_t rtl_433_DecoderHandle;
static QueueHandle_t rtl_433_Queue;

void rtlSetup() {
  r_cfg_t* cfg = &g_cfg;

#ifdef MEMORY_DEBUG
  logprintfLn(LOG_DEBUG, "sizeof(*cfg->demod) %d", sizeof(*cfg->demod));
#endif

  if (!cfg->demod) {
    r_init_cfg(cfg);
    add_log_output(cfg, NULL);
#ifdef MEMORY_DEBUG
    logprintfLn(LOG_DEBUG, "sizeof(cfg) %d, heap %d", sizeof(cfg),
                ESP.getFreeHeap());
#endif
    cfg->conversion_mode = CONVERT_SI; // Default all output to Celsius
    if (rtl_433_ESP::ookModulation) {
      cfg->num_r_devices = NUMOF_OOK_DEVICES;
    } else {
      cfg->num_r_devices = NUMOF_FSK_DEVICES;
    }
    cfg->devices = (r_device*)calloc(cfg->num_r_devices, sizeof(r_device));
    if (!cfg->devices)
      FATAL_CALLOC("cfg->devices");

#ifdef MEMORY_DEBUG
    logprintfLn(LOG_DEBUG, "sizeof(cfg) %d, heap %d", sizeof(cfg),
                ESP.getFreeHeap());
#endif

#ifndef MY_DEVICES
  // This is a generated fragment from tools/update_rtl_433_devices.sh

  if (rtl_433_ESP::ookModulation) {
    memcpy(&cfg->devices[0], &abmt, sizeof(r_device));
    memcpy(&cfg->devices[1], &acurite_rain_896, sizeof(r_device));
    memcpy(&cfg->devices[2], &acurite_th, sizeof(r_device));
    memcpy(&cfg->devices[3], &acurite_txr, sizeof(r_device));
    memcpy(&cfg->devices[4], &acurite_986, sizeof(r_device));
    memcpy(&cfg->devices[5], &acurite_606, sizeof(r_device));
    memcpy(&cfg->devices[6], &acurite_00275rm, sizeof(r_device));
    memcpy(&cfg->devices[7], &acurite_590tx, sizeof(r_device));
    memcpy(&cfg->devices[8], &acurite_01185m, sizeof(r_device));
    memcpy(&cfg->devices[9], &advent_doorbell, sizeof(r_device));
    memcpy(&cfg->devices[10], &akhan_100F14, sizeof(r_device));
    memcpy(&cfg->devices[11], &alectov1, sizeof(r_device));
    memcpy(&cfg->devices[12], &ambient_weather, sizeof(r_device));
    memcpy(&cfg->devices[13], &ambientweather_tx8300, sizeof(r_device));
    memcpy(&cfg->devices[14], &atech_ws308, sizeof(r_device));
    memcpy(&cfg->devices[15], &auriol_4ld5661, sizeof(r_device));
    memcpy(&cfg->devices[16], &auriol_aft77b2, sizeof(r_device));
    memcpy(&cfg->devices[17], &auriol_afw2a1, sizeof(r_device));
    memcpy(&cfg->devices[18], &auriol_ahfl, sizeof(r_device));
    memcpy(&cfg->devices[19], &auriol_hg02832, sizeof(r_device));
    memcpy(&cfg->devices[20], &baldr_rain, sizeof(r_device));
    memcpy(&cfg->devices[21], &blyss, sizeof(r_device));
    memcpy(&cfg->devices[22], &brennenstuhl_rcs_2044, sizeof(r_device));
    memcpy(&cfg->devices[23], &bresser_3ch, sizeof(r_device));
    memcpy(&cfg->devices[24], &bresser_st1005h, sizeof(r_device));
    memcpy(&cfg->devices[25], &bt_rain, sizeof(r_device));
    memcpy(&cfg->devices[26], &burnhardbbq, sizeof(r_device));
    memcpy(&cfg->devices[27], &calibeur_RF104, sizeof(r_device));
    memcpy(&cfg->devices[28], &cardin, sizeof(r_device));
    memcpy(&cfg->devices[29], &celsia_czc1, sizeof(r_device));
    memcpy(&cfg->devices[30], &chuango, sizeof(r_device));
    memcpy(&cfg->devices[31], &cmr113, sizeof(r_device));
    memcpy(&cfg->devices[32], &companion_wtr001, sizeof(r_device));
    memcpy(&cfg->devices[33], &cotech_36_7959, sizeof(r_device));
    memcpy(&cfg->devices[34], &digitech_xc0324, sizeof(r_device));
    memcpy(&cfg->devices[35], &dish_remote_6_3, sizeof(r_device));
    memcpy(&cfg->devices[36], &dooya_curtain, sizeof(r_device));
    memcpy(&cfg->devices[37], &dsc_security, sizeof(r_device));
    memcpy(&cfg->devices[38], &dsc_security_ws4945, sizeof(r_device));
    memcpy(&cfg->devices[39], &ecowitt, sizeof(r_device));
    memcpy(&cfg->devices[40], &eurochron_efth800, sizeof(r_device));
    memcpy(&cfg->devices[41], &elro_db286a, sizeof(r_device));
    memcpy(&cfg->devices[42], &elv_em1000, sizeof(r_device));
    memcpy(&cfg->devices[43], &elv_ws2000, sizeof(r_device));
    memcpy(&cfg->devices[44], &emos_e6016, sizeof(r_device));
    memcpy(&cfg->devices[45], &emos_e6016_rain, sizeof(r_device));
    memcpy(&cfg->devices[46], &enocean_erp1, sizeof(r_device));
    memcpy(&cfg->devices[47], &ert_idm, sizeof(r_device));
    memcpy(&cfg->devices[48], &ert_netidm, sizeof(r_device));
    memcpy(&cfg->devices[49], &ert_scm, sizeof(r_device));
    memcpy(&cfg->devices[50], &esa_energy, sizeof(r_device));
    memcpy(&cfg->devices[51], &esperanza_ews, sizeof(r_device));
    memcpy(&cfg->devices[52], &eurochron, sizeof(r_device));
    memcpy(&cfg->devices[53], &fineoffset_WH2, sizeof(r_device));
    memcpy(&cfg->devices[54], &fineoffset_WH0530, sizeof(r_device));
    memcpy(&cfg->devices[55], &fineoffset_wh1050, sizeof(r_device));
    memcpy(&cfg->devices[56], &fineoffset_wh1080, sizeof(r_device));
    memcpy(&cfg->devices[57], &fordremote, sizeof(r_device));
    memcpy(&cfg->devices[58], &fs20, sizeof(r_device));
    memcpy(&cfg->devices[59], &ft004b, sizeof(r_device));
    memcpy(&cfg->devices[60], &funkbus_remote, sizeof(r_device));
    memcpy(&cfg->devices[61], &gasmate_ba1008, sizeof(r_device));
    memcpy(&cfg->devices[62], &geevon, sizeof(r_device));
    memcpy(&cfg->devices[63], &generic_motion, sizeof(r_device));
    memcpy(&cfg->devices[64], &generic_remote, sizeof(r_device));
    memcpy(&cfg->devices[65], &generic_temperature_sensor, sizeof(r_device));
    memcpy(&cfg->devices[66], &govee, sizeof(r_device));
    memcpy(&cfg->devices[67], &govee_h5054, sizeof(r_device));
    memcpy(&cfg->devices[68], &gt_tmbbq05, sizeof(r_device));
    memcpy(&cfg->devices[69], &gt_wt_02, sizeof(r_device));
    memcpy(&cfg->devices[70], &gt_wt_03, sizeof(r_device));
    memcpy(&cfg->devices[71], &hcs200, sizeof(r_device));
    memcpy(&cfg->devices[72], &hideki_ts04, sizeof(r_device));
    memcpy(&cfg->devices[73], &honeywell, sizeof(r_device));
    memcpy(&cfg->devices[74], &honeywell_wdb, sizeof(r_device));
    memcpy(&cfg->devices[75], &ht680, sizeof(r_device));
    memcpy(&cfg->devices[76], &ibis_beacon, sizeof(r_device));
    memcpy(&cfg->devices[77], &infactory, sizeof(r_device));
    memcpy(&cfg->devices[78], &kw9015b, sizeof(r_device));
    memcpy(&cfg->devices[79], &interlogix, sizeof(r_device));
    memcpy(&cfg->devices[80], &intertechno, sizeof(r_device));
    memcpy(&cfg->devices[81], &jasco, sizeof(r_device));
    memcpy(&cfg->devices[82], &kedsum, sizeof(r_device));
    memcpy(&cfg->devices[83], &kerui, sizeof(r_device));
    memcpy(&cfg->devices[84], &klimalogg, sizeof(r_device));
    memcpy(&cfg->devices[85], &lacrossetx, sizeof(r_device));
    memcpy(&cfg->devices[86], &lacrosse_tx141x, sizeof(r_device));
    memcpy(&cfg->devices[87], &lacrosse_ws7000, sizeof(r_device));
    memcpy(&cfg->devices[88], &lacrossews, sizeof(r_device));
    memcpy(&cfg->devices[89], &lightwave_rf, sizeof(r_device));
    memcpy(&cfg->devices[90], &markisol, sizeof(r_device));
    memcpy(&cfg->devices[91], &maverick_et73, sizeof(r_device));
    memcpy(&cfg->devices[92], &maverick_et73x, sizeof(r_device));
    memcpy(&cfg->devices[93], &mebus433, sizeof(r_device));
    memcpy(&cfg->devices[94], &megacode, sizeof(r_device));
    memcpy(&cfg->devices[95], &missil_ml0757, sizeof(r_device));
    memcpy(&cfg->devices[96], &neptune_r900, sizeof(r_device));
    memcpy(&cfg->devices[97], &new_template, sizeof(r_device));
    memcpy(&cfg->devices[98], &newkaku, sizeof(r_device));
    memcpy(&cfg->devices[99], &nexa, sizeof(r_device));
    memcpy(&cfg->devices[100], &nexus, sizeof(r_device));
    memcpy(&cfg->devices[101], &nice_flor_s, sizeof(r_device));
    memcpy(&cfg->devices[102], &norgo, sizeof(r_device));
    memcpy(&cfg->devices[103], &oil_standard_ask, sizeof(r_device));
    memcpy(&cfg->devices[104], &opus_xt300, sizeof(r_device));
    memcpy(&cfg->devices[105], &oregon_scientific, sizeof(r_device));
    memcpy(&cfg->devices[106], &oregon_scientific_sl109h, sizeof(r_device));
    memcpy(&cfg->devices[107], &oregon_scientific_v1, sizeof(r_device));
    memcpy(&cfg->devices[108], &philips_aj3650, sizeof(r_device));
    memcpy(&cfg->devices[109], &philips_aj7010, sizeof(r_device));
    memcpy(&cfg->devices[110], &proflame2, sizeof(r_device));
    memcpy(&cfg->devices[111], &prologue, sizeof(r_device));
    memcpy(&cfg->devices[112], &proove, sizeof(r_device));
    memcpy(&cfg->devices[113], &quhwa, sizeof(r_device));
    memcpy(&cfg->devices[114], &radiohead_ask, sizeof(r_device));
    memcpy(&cfg->devices[115], &sensible_living, sizeof(r_device));
    memcpy(&cfg->devices[116], &rainpoint, sizeof(r_device));
    memcpy(&cfg->devices[117], &regency_fan, sizeof(r_device));
    memcpy(&cfg->devices[118], &revolt_nc5462, sizeof(r_device));
    memcpy(&cfg->devices[119], &revolt_zx7717, sizeof(r_device));
    memcpy(&cfg->devices[120], &rftech, sizeof(r_device));
    memcpy(&cfg->devices[121], &risco_agility, sizeof(r_device));
    memcpy(&cfg->devices[122], &rosstech_dcu706, sizeof(r_device));
    memcpy(&cfg->devices[123], &rubicson, sizeof(r_device));
    memcpy(&cfg->devices[124], &rubicson_48659, sizeof(r_device));
    memcpy(&cfg->devices[125], &rubicson_pool_48942, sizeof(r_device));
    memcpy(&cfg->devices[126], &s3318p, sizeof(r_device));
    memcpy(&cfg->devices[127], &schou_72543_rain, sizeof(r_device));
    memcpy(&cfg->devices[128], &schraeder, sizeof(r_device));
    memcpy(&cfg->devices[129], &schrader_EG53MA4, sizeof(r_device));
    memcpy(&cfg->devices[130], &schrader_SMD3MA4, sizeof(r_device));
    memcpy(&cfg->devices[131], &scmplus, sizeof(r_device));
    memcpy(&cfg->devices[132], &scve_door, sizeof(r_device));
    memcpy(&cfg->devices[133], &secplus_v1, sizeof(r_device));
    memcpy(&cfg->devices[134], &silvercrest, sizeof(r_device));
    memcpy(&cfg->devices[135], &ss_sensor, sizeof(r_device));
    memcpy(&cfg->devices[136], &skylink_motion, sizeof(r_device));
    memcpy(&cfg->devices[137], &smoke_gs558, sizeof(r_device));
    memcpy(&cfg->devices[138], &solight_te44, sizeof(r_device));
    memcpy(&cfg->devices[139], &somfy_rts, sizeof(r_device));
    memcpy(&cfg->devices[140], &springfield, sizeof(r_device));
    memcpy(&cfg->devices[141], &telldus_ft0385r, sizeof(r_device));
    memcpy(&cfg->devices[142], &tfa_30_3221, sizeof(r_device));
    memcpy(&cfg->devices[143], &tfa_drop_303233, sizeof(r_device));
    memcpy(&cfg->devices[144], &tfa_pool_thermometer, sizeof(r_device));
    memcpy(&cfg->devices[145], &tfa_twin_plus_303049, sizeof(r_device));
    memcpy(&cfg->devices[146], &thermopro_tp11, sizeof(r_device));
    memcpy(&cfg->devices[147], &thermopro_tp12, sizeof(r_device));
    memcpy(&cfg->devices[148], &thermopro_tx2, sizeof(r_device));
    memcpy(&cfg->devices[149], &thermopro_tx2c, sizeof(r_device));
    memcpy(&cfg->devices[150], &thermor, sizeof(r_device));
    memcpy(&cfg->devices[151], &tpms_eezrv, sizeof(r_device));
    memcpy(&cfg->devices[152], &tpms_gm, sizeof(r_device));
    memcpy(&cfg->devices[153], &tpms_tyreguard400, sizeof(r_device));
    memcpy(&cfg->devices[154], &ts_ft002, sizeof(r_device));
    memcpy(&cfg->devices[155], &ttx201, sizeof(r_device));
    memcpy(&cfg->devices[156], &vaillant_vrt340f, sizeof(r_device));
    memcpy(&cfg->devices[157], &vauno_en8822c, sizeof(r_device));
    memcpy(&cfg->devices[158], &visonic_powercode, sizeof(r_device));
    memcpy(&cfg->devices[159], &watts_thermostat, sizeof(r_device));
    memcpy(&cfg->devices[160], &waveman, sizeof(r_device));
    memcpy(&cfg->devices[161], &wec2103, sizeof(r_device));
    memcpy(&cfg->devices[162], &wg_pb12v1, sizeof(r_device));
    memcpy(&cfg->devices[163], &ws2032, sizeof(r_device));
    memcpy(&cfg->devices[164], &wssensor, sizeof(r_device));
    memcpy(&cfg->devices[165], &wt1024, sizeof(r_device));
    memcpy(&cfg->devices[166], &wt450, sizeof(r_device));
    memcpy(&cfg->devices[167], &X10_RF, sizeof(r_device));
    memcpy(&cfg->devices[168], &x10_sec, sizeof(r_device));
    memcpy(&cfg->devices[169], &yale_hsa, sizeof(r_device));
  } else {
    memcpy(&cfg->devices[0], &ambientweather_wh31e, sizeof(r_device));
    memcpy(&cfg->devices[1], &ant_antplus, sizeof(r_device));
    memcpy(&cfg->devices[2], &arad_ms_meter, sizeof(r_device));
    memcpy(&cfg->devices[3], &archos_tbh, sizeof(r_device));
    memcpy(&cfg->devices[4], &arexx_ml, sizeof(r_device));
    memcpy(&cfg->devices[5], &badger_orion, sizeof(r_device));
    memcpy(&cfg->devices[6], &bresser_5in1, sizeof(r_device));
    memcpy(&cfg->devices[7], &bresser_6in1, sizeof(r_device));
    memcpy(&cfg->devices[8], &bresser_7in1, sizeof(r_device));
    memcpy(&cfg->devices[9], &bresser_leakage, sizeof(r_device));
    memcpy(&cfg->devices[10], &bresser_lightning, sizeof(r_device));
    memcpy(&cfg->devices[11], &cavius, sizeof(r_device));
    memcpy(&cfg->devices[12], &ced7000, sizeof(r_device));
    memcpy(&cfg->devices[13], &chamberlain_cwpirc, sizeof(r_device));
    memcpy(&cfg->devices[14], &current_cost, sizeof(r_device));
    memcpy(&cfg->devices[15], &danfoss_CFR, sizeof(r_device));
    memcpy(&cfg->devices[16], &deltadore_x3d, sizeof(r_device));
    memcpy(&cfg->devices[17], &directv, sizeof(r_device));
    memcpy(&cfg->devices[18], &ecodhome, sizeof(r_device));
    memcpy(&cfg->devices[19], &efergy_e2_classic, sizeof(r_device));
    memcpy(&cfg->devices[20], &efergy_optical, sizeof(r_device));
    memcpy(&cfg->devices[21], &emax, sizeof(r_device));
    memcpy(&cfg->devices[22], &emontx, sizeof(r_device));
    memcpy(&cfg->devices[23], &esic_emt7110, sizeof(r_device));
    memcpy(&cfg->devices[24], &fineoffset_WH25, sizeof(r_device));
    memcpy(&cfg->devices[25], &fineoffset_WH51, sizeof(r_device));
    memcpy(&cfg->devices[26], &tfa_303151, sizeof(r_device));
    memcpy(&cfg->devices[27], &fineoffset_wh1080_fsk, sizeof(r_device));
    memcpy(&cfg->devices[28], &fineoffset_wh31l, sizeof(r_device));
    memcpy(&cfg->devices[29], &fineoffset_wh45, sizeof(r_device));
    memcpy(&cfg->devices[30], &fineoffset_wh46, sizeof(r_device));
    memcpy(&cfg->devices[31], &fineoffset_wh55, sizeof(r_device));
    memcpy(&cfg->devices[32], &fineoffset_wn34, sizeof(r_device));
    memcpy(&cfg->devices[33], &fineoffset_ws80, sizeof(r_device));
    memcpy(&cfg->devices[34], &fineoffset_ws90, sizeof(r_device));
    memcpy(&cfg->devices[35], &flowis, sizeof(r_device));
    memcpy(&cfg->devices[36], &ge_coloreffects, sizeof(r_device));
    memcpy(&cfg->devices[37], &geo_minim, sizeof(r_device));
    memcpy(&cfg->devices[38], &gridstream96, sizeof(r_device));
    memcpy(&cfg->devices[39], &gridstream192, sizeof(r_device));
    memcpy(&cfg->devices[40], &gridstream384, sizeof(r_device));
    memcpy(&cfg->devices[41], &hcs200_fsk, sizeof(r_device));
    memcpy(&cfg->devices[42], &holman_ws5029pcm, sizeof(r_device));
    memcpy(&cfg->devices[43], &holman_ws5029pwm, sizeof(r_device));
    memcpy(&cfg->devices[44], &hondaremote, sizeof(r_device));
    memcpy(&cfg->devices[45], &honeywell_cm921, sizeof(r_device));
    memcpy(&cfg->devices[46], &honeywell_wdb_fsk, sizeof(r_device));
    memcpy(&cfg->devices[47], &ikea_sparsnas, sizeof(r_device));
    memcpy(&cfg->devices[48], &inkbird_ith20r, sizeof(r_device));
    memcpy(&cfg->devices[49], &insteon, sizeof(r_device));
    memcpy(&cfg->devices[50], &lacrosse_breezepro, sizeof(r_device));
    memcpy(&cfg->devices[51], &lacrosse_r1, sizeof(r_device));
    memcpy(&cfg->devices[52], &lacrosse_th3, sizeof(r_device));
    memcpy(&cfg->devices[53], &lacrosse_tx31u, sizeof(r_device));
    memcpy(&cfg->devices[54], &lacrosse_tx34, sizeof(r_device));
    memcpy(&cfg->devices[55], &lacrosse_tx29, sizeof(r_device));
    memcpy(&cfg->devices[56], &lacrosse_tx35, sizeof(r_device));
    memcpy(&cfg->devices[57], &lacrosse_wr1, sizeof(r_device));
    memcpy(&cfg->devices[58], &m_bus_mode_c_t, sizeof(r_device));
    memcpy(&cfg->devices[59], &m_bus_mode_c_t_downlink, sizeof(r_device));
    memcpy(&cfg->devices[60], &m_bus_mode_s, sizeof(r_device));
    memcpy(&cfg->devices[61], &m_bus_mode_r, sizeof(r_device));
    memcpy(&cfg->devices[62], &m_bus_mode_f, sizeof(r_device));
    memcpy(&cfg->devices[63], &marlec_solar, sizeof(r_device));
    memcpy(&cfg->devices[64], &maverick_xr30, sizeof(r_device));
    memcpy(&cfg->devices[65], &mueller_hotrod, sizeof(r_device));
    memcpy(&cfg->devices[66], &oil_smart, sizeof(r_device));
    memcpy(&cfg->devices[67], &oil_standard, sizeof(r_device));
    memcpy(&cfg->devices[68], &oil_watchman, sizeof(r_device));
    memcpy(&cfg->devices[69], &oil_watchman_advanced, sizeof(r_device));
    memcpy(&cfg->devices[70], &quinetic, sizeof(r_device));
    memcpy(&cfg->devices[71], &rojaflex, sizeof(r_device));
    memcpy(&cfg->devices[72], &sharp_spc775, sizeof(r_device));
    memcpy(&cfg->devices[73], &simplisafe_gen3, sizeof(r_device));
    memcpy(&cfg->devices[74], &somfy_iohc, sizeof(r_device));
    memcpy(&cfg->devices[75], &srsmith_pool_srs_2c_tx, sizeof(r_device));
    memcpy(&cfg->devices[76], &steelmate, sizeof(r_device));
    memcpy(&cfg->devices[77], &tfa_14_1504_v2, sizeof(r_device));
    memcpy(&cfg->devices[78], &tfa_303196, sizeof(r_device));
    memcpy(&cfg->devices[79], &tfa_marbella, sizeof(r_device));
    memcpy(&cfg->devices[80], &thermopro_tp28b, sizeof(r_device));
    memcpy(&cfg->devices[81], &thermopro_tp828b, sizeof(r_device));
    memcpy(&cfg->devices[82], &thermopro_tp829b, sizeof(r_device));
    memcpy(&cfg->devices[83], &tpms_abarth124, sizeof(r_device));
    memcpy(&cfg->devices[84], &tpms_ave, sizeof(r_device));
    memcpy(&cfg->devices[85], &tpms_bmw, sizeof(r_device));
    memcpy(&cfg->devices[86], &tpms_bmwg3, sizeof(r_device));
    memcpy(&cfg->devices[87], &tpms_citroen, sizeof(r_device));
    memcpy(&cfg->devices[88], &tpms_elantra2012, sizeof(r_device));
    memcpy(&cfg->devices[89], &tpms_ford, sizeof(r_device));
    memcpy(&cfg->devices[90], &tpms_hyundai_vdo, sizeof(r_device));
    memcpy(&cfg->devices[91], &tpms_jansite, sizeof(r_device));
    memcpy(&cfg->devices[92], &tpms_jansite_solar, sizeof(r_device));
    memcpy(&cfg->devices[93], &tpms_kia, sizeof(r_device));
    memcpy(&cfg->devices[94], &tpms_nissan, sizeof(r_device));
    memcpy(&cfg->devices[95], &tpms_pmv107j, sizeof(r_device));
    memcpy(&cfg->devices[96], &tpms_porsche, sizeof(r_device));
    memcpy(&cfg->devices[97], &tpms_renault, sizeof(r_device));
    memcpy(&cfg->devices[98], &tpms_renault_0435r, sizeof(r_device));
    memcpy(&cfg->devices[99], &tpms_toyota, sizeof(r_device));
    memcpy(&cfg->devices[100], &tpms_truck, sizeof(r_device));
    memcpy(&cfg->devices[101], &vevor_7in1, sizeof(r_device));
  }
  
    // end of fragment

#else
    memcpy(&cfg->devices[0], &lacrosse_tx141x, sizeof(r_device));
#endif

#ifdef RTL_FLEX
    // This option is non-functional. The flex decoder is too resource intensive
    // for an ESP32, and needs the ESP32 stack set to 32768 in order for the
    // flex_callback to execute Tested with
    // -DRTL_FLEX="n=Sonoff-PIR3-RF,m=OOK_PWM,s=300,l=860,r=7492,g=868,t=50,y=0,bits>=24,repeats>=5,invert,get=@0:{20}:id,get=@20:{4}:motion:[0:true
    // ],unique"

    r_device* flex_device;
    flex_device = flex_create_device(RTL_FLEX);
    memcpy(&cfg->devices[101], &flex_device, sizeof(r_device));
    register_protocol(cfg, flex_device, NULL);
    alogprintfLn(LOG_INFO, "Flex Decoder enabled: %s", RTL_FLEX);
#endif

// logprintfLn(LOG_INFO, "Location of r_devices: %p", (void *)&r_devices);
// logprintfLn(LOG_INFO, "Location of cfg: %p", (void *)&cfg);
// logprintfLn(LOG_INFO, "cfg size %d", sizeof(r_cfg_t));
// logprintfLn(LOG_INFO, "Location of cfg->devices: %p", (void *)&cfg->devices);
#ifdef MEMORY_DEBUG
    logprintfLn(LOG_INFO, "size of bitbuffer: %d", sizeof(bitbuffer_t));
    logprintfLn(LOG_INFO, "size of pulse_data: %d", sizeof(pulse_data_t));
#endif

#ifdef DEMOD_DEBUG
    logprintfLn(LOG_INFO, "# of device(s) configured %d", cfg->num_r_devices);
    logprintfLn(LOG_INFO, "ssizeof(r_device): %d", sizeof(r_device));
    logprintfLn(LOG_INFO, "cfg->devices size: %d",
                sizeof(r_device) * cfg->num_r_devices);
#endif
#ifdef RTL_DEBUG
    cfg->verbosity = RTL_DEBUG + 5; // 0=normal, 1=verbose, 2=verbose decoders,
        // 3=debug decoders, 4=trace decoding.
#else
    cfg->verbosity = rtlVerbose; // 0=normal, 1=verbose, 2=verbose decoders,
        // 3=debug decoders, 4=trace decoding.
#endif

#ifdef MEMORY_DEBUG
    logprintfLn(LOG_DEBUG, "Pre register_all_protocols heap %d",
                ESP.getFreeHeap());
#endif

    // expand register_all_protocols to determine heap impact from each decoder
    // register_all_protocols(cfg, 0);

    for (int i = 0; i < cfg->num_r_devices; i++) {
      // register all device protocols that are not disabled
      cfg->devices[i].protocol_num = i;
#ifdef MEMORY_DEBUG
      logprintfLn(LOG_DEBUG, "Pre register_protocol %d %s, heap %d", i,
                  cfg->devices[i].name, ESP.getFreeHeap());
#endif
#ifdef RESOURCE_DEBUG
      int preStack = uxTaskGetStackHighWaterMark(NULL);
      int preHeap = ESP.getFreeHeap();
#endif

      char* arg = NULL;
      char verbose[4] = "vvv";
#ifndef RTL_VERBOSE
#  define RTL_VERBOSE -1
#endif
      if (RTL_VERBOSE && i == RTL_VERBOSE) {
        arg = verbose;
      }
      if (cfg->devices[i].disabled <= 0) {
        register_protocol(cfg, &cfg->devices[i], arg);
      }
#ifdef RESOURCE_DEBUG
      int deltaStack = preStack - uxTaskGetStackHighWaterMark(NULL);
      int deltaHeap = preHeap - ESP.getFreeHeap();
      if (deltaStack || (deltaHeap > 200)) {
        logprintfLn(LOG_DEBUG, "Process rtl_433_DecoderTask resource hit %s, deltaStack: %d, stack: %u, deltaHeap: %d, heap: %d", cfg->devices[i].name,
                    deltaStack, uxTaskGetStackHighWaterMark(NULL), deltaHeap, ESP.getFreeHeap());
      }
#endif
    }

#ifdef MEMORY_DEBUG
    logprintfLn(LOG_DEBUG, "Pre xQueueCreate heap %d", ESP.getFreeHeap());
#endif
    rtl_433_Queue = xQueueCreate(5, sizeof(pulse_data_t*));

#ifdef MEMORY_DEBUG
    logprintfLn(LOG_DEBUG, "Pre xTaskCreatePinnedToCore heap %d",
                ESP.getFreeHeap());
#endif
#ifdef RESOURCE_DEBUG
    logprintfLn(LOG_INFO, "rtl_433_Decoder_Stack %d", rtl_433_Decoder_Stack);
#endif

    xTaskCreatePinnedToCore(
        rtl_433_DecoderTask, /* Function to implement the task */
        "rtl_433_DecoderTask", /* Name of the task */
        rtl_433_Decoder_Stack, /* Stack size in bytes */
        NULL, /* Task input parameter */
        rtl_433_Decoder_Priority, /* Priority of the task (set lower than core task) */
        &rtl_433_DecoderHandle, /* Task handle. */
        rtl_433_Decoder_Core); /* Core where the task should run */
  }
}

void _setCallback(rtl_433_ESPCallBack callback, char* messageBuffer,
                  int bufferSize) {
  // logprintfLn(LOG_DEBUG, "_setCallback location: %p", callback);

  r_cfg_t* cfg = &g_cfg;
  cfg->callback = callback;
  cfg->messageBuffer = messageBuffer;
  cfg->bufferSize = bufferSize;
}

void _setDebug(int debug) {
  rtlVerbose = debug;
  logprintfLn(LOG_INFO, "Setting rtl_433 debug to: %d", rtlVerbose);
}

// ---------------------------------------------------------------------------------------------------------

void rtl_433_DecoderTask(void* pvParameters) {
  pulse_data_t* rtl_pulses = nullptr;
  for (;;) {
    // logprintfLn(LOG_DEBUG, "rtl_433_DecoderTask awaiting signal");
    xQueueReceive(rtl_433_Queue, &rtl_pulses, portMAX_DELAY);
    // logprintfLn(LOG_DEBUG, "rtl_433_DecoderTask signal received");
#ifdef MEMORY_DEBUG
    unsigned long signalProcessingStart = micros();
#endif

#ifdef RAW_SIGNAL_DEBUG
    logprintf(LOG_INFO, "RAW (%lu): ", rtl_pulses->signalDuration);
    for (int i = 0; i < rtl_pulses->num_pulses; i++) {
      alogprintf(LOG_INFO, "+%d", rtl_pulses->pulse[i]);
      alogprintf(LOG_INFO, "-%d", rtl_pulses->gap[i]);
#  ifdef SIGNAL_RSSI
      alogprintf(LOG_INFO, "(%d)", rtl_pulses->rssi[i]);
#  endif
    }
    alogprintfLn(LOG_INFO, " ");
#endif
#ifdef MEMORY_DEBUG
    logprintfLn(LOG_INFO, "Pre run_%s_demods: %d", rtl_433_ESP::ookModulation ? "OOK" : "FSK", ESP.getFreeHeap());
#endif
    rtl_pulses->sample_rate = 1.0e6;
    r_cfg_t* cfg = &g_cfg;
    cfg->demod->pulse_data = *rtl_pulses;
    int events = 0;

    if (rtl_433_ESP::ookModulation) {
      events = run_ook_demods(&cfg->demod->r_devs, rtl_pulses);
    } else {
      events = run_fsk_demods(&cfg->demod->r_devs, rtl_pulses);
    }
    if (events == 0) {
#ifdef RTL_ANALYZER
      pulse_analyzer(rtl_pulses, rtl_433_ESP::ookModulation ? 1 : 2);
#endif
      rtl_433_ESP::unparsedSignals++;
#ifdef PUBLISH_UNPARSED
      logprintf(LOG_INFO, "Unparsed Signal length: %lu",
                rtl_pulses->signalDuration);
      alogprintf(LOG_INFO, ", Signal RSSI: %d", rtl_pulses->signalRssi);
      //      alogprintf(LOG_INFO, ", train: %d", _actualPulseTrain);
      //      alogprintf(LOG_INFO, ", messageCount: %d", messageCount);
      alogprintfLn(LOG_INFO, ", pulses: %d", rtl_pulses->num_pulses);

      logprintf(LOG_INFO, "RAW (%lu): ", rtl_pulses->signalDuration);
#  ifndef RAW_SIGNAL_DEBUG
      for (int i = 0; i < rtl_pulses->num_pulses; i++) {
        alogprintf(LOG_INFO, "+%d", rtl_pulses->pulse[i]);
        alogprintf(LOG_INFO, "-%d", rtl_pulses->gap[i]);
#    ifdef SIGNAL_RSSI
        alogprintf(LOG_INFO, "(%d)", rtl_pulses->rssi[i]);
#    endif
      }
      alogprintfLn(LOG_INFO, " ");
#  endif

      // Send a note saying unparsed signal signal received
      data_t* data;
      /* clang-format off */
  data = data_make(
                "model", "",      DATA_STRING,  "undecoded signal",
                "protocol", "",   DATA_STRING,  "signal parsing failed",
                "duration", "",   DATA_INT,     rtl_pulses->signalDuration,
                "rssi", "", DATA_INT,     rtl_pulses->signalRssi,
                "pulses", "",     DATA_INT,     rtl_pulses->num_pulses,
//                "train", "",      DATA_INT,     _actualPulseTrain,
//                "messageCount", "", DATA_INT,   messageCount,
//                "_enabledReceiver", "", DATA_INT, _enabledReceiver,
//                "receiveMode", "", DATA_INT,    receiveMode,
//                "currentRssi", "", DATA_INT,    currentRssi,
//                "rssiThreshold", "", DATA_INT,    rssiThreshold,
                NULL);
      /* clang-format on */

      r_cfg_t* cfg = &g_cfg;
      data_print_jsons(data, cfg->messageBuffer, cfg->bufferSize);
      (cfg->callback)(cfg->messageBuffer);
      data_free(data);

#endif
    }

#ifdef MEMORY_DEBUG
    logprintfLn(LOG_INFO, "Signal processing time: %lu",
                micros() - signalProcessingStart);
    logprintfLn(LOG_INFO, "Post run_ook_demods memory %d", ESP.getFreeHeap());
#endif
#ifdef DEMOD_DEBUG
    logprintfLn(LOG_INFO, "# of messages decoded %d", events);
#endif
    if (events > 0) {
      // alogprintfLn(LOG_INFO, " ");
    }
#if defined(MEMORY_DEBUG)
    else {
      logprintfLn(LOG_DEBUG, "Process rtl_433_DecoderTask stack free: %u",
                  uxTaskGetStackHighWaterMark(rtl_433_DecoderHandle));
      alogprintfLn(LOG_INFO, " ");
    }
#endif
#ifdef MEMORY_DEBUG
    logprintfLn(LOG_INFO, "Pre free rtl_433_DecoderTask: %d",
                ESP.getFreeHeap());
#endif
    free(rtl_pulses);
#ifdef MEMORY_DEBUG
    logprintfLn(LOG_INFO, "Post free rtl_433_DecoderTask: %d",
                ESP.getFreeHeap());
    logprintfLn(LOG_INFO, "rtl_433_DecoderTask uxTaskGetStackHighWaterMark: %d",
                uxTaskGetStackHighWaterMark(NULL));
#endif
  }
}

void processSignal(pulse_data_t* rtl_pulses) {
  // logprintfLn(LOG_DEBUG, "processSignal() about to place signal on
  // rtl_433_Queue");
  if (xQueueSend(rtl_433_Queue, &rtl_pulses, 0) != pdTRUE) {
    logprintfLn(LOG_ERR, "ERROR: rtl_433_Queue full, discarding signal");
    free(rtl_pulses);
  } else {
    // logprintfLn(LOG_DEBUG, "processSignal() signal placed on rtl_433_Queue");
  }
}
