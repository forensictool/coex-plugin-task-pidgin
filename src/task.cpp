#include "task.h"
#include <QSettings>

TaskPidgin::TaskPidgin()
{
//    _contactFile = new QFile("contacts.xml");
//    _accountFile = new QFile("acount.xml");
    _msgFile = new QFile("msg.xml");
//    _currentFile = new QFile("")

    _pWriter = new QXmlStreamWriter();
    _pWriter->setDevice(_msgFile);
    _pWriter->setAutoFormatting(true);
    _pWriter->writeStartDocument();
    _pWriter->writeStartElement("add");

    _bDebug = false;
};

QString TaskPidgin::help()
{
    return "\t--debug - viewing debug messages";
};

QString TaskPidgin::name()
{
    return "PidginWin";
};

QString TaskPidgin::author()
{
    return "Igor Polyakov <fox.user.3@gmail.com>";
};

QString TaskPidgin::description()
{
    return "Task is search logs of Pidgin";
};

bool TaskPidgin::isSupportOS(const coex::ITypeOperationSystem *)
{
    return true; // any os
};

void TaskPidgin::setOption(QStringList options)
{
    if (options.contains("--debug"))
        _bDebug = true;
};

void TaskPidgin::processingContactListPidgin()
{
    QXmlStreamReader xmlReader;
    QFile fileXmlContacts(_pathCurrentFile);
    xmlReader.setDevice(&fileXmlContacts);
    QString accountPidgin, protoPidgin, namePidgin, emailPidgin;
    QString nameElem = "";
    if (fileXmlContacts.open(QIODevice::ReadOnly)) {
        xmlReader.readNext();
        while (!xmlReader.atEnd() && !xmlReader.hasError()) {
            QXmlStreamReader::TokenType token = xmlReader.readNext();
            if (token == QXmlStreamReader::StartDocument) {
                continue;
            }
            if (token == QXmlStreamReader::StartElement) {
                nameElem = xmlReader.name().toString();
                if (xmlReader.name() == "buddy") {
                    foreach(const QXmlStreamAttribute & attr, xmlReader.attributes()) {
                        if (attr.name().toString() == QLatin1String("account")) {
                            accountPidgin = attr.value().toString();
                        } else if (attr.name().toString() == QLatin1String("proto")) {
                            protoPidgin = attr.value().toString();
                        }
                    }
                    namePidgin = "";
                    emailPidgin = "";
                }
            }
            if (token == QXmlStreamReader::Characters) {
                if (nameElem == "name")
                    emailPidgin += xmlReader.text().toString();
                else if (nameElem == "alias")
                    namePidgin += xmlReader.text().toString();
            }
            if (token == QXmlStreamReader::EndElement) {
                if (xmlReader.name() == "buddy") {
                    accountPidgin = accountPidgin.trimmed();
                    protoPidgin = protoPidgin.trimmed();
                    namePidgin = namePidgin.trimmed();
                    emailPidgin = emailPidgin.trimmed();
                    writeContactList(accountPidgin, protoPidgin, namePidgin, emailPidgin);
                }
            }
        }
        fileXmlContacts.close();
    } else {
        qDebug() << "could not opening file \r\n";
    };
};


void TaskPidgin::processingAccountPidgin()
{
    QFile foundFile(_pathCurrentFile);
    QXmlStreamReader xmlReader;
    xmlReader.setDevice(&foundFile);
    QString name, email, protocol, password, nameElem = "";
    if (foundFile.open(QIODevice::ReadOnly)) {
        xmlReader.readNext();
        while (!xmlReader.atEnd() && !xmlReader.hasError()) {
            QXmlStreamReader::TokenType token = xmlReader.readNext();
            if (token == QXmlStreamReader::StartDocument) {
                continue;
            }
            if (token == QXmlStreamReader::StartElement) {
                nameElem = xmlReader.name().toString();
                if (xmlReader.name() == "account") {
                    name = "";
                    email = "";
                    protocol = "";
                    password = "";
                }
            }
            if (token == QXmlStreamReader::Characters) {
                if (nameElem == "alias")
                    name += xmlReader.text().toString();
                else if (nameElem == "password")
                    password += xmlReader.text().toString();
                else if (nameElem == "protocol")
                    protocol += xmlReader.text().toString();
                else if (nameElem == "name")
                    email += xmlReader.text().toString();
            }
            if (token == QXmlStreamReader::EndElement) {
                if (xmlReader.name() == "account") {
                    name = name.trimmed();
                    email = email.trimmed();
                    protocol = protocol.trimmed();
                    password = password.trimmed();
                    writeAccountInfo(name, email, protocol, password);
                }
            }
        }
        foundFile.close();
    } else {
        qDebug() << "Could not opening file \r\n";
    };
};

