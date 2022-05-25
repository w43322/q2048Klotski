#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    QFontDatabase::addApplicationFont(":/fonts/Monaco.ttf");
    QFontDatabase::addApplicationFont(":/fonts/Menlo-Bold.ttf");
    QFontDatabase::addApplicationFont(":/fonts/Menlo-Regular.ttf");
    ui->setupUi(this);
    ui->afterLoginWidget->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButtonRun2048_clicked()
{
    auto wid = Window2048::AskFor("Width", "Please Select Width:", {"4","3","5","6","7","8","9","10"}, this);
    if (!wid)
        return;
    auto hei = Window2048::AskFor("Height", "Please Select Height:", {"4","3","5","6","7","8","9","10"}, this);
    if (!hei)
        return;
    uiWindow2048 = new Window2048(this, wid, hei);
    uiWindow2048->show();
    hide();
}

void MainWindow::on_pushButtonRunKlotski_clicked()
{
    auto ans = WindowKlotski::AskFor("Game", "Please Select Game:", {"横刀立马", "指挥若定", "数字华容道"}, this);
    if (ans == "")
        return;
    uiWindowKlotski = new WindowKlotski(this, ans);
    uiWindowKlotski->show();
    hide();
}

void MainWindow::LogIn()
{
    QFile file(":/xml/database.xml");  //创建XML文件对象
    mydoc.setContent(&file);  //将XML对象赋给QdomDocument类型的Qt文档句柄
    QDomElement root = mydoc.documentElement();  //获取XML文档的DOM根元素
    if(root.hasChildNodes())
    {
        QDomNodeList userList = root.childNodes();  //获取根元素的全部子节点
        bool exist = false;  //指示用户是否存在
        for(int i = 0; i < userList.count(); i++)
        {
            QDomNode user = userList.at(i);  //根据当前索引i获取用户节点元素
            QDomNodeList record = user.childNodes();  //该用户的全部属性元素
           //解析出用户名及密码
            QString uname = record.at(0).toElement().text(); //"user1","user2"
            QString pword = record.at(1).toElement().text(); //"password1","password2"
            if(uname == ui->userNameEdit->text())
            {
                exist = true;  //用户存在
                if(!(pword == ui->passwordEdit->text()))
                {
                    QMessageBox::warning(0, QObject::tr("failed"), "密码错误，请重新输入！");
                    ui->passwordEdit->clear();
                    ui->passwordEdit->setFocus();
                    return;
                }
            }
        } //for
        if(!exist)
        {
            QMessageBox::warning(0, QObject::tr("failed"), "此用户不存在，请重新输入！");
            ui->userNameEdit->clear();
            ui->passwordEdit->clear();
            ui->userNameEdit->setFocus();
            return;
        }
       //用户存在且密码验证通过
        ui->afterLoginWidget->show();
        ui->beforeLoginWidget->hide();
    } //if
}

void MainWindow::on_loginBtn_clicked()
{
    LogIn();
}

