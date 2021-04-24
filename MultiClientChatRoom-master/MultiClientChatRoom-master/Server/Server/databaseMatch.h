#ifndef _DATABASE_H_
#define _DATABASE_H_
#include<iostream>
#include<stdio.h>
#include "sqlite3.h"
#include<vector>
using namespace std;
struct match {

	string id, time, teamA, scoreA, scoreB, teamB;
	string tostring() {
		string infoMatch = id + "   " +time + "             " + teamA + "   " + scoreA + "-" + scoreB + "   " + teamB + "\r\n";
		return infoMatch;
	}
};
vector<match> Matchs;
const char* url2 = "matchData.db";
static int createDB2(const char* s) {
	sqlite3* DB;
	int exit = 0;
	exit = sqlite3_open(s, &DB);
	sqlite3_close(DB);
	return 0;
}
static int createTable2(const char* s) {
	sqlite3* DB;
	string sql = "CREATE TABLE MATCH("  \
		"ID INT PRIMARY KEY     NOT NULL," \
		"TIME           TEXT    NOT NULL," \
		"TEAMA          TEXT    NOT NULL," \
		"SCOREA         TEXT    NOT NULL," \
		"SCOREB         TEXT    NOT NULL," \
		"TEAMB          TEXT    NOT NULL);";


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
static int insertData2(const char* s) {
	sqlite3* DB;
	char* messageError;
	int exit = sqlite3_open(s, &DB);
	string sql = "INSERT INTO MATCH (ID,TIME,TEAMA,SCOREA,SCOREB,TEAMB) "
		"VALUES (1, '23:30', 'Roma', '?', '?','Atalanta' ); "
		"INSERT INTO MATCH (ID,TIME,TEAMA,SCOREA,SCOREB,TEAMB) "
		"VALUES (2, 'FT', 'Millwall', '1', '4','AFC Bournemouth'); "
		"INSERT INTO MATCH (ID,TIME,TEAMA,SCOREA,SCOREB,TEAMB) "
		"VALUES (3, 'HT', 'Birmingham City', '1', '1','Nottingham Forest' ); "
		"INSERT INTO MATCH (ID,TIME,TEAMA,SCOREA,SCOREB,TEAMB) "
		"VALUES (4, 'FT', 'Asernal', '0', '1','Everton' ); "
		;

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
static int callback2(void* NotUsed, int argc, char** argv, char** azColName)
{
	match A;
	for (int i = 0; i < argc; i++)
	{
		if (i == 0) {
			A.id = argv[0];
		}
		if (i == 1) {
			A.time = argv[1];
		}
		if (i == 2) {
			A.teamA = argv[2];
		}
		if (i == 3) {
			A.scoreA = argv[3];
		}
		if (i == 4) {
			A.scoreB = argv[4];
		}
		if (i == 5) {
			A.teamB = argv[5];
		}

	}
	Matchs.push_back(A);
	cout << "New" << endl;
	return 0;
}
static int selectData2(const char* s) {
	sqlite3* DB;
	int exit = sqlite3_open(s, &DB);
	string sql = "SELECT * FROM MATCH;";
	sqlite3_exec(DB, sql.c_str(), callback2, NULL, NULL);
	return 0;
}
#endif // !_DATABASE_H_
