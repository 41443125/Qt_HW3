#include "mywidget.h"
#include <array>

namespace {
bool parseRecords(const QString &text, QVector<QStringList> &records, QString &error)
{
    enum State { Start, Unquoted, Quoted, AfterQuote };
    State state = Start;
    QString field;
    QStringList record;
    bool touched = false;

    auto finishRecord = [&]() {
        record.append(field);
        // 忽略完全空白的記錄，保留含有分隔符號的記錄供後續檢查。
        if (record.size() != 1 || !record.first().trimmed().isEmpty())
            records.append(record);
        field.clear();
        record.clear();
        state = Start;
        touched = false;
    };

    for (int i = 0; i < text.size(); ++i) {
        const QChar ch = text.at(i);
        if (state == Quoted) {
            if (ch == QLatin1Char('"')) {
                if (i + 1 < text.size() && text.at(i + 1) == QLatin1Char('"')) {
                    field += QLatin1Char('"');
                    ++i;
                } else {
                    state = AfterQuote;
                }
            } else {
                field += ch;  // 引號內的逗號與換行都是欄位內容。
            }
            continue;
        }
        if (ch == QLatin1Char('\r') || ch == QLatin1Char('\n')) {
            finishRecord();
            if (ch == QLatin1Char('\r') && i + 1 < text.size()
                && text.at(i + 1) == QLatin1Char('\n'))
                ++i;
            continue;
        }
        touched = true;
        if (ch == QLatin1Char(',')) {
            record.append(field);
            field.clear();
            state = Start;
        } else if (state == Start && ch == QLatin1Char('"')) {
            state = Quoted;
        } else if (state == AfterQuote || ch == QLatin1Char('"')) {
            error = QStringLiteral("CSV 第 %1 筆記錄的雙引號格式錯誤。")
                        .arg(records.size() + 1);
            return false;
        } else {
            field += ch;
            state = Unquoted;
        }
    }
    if (state == Quoted) {
        error = QStringLiteral("CSV 有未關閉的雙引號，請檢查檔案。");
        return false;
    }
    if (touched || !record.isEmpty() || !field.isEmpty())
        finishRecord();
    return true;
}

int headerIndex(QString name)
{
    name = name.trimmed().toLower();
    name.remove(QLatin1Char(' '));
    name.remove(QLatin1Char('_'));
    name.remove(QLatin1Char('-'));
    if (name == QStringLiteral("學號") || name == QStringLiteral("学号")
        || name == QStringLiteral("studentid") || name == QStringLiteral("id"))
        return 0;
    if (name == QStringLiteral("班級") || name == QStringLiteral("班级")
        || name == QStringLiteral("class") || name == QStringLiteral("classname"))
        return 1;
    if (name == QStringLiteral("姓名") || name == QStringLiteral("name"))
        return 2;
    if (name == QStringLiteral("電話") || name == QStringLiteral("电话")
        || name == QStringLiteral("phone") || name == QStringLiteral("tel")
        || name == QStringLiteral("telephone"))
        return 3;
    return -1;
}

QString quote(QString field)
{
    if (field.contains(QLatin1Char(',')) || field.contains(QLatin1Char('"'))
        || field.contains(QLatin1Char('\r')) || field.contains(QLatin1Char('\n'))) {
        field.replace(QStringLiteral("\""), QStringLiteral("\"\""));
        return QLatin1Char('"') + field + QLatin1Char('"');
    }
    return field;
}
}

bool Csv::validate(const Contact &contact, QString &error)
{
    error.clear();
    const auto fields = contact.fields();
    const auto headers = Contact::headers();
    for (int i = 0; i < 4; ++i) {
        if (fields.at(i).trimmed().isEmpty()) {
            error = QStringLiteral("「%1」不可空白。").arg(headers.at(i));
            return false;
        }
    }
    return true;
}

