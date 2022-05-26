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
    SetHelpLabel();
    ui->afterLoginWidget->hide();
    ui->helpWidget->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButtonRun2048_clicked()
{
    QString dbLookupRes = FindElement("Grid2048");
    if (dbLookupRes == "")
    {
        auto wid = Window2048::AskFor("Width", "Please Select Width:", {"4","3","5","6","7","8","9","10"}, this);
        if (!wid)
            return;
        auto hei = Window2048::AskFor("Height", "Please Select Height:", {"4","3","5","6","7","8","9","10"}, this);
        if (!hei)
            return;
        uiWindow2048 = new Window2048(this, wid, hei);
    }
    else
    {
        uiWindow2048 = new Window2048(this, dbLookupRes);
    }
    uiWindow2048->show();
    hide();
}

void MainWindow::on_pushButtonRunKlotski_clicked()
{
    auto ans = WindowKlotski::AskFor("Game", "Please Select Game:", GameKlotski::GameSetStringList, this);
    if (ans == "")
        return;
    uiWindowKlotski = new WindowKlotski(this, ans);
    uiWindowKlotski->show();
    hide();
}

void MainWindow::LogIn()
{
    QString uname, pword;
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
            uname = record.at(0).toElement().text(); //"user1","user2"
            pword = record.at(1).toElement().text(); //"password1","password2"
            if(uname == ui->userNameEdit->text())
            {
                curuser=user.toElement();
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
        userName = uname;
        SetWelcomeLabel();
        ui->afterLoginWidget->show();
        ui->beforeLoginWidget->hide();
    } //if
    file.close();
}

void MainWindow::UpdateElement(const QString& name, const QString& content)
{
    //printf("start\n");
    //qDebug()<<curuser.tagName();
    QDomNodeList userList=curuser.childNodes();
    bool find=false;
    QDomNode Node;
    int num;
    for(int i=0;i<userList.count();i++)
    {
        Node=userList.at(i);
        if(name==Node.toElement().tagName())
        {
            num=i;
            find=true;
            break;
            //qDebug()<<Node.toElement().tagName();
        }
    }
    if(!find)
    {
        QDomElement node=mydoc.createElement(name);
        QDomText text=mydoc.createTextNode(content);
        text.setData(content);
        node.appendChild(text);
        curuser.appendChild(node);
    }
    else
    {
        QDomNode oldnode = Node.firstChild();
        Node.firstChild().setNodeValue(content);
        QDomNode newnode = Node.firstChild();
        Node.replaceChild(newnode,oldnode);
        curuser.childNodes().at(num)=Node;
    }
    QFile file("./xml/database.xml");
    if(!(file.open(QIODevice::WriteOnly | QIODevice::Truncate)))
    {
        //printf("false\n");
        return;
    }
    QTextStream out(&file);
    mydoc.save(out,4);
    file.close();
}

QString MainWindow::FindElement(const QString& element)
{
    QDomNodeList userList=curuser.childNodes();
    for(int i=0;i<userList.count();i++)
    {
        QDomNode Node=userList.at(i);
        //qDebug()<<Node.toElement().text();
        if(element==Node.toElement().tagName())
            return Node.toElement().text();
    }
    return "";
}

void MainWindow::on_loginBtn_clicked()
{
    LogIn();
}

void MainWindow::on_logoutBtn_clicked()
{
    ui->beforeLoginWidget->show();
    ui->afterLoginWidget->hide();
}

void MainWindow::SetWelcomeLabel()
{
    QString STYLE1 =
    "style = '"
    "font-size: 13px;"
    "font-weight: bold;"
    "color: #eee4da;'";
    QString STYLE2 =
    "style = '"
    "font-size: 25px;"
    "font-weight: bold;"
    "color: white;'";
    QString TEXT = "<font " + STYLE1 + ">WELCOME</font><br/>";
    TEXT += "<font " + STYLE2 + ">";
    TEXT += userName + "</font>";
    ui->welcomeLabel->setText(TEXT);
}

void MainWindow::SetHelpLabel()
{
    QString STYLE1 =
    "style = '"
    "font-size: 40px;"
    "font-weight: bold;"
    "color: #eee4da;'";
    QString STYLE2 =
    "style = '"
    "font-size: 18px;"
    "font-weight: bold;"
    "color: white;'";
    QString TEXT = "<font " + STYLE1 + ">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;HELP</font><br/>";
    TEXT += "<font " + STYLE2 + ">" + "Global:" + "</font><br/>";
    TEXT += "<font " + STYLE2 + ">" + "&nbsp;&nbsp;&nbsp;&nbsp;Esc - Exit to Main Menu" + "</font><br/>";
    TEXT += "<font " + STYLE2 + ">" + "2048:" + "</font><br/>";
    TEXT += "<font " + STYLE2 + ">" + "&nbsp;&nbsp;&nbsp;&nbsp;W/A/S/D - Swipe Board" + "</font><br/>";
    TEXT += "<font " + STYLE2 + ">" + "&nbsp;&nbsp;&nbsp;&nbsp;Up/Dn/Lf/Rt - Swipe Board" + "</font><br/>";
    TEXT += "<font " + STYLE2 + ">" + "Klotski:" + "</font><br/>";
    TEXT += "<font " + STYLE2 + ">" + "&nbsp;&nbsp;&nbsp;&nbsp;W/A/S/D - Move Selected Brick" + "</font><br/>";
    TEXT += "<font " + STYLE2 + ">" + "&nbsp;&nbsp;&nbsp;&nbsp;Up/Dn/Lf/Rt - Change Selected Brick" + "</font>";
    ui->helpLabel->setText(TEXT);
}

void MainWindow::on_helpBtn_clicked()
{
    ui->helpWidget->show();
    ui->afterLoginWidget->hide();
}

void MainWindow::on_backBtn_clicked()
{
    ui->afterLoginWidget->show();
    ui->helpWidget->hide();
}

