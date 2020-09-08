#include "optionspage.h"

#include "constants.h"
#include "settings.h"

#include <coreplugin/icore.h>
#include <texteditor/texteditorsettings.h>
#include <utils/settingsutils.h>

#include <QBoxLayout>
#include <QFormLayout>
#include <QGroupBox>

namespace Minimap::Internal {

OptionsPage::OptionsPage(QObject *parent, bool isTextWrappingEnabled) noexcept
    : Core::IOptionsPage(),
      m_textWrapping(isTextWrappingEnabled) {
    this->setId(Constants::MINIMAP_SETTINGS);
    this->setDisplayName(this->tr("Minimap"));
    QObject::connect(TextEditor::TextEditorSettings::instance(),
                     &TextEditor::TextEditorSettings::displaySettingsChanged,
                     this,
                     &OptionsPage::displaySettingsChanged);
}

QWidget *OptionsPage::widget() {
    if (!this->m_widget) {
        this->m_widget = new QWidget;
        QVBoxLayout *layout = new QVBoxLayout;
        QGroupBox *groupBox = new QGroupBox(this->m_widget);
        groupBox->setTitle(this->tr("Minimap"));
        layout->addWidget(groupBox);
        QFormLayout *form = new QFormLayout;
        this->m_enabled = new QCheckBox(groupBox);
        this->m_enabled->setToolTip(
            this->tr("Check to enable Minimap scrollbar"));
        this->m_enabled->setChecked(Settings::enabled());
        form->addRow(this->tr("Enabled:"), this->m_enabled);
        this->m_width = new QSpinBox;
        this->m_width->setMinimum(1);
        this->m_width->setMaximum(std::numeric_limits<int>::max());
        this->m_width->setToolTip(this->tr("The width of the Minimap"));
        this->m_width->setValue(Settings::width());
        form->addRow(this->tr("Width:"), this->m_width);
        this->m_lineCountThresHold = new QSpinBox;
        this->m_lineCountThresHold->setMinimum(1);
        this->m_lineCountThresHold->setMaximum(
            std::numeric_limits<int>::max());
        this->m_lineCountThresHold->setToolTip(
            this->tr("Line count threshold where no Minimap scrollbar is "
                     "to be used"));
        this->m_lineCountThresHold->setValue(Settings::lineCountThreshold());
        form->addRow(this->tr("Line Count Threshold:"),
                     this->m_lineCountThresHold);
        this->m_alpha = new QSpinBox;
        this->m_alpha->setMinimum(0);
        this->m_alpha->setMaximum(255);
        this->m_alpha->setToolTip(
            this->tr("The alpha value of the scrollbar slider"));
        this->m_alpha->setValue(Settings::alpha());
        form->addRow(this->tr("Scrollbar slider alpha value:"), this->m_alpha);
        groupBox->setLayout(form);
        this->m_widget->setLayout(layout);
        this->m_widget->setEnabled(!this->m_textWrapping);
        this->m_widget->setToolTip(
            this->m_textWrapping
                ? this->tr("Disable text wrapping to enable Minimap scrollbar")
                : QString());
    }
    return this->m_widget;
}

void OptionsPage::apply() {
    if (!this->m_widget) {
        return;
    }
    bool save = false;
    if (this->m_enabled->isChecked() != Settings::enabled()) {
        Settings::instance()->setEnabled(this->m_enabled->isChecked());
        save = true;
    }
    if (this->m_width->value() != Settings::width()) {
        Settings::instance()->setWidth(this->m_width->value());
        save = true;
    }
    if (this->m_lineCountThresHold->value() !=
        Settings::lineCountThreshold()) {
        Settings::instance()->setLineCountThreshold(
            this->m_lineCountThresHold->value());
        save = true;
    }
    if (this->m_alpha->value() != Settings::alpha()) {
        Settings::instance()->setAlpha(this->m_alpha->value());
        save = true;
    }
    QSettings *s = Core::ICore::settings();
    if (save) {
        Utils::toSettings(QLatin1String("Minimap"),
                          QLatin1String("text"),
                          s,
                          Settings::instance());
    }
}

void OptionsPage::finish() {
    delete this->m_widget;
}

void OptionsPage::displaySettingsChanged(
    const TextEditor::DisplaySettings &settings) noexcept {
    this->m_textWrapping = settings.m_textWrapping;
    if (this->m_widget) {
        this->m_widget->setEnabled(!this->m_textWrapping);
        this->m_widget->setToolTip(
            this->m_textWrapping
                ? this->tr("Disable text wrapping to enable Minimap scrollbar")
                : QString());
    }
}

} // namespace Minimap::Internal