void TaskPidgin::processingLogPidgin()
{
    QString time, author, message, chatID, account, data, namePidgin;
    QFile fileLogs(_pathCurrentFile);
    QFileInfo fileInfo(_pathCurrentFile);
    QRegExp rxHead, rxBody;
    if (fileLogs.open(QIODevice::ReadOnly | QIODevice::Text)) {
        if (fileInfo.suffix() == "html") {
            rxHead.setPattern(".*h3.*with (.*) at (.*) \\d{2}:\\d{2}:\\d{2} on (.*) .*\\((.*)\\)");
        } else if (fileInfo.suffix() == "txt") {
            rxHead.setPattern(".* with (.*) at (.*) on (.*) \\((.*)\\)");
        }
        QString line = fileLogs.readLine();//read first line, get interesting info)
        rxHead.indexIn(line);
        chatID = rxHead.cap(1);
        account = rxHead.cap(3);
        data = rxHead.cap(2);
        namePidgin = rxHead.cap(4);
        if (fileInfo.suffix() == "html") {
            rxBody.setPattern(".*(\\d{2}:\\d{2}:\\d{2}).*b\\>(.*):\\<\\/b.*font\\>(.*)\\<br");
        } else if (fileInfo.suffix() == "txt") {
            rxBody.setPattern("\\((.*\\d{2}:\\d{2}:\\d{2})\\) (.*): (.*)");
        }
        while (!fileLogs.atEnd()) {
            line = fileLogs.readLine(); // read all file
            rxBody.indexIn(line);
            time = rxBody.cap(1);
            author = rxBody.cap(2);
            message = rxBody.cap(3);
            writeMessage(chatID, account, namePidgin, author, data + " " + time, message);
        }
    } else {
        qDebug() << "could not opening log file: " << fileInfo.absolutePath() << "\r\n";
    }
};

bool TaskPidgin::execute(const coex::IConfig *config)
{
    if (_bDebug) {
        qDebug() << "==========TaskPidgin::execute==========\n";
        qDebug() << " * Debug mode: On";
        qDebug() << " * InputFolder: " << config->inputFolder();
        qDebug() << " * OutputFolder: " << config->outputFolder() << "\n";
    };

    _inPath = config->inputFolder();
    _outPath = config->outputFolder();

    QDir inputDir(_inPath);
    QDir outputDir(_outPath);
    
    outputDir.mkdir("pidgin");
    outputDir.mkdir("pidgin/messages");
    QRegExp pidginPathAccount(".*purple/accounts.xml");
    QRegExp pidginPathContact(".*purple/blist.xml");
    QRegExp pidginPathLogHtml(".*purple/logs.*html");
    QRegExp pidginPathLogTxt(".*purple/logs.*txt");
    QDirIterator dirPath(config->inputFolder(), QDir::Files | QDir::NoSymLinks | QDir::Hidden, QDirIterator::Subdirectories);
    while (dirPath.hasNext()) {
        _pathCurrentFile = dirPath.path();
        if (dirPath.filePath().contains(pidginPathAccount)) {
            processingAccountPidgin();
            dirPath.next();
        } else if (dirPath.filePath().contains(pidginPathContact)) {
            processingContactListPidgin();
            dirPath.next();
        } else if (dirPath.filePath().contains(pidginPathLogTxt) || dirPath.next().contains(pidginPathLogHtml)) {
            processingLogPidgin();
            dirPath.next();
        }
    };
    if (_bDebug) {
        qDebug() << "\n==========TaskPidgin::finish==========\n\n";
    };
    return true;
}

TaskPidgin::~TaskPidgin()
{
    _pWriter->writeEndElement();
    _pWriter->writeEndElement();
    _pWriter->writeEndDocument();
    delete _pWriter;
    delete _msgFile;
}

void TaskPidgin::writeAccountInfo_field( QString field_name, QString field_value )
{
    if(!field_value.isEmpty())
    {
        _pWriter->writeStartElement("field");
        _pWriter->writeAttribute("name", field_name);
        _pWriter->writeCharacters(field_value);
        _pWriter->writeEndElement();
    }
}

void TaskPidgin::writeAccountInfo( QString name, QString email, QString protocol, QString password )
{
    QString md5Id = QCryptographicHash::hash( (email+protocol+name+password).toUtf8(), QCryptographicHash::Md5 ).toHex();
    _pWriter->writeStartElement("doc");
    writeAccountInfo_field("doc_type", "account");
    writeAccountInfo_field("id", "pidgin_"+ md5Id);
    writeAccountInfo_field("application", "pidgin");
    writeAccountInfo_field("account_id", name);
    writeAccountInfo_field("account_mail", email);
    writeAccountInfo_field("account_protocol", protocol);
    writeAccountInfo_field("account_password", password);
    _pWriter->writeEndElement();
}

void TaskPidgin::writeContactList( QString account, QString protocol, QString alias, QString name )
{
    QString md5Id = QCryptographicHash::hash( (protocol+account+name+alias).toUtf8(), QCryptographicHash::Md5 ).toHex();
    _pWriter->writeStartElement("doc");
    writeAccountInfo_field("doc_type", "contact");
    writeAccountInfo_field("id", "pidgin_"+ md5Id);
    writeAccountInfo_field("application", "pidgin");
    writeAccountInfo_field("contact_name", alias);
    writeAccountInfo_field("contact_account", account);
    writeAccountInfo_field("contact_protocol", protocol);
    writeAccountInfo_field("contact_id", name);
    _pWriter->writeEndElement();
}

void TaskPidgin::writeMessage( QString chathID, QString account, QString protocol, QString author, QString dataTime, QString message )
{
    QString md5Id = QCryptographicHash::hash((chathID+account+protocol+author+dataTime+message).toUtf8(), QCryptographicHash::Md5 ).toHex();
    _pWriter->writeStartElement("doc");
    writeAccountInfo_field("doc_type", "message");
    writeAccountInfo_field("id", "pidgin_"+ md5Id);
    writeAccountInfo_field("application", "pidgin");
    writeAccountInfo_field("message_chat_id", chathID);
    writeAccountInfo_field("message_account", account);
    writeAccountInfo_field("message_protocol", protocol);
    writeAccountInfo_field("message_author", author);
    writeAccountInfo_field("message_dataTime", dataTime);
    writeAccountInfo_field("message_text", message);
    _pWriter->writeEndElement();
}
