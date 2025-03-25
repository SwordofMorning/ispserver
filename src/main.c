
#include "rk_aiq_uapi_imgproc_ipc_ipc.h"
#include "call_fun_ipc.h"

#define DBUS_NAME                "rockchip.ispserver"
#define DBUS_PATH                "/"
#define DBUS_IF                  DBUS_NAME ".sysctl"
#define SHARE_PATH               "."

rk_aiq_sys_ctx_t* aiq_ctx = NULL;
const char* sns_entity_name = "m01_b_imx335 1-001a";
const char* iqfile = "/oem/etc/iqfiles";
rk_aiq_working_mode_t work_mode = RK_AIQ_WORKING_MODE_NORMAL;

int main()
{
    call_fun_ipc_client_init(DBUS_NAME, DBUS_IF, DBUS_PATH, SHARE_PATH, 1);
    aiq_ctx = rk_aiq_uapi_sysctl_init(sns_entity_name, iqfile, NULL, NULL);
    printf("-----------------------------------------------------------------------------DEBUG\n");
    XCamReturn ret = rk_aiq_uapi_sysctl_prepare(aiq_ctx, 2592, 1944, work_mode);
    if (ret != XCAM_RETURN_NO_ERROR)
    {
        printf("rk_aiq_uapi_sysctl_prepare failed!\n");
        return 0;
    }
    else
    {
        printf("rk_aiq_uapi_sysctl_prepare Success!\n");
        ret = rk_aiq_uapi_sysctl_start(aiq_ctx);
    }

    while (1)
    {
        sleep(1);

        // opMode_t mode;
        // rk_aiq_uapi_getWBMode(aiq_ctx, &mode);
        printf("Debug\n");
    }

    if (aiq_ctx)
    {
        rk_aiq_uapi_sysctl_stop(aiq_ctx, false);
        rk_aiq_uapi_sysctl_deinit(aiq_ctx);
    }

    return 0;
}