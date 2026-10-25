/********************************************************************************
** Form generated from reading UI file 'mywidget.ui'
**
** Created by: Qt User Interface Compiler version 6.11.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MYWIDGET_H
#define UI_MYWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MyWidget
{
public:
    QVBoxLayout *verticalLayout;
    QTableWidget *tableWidget;
    QFormLayout *formLayout;
    QLabel *studentIdLabel;
    QLineEdit *studentIdEdit;
    QLabel *classLabel;
    QLineEdit *classEdit;
    QLabel *nameLabel;
    QLineEdit *nameEdit;
    QLabel *phoneLabel;
    QLineEdit *phoneEdit;
    QHBoxLayout *buttonLayout;
    QPushButton *importButton;
    QPushButton *addButton;
    QPushButton *exportButton;

    void setupUi(QWidget *MyWidget)
    {
        if (MyWidget->objectName().isEmpty())
            MyWidget->setObjectName("MyWidget");
        MyWidget->resize(820, 620);
        MyWidget->setMinimumSize(QSize(660, 480));
        QFont font;
        font.setFamilies({QString::fromUtf8("Microsoft JhengHei")});
        font.setPointSize(11);
        MyWidget->setFont(font);
        verticalLayout = new QVBoxLayout(MyWidget);
        verticalLayout->setSpacing(10);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(16, 16, 16, 16);
        tableWidget = new QTableWidget(MyWidget);
        if (tableWidget->columnCount() < 4)
            tableWidget->setColumnCount(4);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        tableWidget->setObjectName("tableWidget");
        tableWidget->setRowCount(0);
        tableWidget->setColumnCount(4);

        verticalLayout->addWidget(tableWidget);

        formLayout = new QFormLayout();
        formLayout->setObjectName("formLayout");
        formLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        formLayout->setVerticalSpacing(10);
        studentIdLabel = new QLabel(MyWidget);
        studentIdLabel->setObjectName("studentIdLabel");

        formLayout->setWidget(0, QFormLayout::ItemRole::LabelRole, studentIdLabel);

        studentIdEdit = new QLineEdit(MyWidget);
        studentIdEdit->setObjectName("studentIdEdit");

        formLayout->setWidget(0, QFormLayout::ItemRole::FieldRole, studentIdEdit);

        classLabel = new QLabel(MyWidget);
        classLabel->setObjectName("classLabel");

        formLayout->setWidget(1, QFormLayout::ItemRole::LabelRole, classLabel);

        classEdit = new QLineEdit(MyWidget);
        classEdit->setObjectName("classEdit");

        formLayout->setWidget(1, QFormLayout::ItemRole::FieldRole, classEdit);

        nameLabel = new QLabel(MyWidget);
        nameLabel->setObjectName("nameLabel");

        formLayout->setWidget(2, QFormLayout::ItemRole::LabelRole, nameLabel);

        nameEdit = new QLineEdit(MyWidget);
        nameEdit->setObjectName("nameEdit");

        formLayout->setWidget(2, QFormLayout::ItemRole::FieldRole, nameEdit);

        phoneLabel = new QLabel(MyWidget);
        phoneLabel->setObjectName("phoneLabel");

        formLayout->setWidget(3, QFormLayout::ItemRole::LabelRole, phoneLabel);

        phoneEdit = new QLineEdit(MyWidget);
        phoneEdit->setObjectName("phoneEdit");

        formLayout->setWidget(3, QFormLayout::ItemRole::FieldRole, phoneEdit);


        verticalLayout->addLayout(formLayout);

        buttonLayout = new QHBoxLayout();
        buttonLayout->setObjectName("buttonLayout");
        importButton = new QPushButton(MyWidget);
        importButton->setObjectName("importButton");
        importButton->setMinimumSize(QSize(0, 32));

        buttonLayout->addWidget(importButton);

        addButton = new QPushButton(MyWidget);
        addButton->setObjectName("addButton");
        addButton->setMinimumSize(QSize(0, 32));

        buttonLayout->addWidget(addButton);

        exportButton = new QPushButton(MyWidget);
        exportButton->setObjectName("exportButton");
        exportButton->setMinimumSize(QSize(0, 32));

        buttonLayout->addWidget(exportButton);


        verticalLayout->addLayout(buttonLayout);

        verticalLayout->setStretch(0, 1);
#if QT_CONFIG(shortcut)
        studentIdLabel->setBuddy(studentIdEdit);
        classLabel->setBuddy(classEdit);
        nameLabel->setBuddy(nameEdit);
        phoneLabel->setBuddy(phoneEdit);
#endif // QT_CONFIG(shortcut)
        QWidget::setTabOrder(studentIdEdit, classEdit);
        QWidget::setTabOrder(classEdit, nameEdit);
        QWidget::setTabOrder(nameEdit, phoneEdit);
        QWidget::setTabOrder(phoneEdit, addButton);
        QWidget::setTabOrder(addButton, importButton);
        QWidget::setTabOrder(importButton, exportButton);
        QWidget::setTabOrder(exportButton, tableWidget);

        retranslateUi(MyWidget);

        QMetaObject::connectSlotsByName(MyWidget);
    } // setupUi

    void retranslateUi(QWidget *MyWidget)
    {
        MyWidget->setWindowTitle(QCoreApplication::translate("MyWidget", "\351\200\232\350\250\212\351\214\204", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("MyWidget", "\345\255\270\350\231\237", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("MyWidget", "\347\217\255\347\264\232", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("MyWidget", "\345\247\223\345\220\215", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tableWidget->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("MyWidget", "\351\233\273\350\251\261", nullptr));
        studentIdLabel->setText(QCoreApplication::translate("MyWidget", "\345\255\270\350\231\237\357\274\232", nullptr));
        classLabel->setText(QCoreApplication::translate("MyWidget", "\347\217\255\347\264\232\357\274\232", nullptr));
        nameLabel->setText(QCoreApplication::translate("MyWidget", "\345\247\223\345\220\215\357\274\232", nullptr));
        phoneLabel->setText(QCoreApplication::translate("MyWidget", "\351\233\273\350\251\261\357\274\232", nullptr));
        importButton->setText(QCoreApplication::translate("MyWidget", "\345\214\257\345\205\245", nullptr));
        addButton->setText(QCoreApplication::translate("MyWidget", "\346\226\260\345\242\236", nullptr));
        exportButton->setText(QCoreApplication::translate("MyWidget", "\345\214\257\345\207\272", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MyWidget: public Ui_MyWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MYWIDGET_H
