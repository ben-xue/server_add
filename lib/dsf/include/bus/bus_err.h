#pragma once

#include "comm/stdinc.h"

NS_BEGIN(dsf)

#ifdef __cplusplus
extern "C"
{
#endif


enum BusErrCode
{
	BUS_ERR_CRIT = -1,
	BUS_ERR_OK = 0,			//��ȷ
	BUS_ERR_INPUT_FILE_INVALID,	//��Ч�������ļ�
	BUS_ERR_ADDR_TEMPLATE_FORMAT_ERR,	//addrģ���ʽ����
	BUS_ERR_ADDR_FORMAT_ERR,	//bus��ַ����
	BUS_ERR_EXCEED_MAX_CHANNEL, //�������channel����
	BUS_ERR_CREATE_CHANNEL_SHM_FAILED,	//����channel�����ڴ����
	BUS_ERR_ATTACH_CHANNEL_SHM_FAILED,	//����channel�����ڴ����
	BUS_ERR_ATTACH_CHANNEL_MAGIC_NOT_MATCH,	//bus channel magic��ƥ��
	BUS_ERR_ATTACH_CHANNEL_QUEUE_SIZE_NOT_MATCH,	//�����ڴ��С��ƥ��
	BUS_ERR_CHANNEL_QUEUE_FULL,	//busͨ������
	BUS_ERR_FIND_DST_QUEUE_FAILED,	//Ѱ�Ҷ�Ӧ����ʧ��
	BUS_ERR_ATTACH_SHM_NOT_EXIST,	//�����ڴ治���ڣ�attachʧ��
	BUS_ERR_ATTACH_SHM_MGR_MAGIC_NOT_MATCH,	//�����ڴ�magic��ƥ��
	BUS_ERR_ATTACH_SHM_MGR_SYS_ERR,	//���ع����ڴ�ϵͳ����
	BUS_ERR_API_NEED_INIT,		//��Ҫ��Init����ʹ��
	BUS_ERR_DST_CHANNEL_NOT_FOUND,	//Ŀ��channelδ�ҵ�
	BUS_ERR_BUFF_NOT_ENOUGH,	//������Ϣ��������С����
	BUS_ERR_BUSMGR_NEED_INIT,	//��Ҫ��Init����ʹ��
	BUS_ERR_INVALID_CHANNEL_INDEX,	//channel��Ų��Ϸ�
	BUS_ERR_GET_SHMID_INFO_FAILED,	//��ȡchannel shm��Ϣʧ��
	BUS_ERR_CHANNEL_HAVE_PROCESS_ATTACHED,	//�н����Ѿ�attach�˸�ͨ��
	BUS_ERR_DELETE_CHANNEL_SHM_FAILED,	//ɾ��ĳ��channel�Ĺ����ڴ�ʧ��
	BUS_ERR_BUFF_NO_MSG_IN_CHANNEL,	//ͨ����û����Ϣ
	BUS_ERR_MSG_HEAD_MAGIC_NOT_MATCH,	//��Ϣͷmagic��ƥ��
    BUS_ERR_SEND_FUNC_TYPE_INVALID, // �����ܷ��͹���ID�����Ƿ�
};


#ifdef __cplusplus
}
#endif

NS_END()