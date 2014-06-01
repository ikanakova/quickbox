#include "theapp.h"
//#include "sicliscriptdriver.h"

#include <sidevicedriver.h>
#include <simessage.h>

#include <qf/core/exception.h>
#include <qf/core/utils.h>
//#include <qfscriptdriver.h>
//#include <qffileutils.h>

#include <QMetaType>
#include <QSettings>
#include <QSqlDatabase>
//#include <QScriptEngine>
#include <QFile>
#include <QIcon>

#define SCRIPT_DEBUGGER
#ifdef SCRIPT_DEBUGGER
//#include <QScriptEngineDebugger>
#endif

//========================================================================
//                      TheApp
//========================================================================
TheApp::TheApp(int & argc, char ** argv)
: QApplication(argc, argv)
{
	setApplicationVersion(versionString());
	setWindowIcon(QIcon("://images/qsicli.png"));
	f_siDriver = NULL;
	f_scriptDriver = NULL;
	f_cardLog = NULL;
	f_cardLogFile = NULL;

	//QFScriptDriver::prependSearchDir(QFFileUtils::joinPath(QCoreApplication::applicationDirPath(), "divers/qsicli/scripts"));
}

TheApp::~TheApp()
{
	//qDebug() << "destructing theapp";
	if(f_siDriver) f_siDriver->closeCommPort();
	QF_SAFE_DELETE(f_cardLog);
	QF_SAFE_DELETE(f_cardLogFile);
}

TheApp* TheApp::instance(bool throw_exc)
{
	TheApp *a = qobject_cast<TheApp*>(QApplication::instance());
	if(!a && throw_exc) QF_EXCEPTION("aplikace dosud neni inicializovana");
	return a;
}

SIDeviceDriver* TheApp::siDriver()
{
	if(!f_siDriver) {
		f_siDriver = new SIDeviceDriver(this);
		//connect(f_siDriver, SIGNAL(messageReady(const SIMessageBase&)), this, SLOT(onMessageReady(const SIMessageBase&)));
	}
	return f_siDriver;
}

QSqlDatabase TheApp::sqlConnection()
{
	QSqlDatabase db = QSqlDatabase::database(QSqlDatabase::defaultConnection, false);
	if(!db.isValid()) {
		QSettings settings;
		QString driver = settings.value("sql/connection/driver", "QMYSQL").toString();
		emitLogRequest(qf::core::Log::LOG_INF, tr("Creatng DB connection usin griver '%1'").arg(driver));
		db = QSqlDatabase::addDatabase(driver);
	}
	return db;
}

QTextStream& TheApp::cardLog()
{
	if(!f_cardLog) {
		QSettings settings;
		QString fn = settings.value("app/logging/cardLog").toString();
		if(!fn.isEmpty()) {
			QF_SAFE_DELETE(f_cardLogFile);
			f_cardLogFile = new QFile(fn);
			if(f_cardLogFile->open(QFile::Append)) {
				emitLogRequest(qf::core::Log::LOG_INF, tr("Openned card log file '%1' for append.").arg(fn));
				f_cardLog = new QTextStream(f_cardLogFile);
			}
			else {
				emitLogRequest(qf::core::Log::LOG_ERR, tr("Can't open card log file '%1' for append.").arg(fn));
			}
		}
		if(!f_cardLog) {
			/// log to stdout
			f_cardLog = new QTextStream(stdout);
			emitLogRequest(qf::core::Log::LOG_INF, tr("Card log file redirected to stdout."));
		}
	}
	return *f_cardLog;
}

void TheApp::closeCardLog()
{
	emitLogRequest(qf::core::Log::LOG_INF, tr("Closing card log."));
	QF_SAFE_DELETE(f_cardLog);
}
/*
void TheApp::appendCardLogLine(const QString &line)
{
	cardLog() << line << endl;
}
*/
bool TheApp::isScriptDebuggerEnabled()
{
	foreach(QString arg, arguments()) if(arg == "--script-debugger") return true;
	return false;
}
/*
SICliScriptDriver* TheApp::scriptDriver()
{
	if(!f_scriptDriver) {
		f_scriptDriver = new SICliScriptDriver(this);
		QScriptEngine *eng = new QScriptEngine(f_scriptDriver);
		#ifdef SCRIPT_DEBUGGER
		if(theApp()->isScriptDebuggerEnabled()) {
			QScriptEngineDebugger *debugger = new QScriptEngineDebugger(f_scriptDriver);
			debugger->attachTo(eng);
			//qfInfo() << "Script debugger attached.";
		}
		#endif
		f_scriptDriver->setScriptEngine(eng);
	}
	return f_scriptDriver;
}
*/
qf::core::Log::Level TheApp::logLevelFromSettings()
{
	QSettings settings;
	QString level_str = settings.value("app/logging/level").toString().toLower();
	if(level_str == "trash") return qf::core::Log::LOG_TRASH;
	if(level_str == "debug") return qf::core::Log::LOG_DEB;
	if(level_str == "info") return qf::core::Log::LOG_INF;
	if(level_str == "warning") return qf::core::Log::LOG_WARN;
	if(level_str == "error") return qf::core::Log::LOG_ERR;
	return qf::core::Log::LOG_INF;
}

QString TheApp::versionString()
{
	return "1.0.3";
}
