# Cpp-TCP-Server-Client-Banking-System-Application-

Two console applications will be developed as TCP Server and TCP Client, and transactions on the user accounts, cards and balances of the accounts (saved in any .txt file) belonging to different banks previously created in the server will be performed on the server side.  Optionally, money transfer from the Client will be made via the TCP server. Users will be able to log in with their username and password in the Client application, see their balance, and write the other customer number registered in the system, their name-surname and the amount to send, and if there is enough balance, they will be transferred to the designated user. If there is not enough balance, the transaction cannot be performed. He will also be able to withdraw money from his own balance and deposit money to his balance. When making a transfer from one account to another, if the transferred account belongs to a different bank, a predetermined amount will be charged.

Kullanicilar.txt Format :

Line 1 ->User1_Username
Line 2 ->User1_Password
Line 3 ->User1_BankName
Line 4 ->User1_AccountNumber
Line 5 ->User1_AccountBalance
Line 6 ->User2_Username
Line 7 ->User2_Password
Line 8 ->User2_BankName
Line 9 ->User2_AccountNumber
Line 10 ->User2_AccountBalance
it continues like this...

There are 6 users and 3 different banks defined in the system. When server.cpp is run, user information is printed on the screen.
Warning! If the number of users will be changed (adding a new user or deleting a user), necessary changes should be made on the "int userNumber" and "string kullanicilar[]" in the server.cpp file.
