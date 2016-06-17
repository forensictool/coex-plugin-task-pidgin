#ifndef __TASK_SEARCH_GIT_REPOSITORY_H__
#define __TASK_SEARCH_GIT_REPOSITORY_H__

#include "coex/coex.h"

#include <QCryptographicHash>
#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QDirIterator>
#include <QDataStream>
#include <QFile>
#include <QFileInfo>
#include <QString>
#include <QStringList>
#include <QXmlStreamWriter>

class TaskPidgin : coex::ITask
{
	public:
        TaskPidgin();
		virtual QString help();
		virtual QString name();
		virtual QString author();
		virtual QString description();

		virtual bool isSupportOS(const coex::ITypeOperationSystem *os);
		virtual void setOption(QStringList options);

        /*!
         * \brief TaskPidgin::processingAccountPidgin - Вычитывает xml файл аккаунта Pidgin. Преобразует к структурированному xml
         */
        void processingAccountPidgin();

        /*!
         * \brief TaskPidgin::processingLogPidgin - Вычитывает журнальные файлы Pidgin. Формат файлов html, txt.
         * Преобразует к структурированному формату xml
         */
        void processingLogPidgin();

        /*!
         * \brief TaskPidgin::processingContactListPidgin
         */
        void processingContactListPidgin();

        /*!
         * \brief TaskPidgin::execute - Некий main класса TaskPidgin
         */
		virtual bool execute(const coex::IConfig *config);
        
        ~TaskPidgin();

	private:

        bool _bDebug;

        void writeAccountInfo_field(QString field_name, QString field_value );
        void writeAccountInfo(QString name, QString email, QString protocol, QString password);
        void writeContactList(QString account, QString protocol, QString alias, QString name);
        void writeMessage(QString chathID, QString account, QString protocol, QString author, QString dataTime, QString message);

        QXmlStreamWriter *_pWriter;
        QFile *_msgFile;
        QString _pathCurrentFile;
        QString _inPath;
        QString _outPath;
};

extern "C"
{
	coex::ITask* createTask();
}

#endif // __TASK_SEARCH_GIT_REPOSITORY_H__