bool Csv::decode(const QByteArray &bytes, QVector<Contact> &contacts, QString &error)
{
    error.clear();
    const QByteArray payload = bytes.startsWith(QByteArray::fromHex("efbbbf"))
                                   ? bytes.mid(3) : bytes;
    const QString text = QString::fromUtf8(payload);
    // 往返檢查可在 Qt 5 / Qt 6 都拒絕無效 UTF-8，避免中文靜默變成亂碼。
    if (text.toUtf8() != payload) {
        error = QStringLiteral("CSV 不是有效的 UTF-8。請另存為「CSV UTF-8」後再匯入。");
        return false;
    }

    QVector<QStringList> records;
    if (!parseRecords(text, records, error))
        return false;
    if (records.isEmpty()) {
        error = QStringLiteral("CSV 是空檔案，請加入標題與資料。");
        return false;
    }

    const QStringList header = records.first();
    std::array<int, 4> columns{{-1, -1, -1, -1}};
    for (int i = 0; i < header.size(); ++i) {
        const int target = headerIndex(header.at(i));
        if (target < 0)
            continue;  // 允許其他欄位，但不匯入它們。
        if (columns[target] != -1) {
            error = QStringLiteral("CSV 標題「%1」重複。").arg(Contact::headers().at(target));
            return false;
        }
        columns[target] = i;
    }
    for (int i = 0; i < 4; ++i) {
        if (columns[i] == -1) {
            error = QStringLiteral("CSV 第一列缺少「%1」欄位。必要標題：學號、班級、姓名、電話。")
                        .arg(Contact::headers().at(i));
            return false;
        }
    }

    QVector<Contact> parsed;
    for (int i = 1; i < records.size(); ++i) {
        const auto &row = records.at(i);
        if (row.size() != header.size()) {
            error = QStringLiteral("CSV 第 %1 筆資料有 %2 欄，標題有 %3 欄。")
                        .arg(i).arg(row.size()).arg(header.size());
            return false;
        }
        Contact contact{row.at(columns[0]).trimmed(), row.at(columns[1]).trimmed(),
                        row.at(columns[2]).trimmed(), row.at(columns[3]).trimmed()};
        QString validationError;
        if (!validate(contact, validationError)) {
            error = QStringLiteral("CSV 第 %1 筆資料：%2").arg(i).arg(validationError);
            return false;
        }
        parsed.append(contact);
    }
    contacts = parsed;  // 全部通過檢查後才更新輸出。
    return true;
}

QByteArray Csv::encode(const QVector<Contact> &contacts)
{
    QString text = Contact::headers().join(QLatin1Char(',')) + QStringLiteral("\r\n");
    for (const auto &contact : contacts) {
        QStringList fields;
        for (const auto &field : contact.fields())
            fields.append(quote(field));
        text += fields.join(QLatin1Char(',')) + QStringLiteral("\r\n");
    }
    return QByteArray::fromHex("efbbbf") + text.toUtf8();
}

#include "mywidget.h"

#include "ui_mywidget.h"

#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QHeaderView>
#include <QMessageBox>
#include <QSaveFile>

MyWidget::MyWidget(QWidget *parent) : QWidget(parent), ui(new Ui::MyWidget)
{
    ui->setupUi(this);
    // 啟動時只有欄名，沒有空白資料列，也不自動載入範例 CSV。
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setColumnCount(4);
    ui->tableWidget->setHorizontalHeaderLabels(Contact::headers());
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->exportButton->setEnabled(false);
    connect(ui->addButton, &QPushButton::clicked, this, &MyWidget::addFromForm);
    connect(ui->importButton, &QPushButton::clicked, this, &MyWidget::chooseImportFile);
    connect(ui->exportButton, &QPushButton::clicked, this, &MyWidget::chooseExportFile);
}

MyWidget::~MyWidget() { delete ui; }

void MyWidget::appendRow(const Contact &contact)
{
    const int row = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(row);
    const auto fields = contact.fields();
    for (int column = 0; column < 4; ++column) {
        auto *item = new QTableWidgetItem;
        // 所有欄位都以字串存放，保留學號與電話開頭的 0。
        item->setText(fields.at(column));
        ui->tableWidget->setItem(row, column, item);
    }
    ui->exportButton->setEnabled(true);
}

