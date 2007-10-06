#include <QtCore>
#include "pp.h"
#include "parser.h"
#include "control.h"
#include "binder.h"

static bool preprocess(const QString &sourceFile, QByteArray *out)
{
    rpp::pp_environment env;
    rpp::pp preprocess(env);

    rpp::pp_null_output_iterator null_out;

    const char *ppconfig = ":/trolltech/generator/pp-qt-configuration";
    QFile configFile(ppconfig);
    if (!configFile.open(QFile::ReadOnly)) {
        fprintf(stderr, "Preprocessor configuration file not found '%s'\n", ppconfig);
        return false;
    }

    QByteArray ba = configFile.readAll();
    configFile.close();
    preprocess.operator() (ba.constData(), ba.constData() + ba.size(), null_out);

    QString qtdir = getenv ("QTDIR");
    if (qtdir.isEmpty()) {
        fprintf(stderr, "Generator requires QTDIR to be set\n");
        return false;
    }

    qtdir += "/include";

    QString currentDir = QDir::current().absolutePath();
    QFileInfo sourceInfo(sourceFile);
    QDir::setCurrent(sourceInfo.absolutePath());

    preprocess.push_include_path(".");
    preprocess.push_include_path(QDir::convertSeparators(qtdir).toStdString());
    preprocess.push_include_path(QDir::convertSeparators(qtdir + "/QtXml").toStdString());
    preprocess.push_include_path(QDir::convertSeparators(qtdir + "/QtNetwork").toStdString());
    preprocess.push_include_path(QDir::convertSeparators(qtdir + "/QtCore").toStdString());
    preprocess.push_include_path(QDir::convertSeparators(qtdir + "/QtGui").toStdString());
    preprocess.push_include_path(QDir::convertSeparators(qtdir + "/QtOpenGL").toStdString());

    std::string result;
    result.reserve (20 * 1024); // 20K

    result += "# 1 \"builtins\"\n";
    result += "# 1 \"";
    result += sourceFile.toStdString();
    result += "\"\n";

    preprocess.file (sourceInfo.fileName().toStdString(),
                     rpp::pp_output_iterator<std::string> (result));

    *out = QString::fromStdString(result).toUtf8();
    return true;
}

QString convertAccessPolicy(CodeModel::AccessPolicy policy) {
    switch(policy) {
        case CodeModel::Public: return "public";
        case CodeModel::Private: return "private";
        case CodeModel::Protected: return "protected";
    }
    return QString();
}

QString makeFunction(FunctionModelItem function, const QString preFix = QString()) {
    QString fullName;
    fullName += function->type().toString();
    fullName += " ";
    fullName += preFix;
    fullName += function->name();
    ArgumentList arguments = function->arguments();
    QStringList args;
    for (int i = 0; i < arguments.count(); ++i) {
        QString theargs = arguments[i]->type().toString() + " " + arguments[i]->name();
        if (arguments[i]->defaultValue())
            theargs += " = " + arguments[i]->defaultValueExpression();
        args += theargs;
    }
    fullName += "(" + args.join(", ") + ")";
    if (function->isConstant())
        fullName +=  " const";
    return fullName;
}

QStringList doneClasses;

void outputFile(const QString &fileName, bool onlyPreprocess)
{
    QByteArray contents;
    if (!preprocess(fileName, &contents)) {
        QTextStream error(stderr);
        error << "error preprocessing" << fileName;
        return;
    }

    QTextStream out(stdout);
    if (onlyPreprocess) {
        out << contents;
        return;
    }

    Control control;
    Parser p(&control);
    pool __pool;

    TranslationUnitAST *ast = p.parse(contents, contents.size(), &__pool);

    CodeModel model;
    Binder binder(&model, p.location());
    FileModelItem dom = binder.run(ast);

    int i = 0;
    for (i = 0; i < dom->classes().count(); ++i) {
        QString name = dom->classes().at(i)->name();
        if (doneClasses.contains(name))
            continue;
        out << dom->classes().at(i)->name() << endl;
        doneClasses.append(name);
        FunctionList functions = dom->classes().at(i)->functions();
        for (int j = functions.count() - 1; j >= 0; --j) {
            if (functions.at(j)->accessPolicy() != CodeModel::Private)
                out << "    " << makeFunction(functions.at(j)) << endl;
        }
    }
}

int main(int argc, char **argv)
{
    QString qtdir = getenv ("QTDIR");
    if (qtdir.isEmpty()) {
        fprintf(stderr, "Generator requires QTDIR to be set\n");
        return 1;
    }

    if (argv[1][0] == '-' && argv[1][1] == 'E')
        outputFile(argv[2], true);
    else
        outputFile(argv[1], false);

    return 0;
}

