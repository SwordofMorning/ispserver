#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <microhttpd.h>
#include <json-c/json.h>

#include "mediactl/mediactl.h"
#include "rk_aiq_uapi_imgproc_ipc_ipc.h"
#include "rk_aiq_user_api_a3dlut_ipc.h"
#include "rk_aiq_user_api_ablc_ipc.h"
#include "rk_aiq_user_api_accm_ipc.h"
#include "rk_aiq_user_api_adebayer_ipc.h"
#include "rk_aiq_user_api_adehaze_ipc.h"
#include "rk_aiq_user_api_adpcc_ipc.h"
#include "rk_aiq_user_api_ae_ipc.h"
#include "rk_aiq_user_api_agamma_ipc.h"
#include "rk_aiq_user_api_ahdr_ipc.h"
#include "rk_aiq_user_api_alsc_ipc.h"
#include "rk_aiq_user_api_anr_ipc.h"
#include "rk_aiq_user_api_asharp_ipc.h"
#include "rk_aiq_user_api_awb_ipc.h"
#include "rk_aiq_user_api_imgproc.h"
#include "rk_aiq_user_api_sysctl_ipc.h"

#ifdef __cplusplus
extern "C" {
#endif

#define PORT 8888
#define POSTBUFFERSIZE 512

struct connection_info_struct {
    char *data;
    size_t size;
};

int post_iterator(void *coninfo_cls, enum MHD_ValueKind kind, const char *key,
                        const char *filename, const char *content_type,
                        const char *transfer_encoding, const char *data, uint64_t off,
                        size_t size);

void parse_json(const char *json_str);

int handle_request(void *cls, struct MHD_Connection *connection,
                            const char *url, const char *method,
                            const char *version, const char *upload_data,
                            size_t *upload_data_size, void **con_cls);

void request_completed(void *cls, struct MHD_Connection *connection,
                            void **con_cls, enum MHD_RequestTerminationCode toe);

#ifdef __cplusplus
}
#endif