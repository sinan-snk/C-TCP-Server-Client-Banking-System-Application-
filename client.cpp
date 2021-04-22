#include <iostream>
#include <string>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

using namespace std;

void main()
{
	string ipAddress = "127.0.0.1";			// IP Address of the server
	int port = 54000;						// Listening port # on the server

	// Initialize WinSock
	WSAData data;
	WORD ver = MAKEWORD(2, 2);
	int wsResult = WSAStartup(ver, &data);
	if (wsResult != 0)
	{
		cerr << "Can't start Winsock, Err #" << wsResult << endl;
		return;
	}

	// Create socket
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
	{
		cerr << "Can't create socket, Err #" << WSAGetLastError() << endl;
		WSACleanup();
		return;
	}

	// Fill in a hint structure
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

	// Connect to server
	int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
	if (connResult == SOCKET_ERROR)
	{
		cerr << "Can't connect to server, Err #" << WSAGetLastError() << endl;
		closesocket(sock);
		WSACleanup();
		return;
	}

	// Do-while loop to send and receive data
	char buf[4096];
	string userInput;
	string amount;
	string accNum;
	cout << "TCP Banking System - Created By Sinan SINIK" << endl;
	int kontrol = 0;

	do
	{

		cout << "Please write username : ";
		getline(cin, userInput);


		if (userInput.size() > 0)		// Make sure the user has typed in something
		{
			// Send the text
			int sendResult = send(sock, userInput.c_str(), userInput.size() + 1, 0);
			if (sendResult != SOCKET_ERROR)
			{
				// Wait for response
				ZeroMemory(buf, 4096);
				int bytesReceived = recv(sock, buf, 4096, 0);
				if (bytesReceived > 0)
				{

					if (string(buf, 0, bytesReceived) == "1") {
						kontrol = 1;
						cout << "You are directing to the step of entering the password..." << endl;
					}
					else {
						cout << "Invalid username. Try again." << endl;
					}
				}
			}
		}

	} while (kontrol == 0);
	kontrol = 0;
	do
	{
		cout << "Please write your password : ";
		getline(cin, userInput);


		if (userInput.size() > 0)		
		{

			int sendResult = send(sock, userInput.c_str(), userInput.size() + 1, 0);
			if (sendResult != SOCKET_ERROR)
			{

				ZeroMemory(buf, 4096);
				int bytesReceived = recv(sock, buf, 4096, 0);
				if (bytesReceived > 0)
				{

					if (string(buf, 0, bytesReceived) != "0") {
						cout << "Login Successful. Redirecting Please Wait ..." << endl;
						kontrol = 1;
						cout << "WELCOME" << endl;
						cout << "Bank Name - Account Number - Balance " << endl;
						cout << string(buf, 0, bytesReceived) << endl;
					}
					else {
						cout << "Wrong password. Try again." << endl;
					}
				}
			}
		}

	} while (kontrol == 0);
	kontrol = 0;
	do
	{

		cout << "1- Depositing Money to the Your Account > " << endl;
		cout << "2- Withdrawing Funds From Your Account > " << endl;
		cout << "3- Money Transfer to Another Account > " << endl;
		cout << "4- EXIT > " << endl;
		cout << "Please enter the transaction number you want to do : ";
		getline(cin, userInput);
		int sendResult = send(sock, userInput.c_str(), userInput.size() + 1, 0);
		if (userInput == "1")
		{
			cout << "Enter the Amount to Deposit : ";
			getline(cin, amount);
			int sendResult = send(sock, amount.c_str(), amount.size() + 1, 0);
			ZeroMemory(buf, 4096);
			int bytesReceived = recv(sock, buf, 4096, 0);
			if (bytesReceived > 0)
			{
				cout << "SERVER> " << string(buf, 0, bytesReceived) << endl;
	
			}
		}
		else if (userInput == "2")
		{
			cout << "Enter the Amount to Withdraw : ";
			getline(cin, amount);
			int sendResult = send(sock, amount.c_str(), amount.size() + 1, 0);
			ZeroMemory(buf, 4096);
			int bytesReceived = recv(sock, buf, 4096, 0);
			if (bytesReceived > 0)
			{
				cout << "SERVER> " << string(buf, 0, bytesReceived) << endl;

			}
		}
		else if (userInput == "3")
		{
			cout << "Enter the Account Number to Send Money to : ";
			getline(cin, accNum);
			int sendResult = send(sock, accNum.c_str(), accNum.size() + 1, 0);
			ZeroMemory(buf, 4096);
			int bytesReceived = recv(sock, buf, 4096, 0);
			if (bytesReceived > 0)
			{
				if (string(buf, 0, bytesReceived) == "1")
				{
					cout << "Attention! If you are transferring money to another bank, 10 TL EFT fee will be charged.";
					cout << "Enter the Amount of Money to be Sent : ";
					getline(cin, amount);
					int sendResult = send(sock, amount.c_str(), amount.size() + 1, 0);
					ZeroMemory(buf, 4096);
					int bytesReceived = recv(sock, buf, 4096, 0);
					if (bytesReceived > 0)
					{
						cout << "SERVER> " << string(buf, 0, bytesReceived) << endl;

					}
				}
				else
				{
					cout << "SERVER> " << string(buf, 0, bytesReceived) << endl;
				}

			}
		}
		else if (userInput == "4") {
			kontrol = 1;
		}

	} while (kontrol == 0);

		// Gracefully close down everything
		closesocket(sock);
		WSACleanup();
	}
