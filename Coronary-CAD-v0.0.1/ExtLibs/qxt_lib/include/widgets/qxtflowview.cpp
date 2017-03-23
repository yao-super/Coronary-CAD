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
**
** This is a derived work of PictureFlow (http://pictureflow.googlecode.com)
**
** The original code was distributed under the following terms:
**
** Copyright (C) 2008 Ariya Hidayat (ariya@kde.org)
** Copyright (C) 2007 Ariya Hidayat (ariya@kde.org)
**
** Permission is hereby granted, free of charge, to any person obtaining a copy
** of this software and associated documentation files (the "Software"), to deal
** in the Software without restriction, including without limitation the rights
** to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
** copies of the Software, and to permit persons to whom the Software is
** furnished to do so, subject to the following conditions:
**
** The above copyright notice and this permission notice shall be included in
** all copies or substantial portions of the Software.
**
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
** FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
** AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
** LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
** OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
** THE SOFTWARE.
****************************************************************************/

#include "qxtflowview_p.h"
#include <QWheelEvent>

/*!
    \class QxtFlowView
    \inmodule QxtWidgets
    \brief The QxtFlowView widget is an item view for images with impressive flow effects

    A widget for showin images with animation effects, like Apple's Cover
    Flow (in iTunes and iPod). Images are arranged in form of slides, one
    main slide is shown at the center with few slides on the left and right
    sides of the center slide. When the next or previous slide is brought
    to the front, the whole slides flow to the left or to the right with
    smooth animation effect; until the new slide is finally placed at the
    center.

    \image qxtflowview.png "QxtFlowView in action."

    This is a derived work of \l{http://pictureflow.googlecode.com}{PictureFlow}
 */

/*!
    \enum QxtFlowView::ReflectionEffect
    \brief This enum describes available reflection effects.

    \value NoReflection No reflection
    \value PlainReflection Plain reflection
    \value BlurredReflection Blurred reflection
 */

/*!
    \fn QxtFlowView::currentIndexChanged(QModelIndex index)

    This signal is emitted whenever the current \a index has changed.
 */

/*!
    Constructs a new QxtFlowView with \a parent.
 */
QxtFlowView::QxtFlowView(QWidget* parent): QWidget(parent)
{
    d = new QxtFlowViewPrivate;

    d->model = 0;
    d->picrole = Qt::DecorationRole;
    d->textrole = Qt::DisplayRole;
    d->piccolumn = 0;
    d->textcolumn = 0;



    d->state = new QxtFlowViewState;
    d->state->reset();
    d->state->reposition();

    d->renderer = new QxtFlowViewSoftwareRenderer;
    d->renderer->state = d->state;
    d->renderer->widget = this;
    d->renderer->init();

    d->animator = new QxtFlowViewAnimator;
    d->animator->state = d->state;
    QObject::connect(&d->animator->animateTimer, SIGNAL(timeout()), this, SLOT(updateAnimation()));

    QObject::connect(&d->triggerTimer, SIGNAL(timeout()), this, SLOT(render()));

    setAttribute(Qt::WA_StaticContents, true);
    setAttribute(Qt::WA_OpaquePaintEvent, true);
    setAttribute(Qt::WA_NoSystemBackground, true);
}

/*!
    Destructs the flow view.
 */
QxtFlowView::~QxtFlowView()
{
    delete d->renderer;
    delete d->animator;
    delete d->state;
    delete d;
}

/*!
    Sets the \a model.

    \bold {Note:} The view does not take ownership of the model unless it is the
    model's parent object because it may be shared between many different views.
 */
void QxtFlowView::setModel(QAbstractItemModel * model)
{
    d->setModel(model);
}

/*!
    Returns the model.
 */
QAbstractItemModel * QxtFlowView::model()
{
    return d->model;
}

/*!
    \property QxtFlowView::backgroundColor
    \brief the background color

    The default value is black.
 */
QColor QxtFlowView::backgroundColor() const
{
    return QColor(d->state->backgroundColor);
}

void QxtFlowView::setBackgroundColor(const QColor& c)
{
    d->state->backgroundColor = c.rgb();
    triggerRender();
}

/*!
    \property QxtFlowView::slideSize
    \brief the slide size

    The slide dimensions are in pixels.

    The default value is 150x200.
 */
QSize QxtFlowView::slideSize() const
{
    return QSize(d->state->slideWidth, d->state->slideHeight);
}

void QxtFlowView::setSlideSize(QSize size)
{
    d->state->slideWidth = size.width();
    d->state->slideHeight = size.height();
    d->state->reposition();
    triggerRender();
}

/*!
    \property QxtFlowView::reflectionEffect
    \brief the reflection effect

    The default value is PlainReflection.
 */
QxtFlowView::ReflectionEffect QxtFlowView::reflectionEffect() const
{
    return d->state->reflectionEffect;
}

void QxtFlowView::setReflectionEffect(ReflectionEffect effect)
{
    d->state->reflectionEffect = effect;
    d->reset();
}

/*!
    \property QxtFlowView::pictureRole
    \brief the picture role

    The default value is Qt::DecorationRole.
*/
int QxtFlowView::pictureRole()
{
    return d->picrole;
}

void QxtFlowView::setPictureRole(int a)
{
    d->picrole = a;
    d->reset();
}

/*!
    \property QxtFlowView::pictureColumn
    \brief the picture column

    The default value is \c 0.
 */
int QxtFlowView::pictureColumn()
{
    return d->piccolumn;
}

void QxtFlowView::setPictureColumn(int a)
{
    d->piccolumn = a;
    d->reset();
}

#if 0
int QxtFlowView::textRole()
{
    return d->textrole;
}

void QxtFlowView::setTextRole(int a)
{
    d->textrole = a;
    d->reset();
}

int QxtFlowView::textColumn()
{
    return d->textcolumn;
}

void QxtFlowView::setTextColumn(int a)
{
    d->textcolumn = a;
    d->reset();
}
#endif

/*!
    \property QxtFlowView::rootIndex
    \brief the root index

    The root index is the parent index to the view's toplevel items. The root can be invalid.
 */
QModelIndex QxtFlowView::rootIndex() const
{
    return d->rootindex;
}

void QxtFlowView::setRootIndex(QModelIndex index)
{
    d->rootindex = index;
}

/*!
    \property QxtFlowView::currentIndex
    \brief the current index

    The slide of the current index is shown in the middle of the viewport.

    \bold {Note:} No animation effect will be produced.
    \sa showSlide()
 */
QModelIndex QxtFlowView::currentIndex() const
{
    if (!d->model)
        return QModelIndex();
    return d->currentcenter;
}

void QxtFlowView::setCurrentIndex(QModelIndex index)
{
    d->setCurrentIndex(index);
}

/*! Rerender the widget. Normally this function will be automatically invoked whenever necessary, e.g. during the transition animation. */
void QxtFlowView::render()
{
    d->renderer->dirty = true;
    update();
}

/*! Schedules a rendering update. Unlike render(), this function does not cause immediate rendering.*/
void QxtFlowView::triggerRender()
{
    d->triggerRender();
}

/*! Shows previous slide using animation effect. */
void QxtFlowView::showPrevious()
{
    int step = d->animator->step;
    int center = d->state->centerIndex;

    if (step > 0)
        d->animator->start(center);

    if (step == 0)
        if (center > 0)
            d->animator->start(center - 1);

    if (step < 0)
        d->animator->target = qMax(0, center - 2);
}

/*! Shows next slide using animation effect. */
void QxtFlowView::showNext()
{
    int step = d->animator->step;
    int center = d->state->centerIndex;

    if (step < 0)
        d->animator->start(center);

    if (step == 0)
        if (center < d->state->slideImages.count() - 1)
            d->animator->start(center + 1);

    if (step > 0)
        d->animator->target = qMin(center + 2, d->state->slideImages.count() - 1);
}

/*! Go to specified slide at \a index using animation effect. */
void QxtFlowView::showSlide(QModelIndex index)
{
    int r = d->modelmap.indexOf(index);
    if (r < 0)
        return;

    d->showSlide(r);
}

/*! \reimp */
void QxtFlowView::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Left)
    {
        if (event->modifiers() == Qt::ControlModifier)
            d->showSlide(currentIndex().row() - 10);
        else
            showPrevious();
        event->accept();
        return;
    }

    if (event->key() == Qt::Key_Right)
    {
        if (event->modifiers() == Qt::ControlModifier)
            d->showSlide(currentIndex().row() + 10);
        else
            showNext();
        event->accept();
        return;
    }

    event->ignore();
}

