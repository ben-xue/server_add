#pragma once

#include "stdinc.h"

NS_BEGIN(dsf)

#ifdef __cplusplus
extern "C"
{
#endif

	enum OrmErrCode
	{
		ORM_ERR_CRIT = -1,
		ORM_ERR_OK = 0,			//��ȷ
		ORM_ERR_ASYNC_CALLBACK_LEN_EXCEED,	//�ص���Ϣ����
		ORM_ERR_SEND_PKG_TO_ORM_SVR_FAILED,	//��orm������������Ϣʧ��	
		ORM_ERR_INIT_PROTO_METALIB_FAILED,	//��ʼ��proto metalib failed
		ORM_ERR_LOAD_ORM_PKG_META_FAILED,	//����orm pkg metaʧ��
		ORM_ERR_FIND_DATA_META_FAILED,		//Ѱ�����ݵ�metaʧ��
		ORM_ERR_DATA_META_TYPE_INVALID,		//����meta���ʹ���
		ORM_ERR_CHECK_ENTRY_KEY_PRIM_KEY_NOT_EXIST,	//����û�з���entrylist
		ORM_ERR_CHECK_ENTRY_KEY_NOT_NULL_KEY_NOT_EXIST,	//����Ϊnull��key������
		ORM_ERR_CHECK_ENTRY_KEY_REFER_ENTEY_NOT_EXIST,	//refer entry û��һ�����
		ORM_ERR_CHECK_ENTRY_KEY_SELECT_ENTEY_NOT_EXIST,	//select entry û��һ�����
		ORM_ERR_HOST_BUFF_SIZE_NOT_SUFFICIENT,	//host buff��С����
		ORM_ERR_ENCODE_ALL_ENTRY_FAILED,	//����all entryʧ��
		ORM_ERR_FIND_ENTRY_META_FAIELD,		//����entry name ��Ӧentry metaʧ��
		ORM_ERR_PACK_NAMELIST_ENTRY_FAILED,	//��������б�ʧ��
		ORM_ERR_INVALID_OPER_TYPE,			//���Ϸ��Ĳ�������
		ORM_ERR_PACK_SPLIT_KEY_FAILED,		//���split key��Ϣʧ��
		ORM_ERR_PACK_PRIM_KEY_FAILED,		//���prim key��Ϣʧ��
	};


#ifdef __cplusplus
}
#endif

NS_END()