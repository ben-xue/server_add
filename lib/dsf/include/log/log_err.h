/**
*
* @file   log_err.h
* @author pls
* @brief  ��־������
*/


#ifndef _DSF_DR_ERR_H__
#define _DSF_DR_ERR_H__

#ifdef __cplusplus
extern "C"
{
#endif

enum LogErrCode
{
	LOG_ERR_CRIT = -1,
	LOG_ERR_OK = 0,			//��ȷ
	LOG_ERR_OPEN_FAIL_FAIL,	//���ļ�ʧ��
	LOG_ERR_FILE_ALREADY_OPEN, //�ļ��Ѿ�������
	LOG_ERR_NET_DOWN,		//����������ʧ��
	LOG_ERR_FORWARD_LOOP,	//forward����ѭ��
	LOG_ERR_INVALID_FORMAT,	//��ʽ���Ϸ�
	LOG_ERR_CREATE_NET_FD_FAIL,	//������־socket����ʧ��
	LOG_ERR_SET_NET_FD_NOBLOCK_FAIL,	//������־socket������ʧ��
	LOG_ERR_CREATE_LOCK_FAIL,	// ��־������ʧ��
};


#ifdef __cplusplus
}
#endif



#endif