/*! \reimp */
void QxtFlowView::mousePressEvent(QMouseEvent* event)
{
    d->lastgrabpos = event->pos();
}

/*! \reimp */
void QxtFlowView::mouseMoveEvent(QMouseEvent * event)
{
    int i = (event->pos() - d->lastgrabpos).x() / (d->state->slideWidth / 4);
    if (i > 0)
    {
        showPrevious();
        d->lastgrabpos = event->pos();
    }
    if (i < 0)
    {
        showNext();
        d->lastgrabpos = event->pos();
    }

}

/*! \reimp */
void QxtFlowView::mouseReleaseEvent(QMouseEvent* event)
{
    Q_UNUSED(event);

}

/*! \reimp */
void QxtFlowView::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    d->renderer->paint();
}

/*! \reimp */
void QxtFlowView::resizeEvent(QResizeEvent* event)
{
    triggerRender();
    QWidget::resizeEvent(event);
}

/*! \reimp */
void QxtFlowView::wheelEvent(QWheelEvent * event)
{

    if (event->orientation() == Qt::Horizontal)
    {
        event->ignore();
    }
    else
    {
        int numSteps = -((event->delta() / 8) / 15);



        if (numSteps > 0)
        {
            for (int i = 0;i < numSteps;i++)
            {
                showNext();
            }
        }
        else
        {
            for (int i = numSteps;i < 0;i++)
            {
                showPrevious();
            }
        }
        event->accept();
    }


}

