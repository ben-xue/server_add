//
// Created by Administrator on 2021/4/26.
//

#include "CPlayer.h"

CPlayer::CPlayer()
{
    m_iCmptCnt  = 0;

    AddCmpt(&m_bag);
}

bool CPlayer::AddCmpt(CPlayerCmpt *pCmpt)
{
    //直接用系统的assert，来强制错误
    assert(m_iCmptCnt < MAX_PLAYER_CMPT_CNT);
    m_aCmpt[m_iCmptCnt++] = pCmpt;

    return true;
}

int CPlayer::Init()
{
    return 0;
}

void CPlayer::Resume()
{

}

void CPlayer::Free()
{

}
