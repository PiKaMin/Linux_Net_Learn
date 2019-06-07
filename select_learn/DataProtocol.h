enum DataType{
  TYPE_LOGIN,
  TYPE_LOGIN_RESULT,
  TYPE_LOGOUT,
  TYPE_LOGOUT_RESULT,
  TYPE_ERROR
};

struct DataHeader{
  int dataLength;
  DataType type;
};

struct DataLogin : public DataHeader{
    DataLogin(){
      dataLength = sizeof(DataLogin);
      type = TYPE_LOGIN;
    }
    char username[64];
    char password[64];
};

struct DataLoginRes : public DataHeader{
    DataLoginRes(){
      dataLength = sizeof(DataLoginRes);
      type = TYPE_LOGIN_RESULT;
    }
    int res = 1;

};

struct DataLogout : public DataHeader{
    DataLogout(){
      dataLength = sizeof(DataLogout);
      type = TYPE_LOGOUT;
    }
    char username[64];

};

struct DataLogoutRes : public DataHeader{
    DataLogoutRes(){
      dataLength = sizeof(DataLogoutRes);
      type = TYPE_LOGOUT_RESULT;
    }
    int res = 1;
};

struct DataError : public DataHeader{
  DataError(){
    dataLength = sizeof(DataError);
    type = TYPE_ERROR;
  }
  int res = 0;
};

