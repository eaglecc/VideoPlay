#pragma once
#include "Socket.h"

class MediaFile {
public:
	MediaFile();
	~MediaFile();

	int Open(const HBuffer& path, int nType = 96);	//���ļ�
	HBuffer ReadOneFrame();							//��һ֡,buffer.size()=0��˵��û��֡
	void Close();									//�ر�
	void Reset();									//���ã�ReadOneFrame�ֿ��Լ���

protected:
	//���ʽ��ͬ����ͨ��ReadOneFrame���ò�ͬ��������չ
	
	long FindH264Head(int& headsize);		//��ȡh264ͷ��������-1��ʾ����ʧ��
	HBuffer ReadH264Frame();				//��ȡh264��֡

private:
	long m_size;
	FILE* m_file;
	HBuffer m_filepath;
	//96:h264
	int m_type;				
};

