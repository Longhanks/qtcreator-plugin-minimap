#include "minimapsettings.h"

#include "minimapconstants.h"

#include <coreplugin/icore.h>
#include <extensionsystem/pluginmanager.h>
#include <texteditor/displaysettings.h>
#include <texteditor/texteditoroptionspage.h>
#include <texteditor/texteditorsettings.h>
#include <utils/qtcassert.h>
#include <utils/settingsutils.h>

#include <QCheckBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QPointer>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QWidget>

#include <limits>

namespace Minimap::Internal {

namespace {

const char minimapPostFix[] = "Minimap";
const char enabledKey[] = "Enabled";
const char widthKey[] = "Width";
const char lineCountThresholdKey[] = "LineCountThresHold";
const char alphaKey[] = "Alpha";

MinimapSettings *m_instance = nullptr;

} // namespace

class MinimapSettingsPage : public TextEditor::TextEditorOptionsPage {

public:
    MinimapSettingsPage(QObject *parent)
        : TextEditor::TextEditorOptionsPage(parent), m_widget(nullptr) {
        this->setId(Constants::MINIMAP_SETTINGS);
        this->setDisplayName(tr("Minimap"));
        connect(TextEditor::TextEditorSettings::instance(),
                &TextEditor::TextEditorSettings::displaySettingsChanged,
                this,
                &MinimapSettingsPage::displaySettingsChanged);
        this->m_textWrapping =
            TextEditor::TextEditorSettings::displaySettings().m_textWrapping;
    }

    ~MinimapSettingsPage() override = default;

    QWidget *widget() override {
        if (!this->m_widget) {
            this->m_widget = new QWidget;
            QVBoxLayout *layout = new QVBoxLayout;
            QGroupBox *groupBox = new QGroupBox(this->m_widget);
            groupBox->setTitle(tr("Minimap"));
            layout->addWidget(groupBox);
            QFormLayout *form = new QFormLayout;
            this->m_enabled = new QCheckBox(groupBox);
            this->m_enabled->setToolTip(
                tr("Check to enable Minimap scrollbar"));
            this->m_enabled->setChecked(m_instance->m_enabled);
            form->addRow(tr("Enabled:"), this->m_enabled);
            this->m_width = new QSpinBox;
            this->m_width->setMinimum(1);
            this->m_width->setMaximum(std::numeric_limits<int>::max());
            this->m_width->setToolTip(tr("The width of the Minimap"));
            this->m_width->setValue(m_instance->m_width);
            form->addRow(tr("Width:"), this->m_width);
            this->m_lineCountThresHold = new QSpinBox;
            this->m_lineCountThresHold->setMinimum(1);
            this->m_lineCountThresHold->setMaximum(
                std::numeric_limits<int>::max());
            this->m_lineCountThresHold->setToolTip(
                tr("Line count threshold where no Minimap scrollbar is to be "
                   "used"));
            this->m_lineCountThresHold->setValue(
                m_instance->m_lineCountThreshold);
            form->addRow(tr("Line Count Threshold:"),
                         this->m_lineCountThresHold);
            this->m_alpha = new QSpinBox;
            this->m_alpha->setMinimum(0);
            this->m_alpha->setMaximum(255);
            this->m_alpha->setToolTip(
                tr("The alpha value of the scrollbar slider"));
            this->m_alpha->setValue(m_instance->m_alpha);
            form->addRow(tr("Scrollbar slider alpha value:"), this->m_alpha);
            groupBox->setLayout(form);
            this->m_widget->setLayout(layout);
            this->m_widget->setEnabled(!this->m_textWrapping);
            this->m_widget->setToolTip(
                this->m_textWrapping
                    ? tr("Disable text wrapping to enable Minimap scrollbar")
                    : QString());
        }
        return this->m_widget;
    }

    void apply() override {
        if (!this->m_widget) {
            return;
        }
        bool save(false);
        if (this->m_enabled->isChecked() != MinimapSettings::enabled()) {
            m_instance->setEnabled(this->m_enabled->isChecked());
            save = true;
        }
        if (this->m_width->value() != MinimapSettings::width()) {
            m_instance->setWidth(this->m_width->value());
            save = true;
        }
        if (this->m_lineCountThresHold->value() !=
            MinimapSettings::lineCountThreshold()) {
            m_instance->setLineCountThreshold(
                this->m_lineCountThresHold->value());
            save = true;
        }
        if (this->m_alpha->value() != MinimapSettings::alpha()) {
            m_instance->setAlpha(this->m_alpha->value());
            save = true;
        }
        QSettings *s = Core::ICore::settings();
        if (save) {
            Utils::toSettings(
                minimapPostFix, QLatin1String("text"), s, m_instance);
        }
    }

