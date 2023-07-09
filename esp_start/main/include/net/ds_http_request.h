#ifndef __DS_HTTP_REQUEST_H__
#define __DS_HTTP_REQUEST_H__

typedef enum{
    HTTP_GET_TIME =0,
    HTTP_GET_WEATHER
}HTTP_REQUEST_TYPE_E;

void ds_http_request_init(void);
void http_time_get(void);
void ds_http_request_type(HTTP_REQUEST_TYPE_E type);
void ds_http_request_all(void);

#endif // DS_HTTP_REQUEST_H__