/*! \internal */
void QxtFlowView::updateAnimation()
{
    int old_center = d->state->centerIndex;
    d->animator->update();
    triggerRender();
    if (d->state->centerIndex != old_center)
    {
        d->currentcenter = d->modelmap.at(d->state->centerIndex);
        emit currentIndexChanged(d->currentcenter);
    }
}









void QxtFlowViewPrivate::columnsAboutToBeInserted(const QModelIndex & parent, int start, int end)
{
    Q_UNUSED(parent);
    Q_UNUSED(start);
    Q_UNUSED(end);

}

void QxtFlowViewPrivate::columnsAboutToBeRemoved(const QModelIndex & parent, int start, int end)
{
    Q_UNUSED(parent);
    Q_UNUSED(start);
    Q_UNUSED(end);

}

void QxtFlowViewPrivate::columnsInserted(const QModelIndex & parent, int start, int end)
{
    Q_UNUSED(parent);
    Q_UNUSED(start);
    Q_UNUSED(end);

}

void QxtFlowViewPrivate::columnsRemoved(const QModelIndex & parent, int start, int end)
{
    Q_UNUSED(parent);
    Q_UNUSED(start);
    Q_UNUSED(end);
}

void QxtFlowViewPrivate::dataChanged(const QModelIndex & topLeft, const QModelIndex & bottomRight)
{
    Q_UNUSED(topLeft);
    Q_UNUSED(bottomRight);

    if (topLeft.parent() != rootindex)
        return;

    if (bottomRight.parent() != rootindex)
        return;


    int start = topLeft.row();
    int end = bottomRight.row();

    for (int i = start;i <= end;i++)
        replaceSlide(i, qvariant_cast<QImage>(model->data(model->index(i, piccolumn, rootindex), picrole)));
}

void QxtFlowViewPrivate::headerDataChanged(Qt::Orientation orientation, int first, int last)
{
    Q_UNUSED(orientation);
    Q_UNUSED(first);
    Q_UNUSED(last);





}

void QxtFlowViewPrivate::layoutAboutToBeChanged()
{

}

void QxtFlowViewPrivate::layoutChanged()
{
    reset();
    setCurrentIndex(currentcenter);
}

