
/****************************************************************************
** Copyright (c) 2006 - 2011, the LibQxt project.
** See the Qxt AUTHORS file for a list of authors and copyright holders.
** All rights reserved.
**
** Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are met:
**     * Redistributions of source code must retain the above copyright
**       notice, this list of conditions and the following disclaimer.
**     * Redistributions in binary form must reproduce the above copyright
**       notice, this list of conditions and the following disclaimer in the
**       documentation and/or other materials provided with the distribution.
**     * Neither the name of the LibQxt project nor the
**       names of its contributors may be used to endorse or promote products
**       derived from this software without specific prior written permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
** ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
** WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
** DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
** DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
** (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
** LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
** ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
** SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**
** <http://libqxt.org>  <foundation@libqxt.org>
*****************************************************************************/

#include "qxtlanguagecombobox.h"
#include "qxtlanguagecombobox_p.h"
#include <QDir>
#include <QApplication>
#include <QSet>

class Language;
typedef QList<Language> LanguageList;


static QStringList findQmFiles(const QString& pathToTranslations)
{
    QDir dir(pathToTranslations);
    QStringList fileNames = dir.entryList(QStringList("*.qm"), QDir::Files, QDir::Name);
    QSet<QString> found;
    QMutableStringListIterator i(fileNames);
    while (i.hasNext())
    {
        i.next();
        int start = i.value().indexOf('_');
        int end = i.value().lastIndexOf('.');
        QString s = i.value().mid(start + 1, end - start - 1).toLower();
        if (found.contains(s))
        {
            i.setValue(s);
            found.insert(s);
        }
    }

    return fileNames;
}


class Language
{
public:
    Language(const QLocale::Language& language)
            : _mLanguage(language)
            , _mCountryCode("C")
    {
        QLocale loc(language);
        if (loc.language() == language)
            _mCountryCode = loc.name().right(2);
        else
            _mCountryCode = "";

        _mDisplayName = qApp->translate("QLocale", qPrintable(QLocale::languageToString(_mLanguage)));
    };

    bool operator<(const Language& lang) const
    {
        return _mDisplayName.localeAwareCompare(lang._mDisplayName) < 0;
    }

    static const LanguageList& getAllLanguages()
    {
        if (_smAllLanguages.empty())
        {

            for (int idx = 0; idx < QLocale::LastLanguage; ++idx)
            {
                QLocale::Language l = static_cast<QLocale::Language>(idx);
                if (l == QLocale::LastLanguage)
                    continue;
                // obsolete - NorwegianNynorsk is used instead
                if (l == QLocale::NorwegianNynorsk)
                    continue;
                if (l == QLocale::C)
                    continue;

                _smAllLanguages.push_back(Language(l));
            }

            qSort(_smAllLanguages);
        }
        return _smAllLanguages;
    };

    static LanguageList getTrLanguages(const QString& translationPath)
    {
        LanguageList trLanguages;

        QStringList qms = findQmFiles(translationPath);
        for (int i = 0; i < qms.size(); ++i)
        {
            QLocale locale(qms[i]);
            if (locale.language() == QLocale::C)
                continue;
            trLanguages.push_back(Language(locale.language()));
        }
        qSort(trLanguages);
        return trLanguages;
    };

    const QString& name() const
    {
        return _mDisplayName;
    }
    const QLocale::Language& language() const
    {
        return _mLanguage;
    }
    const QString& countryName() const
    {
        return _mCountryCode;
    }

private:
    QLocale::Language _mLanguage;
    QString _mDisplayName;
    QString _mCountryCode;

    static LanguageList _smAllLanguages;
};

LanguageList Language::_smAllLanguages;

class LanguageModel : public QAbstractTableModel
{
public:
    LanguageModel(const LanguageList& languages, QObject* parent = 0)
            : QAbstractTableModel(parent), _mLanguages(languages)
    {
    }

    int rowCount(const QModelIndex&) const
    {
        return _mLanguages.size();
    }

    int columnCount(const QModelIndex&) const
    {
        return 2;
    }

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const
    {
        if (!index.isValid())
            return QVariant();
        if (_mLanguages.empty())
            return QVariant();

        int idx = qMin(index.row(), _mLanguages.size());
        const Language& l = _mLanguages[idx];
        if (role == Qt::DecorationRole)
        {
            return QIcon(":/flags/" + l.countryName() + ".png");
        }

        if (role == Qt::DisplayRole)
        {
            switch (index.column())
            {
            case 0:
                return l.name();
            case 1:
                return l.language();
            default:
                return QVariant();
            }
        }
        return QVariant();
    }
private:
    LanguageList _mLanguages;
};

QxtLanguageComboBoxPrivate::QxtLanguageComboBoxPrivate()
        : _mDisplayMode(QxtLanguageComboBox::AllLanguages), _mTranslationPath("."), _mModel(0)
{
}

void QxtLanguageComboBoxPrivate::init()
{
    connect(&qxt_p(), SIGNAL(currentIndexChanged(int)), this, SLOT(comboBoxCurrentIndexChanged(int)));
}

