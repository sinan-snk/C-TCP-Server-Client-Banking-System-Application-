#include <iostream>
#include <WS2tcpip.h>
#include <string>
#include <fstream>
#include <sstream>  

#pragma comment (lib, "ws2_32.lib")

using namespace std;

void main()
{
	int userNumber = 6 * 5; //please update according to the number of users. number of users * 5
	string kullanicilar[30]; //please update according to the number of users. number of users * 5
	string satir;
	ifstream okumadosyası;
	okumadosyası.open("kullanicilar.txt");
	for (int i = 0; i < userNumber; i++) {
		getline(okumadosyası, satir);
		kullanicilar[i] = satir;
		cout << kullanicilar[i] << endl;
	}
	int kullanıcıID;

	// Initialze winsock
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);

	int wsOk = WSAStartup(ver, &wsData);
	if (wsOk != 0)
	{
		cerr << "Can't Initialize winsock! Quitting" << endl;
		return;
	}

	// Create a socket
	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening == INVALID_SOCKET)
	{
		cerr << "Can't create a socket! Quitting" << endl;
		return;
	}

	// Bind the ip address and port to a socket
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(54000);
	hint.sin_addr.S_un.S_addr = INADDR_ANY; // Could also use inet_pton .... 

	bind(listening, (sockaddr*)&hint, sizeof(hint));

	// Tell Winsock the socket is for listening 
	listen(listening, SOMAXCONN);

	// Wait for a connection
	sockaddr_in client;
	int clientSize = sizeof(client);

	SOCKET clientSocket = accept(listening, (sockaddr*)&client, &clientSize);

	char host[NI_MAXHOST];		// Client's remote name
	char service[NI_MAXSERV];	// Service (i.e. port) the client is connect on

	ZeroMemory(host, NI_MAXHOST); // same as memset(host, 0, NI_MAXHOST);
	ZeroMemory(service, NI_MAXSERV);

	if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
	{
		cout << host << " connected on port " << service << endl;
	}
	else
	{
		inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
		cout << host << " connected on port " <<
			ntohs(client.sin_port) << endl;
	}

	// Close listening socket
	closesocket(listening);

	// While loop: accept and echo message back to client
	char buf[4096];
	int kontrol = 0;
	while (kontrol == 0)
	{
		ZeroMemory(buf, 4096);

		// Wait for client to send data
		int bytesReceived = recv(clientSocket, buf, 4096, 0);
		string username = string(buf, 0, bytesReceived);

		int i = 0;
		while (i < userNumber && kontrol == 0) {
			if (username == kullanicilar[i]) {
				send(clientSocket, "1", 4096, 0);
				kontrol = 1;
				kullanıcıID = i;
			}
			i += 5;
		}

		if (kontrol == 0) {
			send(clientSocket, "0", 4096, 0);
		}


	}
	kontrol = 0;
	while (kontrol == 0)
	{
		ZeroMemory(buf, 4096);

		int bytesReceived = recv(clientSocket, buf, 4096, 0);
		string pass = string(buf, 0, bytesReceived);

		if (pass == kullanicilar[kullanıcıID + 1]) {
			string metin = kullanicilar[kullanıcıID + 2] + " - " + kullanicilar[kullanıcıID + 3] + " - " + kullanicilar[kullanıcıID + 4];
			send(clientSocket, metin.c_str(), 4096, 0);
			kontrol = 1;
		}
		else {
			send(clientSocket, "0", 4096, 0);
		}


	}
	kontrol = 0;
	while (kontrol == 0)
	{
		ZeroMemory(buf, 4096);

		int bytesReceived = recv(clientSocket, buf, 4096, 0);
		string TrNum = string(buf, 0, bytesReceived);

		if (TrNum == "1") {

			ZeroMemory(buf, 4096);
			int bytesReceived = recv(clientSocket, buf, 4096, 0);
			string amount = string(buf, 0, bytesReceived);
			float bakiye = std::stof(kullanicilar[kullanıcıID + 4]);
			bakiye = std::stof(amount) + std::stof(kullanicilar[kullanıcıID + 4]);
			kullanicilar[kullanıcıID + 4] = std::to_string(bakiye);
			remove("kullanicilar.txt");
			ofstream yarat("kullanicilar.txt");
			for (size_t i = 0; i < userNumber; i++)
			{
				yarat << kullanicilar[i] << endl;
			}
			string metin = "Depositing Successful. New Balance : " + kullanicilar[kullanıcıID + 4];
			send(clientSocket, metin.c_str(), 4096, 0);

		}

		else if (TrNum == "2") {

			ZeroMemory(buf, 4096);
			int bytesReceived = recv(clientSocket, buf, 4096, 0);
			string amount = string(buf, 0, bytesReceived);
			float bakiye = std::stof(kullanicilar[kullanıcıID + 4]);
			if (std::stof(amount) > bakiye) {
				send(clientSocket, "Insufficient Balance! Please Check Your Account.", 4096, 0);
			}
			else
			{
				bakiye = std::stof(kullanicilar[kullanıcıID + 4]) - std::stof(amount);
				kullanicilar[kullanıcıID + 4] = std::to_string(bakiye);
				remove("kullanicilar.txt");
				ofstream yarat("kullanicilar.txt");
				for (size_t i = 0; i < userNumber; i++)
				{
					yarat << kullanicilar[i] << endl;
				}
				string metin = "Withdrawal Successful. New Balance : " + kullanicilar[kullanıcıID + 4];
				send(clientSocket, metin.c_str(), 4096, 0);

			}

		}

		else if (TrNum == "3") {
			int alici = 0;
			int k = 3;
			int kesinti = 0;
			ZeroMemory(buf, 4096);
			int bytesReceived = recv(clientSocket, buf, 4096, 0);
			string accnNum = string(buf, 0, bytesReceived);
			while (k < userNumber && alici == 0)
			{
				if (accnNum == kullanicilar[k]) {
					alici = k;
				}
				else
				{
					k += 5;
				}

			}
			if (alici != 0) {
				send(clientSocket, "1", 4096, 0);
				ZeroMemory(buf, 4096);
				int bytesReceived = recv(clientSocket, buf, 4096, 0);
				string amount = string(buf, 0, bytesReceived);
				float bakiye = std::stof(kullanicilar[kullanıcıID + 4]);
				if (kullanicilar[kullanıcıID + 2] != kullanicilar[k - 1])
				{
					kesinti = 10;
				}

				if ((std::stof(amount)) + kesinti > bakiye) {
					send(clientSocket, "Insufficient Balance! Please Check Your Account.", 4096, 0);
				}
				else
				{

					bakiye = std::stof(kullanicilar[kullanıcıID + 4]) - (std::stof(amount) + kesinti);
					kullanicilar[kullanıcıID + 4] = std::to_string(bakiye);
					float gonderilenbakiye = std::stof(kullanicilar[k + 1]);
					gonderilenbakiye = std::stof(kullanicilar[k + 1]) + std::stof(amount);
					kullanicilar[k + 1] = std::to_string(gonderilenbakiye);
					remove("kullanicilar.txt");
					ofstream yarat("kullanicilar.txt");
					for (size_t i = 0; i < userNumber; i++)
					{
						yarat << kullanicilar[i] << endl;
					}
					string metin = "Send Money Transaction Successful. New Balance : " + kullanicilar[kullanıcıID + 4] ;
					send(clientSocket, metin.c_str(), 4096, 0);

				}
			}
			else
			{
				send(clientSocket, "Account Not Found! Please try again.", 4096, 0);
			}

		}

		else if (TrNum == "4") {
			kontrol = 1;

		}

	}
	// Close the socket
	closesocket(clientSocket);

	// Cleanup winsock
	WSACleanup();

	system("pause");
}
