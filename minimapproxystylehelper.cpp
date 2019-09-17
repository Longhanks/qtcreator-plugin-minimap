#include "minimapproxystylehelper.h"

#include "constants.h"
#include "settings.h"

#include <texteditor/fontsettings.h>
#include <texteditor/textdocument.h>
#include <texteditor/texteditor.h>
#include <utils/theme/theme.h>

#include <QScrollBar>
#include <QTimer>

namespace Minimap::Internal {

MinimapProxyStyleHelper::MinimapProxyStyleHelper(
    TextEditor::BaseTextEditor *editor) noexcept
    : QObject(editor->editorWidget()), m_theme(Utils::creatorTheme()),
      m_editor(editor->editorWidget()) {
    this->m_editor->installEventFilter(this);
    if (!this->m_editor->textDocument()->document()->isEmpty()) {
        this->init();
    } else {
        QObject::connect(this->m_editor->textDocument()->document(),
                         &QTextDocument::contentsChanged,
                         this,
                         &MinimapProxyStyleHelper::contentsChanged);
    }
}

MinimapProxyStyleHelper::~MinimapProxyStyleHelper() noexcept {
    this->m_editor->removeEventFilter(this);
}

bool MinimapProxyStyleHelper::eventFilter(QObject *watched, QEvent *event) {
    if (watched == this->m_editor && event->type() == QEvent::Resize) {
        this->deferedUpdate();
    }
    return false;
}

int MinimapProxyStyleHelper::width() const noexcept {
    int w =
        this->m_editor->extraArea() ? this->m_editor->extraArea()->width() : 0;
    return qMin(this->m_editor->width() - w,
                Settings::width() + Constants::MINIMAP_EXTRA_AREA_WIDTH);
}

const QRect &MinimapProxyStyleHelper::groove() const noexcept {
    return this->m_groove;
}

const QRect &MinimapProxyStyleHelper::addPage() const noexcept {
    return this->m_addPage;
}

const QRect &MinimapProxyStyleHelper::subPage() const noexcept {
    return this->m_subPage;
}

const QRect &MinimapProxyStyleHelper::slider() const noexcept {
    return this->m_slider;
}

int MinimapProxyStyleHelper::lineCount() const noexcept {
    return this->m_lineCount;
}

qreal MinimapProxyStyleHelper::factor() const noexcept {
    return this->m_factor;
}

const QColor &MinimapProxyStyleHelper::background() const noexcept {
    return this->m_backgroundColor;
}

const QColor &MinimapProxyStyleHelper::foreground() const noexcept {
    return this->m_foregroundColor;
}

const QColor &MinimapProxyStyleHelper::overlay() const noexcept {
    return this->m_overlayColor;
}

TextEditor::TextEditorWidget *MinimapProxyStyleHelper::editor() const
    noexcept {
    return this->m_editor;
}

void MinimapProxyStyleHelper::init() noexcept {
    QScrollBar *scrollbar = this->m_editor->verticalScrollBar();
    scrollbar->setProperty(Constants::MINIMAP_STYLE_OBJECT_PROPERTY,
                           QVariant::fromValue<QObject *>(this));
    QObject::connect(this->m_editor->textDocument(),
                     &TextEditor::TextDocument::fontSettingsChanged,
                     this,
                     &MinimapProxyStyleHelper::fontSettingsChanged);
    QObject::connect(this->m_editor->document()->documentLayout(),
                     &QAbstractTextDocumentLayout::documentSizeChanged,
                     this,
                     &MinimapProxyStyleHelper::deferedUpdate);
    QObject::connect(this->m_editor->document()->documentLayout(),
                     &QAbstractTextDocumentLayout::update,
                     this,
                     &MinimapProxyStyleHelper::deferedUpdate);
    QObject::connect(Settings::instance(),
                     &Settings::enabledChanged,
                     this,
                     &MinimapProxyStyleHelper::deferedUpdate);
    QObject::connect(Settings::instance(),
                     &Settings::widthChanged,
                     this,
                     &MinimapProxyStyleHelper::deferedUpdate);
    QObject::connect(Settings::instance(),
                     &Settings::lineCountThresholdChanged,
                     this,
                     &MinimapProxyStyleHelper::deferedUpdate);
    QObject::connect(Settings::instance(),
                     &Settings::alphaChanged,
                     this,
                     &MinimapProxyStyleHelper::fontSettingsChanged);
    QObject::connect(scrollbar,
                     &QAbstractSlider::valueChanged,
                     this,
                     &MinimapProxyStyleHelper::updateSubControlRects);
    this->fontSettingsChanged();
}

void MinimapProxyStyleHelper::contentsChanged() noexcept {
    QObject::disconnect(this->m_editor->textDocument()->document(),
                        &QTextDocument::contentsChanged,
                        this,
                        &MinimapProxyStyleHelper::contentsChanged);
    this->init();
}

void MinimapProxyStyleHelper::fontSettingsChanged() noexcept {
    const TextEditor::FontSettings &settings =
        this->m_editor->textDocument()->fontSettings();
    this->m_backgroundColor =
        settings.formatFor(TextEditor::C_TEXT).background();
    if (!this->m_backgroundColor.isValid()) {
        this->m_backgroundColor =
            this->m_theme->color(Utils::Theme::BackgroundColorNormal);
    }
    this->m_foregroundColor =
        settings.formatFor(TextEditor::C_TEXT).foreground();
    if (!this->m_foregroundColor.isValid()) {
        this->m_foregroundColor =
            this->m_theme->color(Utils::Theme::TextColorNormal);
    }
    if (this->m_backgroundColor.value() < 128) {
        this->m_overlayColor = QColor(Qt::white);
    } else {
        this->m_overlayColor = QColor(Qt::black);
    }
    this->m_overlayColor.setAlpha(Settings::alpha());
    this->deferedUpdate();
}

void MinimapProxyStyleHelper::deferedUpdate() noexcept {
    if (this->m_update) {
        return;
    }
    this->m_update = true;
    QTimer::singleShot(0, this, &MinimapProxyStyleHelper::update);
}

void MinimapProxyStyleHelper::update() noexcept {
    QScrollBar *scrollbar = this->m_editor->verticalScrollBar();
    QSizeF sz = this->m_editor->document()->size();
    this->m_lineCount = sz.toSize().height() + 1;
    int w = scrollbar->width();
    int h = scrollbar->height();
    this->m_factor = this->m_lineCount <= h
                         ? 1.0
                         : h / static_cast<qreal>(this->m_lineCount);
    int width = this->width();
    this->m_groove = QRect(width, 0, w - width, qMin(this->m_lineCount, h));
    this->updateSubControlRects();
    scrollbar->updateGeometry();
    this->m_update = false;
}

void MinimapProxyStyleHelper::updateSubControlRects() noexcept {
    QScrollBar *scrollbar = this->m_editor->verticalScrollBar();
    int viewPortLineCount =
        qRound(this->m_factor * (this->m_lineCount - scrollbar->maximum()));
    int w = scrollbar->width();
    int h = scrollbar->height();
    int value = scrollbar->value();
    int realValue = qRound(this->m_factor * value);
    int min = scrollbar->minimum();
    int max = scrollbar->maximum();
    this->m_addPage = value < max ? QRect(0,
                                          realValue + viewPortLineCount,
                                          w,
                                          h - viewPortLineCount - realValue)
                                  : QRect();
    this->m_subPage = value > min ? QRect(0, 0, w, realValue) : QRect();
    this->m_slider = QRect(0, realValue, w, viewPortLineCount);
    scrollbar->update();
}

} // namespace Minimap::Internal