void QxtFlowViewPrivate::modelAboutToBeReset()
{
}

void QxtFlowViewPrivate::modelReset()
{
    reset();
}

void QxtFlowViewPrivate::rowsAboutToBeInserted(const QModelIndex & parent, int start, int end)
{
    Q_UNUSED(parent);
    Q_UNUSED(start);
    Q_UNUSED(end);
}

void QxtFlowViewPrivate::rowsAboutToBeRemoved(const QModelIndex & parent, int start, int end)
{
    Q_UNUSED(parent);
    Q_UNUSED(start);
    Q_UNUSED(end);
}

void QxtFlowViewPrivate::rowsInserted(const QModelIndex & parent, int start, int end)
{
    if (rootindex != parent)
        return;
    for (int i = start;i <= end;i++)
    {
        QModelIndex idx = model->index(i, piccolumn, rootindex);
        insertSlide(i, qvariant_cast<QImage>(model->data(idx, picrole)));
        modelmap.insert(i, idx);
    }
}

void QxtFlowViewPrivate::rowsRemoved(const QModelIndex & parent, int start, int end)
{
    if (rootindex != parent)
        return;
    for (int i = start;i <= end;i++)
    {
        removeSlide(i);
        modelmap.removeAt(i);
    }

}

void QxtFlowViewPrivate::setModel(QAbstractItemModel * m)
{


    if (model)
    {

        disconnect(this->model, SIGNAL(columnsAboutToBeInserted(const QModelIndex & , int , int)),
                   this, SLOT(columnsAboutToBeInserted(const QModelIndex & , int , int)));
        disconnect(this->model, SIGNAL(columnsAboutToBeRemoved(const QModelIndex & , int , int)),
                   this, SLOT(columnsAboutToBeRemoved(const QModelIndex & , int , int)));
        disconnect(this->model, SIGNAL(columnsInserted(const QModelIndex & , int , int)),
                   this, SLOT(columnsInserted(const QModelIndex & , int , int)));
        disconnect(this->model, SIGNAL(columnsRemoved(const QModelIndex & , int , int)),
                   this, SLOT(columnsRemoved(const QModelIndex & , int , int)));
        disconnect(this->model, SIGNAL(dataChanged(const QModelIndex & , const QModelIndex &)),
                   this, SLOT(dataChanged(const QModelIndex & , const QModelIndex &)));
        disconnect(this->model, SIGNAL(headerDataChanged(Qt::Orientation , int , int)),
                   this, SLOT(headerDataChanged(Qt::Orientation , int , int)));
        disconnect(this->model, SIGNAL(layoutAboutToBeChanged()),
                   this, SLOT(layoutAboutToBeChanged()));
        disconnect(this->model, SIGNAL(layoutChanged()),
                   this, SLOT(layoutChanged()));
        disconnect(this->model, SIGNAL(modelAboutToBeReset()),
                   this, SLOT(modelAboutToBeReset()));
        disconnect(this->model, SIGNAL(modelReset()),
                   this, SLOT(modelReset()));
        disconnect(this->model, SIGNAL(rowsAboutToBeInserted(const QModelIndex & , int , int)),
                   this, SLOT(rowsAboutToBeInserted(const QModelIndex & , int , int)));
        disconnect(this->model, SIGNAL(rowsAboutToBeRemoved(const QModelIndex & , int , int)),
                   this, SLOT(rowsAboutToBeRemoved(const QModelIndex & , int , int)));
        disconnect(this->model, SIGNAL(rowsInserted(const QModelIndex & , int , int)),
                   this, SLOT(rowsInserted(const QModelIndex & , int , int)));
        disconnect(this->model, SIGNAL(rowsRemoved(const QModelIndex & , int , int)),
                   this, SLOT(rowsRemoved(const QModelIndex & , int , int)));
    }
    model = m;
    if (model)
    {
        rootindex = model->parent(QModelIndex());

        connect(this->model, SIGNAL(columnsAboutToBeInserted(const QModelIndex & , int , int)),
                this, SLOT(columnsAboutToBeInserted(const QModelIndex & , int , int)));
        connect(this->model, SIGNAL(columnsAboutToBeRemoved(const QModelIndex & , int , int)),
                this, SLOT(columnsAboutToBeRemoved(const QModelIndex & , int , int)));
        connect(this->model, SIGNAL(columnsInserted(const QModelIndex & , int , int)),
                this, SLOT(columnsInserted(const QModelIndex & , int , int)));
        connect(this->model, SIGNAL(columnsRemoved(const QModelIndex & , int , int)),
                this, SLOT(columnsRemoved(const QModelIndex & , int , int)));
        connect(this->model, SIGNAL(dataChanged(const QModelIndex & , const QModelIndex &)),
                this, SLOT(dataChanged(const QModelIndex & , const QModelIndex &)));
        connect(this->model, SIGNAL(headerDataChanged(Qt::Orientation , int , int)),
                this, SLOT(headerDataChanged(Qt::Orientation , int , int)));
        connect(this->model, SIGNAL(layoutAboutToBeChanged()),
                this, SLOT(layoutAboutToBeChanged()));
        connect(this->model, SIGNAL(layoutChanged()),
                this, SLOT(layoutChanged()));
        connect(this->model, SIGNAL(modelAboutToBeReset()),
                this, SLOT(modelAboutToBeReset()));
        connect(this->model, SIGNAL(modelReset()),
                this, SLOT(modelReset()));
        connect(this->model, SIGNAL(rowsAboutToBeInserted(const QModelIndex & , int , int)),
                this, SLOT(rowsAboutToBeInserted(const QModelIndex & , int , int)));
        connect(this->model, SIGNAL(rowsAboutToBeRemoved(const QModelIndex & , int , int)),
                this, SLOT(rowsAboutToBeRemoved(const QModelIndex & , int , int)));
        connect(this->model, SIGNAL(rowsInserted(const QModelIndex & , int , int)),
                this, SLOT(rowsInserted(const QModelIndex & , int , int)));
        connect(this->model, SIGNAL(rowsRemoved(const QModelIndex & , int , int)),
                this, SLOT(rowsRemoved(const QModelIndex & , int , int)));
    }

    reset();
}


