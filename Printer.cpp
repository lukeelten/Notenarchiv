#include <QPrinter>
#include <QPrintDialog>
#include <QString>
#include <QList>
#include <QSqlTableModel>
#include <QTextDocument>
#include <QSqlRecord>

#include "Printer.h"
#include "Database.h"

Printer::Printer() : m_printer(new QPrinter), m_model(new QSqlTableModel(0, DB->GetDatabase())), m_doc(new QTextDocument) {
    html_before = "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">"
            "<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">"
            "p, li { white-space: pre-wrap; } .table { border: none; } .table p { text-align: center; } .table span { font-size: 14pt; }"
            "</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">;"
            "<table class=\"table\"><tr style=\"width:100%;\"><th style=\"width:40%;\"><p><span>Name</span></p></th><th style=\"width: 20%;\"><p><span>Fach-Nr</span></p></th><th style=\"width: 40%;\"><p><span>Komponist</span></p></th></tr>";
    html_row = "<tr><td><p><span>%1</span></p></td><td><p><span>%2</span></p></td><td><p><span>%3</span></p></td></tr>";

    html_after = "</table></body></html>";
}

Printer::~Printer() {
    if (m_printer) {
        delete m_printer;
        m_printer = nullptr;
    }

    if (m_model) {
        delete m_model;
        m_model = nullptr;
    }

    if (m_doc) {
        delete m_doc;
        m_doc = nullptr;
    }
}

bool Printer::PreparePrinter() {
    QPrintDialog dia(m_printer);

    dia.setOption(QPrintDialog::None);
    if (dia.exec() == QDialog::Accepted) {
        return true;
    }

    return false;
}

void Printer::PrepareTable() {
    m_model->setTable("notenarchiv");
    m_model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    m_model->setSort(1, Qt::AscendingOrder);

    m_model->select();
}

void Printer::PrepareDocument() {
    m_text.clear();
    m_text.append(html_before);

    int nums = m_model->rowCount();

    for (int i = 0; i < nums; i++) {
        QSqlRecord rec = m_model->record(i);

        m_text.append(html_row.arg(rec.value("name").toString(), rec.value("fach").toString(), rec.value("komponist").toString()));
    }

    m_text.append(html_after);
}