bool MyWidget::addContact(const Contact &contact, QString &error)
{
    error.clear();
    const Contact cleaned{contact.studentId.trimmed(), contact.className.trimmed(),
                          contact.name.trimmed(), contact.phone.trimmed()};
    if (!Csv::validate(cleaned, error))
        return false;
    appendRow(cleaned);
    ui->tableWidget->scrollToBottom();
    return true;
}

void MyWidget::addFromForm()
{
    const Contact contact{ui->studentIdEdit->text(), ui->classEdit->text(),
                          ui->nameEdit->text(), ui->phoneEdit->text()};
    QString error;
    if (!addContact(contact, error)) {
        QMessageBox::warning(this, tr("無法新增"), error);
        return;
    }
    ui->studentIdEdit->clear();
    ui->classEdit->clear();
    ui->nameEdit->clear();
    ui->phoneEdit->clear();
    ui->studentIdEdit->setFocus();
}

bool MyWidget::importCsvFile(const QString &path, QString &error, int *importedCount)
{
    error.clear();
    if (importedCount)
        *importedCount = 0;
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        error = tr("無法開啟檔案：%1").arg(file.errorString());
        return false;
    }
    const QByteArray bytes = file.readAll();
    if (file.error() != QFileDevice::NoError) {
        error = tr("讀取失敗：%1").arg(file.errorString());
        return false;
    }
    QVector<Contact> contacts;
    if (!Csv::decode(bytes, contacts, error))
        return false;
    if (contacts.isEmpty()) {
        error = tr("CSV 只有標題，沒有可匯入的資料。");
        return false;
    }

    // 允許相同學號重複新增或匯入。
    for (const auto &contact : contacts)
        appendRow(contact);
    ui->tableWidget->scrollToBottom();
    if (importedCount)
        *importedCount = static_cast<int>(contacts.size());
    return true;
}

void MyWidget::chooseImportFile()
{
    const QString path = QFileDialog::getOpenFileName(
        this, tr("選擇要匯入的 CSV"), QString(), tr("CSV 檔案 (*.csv);;所有檔案 (*)"));
    if (path.isEmpty())
        return;  // 取消選檔時保留原本資料。
    QString error;
    if (!importCsvFile(path, error))
        QMessageBox::warning(this, tr("匯入失敗"), error);
}

bool MyWidget::exportCsvFile(const QString &path, QString &error) const
{
    error.clear();
    QVector<Contact> contacts;
    for (int row = 0; row < ui->tableWidget->rowCount(); ++row) {
        contacts.append({ui->tableWidget->item(row, 0)->text(),
                         ui->tableWidget->item(row, 1)->text(),
                         ui->tableWidget->item(row, 2)->text(),
                         ui->tableWidget->item(row, 3)->text()});
    }
    QSaveFile file(path);
    if (!file.open(QIODevice::WriteOnly)) {
        error = tr("無法建立檔案：%1").arg(file.errorString());
        return false;
    }
    const QByteArray bytes = Csv::encode(contacts);
    if (file.write(bytes) != bytes.size()) {
        error = tr("寫入失敗：%1").arg(file.errorString());
        file.cancelWriting();
        return false;
    }
    if (!file.commit()) {
        error = tr("儲存失敗：%1").arg(file.errorString());
        return false;
    }
    return true;
}

void MyWidget::chooseExportFile()
{
    QString path = QFileDialog::getSaveFileName(
        this, tr("匯出通訊錄"), QStringLiteral("contacts.csv"), tr("CSV 檔案 (*.csv)"));
    if (path.isEmpty())
        return;
    // 對話框以最終路徑處理覆寫提示，不在此偷偷改成另一個檔名。
    if (QFileInfo(path).suffix().compare(QStringLiteral("csv"), Qt::CaseInsensitive) != 0) {
        QMessageBox::warning(this, tr("副檔名錯誤"), tr("請以 .csv 作為檔名結尾。"));
        return;
    }
    QString error;
    if (!exportCsvFile(path, error)) {
        QMessageBox::warning(this, tr("匯出失敗"), error);
        return;
    }
}
