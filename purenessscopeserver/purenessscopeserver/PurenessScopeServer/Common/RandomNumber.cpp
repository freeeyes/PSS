#include "RandomNumber.h"

CRandomNumber::CRandomNumber(void)
{
	m_nMinNumber = 0;
	m_nMaxNumber = 0;
}

CRandomNumber::~CRandomNumber(void)
{
}

void CRandomNumber::SetRange(int nMinNumber, int nMaxNumber)
{
	m_nMinNumber = nMinNumber;
	m_nMaxNumber = nMaxNumber + 1;

	srand((unsigned)time(NULL));
}

int CRandomNumber::GetRandom()
{
	return m_nMinNumber + (int)rand()%(m_nMaxNumber - m_nMinNumber);
}
