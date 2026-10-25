#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QString>
#include <QStringList>
struct Contact
{
    QString studentId;
    QString className;
    QString name;
    QString phone;

    QStringList fields() const { return {studentId, className, name, phone}; }
    static QStringList headers()
    {
        return {QStringLiteral("學號"), QStringLiteral("班級"),
                QStringLiteral("姓名"), QStringLiteral("電話")};
    }
};


#include <QByteArray>
#include <QVector>
namespace Csv {
// CSV 資料的解析、編碼與欄位驗證。
bool decode(const QByteArray &bytes, QVector<Contact> &contacts, QString &error);
QByteArray encode(const QVector<Contact> &contacts);
bool validate(const Contact &contact, QString &error);
}

// 引入 Qt 視窗基底類別。
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class MyWidget; }
QT_END_NAMESPACE

class MyWidget : public QWidget
{
    Q_OBJECT
    // 公開的視窗建構與資料操作介面。
public:
    explicit MyWidget(QWidget *parent = nullptr);
    // 釋放視窗所持有的介面資源。
    ~MyWidget() override;
    bool addContact(const Contact &contact, QString &error);
    bool importCsvFile(const QString &path, QString &error, int *importedCount = nullptr);
    bool exportCsvFile(const QString &path, QString &error) const;
private slots:
    void addFromForm();
    void chooseImportFile();
    void chooseExportFile();

private:
    Ui::MyWidget *ui;
    void appendRow(const Contact &contact);
};
#endif
