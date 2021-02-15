#include "AppComponent.h"

#include <QFile>
#include <QDir>
#include <QJsonDocument>
#include <QTextStream>

Session* AppComponent::session = nullptr;
QList<Dut*> AppComponent::dutList;
TestSequenceManager* AppComponent::testSequenceManager = nullptr;
Logger* AppComponent::logger = nullptr;

//QString AppComponent::workDirectory = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation); // For release version
//QString AppComponent::workDirectory = "./"; //For test version
QString AppComponent::workDirectory = "../.."; //For development

QSettings* AppComponent::settings = nullptr;
QJSEngine* AppComponent::scriptEngine = nullptr;

AppComponent::AppComponent()
{
    if(!session)
    {
        session = new Session;
    }

    if(!testSequenceManager)
    {
        testSequenceManager = new TestSequenceManager;
    }

    if(!logger)
    {
        logger = new Logger;
    }

    if(!settings)
    {
        settings = new QSettings(workDirectory + "/settings.ini", QSettings::IniFormat);
        settings->setValue("workDirectory", QDir(workDirectory).absolutePath()); //Make name of work directory avaliable for other classes that use settings
    }

    if(!scriptEngine)
    {
        scriptEngine = new QJSEngine;
        scriptEngine->installExtensions(QJSEngine::ConsoleExtension);

        scriptEngine->globalObject().setProperty("testSequenceManager", scriptEngine->newQObject(testSequenceManager));

        evaluateScriptFromFile(workDirectory + "/init.js");
        evaluateScriptsFromDirectory(workDirectory + "/sequences");

        scriptEngine->globalObject().setProperty("session", scriptEngine->newQObject(session));
        scriptEngine->globalObject().setProperty("logger", scriptEngine->newQObject(logger));
    }

    if(dutList.isEmpty())
    {
        dutList.push_back(new Dut(1, 1, 1));
        dutList.push_back(new Dut(2, 1, 2));
        dutList.push_back(new Dut(3, 1, 3));

        dutList.push_back(new Dut(4, 2, 1));
        dutList.push_back(new Dut(5, 2, 2));
        dutList.push_back(new Dut(6, 2, 3));

        dutList.push_back(new Dut(7, 3, 1));
        dutList.push_back(new Dut(8, 3, 2));
        dutList.push_back(new Dut(9, 3, 3));

        dutList.push_back(new Dut(10, 4, 1));
        dutList.push_back(new Dut(11, 4, 2));
        dutList.push_back(new Dut(12, 4, 3));

        dutList.push_back(new Dut(13, 5, 1));
        dutList.push_back(new Dut(14, 5, 2));
        dutList.push_back(new Dut(15, 5, 3));

        int counter = 0;
        for(auto & dut : dutList)
        {
            QJSValue currentDut = scriptEngine->newQObject(dut);
            scriptEngine->globalObject().property("dutList").setProperty(counter, currentDut);
            counter++;
        }
    }
}

AppComponent::~AppComponent()
{
    if(session)
    {
        delete session;
        session = nullptr;
    }

    if(testSequenceManager)
    {
        delete testSequenceManager;
        testSequenceManager = nullptr;
    }

    if(logger)
    {
        delete logger;
        logger = nullptr;
    }

    if(settings)
    {
        delete settings;
        settings = nullptr;
    }

    if(!dutList.isEmpty())
    {
        for(auto & dut : dutList)
        {
            delete dut;
        }

        dutList.clear();
    }

    if(scriptEngine)
    {
        delete scriptEngine;
        scriptEngine = nullptr;
    }
}

QJSValue AppComponent::evaluateScriptFromFile(const QString &scriptFileName)
{
    QFile scriptFile(scriptFileName);
    scriptFile.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream in(&scriptFile);
    in.setCodec("Utf-8");
    QJSValue scriptResult = scriptEngine->evaluate(QString(in.readAll()));
    scriptFile.close();
    return scriptResult;
}

QList<QJSValue> AppComponent::evaluateScriptsFromDirectory(const QString& directoryName)
{
    QDir scriptsDir = QDir(directoryName, "*.js", QDir::Name, QDir::Files);
    QStringList fileNames = scriptsDir.entryList();
    QList<QJSValue> results;

    for (auto & i : fileNames)
    {
        results.push_back(evaluateScriptFromFile(scriptsDir.absoluteFilePath(i)));
    }

    return results;
}

QJSValue AppComponent::runScript(const QString& scriptName, const QJSValueList& args)
{
    return scriptEngine->globalObject().property(scriptName).call(args);
}