    void finish() override {
        delete this->m_widget;
    }

private:
    void displaySettingsChanged(const TextEditor::DisplaySettings &settings) {
        this->m_textWrapping = settings.m_textWrapping;
        if (this->m_widget) {
            this->m_widget->setEnabled(!this->m_textWrapping);
            this->m_widget->setToolTip(
                this->m_textWrapping
                    ? tr("Disable text wrapping to enable Minimap scrollbar")
                    : QString());
        }
    }

    QPointer<QWidget> m_widget;
    QCheckBox *m_enabled;
    QSpinBox *m_width;
    QSpinBox *m_lineCountThresHold;
    QSpinBox *m_alpha;
    bool m_textWrapping;
};

MinimapSettings::MinimapSettings(QObject *parent)
    : QObject(parent), m_enabled(true),
      m_width(Constants::MINIMAP_WIDTH_DEFAULT),
      m_lineCountThreshold(Constants::MINIMAP_MAX_LINE_COUNT_DEFAULT),
      m_alpha(Constants::MINIMAP_ALPHA_DEFAULT) {
    QTC_ASSERT(!m_instance, return );
    m_instance = this;
    const QSettings *s = Core::ICore::settings();
    Utils::fromSettings(minimapPostFix, QLatin1String("text"), s, m_instance);
    this->m_settingsPage = new MinimapSettingsPage(this);
    ExtensionSystem::PluginManager::addObject(this->m_settingsPage);
}

MinimapSettings::~MinimapSettings() {
    ExtensionSystem::PluginManager::removeObject(this->m_settingsPage);
    m_instance = nullptr;
}

MinimapSettings *MinimapSettings::instance() {
    return m_instance;
}

void MinimapSettings::toMap(const QString &prefix, QVariantMap *map) const {
    map->insert(prefix + QLatin1String(enabledKey), this->m_enabled);
    map->insert(prefix + QLatin1String(widthKey), this->m_width);
    map->insert(prefix + QLatin1String(lineCountThresholdKey),
                this->m_lineCountThreshold);
    map->insert(prefix + QLatin1String(alphaKey), this->m_alpha);
}

void MinimapSettings::fromMap(const QString &prefix, const QVariantMap &map) {
    this->m_enabled =
        map.value(prefix + QLatin1String(enabledKey), this->m_enabled)
            .toBool();
    this->m_width =
        map.value(prefix + QLatin1String(widthKey), this->m_width).toInt();
    this->m_lineCountThreshold =
        map.value(prefix + QLatin1String(lineCountThresholdKey),
                  this->m_lineCountThreshold)
            .toInt();
    this->m_alpha =
        map.value(prefix + QLatin1String(alphaKey), this->m_alpha).toInt();
}

bool MinimapSettings::enabled() {
    return m_instance->m_enabled;
}

int MinimapSettings::width() {
    return m_instance->m_width;
}

int MinimapSettings::lineCountThreshold() {
    return m_instance->m_lineCountThreshold;
}

int MinimapSettings::alpha() {
    return m_instance->m_alpha;
}

void MinimapSettings::setEnabled(bool enabled) {
    if (this->m_enabled != enabled) {
        this->m_enabled = enabled;
        emit enabledChanged(enabled);
    }
}

void MinimapSettings::setWidth(int width) {
    if (this->m_width != width) {
        this->m_width = width;
        emit widthChanged(width);
    }
}

void MinimapSettings::setLineCountThreshold(int lineCountThreshold) {
    if (this->m_lineCountThreshold != lineCountThreshold) {
        this->m_lineCountThreshold = lineCountThreshold;
        emit lineCountThresholdChanged(lineCountThreshold);
    }
}

void MinimapSettings::setAlpha(int alpha) {
    if (this->m_alpha != alpha) {
        this->m_alpha = alpha;
        emit alphaChanged(alpha);
    }
}

} // namespace Minimap::Internal
