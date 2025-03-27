#include "http_server.h"

int post_iterator(void *coninfo_cls, enum MHD_ValueKind kind, const char *key,
    const char *filename, const char *content_type,
    const char *transfer_encoding, const char *data, uint64_t off,
    size_t size)
{
    struct connection_info_struct *con_info = coninfo_cls;

    if (size > 0)
    {
        char *new_data = realloc(con_info->data, con_info->size + size + 1);
        if (!new_data)
            return MHD_NO;

        con_info->data = new_data;
        memcpy(con_info->data + con_info->size, data, size);
        con_info->size += size;
        con_info->data[con_info->size] = '\0';
    }

    return MHD_YES;
}

void parse_json(const char *json_str)
{
    struct json_object *parsed_json;
    struct json_object *brightness;
    struct json_object *contrast;
    struct json_object *saturation;
    struct json_object *sharpness;

    parsed_json = json_tokener_parse(json_str);
    if (parsed_json == NULL) {
        printf("JSON解析错误!\n");
        return;
    }

    if (!json_object_object_get_ex(parsed_json, "brightness", &brightness) ||
        !json_object_object_get_ex(parsed_json, "contrast", &contrast) ||
        !json_object_object_get_ex(parsed_json, "saturation", &saturation) ||
        !json_object_object_get_ex(parsed_json, "sharpness", &sharpness)) {
        printf("获取JSON字段失败!\n");
        json_object_put(parsed_json);
        return;
    }

    printf("接收到的数据:\n");
    printf("brightness: %d\n", json_object_get_int(brightness));
    printf("contrast: %d\n", json_object_get_int(contrast));
    printf("saturation: %d\n", json_object_get_int(saturation));
    printf("sharpness: %d\n", json_object_get_int(sharpness));

    brightness_set(json_object_get_int(brightness));
    contrast_set(json_object_get_int(contrast));
    saturation_set(json_object_get_int(saturation));
    sharpness_set(json_object_get_int(sharpness));

    json_object_put(parsed_json);
}

int handle_request(void *cls, struct MHD_Connection *connection,
                        const char *url, const char *method,
                        const char *version, const char *upload_data,
                        size_t *upload_data_size, void **con_cls) {
    if (NULL == *con_cls) {
        struct connection_info_struct *con_info;
        con_info = malloc(sizeof(struct connection_info_struct));
        if (con_info == NULL)
            return MHD_NO;
        
        con_info->data = NULL;
        con_info->size = 0;
        *con_cls = con_info;
        return MHD_YES;
    }

    if (strcmp(method, "POST") == 0) {
        struct connection_info_struct *con_info = *con_cls;

        if (*upload_data_size != 0) {
            if (post_iterator(con_info, MHD_POSTDATA_KIND, NULL, NULL, NULL, NULL,
                            upload_data, 0, *upload_data_size) == MHD_NO)
                return MHD_NO;
            
            *upload_data_size = 0;
            return MHD_YES;
        } else {
            if (con_info->data) {
                parse_json(con_info->data);
            }
        }
    }

    const char *response_str = "OK";
    struct MHD_Response *response;
    int ret;

    response = MHD_create_response_from_buffer(strlen(response_str),
                                             (void*)response_str,
                                             MHD_RESPMEM_PERSISTENT);
    ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
    MHD_destroy_response(response);

    return ret;
}

void request_completed(void *cls, struct MHD_Connection *connection,
                            void **con_cls, enum MHD_RequestTerminationCode toe) {
    struct connection_info_struct *con_info = *con_cls;

    if (con_info) {
        if (con_info->data)
            free(con_info->data);
        free(con_info);
        *con_cls = NULL;
    }
}