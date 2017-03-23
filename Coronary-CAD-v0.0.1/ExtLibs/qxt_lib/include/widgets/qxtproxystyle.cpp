#include "qxtproxystyle.h"
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

#include <QStyleFactory>

/*!
    \class QxtProxyStyle
    \inmodule QxtWidgets
    \brief The QxtProxyStyle class provides a proxy style.

    A technique called "proxy style" is a common way for creating
    cross-platform custom styles. Developers often want to do slight
    adjustments in some specific values returned by QStyle. A proxy
    style is the solution to avoid subclassing any platform specific
    style (eg. QPlastiqueStyle, QWindowsXPStyle, or QMacStyle) and
    to retain the native look on all supported platforms.

    The subject has been discussed in \l
    {http://doc.trolltech.com/qq/qq09-q-and-a.html#style}
    {Qt Quarterly 9} (just notice that there are a few noteworthy
    spelling mistakes in the article).

    Proxy styles are becoming obsolete thanks to style sheets
    introduced in Qt 4.2. However, style sheets still is a new
    concept and only a portion of features are supported yet. Both -
    style sheets and proxy styles - have their pros and cons.

    \section1 Usage

    Implement the custom behaviour in a subclass of QxtProxyStyle:
    \code
    class MyCustomStyle : public QxtProxyStyle
    {
       public:
          MyCustomStyle(const QString& baseStyle) : QxtProxyStyle(baseStyle)
          {
          }

          int pixelMetric(PixelMetric metric, const QStyleOption* option = 0, const QWidget* widget = 0) const
          {
             if (metric == QStyle::PM_ButtonMargin)
                return 6;
             return QxtProxyStyle::pixelMetric(metric, option, widget);
          }
    };
    \endcode

    Using the custom style for the whole application:
    \code
    QString defaultStyle = QApplication::style()->objectName();
    QApplication::setStyle(new MyCustomStyle(defaultStyle));
    \endcode

    Using the custom style for a single widget:
    \code
    QString defaultStyle = widget->style()->objectName();
    widget->setStyle(new MyCustomStyle(defaultStyle));
    \endcode
 */

/*!
    Constructs a new QxtProxyStyle for \a baseStyle.
    See QStyleFactory::keys() for supported styles.

    \sa QStyleFactory::keys()
 */
QxtProxyStyle::QxtProxyStyle(const QString& baseStyle)
        : QStyle(), style(QStyleFactory::create(baseStyle))
{
    setObjectName(QLatin1String("proxy"));
}

/*!
    Destructs the proxy style.
 */
QxtProxyStyle::~QxtProxyStyle()
{
    delete style;
}

/*!
    \reimp
 */
void QxtProxyStyle::drawComplexControl(ComplexControl control, const QStyleOptionComplex* option, QPainter* painter, const QWidget* widget) const
{
    style->drawComplexControl(control, option, painter, widget);
}

/*!
    \reimp
 */
void QxtProxyStyle::drawControl(ControlElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget)  const
{
    style->drawControl(element, option, painter, widget);
}

/*!
    \reimp
 */
void QxtProxyStyle::drawItemPixmap(QPainter* painter, const QRect& rect, int alignment, const QPixmap& pixmap) const
{
    style->drawItemPixmap(painter, rect, alignment, pixmap);
}

/*!
    \reimp
 */
void QxtProxyStyle::drawItemText(QPainter* painter, const QRect& rect, int alignment, const QPalette& pal, bool enabled, const QString& text, QPalette::ColorRole textRole) const
{
    style->drawItemText(painter, rect, alignment, pal, enabled, text, textRole);
}

/*!
    \reimp
 */
void QxtProxyStyle::drawPrimitive(PrimitiveElement elem, const QStyleOption* option, QPainter* painter, const QWidget* widget) const
{
    style->drawPrimitive(elem, option, painter, widget);
}

/*!
    \reimp
 */
QPixmap QxtProxyStyle::generatedIconPixmap(QIcon::Mode iconMode, const QPixmap& pixmap, const QStyleOption* option) const
{
    return style->generatedIconPixmap(iconMode, pixmap, option);
}

/*!
    \reimp
 */
QStyle::SubControl QxtProxyStyle::hitTestComplexControl(ComplexControl control, const QStyleOptionComplex* option, const QPoint& pos, const QWidget* widget) const
{
    return style->hitTestComplexControl(control, option, pos, widget);
}

/*!
    \reimp
 */
QRect QxtProxyStyle::itemPixmapRect(const QRect& rect, int alignment, const QPixmap& pixmap) const
{
    return style->itemPixmapRect(rect, alignment, pixmap);
}

/*!
    \reimp
 */
QRect QxtProxyStyle::itemTextRect(const QFontMetrics& metrics, const QRect& rect, int alignment, bool enabled, const QString& text) const
{
    return style->itemTextRect(metrics, rect, alignment, enabled, text);
}

/*!
    \reimp
 */
int QxtProxyStyle::pixelMetric(PixelMetric metric, const QStyleOption* option, const QWidget* widget) const
{
    return style->pixelMetric(metric, option, widget);
}

/*!
    \reimp
 */
void QxtProxyStyle::polish(QWidget* widget)
{
    style->polish(widget);
}

/*!
    \reimp
 */
void QxtProxyStyle::polish(QApplication* app)
{
    style->polish(app);
}

/*!
    \reimp
 */
void QxtProxyStyle::polish(QPalette& pal)
{
    style->polish(pal);
}

/*!
    \reimp
 */
QSize QxtProxyStyle::sizeFromContents(ContentsType type, const QStyleOption* option, const QSize& contentsSize, const QWidget* widget) const
{
    return style->sizeFromContents(type, option, contentsSize, widget);
}

/*!
    \reimp
 */
QPalette QxtProxyStyle::standardPalette() const
{
    return style->standardPalette();
}

/*!
    \reimp
 */
QPixmap QxtProxyStyle::standardPixmap(StandardPixmap standardPixmap, const QStyleOption* option, const QWidget* widget) const
{
    return style->standardPixmap(standardPixmap, option, widget);
}

/*!
    \reimp
 */
int QxtProxyStyle::styleHint(StyleHint hint, const QStyleOption* option, const QWidget* widget, QStyleHintReturn* returnData) const
{
    return style->styleHint(hint, option, widget, returnData);
}

/*!
    \reimp
 */
QRect QxtProxyStyle::subControlRect(ComplexControl control, const QStyleOptionComplex* option, SubControl subControl, const QWidget* widget) const
{
    return style->subControlRect(control, option, subControl, widget);
}

/*!
    \reimp
 */
QRect QxtProxyStyle::subElementRect(SubElement element, const QStyleOption* option, const QWidget* widget) const
{
    return style->subElementRect(element, option, widget);
}

/*!
    \reimp
 */
void QxtProxyStyle::unpolish(QWidget* widget)
{
    style->unpolish(widget);
}

/*!
    \reimp
 */
void QxtProxyStyle::unpolish(QApplication* app)
{
    style->unpolish(app);
}

/*!
    \reimp
 */
QIcon QxtProxyStyle::standardIconImplementation(StandardPixmap standardIcon, const QStyleOption* option, const QWidget* widget) const
{
    QIcon result;
    QMetaObject::invokeMethod(style, "standardIconImplementation", Qt::DirectConnection,
                              Q_RETURN_ARG(QIcon, result),
                              Q_ARG(StandardPixmap, standardIcon),
                              Q_ARG(const QStyleOption*, option),
                              Q_ARG(const QWidget*, widget));
    return result;
}
