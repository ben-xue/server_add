/*
**  
*  通用的业务逻辑框架
*/

#ifndef COMM_INC_H__
#define COMM_INC_H__

#define __STDC_FORMAT_MACROS
#define __STDC_LIMIT_MACROS

#include <inttypes.h>
#include <cstdio>

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <strings.h>

#include "app/app.h"
#include "dr/dr_meta.h"
#include "dr/dr_net.h"
#include "log/log.h"
#include "bus/bus.h"
#include "ormapi/orm_protocol.h"
#include "conn/zone_conn_frame.h"
#include "entity.h"
#include "CTime.h"
#include "monitor_define.h"
#include "klog.h"

#define BZERO(data) bzero(&data, sizeof(data));
#define ARRAY_SIZE(array) (sizeof(array)/sizeof(array[0]))

#define CHECK_RET(expr, args...) do{if (!(expr)){return args; }}while(0)

#define SET_BIT(v,bit) (v|=(0X1<<bit))
#define CLR_BIT(v,bit) (v&=!(0X1<<bit))
#define CHECK_BIT(v,bit) (v&(0X1<<bit))

#endif


