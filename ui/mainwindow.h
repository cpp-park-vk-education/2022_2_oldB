#ifndef MAINWINDOW_H
#define MAINWINDOW_H



#include <QMainWindow>
#include <QKeyEvent>
#include "Server.h"
#include "Client.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
                    MainWindow(QWidget *parent = nullptr);
                    ~MainWindow();

private slots:
    void            on_connectButton_clicked();
    void            on_hostButton_clicked();

    void            on_exitButton_clicked();
    void            on_backButton_2_clicked();

    void            on_connectButton_2_clicked();
    void            on_exitButton_2_clicked();

    void            on_backButton_3_clicked();
    void            on_startButton_clicked();

    void            on_exitButton_3_clicked();
    void            on_sendButton_clicked();

    void            on_exitLobbyButton_clicked();
    void            on_leaveLobbyButton_clicked();

    void            onMessageReceived(std::vector<std::string>& message_set);
    void            onErrorOccured(system::error_code const& ec);

    void            onEnterPressed();
    void            onConnected();

    void            onBadConnect(system::error_code const& ec);
    void            onServerMessageReceived(std::vector<std::string>& message_set);

private:
    void                        sendMessage();
    void                        badConnect(system::error_code const& ec);

    void                        updateUserlist(std::vector<std::string> const& user_list);
    void                        setupAppearance();

    void                        clearWidgets();
    void                        clearInputs();

    void                        updateRoomName(std::string_view room_name);
    void                        onAdminLeave();

    std::vector<std::string>    strToVectorUserlist(std::string_view user_list_str);
    void                        connectClient(std::unique_ptr<Client>&& client);

    bool                        validateInputs(system::error_code const& bad_address);
    bool                        validateInputs();

private:
    Ui::MainWindow*             ui;

    std::unique_ptr<Client>     client;
    std::unique_ptr<Server>     server;

    bool                        hosted{ false };
    bool                        connected{ false };
};
#endif // MAINWINDOW_H
