#ifndef _dataClient_H_
#define _dataClient_H_
#include<iostream>
#include<stdio.h>
#include <sqlite3.h>
#include<vector>
using namespace std;
struct client {
	string user, password;
};
 vector<client> Clients;
 const char*url = "clientdata.db";
static int createDB(const char* s) {
	sqlite3* DB;
	int exit = 0;
	exit = sqlite3_open(s, &DB);
	sqlite3_close(DB);
	return 0;
}
static int createTable(const char* s) {
	sqlite3* DB;
	string sql = "CREATE TABLE USER("\
		"USER 	TEXT PRIMARY KEY  NOT NULL, "\
		"PASSWORD		TEXT NOT NULL); ";

	try {
		int exit = 0;
		exit = sqlite3_open(s, &DB);
		char* messageError;
		exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
		if (exit != SQLITE_OK) {
			cerr << "Error create " << endl;
			sqlite3_free(messageError);
		}
		else {
			cout << "Table created Successfully " << endl;
			sqlite3_close(DB);
		}

	}
	catch (const exception& e) {
		cerr << e.what();
	}
	return 0;
}
static int insertData(const char* s) {
	sqlite3* DB;
	char* messageError;
	int exit = sqlite3_open(s, &DB);
	string sql("INSERT INTO USER (USER,PASSWORD) VALUES ('TONGTHANH','682001');"
		"INSERT INTO USER (USER,PASSWORD) VALUES ('PHANMINHIEU','742001');"
	);
	exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
	if (exit != SQLITE_OK) {
		cerr << "Error insert " << endl;
		sqlite3_free(messageError);
	}
	else {
		cout << "Records created Successfully! " << endl;
	}
	return 0;
}

static int insertData_Client(const char* s,string user,string pass) {
	sqlite3* DB;
	char* messageError;
	int exit = sqlite3_open(s, &DB);

	//user = "'" + user + "'";
	//pass = "'" + pass + "'";
	string sql("INSERT INTO USER (USER,PASSWORD) VALUES ('" +user+ "','"+pass+"');");
	exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
	if (exit != SQLITE_OK) {
		cerr << "Error insert " << endl;
		sqlite3_free(messageError);
	}
	else {
		cout << "Records created Successfully! " << endl;
	}
	return 0;
}

void Add(vector<client> Clients, client A) {

}
static int callback1(void* NotUsed, int argc, char** argv, char** azColName) {
	client A;
	for (int i = 0; i < argc; i++) {
		if (i == 0) {
			A.user = argv[0];
		}
		if (i == 1) {
			A.password = argv[1];
		}

	}
	Clients.push_back(A);
	cout << "New" << endl;
	return 0;
}
static int selectData(const char* s) {
	sqlite3* DB;
	int exit = sqlite3_open(s, &DB);
	string sql = "SELECT * FROM USER;";
	sqlite3_exec(DB, sql.c_str(), callback1, NULL, NULL);
	return 0;
}
bool check_user_exists(string user) {
	Clients.clear();
	selectData(url);
	for (int i = 0; i < Clients.size(); i++) {
		if (Clients[i].user == user)
			return true;
	}
	return false;
}
bool check_password(string pass) {
	for (int i = 0; i < Clients.size(); i++) {
		if (Clients[i].password == pass)
			return true;
	}
	return false;
}

#endif // !_dataClient_H_
