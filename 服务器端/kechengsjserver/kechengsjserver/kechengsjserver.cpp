// kechengsjserver.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include<iostream>
#include<cstring>
#include<string>
#include<algorithm>
#include<cstdio>
#include<map>
#include<fstream>
#include<WinSock2.h>
#include <atlstr.h>
#define BUFFER_LEN 1000
#define PORT 65432
#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable:4996)
const int maxn = 1000;
using namespace std;
map<string, string >userlog;
map<string ,string >userip;
map<string ,string >::iterator iter;
char onlinelist[50][maxn];
int len;
char* zhuan(string a)
{
	char temp[maxn];
	int i;
	for (i = 0; i < a.length(); i++)
	{
		temp[i] = a[i];
	}
	temp[i] = '\0';
	return temp;
}
struct UserInfo
{
	int op;
	char name[maxn], pwd[maxn];
	char ip[maxn];
	char online[50][maxn];
	char remsg[maxn];
	int listlen;
	void getonline()
	{
		listlen = len;
		memset(online, '\0', sizeof(online));
		for (int i = 0; i <len; i++)
		{
			strcpy(online[i], onlinelist[i]);
		}
	}
};
void showuserlist()
{
	for (int i = 0; i < userip.size(); i++)
		cout << onlinelist[i] << endl;
}
void onlineupdate()
{
	int i = 0;
	iter = userip.begin();
	len = 0;
	while (iter != userip.end()) {
		
		if (iter->second == "")
		{
			iter++;
			continue;
		}
			
		len++;
		strcpy(onlinelist[i],zhuan(iter->first.data()));
		cout << "用户名：" << iter->first <<" IP地址："<<iter->second<< endl;
		iter++;
		i++;
	}
	cout << "在线用户列表更新完成！" << endl;
}
void jieshu()
{
	char name[maxn], pwd[maxn];
	ofstream writeFile("D://user.txt");
	iter = userlog.begin();
	while (iter != userlog.end()) {
		//cout << iter->first << " : " << iter->second << endl;

		strcpy(name ,zhuan(iter->first)), strcpy(pwd ,zhuan(iter->second));

		writeFile << name << " " << pwd << endl;
		iter++;
	}
	cout << "用户数据保存成功！" << endl;
}
void init()
{
	ifstream readFile("D://user.txt");
	char temp[1024] = { 0 };
	char name[maxn], pwd[maxn];
	//char a[10000], b[10000];
	while (readFile >>name >> pwd)
	{
		userlog[name] = pwd;
		cout << name << " " << pwd << endl;
	}
	cout << "用户数据导入成功！" << endl;
}