void QxtFlowViewPrivate::clear()
{
    int c = state->slideImages.count();
    for (int i = 0; i < c; i++)
        delete state->slideImages[i];
    state->slideImages.resize(0);

    state->reset();
    modelmap.clear();
    triggerRender();
}


void QxtFlowViewPrivate::triggerRender()
{
    triggerTimer.setSingleShot(true);
    triggerTimer.start(0);
}



void QxtFlowViewPrivate::insertSlide(int index, const QImage& image)
{
    state->slideImages.insert(index, new QImage(image));
    triggerRender();
}

void QxtFlowViewPrivate::replaceSlide(int index, const QImage& image)
{
    Q_ASSERT((index >= 0) && (index < state->slideImages.count()));

    QImage* i = image.isNull() ? 0 : new QImage(image);
    delete state->slideImages[index];
    state->slideImages[index] = i;
    triggerRender();
}

void QxtFlowViewPrivate::removeSlide(int index)
{
    delete state->slideImages[index];
    state->slideImages.remove(index);
    triggerRender();
}


void QxtFlowViewPrivate::showSlide(int index)
{
    if (index == state->centerSlide.slideIndex)
        return;
    animator->start(index);
}



void QxtFlowViewPrivate::reset()
{
    clear();
    if (model)
    {
        for (int i = 0;i < model->rowCount(rootindex);i++)
        {
            QModelIndex idx = model->index(i, piccolumn, rootindex);
            insertSlide(i, qvariant_cast<QImage>(model->data(idx, picrole)));
            modelmap.insert(i, idx);
        }
        if(modelmap.count())
            currentcenter=modelmap.at(0);
        else
            currentcenter=QModelIndex();
    }
    triggerRender();
}



void QxtFlowViewPrivate::setCurrentIndex(QModelIndex index)
{
    if (model->parent(index) != rootindex)
        return;

    int r = modelmap.indexOf(index);
    if (r < 0)
        return;

    state->centerIndex = r;
    state->reset();
    animator->stop(r);
    triggerRender();
}
