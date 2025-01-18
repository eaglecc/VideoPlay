#include "MediaFile.h"

MediaFile::MediaFile() : m_file(NULL), m_type(-1) {
}

MediaFile::~MediaFile() {
	Close();
}

int MediaFile::Open(const HBuffer& path, int nType) {
	m_file = fopen(path, "rb");
	if (m_file == NULL) {
		return -1;
	}
	m_type = nType;
	//�Ƚ��ļ�ָ���ƶ���β����Ȼ��ͨ��ftell�����ƫ�����õ�����
	fseek(m_file, 0, SEEK_END);
	m_size = ftell(m_file);
	Reset();

	return 0;
}

HBuffer MediaFile::ReadOneFrame() {
	switch (m_type) {
		case 96:
			return ReadH264Frame();
			break;
		default:break;
	}
	return HBuffer();
}

void MediaFile::Close() {
	m_type = -1;
	if (m_file != NULL) {
		FILE* pfile = m_file;
		m_file = NULL;
		fclose(pfile);
	}
}

void MediaFile::Reset() {
	if (m_file) {
		fseek(m_file, 0, SEEK_SET);
	}
}

long MediaFile::FindH264Head(int& headsize) {
	while (!feof(m_file)) {
		char c = 0x7F;
		//���ж��Ƿ������β
		while (!feof(m_file)) {
			c = fgetc(m_file);
			if (c == 0) break;
		}

		//��Ϊ�գ�Ȼ�����0���ٶ���1��Ĭ�Ͼ͵�00 00 01
		//���߲�Ϊ�գ�����0���ٶ���0���ٶ���1������00 00 00 01
		if (!feof(m_file)) {
			c = fgetc(m_file);
			if (c == 0) {
				c = fgetc(m_file);
				if (c == 1) {
					headsize = 3;
					return ftell(m_file) - 3;
				}
				else if (c == 0) {
					c = fgetc(m_file);
					if (c == 1) {
						headsize = 4;
						return ftell(m_file) - 4;
					}
				}
			}
		}
	}

	return -1;
}

HBuffer MediaFile::ReadH264Frame() {
	if (m_file) {
		int headsize = 0;
		long off = FindH264Head(headsize);
		if (off == -1)return HBuffer();
		fseek(m_file, off + headsize, SEEK_SET);	//����ͷ֮����ƶ��ļ�ָ��λ��
		long tail = FindH264Head(headsize);
		//����û����һ֡�ˣ�����taile=�ļ��ܳ���
		if (tail == -1) tail = m_size;
		//��������������
		//��������һ֡����һ֮֡��Ӳ�õ�size
		//�ļ��ܳ���-��һ֡���ȵõ�size
		long size = tail - off;
		fseek(m_file, off, SEEK_SET);
		HBuffer result(size);
		fread(result, 1, size, m_file);
		return result;
	}
	return HBuffer();
}