int main()
{
	init();
	//jieshu();
	
	//***********************************************  开始网络
	SOCKET sock_server, newsock;
	fd_set fdsock;
	fd_set fdread;
	struct sockaddr_in addr;
	struct sockaddr_in  client_addr;

	WSADATA wsaData;
	WORD  wVersionRequested = MAKEWORD(2, 2);
	if (WSAStartup(wVersionRequested, &wsaData) != 0)
	{
		cout << "加载winsock.dll失败！\n";
		return 0;
	}

	if ((sock_server = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		cout << "创建套接字失败！\n";
		WSACleanup();
		return 0;
	}

	int addr_len = sizeof(struct sockaddr_in);
	memset((void *)&addr, 0, addr_len);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(sock_server, (struct sockaddr *)&addr, sizeof(addr)) != 0)
	{
		cout << "地址绑定失败！\n";
		closesocket(sock_server);
		WSACleanup();
		return 0;
	}
	if (listen(sock_server, 0) != 0)
	{
		cout << "listen函数调用失败！\n";
		closesocket(sock_server);
		WSACleanup();
		return 0;
	}
	else
		cout << "listenning......\n";
	FD_ZERO(&fdsock);
	FD_SET(sock_server, &fdsock);

	while (true)
	{
		FD_ZERO(&fdread);
		fdread = fdsock;
		if (select(0, &fdread, NULL, NULL, NULL) > 0)
		{
			for (int i = 0; i < fdsock.fd_count; i++)
			{
				if (FD_ISSET(fdsock.fd_array[i], &fdread))
				{
					if (fdsock.fd_array[i] == sock_server)
					{
						newsock = accept(sock_server, (struct sockaddr *) &client_addr, &addr_len);
						if (newsock == INVALID_SOCKET)
						{
							cout << "accept函数调用失败！\n";
							for (int j = 0; j <fdsock.fd_count; j++)
								closesocket(fdsock.fd_array[j]);
							WSACleanup();
							return 0;
						}
						else
						{
							// **********************************************************************接受登陆信息
							int size;
							UserInfo userinfo;
							if ((size = recv(newsock,(char *)&userinfo, sizeof(userinfo), 0)) < 0)
							{
								cout << "接收消息失败！错误代码：" << WSAGetLastError() << endl;
								closesocket(newsock);
								return 0;
							}
							int userop = userinfo.op;
							if (userop == 1) //当前是登陆信息
							{
								string name = userinfo.name, pwd = userinfo.pwd;
								UserInfo reinfo;
								strcpy(reinfo.name , userinfo.name);
								reinfo.op = 1;
								memset(reinfo.remsg, 0, sizeof(reinfo.remsg));
								if (userlog[name] == pwd)
								{
									char yes[10] = "yes";
									cout << "用户：" << name << " 登陆成功！" << endl;
									strcpy(reinfo.remsg, yes);
									
									cout << "回复消息："<<reinfo.remsg << endl;
									userip[name] = inet_ntoa(client_addr.sin_addr);
									/*****************************************************/
									struct sockaddr_in sa;

									if (!getpeername(newsock, (struct sockaddr *)&sa, &len))
									{
										printf("对方IP：%s ", inet_ntoa(sa.sin_addr));
										printf("对方PORT：%d ", ntohs(sa.sin_port));
									}
								
									/******************************************************/
									onlineupdate();
									showuserlist();
									reinfo.getonline();
									// ***************************发送登陆成功信息
									if ((size = send(newsock, (char *)&reinfo, sizeof(reinfo), 0)) < 0)
										cout << "发送信息失败！错误代码:" << WSAGetLastError() << endl;
									else if (size == 0)
										cout << "对方已关闭连接！\n";

									//******************************************
									
									cout << "fd size:" << fdsock.fd_count << endl;

									reinfo.op = 4;//回复消息;
									for (int j = 0; j < fdsock.fd_count; j++)
									{
										if (fdsock.fd_array[j] != sock_server && j != i)
											send(fdsock.fd_array[j], (char *)&reinfo, sizeof(reinfo), 0);
									}
									FD_SET(newsock, &fdsock);
								}
								else
								{
									strcpy(reinfo.remsg, "no");
									if ((size = send(newsock, (char *)&reinfo, sizeof(reinfo), 0)) < 0)
										cout << "发送信息失败！错误代码:" << WSAGetLastError() << endl;
									else if (size == 0)
										cout << "对方已关闭连接！\n";
									closesocket(newsock);
								}
							}
							else if (userop == 2)
							{
								cout << "接受注册消息!" << endl;
								string name = userinfo.name, pwd = userinfo.pwd;
								cout << name << " " << pwd << endl;
								UserInfo reinfo;
								if (userlog[name].length()==0)
								{
									strcpy(reinfo.remsg, "注册成功");
									userlog[name]=pwd;
								}
								else 
									strcpy(reinfo.remsg, "注册失败，用户名重复");
								reinfo.remsg[strlen(reinfo.remsg)] = '\0';

								cout << reinfo.remsg << endl;
								// ***************************发送注册成功信息
								if ((size = send(newsock, (char *)&reinfo, sizeof(reinfo), 0)) < 0)
									cout << "发送信息失败！错误代码:" << WSAGetLastError() << endl;
								else if (size == 0)
									cout << "对方已关闭连接！\n";
								jieshu();
								//******************************************

							}
							
							
						}
					}
					else           //***********************************************断开连接消息
					{
						
						//memset((void *)msgbuffer, 0, sizeof(msgbuffer));

						UserInfo userinfo;

						int size = recv(fdsock.fd_array[i], (char *)&userinfo, sizeof(userinfo), 0);
						if (size < 0)
						{
							cout << "接收信息失败! 这次是size小于0了" << endl;
							closesocket(fdsock.fd_array[i]);
							FD_CLR(fdsock.fd_array[i], &fdsock);
						}
						else if (size == 0)
						{
							cout << "对方已关闭！\n";
							closesocket(fdsock.fd_array[i]);
							FD_CLR(fdsock.fd_array[i], &fdsock);
							cout << "清除退出的用户成功" << endl;
							//*********************************************************************************
							
						}
						else
						{
							
							if (userinfo.op == 3) //接收注销信息
							{
								string str = userinfo.name;
								cout << "接收注销信息：" << str << endl;
								//********************************************************
								str = userinfo.name;
								userip[str] = "";
								onlineupdate();
								//cout << "lalalla" << endl;
								FD_CLR(fdsock.fd_array[i], &fdsock);
								UserInfo reinfo;
								//cout << "准备发送！！！！" << endl;
								reinfo.op = 4;//回复消息;
								reinfo.getonline();
								for (int j = 0; j < fdsock.fd_count; j++)
								{
									if (fdsock.fd_array[j] != sock_server)
										send(fdsock.fd_array[j], (char *)&reinfo, sizeof(reinfo), 0);
								}
								cout << "更新在线列表并转发给其他人更新列表！" << endl;
							}
							else if (userinfo.op == 4)
							{
								UserInfo reinfo;
								memset(reinfo.ip, '\0', sizeof(reinfo.ip));
								reinfo.op =5;
								string str = userinfo.name;
								string temp = userip[str];
								strcpy(reinfo.name, userinfo.name);
								for (int it = 0; it < temp.length(); it++)
								{
									reinfo.ip[it] = temp[it];
								}
								reinfo.ip[temp.length()] = '\0';
								cout << "返回的name 和ip地址" << reinfo.name << "," << reinfo.ip << endl;
								send(fdsock.fd_array[i], (char*)&reinfo, sizeof(reinfo), 0);
								cout << "p2p请求的ip 发送完毕！" << endl;
							}
							

						}
						
					}
				}
			}
		}
		else
		{
			cout << "Select调用失败！\n";
			break;
		}
	}

	for (int j = 0; j < fdsock.fd_count; j++)
		closesocket(fdsock.fd_array[j]);
	WSACleanup();
	return 0;
	
}
