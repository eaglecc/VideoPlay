#pragma once
#include "base.h"
#include "Socket.h"

class RTPHeader {
public:
	/*
	* λ�����
	* ע�⣺DatasheetĬ������Ǹ�λ���ұ��ǵ�λ
	* λ�������������ǵ�λ�����������Ǹ�λ�����ֽ�Ϊ��λ
	*/

	unsigned short csrccount : 4;	//cc����
	unsigned short extension : 1;	//X ��չ
	unsigned short padding : 1;		//P ���
	unsigned short version : 2;		//V �汾
	unsigned short pytype : 7;		//PT��Ч����
	unsigned short mark : 1;		//M ���
	unsigned short serial;			//���к�
	unsigned timestamp;				//ʱ��
	unsigned ssrc;					//ͬ����Դ
	unsigned csrc[15];				//��Լ��Դ

public:
	RTPHeader();
	RTPHeader(const RTPHeader& header);
	RTPHeader& operator=(const RTPHeader& header);
	operator HBuffer();
};

class RTPFrame {
public:
	RTPHeader m_head;		//RTPͷ���
	HBuffer m_pyload;		//RTP����
public:
	operator HBuffer();
};

class RTPHelper {
public:
	RTPHelper() :timestamp(0), m_udp(false) {
		m_udp.Bind(HAddress("0.0.0.0", (short)55000));
		//m_file = fopen("./out.bin", "wb+");
	}
	~RTPHelper() {
		//fclose(m_file);
	}

	int SendMediaFrame(RTPFrame& rtpframe, HBuffer& frame, const HAddress& client);

private:
	int GetFrameSepSize(HBuffer& frame);	//��ȡ֡ͷ��С
	int SendFrame(const HBuffer& frame, const HAddress& client);

private:
	DWORD timestamp;
	HSocket m_udp;
	FILE* m_file;
};

