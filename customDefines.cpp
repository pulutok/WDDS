//
// Created by skyclad on 1/24/17.
//
#include <sys/param.h>
#include <string>

#define MAX_BUF_LEN 65536

#pragma pack(push, 1)
typedef struct beacon_frame {
    u_int16_t frame_control;
    u_int16_t duration;
    u_char dest[6];
    u_char src[6];
    u_char bss[6];
    u_int16_t seq_control;
} BF;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct ieee80211_wlan_management {
    u_int64_t timestamp;
    u_int16_t beacon_interval;
    u_int16_t capabilities_information;
} IEEE80211 ;
#pragma pack(pop)

struct wdds_log {
    u_int timestamp;
    std::string src_mac;
    std::string dst_mac;
};