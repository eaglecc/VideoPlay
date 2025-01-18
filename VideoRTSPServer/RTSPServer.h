#pragma once
#include "Socket.h"
#include "CHeQueue.h"
#include "HeThread.h"
#include "RTPHelper.h"
#include "MediaFile.h"
#include <string>
#include <map>

/*������*/
class RTSPRequest {
public:
	RTSPRequest();
	RTSPRequest(const RTSPRequest& protocol);
	RTSPRequest& operator=(const RTSPRequest& protocol);
	~RTSPRequest() { m_method = -1; }

	/*set*/
	void SetMethod(const HBuffer& method);
	void SetUrl(const HBuffer& url);
	void SetSequence(const HBuffer& seq);
	void SetClientPort(int ports[]);
	void SetSession(const HBuffer& session);
	/*get*/
	int method() const { return m_method; }
	const HBuffer& url() const { return m_url; }
	const HBuffer& session() const { return m_session; }
	const HBuffer& sequence() const { return m_seq; }
	const HBuffer& port(int index = 0) const { return index ? m_client_port[1] : m_client_port[0]; }

private:
	int m_method;				//-1:��ʼ�� 0:options 1:describe 2:setup 3:play 4:tearown
	HBuffer m_url;				//�����ַ
	HBuffer m_session;			//����Ựid
	HBuffer m_seq;				//�����������к�
	HBuffer m_client_port[2];	//client port
};

/*�ظ�����Ӧ����*/
class RTSPReply {
public:
	RTSPReply();
	RTSPReply(const RTSPReply& protocol);
	RTSPReply& operator=(const RTSPReply& protocol);
	~RTSPReply() { m_method = -1; }
	HBuffer toBuffer();

	/*set*/
	void SetMethod(int method);
	void SetOptions(const HBuffer& options);
	void SetSequence(const HBuffer& seq);
	void SetSdp(const HBuffer& sdp);
	void SetClientPort(const HBuffer& port0, const HBuffer& port1);
	void SetServerPort(const HBuffer& port0, const HBuffer& port1);
	void SetSession(const HBuffer& session);

private:
	int m_method;				//-1:��ʼ�� 0:options 1:describe 2:setup 3:play 4:tearown
	int m_client_port[2];		//client port
	int m_server_port[2];		//server port
	HBuffer m_sdp;				//����Ự������Ϣ
	HBuffer m_seq;				//�����������к�
	HBuffer m_options;			//����֧�������б�
	HBuffer m_session;			//����Ựid
};

class RTSPSession;
class RTSPServer;
typedef void (*RTSPPLAYCB)(RTSPServer* thiz, RTSPSession& session);

/*�Ựid��*/
class RTSPSession {
public:
	RTSPSession();
	RTSPSession(const HSocket& client);
	RTSPSession(const RTSPSession& session);
	RTSPSession& operator=(const RTSPSession& session);
	~RTSPSession() {}
	int PickRequestAndReply(RTSPPLAYCB cb, RTSPServer* thiz);	//����request����Ӧ��
	HAddress GetClientUDPAddress() const;

private:
	HBuffer PickOneLine(HBuffer& buffer);					//ѡ��һ�д���
	HBuffer Pick();											//ѡ��
	RTSPRequest AnalyseRequest(const HBuffer& buffer);		//����
	RTSPReply Reply(const RTSPRequest& request);			//�ظ�

private:
	HBuffer m_id;		//session id
	HSocket m_client;	//��Ӧ�׽���
	short m_port;
};


class RTSPServer : public ThreadFuncBase {
public:
	RTSPServer() :m_socket(true), m_status(0), m_pool(10) {
		m_threadMain.UpdateWorker(ThreadWorker(this, (FUNCTYPE)&RTSPServer::threadWorker));
		m_h264.Open("./test.h264");
	}
	int Init(const std::string& strIP = "0.0.0.0", short port = 554);	//��ʼ��
	int Invoke();														//����
	void Stop();														//ֹͣ
	~RTSPServer() {
		Stop();
	}

protected:
	int threadWorker();										//�߳�����return0������������ֹ�����ྯ��
	int ThreadSession();									//����session�߳�
	static void PlayCallBack(RTSPServer* thiz, RTSPSession& session);	//�ص�
	void UdpWorker(const HAddress& Client);					//udp�ɻ�

private:
	HSocket m_socket;
	HAddress m_addr;
	int m_status;											//״̬��0δ��ʼ�� 1��ɳ�ʼ�� 2�������� 3�ر�
	HeThread m_threadMain;									//���߳�
	HeThreadPool m_pool;									//session�̳߳�
	static SocketIniter init;								//�����ʼ������
	CHeQueue<RTSPSession> m_lstSession;						//�̰߳�ȫ�Ķ���
	RTPHelper m_helper;										//RTP
	MediaFile m_h264;										//��ý��
};

