class IRepositoryUser {
    virtual Users GetUserByLogin(string Login) = 0;
    virtual void InsertUser(Users U) = 0;
    virtual int GetNewId() = 0;
    virtual bool CheckUserLogin(string Login) = 0;
    virtual std::vector<Users> GetAllUsers() = 0;
    virtual void UpdateUserRole(int Id) = 0;
    virtual void UpdateUser(Users u) = 0;
    virtual void DeleteUser(Users U) = 0;
};

class IRepositoryMessage {
    //virtual
};