
#include "Ali_iot.h"

#include <string.h>

#define ALI_IP_ADDR						"139.196.135.135"
#define ALI_IP_ADDR0					139
#define ALI_IP_ADDR1					196
#define ALI_IP_ADDR2					135
#define ALI_IP_ADDR3					135

#define ALI_PORT							1883

#define ALI_CLIENT_ID							8056f2f2c01d
#define ALI_TIMESTAMP							123456

#define ALI_MQTT_CLIENTID			"8056f2f2c01d|securemode=3,signmethod=hmacsha1,timestamp=123456|"
#define ALI_MQTT_USERNAME			"sensor&a1IQnndIVtY"
#define ALI_MQTT_PASSWORD			"0dac29262c34d38d795485f47c34a9db800f7bbd"

#define ALI_TOPIC							"/a1IQnndIVtY/sensor/sensor_data"

#define publish_data					"STM32F407_FreeRTOS10.0.0_lwIP2.1.2"

ip4_addr_t	ali_ipaddr;

mqtt_client_t client;
struct mqtt_connect_client_info_t client_info;
err_t mqtt_status;


void Ali_Mqtt_Connect(void)
{
	client_info.client_id = ALI_MQTT_CLIENTID;
	client_info.client_pass = ALI_MQTT_PASSWORD;
	client_info.client_user = ALI_MQTT_USERNAME;
	client_info.keep_alive = 60;
	client_info.will_msg = NULL;
	client_info.will_qos = 1;
	client_info.will_retain = 0;
	client_info.will_topic = ALI_TOPIC;
	
	IP4_ADDR(&ali_ipaddr , ALI_IP_ADDR0 , ALI_IP_ADDR1 , ALI_IP_ADDR2 , ALI_IP_ADDR3);
	
	mqtt_status = mqtt_client_connect(&client , &ali_ipaddr , ALI_PORT , Ali_Mqtt_Connect_CallBack_Func , NULL , &client_info);
	
	if(mqtt_status == ERR_OK)
		printf("mqtt_connect is succeed \n");
	else
		printf("mqtt_connect is false %d \n",mqtt_status);
	
}


void Ali_Mqtt_Connect_CallBack_Func(mqtt_client_t *client, void *arg, mqtt_connection_status_t status)
{
	switch (status)
	{
		case MQTT_CONNECT_ACCEPTED:
				Ali_Mqtt_Connect_accepted(client);
			break;
	}
}

void Ali_Mqtt_Connect_accepted(mqtt_client_t *client)
{
	/* subscribe topic messages */
	mqtt_subscribe(client , ALI_TOPIC , 1 , Ali_Mqtt_subscribe_CallBack_Func , NULL);
	
	/* topic messages data callback */
	mqtt_set_inpub_callback (client , Ali_Mqtt_publish_CallBack_Func , Ali_Mqtt_data_CallBack_Func , NULL);
	
	/*  */
	mqtt_status = mqtt_publish(client , ALI_TOPIC , publish_data , strlen(publish_data) , 1 , 0 , Ali_Mqtt_publishData_CallBack_Func , NULL);
	if(mqtt_status ==ERR_OK)
		printf("mqtt_publish is succeed \n");
	else
		printf("mqtt_publish is false %d\n",mqtt_status);
}

void Ali_Mqtt_subscribe_CallBack_Func(void *arg, err_t err)
{
	
}

void Ali_Mqtt_publish_CallBack_Func(void *arg, const char *topic, u32_t tot_len)
{
	printf("the topic is %s\n",topic);
}

void Ali_Mqtt_data_CallBack_Func(void *arg, const u8_t *data, u16_t len, u8_t flags)
{
	printf("the messages is %s\n",data);
}

void Ali_Mqtt_publishData_CallBack_Func(void *arg, err_t err)
{
	
}