void QxtLanguageComboBoxPrivate::setTranslationPath(const QString& path)
{
    if (_mTranslationPath == path)
        return;

    _mTranslationPath = path;
    reset();
}

void QxtLanguageComboBoxPrivate::setDisplayMode(QxtLanguageComboBox::DisplayMode mode)
{
    if (_mDisplayMode == mode && _mModel != 0)
        return;

    _mDisplayMode = mode;
    reset();
}

void QxtLanguageComboBoxPrivate::reset()
{
    if (_mModel != 0)
    {
        delete _mModel;
        _mModel = 0;
    }

    QLocale::Language currentLang = currentLanguage();
    if (_mDisplayMode == QxtLanguageComboBox::AllLanguages)
        _mModel = new LanguageModel(Language::getAllLanguages(), &qxt_p());
    else
        _mModel = new LanguageModel(Language::getTrLanguages(_mTranslationPath), &qxt_p());

    qxt_p().setModel(_mModel);
    qxt_p().setModelColumn(0);

    setCurrentLanguage(currentLang);
}

void QxtLanguageComboBoxPrivate::comboBoxCurrentIndexChanged(int)
{
    handleLanguageChange();
}

void QxtLanguageComboBoxPrivate::handleLanguageChange()
{
    emit qxt_p().currentLanguageChanged(currentLanguage());
    emit qxt_p().currentLanguageNameChanged(currentLanguageName());
}

QLocale::Language QxtLanguageComboBoxPrivate::currentLanguage() const
{
    if (_mModel == NULL)
        return QLocale::C;

    QModelIndex idx = _mModel->index(qxt_p().currentIndex(), 1);
    QLocale::Language currentLang = static_cast<QLocale::Language>(idx.data().toInt());
    return currentLang;
}

QString QxtLanguageComboBoxPrivate::currentLanguageName() const
{
    return qxt_p().currentText();
}

void QxtLanguageComboBoxPrivate::setCurrentLanguage(QLocale::Language language)
{
    // column 1 is QLocale::Language
    QModelIndex start = _mModel->index(0, 1);
    QModelIndexList result = _mModel->match(start, Qt::DisplayRole, language, 1, Qt::MatchExactly);
    if (!result.isEmpty())
    {
        qxt_p().setCurrentIndex(result.first().row());
        handleLanguageChange();
    }
//   else
//     qDebug() << "Cannot setCurrentLanguage: " << language << _mModel;
}

/*!
    \class QxtLanguageComboBox
    \inmodule QxtWidgets
    \brief The QxtLanguageComboBox widget is an extended QComboBox to display languages.

    QxtLanguageComboBox is a specialized combo box to display spoken languages.
    The languages are taken from QLocale::Language.

    \image qxtlanguagecombobox.png "QxtLanguageComboBox in Plastique style."
 */

/*!
    \enum QxtLanguageComboBox::DisplayMode

    This enum describes the displayed languages.

    \value AllLanguages          Show all languages.
    \value AvailableTranslations Show translated languages.

    \sa QxtLanguageComboBox::displayMode
 */

/*!
    \fn QxtLanguageComboBox::currentLanguageChanged(QLocale::Language language)

    This signal is emitted whenever the current selected \a language has been changed.
 */

/*!
    \fn QxtLanguageComboBox::currentLanguageNameChanged(const QString& name)

    This signal is emitted whenever the of the current selected language with \a name has been changed.
 */


/*!
    Constructs a new QxtCountryComboBox with \a parent.
 */
QxtLanguageComboBox::QxtLanguageComboBox(QWidget* parent)
        : QComboBox(parent)
{
    QXT_INIT_PRIVATE(QxtLanguageComboBox);
    setDisplayMode(AllLanguages);
    setCurrentLanguage(QLocale::system().language());
    qxt_d().init();
}

/*!
    Destructs the combo box.
 */
QxtLanguageComboBox::~QxtLanguageComboBox()
{
}

/*!
    \property QxtLanguageComboBox::currentLanguage
    \brief the current selected language.
 */
QLocale::Language QxtLanguageComboBox::currentLanguage() const
{
    return qxt_d().currentLanguage();
}

/*!
    \property QxtLanguageComboBox::currentLanguageName
    \brief the name of the current selected language.
 */
QString QxtLanguageComboBox::currentLanguageName() const
{
    return qxt_d().currentLanguageName();
}

void QxtLanguageComboBox::setCurrentLanguage(QLocale::Language language)
{
    qxt_d().setCurrentLanguage(language);
}

/*!
    \property QxtLanguageComboBox::displayMode
    \brief the display mode of the widget.
 */
void QxtLanguageComboBox::setDisplayMode(DisplayMode mode)
{
    qxt_d().setDisplayMode(mode);
}

QxtLanguageComboBox::DisplayMode QxtLanguageComboBox::displayMode() const
{
    return qxt_d().displayMode();
}

/*!
    \property QxtLanguageComboBox::translationPath
    \brief the path to the translation files.
 */
void QxtLanguageComboBox::setTranslationPath(const QString& path)
{
    qxt_d().setTranslationPath(path);
}

QString QxtLanguageComboBox::translationPath() const
{
    return qxt_d().translationPath();
}
